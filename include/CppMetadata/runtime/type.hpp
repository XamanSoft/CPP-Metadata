#ifndef _CPPMETADATA_RUNTIME_TYPE_HPP
#define _CPPMETADATA_RUNTIME_TYPE_HPP

#include <stdexcept>

#define MD_TYPE_CONCAT(X,Y) X##Y  // helper macro
#define MD_TYPE_UNIQUE(X,Y) MD_TYPE_CONCAT(X,Y)

#define MD_DECLARE_TYPE(type) namespace CppMetadata { namespace Runtime {	template <> inline CppMetadata::Type& retriveRuntimeType<type>() { return CppMetadata::Runtime::retriveType(#type); } } }
#define MD_REGISTER_TYPE(type) static CppMetadata::Runtime::RegisterType<type> MD_TYPE_UNIQUE(register_type_,__LINE__)(#type)

namespace CppMetadata {

namespace Runtime {

	extern "C" int registerType(char const* name, CppMetadata::Type& type); // int = type-id
	extern "C" CppMetadata::Type& retriveType(char const* name);

	template <typename Tp>
	inline CppMetadata::Type& retriveRuntimeType()
	{
		throw std::invalid_argument("Use MD_DECLARE_TYPE macro to declare a type!");
	}

}

}

// Declare primitive types
MD_DECLARE_TYPE(void); // void? :O
MD_DECLARE_TYPE(void*);

// Boolean
MD_DECLARE_TYPE(bool);

// Chars
MD_DECLARE_TYPE(signed char);
MD_DECLARE_TYPE(char);
MD_DECLARE_TYPE(unsigned char);
MD_DECLARE_TYPE(wchar_t);
MD_DECLARE_TYPE(char16_t);
MD_DECLARE_TYPE(char32_t);

// Strings
MD_DECLARE_TYPE(char const *);
MD_DECLARE_TYPE(wchar_t const *);
MD_DECLARE_TYPE(char16_t const *);
MD_DECLARE_TYPE(char32_t  const *);

MD_DECLARE_TYPE(char *);
MD_DECLARE_TYPE(wchar_t *);
MD_DECLARE_TYPE(char16_t *);
MD_DECLARE_TYPE(char32_t  *);

// Integers
MD_DECLARE_TYPE(short int);
MD_DECLARE_TYPE(unsigned short int);
MD_DECLARE_TYPE(int);
MD_DECLARE_TYPE(unsigned int);
MD_DECLARE_TYPE(long int);
MD_DECLARE_TYPE(unsigned long int);
MD_DECLARE_TYPE(long long int);
MD_DECLARE_TYPE(unsigned long long int);

// Floating point
MD_DECLARE_TYPE(float);
MD_DECLARE_TYPE(double);
MD_DECLARE_TYPE(long double);

MD_DECLARE_TYPE(CppMetadata::Value*);
MD_DECLARE_TYPE(CppMetadata::Value const*);
MD_DECLARE_TYPE(CppMetadata::Object*);
MD_DECLARE_TYPE(CppMetadata::Object const*);

#include "value.hpp"

namespace CppMetadata {

namespace Runtime {
	template <typename Tp>
	class RegisterType: public CppMetadata::Type
	{
		int type_id{0};
		char const* type_name{nullptr};
		int type_size{0};

	public:
		RegisterType(char const* t_name): type_id(registerType(t_name, *this)), type_name(t_name), type_size(sizeof(Tp)) {}
		virtual ~RegisterType(){}

	    int id() const { return type_id; }
		char const* const name() const { return type_name; }
		int size() const { return type_size; }

		bool isEqual(CppMetadata::Type const& type) const { return type_id == type.id(); }
		bool isNotEqual(CppMetadata::Type const& type) const { return type_id != type.id(); }

		CppMetadata::Value* createValue() { return new CppMetadata::Runtime::Value<Tp>(); }
		CppMetadata::Value* createValue(CppMetadata::Value const& args)  { return new CppMetadata::Runtime::Value<Tp>(args); }
	};

	template <>
	class RegisterType<void>: public CppMetadata::Type
	{
		int type_id{0};
		char const* type_name{nullptr};
		int type_size{0};

	public:
		RegisterType(char const* t_name): type_id(registerType(t_name, *this)), type_name(t_name), type_size(0) {}
		virtual ~RegisterType(){}

	    int id() const { return type_id; }
		char const* const name() const { return type_name; }
		int size() const { return type_size; }

		bool isEqual(CppMetadata::Type const& type) const { return type_id == type.id(); }
		bool isNotEqual(CppMetadata::Type const& type) const { return type_id != type.id(); }

		CppMetadata::Value* createValue() { return new CppMetadata::Runtime::Value<void>(); }
		CppMetadata::Value* createValue(CppMetadata::Value const& args)  { return new CppMetadata::Runtime::Value<void>(args); }
	};

	template <typename Tp>
	class Type: public CppMetadata::Type
	{
		CppMetadata::Type &type_proxy;

	public:
		Type(): type_proxy(retriveRuntimeType<Tp>()) {}
		virtual ~Type(){}

	    int id() const { return type_proxy.id(); }
		char const* const name() const { return type_proxy.name(); }
		int size() const { return type_proxy.size(); }

		bool isEqual(CppMetadata::Type const& type) const { return type_proxy.isEqual(type); }
		bool isNotEqual(CppMetadata::Type const& type) const { return type_proxy.isNotEqual(type); }

		CppMetadata::Value* createValue() { return type_proxy.createValue(); }
		CppMetadata::Value* createValue(CppMetadata::Value const& args)  { return type_proxy.createValue(args); }

		ValuePtr<Tp> createMultiValue() { return ValuePtr<Tp>(type_proxy.createValue()); }
		ValuePtr<Tp> createMultiValue(CppMetadata::Value const& args)  { return ValuePtr<Tp>(type_proxy.createValue(args)); }
	};
}

}

#endif
