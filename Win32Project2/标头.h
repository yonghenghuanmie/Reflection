#pragma once
#include "../Win32Project1/Macro.h"

declaration void* addrfunB(const char *);
declaration const char* strfunB(const char*);

class B
{
public:
	int test()
	{
		return 2;
	}

	void initial()
	{
		str="hello,world.";
	}

	char *str;
};