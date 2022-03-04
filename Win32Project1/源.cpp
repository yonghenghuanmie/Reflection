#include <cstddef>
#include <unordered_map>
#include <string>
#include <iostream>
#include <windows.h>
#define Export
#include "БъЭЗ.h"

//UB in C++
union TypeCast
{
	void *address;
	decltype(&A::test) test;
	decltype(&A::initial) initial;
	decltype(&A::dispose) dispose;
}typecast;

std::unordered_map<std::string,void*> address_map;
std::unordered_map<std::string,const char*> string_map;

BOOL WINAPI DllMain
(
	_In_ HINSTANCE hinstDLL,
	_In_ DWORD     fdwReason,
	_In_ LPVOID    lpvReserved
)
{
	if(fdwReason==DLL_PROCESS_ATTACH)
	{
		typecast.test=&A::test;
		address_map.emplace("test",typecast.address);
		typecast.initial=&A::initial;
		address_map.emplace("initial",typecast.address);
		typecast.dispose=&A::dispose;
		address_map.emplace("dispose",typecast.address);
		address_map.emplace("value",(void*)offsetof(A,value));

		string_map.emplace("test","int");
		string_map.emplace("initial","void");
		string_map.emplace("dispose","void");
		string_map.emplace("value","int");
	}
	return TRUE;
}

void* addrfunA(const char *s)
{
	auto iterator=address_map.find(s);
	if(iterator!=address_map.end())
		return iterator->second;
	return nullptr;
}

const char* strfunA(const char *s)
{
	auto iterator=string_map.find(s);
	if(iterator!=string_map.end())
		return iterator->second;
	return "";
}