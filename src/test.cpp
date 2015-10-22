#include <list>
#include <string>
#include <iostream>

struct Value {
    virtual ~Value() {}
    virtual std::string ToString() const =0;
    virtual void Show() const =0;
};

template <typename Tp>
struct Values: public Value {
    virtual ~Values() {}
    
    Values(Tp V): Val(V) {}
    Tp Val;
    
    virtual std::string ToString() const { return std::string(); /* ToDo w/ sstream */ }
    virtual void Show() const { std::cout << Val << std::endl; }
};

struct Arguments {

    template<class... Args>
    Arguments(Args&&... tail)
    {
        Build(tail...);
    }
    
    static bool deleteAll(Value * value) { delete value; return true; }
    
    virtual ~Arguments()
    {
        contents.remove_if(deleteAll);
    }
    
    template<class T, class... Args>
    void Build(T head, Args&&... tail) 
    { 
        contents.push_back(new Values<T>(head)); 
        Build(tail...);
    }

    template<class T>
    void Build(T head)
    { 
        contents.push_back(new Values<T>(head)); 
    }

    void Build() {}
    
    operator std::list<Value*>() { return contents; }

    std::list<Value*> contents;
};

void test(const std::list<Value*> args = std::list<Value*>())
{
    for (auto& arg : args) {
        arg->Show();
    }
}

template <typename... Args>
void foo(Args&&... args)
{
    test(Arguments(args...));
}

int main()
{
    foo(0, 1, 2.19, 3, 4, 5.2);
    return 0;
}