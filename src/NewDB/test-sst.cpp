#include "block.hpp"
#include "block.cpp"
#include "sst.hpp"
#include "sst.cpp"

#include <vector>
int main()
{

// ------------------------------WORKLOAD-----------------------------
    std::vector<Entry<int,int>> entries;
    for(int i = 0; i < 10; i = i+2)
    {

        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(i, {i+1, i+2, i+3}, false, TS);
        
        entries.push_back(temp);
    }
     for(int i = 10; i < 20; i = i+2)
    {

        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(i, {i+1, i+2, i+3}, true, TS);
        
        entries.push_back(temp);
    }
    for(int i = 20; i < 220; i = i+2)
    {

        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(i, {i+1, i+2, i+3}, false, TS);
        
        entries.push_back(temp);
    }

    templatedb::Build_SST<int, int> builder = templatedb::Build_SST<int, int>(entries, 8*1024, 0, 0);

    std::cout << "BLOCK VECTOR SIZE" << builder.block_vector.size()<<std::endl;

    //-----------------print elements to see if they are added correctly--------------------


    // builder.print_block(builder.block_vector[0]);
    // builder.print_block(builder.block_vector[1]);


    //------------------check if overflowed. Should overflow for the above data--------------


    // if (builder.overflow)
    //     std::cout<<"OVERFLOWED!!!"<<std::endl;
    // std::cout << "BLOCK VECTOR SIZE" << builder.block_vector.size()<<std::endl;

    //--------------------------------test build function---------------------------------


    SST<int,int> sst = builder.build();

    std::vector<Entry<int,int>> entries_2;
     for(int i = 10; i < 20; i ++)
    {

        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(i, {i+2, i+4, i+6}, false, TS);
        
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

    std::cout<<"ITS TRUE"<<entries_2[0].tomb<<std::endl;
    templatedb::Build_SST<int, int> builder_2 = templatedb::Build_SST<int, int>(entries_2, 8*1024, 0, 0);

    SST<int,int> sst_2 = builder_2.build();

    SST<int,int> sst_3 = templatedb::Build_SST<int, int>::merge_flush(sst, sst_2);


}
