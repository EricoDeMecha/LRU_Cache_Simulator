/*
	Cache Simulator (Starter Code) by Justin Goins
	Oregon State University
	Spring Term 2021
*/

#include "CacheSimulator.h"

Simulator::Simulator(CacheInfo &ci, string tracefile) : Cache(ci) {
    // store the configuration info
    this->ci = ci;
    this->inputFile = std::move(tracefile);
    this->outputFile = this->inputFile + ".out";
    // compute the other cache parameters
    this->ci.numByteOffsetBits = log2(ci.blockSize);
    this->ci.numSetIndexBits = log2(ci.numberSets);
    // initialize the counters

    // create your cache structure
    // ...
    // manual test code to see if the cache is behaving properly
    // will need to be changed slightly to match the function prototype
    /*
    cacheAccess(false, 0);
    cacheAccess(false, 128);
    cacheAccess(false, 256);

    cacheAccess(false, 0);
    cacheAccess(false, 128);
    cacheAccess(false, 256);
    */
}


/*
	Starts reading the tracefile and processing memory operations.
*/
void Simulator::runTracefile() {
    cout << "Input tracefile: " << inputFile << endl;
    cout << "Output file name: " << outputFile << endl;

    // process each input line
    string line;
    // define regular expressions that are used to locate commands
    regex commentPattern("==.*");
    regex instructionPattern("I .*");
    regex loadPattern(" (L )(.*)(,)([[:digit:]]+)$");
    regex storePattern(" (S )(.*)(,)([[:digit:]]+)$");
    regex modifyPattern(" (M )(.*)(,)([[:digit:]]+)$");

    // open the output file
    ofstream outfile(outputFile);
    // open the output file
    ifstream infile(inputFile);
    // parse each line of the file and look for commands
    while (getline(infile, line)) {
        // these strings will be used in the file output
        string opString, activityString;
        smatch match; // will eventually hold the hexadecimal address string
        unsigned long int address;
        // create a struct to track cache responses
        CacheResponse response{};

        // ignore comments
        if (std::regex_match(line, commentPattern) || std::regex_match(line, instructionPattern)) {
            // skip over comments and CPU instructions
            continue;
        } else if (std::regex_match(line, match, loadPattern)) {
            cout << "Found a load op!" << endl;
            istringstream hexStream(match.str(2));
            hexStream >> std::hex >> address;
            outfile << match.str(1) << match.str(2) << match.str(3) << match.str(4);
            load(&response, false, address, stoi(match.str(4)));
            logEntry(outfile, &response);

        } else if (std::regex_match(line, match, storePattern)) {
            cout << "Found a store op!" << endl;
            istringstream hexStream(match.str(2));
            hexStream >> std::hex >> address;
            outfile << match.str(1) << match.str(2) << match.str(3) << match.str(4);
            store(&response, true, address, stoi(match.str(4)));
            logEntry(outfile, &response);
        } else if (std::regex_match(line, match, modifyPattern)) {
            cout << "Found a modify op!" << endl;
            istringstream hexStream(match.str(2));
            // first process the read operation
            hexStream >> std::hex >> address;
            outfile << match.str(1) << match.str(2) << match.str(3) << match.str(4);
            modify(&response, false, address, stoi(match.str(4)));
            logEntry(outfile, &response);
            outfile << endl;
            // now process the write operation
            hexStream >> std::hex >> address;
            outfile << match.str(1) << match.str(2) << match.str(3) << match.str(4);
            store(&response, true, address, stoi(match.str(4)));
            logEntry(outfile, &response);
        } else {
            throw runtime_error("Encountered unknown line format in tracefile.");
        }
        outfile << endl;
    }
    // add the final cache statistics
    outfile << "Hits: " << Hits << " Misses: " << Misses
            << " Evictions: " << Evictions << endl;
    outfile << "Cycles: " << Cycles << endl;

    infile.close();
    outfile.close();
}

/*
	Report the results of a memory access operation.
*/
void Simulator::logEntry(ofstream& outfile, CacheResponse* response) {
    outfile << " " << response->cycles;
    if (response->hits > 0)
        outfile << " hit";
    if (response->misses > 0)
        outfile << " miss";
    if (response->evictions > 0)
        outfile << " eviction";
}


/*
	Calculate the block index and tag for a specified address.
*/
Simulator::AddressInfo Simulator::getAddressInfo(unsigned long int address) {
    AddressInfo ai{};
    // this code should be changed to assign the proper index and tag

    /*Get and set operations*/
    if(isHit(address)){
        HitHandler(address);
        Hits++;
    }else{
        subs(address);
        HitHandler(address);
        Misses++;
    }
    return ai;
}

/*Load */
void Simulator::load(CacheResponse* response, bool isWrite, unsigned long int address, int numBytes) {
    // determine the index and tag
    AddressInfo ai = getAddressInfo(address);

    cout << "\tSet index: " << ai.setIndex << ", tag: " << ai.tag << endl;
    // your code should also calculate the proper number of cycles that were used for the operation
    response->cycles = 0;

    // your code needs to update the global counters that track the number of hits, misses, and evictions

    if (response->hits > 0)
        cout << "Operation at address " << std::hex << address << " caused " << response->hits << " hit(s)." << std::dec << endl;
    if (response->misses > 0)
        cout << "Operation at address " << std::hex << address << " caused " << response->misses << " miss(es)." << std::dec << endl;

    cout << "-----------------------------------------" << endl;

    return;
}

void Simulator::store(CacheResponse* response, bool isWrite, unsigned long int address, int numBytes) {
    // determine the index and tag
    AddressInfo ai = getAddressInfo(address);

    cout << "\tSet index: " << ai.setIndex << ", tag: " << ai.tag << endl;

    // your code should also calculate the proper number of cycles that were used for the operation
    response->cycles = 0;

    // your code needs to update the global counters that track the number of hits, misses, and evictions

    if (response->hits > 0)
        cout << "Operation at address " << std::hex << address << " caused " << response->hits << " hit(s)." << std::dec << endl;
    if (response->misses > 0)
        cout << "Operation at address " << std::hex << address << " caused " << response->misses << " miss(es)." << std::dec << endl;

    cout << "-----------------------------------------" << endl;

    return;
}
void Simulator::modify(CacheResponse* response, bool isWrite, unsigned long int address, int numBytes) {
    // determine the index and tag
    AddressInfo ai = getAddressInfo(address);

    cout << "\tSet index: " << ai.setIndex << ", tag: " << ai.tag << endl;

    // your code should also calculate the proper number of cycles that were used for the operation
    response->cycles = 0;

    // your code needs to update the global counters that track the number of hits, misses, and evictions

    if (response->hits > 0)
        cout << "Operation at address " << std::hex << address << " caused " << response->hits << " hit(s)." << std::dec << endl;
    if (response->misses > 0)
        cout << "Operation at address " << std::hex << address << " caused " << response->misses << " miss(es)." << std::dec << endl;

    cout << "-----------------------------------------" << endl;

    return;
}