#ifndef _CPPMETADATA_VALUE_HPP
#define _CPPMETADATA_VALUE_HPP

namespace CppMetadata {

class Value
{
public:
    virtual Type const& type() const =0;
	
	enum RoleType { NONE =0, VALUE, FUNCTION };
	
	virtual char const* const name() const =0;
	virtual int role() const =0;

	virtual Value* action(Value const& value) =0;
	virtual Value const* action(Value const& value) const =0;
	
	virtual void release() const =0;
	
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
	virtual Tp const& act(Tp const& value = Tp()) =0;
	virtual Tp const& act(Tp const& value = Tp()) const =0;
	
	virtual Tp const& operator=(Tp const& val) =0;
	virtual operator Tp() const =0;
	
	virtual ~MultiValue(){}
};
	
}

#endif
