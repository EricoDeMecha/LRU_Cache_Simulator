
#include <utility>

#include "CacheSimulator.h"
/*
	This function creates the cache and starts the simulator.
	Accepts core ID number, configuration info, and the name of the tracefile to read.
*/
void initializeCache(int id, CacheInfo config, string tracefile) {
    Simulator singlecore = Simulator( config, std::move(tracefile));
    singlecore.runTracefile();
}

/*
	This function accepts a configuration file and a trace file on the command line.
	The code then initializes a cache simulator and reads the requested trace file(s).
*/
int main(int argc, char* argv[]) {
    CacheInfo config{};
    if (argc < 3) {
        perror("Usage: # example invocation of cache simulator\n"
               "cache_sim ./resources/testconfig ./resources/simpletracefile\n"
               "Output file written to ./resources/simpletracefile.out\n");
        return 1;
    }

    // determine how many cache levels the system is using
    unsigned int numCacheLevels;

    // read the configuration file
    cout << "Reading config file: " << argv[1] << endl;
    ifstream infile(argv[1]);
    unsigned int tmp;
    infile >> numCacheLevels;
    infile >> config.memoryAccessCycles;
    infile >> config.numberSets;
    infile >> config.blockSize;
    infile >> config.associativity;
    infile >> tmp;
    config.rp = static_cast<ReplacementPolicy>(tmp);
    infile >> tmp;
    config.wp = static_cast<WritePolicy>(tmp);
    infile >> config.cacheAccessCycles;
    infile.close();

    // Examples of how you can access the configuration file information
    cout << "System has " << numCacheLevels << " cache(s)." << endl;
    cout << config.numberSets << " sets with " << config.blockSize << " bytes in each block. N = " << config.associativity << endl;

    if (config.rp == ReplacementPolicy::Random)
        cout << "Using random replacement protocol" << endl;
    else
        cout << "Using LRU protocol" << endl;

    if (config.wp == WritePolicy::WriteThrough)
        cout << "Using write-through policy" << endl;
    else
        cout << "Using write-back policy" << endl;

    // start the cache operation...
    string tracefile(argv[2]);
    initializeCache(0, config, tracefile);

    return 0;
}
