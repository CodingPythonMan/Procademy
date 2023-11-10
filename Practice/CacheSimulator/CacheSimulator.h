#pragma once

#define WAY 8
#define CACHE_INDEX 64

// 64bit ����ȯ�游 ����Ѵ�.
struct TagArray {
	unsigned __int64 tags[WAY];
	int roundRobin;
};

extern TagArray tagArrays[64];

void CacheSimulate(void* pointer);

void PrintCacheInfo(unsigned char cacheIndex, unsigned __int64 tag, unsigned __int64 address);