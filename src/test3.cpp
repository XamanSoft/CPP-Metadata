//Title of this code
//clang 3.6.0

#include <iostream>

class Value;

class Type
{
public:
    virtual int id() const =0;
    virtual char const* const name() const =0;
    virtual int size() const =0;
    
    virtual Value* createValue(const Value& value);
    //virtual Value* createValue(const Arguments& args);
};

#define MD_REGISTER_TYPE(type)

class Value
{
public:
    virtual const Type& type() const =0;
    
    virtual const Value& get() const =0;
    virtual void set(const Value& val) =0;
};

class Constant;
class Variable;

class Member
{
public:
    enum Type {
        MT_CONSTANT,
        MT_VARIABLE
    };
    
    virtual enum Type type() const =0;
    
    virtual Constant& toConstant() const =0;
    virtual Variable& toVariable() const =0;
};

class Object
{
public:
    struct ObjectInfo
    {
        const char* name;
        const Type* type;
        const Object* object;
    };
    
    virtual const ObjectInfo& objectInfo() const =0;
    virtual int memberCount() const =0;
    virtual bool hasMember(const char* m_name) const =0;
    virtual Member& member(const char* m_name) const =0;
};

#include <map>

#define MD_OBJECT(o_name) \
    private:\
    ObjectInfo o_info{ #o_name, nullptr, this }; \
    std::map<const char*,Member*> members;\
    \
    public: \
    const ObjectInfo& objectInfo() const { return o_info; } \
    int memberCount() const { return members.size(); };\
    bool hasMember(const char* m_name) const { return members.count(m_name)>0; };\
    Member& member(const char* m_name) const { return *members.at(m_name); };\
    Member& operator[] (const char* m_name) const { return *members.at(m_name); };\
    private:

class Test: public Object
{
    MD_OBJECT(Test);
};

int main()
{
    Test test;
    
    std::cout << "Hello, world!\n";
}