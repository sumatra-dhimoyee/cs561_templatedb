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

#include <vector>
template<typename K,typename V>
vector<Entry<K,V>> generate_vector(int low, int high)
{
    std::vector<Entry<int,string>> entries;
    for(int i = low; i < high;  i++)
    {

        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<int, string> temp = templatedb::Entry<int, string>(i, {to_string(i+1), to_string(i+2), to_string(i+3)}, false, TS);
        
        entries.push_back(temp);
    }
    return entries;

}
int main()
{

    
    

    std::vector<Entry<int,string>> entries_1 = generate_vector<int,string>(0,163);
    std::vector<Entry<int,string>> entries_2 = generate_vector<int,string>(200,363);
    std::vector<Entry<int,string>> entries_3 = generate_vector<int,string>(400,563);
    std::vector<Entry<int,string>> entries_4 = generate_vector<int,string>(600,763);
    std::vector<Entry<int,string>> entries_5 = generate_vector<int,string>(800,963);
    std::vector<Entry<int,string>> entries_6 = generate_vector<int,string>(1000,1163);
    std::vector<Entry<int,string>> entries_7 = generate_vector<int,string>(1200,1363);
    std::vector<Entry<int,string>> entries_8 = generate_vector<int,string>(1400,1563);
    std::vector<Entry<int,string>> entries_9 = generate_vector<int,string>(1600,1763);
    std::vector<Entry<int,string>> entries_10 = generate_vector<int,string>(1800,1963);
    std::vector<Entry<int,string>> entries_11 = generate_vector<int,string>(2000,2163);
    std::vector<Entry<int,string>> entries_12= generate_vector<int,string>(2200,2363);

// ----------------------------test leveled lsm -------------------------------------
/*
    templatedb::LSM<int,int> LeveledLSM = templatedb::LSM<int,int>(4*1024, 3, true, 1000, 10);
    LeveledLSM.create_sst(entries_1);
    LeveledLSM.create_sst(entries_2);
    LeveledLSM.create_sst(entries_3);
    LeveledLSM.create_sst(entries_4);
    LeveledLSM.create_sst(entries_5);
    LeveledLSM.create_sst(entries_6);
    LeveledLSM.create_sst(entries_7);
    LeveledLSM.create_sst(entries_8);
    LeveledLSM.create_sst(entries_9);
    LeveledLSM.create_sst(entries_10);
    LeveledLSM.create_sst(entries_11);
    LeveledLSM.create_sst(entries_12);
    
  
*/

// -----------------------------test tiered lsm --------------------------------------

templatedb::LSM<int,string> TieredLSM = templatedb::LSM<int,string>(4*1024, 3, false, 1000, 10);

    TieredLSM.create_sst(entries_1);
    TieredLSM.create_sst(entries_2);
    TieredLSM.create_sst(entries_3);
    // TieredLSM.create_sst(entries_4);
    // TieredLSM.create_sst(entries_5);
    // TieredLSM.create_sst(entries_6);
    std::cout<<TieredLSM.levels.size()<<std::endl;
    // std::cout<<TieredLSM.levels[i].<<std::endl;
    // int k = TieredLSM.levels[1].bloom_lookup(450);
    // int j = TieredLSM.levels[1].get_block_index(k, 450);
    // std::cout<<k<<std::endl;
    std::cout<<WRITE_IO<<std::endl;
    std::cout << "Size of str object: " << sizeof(int) << " bytes" << std::endl;

}
