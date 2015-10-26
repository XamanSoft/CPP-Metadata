#ifndef _CPPMETADATA_RUNTIME_VALUE_HPP
#define _CPPMETADATA_RUNTIME_VALUE_HPP

namespace CppMetadata {
	
class Type;
class Arguments;

namespace Runtime {
	
	template <typename Tp>
	inline Tp valueConstructor(Arguments const& args)
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
		Value(CppMetadata::Value const& val): value_type(val.type()) { setValue(val); }
		Value(CppMetadata::Value const* val): value_type(val->type()) { setValue(*val); }
		Value(CppMetadata::Type const& v_type): value_type(v_type) {}
		Value(CppMetadata::Type const& v_type, Arguments const& args): value(valueConstructor<Tp>(args)), value_type(v_type) {}
		
		virtual ~Value(){ }
		
		CppMetadata::Type const& type() const { return value_type; }
    
		CppMetadata::Value const& getValue() const { return *this; }
		void setValue(CppMetadata::Value const& val) { if (value_type.isEqual(val.type())) value = static_cast<CppMetadata::Runtime::Value<Tp> const &>(val).value; }
		
		void release() const { ::delete this; }
		
		Tp const& get() const { return value; }
		void set(Tp const& val) { value = val; }

		Tp const& operator=(Tp const& val) { return value = val; }
		operator Tp() const { return value; };
	};
	
	template <>
	class Value<void>: public CppMetadata::Value
	{
		CppMetadata::Type const& value_type;

	public:
		Value(): value_type(retriveRuntimeType<void>()) {}
		Value(CppMetadata::Value const& val): value_type(val.type()) { setValue(val); }
		Value(CppMetadata::Value const* val): value_type(val->type()) { setValue(*val); }
		Value(CppMetadata::Type const& v_type): value_type(v_type) {}
		Value(CppMetadata::Type const& v_type, Arguments const& args): value_type(v_type) {}
		
		virtual ~Value(){ }
		
		CppMetadata::Type const& type() const { return value_type; }
    
		CppMetadata::Value const& getValue() const { return *this; }
		void setValue(CppMetadata::Value const& val) { }
		
		void release() const { ::delete this; }
	};
	
	template <typename Tp>
	class ValuePtr: public CppMetadata::MultiValue<Tp>
	{
		CppMetadata::MultiValue<Tp>* value_ptr{nullptr};

	public:
		ValuePtr() {}
		ValuePtr(Tp const& val): value_ptr(new Runtime::Value<Tp>(val)) {}
		ValuePtr(CppMetadata::MultiValue<Tp>* v_ptr): value_ptr(v_ptr) {}
		ValuePtr(CppMetadata::Value* v_ptr) { if (v_ptr->type().id() == retriveRuntimeType<Tp>().id()) value_ptr = static_cast<CppMetadata::MultiValue<Tp>*>(v_ptr); }
		
		virtual ~ValuePtr(){ value_ptr->release(); }
		
		CppMetadata::Type const& type() const { return value_ptr->type(); }
    
		CppMetadata::Value const& getValue() const { return *value_ptr; }
		void setValue(CppMetadata::Value const& val) { value_ptr->setValue(val); }
		
		void release() const { value_ptr->release(); value_ptr = nullptr; }
		
		Tp const& get() const { return value_ptr->get(); }
		void set(Tp const& val) { value_ptr->set(val); }

		Tp const& operator=(Tp const& val) { value_ptr->set(val); return value_ptr->get(); }
		operator Tp() const { return value_ptr->get(); };
		
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
	return static_cast<CppMetadata::MultiValue<Tp> const&>(*this).get();
}

#define MD_VALUE_ENABLE_ARGS(type) namespace CppMetadata { namespace Runtime { template<> inline type valueConstructor<type>(Arguments const& args) { return type(args); } } }
	
}

#endif
