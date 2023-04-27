#ifndef _TEMPLATEDB_SST_H_
#define _TEMPLATEDB_SST_H_

#include <fstream>
#include <iostream>
#include <iterator>
#include "memcache.hpp"
#include "sst.hpp"
#include "block.hpp"



using namespace templatedb;
using namespace std;


class MemCache {
private:   
    vector<Entry> _memcache;
    size_t _bufferSize;

public:
    MemCache(size_t bufferSize, vector<Entry> memcache){
        _bufferSize= bufferSize* 1024* 1024;
        _memcache=memcache;

    }
    void put(K key, V value);
    vector<Entry>& get_entry(K key);
    bool empty();
    vector<Entry>& get_memcache();
    void sort();
    vector<K>& build_key_vector();
    vector<V>& build_value_vector();

};

#endif