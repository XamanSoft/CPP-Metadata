#ifndef _CPPMETADATA_TYPE_HPP
#define _CPPMETADATA_TYPE_HPP

namespace CppMetadata {
	
class Value;
class Arguments;

class Type
{
public:
    virtual int id() const =0;
    virtual char const* const name() const =0;
    virtual int size() const =0;
};

namespace Runtime {
	typedef Value* (*ValueConstructor)();
		
	Value* createValue(char const* name);
	Value* createValue(char const*name, const Arguments& args);
		
	int registerType(char const* name, ValueConstructor f_constructor); // int = type-id
}

#define MD_REGISTER_TYPE(type)
	
}

#endif
