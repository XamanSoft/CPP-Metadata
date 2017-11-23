#include <CppMetadata.hpp>
#include <iostream>
#include <string>

int main()
{
	CppMetadata::VM vm;
	std::string code;
	
	std::cout << "Enter JS Code: ";
	std::getline(std::cin, code);
	/*CppMetadata::Runtime::ValuePtr<double> num =*/ vm.execute(code.c_str(), CppMetadata::VM::IT_CODE);
	/*if (!num.isNull())
		std::cout << "value " << (double)num << std::endl;
	else
		std::cout << "Not a value" << std::endl;*/
	
	return 0;
}
