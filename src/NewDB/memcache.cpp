#ifndef _TEMPLATEDB_SST_H_
#define _TEMPLATEDB_SST_H_

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "block.hpp"
#include "sst.hpp"

using namespace templatedb;
using namespace std;


class MemCache {
private:   
    vector<Entry> memcache;
    size_t BUFFER_SIZE= 8*1024*1024;

public:
    void put(K key, V value){
        if (memcache.size()<BUFFER_SIZE){
            memcache.push_back(Entry(key, vector<V> val(value), false, system_clock::now()));
        }
        else{
            flush();
            memcache.push_back(Entry(key, vector<V> val(value), false, system_clock::now()));
        }
        
    }

    vector<Entry>& get_entry(K key) {
        for (auto& entry : memcache){
            if (entry.key == key){
                return entry.val[0];
            }
        }
    }

    bool empty() {
        return memcache.empty()
    }
    
    vector<Entry>& get_memcache(){
        return memcache;
    }

    struct
    {
        bool operator()(Entry a, Entry b) const { return a.key < b.key; }
    }
    entry_compare;

    void sort(){
        std::sort(memcache.begin(), data.end(), entry_compare);
    }

    vector<K>& build_key_vector(){
        vector<K> keys;
        for (auto& entry : memcache){
            keys.push_back(entry.key);
        }
        return keys;
    }

    vector<V>& build_value_vector(){
        vector<V> values;
        for (auto& entry: memcache){
            values.push_back(entry.value[0]);
        }
        return values;
    }
}



#endif