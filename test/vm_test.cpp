#include <CppMetadata.hpp>
#include <iostream>

int main()
{
	CppMetadata::VM vm;
	
	CppMetadata::Runtime::ValuePtr<double> num = vm.execute("function test() { return 123.456; }", CppMetadata::VM::IT_CODE);
	if (!num.isNull())
		std::cout << "value " << (double)num << std::endl;
	else
		std::cout << "Not a value" << std::endl;
	
	return 0;
}
