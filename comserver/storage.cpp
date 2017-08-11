#include "storage.h"

Storage::Storage(void)
{
	laststore = time(0);
	lastremove = 0;
	storetimeout = 5;//*60;
	removetimeout = 24 * 60 * 60;
	storedir = "/root/history/";
	prefix = "comx";
}

bool Storage::StoreTimeout(void)
{
	time_t now = time(0);

	if( abs(now - laststore) > storetimeout )
	{
		laststore = now;
		return true;
	}
	return false;
}
bool Storage::RemoveTimeout(void)
{
	time_t now = time(0);
	if( abs(now - lastremove) > removetimeout )
	{
		lastremove = now;
		return true;
	}
	return false;
}
void Storage::SetName(const string& name)
{
	prefix = name;
	laststore = time(0);
}
void Storage::Store(map<int,ValueNode>& m)
{
	if( StoreTimeout() == false )
	{
		Remove();
		return;
	}
	time_t now = time(0)+(8*60*60);
	struct tm *tp = gmtime(&now);
	xstring filename;
	if( tp )
	{
		filename.format("%s%s-%d-%d-%d.txt", 
				storedir.data(),
				prefix.data(), 
				tp->tm_year+1900, 
				tp->tm_mon+1, 
				tp->tm_mday);
	}
	else
	{
		filename.format("%s%s-history.txt", storedir.data(), prefix.data());
	}
	if( filename.empty() )
	{
		return;
	}
	FILE *fp = fopen(filename.data(), "a");
	if( NULL == fp )
	{
		return;
	}
	xstring datetime;
	datetime.format("%d-%d-%d %d:%d:%d\n",
					tp->tm_year+1900,
					tp->tm_mon+1,
					tp->tm_mday,
					tp->tm_hour,
					tp->tm_min,
					tp->tm_sec);
	fwrite(datetime.data(), datetime.length(), 1, fp);

	map<u8,map<u8,map<u16,u16> > > mmm;
	for(map<int,ValueNode>::iterator i = m.begin(); i != m.end(); i++)
	{
		mmm[i->second.slave][i->second.fcode][i->second.offset] = i->second.value;
	}
	Store(fp, mmm);
	fwrite("\n",1,1,fp);
	fclose(fp);
}
void Storage::Store(FILE *fp, map<u8,map<u8,map<u16,u16> > >& mmm)
{
		map<u16,u16>::iterator C;
		map<u8,map<u16,u16> >::iterator B;
		map<u8,map<u8,map<u16,u16> > >::iterator A;

		for(A = mmm.begin(); A != mmm.end(); A++)
		{
			for(B = A->second.begin(); B != A->second.end(); B++)
			{
				list<u16> vlist;
				unsigned char offset = 0;
				unsigned char begin = 0;
				for(C = B->second.begin(); C != B->second.end(); C++)
				{
					if( vlist.begin() == vlist.end() )
					{
						begin = C->first;
						offset = begin;
						vlist.push_back(C->second);
						continue;
					}
					if( offset+1 == C->first )
					{
						offset = C->first;
						vlist.push_back(C->second);
						continue;
					}
					Store(fp, A->first, B->first, begin, vlist);
					vlist.clear();
					if( C != B->second.end() )
					{
						offset = C->first;
						begin = offset;
						vlist.push_back(C->second);
					}
				}//for(C)
				if( vlist.begin() != vlist.end() )
				{
					Store(fp, A->first, B->first, begin, vlist);
				}
			}//for(B)
		}//for(A)
}
void Storage::Store(FILE *fp, u8 slave, u8 fcode, u16 offset, list<u16> &l)
{
	xstring data;

	data.format("\t%d:%d:%d[", slave, fcode, offset);
	for(list<u16>::iterator i = l.begin(); i != l.end(); i++)
	{
		xstring tmp;
		tmp.format("%d,", *i);
		data += tmp;
	}
	data[data.length()-1] = ']';
	data += "\n";
	fwrite(data.data(), data.length(), 1, fp);
}
void Storage::Remove(void)
{
	time_t now = 0;
	struct tm *t = NULL;

	if( RemoveTimeout() == false )
	{
		return;
	}
	now = time(0) + (8*60*60) - (60*24*60*60);

	for(int i = 0; i < 10; i++)
	{
		now -= (24 * 60 * 60);
		if( t = gmtime(&now) )
		{
			xstring filename;
			filename.format("%s%s-%d-%d-%d.txt",
					storedir.data(),
					prefix.data(),
					t->tm_year+1900,
					t->tm_mon+1,
					t->tm_mday);
			if( filename.empty() == false )
			{
				//printf("remove(%s)\n", filename.data());
				remove(filename.data());
			}
		}
	}//for(
}//
