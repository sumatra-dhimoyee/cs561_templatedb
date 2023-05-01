#include "block.hpp"
#include "block.cpp"
#include "sst.hpp"
#include "sst.cpp"
#include "level.hpp"
#include "level.cpp"
#include "BloomFilter/BloomFilter.h"
#include "BloomFilter/BloomFilter.cpp"
#include "BloomFilter/murmurhash.cpp"
#include "BloomFilter/murmurhash.h"

#include <vector>
int main()
{

// ------------------------------WORKLOAD-----------------------------
    std::vector<Entry<int,int>> entries;
    for(int i = 0; i < 10;  i=i+2)
    {

        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(i, {i+1, i+2, i+3}, false, TS);
        
        entries.push_back(temp);
    }
     for(int i = 10; i < 20;  i=i+2)
    {

        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(i, {i+1, i+2, i+3}, true, TS);
        
        entries.push_back(temp);
    }
    for(int i = 20; i < 220;  i=i+2)
    {

        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(i, {i+1, i+2, i+3}, false, TS);
        
        entries.push_back(temp);
    }
    std::vector<zone<int>> fp;
    BF::BloomFilter bf = BF::BloomFilter(1000, 10);
    templatedb::Build_SST<int, int> builder = templatedb::Build_SST<int, int>(entries, 8*1024, 0, fp, bf);
    //-------------------------test bloom filter-------------------
    // for(int i = 0 ; i < 230; i++)
    // {
    //     bool query_result = bf.query(std::to_string(i));
    //     if (!query_result) {
    //     std::cout<< i<<" IS NOT PRESENT IN THE FILTER"<< std::endl;
    //     }
        
    // }

    //-----------------print elements to see if they are added correctly--------------------
    
    SST<int,int> sst = builder.build();
    std::cout << "BLOCK VECTOR SIZE" << sst.block_vector.size()<<std::endl;
    builder.print_block(sst.block_vector[0]);
    templatedb::Level<int,int> lvl = templatedb::Level<int,int>(sst, this->no_runs, level_size, i, fp, bf); 

    // builder.print_block(builder.block_vector[1]);


    //------------------check if overflowed. Should overflow for the above data--------------


    // if (builder.overflow)
    //     std::cout<<"OVERFLOWED!!!"<<std::endl;
    // std::cout << "BLOCK VECTOR SIZE" << builder.block_vector.size()<<std::endl;

    //--------------------------------test build function---------------------------------

/*
    SST<int,int> sst = builder.build();

    std::vector<Entry<int,int>> entries_2;
     for(int i = 10; i < 20; i ++)
    {

        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(i, {i+2, i+4, i+6}, true, TS);
        
        entries_2.push_back(temp);
    }
    for(int i = 230; i < 330; i ++)
    {

        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(i, {i+1, i+2, i+3}, false, TS);
        
        entries_2.push_back(temp);
    }

   
    for(int i = 330; i < 420; i ++)
    {

        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(i, {i+2, i+4, i+6}, false, TS);
        entries_2.push_back(temp);

        
    }

    std::vector<zone<int>> fp_2;
    BF::BloomFilter bf_2 = BF::BloomFilter(1000, 10);
    templatedb::Build_SST<int, int> builder_2 = templatedb::Build_SST<int, int>(entries_2, 8*1024, 0, fp_2, bf_2);
     
    SST<int,int> sst_2 = builder_2.build();
    std::cout<<"ITS TRUE"<<sst_2.block_vector[0].data[0].tomb<<std::endl;

    std::vector<zone<int>> fp_3;
    BF::BloomFilter bf_3 = BF::BloomFilter(1000, 10);
    SST<int,int> sst_3 = templatedb::Build_SST<int, int>::merge_sst(sst, sst_2, fp_3, bf_3);

    for(int i = 0 ; i < 450; i++)
    {
        bool query_result = bf_3.query(std::to_string(i));
        if (!query_result) {
        std::cout<< i<<" IS NOT PRESENT IN THE FILTER"<< std::endl;
        }
        
    }
*/

}
