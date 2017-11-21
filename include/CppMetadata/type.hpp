#ifndef _CPPMETADATA_TYPE_HPP
#define _CPPMETADATA_TYPE_HPP

namespace CppMetadata {
	
class Value;

class Type
{
public:
    virtual int id() const =0;
    virtual char const* const name() const =0;
    virtual int size() const =0;
	
	virtual bool isEqual(Type const& type) const =0;
	virtual bool isNotEqual(Type const& type) const =0;
	
	virtual Value* createValue() =0;
	virtual Value* createValue(Value const& args) =0;
	
	virtual ~Type(){}
};
	
}

#endif
