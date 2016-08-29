#include "MemoryManager.h"

void Fun(char* p)
{
	p = new char[20];
}

void Test()
{
	char* src = NULL;
	Fun(src);

	strcpy(src, "change world!");
	cout<<src<<endl;

	delete[] src;
}

void Test1()
{
	List<int> l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);
	l.PushBack(5);

	l.Print();

	Node<int>* ret = l.Find(1);
	l.Erase(ret);

	ret = l.Find(4);
	l.Erase(ret);

	ret = l.Find(5);
	l.Erase(ret);

	ret = l.Find(3);
	l.Erase(ret);

	ret = l.Find(2);
	l.Erase(ret);

	l.Print();
}

class String
{
public:
	String(const char* ptr = "")
		:_ptr(new char[strlen(ptr) + 1])
	{
		cout<<"String()"<<endl;

		strcpy(_ptr, ptr);
	}

	~String()
	{
		cout<<"~String()"<<endl;

		if (_ptr)
		{
			delete[] _ptr;
		}
	}
private:
	char* _ptr;
};

void Test2()
{
	int* p1 = NEW(int);
	char* p2 = NEW(char);

	int* p3 = NEW_ARRAY(int, 5);
	char* p4 = NEW_ARRAY(char,5);

	String* p5 = NEW(String);
	String* p6 = NEW_ARRAY(String,5);

	DELETE(p1);
	//DELETE(p2);
	DELETE(p5);

	DELETE_ARRAY(p3);
	DELETE_ARRAY(p4);
	//DELETE_ARRAY(p6);
}

// atexit 注册一个函数，让这个函数在main结束以后执行。


class Singleton
{
public:
	static Singleton* GetInstance()
	{
		if(_sInstance == NULL)
		{
			_sInstance = new Singleton();
		}

		return _sInstance;
		//return new Singleton();
	}

	void Print1()
	{
		cout<<"xxxx"<<endl;
	}

	void Print2()
	{
		cout<<"xxxx"<<endl;
	}

private:
	Singleton()
	{}

	//?
	Singleton(const Singleton& s)
	{}

	static Singleton* _sInstance;
};

Singleton* Singleton::_sInstance = NULL;

#include <map>

int main()
{
	//Test1();
	//Test2();

	multimap<char,int> first;
	multimap<char,int>::const_reverse_iterator

	first.insert(pair<char,int>('a',10));
	first.insert(pair<char,int>('b',15));
	first.insert(pair<char,int>('b',20));
	first.insert(pair<char,int>('c',25));


	return 0;
}