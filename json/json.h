#ifndef __JSON_H__
#define __JSON_H__
#include "xstring.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;

class JsonValue : public Value
{
public:
	bool				tobool(void);
	int					toint(void);
	unsigned int		touint(void);
	long long			toint64(void);
	unsigned long long  touint64(void);
	float				tofloat(void);
	double				todouble(void);
	xstring				tostring(void);
};

class Json
{
private:	
	Value nullvalue;
	Document doc;

public:
	Json(void);
	Json(const char* s);
	Json(const xstring& s);

public:
	bool Parse(const char* s);
	bool Parse(const xstring& s);
	Value& operator[](const char* s);
	Value& operator[](const xstring& s);
	Value& operator[](const unsigned int i);
};

#endif//__JSON_H__
