#ifndef _CPPMETADATA_RUNTIME_OBJECT_HPP
#define _CPPMETADATA_RUNTIME_OBJECT_HPP

#include <map>

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

#ifdef _MD_OBJECT_DECL

#define MD_OBJECT_REGDEC(object) static CppMetadata::Runtime::RegisterObject<object> MD_OBJECT_UNIQUE(register_type_,object)(#object)
#define MD_OBJECT_REGISTER_ARGS(object) MD_OBJECT_REGDEC(object); CppMetadata::Object* object::create(CppMetadata::Arguments const& args) { return new object(args); }
#define MD_OBJECT_REGISTER(object) MD_OBJECT_REGDEC(object); CppMetadata::Object* object::create(CppMetadata::Arguments const& args) { return new object(); }

#define MD_OBJECT(o_name) \
    private:\
	static CppMetadata::Object* create(CppMetadata::Arguments const& args);\
	friend class CppMetadata::Runtime::RegisterObject<o_name>;\
    CppMetadata::Object::ObjectInfo o_info{ #o_name, nullptr, this }; \
    std::map<const char*,CppMetadata::Member*> members;\
    typedef o_name object_self_t;\
	template <class ObjTp> friend void CppMetadata::Runtime::registerMember(ObjTp*, char const* const, CppMetadata::Member*);\
    public: \
    const CppMetadata::Object::ObjectInfo& objectInfo() const { return o_info; } \
    int memberCount() const { return members.size(); };\
    bool hasMember(const char* m_name) const { return members.count(m_name)>0; };\
    CppMetadata::Member& member(const char* m_name) const { return *members.at(m_name); };\
    CppMetadata::Member& operator[] (const char* m_name) const { return *members.at(m_name); };\
    virtual void release() { delete this; }\
    private:
#else
#define MD_OBJECT(o_name) \
	private: \
	typedef o_name object_self_t;\
	CppMetadata::Object* object{nullptr}; \
	public: \
	template <typename... params_type>\
	inline o_name(params_type&&... params): object(retriveObject(#o_name, CppMetadata::Runtime::ArgumentsBuild(params...))) {} \
    virtual const CppMetadata::Object::ObjectInfo& objectInfo() const { return object->objectInfo(); } \
    virtual int memberCount() const { if (object) return object->memberCount(); return 0; };\
    virtual bool hasMember(const char* m_name) const { if (object) return object->hasMember(m_name); return false; };\
    virtual CppMetadata::Member& member(const char* m_name) const { return object->member(m_name); };\
    virtual CppMetadata::Member& operator[] (const char* m_name) const { return object->member(m_name); };\
    virtual ~o_name() { if (object) object->release(); } \
	virtual void release() { if (object) object->release(); } \
    private:

#endif

#endif
