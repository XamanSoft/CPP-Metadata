#include <CppMetadata.hpp>
#include <atomic>
#include <mutex>
#include <map>

using namespace CppMetadata;

std::mutex register_mutex;
std::atomic<int> type_id(0);

struct TypeRegister
{
	int id;
	Type* type;
	
	TypeRegister(): id(0), type(nullptr){}
	TypeRegister(int id, Type*type): id(id), type(type) {}
};

std::map<char const*, TypeRegister> type_map;

extern "C" int registerType(char const* name, Type& type)
{
	register_mutex.lock();
	int id = ++type_id;
	type_map[name] = TypeRegister(id, &type);
	register_mutex.unlock();
	return id;
}

extern "C" Type& retriveType(char const* name)
{
	register_mutex.lock();
	Type* ret = type_map[name].type;
	register_mutex.unlock();
	return *ret;
}

MD_REGISTER_TYPE(bool);

MD_REGISTER_TYPE(signed char);
MD_REGISTER_TYPE(char);
MD_REGISTER_TYPE(char const *);
MD_REGISTER_TYPE(unsigned char);
MD_REGISTER_TYPE(wchar_t);
MD_REGISTER_TYPE(char16_t);
MD_REGISTER_TYPE(char32_t);

MD_REGISTER_TYPE(short int);
MD_REGISTER_TYPE(unsigned short int);
MD_REGISTER_TYPE(int);
MD_REGISTER_TYPE(unsigned int);
MD_REGISTER_TYPE(long int);
MD_REGISTER_TYPE(unsigned long int);
MD_REGISTER_TYPE(long long int);
MD_REGISTER_TYPE(unsigned long long int);

MD_REGISTER_TYPE(float);
MD_REGISTER_TYPE(double);
MD_REGISTER_TYPE(long double);

#include <iostream>

int main()
{
	Runtime::ArgumentsBuild arguments(10, 20, 0.5, "OK");
	Runtime::Arguments args(arguments);
	
	std::cout << args.count() << std::endl;
	
	Runtime::Arguments::reverse_iterator it = args.rbegin();
	for (; it != args.rend(); it++)
		std::cout << (*it)->type().name() << std::endl;
	
	/*for (Value* value : args)
	{
		//std::cout << value << std::endl;
		std::cout << value->type().name() << std::endl;
	}*/
	
	return 0;
}
