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

    // for(int i = 0; i < 10; i = i+2)
    // {

    //     auto TS = std::chrono::system_clock::now();
    //     templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(i, {i+1, i+2, i+3}, false, TS);
        
    //     entries.push_back(temp);
    // }
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
    std::vector<int> val;
    val.push_back(22);
    memcache.put(9,val);
    std::vector<int> val1;
    val1.push_back(11);
    memcache.put(2,val1);
    std::vector<int> val2;
    val2.push_back(33);
    memcache.put(4, val2);
    std::vector<int> val3;
    val3.push_back(23);
    memcache.put(8,val3);
    std::vector<int> val4;
    val4.push_back(66);
    memcache.put(66,val4);
    memcache.printMemcache();
    memcache.deleteEntry(2);
    memcache.deleteEntry(3);
    std::vector<int> val5;
    val5.push_back(69);
    memcache.updateEntry(4, val5);
    memcache.updateEntry(2, val5);
    std::cout<<"After the delete "<<std::endl;
    memcache.printMemcache();


    return 0;

}
