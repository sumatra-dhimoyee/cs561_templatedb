#include "block.hpp"
#include "block.cpp"
#include <vector>
int main()
{

    // create a Build_Block object
    
    std::vector<Entry<int,int>> data;
    std::vector<int> keys;
    keys.push_back(1);
    keys.push_back(2);
    keys.push_back(3);
    keys.push_back(43);
    std::vector<std::vector<int> > values;
    values.emplace_back(std::initializer_list<int>{24, 46, 37, 42});
    values.emplace_back(std::initializer_list<int>{103, 509, 21, 455});
    values.emplace_back(std::initializer_list<int>{55, 66, 77,  88});
    values.emplace_back(std::initializer_list<int>{1,2,3,});
  

    for (int i = 0; i <keys.size(); i++)
    {
        auto TS = std::chrono::system_clock::now();
        
        templatedb::Entry<int, int> entry = templatedb::Entry<int, int>(keys[i], values[i], false, TS);
        data.push_back(entry);
    }
      
    size_t size = 0;
    for (int i = 0; i<data.size(); i++)
    {
        size_t temp_size = (sizeof(int)+sizeof(std::chrono::time_point<std::chrono::system_clock>) + sizeof(bool));
        temp_size = temp_size +data[i].value.size()*sizeof(int);
        std::cout<<"SIZE OF "<<i<<" "<<temp_size<<std::endl;
        size = size +temp_size;
    }
    std::cout<<size<<std::endl;
    templatedb::Build_Block<int, int> builder = templatedb::Build_Block<int, int>(data);
    auto TS = std::chrono::system_clock::now();
    templatedb::Entry<int, int> entry_one = templatedb::Entry<int, int>(43, {1, 2, 3}, false, TS);
    TS = std::chrono::system_clock::now();
    templatedb::Entry<int, int> entry_two = templatedb::Entry<int, int>(56, {4,5,6,7}, false, TS);
    TS = std::chrono::system_clock::now();
    templatedb::Entry<int, int> entry_three = templatedb::Entry<int, int>(71, {6}, false, TS);

    
    //add some key-value pairs to the builder
    builder.add_KV(entry_one);
    builder.add_KV(entry_two);
    builder.add_KV(entry_three);
    builder.print_block();

    //---------------------test add_to_block function--------------------
    /*
    std::vector<Entry<int,int>> entries;
    for(int i = 0; i < 220; i ++)
    {
        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(43, {1, 2, 3}, false, TS);
        entries.push_back(temp);
    }
    Block<int, int> block = builder.build();
    int index = templatedb::Build_Block<int, int>::add_to_block(block, entries);
    
    Build_Block<int, int> builder_two = Build_Block<int, int>(block.data);
    
    std::cout<<"INDEX: "<<index<<std::endl;
    builder_two.print_block();
    */

    //---------------------test constructor--------------------
    
    /*
    std::vector<Entry<int,int>> entries;
    size_t test_cons_size = 0;
    for(int i = 0; i < 220; i ++)
    {

        auto TS = std::chrono::system_clock::now();
        templatedb::Entry<int, int> temp = templatedb::Entry<int, int>(43, {1, 2, 3}, false, TS);
        
        entries.push_back(temp);
    }
    
    Build_Block<int, int> builder_two = Build_Block<int, int>(entries);
    
    std::cout<<"INDEX: "<<builder_two.enteries_kept_size()<<std::endl;
    builder_two.print_block();
    
*/

    return 0;
}