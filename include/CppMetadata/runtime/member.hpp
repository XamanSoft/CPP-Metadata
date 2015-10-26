#ifndef _CPPMETADATA_RUNTIME_MEMBER_HPP
#define _CPPMETADATA_RUNTIME_MEMBER_HPP

#include <iostream>

namespace CppMetadata {

#define MD_OBJECT_PROPERTY(type, name)

#define MD_OBJECT_FUNCTION_NAME(name) _md_object_function_##name

#define MD_OBJECT_FUNCTION(type, name, params...) \
	CppMetadata::Runtime::Function<object_self_t, type, params> name{this,#name,&object_self_t::MD_OBJECT_FUNCTION_NAME(name)};\
	type MD_OBJECT_FUNCTION_NAME(name)(params)

#define MD_OBJECT_FUNCTION_NP(type, name) \
	CppMetadata::Runtime::Function<object_self_t, type> name{this,#name,&object_self_t::MD_OBJECT_FUNCTION_NAME(name)};\
	type MD_OBJECT_FUNCTION_NAME(name)()

#define MD_OBJECT_FUNCTION_NR(name, params...) \
	CppMetadata::Runtime::FunctionNR<object_self_t, type, params> name{this,#name,&object_self_t::MD_OBJECT_FUNCTION_NAME(name)};\
	void MD_OBJECT_FUNCTION_NAME(name)(params...)

#define MD_OBJECT_FUNCTION_NR_NP(name) \
	CppMetadata::Runtime::FunctionNR<object_self_t, type> name{this,#name,&object_self_t::MD_OBJECT_FUNCTION_NAME(name)};\
	void MD_OBJECT_FUNCTION_NAME(name)()

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

template <class ObjTp, typename ret_val, typename... params_type>
class FunctionBase: public CppMetadata::Member
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
	FunctionBase(ObjTp* obj, char const* const name, function_t func): function_name(name), function(func), object(obj) {}
	
	char const* const name() const { return function_name; }
	CppMetadata::Value* action(CppMetadata::Value const& value) { CppMetadata::Arguments* args = value; return unpack_caller()(object, function, *args); }
	CppMetadata::Value* action(CppMetadata::Value const& value) const { CppMetadata::Arguments const* args = value; return unpack_caller()(object, function, *args); }
};

template <class ObjTp, typename ret_val, typename... params_type>
class Function: public FunctionBase<ObjTp,ret_val,params_type...>
{
public:
	Function(ObjTp* obj, char const* const name, typename FunctionBase<ObjTp,ret_val,params_type...>::function_t func): FunctionBase<ObjTp,ret_val,params_type...>(obj, name, func) {}

	ret_val operator()(params_type&&... params)
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments*> value_arg(&arguments);
		
		CppMetadata::Value* val = FunctionBase<ObjTp,ret_val,params_type...>::action(value_arg);
		ret_val res = *val;
		val->release();
		return res;
	}
	
	ret_val operator()(params_type&&... params) const
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments const*> value_arg(&arguments);
		
		CppMetadata::Value* val = FunctionBase<ObjTp,ret_val,params_type...>::action(value_arg);
		ret_val res = *val;
		val->release(); // fix-it to const
		return res;
	}
};

template <class ObjTp, typename... params_type>
class FunctionNR: public FunctionBase<ObjTp,void,params_type...>
{
public:
	FunctionNR(ObjTp* obj, char const* const name, typename FunctionBase<ObjTp,void,params_type...>::function_t func): FunctionBase<ObjTp,void,params_type...>(obj, name, func) {}
	
	void operator()(params_type&&... params)
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments*> value_arg(&arguments);
		
		FunctionBase<ObjTp,void,params_type...>::action(value_arg)->release();
	}
	
	void operator()(params_type&&... params) const
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments const*> value_arg(&arguments);
		
		FunctionBase<ObjTp,void,params_type...>::action(value_arg)->release();
	}
};

/*
template <typename Tp>
class Property: public CppMetadata::Member
{
public:
	char const* const name() const =0;
    
	Value* action(Value const& value) =0;
	Value* action(Value const& value) const =0;
};*/
	
}

}

#endif
