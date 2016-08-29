#include <iostream>
#include <string>
#include <stdarg.h>
using namespace std;

#include "List.hpp"

class SaveAdapter
{
public:
	virtual void Save(const char* format, ...) = 0;
};

// 控制台保存适配器
class ConsoleSaveAdapter : public SaveAdapter
{
public:
	virtual void Save(const char* format, ...)
	{
		va_list args;
		va_start (args, format);
		vfprintf (stdout, format, args);
		va_end (args);
	}
};

// 文件保存适配器
class FileSaveAdapter : public SaveAdapter
{
public:
	FileSaveAdapter(const char* filename = "MemoryManagerReport.txt")
	{
		_fout = fopen(filename, "w");
		if (_fout == NULL)
		{
			cout<<"open file failed"<<endl;
		}
	}

	virtual void Save(const char* format, ...)
	{
		va_list args;
		va_start (args, format);
		vfprintf (_fout, format, args);
		va_end (args);
	}
private:
	FILE* _fout;
};

struct MemoryBlockInfo
{
	void* _ptr;
	string _filename;
	int  _line;

	MemoryBlockInfo(void* ptr = NULL, const char* filename = "", int line = 0);
	bool operator==(const MemoryBlockInfo& x);

	friend ostream& operator<<(ostream& os, const MemoryBlockInfo& info);
};

class MemoryManager
{
public:
	static MemoryManager* GetInstance();
	void* Alloc (size_t size,
		const char* filename, int line);
	void Dealloc (void* ptr);

	static void SaveResult();
	void _SaveResult(SaveAdapter& sa);

	static void Print();

protected:
	void _Print();
	MemoryManager();

protected:
	static MemoryManager* _sInstance;
	List<MemoryBlockInfo> _infos;	// 存储内存块信息的容器
};

template<class T>
inline T* __NEW(size_t size, const char* filename, int line)
{
	return new(MemoryManager::GetInstance()->Alloc(sizeof(T), filename, line))T;
}

template<class T>
inline void __DELETE(T* ptr)
{
	ptr->~T();
	MemoryManager::GetInstance()->Dealloc(ptr);
}

template<class T>
inline T* __NEW_ARRAY(size_t num, const char* filename, int line)
{
	// 构造函数
	T* ptr = (T*)MemoryManager::GetInstance()->Alloc(sizeof(T)*num + 4, filename, line);

	*(int*)ptr = num;
	T* selfPtr = (T*)((char*)ptr + 4);
	for (size_t i = 0; i < num; ++i)
	{
		new(selfPtr + i)T;
	}

	return selfPtr;
}

template<class T>
inline void __DELETE_ARRAY(T* ptr)
{
	// 析构函数
	T* selfPtr = (T*)((char*)ptr - 4);
	size_t num = *(int*)selfPtr;
	for (size_t i = 0; i < num; ++i)
	{
		ptr[i].~T();
	}

	MemoryManager::GetInstance()->Dealloc(selfPtr);
}

// 管理内存对象
#define NEW(type)	\
	__NEW<type>(sizeof(type), __FILE__, __LINE__)

#define DELETE(ptr)	\
	__DELETE(ptr)

// 管理内存对象数组
#define NEW_ARRAY(type, num) \
	__NEW_ARRAY<type>(num, __FILE__, __LINE__)

#define DELETE_ARRAY(ptr) \
	__DELETE_ARRAY(ptr)
