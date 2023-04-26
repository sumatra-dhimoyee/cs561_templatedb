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

template<typename K, typename V>
    class Entry
    {
        public:
            K key;
            std::vector<V> value;
            bool tomb;
            std::chrono::time_point<std::chrono::system_clock> TS;
            Entry(K _key, std::vector<V> _value, bool _tomb, std::chrono::time_point<std::chrono::system_clock> _TS)
            {
                key = _key;
                value = _value;
                tomb = _tomb;
                TS = _TS;
            }


            
    };

class MemTable {
private:   
    vector<Entry> memtable;

public:
    void put(K key, V value){
        memtable.push_back(Entry(key, vector<V> val(value), false, system_clock::now()));
    }

    vector<Entry>& get_entry(K key) {
        for (auto& entry: memtable){
            if (entry.key == key){
                return entry.val[0];
            }
        }
    }

    bool empty() {
        mem
    }
    
}






#endif