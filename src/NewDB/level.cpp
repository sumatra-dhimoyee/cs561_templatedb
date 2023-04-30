#include "level.hpp"

using namespace templatedb;

template<typename K, typename V>
Level<K,V> :: Level(SST<K,V> sst, uint8_t _no_runs, size_t _level_size, uint8_t level)
{
    this->no_runs = _no_runs;
    this->run_size = _level_size/_no_runs;
    this->level_size = _level_size;
    this->level = level;
    // Build_SST<K,V> sst_builder = Build_SST<K,V>(entry_vector, this->run_size, this->level);
    // SST<K,V> sst = sst_builder.build();
    this->sst_vector.push_back(sst);

}

template<typename K, typename V>
bool add_sst(SST<K,V> sst, bool merge);
{
    // Build_SST<K,V> sst_builder = Build_SST<K,V>(entry_vector, this->run_size,  this->level);
    // SST<K,V> sst = sst_builder.build();
    if(this->sst_vector.empty())
    {
        this->sst_vector.push_back(sst);
        return true;
    }
    if(merge)
    {
        SST<K,V> merged_sst = Build_SST<int, int>::merge_sst(this->sst_vector[0], sst);
        this->sst_vector.clear();
        this->sst_vector.push_back(merge_sst);
        if (merge_sst.overflow)
        {return false;}
        return true;

    }
    else
    {
        this->sst_vector.push_back(sst);
        if(this->sst_vector.size() > this->no_runs)
        {return false;}
        return true;
    }


}

template<typename K, typename V>
SST<K,V> Level<K,V>::merge_runs()
{
    
    int i = 1;
    SST<K,V> merged_sst = this->sst_vector[0];
    while(i < this->sst_vector.size())
    {
    SST<K,V> sst = Build_SST<int,int>::merge_sst(merge_sst, this->sst_vector[i]);
    merge_sst = sst;
    i++;
    }

    return merge_sst;
}

template<typename K, typename V>
void Level<K,V>::clear()
{
    this->sst_vector.clear();
}
