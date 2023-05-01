#include "block.hpp"
#include "block.cpp"
#include "sst.hpp"
#include "sst.cpp"
#include "memcache.hpp"
#include "memcache.cpp"
#include <vector>



int main(){

    // ------------------------------WORKLOAD-----------------------------
    std::vector<Entry<int,int>> entries;
    size_t bufferSize= 8*1024*1024;

    for(int i = 0; i < 10; i = i+2)
    {

        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(i, {i+1, i+2, i+3}, false, TS);
        
        entries.push_back(temp);
    }
    // for(int i = 10; i < 20; i = i+2)
    // {

    //     auto TS = std::chrono::system_clock::now();
    //     templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(i, {i+1, i+2, i+3}, true, TS);
        
    //     entries.push_back(temp);
    // }
    // for(int i = 20; i < 220; i = i+2)
    // {

    //     auto TS = std::chrono::system_clock::now();
    //     templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(i, {i+1, i+2, i+3}, false, TS);
        
    //     entries.push_back(temp);
    // }

    templatedb::MemCache <int,int> memcache = templatedb::MemCache<int,int>(bufferSize, entries);
    memcache.printMemcache();


    return 0;

}
