#ifndef _CPPMETADATA_VM_HPP
#define _CPPMETADATA_VM_HPP

namespace CppMetadata {

#ifdef _MD_OBJECT_DECL
namespace Runtime {
#endif

class VM: public CppMetadata::Object
{
	MD_OBJECT(VM);
public:
	enum InputType { IT_CODE, IT_FILE };
	
    MD_OBJECT_FUNCTION(char const*, name);	
	MD_OBJECT_FUNCTION(int, execute, char const* input, int type = VM::IT_CODE);

	//MD_OBJECT_PROPERTY_GS(int,prop,&VM::prop_getter,&VM::prop_setter);
	
#ifdef _MD_OBJECT_DECL
private:
//	int prop_getter(int const& value);
//	void prop_setter(int const& value);
#endif
};

#ifdef _MD_OBJECT_DECL
}
#endif
	
}

#endif
