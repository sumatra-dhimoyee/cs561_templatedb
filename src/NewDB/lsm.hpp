#ifndef _TEMPLATEDB_LSM_H_
#define _TEMPLATEDB_LSM_H_

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "level.hpp"
#include <cmath>
#include "memcache.hpp"

namespace templatedb
{

    template<typename K, typename V>
    class LSM
    {
        private:
        std::vector<level<K,V>> levels;
        MemCache<K,V> memcache;
        uint8_t no_levels;
        size_t mem_size;
        uint8_t T_ratio;
        bool leveled;
        int bf_num_elem;
        int bf_num_bits_per_elem;

        public:

        LSM(std::vector<Entry<K,V>> entries, size_t _mem_size, uint8_t _T_ratio, bool _leveled);
        void add_buffer(std::vector<K,V> entries, bool leveled);

        // void put(Entry<K,V> entry);

        //create function which takes in a vector of Entry and puts it in memtable
        //create function which takes the data in memcache after it's full add it to the levels. 

        std::vector<V> get(K key);
        MemCache<K,V> createMemcache();
        void put(K key, V value);
        bool update(K key, V value);
        V pointQuery(K key);
        bool deleteQuery(K key);
        std::vector<V> rangeQuery(K minkey, K maxkey);
        bool deleteRangeQuery(K minkey, K maxkey);
        bool garbageCollection();
        

    };
}

#endif