#ifndef _CPPMETADATA_RUNTIME_OBJECT_HPP
#define _CPPMETADATA_RUNTIME_OBJECT_HPP

#define MD_OBJECT_CONCAT(X,Y) X##Y  // helper macro
#define MD_OBJECT_UNIQUE(X,Y) MD_OBJECT_CONCAT(X,Y)

#define MD_OBJECT_REGISTER(object) CppMetadata::Object* _md_object_new_##object(CppMetadata::Arguments const& args){ return new object(); } 
#define MD_OBJECT_REGISTER_ARGS(object) CppMetadata::Object* _md_object_new_##object(CppMetadata::Arguments const& args){ return new object(args); } 

#endif
