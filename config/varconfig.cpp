#include "varconfig.h"
#include "config-file.h"
#include "config-field.h"

bool VarConfig::GetConfig(xstring com, map<int,ConfigNode>& m)
{
	if( table.find(com) == table.end() )
	{
		return false;
	}
	m.clear();
	m = table[com];
	return true;
}
bool VarConfig::ReadConfig(const xstring& file)
{
	FILE *fp = NULL;

	if( file.empty() )
	{
		return false;
	}
	table.clear();
	fp = config_open(file.data());
	for(int i = 0; fp && i < 0xffff; i++)
	{
		char buf[32] = {0};
		if( config_readline(fp, buf, sizeof(buf)) < 1 )
		{
			break;
		}
		GetLine(buf);
	}
	return 0;
}
void VarConfig::GetLine(const char *line)
{
	char buf[32] = {0};
	FieldNode node;

	#define set_node_filed(s, n, b, l)\
	do{\
		if( get_fieldn(s, n, b, l) > 0 )\
		{\
			node.field##n.format("%s", b);\
		}\
	}while(0)	

	set_node_filed(line, 1, buf, sizeof(buf));
	set_node_filed(line, 2, buf, sizeof(buf));
	set_node_filed(line, 3, buf, sizeof(buf));
	set_node_filed(line, 4, buf, sizeof(buf));
	set_node_filed(line, 5, buf, sizeof(buf));
	set_node_filed(line, 6, buf, sizeof(buf));
	set_node_filed(line, 7, buf, sizeof(buf));
	set_node_filed(line, 8, buf, sizeof(buf));
	set_node_filed(line, 9, buf, sizeof(buf));

	Xchange(node);
}
void VarConfig::Xchange(FieldNode& node)
{
	xstring keyA = node.field1;
	int keyB = (node.field2.toint() << 16) | node.field4.toint();

	if( keyA.empty() == false )
	{
		ConfigNode& v = table[keyA][keyB];
		v.slave  = node.field2.toint();
		v.fcode  = node.field3.toint();
		v.offset = node.field4.toint();
		v.count  = node.field5.toint();
	}
}
void VarConfig::Show(void)
{
	for(map<xstring,map<int,ConfigNode> >::iterator i = table.begin(); i != table.end(); i++)
	{
		for(map<int,ConfigNode>::iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			printf("%s=>[", i->first.data());
			printf("%d, ", j->second.slave);
			printf("%d, ", j->second.fcode);
			printf("%d, ", j->second.offset);
			printf("%d]\n", j->second.count);
		}
	}
}
void VarConfig::ShowConfigNode(map<int,ConfigNode>& m)
{
	for(map<int,ConfigNode>::iterator i = m.begin(); i != m.end(); i++)
	{
		printf("%d=>[", i->first);
		printf(" %d,", i->second.slave);
		printf(" %d,", i->second.fcode);
		printf(" %d,", i->second.offset);
		printf(" %d]\n", i->second.count);
	}

}
