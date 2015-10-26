#ifndef _CPPMETADATA_VALUE_HPP
#define _CPPMETADATA_VALUE_HPP

namespace CppMetadata {

class Value
{
public:
    virtual Type const& type() const =0;
    
    virtual Value const& getValue() const =0;
    virtual void setValue(Value const& val) =0;
	
	virtual void release() =0;
	
	virtual ~Value(){}
	
	template <typename Tp>
	Tp const& operator=(Tp const& val);
	
	template <typename Tp>
	operator Tp() const;
};

template <typename Tp>
class MultiValue: public Value
{
public:
	virtual Tp const& get() const =0;
    virtual void set(Tp const& val) =0;
	
	virtual Tp const& operator=(Tp const& val) =0;
	virtual operator Tp() const =0;
	
	virtual ~MultiValue(){}
};
	
}

#endif
