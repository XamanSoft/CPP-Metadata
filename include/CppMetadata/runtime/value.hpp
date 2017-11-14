#ifndef _CPPMETADATA_RUNTIME_VALUE_HPP
#define _CPPMETADATA_RUNTIME_VALUE_HPP

namespace CppMetadata {
	
class Type;
class Arguments;

namespace Runtime {
	
	template <typename Tp>
	inline Tp valueConstructor(CppMetadata::Arguments const& args)
	{
		return Tp();
	}
	
	template <typename Tp>
	class Value: public CppMetadata::MultiValue<Tp>
	{
		Tp value{Tp()};
		CppMetadata::Type const& value_type;

	public:
		Value(): value_type(retriveRuntimeType<Tp>()) {}
		Value(Tp const& val): value(val), value_type(retriveRuntimeType<Tp>()) {}
		Value(CppMetadata::Value const& val): value_type(val.type()) { action(val); }
		Value(CppMetadata::Value const* val): value_type(val->type()) { action(*val); }
		Value(CppMetadata::Type const& v_type): value_type(v_type) {}
		Value(CppMetadata::Type const& v_type, CppMetadata::Arguments const& args): value(valueConstructor<Tp>(args)), value_type(v_type) {}

		virtual ~Value(){ }

		char const* const name() const { return nullptr; }
		int role() const { return VALUE; }
		
		CppMetadata::Type const& type() const { return value_type; }
    
		CppMetadata::Value const* action(CppMetadata::Value const& val) const { return this; }
		CppMetadata::Value* action(CppMetadata::Value const& val) { if (value_type.isEqual(val.type())) value = static_cast<CppMetadata::Runtime::Value<Tp> const &>(val).value; return this; }
		
		void release() const { ::delete this; }

		Tp const& act(Tp const& val) { value = val; return value; }
		Tp const& act(Tp const& val) const { return value; }

		Tp const& operator=(Tp const& val) { return value = val; }
		operator Tp() const { return value; };
	};
	
	template <>
	class Value<void>: public CppMetadata::Value
	{
		CppMetadata::Type const& value_type;

	public:
		Value(): value_type(retriveRuntimeType<void>()) {}
		Value(CppMetadata::Value const& val): value_type(val.type()) { action(val); }
		Value(CppMetadata::Value const* val): value_type(val->type()) { action(*val); }
		Value(CppMetadata::Type const& v_type): value_type(v_type) {}
		Value(CppMetadata::Type const& v_type, CppMetadata::Arguments const& args): value_type(v_type) {}
		
		virtual ~Value(){ }
		
		char const* const name() const { return "null"; }
		int role() const { return NONE; }
		
		CppMetadata::Type const& type() const { return value_type; }
    	
		CppMetadata::Value const* action(CppMetadata::Value const& val) const { return this; }
		CppMetadata::Value* action(CppMetadata::Value const& val) { return this; }
		
		void release() const { ::delete this; }
	};
	
	template <typename Tp>
	class ValuePtr: public CppMetadata::MultiValue<Tp>
	{
		mutable CppMetadata::MultiValue<Tp>* value_ptr{nullptr};

	public:
		ValuePtr() {}
		ValuePtr(Tp const& val): value_ptr(new Runtime::Value<Tp>(val)) {}
		ValuePtr(CppMetadata::MultiValue<Tp>* v_ptr): value_ptr(v_ptr) {}
		ValuePtr(CppMetadata::Value* v_ptr) { if (v_ptr && (v_ptr->type().id() == retriveRuntimeType<Tp>().id())) value_ptr = static_cast<CppMetadata::MultiValue<Tp>*>(v_ptr); }
		
		virtual ~ValuePtr(){ if (value_ptr) value_ptr->release(); }
		
		char const* const name() const { return value_ptr->name(); }
		int role() const { return value_ptr->role(); }
		
		CppMetadata::Type const& type() const { return value_ptr->type(); }
    		
