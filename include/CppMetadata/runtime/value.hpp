#ifndef _CPPMETADATA_RUNTIME_VALUE_HPP
#define _CPPMETADATA_RUNTIME_VALUE_HPP

namespace CppMetadata {

namespace Runtime {

	template <typename Tp>
	inline Tp valueConstructor(CppMetadata::Value const& args)
	{
		return Tp();
	}
	
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

	template <typename... params_type>
	class Value: public CppMetadata::MultiValue<params_type...>
	{
		CppMetadata::Value* values[sizeof...(params_type)];
		CppMetadata::Type const& value_type;
		
		template<class T, class... Args>
		void Build(int index, T head, Args... params)
		{
			if (values[index] == nullptr) {
				values[index] = new Runtime::Value<T>(head);
				Build(index+1, params...);
				return;
			}

			*static_cast<Runtime::Value<T>*>(values[index]) = head;
			Build(index+1, params...);
		}

		template<class T>
		void Build(int index, T head)
		{
			if (values[index] == nullptr) {
				values[index] = new Runtime::Value<T>(head);
				return;
			}

			*static_cast<Runtime::Value<T>*>(values[index]) = head;
		}

		void Build(int index) {}

	public:
		Value(): values(), value_type(retriveRuntimeType<void>()) {}
		Value(CppMetadata::Value* const* vals): values(), value_type(retriveRuntimeType<void>()) { for (int i = 0; i < sizeof...(params_type); i++) values[i] = vals[i]->clone(); }
		Value(params_type... params): values(), value_type(retriveRuntimeType<void>()) { Build(0, params...); }

		virtual ~Value() { for (int i = 0; i < sizeof...(params_type); i++) values[i]->release(); }

		char const* const name() const { return nullptr; }
		CppMetadata::Type const& type() const { return value_type; }
		int role() const { return CppMetadata::Value::MULTIVALUE; }
		int count() const { return sizeof...(params_type); }
		
		CppMetadata::Value* clone() const { return new Runtime::Value<params_type...>(values); }
		
		CppMetadata::Value& at(int index) { return *values[index]; }
		CppMetadata::Value const& at(int index) const { return *values[index]; }
    
		CppMetadata::Value const* action(CppMetadata::Value const& val) const { return this; }
		CppMetadata::Value* action(CppMetadata::Value const& val)
		{
			if (val.count() == sizeof...(params_type))
				for (int i = 0; i < sizeof...(params_type); i++)
				{
					if (values[i] == nullptr)
						values[i]->release();

					values[i] = val.at(i).clone();
				}

			return this;
		}

		CppMetadata::Value& operator[](int index) { return *values[index]; }
		CppMetadata::Value const& operator()(int index) const { return *values[index]; }
		
		CppMetadata::MultiValue<params_type...>& operator()(params_type... params) { return *this; }
		CppMetadata::MultiValue<params_type...> const& operator()(params_type... params) const { return *this; }

		void release() const { ::delete this; }
	};
	
	template <typename Tp>
	class Value<Tp>: public CppMetadata::MultiValue<Tp>
	{
		Tp value;
		CppMetadata::Type const& value_type;

	public:
		Value(): value(Tp()), value_type(retriveRuntimeType<Tp>()) { }
		Value(Tp val): value(val), value_type(retriveRuntimeType<Tp>()) { }

		char const* const name() const { return nullptr; }
		CppMetadata::Type const& type() const { return value_type; }
		int role() const { return CppMetadata::Value::VALUE; }
		int count() const { return 1; }

		CppMetadata::Value* clone() const { return new Runtime::Value<Tp>(value); }

		CppMetadata::Value& at(int index) { return *this; }
		CppMetadata::Value const& at(int index) const { return *this; }

		CppMetadata::Value* action(CppMetadata::Value const& val) { if (value_type.isEqual(val.type())) value = static_cast<CppMetadata::Runtime::Value<Tp> const&>(val).value; return this; }
		CppMetadata::Value const* action(CppMetadata::Value const& value) const { return this; }

		void release() const { ::delete this; }

