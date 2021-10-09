#ifndef _CACHESIMULATOR_H_
#define _CACHESIMULATOR_H_

#include "CacheStuff.h"

struct  Counter{
    unsigned int Cycles;
    unsigned int Hits;
    unsigned int Misses;
    unsigned int Evictions;

    // initialize the counter variables
    explicit Counter(): Cycles(0), Hits(0), Misses(0), Evictions(0){}
};

class Simulator : public Cache, Counter {
public:
    struct AddressInfo {
        unsigned long int tag;
        unsigned int setIndex;
    };
    Simulator(CacheInfo &ci, string tracefile);
    void runTracefile();

private:
    std::string inputFile, outputFile;

    CacheInfo ci{};

    // function to allow read or write access to the cache
    void load(CacheResponse*, bool, unsigned long int, int);
    void store(CacheResponse*, bool, unsigned long int, int);
    void modify(CacheResponse*, bool, unsigned long int, int);
    // function that can compute the index and tag matching a specific address
    AddressInfo getAddressInfo(unsigned long int);
    // function to add entry into output file
    void logEntry(std::ofstream&, CacheResponse*);
};

#endif //CACHESIMULATOR
