#include "level.hpp"

using namespace templatedb;

template<typename K, typename V>
Level<K,V>::Level(SST<K,V> sst, uint8_t _no_runs, size_t _level_size, uint8_t level, std::vector<zone<K>>& fp, BloomFilter& bf)
{
    this->no_runs = _no_runs;
    this->run_size = _level_size/_no_runs;
    this->level_size = _level_size;
    this->level = level;
    this->FP.reserve(this->no_runs);
    this->FP.push_back(fp);
    // Build_SST<K,V> sst_builder = Build_SST<K,V>(entry_vector, this->run_size, this->level);
    // SST<K,V> sst = sst_builder.build();
    this->sst_vector.push_back(sst);
    this->BF.reserve(this->no_runs);
    this->BF.push_back(bf);
    

}

template<typename K, typename V>
bool Level<K,V>::add_sst(SST<K,V> sst, bool merge, std::vector<zone<K>>& fp, BloomFilter& bf);
{
    // Build_SST<K,V> sst_builder = Build_SST<K,V>(entry_vector, this->run_size,  this->level);
    // SST<K,V> sst = sst_builder.build();
    if(this->sst_vector.empty())
    {
        this->sst_vector.push_back(sst);
        this->FP.push_back(fp);
        this->BF.push_back(bf);
        return true;
    }
    if(merge)
    {
        SST<K,V> merged_sst = Build_SST<int, int>::merge_sst(this->sst_vector[0], sst, fp, bf);
        this->clear();
        this->sst_vector.push_back(merge_sst);
        this->FP.push_back(fp);
        if (merge_sst.overflow)
        {return false;}
        return true;

    }
    else
    {
        this->sst_vector.push_back(sst);
        this->FP.push_back(fp);
        this->BF.push_back(bf);
        if(this->sst_vector.size() > this->no_runs)
        {return false;}
        return true;
    }


}

template<typename K, typename V>
SST<K,V> Level<K,V>::merge_runs(std::vector<zone<K>>& fp, BloomFilter& bf)
{
    
    int i = 1;
    SST<K,V> merged_sst = this->sst_vector[0];
    while(i < this->sst_vector.size())
    {
    SST<K,V> sst = Build_SST<int,int>::merge_sst(merge_sst, this->sst_vector[i], fp, bf);
    merge_sst = sst;
    i++;
    }

    return merge_sst;
}

template<typename K, typename V>
void Level<K,V>::clear()
{
    this->sst_vector.clear();
    this->FP.clear();
}

template<typename K, typename V>
int Level<K,V>::get_block_index(int sorted_run, K key)
{
    for(int i = 0 ; i < FP[sorted_run]; i++)
    {
        if(FP[sorted_run][i].min<= key <= FP[sorted_run][i].max)
            return FP[sorted_run][i].block_index;
    }
    //return -1 of key not found
    return -1;
}

template<typename K, typename V>
int Level<K,V>::lookup(int sorted_run, int block_index, K key)
{
    //implement read_block function
    return read_block(this->sst_vector[sorted_run].block_vector[block_index], key);
}

template<typename K, typename V>
int Level<K,V>::get_sorted_run_no( K key)
{
    //implement bloomfilter lookup
    for (auto rit = this->BF.rbegin(); rit != this->BF.rend(); ++rit) {
        bool query_result = this->BF[rit].query(std::to_string(key));
        if(query_result)
            return rit;
    }
    return -1;
}

template<typename K, typename V>
std::vector<SST<K,V>> Level<K,V>::get_sst_vector(){
    return this->sst_vector;
}
