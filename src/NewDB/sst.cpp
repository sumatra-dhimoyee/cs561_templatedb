#include "sst.hpp"

using namespace templatedb;




template<typename K,typename V>
Build_SST<K,V> ::Build_SST(std::vector<Entry<K,V>> _data,size_t max_size, uint8_t _run, uint8_t level, bool _overflow)
{
    this->max_size = _max_size;
    this->run = _run;
    this->level = _level;
    this->size = sizeof(size_t) +sizeof(uint8_t) +sizeof(uint8_t);
    this->overflow = _overflow;
    int curr_block_size = 0;
    int perm_size = _data.size();
    while(!(curr_block_size == perm_size)
    {
    _data.erase(_data.begin(), _data.begin() + curr_block_size);
    Build_Block <K,V> builder = Build_Block<K,V>(_data);
   
    
    if(this->size + builer.current_size() > this->max_size)
    {
        this->overflow = true;
    }
    this->size = this->size + builer.current_size()
    curr_block_size= curr_block_size + builder.enteries_kept_size();
    Block<K,V> block = builder.build();
    this->_block_vector.push_back(block);
    

    }
}

template<typename K, typename V>
std::vector<Entry<K,V>> merge_sorted_vectors(std::vector<Block<K,V>>& B1, std::vector<Block<K,V>>& B2)
{
    std::vector<Entry<K,V> merged_vector;
    int vec_i = 0, vec_j = 0, i = 0, j = 0;

    while (i < B1.size() && j < B2.size())
    {
        while(vec_i < B1[i].data.size() && vec_j < B2[j].data.size())
        {
            if(B1[i].data[vec_i].key < B2[j].data[vec_j].key )
            {
                merged_vector.push_back(B1[i].data[vec_i]);
                vec_i++;
            }
            else if (B1[i].data[vec_i].key > B2[j].data[vec_j].key)
            {
                merged_vector.push_back(B2[i].data[vec_j]);
                vec_j++;
            }
            else
            {
                if(B1[i].data[vec_i].TS)
                {
                    merged_vector.push_back(B1[i].data[vec_i];
                    merged_vector.push_back(B1[j].data[vec_j]);
                    vec_i++;
                    vec_j++;
                }
                else
                {
                    merged_vector.push_back(B1[j].data[vec_j]);
                    vec_j++;
                }
            }

        }
        if(vec_i == B1[i].data.size())
        {
            i++;
            vec_i = 0;
        }
        if(vec_j == B2[j].data.size())
        {
            j++;
            vec_j = 0;
        }
    }
    

    if(i < B1.size())
    {
        for(int k = i; k < B1.size(); k++)
        {
            merged_vector.insert(merged_vector.end(), B1[k].data.begin() + vec_i, B1[k].data.end());
            vec_i = 0;  
        }
    }
    else if(j < B2.size())
    {
        for(int k = j; k < B2.size(); k++)
        {
            merged_vector.insert(merged_vector.end(), B2[k].data.begin() + vec_j, B2[k].data.end());
            vec_j = 0;  
        }
    }
    return merged_vector;
}

template<typename K, typename V>
SST<K,V> Build_SST<K,V>::merge_flush(SST<K,V>& first_sst, SST<K,V>& second_sst)
{

    std::vector<Entry<K,V> merged_entries = merge_sorted_vectors(first_sst.block_vector, second_sst.block_vector);

    Build_SST<K,V> sst_builder = Build_SST(merged_entries, first_sst.max_size, first_sst.run, first_sst.level, false);
    SST<K,V> sst = sst_builder.build();
     
}  

template<typename K, typename V>
SST<K,V> Build_SST<K,V>::build()
{
    SST<K,V> sst = SST(this->block_vector, this->size, this->run, this->level, this->max_size, this->overflow);
}




