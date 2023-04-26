#include "sst.hpp"

using namespace templatedb;




template<typename K,typename V>
Build_SST<K,V> ::Build_SST(std::vector<Entry<K,V>> _data,size_t max_size, uint8_t _run, uint8_t level)
{
    this->max_size = _max_size;
    this->run = _run;
    this->level = _level;
    this->size = sizeof(size_t) +sizeof(uint8_t) +sizeof(uint8_t);
    int curr_block_size = 0;
    while(!(curr_block_size == _data.size())
    {
    _data.erase(_data.begin(), _data.begin() + curr_block_size);
    Build_Block <K,V> builder = Build_Block<K,V>(_data);
   
    
    if(this->size + builer.current_size() > this->max_size)
    {
        //change this to overflow later
        //remove break later

        this->num_extra_entries = curr_block_size;
        break;
    }
    this->size = this->size + builer.current_size()
    curr_block_size= curr_block_size + builder.enteries_kept_size();
    Block<K,V> block = builder.build();
    this->data.push_back(block);
    

    }
}

template<typename K, typename V>
std::vector<Entry<K,V>> merge_sorted_vectors(std::vector<Entry<K,V>>& vector_1, std::vector<Entry<K,V>> vector_2)
{
    std::vector<Entry<K,V> merged_vector;
    int index_vector_1 = 0;
    int index_vector_2 = 0;
    while(index_vector_1 <= vector1.size() || vect )
}

template<typename K, typename V>
bool Build_SST<K,V>::merge_flush(SST<K,V>& second_sst)
{

     
}  

template<typename K, typename V>
SST<K,V> Build_SST<K,V>::build()
{
    SST<K,V> sst = SST(this->block_vector, this->offset, this->run, this->level, this->max_size)
}




