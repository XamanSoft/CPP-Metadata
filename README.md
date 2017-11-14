## CPP-Metadata

The aim of this project is to create a simple metadata system to serve as a bind to
other projects when a plugin system is needed.

**warning:** the C++ code of this project will be done in C++11.

## Usage example (test/test.cpp)

```cpp
#include <CppMetadata.hpp>
#include <iostream>

class Test: public CppMetadata::Object
{
	MD_OBJECT(Test);

public:
	MD_OBJECT_FUNCTION(char const*,name)
	MD_OBJECT_FUNCTION_BODY
	(
		return o_info.name;
	)
	
	MD_OBJECT_FUNCTION(int,index,int i)
	MD_OBJECT_FUNCTION_BODY
	(
		return i;
	)
	
	MD_OBJECT_FUNCTION(show,int const& i)
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
```

### ToDo
* Add more pointer checks
* Add Object members iterator
* Create documentation
