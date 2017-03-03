#include <CppMetadata.hpp>
#include <iostream>

#ifdef _MD_OBJECT_DECL
namespace CppMetadata {
namespace Runtime {
#endif

class Test: public CppMetadata::Object
{
	MD_OBJECT(Test);

public:
	MD_OBJECT_FUNCTION_NP(char const*,name);	
	MD_OBJECT_FUNCTION(int,index,int const& i);
	MD_OBJECT_FUNCTION_NR(show,int const& i);
	MD_OBJECT_PROPERTY_GS(int,prop,&Test::prop_getter,&Test::prop_setter);
	
#ifdef _MD_OBJECT_DECL
private:
	int prop_getter(int const& value);
	void prop_setter(int const& value);
#endif
};

#ifdef _MD_OBJECT_DECL
}}
#endif