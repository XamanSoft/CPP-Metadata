#ifndef _METADATA_PROPERTY_HPP
#define _METADATA_PROPERTY_HPP

#include "member.hpp"

namespace MetaData {
	
template <typename T, typename Get, typename Set>
class Property: public Member
{
public:
	virtual Value& operator()();
};

}

#endif
