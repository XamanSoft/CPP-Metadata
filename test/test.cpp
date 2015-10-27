#include <CppMetadata.hpp>
#include <iostream>

class Test: public CppMetadata::Object
{
	MD_OBJECT(Test);

public:
	MD_OBJECT_FUNCTION_NP(char const*,name)
	MD_OBJECT_FUNCTION_BODY
	(
		return o_info.name;
	)
	
	MD_OBJECT_FUNCTION(int,index,int i)
	MD_OBJECT_FUNCTION_BODY
	(
		return i;
	)
	
	MD_OBJECT_FUNCTION_NR(show,int const& i)
	MD_OBJECT_FUNCTION_BODY
	(
		std::cout << "show " << i << std::endl;
	)
	
	MD_OBJECT_PROPERTY_GS(int,prop,&Test::prop_getter,&Test::prop_setter);
	
	int prop_getter(int const& value)
	{
		return value*2;
	}
	
	void prop_setter(int const& value)
	{
		show(value);
	}
};

int main()
{
	Test test;
	
	test.prop = 1;
	test.show(test.prop);
	test.prop = 3;
	test.show(test.prop);
	
	return 0;
}
