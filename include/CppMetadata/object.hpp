#ifndef _CPPMETADATA_OBJECT_HPP
#define _CPPMETADATA_OBJECT_HPP

namespace CppMetadata {

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
    virtual Value& member(const char* m_name) const =0;
    
    virtual void release() =0;
};
	
}

#endif
