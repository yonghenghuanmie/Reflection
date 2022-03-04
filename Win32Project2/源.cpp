#include <cstddef>
#include <string>
#define Export
#include "БъЭЗ.h"

//UB in C++
union TypeCast
{
	void *address;
	decltype(&B::test) test;
	decltype(&B::initial) initial;
}typecast;

void* addrfunB(const char *s)
{
	std::string str=s;
	if(str=="test")
		typecast.test=&B::test;
	else if(str=="initial")
		typecast.initial=&B::initial;
	else if(str=="str")
		return offsetof(B,str);
	else
		return nullptr;
	return typecast.address;
}

const char* strfunB(const char *s)
{
	std::string str=s;
	if(str=="test")
		return "int";
	else if(str=="initial")
		return "void";
	else if(str=="str")
		return typeid(B::str).name();
	else
		return "";
}