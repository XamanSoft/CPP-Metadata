#ifndef _CPPMETADATA_RUNTIME_MEMBER_HPP
#define _CPPMETADATA_RUNTIME_MEMBER_HPP

namespace CppMetadata {

namespace Runtime {

template <std::size_t... Indices>
struct indices {
	using next = indices<Indices..., sizeof...(Indices)>;
};
template <std::size_t N>
struct build_indices {
	using type = typename build_indices<N-1>::type::next;
};
template <>
struct build_indices<0> {
	using type = indices<>;
};
template <std::size_t N>
using BuildIndices = typename build_indices<N>::type;

template <class ObjTp>
void registerMember(ObjTp* object, char const* const name, CppMetadata::Member* member)
{
	object->members[name] = member;
}

#ifdef _MD_OBJECT_DECL

#define MD_OBJECT_PROPERTY(type, name) CppMetadata::Runtime::Property<object_self_t, type> name{this,#name};
#define MD_OBJECT_PROPERTY_GS(type, name, ...) CppMetadata::Runtime::Property<object_self_t, type> name{this,#name,__VA_ARGS__};

#define MD_OBJECT_FUNCTION_NAME(name) _md_object_function_##name

#define MD_OBJECT_FUNCTION(type, name, ...) \
	CppMetadata::Runtime::apply_args<CppMetadata::Runtime::Function,object_self_t, type, type(__VA_ARGS__)>::Type name{this,#name,&object_self_t::MD_OBJECT_FUNCTION_NAME(name)};\
	type MD_OBJECT_FUNCTION_NAME(name)(__VA_ARGS__)

#define MD_OBJECT_FUNCTION_NP(type, name) \
	CppMetadata::Runtime::Function<object_self_t, type> name{this,#name,&object_self_t::MD_OBJECT_FUNCTION_NAME(name)};\
	type MD_OBJECT_FUNCTION_NAME(name)()

#define MD_OBJECT_FUNCTION_NR(name, ...) \
	CppMetadata::Runtime::apply_args_nr<CppMetadata::Runtime::FunctionNR,object_self_t, void(__VA_ARGS__)>::Type name{this,#name,&object_self_t::MD_OBJECT_FUNCTION_NAME(name)};\
	void MD_OBJECT_FUNCTION_NAME(name)(__VA_ARGS__)

#define MD_OBJECT_FUNCTION_NR_NP(name) \
	CppMetadata::Runtime::FunctionNR<object_self_t, type> name{this,#name,&object_self_t::MD_OBJECT_FUNCTION_NAME(name)};\
	void MD_OBJECT_FUNCTION_NAME(name)()
	
#define MD_OBJECT_FUNCTION_BODY(body) { body; }

template<template<typename...> class C,typename A1, typename A2,typename T>
struct apply_args;

template<template<typename...> class C, typename Arg1, typename Arg2,typename R,typename... Args>
struct apply_args<C, Arg1, Arg2, R(Args...) >
{
    typedef C<Arg1, Arg2, Args...> Type;
};

template<template<typename...> class C,typename A1, typename T>
struct apply_args_nr;

template<template<typename...> class C, typename Arg1,typename R,typename... Args>
struct apply_args_nr<C, Arg1, R(Args...) >
{
    typedef C<Arg1, Args...> Type;
};

template <class ObjTp, typename ret_val, typename... params_type>
class Function: public CppMetadata::Member
{
public:
	typedef ret_val (ObjTp::*function_t)(params_type...);
private:
	struct unpack_caller
	{
	private:
		template <size_t... I>
		CppMetadata::Value* call(ObjTp* object, function_t f, CppMetadata::Arguments& args, indices<I...>) const
		{
			return new Runtime::Value<ret_val>((object->*f)(*args.get(I)...));
		}
		
		template <size_t... I>
		CppMetadata::Value* call(ObjTp * object, function_t f, CppMetadata::Arguments const& args, indices<I...>) const
		{
			return new Runtime::Value<ret_val>((object->*f)(*args.get(I)...));
		}

	public:
		CppMetadata::Value* operator () (ObjTp* object, function_t f, CppMetadata::Arguments& args)
		{
			//assert(args.size() == num_args); // just to be sure
			return call(object, f, args, BuildIndices<sizeof...(params_type)>{});
		}
		
		CppMetadata::Value* operator () (ObjTp* object, function_t f, CppMetadata::Arguments const& args) const
		{
			//assert(args.size() == num_args); // just to be sure
			return call(object, f, args, BuildIndices<sizeof...(params_type)>{});
		}
	};

	char const* const function_name;
	function_t function;
	ObjTp* object;
	
public:	
	Function(ObjTp* obj, char const* const name, function_t func): function_name(name), function(func), object(obj) { registerMember<ObjTp>(object,name,this); }
	
	char const* const name() const { return function_name; }
	CppMetadata::Value* action(CppMetadata::Value const& value) { CppMetadata::Arguments* args = value; return unpack_caller()(object, function, *args); }
	CppMetadata::Value* action(CppMetadata::Value const& value) const { CppMetadata::Arguments const* args = value; return unpack_caller()(object, function, *args); }

	ret_val operator()(params_type&&... params)
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments*> value_arg(&arguments);
		
		CppMetadata::Value* val = action(value_arg);
		ret_val res = *val;
		val->release();
		return res;
	}
	
	ret_val operator()(params_type&&... params) const
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments const*> value_arg(&arguments);
		
		CppMetadata::Value* val = action(value_arg);
		ret_val res = *val;
		val->release();
		return res;
	}
};

template <class ObjTp, typename... params_type>
class FunctionNR: public CppMetadata::Member
{
public:
	typedef void (ObjTp::*function_t)(params_type...);
private:
	struct unpack_caller
	{
	private:
		template <size_t... I>
		CppMetadata::Value* call(ObjTp* object, function_t f, CppMetadata::Arguments& args, indices<I...>) const
		{
			(object->*f)(*args.get(I)...);
			return new Runtime::Value<void>();
		}
		
		template <size_t... I>
		CppMetadata::Value* call(ObjTp * object, function_t f, CppMetadata::Arguments const& args, indices<I...>) const
		{
			(object->*f)(*args.get(I)...);
			return new Runtime::Value<void>();
		}

	public:
		CppMetadata::Value* operator () (ObjTp* object, function_t f, CppMetadata::Arguments& args)
		{
			//assert(args.size() == num_args); // just to be sure
			return call(object, f, args, BuildIndices<sizeof...(params_type)>{});
		}
		
		CppMetadata::Value* operator () (ObjTp* object, function_t f, CppMetadata::Arguments const& args) const
		{
			//assert(args.size() == num_args); // just to be sure
			return call(object, f, args, BuildIndices<sizeof...(params_type)>{});
		}
	};

	char const* const function_name;
	function_t function;
	ObjTp* object;
	
public:	
	FunctionNR(ObjTp* obj, char const* const name, function_t func): function_name(name), function(func), object(obj) { registerMember<ObjTp>(object,name,this); }
	
	char const* const name() const { return function_name; }
	CppMetadata::Value* action(CppMetadata::Value const& value) { CppMetadata::Arguments* args = value; return unpack_caller()(object, function, *args); }
	CppMetadata::Value* action(CppMetadata::Value const& value) const { CppMetadata::Arguments const* args = value; return unpack_caller()(object, function, *args); }
	
	void operator()(params_type&&... params)
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments*> value_arg(&arguments);
		
		action(value_arg)->release();
	}
	
	void operator()(params_type&&... params) const
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments const*> value_arg(&arguments);
		
		action(value_arg)->release();
	}
};

template <class ObjTp, typename Tp>
class Property: public CppMetadata::Member
{
private:
	char const* const property_name;
	Tp property_value{Tp()};
	ObjTp* object;	
public:
	typedef Tp (ObjTp::*getter_t)(int const&);
	typedef void (ObjTp::*setter_t)(Tp const&);
	
