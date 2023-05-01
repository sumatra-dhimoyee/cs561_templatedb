#include "sst.hpp"

using namespace templatedb;




template<typename K,typename V>
Build_SST<K,V> ::Build_SST(std::vector<Entry<K,V>> _data, size_t _max_size, uint8_t _level,  std::vector<zone<K>>& fp, BF::BloomFilter& bf, bool _overflow)
{
    this->max_size = _max_size;
    // this->run = _run;
    this->level = _level;
    this->size = sizeof(size_t) +sizeof(uint8_t) +sizeof(uint8_t);
    this->overflow = _overflow;
    int curr_block_size = 0;
    int perm_size = _data.size();
    int kept_entries = 0;
    while(!(curr_block_size == perm_size))
    {
    _data.erase(_data.begin(), _data.begin() + kept_entries);

    Build_Block<K,V> builder = Build_Block<K,V>(_data, bf);
   
    kept_entries = builder.enteries_kept_size();
    if(this->size + builder.current_size() > this->max_size)
    {
        this->overflow = true;
    }
    this->size = this->size + builder.current_size();
    curr_block_size= curr_block_size + kept_entries;
    Block<K,V> block = builder.build();
    this->block_vector.push_back(block);
    zone<K> Z = zone<K>(builder.block_min(), builder.block_max(), this->block_vector.size()-1);
    fp.push_back(Z);
    

    }
}

template<typename K, typename V>
std::vector<Entry<K,V>> merge_sorted_vectors(std::vector<Block<K,V>>& B1, std::vector<Block<K,V>>& B2)
{
    std::vector<Entry<K,V>> merged_vector;
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
                
                if(B1[i].data[vec_i].tomb)
                {
                    
                    merged_vector.push_back(B1[i].data[vec_i]);
                    merged_vector.push_back(B2[j].data[vec_j]);
                    vec_i++;
                    vec_j++;
                }
                else
                {
                    // std::cout<<"KEY 1: "<<B1[i].data[vec_i].key<<" KEY 2: "<<B2[j].data[vec_j].key<<std::endl;
                    // std::cout<<"I AM HERE"<<std::endl;
                    merged_vector.push_back(B2[j].data[vec_j]);
                    vec_i++;
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
SST<K,V> Build_SST<K,V>::merge_sst(SST<K,V>& first_sst, SST<K,V>& second_sst, std::vector<zone<K>>& fp, BF::BloomFilter& bf)
{

    std::vector<Entry<K,V>> merged_entries = merge_sorted_vectors(first_sst.block_vector, second_sst.block_vector);

    Build_SST<K,V> sst_builder = Build_SST(merged_entries, first_sst.max_size,  first_sst.level, fp, bf, false);
    SST<K,V> sst = sst_builder.build();
    // for (int i = 0; i < sst_builder.block_vector.size();i++)
    // {
    //     sst_builder.print_block(sst_builder.block_vector[i]);
    // }
    return sst;
     
}  



template<typename K, typename V>
SST<K,V> Build_SST<K,V>::build()
{
    SST<K,V> sst = SST<K,V>(this->block_vector, this->size,  this->level, this->max_size, this->overflow);
    return sst;
}

//remove later just for testing
template<typename K, typename V>
void Build_SST<K, V>::print_block(Block<K,V>& block)
{
    std::cout<<"PRINTING BLOCK ..."<<std::endl;
    for (int i = 0; i< block.data.size(); i++)
    {
        std::cout<<block.data[i].key<<" { ";
        for (int j = 0 ; j < block.data[i].value.size(); j++)
        {
            std::cout<< block.data[i].value[j]<<", ";
        }
        std::cout<<"}"<<std::endl;
    }

}




