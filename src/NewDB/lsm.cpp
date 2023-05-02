#include "level.hpp"

using namespace templatedb;

template<typename K, typename V>
LSM<K,V>::LSM(size_t _mem_size, uint8_t _T_ratio, bool _leveled, int _bf_num_elem, int _bf_num_bits_per_elem )
{

    this->mem_size = _mem_size;
    this->T_ratio = _T_ratio;
    this->leveled = _leveled;
    this->bf_num_elem = _bf_num_elem;
    this->bf_num_bits_per_elem = _bf_num_bits_per_elem;
    if (this->leveled)
    {this->no_runs = 1;}
    else
    {
        this->no_runs = this->T_ratio;
        
    }
    
}

template<typename K, typename V>
void LSM<K,V>::create_sst(std::vector<Entry<K,V>> entries)
{
    size_t extra_size = sizeof(size_t)*2 +sizeof(uint8_t) +sizeof(bool);
    size_t _level_size = (this->mem_size+extra_size)* this->T_ratio;
    size_t _run_size;
    if(this->leveled)
        _run_size = _level_size;
    else
        _run_size = (_level_size)/this->T_ratio;

    std::vector<zone<K>> fp;
    BF::BloomFilter bf = BF::BloomFilter(this->bf_num_elem, this->bf_num_bits_per_elem);
    Build_SST<K,V> sst_builder = Build_SST<K,V>(entries, _run_size, this->levels.size() , fp, bf);
    SST<K,V> sst = sst_builder.build();
        bool x = false;
        int i = 0;
        while (!x)
        {
            if(i < this->levels.size())
            {
                // std::cout<<"_______ADDING _1_____________"<<std::endl;
                x = levels[i].add_sst(sst, this->leveled, fp, bf);
                if(!x)
                {
                    
                    sst = levels[i].merge_runs(fp, bf);
                    levels[i].clear();
                    _level_size = _level_size*this->T_ratio;
                    if(leveled)
                        _run_size = _level_size;
                    else
                        _run_size = (_level_size)/this->T_ratio;
                    sst.max_size = _run_size;
                    if(sst.offset <= sst.max_size)
                        sst.overflow = false;
                    i++;
                }
                
            }
            else{
                // std::cout<<"_______ADDING _2_____________ "<<i <<std::endl;
                // _level_size = (this->mem_size+extra_size) * pow(this->T_ratio, i+1);
                sst.max_size = _run_size;
                Level<K,V> lvl = Level<K,V>(sst, this->no_runs, _level_size, i, fp, bf);
                this->levels.push_back(lvl);
                x = true;
                
            }
        }
    // std::cout<<"NUM LEVELS: "<<this->levels.size()<<std::endl;
    
}

template<typename K, typename V>
std::vector<V> LSM<K,V>::get(K key)
{
    // rough implementation -- does not work --- just for understanding the workflow of fence pointers
    std::vector<V> ret;
    int sorted_run = levels[0].bloom_lookup(key);
   int x = levels[0].get_block_index(sorted_run, key);
   if (x>=0)
   {
    std::vector<V> ret = levels[0].lookup(sorted_run, x, key);
    return ret;
   }
   return ret;
}

template<typename K, typename V>
MemCache<K,V> LSM<K,V>::createMemcache(){
    std::vector<Entry<K,V>> entries;
    this->memcache= new MemCache(mem_size, entries);
    return this->memcache;

}

template<typename K, typename V>
void LSM<K,V>::put(K key, V val){
    std::vector<V> value;
    value.push_back(val);
    bool toMemcache= this->memcache.put(key, value);
    if(!toMemcache){
        create_sst(this->memcache);
        this->memcache.clearMemcache();
        this->memcache.put(key, value);
    }
}


template<typename K, typename V>
bool LSM<K,V>::update(K key, V value){
    std::vector<V> value;
    value.push_back(val);
    
    bool toMemcache= this->memcache.updateEntry(key, value);
    if(!toMemcache){
        create_sst(this->memcache);
        this->memcache.clearMemcache();
        this->memcache.updateEntry(key, value);
    }

}

template<typename K, typename V>
V LSM<K,V>::pointQuery(K key){
    if(this->memcache.entryExist(key)){
        return this->memcache.getEntry().value[0];
    }else{
        for(int i=0; i<no_levels; i++){
            int bloomFinder = this->levels[i].bloom_lookup(key);
            if(bloomFilter!=-1){
                int blockIndex= this->levels[i].get_block_index(bloomFinder, key);
                if(blockIndex!=-1){
                    std::vector<SST<K,V>> sstVector=this->levels[i].get_sst_vector();
                    Block<K,V> tempBlock = sstVector[blockIndex].getBlock();
                    if(tempBlock.entryExist()){
                        Entry<K,V> tempEntry = tempBlock.getEntry();
                        return tempEntry.value[0];
                    }
                }
            }
        }

        throw std::runtime_error("Entry not found.");

    }


}

template<typename K, typename V>
bool LSM<K,V>::deleteQuery(K key){  
    bool toMemcache= this->memcache.deleteEntry(key);
    if(!toMemcache){
        create_sst(this->memcache);
        this->memcache.clearMemcache();
        this->memcache.deleteEntry(key, value);
    }

}


template<typename K, typename V>
std::vector<V> LSM<K,V>::rangeQuery(K minkey, K maxkey){

}

template<typename K, typename V>
bool LSM<K,V>::deleteRangeQuery(K minkey, K maxkey){

}

