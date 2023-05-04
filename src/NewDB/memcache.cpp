#include "memcache.hpp"
#include "sst.hpp"
#include "block.hpp"


using namespace templatedb;
using namespace std;


template<typename K,typename V>
bool MemCache<K,V>::put(K key, std::vector<V> value){

    size_t entrySize = sizeof(K)+sizeof(std::chrono::time_point<std::chrono::system_clock>) + sizeof(bool) + sizeof(V)*value.size();
    // std::cout<<"BUFFER SIZE: "<<this->maxBufferSize<<std::endl;
//   std::cout<<"ENTRY SIZE: "<<entrySize<<std::endl;
    if((this->bufferSize + entrySize) <= this->maxBufferSize){
        if(this->entryExist(key)){
            this->updateEntry(key, value);
            this->bufferSize = this->bufferSize +entrySize;
            return true;
        }
        Entry<K,V> entry= Entry<K,V>(key, value, false, std::chrono::system_clock::now());
        this->memcache.push_back(entry);
        this->bufferSize = this->bufferSize +entrySize;
        //this->bufferSize+=entrySize;
        //this->sortEntries();
        return true;
    }
    else{
        // std::cout<<"FINAL BUFFER SIZE: "<<this->memcache.size()<<std::endl;
        return false;
    }
}

template<typename K, typename V>
int MemCache<K,V>::binarySearch(std::vector<Entry<K,V>> entries, int l, int r, K key){
    int i = 0;
    while(l<=r){
        // std::cout<<"I am here 6 "<< l<<" "<< entries[l].key <<"  "<<r<<" "<< entries[r].key<<" "<<key<< std::endl;

        int m = l+(r-l)/2;
        // std::cout<<"key "<< m<<" " <<entries[m].key<<  std::endl;
        
        if(entries[m].key == key){
            return m;
        }
        if(entries[m].key < key){
            //  std::cout<<"key -1 "<< entries[m].key<<  std::endl;
            l= m+1;
            //  std::cout<<"key -1 "<< entries[l].key<<  std::endl;
        }else{
            //  std::cout<<"key -2 "<< entries[m].key<<  std::endl;
            r = m-1;
        }
        // if ( i == 6)
        //     break;
        i++;
        
    }
    return -1;
}

template<typename K,typename V>
Entry<K,V>& MemCache<K,V>::getEntry(K key){
    // std::cout<<"I am here 5"<<std::endl;
    //int idx=binarySearch(this->memcache, 0, this->memcache.size()-1, key);
    int idx=-1;
    for(int i = 0; i < this->memcache.size(); i++)
    {
        // std::cout<<"I AM HERE 6"<<std::endl;
        if (this->memcache[i].key == key){
            idx=i;
        }

    }
    
    return this->memcache[idx];

    // K temp;
    // for (Entry<K, V>& entry : this->memcache){
    //     if (entry.key == key){
    //         temp = entry;
    //     }
    //     throw std::runtime_error("Entry not found.");
    // }
    // return temp;
}

template<typename K,typename V>
bool MemCache<K,V>::entryExist(K key){
    // std::cout<<"I AM HERE 5 "<< this->memcache.size()<<std::endl;
    for(int i = 0; i < this->memcache.size(); i++)
    {
        // std::cout<<"I AM HERE 6"<<std::endl;
        if (this->memcache[i].key == key){
            return true;
        }

    }
    //  std::cout<<"I AM HERE 7"<<std::endl;
    // for (Entry<K, V> entry : this->memcache){
    //     std::cout<<"I AM HERE 6"<<std::endl;
    //     if (entry.key == key && !entry.tomb){
    //         return true;
    //     }
    // }
    return false;
}



template<typename K,typename V>
bool MemCache<K,V>::updateEntry(K key, std::vector<V> value){
    try {
        if(this->entryExist(key)){
            Entry<K, V>& entry = this->getEntry(key);
            if(entry.tomb){
                entry.tomb=false;
            }
            entry.value = value;
            entry.TS= std::chrono::system_clock::now();
            //this->sortEntries();
        }else{
            this->put(key, value);
        }
        return true;
    }
    catch (const std::exception& e) {
        //std::cerr << "Error: Entry Cannot be Updated" << e.what() << std::endl;
        return false;
    }
}

template<typename K,typename V>
bool MemCache<K,V>::deleteEntry(K key){
    
    try {
        // std::cout<<"I AM HERE 3"<<std::endl;
        if(this->entryExist(key)){
            
            Entry<K, V>& entry = this->getEntry(key);
            entry.tomb = true;
            entry.TS= std::chrono::system_clock::now();
            std::cout<<"Inside"<<std::endl;
            return true;
        }else{
            
            std::vector<V> value = {};
            size_t entrySize = sizeof(K)+sizeof(std::chrono::time_point<std::chrono::system_clock>) + sizeof(bool) + sizeof(V)*value.size();
            // V val = 0;
            // value.push_back(val);
            if(this->bufferSize + entrySize <= this->maxBufferSize)
            {
                Entry<K,V> entry= Entry<K,V>(key, value, true, std::chrono::system_clock::now());
                // std::cout<<"I AM HERE 4"<<std::endl;
                this->memcache.push_back(entry);
                //this->bufferSize = this->bufferSize +entrySize;
                //this->bufferSize+=entrySize;
                //this->sortEntries();
                return true;
            }
            else
            {
                return false;
            }
            
        }
        
    }
    catch (const std::exception& e) {

        std::cerr << "Error: Entry Cannot be Deleted" << e.what() << std::endl;
        return false;
    }
}

template<typename K,typename V>
bool MemCache<K,V>::compareEntries (Entry<K,V>& i,Entry<K,V>& j) { return (i.key<j.key); }

template<typename K,typename V>
void MemCache<K,V>::sortEntries(){
    std::sort(this->memcache.begin(), this->memcache.end(), [](const Entry<K,V>& a, const Entry<K,V>& b) {
        return a.key < b.key;
        });
}

template<typename K,typename V>
vector<Entry<K,V>>& MemCache<K,V>::getMemcache(){
    return this->memcache;
}

// template<typename K,typename V>
// int MemCache<K,V>::getBufferSize(){
//     return this->bufferSize.size();
// }

template<typename K,typename V>
bool MemCache<K,V>::isEmpty(){
    return this->memcache.empty();
}

template<typename K,typename V>
vector<K>& MemCache<K,V>::buildKeyVector(){
    vector<K> keys;
    for(Entry<K,V>& entry : this->memcache){
        keys.push_back(entry.key);
    }
    return keys;
}

template<typename K,typename V>
vector<vector<V>>& MemCache<K,V>::buildValueVector(){
    vector<vector<V>> values;
    for(Entry<K,V>& entry : this->memcache){
        values.push_back(entry.value);
    }
    return values;
}

template<typename K,typename V>
vector<V>& MemCache<K,V>::buildIntValueVector(){
    vector<vector<V>> values;
    for(Entry<K,V>& entry : this->memcache){
        values.push_back(entry.value[0]);
    }
    return values;
}

template<typename K,typename V>
void MemCache<K,V>::clearMemcache(){
    this->memcache.clear();
    this->bufferSize=0;
}



template<typename K,typename V>
void MemCache<K,V>::printMemcache(){
    for(int i=0; i< this->memcache.size(); i++){
        for(int j =0; j<this->memcache[i].value.size(); j++){
            std::cout<<"Element at Index "<<i << " : " << " Key: " << this->memcache[i].key << " Value:  " << this->memcache[i].value[j] << " tomb:  " << this->memcache[i].tomb <<std::endl;
        }
        
    }
}







