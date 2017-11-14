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
