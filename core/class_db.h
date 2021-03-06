/*************************************************************************/
/*  class_db.h                                                           */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                    http://www.godotengine.org                         */
/*************************************************************************/
/* Copyright (c) 2007-2017 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2017 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#ifndef CLASS_DB_H
#define CLASS_DB_H

#include "method_bind.h"
#include "object.h"
#include "print_string.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

struct ParamHint {

	String name;
	PropertyHint hint;
	String hint_text;
	Variant default_val;

	ParamHint(const String &p_name = "", PropertyHint p_hint = PROPERTY_HINT_NONE, const String &p_hint_text = "", const Variant &p_default_val = Variant())
		: name(p_name),
		  hint(p_hint),
		  hint_text(p_hint_text),
		  default_val(p_default_val) {
	}
};

struct ParamDef {
	bool used;
	Variant val;
	_FORCE_INLINE_ ParamDef() { used = false; }
	ParamDef(const Variant &p_variant);
};

//#define DEFVAL( m_defval ) ParamDef(m_defval)
#define DEFVAL(m_defval) (m_defval)

//#define SIMPLE_METHODDEF

#ifdef DEBUG_METHODS_ENABLED

struct MethodDefinition {

	StringName name;
	Vector<StringName> args;
	MethodDefinition() {}
	MethodDefinition(const char *p_name)
		: name(p_name) {}
	MethodDefinition(const StringName &p_name)
		: name(p_name) {}
};

MethodDefinition D_METHOD(const char *p_name);
MethodDefinition D_METHOD(const char *p_name, const char *p_arg1);
MethodDefinition D_METHOD(const char *p_name, const char *p_arg1, const char *p_arg2);
MethodDefinition D_METHOD(const char *p_name, const char *p_arg1, const char *p_arg2, const char *p_arg3);
MethodDefinition D_METHOD(const char *p_name, const char *p_arg1, const char *p_arg2, const char *p_arg3, const char *p_arg4);
MethodDefinition D_METHOD(const char *p_name, const char *p_arg1, const char *p_arg2, const char *p_arg3, const char *p_arg4, const char *p_arg5);
MethodDefinition D_METHOD(const char *p_name, const char *p_arg1, const char *p_arg2, const char *p_arg3, const char *p_arg4, const char *p_arg5, const char *p_arg6);
MethodDefinition D_METHOD(const char *p_name, const char *p_arg1, const char *p_arg2, const char *p_arg3, const char *p_arg4, const char *p_arg5, const char *p_arg6, const char *p_arg7);
MethodDefinition D_METHOD(const char *p_name, const char *p_arg1, const char *p_arg2, const char *p_arg3, const char *p_arg4, const char *p_arg5, const char *p_arg6, const char *p_arg7, const char *p_arg8);
MethodDefinition D_METHOD(const char *p_name, const char *p_arg1, const char *p_arg2, const char *p_arg3, const char *p_arg4, const char *p_arg5, const char *p_arg6, const char *p_arg7, const char *p_arg8, const char *p_arg9);
MethodDefinition D_METHOD(const char *p_name, const char *p_arg1, const char *p_arg2, const char *p_arg3, const char *p_arg4, const char *p_arg5, const char *p_arg6, const char *p_arg7, const char *p_arg8, const char *p_arg9, const char *p_arg10);

#else

//#define NO_VARIADIC_MACROS

#ifdef NO_VARIADIC_MACROS

static _FORCE_INLINE_ const char *D_METHOD(const char *m_name, ...) {
	return m_name;
}

#else

// When DEBUG_METHODS_ENABLED is set this will let the engine know
// the argument names for easier debugging.
#define D_METHOD(m_c, ...) m_c

#endif

#endif

class ClassDB {
public:
	enum APIType {
		API_CORE,
		API_EDITOR,
		API_NONE
	};

public:
	struct PropertySetGet {

		int index;
		StringName setter;
		StringName getter;
		MethodBind *_setptr;
		MethodBind *_getptr;
		Variant::Type type;
	};

	struct ClassInfo {

		APIType api;
		ClassInfo *inherits_ptr;
		HashMap<StringName, MethodBind *, StringNameHasher> method_map;
		HashMap<StringName, int, StringNameHasher> constant_map;
		HashMap<StringName, MethodInfo, StringNameHasher> signal_map;
		List<PropertyInfo> property_list;
#ifdef DEBUG_METHODS_ENABLED
		HashMap<StringName, List<StringName> > enum_map;
		List<StringName> constant_order;
		List<StringName> method_order;
		Set<StringName> methods_in_properties;
		List<MethodInfo> virtual_methods;
		StringName category;
#endif
		HashMap<StringName, PropertySetGet, StringNameHasher> property_setget;

		StringName inherits;
		StringName name;
		bool disabled;
		Object *(*creation_func)();
		ClassInfo();
		~ClassInfo();
	};

	template <class T>
	static Object *creator() {
		return memnew(T);
	}

	static RWLock *lock;
	static HashMap<StringName, ClassInfo, StringNameHasher> classes;
	static HashMap<StringName, StringName, StringNameHasher> resource_base_extensions;
	static HashMap<StringName, StringName, StringNameHasher> compat_classes;

#ifdef DEBUG_METHODS_ENABLED
	static MethodBind *bind_methodfi(uint32_t p_flags, MethodBind *p_bind, const MethodDefinition &method_name, const Variant **p_defs, int p_defcount);
#else
	static MethodBind *bind_methodfi(uint32_t p_flags, MethodBind *p_bind, const char *method_name, const Variant **p_defs, int p_defcount);
#endif

	static APIType current_api;

	static void _add_class2(const StringName &p_class, const StringName &p_inherits);

public:
	// DO NOT USE THIS!!!!!! NEEDS TO BE PUBLIC BUT DO NOT USE NO MATTER WHAT!!!
	template <class T>
	static void _add_class() {

		_add_class2(T::get_class_static(), T::get_parent_class_static());
#if 0
		GLOBAL_LOCK_FUNCTION;

		StringName name = T::get_class_static();

		ERR_FAIL_COND(types.has(name));

		types[name]=TypeInfo();
		TypeInfo &ti=types[name];
		ti.name=name;
		ti.inherits=T::get_parent_class_static();

		if (ti.inherits) {

			ERR_FAIL_COND( !types.has(ti.inherits) ); //it MUST be registered.
			ti.inherits_ptr = &types[ti.inherits];

		} else {
			ti.inherits_ptr=NULL;
		}
#endif
	}

	template <class T>
	static void register_class() {

		GLOBAL_LOCK_FUNCTION;
		T::initialize_class();
		ClassInfo *t = classes.getptr(T::get_class_static());
		ERR_FAIL_COND(!t);
		t->creation_func = &creator<T>;
		T::register_custom_data_to_otdb();
	}

	template <class T>
	static void register_virtual_class() {

		GLOBAL_LOCK_FUNCTION;
		T::initialize_class();
		//nothing
	}

	template <class T>
	static Object *_create_ptr_func() {

		return T::create();
	}

	template <class T>
	static void register_custom_instance_class() {

		GLOBAL_LOCK_FUNCTION;
		T::initialize_class();
		ClassInfo *t = classes.getptr(T::get_class_static());
		ERR_FAIL_COND(!t);
		t->creation_func = &_create_ptr_func<T>;
		T::register_custom_data_to_otdb();
	}

	static void get_class_list(List<StringName> *p_classes);
	static void get_inheriters_from_class(const StringName &p_class, List<StringName> *p_classes);
	static StringName get_parent_class_nocheck(const StringName &p_class);
	static StringName get_parent_class(const StringName &p_class);
	static bool class_exists(const StringName &p_class);
	static bool is_parent_class(const StringName &p_class, const StringName &p_inherits);
	static bool can_instance(const StringName &p_class);
	static Object *instance(const StringName &p_class);
	static APIType get_api_type(const StringName &p_class);

	static uint64_t get_api_hash(APIType p_api);

#if 0
	template<class N, class M>
	static MethodBind* bind_method(N p_method_name, M p_method,
		//default arguments
		ParamDef d1=ParamDef(),
		ParamDef d2=ParamDef(),
		ParamDef d3=ParamDef(),
		ParamDef d4=ParamDef(),
		ParamDef d5=ParamDef()
		) {

		return bind_methodf(METHOD_FLAGS_DEFAULT,p_method_name, p_method, d1,d2,d3,d4,d5);
	}



	template<class N, class M>
	static MethodBind* bind_methodf(uint32_t p_flags, N p_method_name, M p_method,


		//default arguments
		const ParamDef &d1=ParamDef(),
		const ParamDef &d2=ParamDef(),
		const ParamDef &d3=ParamDef(),
		const ParamDef &d4=ParamDef(),
		const ParamDef &d5=ParamDef()
		) {

		GLOBAL_LOCK_FUNCTION;

		MethodDefinition method_name=p_method_name;

		MethodBind *bind = create_method_bind(p_method);
		bind->set_name(method_name.name);
		ERR_FAIL_COND_V(!bind,NULL);

		String instance_type=bind->get_instance_type();

		TypeInfo *type=types.getptr(instance_type);
		if (!type) {
			memdelete(bind);
			ERR_FAIL_COND_V(!type,NULL);
		}

		if (type->method_map.has(method_name.name)) {
			memdelete(bind);
			// overloading not supported
			ERR_EXPLAIN("Method already bound: "+instance_type+"::"+method_name.name);
			ERR_FAIL_V(NULL);
		}
		bind->set_argument_names(method_name.args);
		type->method_map[method_name.name]=bind;

		Vector<Variant> defvals;

#define PARSE_DEFVAL(m_defval)              \
	if (d##m_defval.used)                   \
		defvals.insert(0, d##m_defval.val); \
	else                                    \
		goto set_defvals;


		PARSE_DEFVAL(1);
		PARSE_DEFVAL(2);
		PARSE_DEFVAL(3);
		PARSE_DEFVAL(4);
		PARSE_DEFVAL(5);
		set_defvals:

		bind->set_default_arguments(defvals);
		bind->set_hint_flags(p_flags);

		return bind;
#undef PARSE_DEFVAL
	}
#else

#if 0
	template<class N, class M>
	static MethodBind* bind_method(N p_method_name, M p_method,
		//default arguments
		const ParamDef &d1=ParamDef(),
		const ParamDef &d2=ParamDef(),
		const ParamDef &d3=ParamDef(),
		const ParamDef &d4=ParamDef(),
		const ParamDef &d5=ParamDef()
		) {

		MethodDefinition method_name=p_method_name;

		MethodBind *bind = create_method_bind(p_method);

		return bind_methodfi(METHOD_FLAGS_DEFAULT,bind,method_name,d1,d2,d3,d4,d5); //use static function, much smaller binary usage
	}
#endif

	template <class N, class M>
	static MethodBind *bind_method(N p_method_name, M p_method) {

		MethodBind *bind = create_method_bind(p_method);

		return bind_methodfi(METHOD_FLAGS_DEFAULT, bind, p_method_name, NULL, 0); //use static function, much smaller binary usage
	}

	template <class N, class M>
	static MethodBind *bind_method(N p_method_name, M p_method, const Variant &p_def1) {

		MethodBind *bind = create_method_bind(p_method);
		const Variant *ptr[1] = { &p_def1 };

		return bind_methodfi(METHOD_FLAGS_DEFAULT, bind, p_method_name, ptr, 1);
	}

	template <class N, class M>
	static MethodBind *bind_method(N p_method_name, M p_method, const Variant &p_def1, const Variant &p_def2) {

		MethodBind *bind = create_method_bind(p_method);
		const Variant *ptr[2] = { &p_def1, &p_def2 };

		return bind_methodfi(METHOD_FLAGS_DEFAULT, bind, p_method_name, ptr, 2);
	}

	template <class N, class M>
	static MethodBind *bind_method(N p_method_name, M p_method, const Variant &p_def1, const Variant &p_def2, const Variant &p_def3) {

		MethodBind *bind = create_method_bind(p_method);
		const Variant *ptr[3] = { &p_def1, &p_def2, &p_def3 };

		return bind_methodfi(METHOD_FLAGS_DEFAULT, bind, p_method_name, ptr, 3);
	}

	template <class N, class M>
	static MethodBind *bind_method(N p_method_name, M p_method, const Variant &p_def1, const Variant &p_def2, const Variant &p_def3, const Variant &p_def4) {

		MethodBind *bind = create_method_bind(p_method);
		const Variant *ptr[4] = { &p_def1, &p_def2, &p_def3, &p_def4 };

		return bind_methodfi(METHOD_FLAGS_DEFAULT, bind, p_method_name, ptr, 4);
	}

	template <class N, class M>
	static MethodBind *bind_method(N p_method_name, M p_method, const Variant &p_def1, const Variant &p_def2, const Variant &p_def3, const Variant &p_def4, const Variant &p_def5) {

		MethodBind *bind = create_method_bind(p_method);
		const Variant *ptr[5] = { &p_def1, &p_def2, &p_def3, &p_def4, &p_def5 };

		return bind_methodfi(METHOD_FLAGS_DEFAULT, bind, p_method_name, ptr, 5);
	}

	template <class N, class M>
	static MethodBind *bind_method(N p_method_name, M p_method, const Variant &p_def1, const Variant &p_def2, const Variant &p_def3, const Variant &p_def4, const Variant &p_def5, const Variant &p_def6) {

		MethodBind *bind = create_method_bind(p_method);
		const Variant *ptr[6] = { &p_def1, &p_def2, &p_def3, &p_def4, &p_def5, &p_def6 };

		return bind_methodfi(METHOD_FLAGS_DEFAULT, bind, p_method_name, ptr, 6);
	}

#if 0
	template<class N, class M>
	static MethodBind* bind_methodf(uint32_t p_flags, N p_method_name, M p_method,

		const ParamDef& d1=ParamDef(),
		const ParamDef& d2=ParamDef(),
		const ParamDef& d3=ParamDef(),
		const ParamDef& d4=ParamDef(),
		const ParamDef& d5=ParamDef()
		) {

		MethodDefinition method_name=p_method_name;

		MethodBind *bind = create_method_bind(p_method);

		return bind_methodfi(p_flags,bind,method_name,d1,d2,d3,d4,d5); //use static function, much smaller binary usage
	}
#endif

#endif
	template <class M>
	static MethodBind *bind_vararg_method(uint32_t p_flags, StringName p_name, M p_method, const MethodInfo &p_info = MethodInfo(), const Vector<Variant> &p_default_args = Vector<Variant>()) {

		GLOBAL_LOCK_FUNCTION;

		MethodBind *bind = create_vararg_method_bind(p_method, p_info);
		ERR_FAIL_COND_V(!bind, NULL);

		bind->set_name(p_name);
		bind->set_default_arguments(p_default_args);

		String instance_type = bind->get_instance_class();

		ClassInfo *type = classes.getptr(instance_type);
		if (!type) {
			memdelete(bind);
			ERR_FAIL_COND_V(!type, NULL);
		}

		if (type->method_map.has(p_name)) {
			memdelete(bind);
			// overloading not supported
			ERR_EXPLAIN("Method already bound: " + instance_type + "::" + p_name);
			ERR_FAIL_V(NULL);
		}
		type->method_map[p_name] = bind;
#ifdef DEBUG_METHODS_ENABLED
		bind->set_return_type("Variant");
		type->method_order.push_back(p_name);
#endif

		return bind;
	}

	static void add_signal(StringName p_class, const MethodInfo &p_signal);
	static bool has_signal(StringName p_class, StringName p_signal);
	static bool get_signal(StringName p_class, StringName p_signal, MethodInfo *r_signal);
	static void get_signal_list(StringName p_class, List<MethodInfo> *p_signals, bool p_no_inheritance = false);

	static void add_property_group(StringName p_class, const String &p_name, const String &p_prefix = "");
	static void add_property(StringName p_class, const PropertyInfo &p_pinfo, const StringName &p_setter, const StringName &p_getter, int p_index = -1);
	static void get_property_list(StringName p_class, List<PropertyInfo> *p_list, bool p_no_inheritance = false, const Object *p_validator = NULL);
	static bool set_property(Object *p_object, const StringName &p_property, const Variant &p_value, bool *r_valid = NULL);
	static bool get_property(Object *p_object, const StringName &p_property, Variant &r_value);
	static bool has_property(const StringName &p_class, const StringName &p_property, bool p_no_inheritance = false);
	static int get_property_index(const StringName &p_class, const StringName &p_property, bool *r_is_valid = NULL);
	static Variant::Type get_property_type(const StringName &p_class, const StringName &p_property, bool *r_is_valid = NULL);
	static StringName get_property_setter(StringName p_class, const StringName p_property);
	static StringName get_property_getter(StringName p_class, const StringName p_property);

	static bool has_method(StringName p_class, StringName p_method, bool p_no_inheritance = false);
	static void set_method_flags(StringName p_class, StringName p_method, int p_flags);

	static void get_method_list(StringName p_class, List<MethodInfo> *p_methods, bool p_no_inheritance = false, bool p_exclude_from_properties = false);
	static MethodBind *get_method(StringName p_class, StringName p_name);

	static void add_virtual_method(const StringName &p_class, const MethodInfo &p_method, bool p_virtual = true);
	static void get_virtual_methods(const StringName &p_class, List<MethodInfo> *p_methods, bool p_no_inheritance = false);

	static void bind_integer_constant(const StringName &p_class, const StringName &p_enum, const StringName &p_name, int p_constant);
	static void get_integer_constant_list(const StringName &p_class, List<String> *p_constants, bool p_no_inheritance = false);
	static int get_integer_constant(const StringName &p_class, const StringName &p_name, bool *p_success = NULL);

#ifdef DEBUG_METHODS_ENABLED
	static StringName get_integer_constant_enum(const StringName &p_class, const StringName &p_name, bool p_no_inheritance = false);
	static void get_enum_list(const StringName &p_class, List<StringName> *p_enums, bool p_no_inheritance = false);
	static void get_enum_constants(const StringName &p_class, const StringName &p_enum, List<StringName> *p_constants, bool p_no_inheritance = false);
#endif

	static StringName get_category(const StringName &p_node);

	static bool get_setter_and_type_for_property(const StringName &p_class, const StringName &p_prop, StringName &r_class, StringName &r_setter);

	static void set_class_enabled(StringName p_class, bool p_enable);
	static bool is_class_enabled(StringName p_class);

	static void add_resource_base_extension(const StringName &p_extension, const StringName &p_class);
	static void get_resource_base_extensions(List<String> *p_extensions);
	static void get_extensions_for_type(const StringName &p_class, List<String> *p_extensions);

	static void add_compatibility_class(const StringName &p_class, const StringName &p_fallback);
	static void init();

	static void set_current_api(APIType p_api);
	static void cleanup();
};

#ifdef DEBUG_METHODS_ENABLED

#define BIND_CONSTANT(m_constant) \
	ClassDB::bind_integer_constant(get_class_static(), StringName(), #m_constant, m_constant);

#define BIND_ENUM_CONSTANT(m_constant) \
	ClassDB::bind_integer_constant(get_class_static(), __constant_get_enum_name(m_constant, #m_constant), #m_constant, m_constant);

#else

#define BIND_CONSTANT(m_constant) \
	ClassDB::bind_integer_constant(get_class_static(), StringName(), #m_constant, m_constant);

#define BIND_ENUM_CONSTANT(m_constant) \
	ClassDB::bind_integer_constant(get_class_static(), StringName(), #m_constant, m_constant);

#endif

#ifdef TOOLS_ENABLED

#define BIND_VMETHOD(m_method) \
	ClassDB::add_virtual_method(get_class_static(), m_method);

#else

#define BIND_VMETHOD(m_method)

#endif

#endif // CLASS_DB_H
