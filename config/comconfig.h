#ifndef __COMCONFIG_H__
#define __COMCONFIG_H__
#include <map>
#include "entity.h"

class ComConfig
{
public:
	map<xstring,ComNode>  configs;
	map<xstring,xstring>  devnodes;

public:
	ComConfig(void)
	{
		devnodes["com1"] = "/dev/ttyS0";
		devnodes["COM1"] = "/dev/ttyS0";
		devnodes["com2"] = "/dev/ttyS1";
		devnodes["COM2"] = "/dev/ttyS1";
		devnodes["com3"] = "/dev/ttyS2";
		devnodes["COM3"] = "/dev/ttyS2";
		devnodes["com4"] = "/dev/ttyS3";
		devnodes["COM4"] = "/dev/ttyS3";
		devnodes["com5"] = "/dev/ttyS4";
		devnodes["COM5"] = "/dev/ttyS4";
		devnodes["com6"] = "/dev/ttyS5";
		devnodes["COM6"] = "/dev/ttyS5";
		devnodes["com7"] = "/dev/ttyS6";
		devnodes["COM7"] = "/dev/ttyS6";
		devnodes["com8"] = "/dev/ttyS7";
		devnodes["COM8"] = "/dev/ttyS7";
	}
public:
	void GetConfig(const xstring&, ComNode&);
	bool ReadConfig(const xstring& file);
	void GetLine(const char *line);
	void Xchange(FieldNode& node);
	void Show(void);
};

#endif//__COMCONFIG_H__
