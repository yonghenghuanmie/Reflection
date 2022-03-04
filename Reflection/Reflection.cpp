#include <iostream>
#include <windows.h>
#include "Reflection.h"
#include "../Win32Project1/标头.h"
#include "../Win32Project2/标头.h"
#pragma comment(lib,"Win32Project2.lib")

inline Object::~Object()
{
	if(destructor!="")
		Reflection::GetInstance()->CallMemberFunction<void>(*this,destructor);
	std::free(address);
	address=nullptr;
}

Reflection *Reflection::instance;

int main()
{
	//HINSTANCE hInstanceA=LoadLibraryA("Win32Project1.dll"),hInstanceB=LoadLibraryA("Win32Project2.dll");
	//Reflection *reflection=Reflection::GetInstance();
	//reflection->RegisterClass<A>("A",(Reflection::addrfun)GetProcAddress(hInstanceA,"addrfunA"),
	//	(Reflection::strfun)GetProcAddress(hInstanceA,"strfunA"));
	//reflection->RegisterClass<B>("B",(Reflection::addrfun)GetProcAddress(hInstanceB,"addrfunB"),
	//	(Reflection::strfun)GetProcAddress(hInstanceB,"strfunB"));
	//Object objectB=reflection->NewObject("B");
	////std::cout<<reflection->GetMemberVariable<char*>(object,"str")<<std::endl;
	//std::cout<<reflection->CallMemberFunction<int>(objectB,"test")<<std::endl;
	//reflection->CallMemberFunction<void>(objectB,"initial");
	//std::cout<<reflection->GetMemberVariable<char*>(objectB,"str")<<std::endl;
	//Object objectA=reflection->NewObject("A","dispose","initial");
	//std::cout<<reflection->GetMemberVariable<int>(objectA,"value")<<std::endl;
	//std::cout<<reflection->CallMemberFunction<int>(objectA,"test")<<std::endl;

	HINSTANCE hInstance=nullptr;
	Reflection *reflection=Reflection::GetInstance();
	Object object;
	std::string str;
	while(std::cin>>str)
	{
		if(str=="quit")
			break;
		if(hInstance)
			FreeLibrary(hInstance);
		if(hInstance=LoadLibraryA(str.c_str()))
		{
			if(str=="Win32Project1.dll")
			{
				reflection->RegisterClass<A>("A",(Reflection::addrfun)GetProcAddress(hInstance,"addrfunA"),
					(Reflection::strfun)GetProcAddress(hInstance,"strfunA"));
				object=reflection->NewObject("A","dispose","initial");
			}
			else
			{
				reflection->RegisterClass<B>("B",(Reflection::addrfun)GetProcAddress(hInstance,"addrfunB"),
					(Reflection::strfun)GetProcAddress(hInstance,"strfunB"));
				object=reflection->NewObject("B");
			}
			std::cout<<reflection->CallMemberFunction<int>(object,"test")<<std::endl;
		}
	}
	return 0;
}