#include "CentralCache.h"
#include "PageCaceh.h"

CentralCache CentralCache::_inst;

//// 打桩
//// 从中心缓存获取一定数量的对象给thread cache
//size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t n, size_t bytes)
//{
//	start = malloc(bytes * n);
//	end = (char*)start + bytes*(n-1);
//
//	void* cur = start;
//	while (cur <= end)
//	{
//		void* next = (char*)cur + bytes;
//		NEXT_OBJ(cur) = next;
//		
//		cur = next;
//	}
//
//	NEXT_OBJ(end) = nullptr;
//
//	return n;
//}

Span* CentralCache::GetOneSpan(SpanList* spanlist, size_t bytes)
{
	Span* span = spanlist->begin();
	while (span != spanlist->end())
	{
		if (span->_objlist != nullptr)
			return span;

		span = span->_next;
	}

	// 向pagecache申请一个新的合适大小的span
	size_t npage = ClassSize::NumMovePage(bytes);
	Span* newspan = PageCache::GetInstance()->NewSpan(npage);

	// 将span的内存切割成一个个bytes大小的对象挂起来
	char* start = (char*)(newspan->_pageid << PAGE_SHIFT);
	char* end = start + (newspan->_npage << PAGE_SHIFT);
	char* cur = start;
	char* next = cur + bytes;
	while (next < end)
	{
		NEXT_OBJ(cur) = next;
		cur = next;
		next = cur + bytes;
	}
	NEXT_OBJ(cur) = nullptr;
	newspan->_objlist = start;
	newspan->_objsize = bytes;
	newspan->_usecount = 0;

	// 将newspan插入到spanlist
	spanlist->PushFront(newspan);
	return newspan;
}


// 从中心缓存获取一定数量的对象给thread cache
size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t num, size_t bytes)
{
	size_t index = ClassSize::Index(bytes);
	SpanList* spanlist = &_spanlist[index];
	Span* span = GetOneSpan(spanlist, bytes);
	

	void* cur = span->_objlist;
	void* prev = cur;
	size_t fetchnum = 0;
	while (cur != nullptr && fetchnum < num)
	{
		prev = cur;
		cur = NEXT_OBJ(cur);
		++fetchnum;
	}
	
	start = span->_objlist;
	end = prev;
	NEXT_OBJ(end) = nullptr;

	span->_objlist = cur;
	span->_usecount += fetchnum;
	
	return fetchnum;
}

