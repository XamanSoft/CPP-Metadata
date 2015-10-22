#ifndef _METADATA_OBJECT_HPP
#define _METADATA_OBJECT_HPP

#include <string>
#include <member.hpp>

namespace MetaData {
	
class Object
{
public:
	Object(const std::string &objectName);
	virtual ~Object();
	
	int memberCount();
	Member& operator[](const std::string& name);

protected:
	std::map<std::string,Member*> members;
	std::string objectName;
};
	
}

#endif
