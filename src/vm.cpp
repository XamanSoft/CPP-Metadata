#define _MD_VM_OBJECT_DECL
#include <CppMetadata.hpp>
#include <duktape.h>
#include <limits>
#include <iostream>

using namespace CppMetadata::Runtime;

MD_OBJECT_REGISTER(VM);

VM::VM(): vm_context(duk_create_heap_default()) {
	
}

VM::~VM() {
	duk_destroy_heap(vm_context);
}

CppMetadata::Value* VM::MD_OBJECT_FUNCTION_NAME(execute)(char const* input, int type) {
	CppMetadata::Value* res = nullptr;
	if (duk_peval_string(vm_context, input) != 0) {
		std::cout << "eval failed: " << duk_safe_to_string(vm_context, -1) << std::endl;
	} else {
		res = vm_get_value();
	}
	duk_pop(vm_context);
	return res;
}

CppMetadata::Value* VM::MD_OBJECT_FUNCTION_NAME(get)(char const* name) {
	return nullptr;
}

int VM::MD_OBJECT_FUNCTION_NAME(put)(char const* name, CppMetadata::Value* value) {
	return 0;
}

CppMetadata::Value* VM::vm_get_value() {
	CppMetadata::Value* res = nullptr;
	
	if (duk_is_boolean(vm_context, -1))
		res = new CppMetadata::Runtime::Value<bool>(duk_get_boolean(vm_context, -1));
	else if (duk_is_nan(vm_context, -1))
		res = new CppMetadata::Runtime::Value<double>(std::numeric_limits<double>::quiet_NaN());
	else if (duk_is_null(vm_context, -1))
		res = nullptr;
	else if (duk_is_number(vm_context, -1))
		res = new CppMetadata::Runtime::Value<double>(duk_get_number(vm_context, -1));
	else if (duk_is_object(vm_context, -1))
		res = nullptr;
	else if (duk_is_string(vm_context, -1)) {
		if (!value_str.empty()) value_str.clear();
		value_str = duk_get_string(vm_context, -1);
		res =  new CppMetadata::Runtime::Value<char const*>(value_str.c_str());
	} else if (duk_is_undefined(vm_context, -1))
		res = nullptr;

	return res;
}