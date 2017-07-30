#ifndef __VARCONFIG_H__
#define __VARCONFIG_H__
#include <map>
#include "entity.h"
#include "xstring.h"


class VarConfig
{
public:
	map<xstring, map<int,ConfigNode> > table;

public:
	bool GetConfig(xstring, map<int,ConfigNode>&);

public:
	bool ReadConfig(const xstring& file);
	void GetLine(const char *line);
	void Xchange(FieldNode&);
	void Show(void);
	void ShowConfigNode(map<int,ConfigNode>&);
};

#endif//__VARCONFIG_H__
