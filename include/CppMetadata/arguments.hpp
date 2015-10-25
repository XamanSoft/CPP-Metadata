#ifndef _METADATA_ARGUMENTS_HPP
#define _METADATA_ARGUMENTS_HPP

namespace CppMetadata {
	
class Arguments {
public:
	virtual ~Arguments(){}
	
	virtual bool empty() const =0;
	virtual int count() const =0;
	virtual Value* get(int const& index) const =0;
	
	virtual void push(Value* value) =0;
};

}

#endif
