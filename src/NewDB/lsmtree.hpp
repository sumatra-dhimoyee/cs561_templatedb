#ifndef _TEMPLATEDB_SST_H_
#define _TEMPLATEDB_SST_H_

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "memcache.hpp"
#include "block.hpp"
#include "sst.hpp"

using namespace templatedb;
using namespace std;

class LSMTree {
private:
    int bufferSize;
    int totalLevel;
    int totalSST;
    int totalEntry;
    int tratio;
    int tierCount;

public:
    LSMTree(){
        
    }
    void createEntry();
    void flush();
    void compaction();
    

};

#endif