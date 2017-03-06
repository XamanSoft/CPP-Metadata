#ifndef _CPPMETADATA_VM_HPP
#define _CPPMETADATA_VM_HPP

#ifdef _MD_VM_OBJECT_DECL
#include <CppMetadata/object_impl.hpp>
#endif

#ifdef _MD_OBJECT_DECL
#include <string>
typedef struct duk_hthread duk_context;
#endif

namespace CppMetadata {
	
#ifdef _MD_OBJECT_DECL
namespace Runtime {
#endif

class VM: public CppMetadata::Object
{
	MD_OBJECT(VM);
public:
	enum InputType { IT_CODE, IT_OBJECT, IT_FILE };
	
	MD_OBJECT_FUNCTION(CppMetadata::Value*, execute, char const* input, int type);
	MD_OBJECT_FUNCTION(CppMetadata::Value*, get, char const* name);
	MD_OBJECT_FUNCTION(int, put, char const* name, CppMetadata::Value* value);

	//MD_OBJECT_PROPERTY_GS(int,prop,&VM::prop_getter,&VM::prop_setter);
	
#ifdef _MD_OBJECT_DECL
	VM();
	virtual ~VM();

private:
//	int prop_getter(int const& value);
//	void prop_setter(int const& value);

	CppMetadata::Value* vm_get_value();
	std::string value_str;
	duk_context* vm_context;
#endif
};

#ifdef _MD_OBJECT_DECL
}
#include <CppMetadata/object_defs.hpp>
#endif
	
}

#endif
