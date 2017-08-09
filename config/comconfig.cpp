#include "comconfig.h"
#include "config-file.h"
#include "config-field.h"

ComConfig::ComConfig(void)
{
	{
		InitNodes(16);
	}
}
void ComConfig::InitNodes(int n)
{
	for(int i = 0; i < n; i++)
	{
		xstring comx, ttyx;
		comx.format("com%d", i+1);
		ttyx.format("/dev/ttyS%d", i);
		devnodes[comx] = ttyx;
	}
}

void ComConfig::GetConfig(const xstring& com, ComNode& node)
{
	map<xstring,ComNode>::iterator i = configs.find(com);

	if( i != configs.end() )
	{
		node = i->second;
	}
}
bool ComConfig::ReadConfig(const xstring& file)
{
	FILE *fp = NULL;

	if( file.empty() )
	{
		return false;
	}
	configs.clear();
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

void ComConfig::GetLine(const char *line)
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

	Xchange(node);
}
void ComConfig::Xchange(FieldNode& node)
{
	xstring name = node.field1;

	if( devnodes.find(name) == devnodes.end() )
	{
		return;
	}
	configs[name].name		= devnodes[name];
	configs[name].baud		= node.field2.toint();;
	configs[name].parity	= node.field3.toint();;
	configs[name].bsize	= node.field4.toint();;
	configs[name].stop		= node.field5.toint();;
}
void ComConfig::Show(void)
{
	for(map<xstring,ComNode>::iterator i = configs.begin(); i != configs.end(); i++)
	{
		printf("%s=>[", i->first.data());
		printf("%s, ", i->second.name.data());
		printf("%d, ", i->second.baud);
		printf("%d, ", i->second.parity);
		printf("%d, ", i->second.bsize);
		printf("%d]\n", i->second.stop);
	}
}
