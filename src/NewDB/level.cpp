#include "level.hpp"

using namespace templatedb;

template<typename K, typename V>
Level<K,V> :: Level(SST<K,V> sst, int _no_runs, size_t _level_size, uint8_t level, std::vector<zone<K>>& fp, BF::BloomFilter& bf)
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
    // std::cout<<"SIZE: "<<sst.offset<<" MAX SIZE: "<<sst.max_size<<std::endl;
    // std::cout<<"NUMBER OF RUNS: "<<this->sst_vector.size()<<std::endl;
    

}

template<typename K, typename V>
bool Level<K,V> ::add_sst(SST<K,V> sst, bool leveled, std::vector<zone<K>>& fp, BF::BloomFilter& bf, u_int8_t no_levels)
{
    // Build_SST<K,V> sst_builder = Build_SST<K,V>(entry_vector, this->run_size,  this->level);
    // SST<K,V> sst = sst_builder.build();
    if(this->sst_vector.empty())
    {

        this->sst_vector.push_back(sst);
        this->FP.push_back(fp);
        this->BF.push_back(bf);
        return true;
        // std::cout<<"NUMBER OF RUNS: "<<this->sst_vector.size()<<std::endl;
        // std::cout<<"SIZE: "<<sst.offset<<" MAX SIZE: "<<sst.max_size<<std::endl;
    }
    if(leveled)
    {
        
        SST<K,V> merged_sst = Build_SST<K,V>::merge_sst(this->sst_vector[0], sst, fp, bf, no_levels);
        // std::cout<<"SST SIZE "<< merged_sst.offset<<" SST MAX SIZE "<<merged_sst.max_size<<std::endl;
        // std::cout<<merged_sst.overflow<<std::endl;
        this->clear();
        this->sst_vector.push_back(merged_sst);
        this->FP.push_back(fp);
        // std::cout<<"SIZE: "<<merged_sst.offset<<" MAX SIZE: "<<merged_sst.max_size<<std::endl;
        // Build_SST<K,V> builder = Build_SST<K,V>(sst.block_vector[0].data, sst.max_size, sst.level, fp,bf);
        // for(int i = 0; i <merged_sst.block_vector.size(); i++)
        // {
        //     builder.print_block(merged_sst.block_vector[i]);
        // }
        
        if (merged_sst.overflow)
        {
            // std::cout<<"OVERFLOWWW ---"<<std::endl;
            return false;
        }
        return true;

    }
    else
    {
        

        
        this->sst_vector.push_back(sst);
        // std::cout<<"NUMBER OF RUNS: "<<this->sst_vector.size()<<std::endl;
        // std::cout<<"MAX RUNSSSS: "<<(this->no_runs)<<std::endl;
        this->FP.push_back(fp);
        this->BF.push_back(bf);
        // std::cout<<"SIZE: "<<sst.offset<<" MAX SIZE: "<<sst.max_size<<std::endl;
        if(this->sst_vector.size() >= this->no_runs)
        {
            return false;
        }
        return true;
    }


}

template<typename K, typename V>
SST<K,V> Level<K,V>::merge_runs(std::vector<zone<K>>& fp, BF::BloomFilter& bf, u_int8_t no_levels)
{
    
    int i = this->sst_vector.size() -1;
    SST<K,V> merged_sst = this->sst_vector[i];
    while(i-1 >= 0)
    {
        // std::cout<<"LEVELLLL"<<std::endl;
    SST<K,V> sst = Build_SST<K,V>::merge_sst(this->sst_vector[i-1], merged_sst, fp, bf, no_levels);
    merged_sst = sst;
    i--;
    
    }

    //  for(int i = 0; i< fp.size();i++)
    // {
    //     std::cout<<fp[i].min<<" "<< fp[i].max <<std::endl;
    // }

    return merged_sst;
}

template<typename K, typename V>
void Level<K,V>::clear()
{
    this->sst_vector.clear();
    this->FP.clear();
    this->BF.clear();
}

template<typename K, typename V>
int Level<K,V>::get_block_index(int sorted_run, K key)
{
    // std::cout<<this->FP[0].size()<<std::endl;
    for(int i = 0 ; i < this->FP[sorted_run].size(); i++)
    {
        // std::cout<<FP[sorted_run][i].min<<" "<< FP[sorted_run][i].max <<std::endl;
        if((FP[sorted_run][i].min<= key) && (key <= FP[sorted_run][i].max))
        {
            
            return FP[sorted_run][i].block_index;
        }
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
int Level<K,V>::bloom_lookup( K key)
{
    //implement bloomfilter lookup
    for(int i = this->BF.size() - 1; i >= 0; i--)
    {
        bool query_result;
         try {
                query_result = this->BF[i].query(key);
            } catch(const std::exception& e) {
                    
                query_result = this->BF[i].query(key);
            }
         
        if(query_result)
            return i;
    }
    return -1;
}

template<typename K, typename V>
std::vector<SST<K,V>> Level<K,V>::get_sst_vector(){
    return this->sst_vector;
}