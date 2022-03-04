#pragma once
#include <cstdlib>
#include <tuple>
#include <string>
#include <stdexcept>
#include <unordered_map>

class Reflection;

class Object
{
public:
	Object(){}

	Object(const std::string &n,size_t s,const std::string &c,const std::string &d)
		:name(n),size(s),constructor(c),destructor(d)
	{
		address=std::calloc(1,size);
	}

	Object(const Object &object):
		name(object.name),size(object.size),constructor(object.constructor),destructor(object.destructor)
	{
		if(object.address)
		{
			address=std::calloc(1,size);
			std::memmove(address,object.address,size);
		}
	}

	Object(Object &&object)
		:name(std::move(object.name)),size(object.size),address(object.address),
		constructor(std::move(object.constructor)),destructor(std::move(object.destructor))
	{
		object.address=nullptr;
	}

	Object& operator=(const Object &object)
	{
		std::free(address);
		name=object.name;
		size=object.size;
		constructor=object.constructor;
		destructor=object.destructor;
		if(object.address)
		{
			address=std::calloc(1,size);
			std::memmove(address,object.address,size);
		}
		return *this;
	}

	Object& operator=(Object &&object)
	{
		std::free(address);
		name=std::move(object.name);
		size=object.size;
		constructor=std::move(object.constructor);
		destructor=std::move(object.destructor);
		address=object.address;
		object.address=nullptr;
		return *this;
	}

	~Object();

	std::string GetClassName()
	{
		return name;
	}

private:
	friend class Reflection;
	void *address=nullptr;
	std::string name;
	std::size_t size;
	std::string constructor,destructor;
};

class Reflection
{
public:
	static Reflection* GetInstance()
	{
		if(instance)
			return instance;
		return new Reflection();
	}

	~Reflection(){ delete instance; }

	using addrfun=void* (*)(const char*);
	using strfun=const char* (*)(const char*);

	template<class Class>
	void RegisterClass(const std::string &ClassName,addrfun MemberAddress,strfun TypeCheck)
	{
		if(MemberAddress&&TypeCheck)
			map[ClassName]={MemberAddress,TypeCheck,sizeof(Class)};
		else
			throw std::runtime_error("Please register a valid function.");
	}

	template<class... Arguments>
	Object NewObject(const std::string &ClassName,const std::string &Destructor="",const std::string &Constructor="",Arguments... arguments)
	{
		try
		{
			Object object(ClassName,std::get<2>(map.at(ClassName)),Constructor,Destructor);
			if(Constructor!="")
				CallMemberFunction<void>(object,Constructor,arguments...);
			return object;
		}
		catch(std::out_of_range &e)
		{
			e=std::out_of_range("Do not found this class.");
			throw;
		}
	}

	template<class ReturnType>
	ReturnType GetMemberVariable(const Object &object,const std::string &MemberName)
	{
		addrfun MemberAddress=nullptr;
		strfun TypeCheck=nullptr;
		try
		{
			auto tuple=map.at(object.name);
			MemberAddress=std::get<0>(tuple);
			TypeCheck=std::get<1>(tuple);
		}
		catch(std::out_of_range &e)
		{
			e=std::out_of_range("Do not found this class.");
			throw;
		}
		size_t offset=(size_t)MemberAddress(MemberName.c_str());
		if(offset>=object.size||std::string(TypeCheck(MemberName.c_str()))!=typeid(ReturnType).name())
			throw std::runtime_error("Variable types do not match.");
		char *address=(char*)object.address;
		address+=offset;
		return *(ReturnType*)address;
	}

	template<class First,class... Next>
	std::string strcat(First f,Next... n)
	{
		return std::string(typeid(f).name())+strcat(n...);
	}

	auto strcat()
	{
		return "";
	}

	template<class ReturnType,class... Arguments>
	ReturnType CallMemberFunction(const Object &object,const std::string &MemberName,Arguments... arguments)
	{
		addrfun MemberAddress;
		strfun TypeCheck;
		try
		{
			auto tuple=map.at(object.name);
			MemberAddress=std::get<0>(tuple);
			TypeCheck=std::get<1>(tuple);
		}
		catch(std::out_of_range &e)
		{
			e=std::out_of_range("Do not found this class.");
			throw;
		}
		auto address=MemberAddress(MemberName.c_str());
		if(address==nullptr||address<(void*)object.size)
			throw std::runtime_error("Do not found this member function or this is a member variable.");
		if(std::string(typeid(ReturnType).name())+strcat(arguments...)!=TypeCheck(MemberName.c_str()))
			throw std::runtime_error("ReturnType or Arguments do not match.");
		using funtype=ReturnType(*)(void*,Arguments...);
		return ((funtype)address)(object.address,arguments...);
	}
private:
	Reflection(){ instance=this; }
	static Reflection *instance;
	std::unordered_map<std::string,std::tuple<addrfun,strfun,size_t>> map;
};