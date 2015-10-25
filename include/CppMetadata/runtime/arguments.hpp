#ifndef _METADATA_RUNTIME_ARGUMENTS_HPP
#define _METADATA_RUNTIME_ARGUMENTS_HPP

#include <vector>
#include <iterator>

namespace CppMetadata {

namespace Runtime {

// Build and implements Arguments interface
struct ArgumentsBuild: public CppMetadata::Arguments {

    template<class... Args>
    ArgumentsBuild(Args&&... tail)
    {
        Build(tail...);
    }
    
    virtual ~ArgumentsBuild() { for (std::vector<CppMetadata::Value*>::iterator it = contents.begin(); it != contents.end(); it++) (*it)->release(); }
    
    template<class T, class... Args>
    void Build(T head, Args&&... tail) 
    { 
        contents.push_back(new Runtime::Value<T>(head)); 
        Build(tail...);
    }

    template<class T>
    void Build(T head)
    { 
        contents.push_back(new Runtime::Value<T>(head));
    }

    void Build() {}
	
	bool empty() const { return contents.empty(); }
	int count() const { return contents.size(); }
	CppMetadata::Value* get(int const& index) const { if (index >= contents.size()) return nullptr; return contents[index]; }
	
	void push(CppMetadata::Value* value)
	{
		contents.push_back(value);
	}

private:
    std::vector<CppMetadata::Value*> contents;
};

// Walk and interace with Arguments interface
class Arguments: public CppMetadata::Arguments
{
	struct ArgumentsNav
	{
		int currentPos;
		CppMetadata::Value* currentValue;
		int size;
		bool rev = false;
		
		CppMetadata::Arguments& arguments;
		
		ArgumentsNav(CppMetadata::Arguments& args, bool r = false): currentPos(r ? (args.count()-1) : 0), currentValue(args.get(r ? (args.count()-1) : 0)), size(args.count()), arguments(args), rev(r) {}
		
		// Overloading ++ for Pre-Increment
		CppMetadata::Value* operator++() {			
			if ( (rev && currentPos <= -1) || (!rev && currentPos >= size))
			{
				return nullptr;
			}
			
			currentPos = rev ? currentPos-1 : currentPos+1;
			return currentValue = arguments.get(currentPos);
		}

		// Overloading ++ for Post-Increment
		CppMetadata::Value* operator++(int) {
			CppMetadata::Value* value = nullptr;
			
			if (rev && currentPos > -1)
			{
				value = arguments.get(--currentPos);
			}
			else if (!rev && currentPos < size)
			{			
				value = arguments.get(++currentPos);
			}
			
			return currentValue = value;
		}
		
		void reset()
		{
			currentPos = rev ? size-1 : 0;
		}
		
		void reverse()
		{
			currentPos = rev ? 0 : size-1;
			rev = !rev;
		}
	};
	
	CppMetadata::Arguments& arguments;
	
public:
	class iterator: public std::iterator<std::input_iterator_tag, CppMetadata::Value*>
	{
		public:		
			iterator(CppMetadata::Arguments& arguments, bool rev = false) : arguments_nav(arguments, rev) { }
			iterator(const iterator& mit) : arguments_nav(mit.arguments_nav) {}
			iterator& operator++() { arguments_nav++; return *this; }
			iterator operator++(int) {iterator tmp(*this); arguments_nav++; return tmp;}
			bool operator==(const iterator& rhs) { return arguments_nav.currentValue == rhs.arguments_nav.currentValue; }
			bool operator!=(const iterator& rhs) { return arguments_nav.currentValue != rhs.arguments_nav.currentValue; }
			CppMetadata::Value*& operator*() { return arguments_nav.currentValue; }
			CppMetadata::Value** operator->() { return &arguments_nav.currentValue; }
		private:
			ArgumentsNav arguments_nav;
	};

	class const_iterator: public std::iterator<std::input_iterator_tag, CppMetadata::Value*>
	{
		public:
			const_iterator(CppMetadata::Arguments& arguments, bool rev = false) : arguments_nav(arguments, rev) { }
			const_iterator(const const_iterator& mit) : arguments_nav(mit.arguments_nav) {}
			const_iterator& operator++() { arguments_nav++; return *this; }
			const_iterator operator++(int) {const_iterator tmp(*this); arguments_nav++; return tmp;}
			bool operator==(const const_iterator& rhs) const { return arguments_nav.currentValue == rhs.arguments_nav.currentValue; }
			bool operator!=(const const_iterator& rhs) const { return arguments_nav.currentValue != rhs.arguments_nav.currentValue; }
			CppMetadata::Value* const & operator*() const { return arguments_nav.currentValue; }
			CppMetadata::Value* const * operator->() const  { return &arguments_nav.currentValue; }
		private:
			ArgumentsNav arguments_nav;
	};
	
	class reverse_iterator: public iterator
	{
		public:
			reverse_iterator(CppMetadata::Arguments& arguments, bool rev = true) : iterator(arguments, rev) { }
	};
	
	class const_reverse_iterator: public const_iterator
	{
		public:
			const_reverse_iterator(CppMetadata::Arguments& arguments, bool rev = true) : const_iterator(arguments, rev) { }
	};
	
	Arguments(CppMetadata::Arguments& args): arguments(args)
	{
	}
	
	virtual ~Arguments(){ }
	
	bool empty() const { return arguments.empty(); }
	int count() const { return arguments.count(); }
	int size() const { return arguments.count(); }
	CppMetadata::Value* get(int const& index) const { return arguments.get(index); }
	
	void push(CppMetadata::Value* value)
	{
		arguments.push(value);
	}
	
	iterator begin()
	{
		return iterator(arguments);
	}

	iterator end()
	{
		return iterator(arguments, true);
	}

	const_iterator begin() const
	{
		return const_iterator(arguments);
	}

	const_iterator end() const
	{
		return const_iterator(arguments, true);
	}

	reverse_iterator rbegin()
	{
		return reverse_iterator(arguments);
	}

	reverse_iterator rend()
	{
		return reverse_iterator(arguments, false);
	}

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator(arguments);
	}

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator(arguments, false);
	}

	const_iterator cbegin() const
	{
		return const_iterator(arguments);
	}

	const_iterator cend() const
	{
		return const_iterator(arguments, true);
	}

	const_reverse_iterator crbegin() const
	{
		return const_reverse_iterator(arguments);
	}

	const_reverse_iterator crend() const
	{
		return const_reverse_iterator(arguments, false);
	}
};

}

}

#endif
