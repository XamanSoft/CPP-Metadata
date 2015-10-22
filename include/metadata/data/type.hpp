#ifndef _METADATA_DATA_TYPE_HPP
#define _METADATA_DATA_TYPE_HPP

namespace Metadata {
namespace Data {

class Value;

class Type
{
public:
	virtual ~Type();
	
	virtual char const* typeName() =0;
	virtual Value* create() =0;
	virtual bool isValueFromThisType(Value const& value) =0;
	virtual bool isType(Type const* type) =0;

	virtual bool operator==(Type const* type) const =0;
	virtual bool operator!=(Type const* type) const =0;
	virtual bool operator==(Type const& type) const =0;
	virtual bool operator!=(Type const& type) const =0;
	
protected:
	Type();
	
private:
	Type(const Type&);
	Type& operator=(const Type &);
};

bool registerType(char const* type, Type& tpValueFactory);
Type* getType(char const* typeName);

}
}

#endif
