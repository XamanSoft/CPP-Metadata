#ifndef _METADATA_MEMBERS_BASE_HPP
#define _METADATA_MEMBERS_BASE_HPP

#include "arguments.hpp"

namespace MetaData {
namespace Members {
	
class Base
{
public:
	enum Type {
		TP_ERROR,
		TP_VOID,
		TP_PROPERTY,
		TP_FUNCTION
	};

	virtual enum Type type() const =0;
	virtual char const* name() const =0;
	virtual int size() const =0;
	
	virtual Value* operator()(Arguments const& args = Arguments()) =0;
	virtual Value* operator=(Value const& value) =0;
};

}
}

#endif
