/*
	Cache Simulator (Starter Code) by Justin Goins
	Oregon State University
	Spring Term 2021
*/

#ifndef _CACHESTUFF_H_
#define _CACHESTUFF_H_

#include <bitset>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <regex>
#include <sstream>
#include <climits>
#include <random>

using namespace std;

enum class ReplacementPolicy {
	Random,
	LRU
};

enum class WritePolicy {
	WriteThrough,
	WriteBack
};

// structure to hold information about a particular cache
struct CacheInfo {
	unsigned int numByteOffsetBits;
	unsigned int numSetIndexBits;
	unsigned int numberSets; // how many sets are in the cache
	unsigned int blockSize; // size of each block in bytes
	unsigned int associativity; // the level of associativity (N)
	ReplacementPolicy rp;
	WritePolicy wp;
	unsigned int cacheAccessCycles;
	unsigned int memoryAccessCycles;
};

// this structure can filled with information about each memory operation
struct CacheResponse {
	int hits; // how many caches did this memory operation hit?
	int misses; // how many caches did this memory operation miss?
	int evictions; // did this memory operation involve one or more evictions?
	int dirtyEvictions; // were any evicted blocks marked as dirty? (relevant for write-back cache)
	unsigned int cycles; // how many clock cycles did this operation take?
};

/* cache */
class Cache{
protected:
    std::vector<uint64_t> LRU_priority;
public:
    explicit Cache(CacheInfo &info);
    struct AddressInfo {
        unsigned long int tag;
        unsigned int setIndex;
    };
    AddressInfo addr_info{};
    CacheInfo _info{};
    // cache structure
    std::bitset<32> cache[USHRT_MAX]; /* O(1){} time complexity */
    bool isHit(const std::bitset<32>& adr);
    bool check_if_identical(const uint64_t  &index, const uint &_bit_tag,const  std::bitset<32> &lhs, const std::bitset<32> rhs[]);
    void HitHandler(const std::bitset<32>& addr);
    void subs(const std::bitset<32>& addr);
    uint64_t findCacheBlockIndex(const std::bitset<32>& addr);
};
#endif //CACHESTUFF
