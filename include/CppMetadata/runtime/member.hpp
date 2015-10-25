#ifndef _CPPMETADATA_MEMBER_HPP
#define _CPPMETADATA_MEMBER_HPP

namespace CppMetadata {
	
#define MD_OBJECT_MEMBER(type, name)

class Constant;
class Variable;

class Member
{
public:
    enum Type {
        MT_CONSTANT,
        MT_VARIABLE
    };
    
    virtual enum Type type() const =0;
    
    virtual Constant& toConstant() const =0;
    virtual Variable& toVariable() const =0;
};

namespace Runtime {
	
}

}

#endif