		CppMetadata::MultiValue<Tp>& operator()(Tp param) { value = param; return *this; }
		CppMetadata::MultiValue<Tp> const& operator()(Tp param) const { return *this; }

		Tp const& operator=(Tp const& val) { return value = val; }
		operator Tp() const { return value; }

		virtual ~Value() { }
	};
	
	template <>
	class Value<void>: public CppMetadata::Value
	{
	public:
		Value() {}
		
		virtual ~Value(){ }

		char const* const name() const { return "null"; }
		CppMetadata::Type const& type() const { return retriveRuntimeType<void>(); }
		int role() const { return NONE; }
		int count() const { return 0; }
	
		CppMetadata::Value* clone() const { return new Runtime::Value<void>(); }
		
		CppMetadata::Value& at(int index) { return *this; }
		CppMetadata::Value const& at(int index) const { return *this; }
		
		CppMetadata::Value const* action(CppMetadata::Value const& val) const { return this; }
		CppMetadata::Value* action(CppMetadata::Value const& val) { return this; }

		void release() const { ::delete this; }
	};
	
	template <>
	class Value<>: public CppMetadata::Value
	{
	public:
		Value() {}
		Value(CppMetadata::Value const& val) { action(val); }
		Value(CppMetadata::Value const* val) { action(*val); }
		
		virtual ~Value(){ }

		char const* const name() const { return "null"; }
		CppMetadata::Type const& type() const { return retriveRuntimeType<void>(); }
		int role() const { return NONE; }
		int count() const { return 0; }
	
		CppMetadata::Value* clone() const { return new Runtime::Value<>(); }
		
		CppMetadata::Value& at(int index) { return *this; }
		CppMetadata::Value const& at(int index) const { return *this; }
		
		CppMetadata::Value const* action(CppMetadata::Value const& val) const { return this; }
		CppMetadata::Value* action(CppMetadata::Value const& val) { return this; }

		void release() const { ::delete this; }
	};
	
	template <typename... Types>
	inline Value<Types...>* buildValues(Types... args)
	{
		return new Value<Types...>(args...);
	}
	
	template <typename Tp>
	class ValuePtr: public CppMetadata::MultiValue<Tp>
	{
		mutable CppMetadata::MultiValue<Tp>* value_ptr{nullptr};

	public:
		ValuePtr() {}
		ValuePtr(Tp const& val): value_ptr(new Runtime::Value<Tp>(val)) {}
		ValuePtr(CppMetadata::MultiValue<Tp>* v_ptr): value_ptr(v_ptr) {}
		ValuePtr(CppMetadata::Value* v_ptr) { value_ptr = static_cast<CppMetadata::MultiValue<Tp>*>(v_ptr); }
		
		virtual ~ValuePtr(){ if (value_ptr) value_ptr->release(); }
		
		char const* const name() const { return value_ptr->name(); }
		CppMetadata::Type const& type() const { return value_ptr->type(); }
		int role() const { return value_ptr->role(); }
		int count() const { return value_ptr->count(); }
	
		CppMetadata::Value* clone() const { return value_ptr->clone(); }
		
		CppMetadata::Value& at(int index) { return value_ptr->at(index); }
		CppMetadata::Value const& at(int index) const { return value_ptr->at(index); }
    		
		CppMetadata::Value const* action(CppMetadata::Value const& val) const { return value_ptr; }
		CppMetadata::Value* action(CppMetadata::Value const& val) { return value_ptr->action(val); }
		
		void release() const { value_ptr->release(); value_ptr = nullptr; }
	
		CppMetadata::MultiValue<Tp>& operator()(Tp param) { value_ptr->operator()(param); return *this; }
		CppMetadata::MultiValue<Tp> const& operator()(Tp param) const { value_ptr->operator()(param); return *this; }
		
		Tp const& operator=(Tp const& val) { return value_ptr->operator=(val); }
		operator Tp() const { return value_ptr->operator Tp(); }

		bool isNull() { return value_ptr == nullptr; }
		
