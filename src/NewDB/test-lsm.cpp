#include "block.hpp"
#include "block.cpp"
#include "sst.hpp"
#include "sst.cpp"
#include "level.hpp"
#include "level.cpp"
#include "lsm.hpp"
#include "lsm.cpp"
#include "BloomFilter/BloomFilter.h"
#include "BloomFilter/BloomFilter.cpp"
#include "BloomFilter/murmurhash.cpp"
#include "BloomFilter/murmurhash.h"
#include "memcache.hpp"
#include "memcache.cpp"

#include <vector>
template<typename K,typename V>
vector<Entry<K,V>> generate_vector(int low, int high)
{
    std::vector<Entry<string,string>> entries;
    for(int i = low; i < high;  i++)
    {

        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<string, string> temp = templatedb::Entry<string, string>(to_string(i), {to_string(i+1), to_string(i+2), to_string(i+3)}, false, TS);
        
        entries.push_back(temp);
    }
    return entries;

}
int main()
{

    
    

    std::vector<Entry<string,string>> entries_1 = generate_vector<string,string>(0,163);
    std::vector<Entry<string,string>> entries_2 = generate_vector<string,string>(200,363);
    std::vector<Entry<string,string>> entries_3 = generate_vector<string,string>(400,563);
    std::vector<Entry<string,string>> entries_4 = generate_vector<string,string>(600,763);
    std::vector<Entry<string,string>> entries_5 = generate_vector<string,string>(800,963);
    std::vector<Entry<string,string>> entries_6 = generate_vector<string,string>(1000,1163);
    std::vector<Entry<string,string>> entries_7 = generate_vector<string,string>(1200,1363);
    std::vector<Entry<string,string>> entries_8 = generate_vector<string,string>(1400,1563);
    std::vector<Entry<string,string>> entries_9 = generate_vector<string,string>(1600,1763);
    std::vector<Entry<string,string>> entries_10 = generate_vector<string,string>(1800,1963);
    std::vector<Entry<string,string>> entries_11 = generate_vector<string,string>(2000,2163);
    std::vector<Entry<string,string>> entries_12= generate_vector<string,string>(2200,2363);

// ----------------------------test leveled lsm -------------------------------------

    templatedb::LSM<string, string> LeveledLSM = templatedb::LSM<string,string>(4*1024, 3, true, 1000, 10);
    for(int i = 0 ; i< entries_1.size(); i++)
    {
        LeveledLSM.put(entries_1[i].key, entries_1[i].value);
    }
    // LeveledLSM.create_sst(entries_1);
     for(int i = 0 ; i< entries_2.size(); i++)
    {
        LeveledLSM.put(entries_2[i].key, entries_2[i].value);
    }
    for(int i = 0 ; i< entries_3.size(); i++)
    {
        LeveledLSM.put(entries_3[i].key, entries_3[i].value);
    }
    for(int i = 0 ; i< entries_4.size(); i++)
    {
        LeveledLSM.put(entries_4[i].key, entries_4[i].value);
    }
    for(int i = 0 ; i< entries_5.size(); i++)
    {
        LeveledLSM.put(entries_5[i].key, entries_5[i].value);
    }
    // LeveledLSM.create_sst(entries_1);
    // LeveledLSM.create_sst(entries_2);
    // LeveledLSM.create_sst(entries_3);
    // LeveledLSM.create_sst(entries_4);
    // LeveledLSM.create_sst(entries_5);
    // LeveledLSM.create_sst(entries_6);
    // LeveledLSM.create_sst(entries_7);
    // LeveledLSM.create_sst(entries_8);
    // LeveledLSM.create_sst(entries_9);
    // LeveledLSM.create_sst(entries_10);
    // LeveledLSM.create_sst(entries_11);
    // LeveledLSM.create_sst(entries_12);


    // LeveledLSM.put(entries_1[0].key, entries_1[0].value);
    // LeveledLSM.put(entries_1[0].key, entries_1[0].value);
    std::vector<string> tempVal;
    bool flag= true;
    //LeveledLSM.pointQuery(405, tempVal, flag);
    // std::cout<<"HERE"<<std::endl;
    // LeveledLSM.deleteQuery("405");
    // std::cout<<"HERE1"<<std::endl;
    LeveledLSM.pointQuery("2", tempVal, flag);
    //LeveledLSM.deleteQuery("602");
    //LeveledLSM.pointQuery("602", tempVal, flag);
    LeveledLSM.rangeQuery("601", "609");
    //LeveledLSM.deleteRangeQuery("601", "609");
    if(flag)
    {
        for(int i=0; i<tempVal.size(); i++){
        std::cout << "Element at Key 136 is " << tempVal[i] << std::endl;
    }
    }
    



// // -----------------------------test tiered lsm --------------------------------------

//  templatedb::LSM<int,int> TieredLSM = templatedb::LSM<int,int>(4*1024, 3, false, 1000, 10);

//     TieredLSM.create_sst(entries_1);
//     TieredLSM.create_sst(entries_2);
//     TieredLSM.create_sst(entries_3);
//     // TieredLSM.create_sst(entries_4);
//     // TieredLSM.create_sst(entries_5);
//     // TieredLSM.create_sst(entries_6);
//     std::cout<<TieredLSM.levels.size()<<std::endl;
//     // std::cout<<TieredLSM.levels[i].<<std::endl;
//     // int k = TieredLSM.levels[1].bloom_lookup(450);
//     // int j = TieredLSM.levels[1].get_block_index(k, 450);
//     // std::cout<<k<<std::endl;
//     std::cout<<WRITE_IO<<std::endl;

}