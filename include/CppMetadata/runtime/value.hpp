#ifndef _CPPMETADATA_RUNTIME_VALUE_HPP
#define _CPPMETADATA_RUNTIME_VALUE_HPP

#include <memory>

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
		Value(CppMetadata::Value const& val): value_type(val.type()) { set(val); }
		Value(CppMetadata::Type const& v_type): value_type(v_type) {}
		Value(CppMetadata::Type const& v_type, Arguments const& args): value(valueConstructor<Tp>(args)), value_type(v_type) {}
		
		CppMetadata::Type const& type() const { return value_type; }
    
		CppMetadata::Value const& getValue() const { return *this; }
		void setValue(CppMetadata::Value const& val) { if (value_type.isEqual(val.type())) value = static_cast<CppMetadata::Runtime::Value<Tp> const &>(val).value; }
		
		void release() { delete this; }
		
		Tp const& get() const { return value; }
		void set(Tp const& val) { value = val; }

		Tp const& operator=(Tp const& val) { return value = val; }
		virtual operator Tp() const { return value; };
	};
	
	template <typename Tp>
	class ValuePtr: public CppMetadata::MultiValue<Tp>
	{
		std::shared_ptr<CppMetadata::MultiValue<Tp>> value_ptr{nullptr};

	public:
		ValuePtr() {}
		ValuePtr(CppMetadata::MultiValue<Tp>* v_ptr): value_ptr(v_ptr) {}
		ValuePtr(CppMetadata::Value* v_ptr) { if (v_ptr->type().id() == retriveRuntimeType<Tp>().id()) value_ptr.reset(static_cast<CppMetadata::MultiValue<Tp>*>(v_ptr)); }
		
		CppMetadata::Type const& type() const { return value_ptr->type(); }
    
		CppMetadata::Value const& getValue() const { return *value_ptr; }
		void setValue(CppMetadata::Value const& val) { value_ptr->setValue(val); }
		
		void release() { value_ptr.reset(); }
		
		Tp const& get() const { return value_ptr->get(); }
		void set(Tp const& val) { value_ptr->set(val); }

		Tp const& operator=(Tp const& val) { value_ptr->set(val); return value_ptr->get(); }
		virtual operator Tp() const { return value_ptr->get(); };
		
		void operator=(CppMetadata::MultiValue<Tp>* v_ptr) { value_ptr.reset(v_ptr); }
	};
}

#define MD_VALUE_ENABLE_ARGS(type) namespace CppMetadata { namespace Runtime { template<> inline type valueConstructor<type>(Arguments const& args) { return type(args); } } }
	
}

#endif
