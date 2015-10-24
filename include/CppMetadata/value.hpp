#ifndef _CPPMETADATA_VALUE_HPP
#define _CPPMETADATA_VALUE_HPP

namespace CppMetadata {

class Value
{
public:
    virtual const Type& type() const =0;
    
    virtual const Value& get() const =0;
    virtual void set(const Value& val) =0;
    
    virtual void release() =0;
};

namespace Runtime {
	template <typename Type>
	class Value
	{
	public:
		virtual const Type& type() const =0;
		
		virtual const Value& get() const =0;
		virtual void set(const Value& val) =0;
		
		virtual void release() =0;
	};
}
	
}

#endif
