#include "memcache.hpp"
#include "sst.hpp"
#include "block.hpp"


using namespace templatedb;
using namespace std;


template<typename K,typename V>
bool MemCache<K,V>::put(K key, std::vector<V> value){

    size_t entrySize = sizeof(K)+sizeof(std::chrono::time_point<std::chrono::system_clock>) + sizeof(bool) + sizeof(V)*value.size();
    std::cout<<"BUFFER SIZE: "<<this->maxBufferSize<<std::endl;
  
    if(entrySize <this->maxBufferSize){
        if(this->entryExist(key)){
            this->updateEntry(key, value);
            return true;
        }
        Entry<K,V> entry= Entry<K,V>(key, value, false, std::chrono::system_clock::now());
        this->memcache.push_back(entry);
        //this->bufferSize+=entrySize;
        this->sortEntries();
        return true;
    }
    else{
        std::cout<<"FINAL BUFFER SIZE: "<<this->memcache.size()<<std::endl;
        return false;
    }
}

template<typename K, typename V>
int MemCache<K,V>::binarySearch(std::vector<Entry<K,V>> entries, int l, int r, K key){
    while(l<=r){
        int m = l+(r-1)/2;
        if(entries[m].key == key){
            return m;
        }
        if(entries[m].key < key){
            l= m+1;
        }else{
            r = m-1;
        }
    }
    return -1;
}

template<typename K,typename V>
Entry<K,V>& MemCache<K,V>::getEntry(K key){
    int idx=binarySearch(this->memcache, 0, this->memcache.size(), key);
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
    for (Entry<K, V>& entry : this->memcache){
        if (entry.key == key){
            return true;
        }
    }
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
            this->sortEntries();
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
        if(this->entryExist(key)){
            Entry<K, V>& entry = this->getEntry(key);
            entry.tomb = true;
            entry.TS= std::chrono::system_clock::now();
            std::cout<<"Inside"<<std::endl;
        }else{
            std::vector<V> value;
            V val = 0;
            value.push_back(val);
            Entry<K,V> entry= Entry<K,V>(key, value, true, std::chrono::system_clock::now());
            this->memcache.push_back(entry);
            //this->bufferSize+=entrySize;
            this->sortEntries();
        }
        return true;
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
    //this->bufferSize=0;
}

template<typename K,typename V>
void MemCache<K,V>::printMemcache(){
    for(int i=0; i< this->memcache.size(); i++){
        for(int j =0; j<this->memcache[i].value.size(); j++){
            std::cout<<"Element at Index "<<i << " : " << " Key: " << this->memcache[i].key << " Value:  " << this->memcache[i].value[j] << " tomb:  " << this->memcache[i].tomb <<std::endl;
        }
        
    }
}







