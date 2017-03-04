#ifndef _CPPMETADATA_RUNTIME_OBJECT_HPP
#define _CPPMETADATA_RUNTIME_OBJECT_HPP

#define MD_OBJECT_CONCAT(X,Y) X##Y  // helper macro
#define MD_OBJECT_UNIQUE(X,Y) MD_OBJECT_CONCAT(X,Y)

#define MD_OBJECT_DECLARE(object) namespace CppMetadata { namespace Runtime {	template <> inline CppMetadata::object& retriveRuntimeType<object>() { return CppMetadata::Runtime::retriveType(#object); } } }

namespace CppMetadata {
namespace Runtime {	
	typedef CppMetadata::Object* (*object_create_t)(CppMetadata::Arguments const& args);

	extern "C" CppMetadata::Object* retriveObject(char const* name, CppMetadata::Arguments const& args);
	extern "C" int registerObject(char const* name, object_create_t oc); // int = object-id
	
	template<typename obj>
	struct RegisterObject { RegisterObject(const char* name) { registerObject(name, obj::create); }  };
	
	template <typename Tp>
	inline CppMetadata::Object& retriveRuntimeObject()
	{
		throw std::invalid_argument("Use MD_OBJECT_DECLARE macro to declare a object!");
	}
	
}
}

#endif
