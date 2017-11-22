#ifndef _CPPMETADATA_VALUE_HPP
#define _CPPMETADATA_VALUE_HPP

namespace CppMetadata {
	
class Value
{
public:
	enum RoleType { FUNCTION = -1, NONE =0, VALUE = 1, MULTIVALUE };
	
	virtual char const* const name() const =0;
	virtual CppMetadata::Type const& type() const =0;
	virtual int role() const =0;
	virtual int count() const =0;
	
	virtual Value* clone() const =0;
	
	virtual Value& at(int index) =0;
	virtual Value const& at(int index) const =0;

	virtual Value* action(Value const& value) =0;
	virtual Value const* action(Value const& value) const =0;

	virtual void release() const =0;

	virtual ~Value() { }

	template <typename Tp>
	Tp const& operator[](int index) const;

	template <typename Tp>
	Tp& operator[](int index);

	template <typename Tp>
	Tp const& operator=(Tp const& val);

	template <typename Tp>
	operator Tp() const;
};

template <typename... params_type>
class MultiValue: public Value
{
	template<const int N, typename T, typename... types>
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
	template<const int N>
	using Type = typename get_Nth_type<N, params_type...>::type;
	
	virtual Value& operator[](int index) =0;
	virtual Value const& operator()(int index) const =0;
	
	virtual MultiValue<params_type...>& operator()(params_type... params) =0;
	virtual MultiValue<params_type...> const& operator()(params_type... params) const =0;
	
	virtual ~MultiValue(){}
};

template <typename Tp>
class MultiValue<Tp>: public Value
{
public:
	virtual MultiValue<Tp>& operator()(Tp param) =0;
	virtual MultiValue<Tp> const& operator()(Tp param) const =0;

	virtual Tp const& operator=(Tp const& val) =0;
	virtual operator Tp() const =0;
	
	virtual ~MultiValue(){}
};

template <>
class MultiValue<void>: public Value
{
public:
	virtual ~MultiValue(){}
};

// Deduce Value from args to remove const and references
template <typename... Types>
Value* newValue(Types... args);

}

#endif
