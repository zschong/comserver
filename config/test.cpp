#include "varconfig.h"
#include "comconfig.h"

int main(void)
{
	VarConfig varcfg;
	ComConfig comcfg;
	map<int, ConfigNode> m;

	varcfg.ReadConfig("varconfig.txt");
	varcfg.Show();
	comcfg.ReadConfig("comconfig.txt");
	comcfg.Show();
	varcfg.GetConfig("com2", m);
	varcfg.ShowConfigNode(m);

	return 0;
}
