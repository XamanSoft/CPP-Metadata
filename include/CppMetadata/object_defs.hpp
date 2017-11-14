#ifdef MD_OBJECT
#ifdef _MD_OBJECT_DECL
#undef _MD_OBJECT_DECL
#endif
#undef MD_OBJECT
#undef MD_OBJECT_PROPERTY
#undef MD_OBJECT_PROPERTY_GS
#undef MD_OBJECT_FUNCTION
#undef MD_OBJECT_FUNCTION_NP
#undef MD_OBJECT_FUNCTION_NR
#undef MD_OBJECT_FUNCTION_NR_NP
#undef MD_OBJECT_FUNCTION_BODY
#endif

#define MD_OBJECT(o_name) \
	private: \
	CppMetadata::Object* object{nullptr}; \
	public: \
	template <typename... params_type>\
	inline o_name(params_type... params) { \
		CppMetadata::Object* _md_object_new_##o_name(CppMetadata::Arguments const& args); \
		object = _md_object_new_##o_name(CppMetadata::Runtime::ArgumentsBuild(params...)); \
	} \
    virtual const CppMetadata::Object::ObjectInfo& objectInfo() const { return object->objectInfo(); } \
    virtual int memberCount() const { if (object) return object->memberCount(); return 0; };\
    virtual bool hasMember(const char* m_name) const { if (object) return object->hasMember(m_name); return false; };\
    virtual CppMetadata::Value& member(const char* m_name) const { return object->member(m_name); };\
    virtual CppMetadata::Value& operator[] (const char* m_name) const { return object->member(m_name); };\
    virtual ~o_name() { if (object) object->release(); } \
	virtual void release() { if (object) object->release(); object = nullptr; } \
    private:
	
#define MD_OBJECT_PROPERTY(type, name) CppMetadata::Runtime::Access::Property<type> name{this,#name};
#define MD_OBJECT_PROPERTY_GS(type, name, ...) CppMetadata::Runtime::Access::Property<type> name{this,#name};

#define MD_OBJECT_FUNCTION(type, name, ...) \
	CppMetadata::Runtime::Access::apply_args<CppMetadata::Runtime::Access::Function, type, type(__VA_ARGS__)>::Type name{this,#name}
	
#define MD_OBJECT_FUNCTION_BODY(body)