		CppMetadata::Value const* action(CppMetadata::Value const& val) const { return value_ptr; }
		CppMetadata::Value* action(CppMetadata::Value const& val) { return value_ptr->action(val); }
		
		void release() const { value_ptr->release(); value_ptr = nullptr; }
		
		Tp const& act(Tp const& value) { return value_ptr->act(value); }
		Tp const& act(Tp const& value) const { return value_ptr->act(); }

		Tp const& operator=(Tp const& val) { return value_ptr->act(val); }
		operator Tp() const { return value_ptr->act(); };
		
		bool isNull() { return value_ptr == nullptr; }
		
		void operator=(CppMetadata::MultiValue<Tp>* v_ptr) { value_ptr = v_ptr; }
	};
	
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
void registerMember(ObjTp* object, char const* const name, CppMetadata::Value* member)
{
	object->members[name] = member;
}


// Object member implementation
namespace Impl {

template<template<typename...> class C,typename A1, typename A2,typename T>
struct apply_args;

template<template<typename...> class C, typename Arg1, typename Arg2,typename R,typename... Args>
struct apply_args<C, Arg1, Arg2, R(Args...) >
{
    typedef C<Arg1, Arg2, Args...> Type;
};

template <class ObjTp, typename ret_val, typename... params_type>
class Function: public CppMetadata::Value
{
	CppMetadata::Type const& value_type{retriveRuntimeType<ret_val>()};
public:
	typedef ret_val (ObjTp::*function_t)(params_type...);
	
	int role() const { return CppMetadata::Value::FUNCTION; }
	CppMetadata::Type const& type() const { return value_type; }
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
	
	void release() const {}
	
	char const* const name() const { return function_name; }
	
	CppMetadata::Value* action(CppMetadata::Value const& value) { CppMetadata::Arguments* args = value; return unpack_caller()(object, function, *args); }
	CppMetadata::Value const* action(CppMetadata::Value const& value) const { CppMetadata::Arguments const* args = value; return unpack_caller()(object, function, *args); }

	ret_val operator()(params_type... params)
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments*> value_arg(&arguments);
		
		CppMetadata::Value* val = action(value_arg);
		ret_val res = *val;
		val->release();
		return res;
	}
	
	ret_val operator()(params_type... params) const
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
class Function<ObjTp, void, params_type...>: public CppMetadata::Value
{
	CppMetadata::Type const& value_type{retriveRuntimeType<void>()};
public:
	typedef void (ObjTp::*function_t)(params_type...);
	
	int role() const { return CppMetadata::Value::FUNCTION; };
	CppMetadata::Type const& type() const { return value_type; }
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
	Function(ObjTp* obj, char const* const name, function_t func): function_name(name), function(func), object(obj) { registerMember<ObjTp>(object,name,this); }
	
	void release() const {}
	
	char const* const name() const { return function_name; }
	
	CppMetadata::Value* action(CppMetadata::Value const& value) { CppMetadata::Arguments* args = value; return unpack_caller()(object, function, *args); }
	CppMetadata::Value const* action(CppMetadata::Value const& value) const { CppMetadata::Arguments const* args = value; return unpack_caller()(object, function, *args); }
	
	void operator()(params_type... params)
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments*> value_arg(&arguments);
		
		action(value_arg)->release();
	}
	
	void operator()(params_type... params) const
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments const*> value_arg(&arguments);
		
		action(value_arg)->release();
	}
};

template <class ObjTp, typename Tp>
class Property: public CppMetadata::Value
{
private:
	char const* const property_name;
	Tp property_value{Tp()};
	CppMetadata::Type const& value_type{retriveRuntimeType<Tp>()};
	ObjTp* object;
public:
	typedef Tp (ObjTp::*getter_t)(int const&);
	typedef void (ObjTp::*setter_t)(Tp const&);
	
	getter_t getter{nullptr};
	setter_t setter{nullptr};
	
	Property(ObjTp* obj, char const* const name, getter_t get = nullptr, setter_t set = nullptr):
		property_name(name), object(obj), getter(get), setter(set) { registerMember<ObjTp>(object,name,this); }
		
