#ifndef _CPPMETADATA_HPP
#define _CPPMETADATA_HPP

// Type definitions
#include <CppMetadata/type.hpp>
#include <CppMetadata/value.hpp>
//#include <CppMetadata/arguments.hpp>
//#include <CppMetadata/member.hpp>
//#include <CppMetadata/object.hpp>

// Type implementations
#include <CppMetadata/runtime/type.hpp>
#include <CppMetadata/runtime/value.hpp>
//#include <CppMetadata/runtime/arguments.hpp>
//#include <CppMetadata/runtime/member.hpp>
//#include <CppMetadata/runtime/object.hpp>

// Declare primitive types

// Boolean
MD_DECLARE_TYPE(bool);

// Chars
MD_DECLARE_TYPE(signed char);
MD_DECLARE_TYPE(char);
MD_DECLARE_TYPE(unsigned char);
MD_DECLARE_TYPE(wchar_t);
MD_DECLARE_TYPE(char16_t);
MD_DECLARE_TYPE(char32_t);

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

#endif
