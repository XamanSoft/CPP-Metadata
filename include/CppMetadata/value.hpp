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

template <typename... params_type>
class MultiValue: public Value
{
	template<std::size_t N, typename T, typename... types>
	struct get_Nth_type
	{
		using type = typename get_Nth_type<N - 1, types...>::type;
	};

	template<typename T, typename... types>
	struct get_Nth_type<0, T, types...>
	{
		using type = T;
	};
	
public:
	template<std::size_t N>
	using Type = typename get_Nth_type<N, params_type...>::type;

	virtual Type<0> const& act() const =0;
	virtual Type<0> const& act(Type<0> const& value) =0;

	virtual Type<0> const& operator=(Type<0> const& val) =0;
	virtual operator Type<0>() const =0;
	
	virtual ~MultiValue(){}
};
	
}

#endif
