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

MD_REGISTER_TYPE(void);
MD_REGISTER_TYPE(void*);

MD_REGISTER_TYPE(bool);

MD_REGISTER_TYPE(signed char);
MD_REGISTER_TYPE(char);
MD_REGISTER_TYPE(unsigned char);
MD_REGISTER_TYPE(wchar_t);
MD_REGISTER_TYPE(char16_t);
MD_REGISTER_TYPE(char32_t);

MD_REGISTER_TYPE(char const *);
MD_REGISTER_TYPE(wchar_t const *);
MD_REGISTER_TYPE(char16_t const *);
MD_REGISTER_TYPE(char32_t  const *);

MD_REGISTER_TYPE(char *);
MD_REGISTER_TYPE(wchar_t *);
MD_REGISTER_TYPE(char16_t *);
MD_REGISTER_TYPE(char32_t  *);

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

MD_REGISTER_TYPE(CppMetadata::Arguments*);
MD_REGISTER_TYPE(CppMetadata::Arguments const*);

#include <iostream>

class Test: public Object
{
	MD_OBJECT(Test);

public:
	MD_OBJECT_FUNCTION_NP(char const*,name);
};

char const* Test::MD_OBJECT_FUNCTION_NAME(name)()
{
	return o_info.name;
}

int main()
{
	Test test;
	
	std::cout << "Classe " << test.name() << std::endl;
	
	return 0;
}
