#ifndef _CPPMETADATA_RUNTIME_OBJECT_HPP
#define _CPPMETADATA_RUNTIME_OBJECT_HPP

#define MD_OBJECT_CONCAT(X,Y) X##Y  // helper macro
#define MD_OBJECT_UNIQUE(X,Y) MD_OBJECT_CONCAT(X,Y)

namespace CppMetadata {

class Object;

namespace Runtime {

class ObjectRegisterType
{
public:
	virtual CppMetadata::Object* create(CppMetadata::Value const& args) =0;
};

extern "C" int registerObject(char const* name, ObjectRegisterType* object_reg); // int = object-id
extern "C" CppMetadata::Object* createObject(char const* name, CppMetadata::Value const& arg);

template <typename ObjectTp>
class ObjectRegisterNoArgs: public ObjectRegisterType
{
public:
	ObjectRegisterNoArgs(char const* object_name)
	{
		registerObject(object_name, this);
	}
	
	CppMetadata::Object* create(CppMetadata::Value const& args){ return new ObjectTp(); } 
};

template <typename ObjectTp>
class ObjectRegisterArgs: public ObjectRegisterType
{
public:
	ObjectRegisterArgs(char const* object_name)
	{
		registerObject(object_name, this);
	}
	
	CppMetadata::Object* create(CppMetadata::Value const& args){ return new ObjectTp(args); } 
};


}
}

#define MD_OBJECT_REGISTER(object) CppMetadata::Object* _md_object_new_##object(CppMetadata::Value const& args){static CppMetadata::Runtime::ObjectRegisterNoArgs<object> object_register(#object); return object_register.create(args); }
#define MD_OBJECT_REGISTER_ARGS(object) CppMetadata::Object* _md_object_new_##object(CppMetadata::Value const& args){static CppMetadata::Runtime::ObjectRegisterArgs<object> object_register(#object); return object_register.create(args); }

#endif
