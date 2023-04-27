#include "memcache.hpp"
#include "sst.hpp"


using namespace templatedb;
using namespace std;


template<typename K, typename V>
void put(K key, V value){
    if (memcache.size()< _bufferSize){
        memcache.push_back(Entry(key, vector<V> val(value), false, system_clock::now()));
    }
    else{
        flush();
        memcache.push_back(Entry(key, vector<V> val(value), false, system_clock::now()));
    }
    
}

template<typename K, typename V>
vector<Entry<K,V>>& get_entry(K key) {
    for (auto& entry : memcache){
        if (entry.key == key){
            return entry.val[0];
        }
    }
}

template<typename K, typename V>
bool empty() {
    return memcache.empty()
}

template<typename K, typename V>   
vector<Entry<K,V>>& get_memcache(){
    return memcache;
}

template<typename K, typename V> 
bool entryComparator(Entry<K,V> a, Entry<K,V> b){
    const { return a.key < b.key; }
}

template<typename K, typename V>
void sort(){
    std::sort(memcache.begin(), data.end(), entryComparator);
}

template<typename K, typename V>
vector<K>& build_key_vector(){
    vector<K> keys;
    for (auto& entry : memcache){
        keys.push_back(entry.key);
    }
    return keys;
}

template<typename K, typename V>
vector<V>& build_value_vector(){
    vector<V> values;
    for (auto& entry: memcache){
        values.push_back(entry.value[0]);
    }
    return values;
}

