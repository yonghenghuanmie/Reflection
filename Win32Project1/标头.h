#pragma once
#include "Macro.h"

declaration void* addrfunA(const char *);
declaration const char* strfunA(const char*);

class A
{
public:
	void initial()
	{
		value=3;
		std::cout<<"Construct"<<std::endl;
	}
	int test()
	{
		return 1;
	}

	void dispose()
	{
		std::cout<<"Destruct"<<std::endl;
	}
	int value;
};