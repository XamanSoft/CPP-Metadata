#ifndef _METADATA_ARGUMENTS_HPP
#define _METADATA_ARGUMENTS_HPP

namespace MetaData {
	
struct Arguments {
	virtual ~Arguments(){}
	
	virtual bool empty() const =0;
	virtual size_t count() const =0;
	virtual Value& operator[](int index) const =0;
};

struct ArgumentsList: public Arguments {

    template<class... Args>
    Arguments(Args&&... tail)
    {
        Build(tail...);
    }
    
    virtual ~Arguments() {}
    
    template<class T, class... Args>
    void Build(T head, Args&&... tail) 
    { 
        contents.push_back(head); 
        Build(tail...);
    }

    template<class T>
    void Build(T head)
    { 
        contents.push_back(head); 
    }

    void Build() {}

    typedef std::vector<Value>::iterator iterator;
    typedef std::vector<Value>::const_iterator const_iterator;
    typedef std::vector<Value>::reverse_iterator reverse_iterator;
    typedef std::vector<Value>::const_reverse_iterator const_reverse_iterator;
    
    iterator begin()
    {
       return contents.begin();
    }
    
    iterator end()
    {
       return contents.end(); 
    }
    
    const_iterator begin() const
    {
       return contents.begin();
    }
    
    const_iterator end() const
    {
        return contents.end();
    }
    
    reverse_iterator rbegin()
    {
       return contents.rbegin();
    }
    
    reverse_iterator rend()
    {
        return contents.rend();
    }
    
    const_reverse_iterator rbegin() const
    {
       return contents.rbegin();
    }
    
    const_reverse_iterator rend() const
    {
        return contents.rend();
    }
    
    const_iterator cbegin() const
    {
       return contents.cbegin();
    }
    
    const_iterator cend() const
    {
        return contents.cend();
    }
    
    const_reverse_iterator crbegin() const
    {
       return contents.crbegin();
    }
    
    const_reverse_iterator crend() const
    {
        return contents.crend();
    }
	
	virtual bool empty() const 
	{
		return contents.empty();
	}
	
	virtual size_t count() const
	{
		return contents.size();
	}
    
    Value& operator[](int index) const
    {
        return contents[index];
    }

private:
    std::vector<Value> contents;
};

}

#endif
