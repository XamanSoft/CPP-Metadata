#ifndef _CPPMETADATA_OBJECT_HPP
#define _CPPMETADATA_OBJECT_HPP

namespace CppMetadata {
	
#include <map>

#define MD_OBJECT(o_name) \
    private:\
    ObjectInfo o_info{ #o_name, nullptr, this }; \
    std::map<const char*,Member*> members;\
    \
    public: \
    const ObjectInfo& objectInfo() const { return o_info; } \
    int memberCount() const { return members.size(); };\
    bool hasMember(const char* m_name) const { return members.count(m_name)>0; };\
    Member& member(const char* m_name) const { return *members.at(m_name); };\
    Member& operator[] (const char* m_name) const { return *members.at(m_name); };\
    virtual void release() { delete this; }\
    private:
	
class Object
{
public:
    struct ObjectInfo
    {
        const char* name;
        const Type* type;
        const Object* object;
    };
    
    virtual const ObjectInfo& objectInfo() const =0;
    virtual int memberCount() const =0;
    virtual bool hasMember(const char* m_name) const =0;
    virtual Member& member(const char* m_name) const =0;
    
    virtual void release() =0;
};
	
}

#endif