	void release() const {}
	
	char const* const name() const { return property_name; }
	int role() const { return CppMetadata::Value::VALUE; }
	CppMetadata::Type const& type() const { return value_type; }
    
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
	
	CppMetadata::Value const* action(CppMetadata::Value const& value) const
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

}


// Object member access
namespace Access {
	
template<template<typename...> class C,typename A1, typename T>
struct apply_args;

template<template<typename...> class C, typename Arg1, typename R,typename... Args>
struct apply_args<C, Arg1, R(Args...) >
{
    typedef C<Arg1, Args...> Type;
};

template <typename ret_val, typename... params_type>
class Function: public CppMetadata::Value
{
private:
	char const* const function_name;
	CppMetadata::Value* member;
	CppMetadata::Object* object;
	
public:	
	Function(CppMetadata::Object* obj, char const* const name): function_name(name), object(obj) { }
	
	void release() const {}
	
	char const* const name() const { return function_name; }
	int role() const { return object->member(function_name).role(); }
	CppMetadata::Type const& type() const { return object->member(function_name).type(); }
	
	CppMetadata::Value* action(CppMetadata::Value const& value) { return object->member(function_name).action(value); }
	CppMetadata::Value const* action(CppMetadata::Value const& value) const { return object->member(function_name).action(value); }

	ret_val operator()(params_type... params)
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments*> value_arg(&arguments);
		
		CppMetadata::Value* val = action(value_arg);
		ret_val res = *val;
		val->release();
		return res;
	}
	
	ret_val operator()(params_type... params) const
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
class Function<void, params_type...>: public CppMetadata::Value
{
private:
	char const* const function_name;
	CppMetadata::Value* member;
	CppMetadata::Object* object;
	
public:	
	Function(CppMetadata::Object* obj, char const* const name): function_name(name), object(obj) { }
	
	void release() const {}
	
	char const* const name() const { return function_name; }
	int role() const { return object->member(function_name).role(); }
	CppMetadata::Type const& type() const { return object->member(function_name).type(); }
	
	CppMetadata::Value* action(CppMetadata::Value const& value) { return object->member(function_name).action(value); }
	CppMetadata::Value const* action(CppMetadata::Value const& value) const { return object->member(function_name).action(value); }
	
	void operator()(params_type... params)
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments*> value_arg(&arguments);
		
		action(value_arg)->release();
	}
	
	void operator()(params_type... params) const
	{
		Runtime::ArgumentsBuild arguments(params...);
		Runtime::Value<CppMetadata::Arguments const*> value_arg(&arguments);
		
		action(value_arg)->release();
	}
};

template <typename Tp>
class Property: public CppMetadata::Value
{
private:
	char const* const property_name;
	CppMetadata::Object* obj;
public:	
	Property(CppMetadata::Object* obj, char const* const name):
		property_name(name), obj(obj) { }
	
	void release() const {}
	
	char const* const name() const { return property_name; }
    int role() const { return obj->member(property_name).role(); }
	CppMetadata::Type const& type() const { return obj->member(property_name).type(); }
	
	CppMetadata::Value* action(CppMetadata::Value const& value)
	{
		return obj->member(property_name).action(value);
	}
	
	CppMetadata::Value const* action(CppMetadata::Value const& value) const
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
		CppMetadata::Value const* val = action(Runtime::Value<void>());
		Tp res = *val;
		val->release();
		return res;
	}
};

}
	
}

template <typename Tp>
Tp const& Value::operator=(Tp const& val)
{
	return static_cast<CppMetadata::MultiValue<Tp>&>(*this) = val;
}

template <typename Tp>
Value::operator Tp() const
{
	return static_cast<CppMetadata::MultiValue<Tp> const&>(*this).act();
}

#define MD_VALUE_ENABLE_ARGS(type) namespace CppMetadata { namespace Runtime { template<> inline type valueConstructor<type>(CppMetadata::Arguments const& args) { return type(args); } } }
	
}

#endif
