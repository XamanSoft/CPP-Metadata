#include <CppMetadata.hpp>
#include <iostream>

int main()
{
	CppMetadata::VM vm;
	
	CppMetadata::Runtime::ValuePtr<double> num = vm.execute("(function() { return 123; })()", CppMetadata::VM::IT_CODE);
	std::cout << "value " << (double)num << std::endl;
	
	return 0;
}
