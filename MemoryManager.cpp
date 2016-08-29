#include "MemoryManager.h"

MemoryManager* MemoryManager::_sInstance = NULL;

MemoryBlockInfo::MemoryBlockInfo(void* ptr, const char* filename, int line)
		:_ptr(ptr)
		,_filename(filename)
		,_line(line)
{}

bool MemoryBlockInfo::operator==(const MemoryBlockInfo& x)
{
	return _ptr == x._ptr;
}

ostream& operator<<(ostream& os, const MemoryBlockInfo& info)
{
	os<<"Ptr->"<<info._ptr<<endl;
	os<<"Filename:"<<info._filename<<endl;
	os<<"Line:"<<info._line<<endl;

	return os;
}

MemoryManager* MemoryManager::GetInstance()
{
	if (_sInstance == NULL)
	{
		_sInstance = new MemoryManager();
	}

	return _sInstance;
}

void* MemoryManager::Alloc (size_t size,
		const char* filename, int line)
{
	// 申请内存
	void* ptr = malloc(size);

	// 记录内存块信息
	if (ptr)
	{
		_infos.PushBack(MemoryBlockInfo(ptr, filename, line));
	}

	return ptr;
}

void MemoryManager::Dealloc (void* ptr)
{
	if (ptr)
	{
		Node<MemoryBlockInfo>* ret = _infos.Find(MemoryBlockInfo(ptr));
		_infos.Erase(ret);
	}

	return free(ptr);
}

void MemoryManager::Print()
{
	MemoryManager::GetInstance()->_Print();
}

void MemoryManager::_Print()
{
	_infos.Print();
}

MemoryManager::MemoryManager()
{
	//atexit(Print);
	atexit(SaveResult);
}

void MemoryManager::SaveResult()
{
	GetInstance()->_SaveResult(ConsoleSaveAdapter());
	GetInstance()->_SaveResult(FileSaveAdapter());
}

void MemoryManager::_SaveResult(SaveAdapter& sa)
{
	Node<MemoryBlockInfo>* begin = _infos._head;
	while(begin)
	{
		sa.Save("Ptr: %p\nFilename:%s\nline:%d\n\n"
			,begin->_data._ptr
			,begin->_data._filename.c_str()
			,begin->_data._line);

		begin = begin->_next;
	}
}