		void operator=(CppMetadata::Value* v_ptr) { value_ptr = static_cast<CppMetadata::MultiValue<Tp>*>(v_ptr); }
		void operator=(CppMetadata::MultiValue<Tp>* v_ptr) { value_ptr = v_ptr; }
	};

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
		CppMetadata::Value* call(ObjTp* object, function_t f, CppMetadata::Value& args, indices<I...>) const
		{
			return new Runtime::Value<ret_val>((object->*f)(args.at(I)...));
		}
		
		template <size_t... I>
		CppMetadata::Value* call(ObjTp * object, function_t f, CppMetadata::Value const& args, indices<I...>) const
		{
			return new Runtime::Value<ret_val>((object->*f)(args.at(I)...));
		}

	public:
		CppMetadata::Value* operator () (ObjTp* object, function_t f, CppMetadata::Value& args)
		{
			//assert(args.size() == num_args); // just to be sure
			return call(object, f, args, BuildIndices<sizeof...(params_type)>{});
		}
		
		CppMetadata::Value* operator () (ObjTp* object, function_t f, CppMetadata::Value const& args) const
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
	int count() const { return sizeof...(params_type); }
	
	CppMetadata::Value* clone() const { return nullptr; }
	
	CppMetadata::Value& at(int index) { return *this; }
	CppMetadata::Value const& at(int index) const { return *this; }
	
	CppMetadata::Value* action(CppMetadata::Value const& value) { return unpack_caller()(object, function, value); }
	CppMetadata::Value const* action(CppMetadata::Value const& value) const { return unpack_caller()(object, function, value); }

	ret_val operator()(params_type... params)
	{
		CppMetadata::Value* args = buildValues(params...);
		CppMetadata::Value* val = action(*args);
		ret_val res = *val;
		val->release();
		args->release();
		return res;
	}

