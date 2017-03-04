#define _TEST_DECL
#include "test.hpp"

using namespace CppMetadata::Runtime;

MD_OBJECT_REGISTER(Test);

char const* Test::MD_OBJECT_FUNCTION_NAME(name)() {
	return o_info.name;
}

int Test::MD_OBJECT_FUNCTION_NAME(index)(int const& i) {
	return i;
}
	
int Test::prop_getter(int const& value)
{
	return value*2;
}

void Test::prop_setter(int const& value)
{
	show(value);
}

void Test::MD_OBJECT_FUNCTION_NAME(show)(int const& i) {
	std::cout << "show " << i << std::endl;
}

