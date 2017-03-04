#ifndef _CPPMETADATA_MEMBER_HPP
#define _CPPMETADATA_MEMBER_HPP

namespace CppMetadata {
	
class Value;
class Arguments;

class Member
{
public:
	enum MemberType { NONE =0, FUNCTION, PROPERTY };
	
	virtual char const* const name() const =0;
	virtual int type() const =0;
    
	virtual Value* action(Value const& value) =0;
	virtual Value* action(Value const& value) const =0;
};

}

#endif
