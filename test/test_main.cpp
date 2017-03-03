#include "test.hpp"

int main()
{
	Test test;
	
	test.prop = 1;
	test.show(test.prop);
	test.prop = 3;
	test.show(test.prop);
	
	return 0;
}