	getter_t getter{nullptr};
	setter_t setter{nullptr};
	
	Property(ObjTp* obj, char const* const name, getter_t get = nullptr, setter_t set = nullptr):
		property_name(name), object(obj), getter(get), setter(set) { registerMember<ObjTp>(object,name,this); }
	
	char const* const name() const { return property_name; }
    
	CppMetadata::Value* action(CppMetadata::Value const& value)
	{		
		if (value.type().isNotEqual(Runtime::Type<void>()) && setter != nullptr)
		{
			property_value = value;
			(object->*setter)(property_value);
		}
		
		Tp res = property_value;
		if (getter != nullptr)
			res = (object->*getter)(property_value);
		
		return new Runtime::Value<Tp>(res);
	}
	
	CppMetadata::Value* action(CppMetadata::Value const& value) const
	{
		Tp res = property_value;
		
		if (getter != nullptr)
			res = (object->*getter)(property_value);
		
		return new Runtime::Value<Tp>(res);
	}
	
	Tp operator=(Tp value)
	{	
		CppMetadata::Value* val = action(Runtime::Value<Tp>(value));
		Tp res = *val;
		val->release();
		return res;
	}
	
	operator Tp() const
	{
		CppMetadata::Value* val = action(Runtime::Value<void>());
		Tp res = *val;
		val->release();
		return res;
	}
};

#else

#define MD_OBJECT_PROPERTY(type, name) CppMetadata::Runtime::Property<type> name{this,#name};
#define MD_OBJECT_PROPERTY_GS(type, name, ...) CppMetadata::Runtime::Property<type> name{this,#name};

#define MD_OBJECT_FUNCTION(type, name, ...) \
	CppMetadata::Runtime::apply_args<CppMetadata::Runtime::Function, type, type(__VA_ARGS__)>::Type name{this,#name}

#define MD_OBJECT_FUNCTION_NP(type, name) \
	CppMetadata::Runtime::Function<type> name{this,#name}

#define MD_OBJECT_FUNCTION_NR(name, ...) \
	CppMetadata::Runtime::apply_args_nr<CppMetadata::Runtime::FunctionNR, void(__VA_ARGS__)>::Type name{this,#name}

#define MD_OBJECT_FUNCTION_NR_NP(name) \
	CppMetadata::Runtime::FunctionNR<type> name{this,#name}
	
#define MD_OBJECT_FUNCTION_BODY(body)

template<template<typename...> class C,typename A1, typename T>
struct apply_args;

template<template<typename...> class C, typename Arg1, typename R,typename... Args>
struct apply_args<C, Arg1, R(Args...) >
{
    typedef C<Arg1, Args...> Type;
};

template<template<typename...> class C, typename T>
struct apply_args_nr;

template<template<typename...> class C,typename R,typename... Args>
struct apply_args_nr<C, R(Args...) >
{
    typedef C<Args...> Type;
};

template <typename ret_val, typename... params_type>
class Function: public CppMetadata::Member
{
private:
	char const* const function_name;
	CppMetadata::Member* member;
	CppMetadata::Object* object;
	
public:	
	Function(CppMetadata::Object* obj, char const* const name): function_name(name), object(obj) { }
	
	char const* const name() const { return function_name; }
	CppMetadata::Value* action(CppMetadata::Value const& value) { return object->member(function_name).action(value); }
	CppMetadata::Value* action(CppMetadata::Value const& value) const { return object->member(function_name).action(value); }

	ret_val operator()(params_type&&... params)
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments*> value_arg(&arguments);
		
		CppMetadata::Value* val = action(value_arg);
		ret_val res = *val;
		val->release();
		return res;
	}
	
	ret_val operator()(params_type&&... params) const
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments const*> value_arg(&arguments);
		
		CppMetadata::Value* val = action(value_arg);
		ret_val res = *val;
		val->release();
		return res;
	}
};

template <typename... params_type>
class FunctionNR: public CppMetadata::Member
{
private:
	char const* const function_name;
	CppMetadata::Member* member;
	CppMetadata::Object* object;
	
public:	
	FunctionNR(CppMetadata::Object* obj, char const* const name): function_name(name), object(obj) { }
	
	char const* const name() const { return function_name; }
	CppMetadata::Value* action(CppMetadata::Value const& value) { return object->member(function_name).action(value); }
	CppMetadata::Value* action(CppMetadata::Value const& value) const { return object->member(function_name).action(value); }
	
	void operator()(params_type&&... params)
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments*> value_arg(&arguments);
		
		action(value_arg)->release();
	}
	
	void operator()(params_type&&... params) const
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments const*> value_arg(&arguments);
		
		action(value_arg)->release();
	}
};

template <typename Tp>
class Property: public CppMetadata::Member
{
private:
	char const* const property_name;
	CppMetadata::Object* obj;
public:	
	Property(CppMetadata::Object* obj, char const* const name):
		property_name(name), obj(obj) { }
	
	char const* const name() const { return property_name; }
    
	CppMetadata::Value* action(CppMetadata::Value const& value)
	{
		return obj->member(property_name).action(value);
	}
	
	CppMetadata::Value* action(CppMetadata::Value const& value) const
	{
		return obj->member(property_name).action(value);
	}
	
	Tp operator=(Tp value)
	{	
		CppMetadata::Value* val = action(Runtime::Value<Tp>(value));
		Tp res = *val;
		val->release();
		return res;
	}
	
	operator Tp() const
	{
		CppMetadata::Value* val = action(Runtime::Value<void>());
		Tp res = *val;
		val->release();
		return res;
	}
};

#endif
	
}

}

#endif
