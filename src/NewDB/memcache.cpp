#include "memcache.hpp"
#include "sst.hpp"
#include "block.hpp"


using namespace templatedb;
using namespace std;


template<typename K,typename V>
bool MemCache<K,V>::put(K key, std::vector<V> value){

    size_t entrySize = sizeof(K)+sizeof(std::chrono::time_point<std::chrono::system_clock>) + sizeof(bool) + sizeof(V)*value.size();
  
    if(entrySize >this->maxBufferSize){
        Entry<K,V> entry= new Entry<K,V>(key, value, false, std::chrono::system_clock::now());
        this->memcache.push_back(entry);
        this->bufferSize+=entrySize;
        this->sort();
        return true;
    }
    else{
        std::cout<<"FINAL BUFFER SIZE: "<<this->bufferSize<<std::endl;
        return false;
    }
}

template<typename K,typename V>
Entry<K,V>& MemCache<K,V>::getEntry(K key){
    for (Entry<K, V>& entry : this->memcache){
        if (entry.key == key){
            return entry;
        }
        throw std::runtime_error("Entry not found.");
    }
}

template<typename K,typename V>
bool MemCache<K,V>::updateEntry(K key, std::vector<V> value){
    try {
        Entry<K, V>& entry = this->get_entry(key);
        entry.value = value;
        entry.TS= std::chrono::system_clock::now();
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: Entry Cannot be Updated" << e.what() << std::endl;
        return false;
    }
}

template<typename K,typename V>
bool MemCache<K,V>::deleteEntry(K key){
    try {
        Entry<K, V>& entry = this->get_entry(key);
        entry.tomb = true;
        entry.TS= std::chrono::system_clock::now();
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: Entry Cannot be Deleted" << e.what() << std::endl;
        return false;
    }
}

template<typename K,typename V>
bool MemCache<K,V>::compareEntries (Entry<K,V> i,Entry<K,V> j) { return (i.key<j.key); }

template<typename K,typename V>
void MemCache<K,V>::sort(){
    std::sort(this->memcache.begin(), this->memcache.end(), compareEntries());
}

template<typename K,typename V>
vector<Entry<K,V>>& MemCache<K,V>::getMemcache(){
    return this->memcache;
}

template<typename K,typename V>
int MemCache<K,V>::getBufferSize(){
    return this->bufferSize.empty();
}

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






