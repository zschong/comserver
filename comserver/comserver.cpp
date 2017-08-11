#include "showhex.h"
#include "comserver.h"


bool ComServer::SetCom(ComNode& com)
{
	if( comlock.Lock() )
	{
		comconfig = com;
		comchange = true;
		comlock.Unlock();
		return true;
	}
	return false;
}
bool ComServer::SetName(const xstring& comname)
{
	if( Lock() )
	{
		name = comname;
		storage.SetName(name);
		Unlock();
		return true;
	}
	return false;
}
bool ComServer::SetConfig(map<int, ConfigNode>& m)
{
	if( configlockA.Lock() )
	{
		configA.clear();
		configA = m;
		configchange = true;
		configlockA.Unlock();
		return true;
	}
	return false;
}
bool ComServer::GetValue(const ConfigNode& node, map<int,unsigned short>& m)
{
	if( cachelock.Lock() )
	{
		m.clear();
		for(unsigned short i = 0; i < node.count; i++)
		{
			int key = (node.slave << 24) | (node.fcode << 16) | (node.offset + i);
			map<int,ValueNode>::iterator it = cache.find(key);

			if( cache.end() == it )
			{
				break;//unlock
			}
			if( abs(time(0) - it->second.lastvalue) > 4 )
			{
				break;//unlock
			}
			m[i] = it->second.value;
		}
		cachelock.Unlock();
		return (m.size() == node.count);
	}
	return false;
}
bool ComServer::GetJson(xstring& s)
{
	bool change = false;

	s.clear();

	if( cachelock.Lock() )
	{
		s = "{\"valus\",[";
		for(map<int,ValueNode>::iterator i = cache.begin(); i != cache.end(); i++)
		{
			if( abs(time(0) - i->second.lastvalue ) >= 5 )
			{
				continue;
			}
			xstring kv;
			s += "{";
			kv.format("\"%s\":%d,", "slave", i->second.slave);
			s += kv;
			kv.format("\"%s\":%d,", "fcode", i->second.fcode);
			s += kv;
			kv.format("\"%s\":%d,", "offset", i->second.offset);
			s += kv;
			kv.format("\"%s\":%d", "value", i->second.value);
			s += kv;
			s += "},";
			change = true;
		}
		if( change )
		{
			s[s.length()-1] = ']';
		}
		else
		{
			s += "]";
		}
		s += "}";
		cachelock.Unlock();
	}
	return (s.empty() == false);
}
void ComServer::CheckConfig(void)
{
	if( !configchange )
	{
		return;
	}
	if( configlockA.Lock() )
	{
		configB.clear();
		configB = configA;
		configchange = false;
		configlockA.Unlock();
	}
}
void ComServer::CheckCom(void)
{
	if( !comchange )
	{
		return;
	}
	if( comlock.Lock() )
	{
		if( reader.Open(comconfig.name) )
		{
			if( reader.Set(comconfig.baud,
						comconfig.parity,
						comconfig.bsize,
						comconfig.stop) 
			&& reader.SetMode(Mode_485) )
			{
				printf("%s(%d,%d,%d,%d)\n",
				comconfig.name.data(),
				comconfig.baud,
				comconfig.parity,
				comconfig.bsize,
				comconfig.stop
				);
				comchange = false;
			}
		}
		comlock.Unlock();
	}
}
void ComServer::Request(ConfigNode& node)
{
	switch(node.fcode)
	{
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	{
		Requestx03 request;

		//make request frame
		request.SetSlave( node.slave );
		request.SetFcode( node.fcode );
		request.SetOffset( node.offset );
		request.SetCount( node.count );
		request.SetCRC( request.CalcCRC() );

		//clear recv buffer
		reader.ReadData();
		reader.ReadData();
		reader.Cleanup();

		//send frame
		reader.Send(request.data, request.CRCLen()+2);

		//recieve response data in timeout seconds
		for(int i = 0; i < node.timeout; i++)
		{
			unsigned char *p = reader.ReadData();
			Responsex03 *res = (Responsex03*)p;
			if( res->Check() )
			{
				Response(request, *res);
				break;
			}
			usleep(1000);
		}
	}break;
	case 0x05:
	case 0x06:
	case 0x0f:
	case 0x10:
		break;
	}

	usleep(100*1000);
}
void ComServer::Response(ModbusBase &req, ModbusBase &res)
{
	switch(res.GetFcode())
	{
	case 0x01:
	case 0x02:
		{
			Requestx01  *request = (Requestx01*)&req;
			Responsex01 *respons = (Responsex01*)&res;
			unsigned short offset= request->GetOffset();
			unsigned short count = request->GetCount();
			unsigned char bcount = respons->GetBcount();

			for(int i = 0; i < bcount; i++)
			{
				//todo
			}
		}break;
	case 0x03:
	case 0x04:
		{
			Requestx03  &request = *(Requestx03*)&req;
			Responsex03 &respons = *(Responsex03*)&res;
			unsigned short offset= request.GetOffset();
			unsigned short count = request.GetCount();
			unsigned char  slave = respons.GetSlave();
			unsigned char  fcode = respons.GetFcode();
			unsigned char bcount = respons.GetBcount()/2;
			//showhex(res.data, respons.CRCLen()+2);
			if( cachelock.Lock() )
			{
				for(int i = 0; i < bcount; i++)
				{
					int key = (slave << 24) | (fcode << 16) | (offset+i);
					ValueNode &vnode = cache[key];
					vnode.slave = slave;
					vnode.fcode = fcode;
					vnode.offset = offset+i;
					vnode.value = respons.GetValue(i);
					vnode.lastvalue = time(0);
				}
				//showhex(request.data, request.CRCLen()+2);
				//showhex(respons.data, respons.CRCLen()+2);
				//storage history values
				storage.Store(cache);
				cachelock.Unlock();
			}
		}break;
	case 0x05:
	case 0x06:
		{
			Requestx05 *request = (Requestx05*)&req;
			Responsex05 *respons = (Responsex05*)&res;
		}break;
	case 0x0f:
	case 0x10:
		{
			Requestx0f *request = (Requestx0f*)&req;
			Responsex0f *respons = (Responsex0f*)&res;
		}break;
	}
}
void ComServer::Loop(void)
{
	CheckCom();
	CheckConfig();
	for(B = configB.begin(); B != configB.end(); B++)
	{
		Request(B->second);
	}
	usleep(100*1000);
}
