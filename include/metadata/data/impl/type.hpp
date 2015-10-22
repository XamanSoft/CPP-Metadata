#ifndef _METADATA_DATA_IMPL_TYPE_HPP
#define _METADATA_DATA_IMPL_TYPE_HPP

#include "../type.hpp"

namespace Metadata {
namespace Data {
	
class Value;

template <typename Tp>
class TypeAny: public Type
{
public:
	virtual ~TypeAny();
	
	virtual char const* typeName() =0;
	virtual Value* create() =0;
	virtual bool isValueFromThisType(Value const& value) =0;
	virtual bool isType(Type const* type) =0;

	virtual bool operator==(Type const* type) const =0;
	virtual bool operator!=(Type const* type) const =0;
	virtual bool operator==(Type const& type) const =0;
	virtual bool operator!=(Type const& type) const =0;
	
protected:
	TypeAny();
	
private:
	TypeAny(const TypeAny&);
	TypeAny& operator=(const TypeAny &);
};

}
}

#endif
