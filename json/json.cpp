#include "json.h"


Json::Json(void)
{
}
Json::Json(const char* s)
{
	Parse(s);
}
Json::Json(const xstring& s)
{
	Parse(s);
}

bool Json::Parse(const char* s)
{
	if( NULL == s )
	{
		return false;
	}
	doc.Parse(s);
	return (doc.HasParseError() == false);
}
bool Json::Parse(const xstring& s)
{
	if( s.empty() )
	{
		return false;
	}
	return Parse( (const char*)s.data() );
}

Value& Json::operator[](const char* s)
{
	if( NULL == s )
	{
		return nullvalue;
	}
	if( doc.HasMember(s) )
	{
		return doc[s];
	}
	return nullvalue;
}

Value& Json::operator[](const xstring& s)
{
	if( s.empty() )
	{
		return nullvalue;
	}
	return (*this)[s.data()];
}
Value& Json::operator[](const unsigned int i)
{
	if(doc.MemberCount() < i )
	{
		return nullvalue;
	}
	return doc[i];
}

#define JSON_TEST
#ifdef JSON_TEST
int main(void)
{
	Json json;
	StringBuffer s;
	Writer<StringBuffer> writer(s);

	writer.StartObject();
	writer.Key("xx");
	writer.String("yy");
	{
		writer.Key("values");
		writer.StartArray();
		for(int i = 0; i < 10; i++)
		{
			writer.Int(i);
		}
		writer.EndArray();
	}
	writer.EndObject();

	printf("%s\n", s.GetString());
	json.Parse(s.GetString());	
	printf("v.size=%d\n", json["values"].Size());

	return 0;
}
#endif
