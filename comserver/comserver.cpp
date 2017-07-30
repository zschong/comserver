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
		for(unsigned short i = 0; i < node.offset; i++)
		{
			int key = (node.slave << 16) + (node.offset + i);
			map<int,ValueNode>::iterator it = cache.find(key);

			if( it == cache.end() )
			{
				m[i] = 0;
			}
			else
			{
				m[i] = it->second.value;
			}
		}
		cachelock.Unlock();
		return (m.size() > 0);
	}
	return false;
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
		printf("%s,%d,%d,%d,%d\n",
		comconfig.name.data(),
		comconfig.baud,
		comconfig.parity,
		comconfig.bsize,
		comconfig.stop);
		comlock.Unlock();
	}
}
void ComServer::Request(ConfigNode& node)
{
	unsigned char slave = node.slave;
	unsigned char fcode = node.fcode;
	unsigned short offset = node.offset;
	unsigned short count  = node.count;

	switch(fcode)
	{
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	{
		Requestx01 request;
		Responsex01 respons;

		request.SetSlave(slave);
		request.SetFcode(fcode);
		request.SetOffset(offset);
		request.SetCount(count);
		request.SetCRC(request.CalcCRC());
		for(int i = 0; i < request.CRCLen()+2; i++)
		{
			printf("%02X ", request.data[i]);
		}
		printf(":check %s\n", request.Check() ? "ok" : "no");
		Response(request, respons);
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
			Requestx03  *request = (Requestx03*)&req;
			Responsex03 *respons = (Responsex03*)&res;
			unsigned char  slave = respons->GetSlave();
			unsigned short offset= request->GetOffset();
			unsigned short count = request->GetCount();
			unsigned char bcount = respons->GetBcount()/2;
			if( cachelock.Lock() )
			{
				for(int i = 0; i < bcount; i++)
				{
					int key = (slave << 16) | (offset+=2);
					ValueNode &vnode = cache[key];
					vnode.slave = slave;
					vnode.offset = offset;
					vnode.value = respons->GetValue(i);
				}
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
