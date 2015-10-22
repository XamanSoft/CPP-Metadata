#ifndef _METADATA_VALUE_HPP
#define _METADATA_VALUE_HPP

namespace Metadata {
namespace Data {
	
class Type;

class Value
{
public:
	
	Type& type();
	
	template <typename T>
	Value (T data)
	{
		
	}
	
	template <typename T>
	T operator=(const T& value)
	{
		
	}
};

}
}

#endif