	ret_val operator()(params_type... params) const
	{		
		CppMetadata::Value* args = buildValues(params...);
		CppMetadata::Value* val = action(*args);
		ret_val res = *val;
		val->release();
		args->release();
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
		CppMetadata::Value* call(ObjTp* object, function_t f, CppMetadata::Value& args, indices<I...>) const
		{
			(object->*f)(args.at(I)...);
			return new Runtime::Value<void>();
		}
		
		template <size_t... I>
		CppMetadata::Value* call(ObjTp * object, function_t f, CppMetadata::Value const& args, indices<I...>) const
		{
			(object->*f)(args.at(I)...);
			return new Runtime::Value<void>();
		}

	public:
		CppMetadata::Value* operator () (ObjTp* object, function_t f, CppMetadata::Value& args)
		{
			//assert(args.size() == num_args); // just to be sure
			return call(object, f, args, BuildIndices<sizeof...(params_type)>{});
		}
		
		CppMetadata::Value* operator () (ObjTp* object, function_t f, CppMetadata::Value const& args) const
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
	int count() const { return sizeof...(params_type); }
	
	CppMetadata::Value* clone() const { return nullptr; }
	
	CppMetadata::Value& at(int index) { return *this; }
	CppMetadata::Value const& at(int index) const { return *this; }
	
	CppMetadata::Value* action(CppMetadata::Value const& value) { return unpack_caller()(object, function, value); }
	CppMetadata::Value const* action(CppMetadata::Value const& value) const { return unpack_caller()(object, function, value); }

	void operator()(params_type... params)
	{
		CppMetadata::Value* val = buildValues(params...);
		action(*val)->release();
		val->release();
	}
	
	void operator()(params_type... params) const
	{
		CppMetadata::Value* val = buildValues(params...);
		action(*val)->release();
		val->release();
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
	typedef Tp (ObjTp::*getter_t)(Tp const&);
	typedef void (ObjTp::*setter_t)(Tp const&);
	
	getter_t getter{nullptr};
	setter_t setter{nullptr};
	
	Property(ObjTp* obj, char const* const name, getter_t get = nullptr, setter_t set = nullptr):
		property_name(name), object(obj), getter(get), setter(set) { registerMember<ObjTp>(object,name,this); }
		
	void release() const {}
	
	char const* const name() const { return property_name; }
	CppMetadata::Type const& type() const { return value_type; }
	int role() const { return CppMetadata::Value::VALUE; }
	int count() const { return 1; }
	
	CppMetadata::Value* clone() const { return nullptr; }
	
	CppMetadata::Value& at(int index) { return *this; }
	CppMetadata::Value const& at(int index) const { return *this; }
    
	CppMetadata::Value* action(CppMetadata::Value const& value)
	{
		if (value.type().isNotEqual(Runtime::Type<void>()))
		{
			property_value = value;
			if (setter != nullptr)
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
	CppMetadata::Type const& type() const { return object->member(function_name).type(); }
	int role() const { return object->member(function_name).role(); }
	int count() const { return object->member(function_name).count(); }
	
	virtual CppMetadata::Value* clone() const { return object->member(function_name).clone(); }
	
	virtual CppMetadata::Value& at(int index) { return object->member(function_name).at(index); }
	virtual CppMetadata::Value const& at(int index) const { return object->member(function_name).at(index); }
	
	CppMetadata::Value* action(CppMetadata::Value const& value) { return object->member(function_name).action(value); }
	CppMetadata::Value const* action(CppMetadata::Value const& value) const { return object->member(function_name).action(value); }
	
	ret_val operator()(params_type... params)
	{
		CppMetadata::Value* args = buildValues(params...);
		CppMetadata::Value* val = action(*args);
		ret_val res = *val;
		val->release();
		args->release();
		return res;
	}
	
	ret_val operator()(params_type... params) const
	{	
		CppMetadata::Value* args = buildValues(params...);
		CppMetadata::Value* val = action(*args);
		ret_val res = *val;
		val->release();
		args->release();
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
	CppMetadata::Type const& type() const { return object->member(function_name).type(); }
	int role() const { return object->member(function_name).role(); }
	int count() const { return object->member(function_name).count(); }
	
	virtual CppMetadata::Value* clone() const { return object->member(function_name).clone(); }
	
	virtual CppMetadata::Value& at(int index) { return object->member(function_name).at(index); }
	virtual CppMetadata::Value const& at(int index) const { return object->member(function_name).at(index); }
	
	CppMetadata::Value* action(CppMetadata::Value const& value) { return object->member(function_name).action(value); }
	CppMetadata::Value const* action(CppMetadata::Value const& value) const { return object->member(function_name).action(value); }
	
	void operator()(params_type... params)
	{
		CppMetadata::Value* args = buildValues(params...);
		action(*args)->release();
		args->release();
	}
	
	void operator()(params_type... params) const
	{
		CppMetadata::Value* args = buildValues(params...);
		action(*args)->release();
		args->release();
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
	CppMetadata::Type const& type() const { return obj->member(property_name).type(); }
    int role() const { return obj->member(property_name).role(); }
	int count() const { return obj->member(property_name).count(); }
	
	CppMetadata::Value* clone() const { return obj->member(property_name).clone(); }
	
	CppMetadata::Value& at(int index) { return obj->member(property_name).at(index); }
	CppMetadata::Value const& at(int index) const { return obj->member(property_name).at(index); }
	
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
Tp const& Value::operator[](int index) const
{
	if (index > this->count()) return Tp();

	return static_cast<CppMetadata::MultiValue<Tp> const&>(this->at(index));
}

template <typename Tp>
Tp& Value::operator[](int index)
{
	if (index > this->count()) return Tp();

	return static_cast<CppMetadata::MultiValue<Tp> const&>(this->at(index));
}

template <typename Tp>
Tp const& Value::operator=(Tp const& val)
{
	return static_cast<CppMetadata::MultiValue<Tp>&>(*this) = val;
}

template <typename Tp>
Value::operator Tp() const
{
	return static_cast<CppMetadata::MultiValue<Tp> const&>(*this);
}

#define MD_VALUE_ENABLE_ARGS(type) namespace CppMetadata { namespace Runtime { template<> inline type valueConstructor<type>(CppMetadata::Value const& args) { return type(args); } } }
	
}

#endif
