#include <string>
#include <iostream>

#ifdef __GNUG__
#include <cxxabi.h>
#endif

template <typename T>
struct Test
{
    std::string name()
    {
#if defined(__GXX_RTTI) || defined(_CPPRTTI)
        static std::string n;

        if (n.empty())
        {
            n = typeid(T).name();

#ifdef __GNUG__
            char * name = 0;
            int status;
            
            name = abi::__cxa_demangle(n.c_str(), 0, 0, &status);
            if (name != 0) {
                n = name;
            }
#elif _MSC_VER
            int pos = sname.find(" ");
			if (pos > 0)
				sname = sname.substr(pos+1);
#endif
        }
        
        return n;
#else
        return std::string();
#endif
    }
};

#define TEST_ENABLE_TYPE(T) \
template <> \
struct Test<T> \
{ \
    std::string name() \
    { \
        static std::string n = #T; \
        return n; \
    } \
};

TEST_ENABLE_TYPE(int);
TEST_ENABLE_TYPE(float);
TEST_ENABLE_TYPE(double);

class TestClass;
TEST_ENABLE_TYPE(TestClass);

class TestClass: public Test<TestClass>
{
    std::string const n = std::string("TestClass");
};

int main()
{
    std::cout << Test<int>().name() << std::endl;
    std::cout << Test<float>().name() << std::endl;
    std::cout << Test<double>().name() << std::endl;
    std::cout << Test<char>().name() << std::endl;
    std::cout << TestClass().name() << std::endl;
    return 0;
}
