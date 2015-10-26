#ifndef _CPPMETADATA_RUNTIME_OBJECT_HPP
#define _CPPMETADATA_RUNTIME_OBJECT_HPP

#include <map>

#define MD_OBJECT(o_name) \
    private:\
    CppMetadata::Object::ObjectInfo o_info{ #o_name, nullptr, this }; \
    std::map<const char*,CppMetadata::Member*> members;\
    typedef o_name object_self_t;\
    public: \
    const CppMetadata::Object::ObjectInfo& objectInfo() const { return o_info; } \
    int memberCount() const { return members.size(); };\
    bool hasMember(const char* m_name) const { return members.count(m_name)>0; };\
    CppMetadata::Member& member(const char* m_name) const { return *members.at(m_name); };\
    CppMetadata::Member& operator[] (const char* m_name) const { return *members.at(m_name); };\
    virtual void release() { delete this; }\
    private:

#endif
