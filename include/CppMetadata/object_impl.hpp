#ifndef _MD_OBJECT_DECL
#define _MD_OBJECT_DECL
#endif

#ifdef MD_OBJECT
#undef MD_OBJECT
#undef MD_OBJECT_PROPERTY
#undef MD_OBJECT_FUNCTION
#undef MD_OBJECT_FUNCTION_BODY
#endif

#include <map>
#include <string>

#define MD_OBJECT(o_name) \
    private:\
    CppMetadata::Object::ObjectInfo o_info{ #o_name, nullptr, this }; \
    std::map<std::string,CppMetadata::Value*> members;\
    typedef o_name object_self_t;\
	template <class ObjTp> friend void CppMetadata::Runtime::registerMember(ObjTp*, char const* const, CppMetadata::Value*);\
    public: \
    const CppMetadata::Object::ObjectInfo& objectInfo() const { return o_info; } \
    int memberCount() const { return members.size(); };\
    bool hasMember(const char* m_name) const { return members.count(m_name)>0; };\
    CppMetadata::Value& member(const char* m_name) const { return *members.at(m_name); };\
    CppMetadata::Value& operator[] (const char* m_name) const { return *members.at(m_name); };\
    virtual void release() { delete this; }\
    private:
	
#define MD_OBJECT_PROPERTY(type, name, ...) CppMetadata::Runtime::Impl::Property<object_self_t, type> name{this,#name,__VA_ARGS__};

#ifndef MD_OBJECT_FUNCTION_NAME
#define MD_OBJECT_FUNCTION_NAME(name) _md_object_function_##name
#endif

#define MD_OBJECT_FUNCTION(type, name, ...) \
	CppMetadata::Runtime::Impl::apply_args<CppMetadata::Runtime::Impl::Function,object_self_t, type, type(__VA_ARGS__)>::Type name{this,#name,&object_self_t::MD_OBJECT_FUNCTION_NAME(name)};\
	type MD_OBJECT_FUNCTION_NAME(name)(__VA_ARGS__)
	
#define MD_OBJECT_FUNCTION_BODY(body) { body; }
