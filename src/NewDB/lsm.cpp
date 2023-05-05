#include "level.hpp"

using namespace templatedb;

template<typename K, typename V>
LSM<K,V>::LSM(size_t _mem_size, uint8_t _T_ratio, bool _leveled, int _bf_num_elem, int _bf_num_bits_per_elem)
{

    std::vector<Entry<K,V>> entries;
    this->mem_size = _mem_size;
    this->memcache = new MemCache<K,V>(this->mem_size, entries);
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
    // std::cout<<"HHAHHA"<<std::endl;
    size_t extra_size = sizeof(size_t)*2 +sizeof(uint8_t) +sizeof(bool);
    // size_t _level_size = (this->mem_size+extra_size)* this->T_ratio;
    size_t _level_size = (this->mem_size)* this->T_ratio;
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
                x = levels[i].add_sst(sst, this->leveled, fp, bf, this->levels.size());
                if(!x)
                {
                    
                    sst = levels[i].merge_runs(fp, bf, this->levels.size());
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

// template<typename K, typename V>
// MemCache<K,V> LSM<K,V>::createMemcache(){
//     std::vector<Entry<K,V>> entries;
//     this->memcache= MemCache<K,V>(this->mem_size, entries);
//     return this->memcache;

// }

template<typename K, typename V>
void LSM<K,V>::put(K key, std::vector<V> value){
    // std::vector<V> value;
    // value.push_back(val);
    bool toMemcache= this->memcache->put(key, value);
    if(toMemcache)
    {
        // std::cout<<" not creating sst"<<std::endl;
    }
    
    if(!toMemcache){
        // std::cout<<"creating sst"<<std::endl;
        this->memcache->sortEntries();
        create_sst(this->memcache->getMemcache());
        this->memcache->clearMemcache();
        this->memcache->put(key, value);
    }
}


template<typename K, typename V>
void LSM<K,V>::update(K key, std::vector<V> value){
    // std::vector<V> val;
    // value.push_back(val);
    
    bool toMemcache= this->memcache->updateEntry(key, value);
    if(!toMemcache){
        create_sst(this->memcache->getMemcache());
        this->memcache->clearMemcache();
        this->memcache->updateEntry(key, value);
    }

}

template<typename K, typename V>
void LSM<K,V>::pointQuery(K key, std::vector<V>& result, bool& flag ){
    
    // std::cout<<"I AM HERE 1"<<std::endl;
    
    //Entry<K,V> entry;
    // std::cout<<"I AM HERE 2"<<std::endl;
    if(this->memcache->entryExist(key)){
        // std::cout<<"I AM HERE 3"<<std::endl;
        if(!this->memcache->getEntry(key).tomb)
        {
            
            result = this->memcache->getEntry(key).value;
        }
        
        //  std::cout<<"I AM HERE 4"<<std::endl;
    }else{
        // std::cout<<"NUM LEVELS: "<<this->levels.size()<<std::endl;
        for(int i=0; i<this->levels.size(); i++){
            int bloomFinder = this->levels[i].bloom_lookup(key);
            // std::cout << "bloomFinder " << bloomFinder << std::endl;
            if(bloomFinder!=-1){
                // std::cout << "bloomFinder " << bloomFinder << std::endl;
                std::vector<SST<K,V>> sstVector=this->levels[i].get_sst_vector();
                SST<K,V> tempSST= sstVector[bloomFinder];
                int blockIndex= this->levels[i].get_block_index(bloomFinder, key);
                // std::cout << "blockIndex " << blockIndex << std::endl;
                if(blockIndex!=-1){
                    Block<K,V> tempBlock = tempSST.block_vector[blockIndex];
                    // std::vector<SST<K,V>> sstVector=this->levels[i].get_sst_vector();
                    // Block<K,V> tempBlock = sstVector[bloomFinder].block_vector(blockIndex);
                    if(tempBlock.entryExist(key)){
                        Entry<K,V> tempEntry = tempBlock.getEntry(key);
                        READ_IO++;
                        result = tempEntry.value;
                    }
                }
            }
        }
        
        

    }
    if(result.empty())
    {
        flag = false;
    }
    
    


}

template<typename K, typename V>
void LSM<K,V>::deleteQuery(K key){  
    // std::cout<<"I AM HERE"<<std::endl;
    bool toMemcache= this->memcache->deleteEntry(key);
    // std::cout<<"I AM HERE 2"<<std::endl;
    if(!toMemcache){

        create_sst(this->memcache->getMemcache());
        this->memcache->clearMemcache();
        this->memcache->deleteEntry(key);
    }

}

template<typename K, typename V>
struct compare {
	bool operator()(
		pair<Entry<K,V>, vector<int>> a, pair<Entry<K,V>, vector<int>> b) {
            Entry<K,V>& entry_a= a.first;
            Entry<K,V>& entry_b= b.first;
		return entry_a.key > entry_b.key;
	}
};

template<typename K, typename V>
void LSM<K,V>::rangeQuery(K minkey, K maxkey){
    //if maxkey is less than memcache 0
    std::vector<Entry<K,V>> memTemp;
    for(Entry<K,V>& entry: this->memcache->getMemcache()){
        if(entry.key>=minkey && entry.key<=maxkey){
            memTemp.push_back(entry);
        }
    }
    priority_queue<pair<Entry<K,V>, vector<int>>, vector<pair<Entry<K,V>, vector<int>>>, compare<K,V>> pq; 

    std::vector<int> vectorMem(4,0);
    pq.push(make_pair(memTemp[0], vectorMem));

    // Insert the first element of each array into the heap 
    for (int i = 0; i < this->levels.size(); i++) { 
        std::vector<int> idx(4, 0);
        idx[0]= i+1;
        Entry<K,V> tempEntry = this->levels[i].get_sst_vector()[0].block_vector[0].data[0];
        pq.push(make_pair(tempEntry, idx));
        //pq.push(make_pair(arr[i][0], make_pair(i, 0))); 
    } 

    // Merge the arrays 
    while (!pq.empty()) { 
        pair<Entry<K,V>, vector<int>> curr = pq.top(); 
        pq.pop(); 

        Entry<K,V> tempEntry1 = curr.first; 
        std::vector<int> idx1 = curr.second; 

        for(int i=0; i<tempEntry1.value.size(); i++){
            std::cout<<tempEntry1.value[i]<<std::endl;
        }


 
        //std::cout<<"SIZE OF "<<i<<" "<<temp_size<<std::endl;
        //result.push_back(); 

        // If the current element is not the last element of the array, 
        // push the next element of the array into the heap 
        if(idx1[0]==0 && idx1[1]+1<memTemp.size()){
            idx1[1]=idx1[1]+1;
            pq.push(make_pair(memTemp[idx1[1]], idx1));
        }
        else if((idx1[3]+1)<this->levels[idx1[0]].get_sst_vector()[idx1[1]].block_vector[idx1[2]].data.size()){
            idx1[3]= idx1[3]+1;

            pq.push(make_pair(levels[idx1[0]].get_sst_vector()[idx1[1]].block_vector[idx1[2]].data[idx1[3]], idx1));
        }
        else if((idx1[3]+1)==this->levels[idx1[0]].get_sst_vector()[idx1[1]].block_vector[idx1[2]].data.size()){
            idx1[2]= idx1[2]+1;
            idx1[3]=0;
            pq.push(make_pair(levels[idx1[0]].get_sst_vector()[idx1[1]].block_vector[idx1[2]].data[idx1[3]], idx1));
        }
        else if((idx1[2]+1)==this->levels[idx1[0]].get_sst_vector()[idx1[1]].block_vector.size()){
            idx1[1]= idx1[1]+1;
            idx1[2]=0;
            idx1[3]=0;
            pq.push(make_pair(levels[idx1[0]].get_sst_vector()[idx1[1]].block_vector[idx1[2]].data[idx1[3]], idx1));
        }
        else if((idx1[1]+1)==this->levels[idx1[0]].get_sst_vector().size()){
            idx1[0]= idx1[0]+1;
            idx1[1]=0;
            idx1[2]=0;
            idx1[3]=0;
            pq.push(make_pair(levels[idx1[0]].get_sst_vector()[idx1[1]].block_vector[idx1[2]].data[idx1[3]], idx1));
        }
        // else if((idx1[0])==this->levels.size()){
        //     break;
        // }
    } 



}

template<typename K, typename V>
void LSM<K,V>::deleteRangeQuery(K minkey, K maxkey){
    for(Entry<K,V>& entry: this->memcache->getMemcache()){
        if(entry.key>=minkey && entry.key<=maxkey){
            this->memcache->deleteEntry(entry.key);
        }
    }
    for(int i=0; i<this->levels.size(); i++){
        std::vector<SST<K,V>> sstVector=this->levels[i].get_sst_vector();
        for(int j=0; j<sstVector.size(); j++){
            std::vector<Block<K,V>> blockVector = sstVector[j];
            for(int k=0; k<blockVector.size(); k++){
                K blockmin = blockVector[k].block_min();
                K blockmax = blockVector[k].block_max();
                if(blockmin>=minkey && blockmin<=maxkey && blockmax<=maxkey && blockmax>=minkey){
                    for(K m=blockmin; m<=blockmax; m++){
                        this->deleteQuery(m);
                    }
                }
            }
        }
    }
}

