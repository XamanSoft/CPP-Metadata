#define _MD_VM_OBJECT_DECL
#include <CppMetadata.hpp>
#include <duktape.h>
#include <limits>
#include <iostream>

using namespace CppMetadata::Runtime;

MD_OBJECT_REGISTER(VM);

static duk_ret_t do_print(duk_context *ctx);
static duk_ret_t js_Object_ctor(duk_context *ctx);
static duk_ret_t js_Object_dtor(duk_context *ctx);
static duk_ret_t js_Object_memberCount(duk_context *ctx);
static duk_ret_t js_Object_hasMember(duk_context *ctx);
static duk_ret_t js_Object_member(duk_context *ctx);

VM::VM(): vm_context(duk_create_heap_default()) {
	const duk_function_list_entry sigil_funcs[] = {
		{ "print", &do_print, DUK_VARARGS },
		{ NULL, NULL, 0 }
	};
	
	// methods, add more here
	const duk_function_list_entry js_Object_methods[] = {
		{ "memberCount",   js_Object_memberCount,  0   },
		{ "hasMember",   js_Object_hasMember,  0   },
		{ "member",   js_Object_member,  0   },
		{ nullptr,  nullptr,        0   }
	};

	duk_push_global_object(vm_context);
	duk_push_object(vm_context);
	duk_put_function_list(vm_context, -1, sigil_funcs);
	duk_push_pointer(vm_context, &vm_info);
	duk_put_prop_string(vm_context, -2, "\xff""\xff""data");
	duk_push_c_function(vm_context, js_Object_ctor, 1);
	
	duk_push_object(vm_context);
    duk_put_function_list(vm_context, -1, js_Object_methods);
    duk_put_prop_string(vm_context, -2, "prototype");
	
	duk_put_prop_string(vm_context, -2, "Object");
	duk_put_prop_string(vm_context, -2, "$");  /* -> [ ... global ] */
	duk_pop(vm_context);
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

duk_ret_t do_print(duk_context *ctx) {
	int n = duk_get_top(ctx);
	
	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, "\xff""\xff""data");
	void* data = *static_cast<void**>(duk_to_pointer(ctx, -1));
	duk_pop(ctx);
	
	//if (data == nullptr)
	//	return 0;

	for (int i = 0; i < n; i++) {
		std::cout << duk_safe_to_string(ctx, i);
	}

	return 0;  /* no return value */
}

/*
 * This is Point function, constructor. Note that it can be called
 * as a standard function call, you may need to check for
 * duk_is_constructor_call to be sure that it is constructed
 * as a "new" statement.
 */
duk_ret_t js_Object_ctor(duk_context *ctx)
{
    // Get arguments
    const char* name = duk_require_string(ctx, 0);

    // Push special this binding to the function being constructed
    duk_push_this(ctx);

    // Store the underlying object
    duk_push_pointer(ctx, CppMetadata::Runtime::createObject(name, CppMetadata::Runtime::Value<void>()));
    duk_put_prop_string(ctx, -2, "\xff""\xff""data");

    // Store a boolean flag to mark the object as deleted because the destructor may be called several times
    duk_push_boolean(ctx, false);
    duk_put_prop_string(ctx, -2, "\xff""\xff""deleted");

    // Store the function destructor
    duk_push_c_function(ctx, js_Object_dtor, 1);
    duk_set_finalizer(ctx, -2);

    return 0;
}

/*
 * This is the point destructor
 */
duk_ret_t js_Object_dtor(duk_context *ctx)
{
    // The object to delete is passed as first argument instead
    duk_get_prop_string(ctx, 0, "\xff""\xff""deleted");

    bool deleted = duk_to_boolean(ctx, -1);
    duk_pop(ctx);

    if (!deleted) {
        duk_get_prop_string(ctx, 0, "\xff""\xff""data");
        delete static_cast<CppMetadata::Object *>(duk_to_pointer(ctx, -1));
        duk_pop(ctx);

        // Mark as deleted
        duk_push_boolean(ctx, true);
        duk_put_prop_string(ctx, 0, "\xff""\xff""deleted");
    }

    return 0;
}

/*
 * Basic toString method
 */
duk_ret_t js_Object_memberCount(duk_context *ctx)
{
    duk_push_this(ctx);
    duk_get_prop_string(ctx, -1, "\xff""\xff""data");
    CppMetadata::Object *object = static_cast<CppMetadata::Object *>(duk_to_pointer(ctx, -1));
    duk_pop(ctx);
    duk_push_sprintf(ctx, "%s", object->objectInfo().name);

    return 1;
}

/*
 * Basic toString method
 */
duk_ret_t js_Object_hasMember(duk_context *ctx)
{
    duk_push_this(ctx);
    duk_get_prop_string(ctx, -1, "\xff""\xff""data");
    CppMetadata::Object *object = static_cast<CppMetadata::Object *>(duk_to_pointer(ctx, -1));
    duk_pop(ctx);
    duk_push_sprintf(ctx, "%s", object->objectInfo().name);

    return 1;
}

/*
 * Basic toString method
 */
duk_ret_t js_Object_member(duk_context *ctx)
{
    duk_push_this(ctx);
    duk_get_prop_string(ctx, -1, "\xff""\xff""data");
   CppMetadata::Object *object = static_cast<CppMetadata::Object *>(duk_to_pointer(ctx, -1));
    duk_pop(ctx);
    duk_push_sprintf(ctx, "%s", object->objectInfo().name);

    return 1;
}
