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
	ComConfig(void);
public:
	void InitNodes(int n);
	void GetConfig(const xstring&, ComNode&);
	bool ReadConfig(const xstring& file);
	void GetLine(const char *line);
	void Xchange(FieldNode& node);
	void Show(void);
};

#endif//__COMCONFIG_H__
