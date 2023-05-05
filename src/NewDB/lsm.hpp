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
#include "memcache.hpp"
#include <cmath>
#include <queue>

namespace templatedb
{

    template<typename K, typename V>
    class LSM
    {
        // private:
        public:
        std::vector<Level<K,V>> levels;
        MemCache<K,V>* memcache;
        uint8_t no_levels;
        int no_runs;
        size_t mem_size;
        uint8_t T_ratio;
        bool leveled;
        int bf_num_elem;
        int bf_num_bits_per_elem;
        int READ_IO = 0;
        int WRITE_IO = 0;

        
        // public:
        LSM(size_t _mem_size, uint8_t T_ratio, bool leveled, int _bf_num_elem, int _bf_num_bits_per_elem);
        void create_sst(std::vector<Entry<K,V>> entries);

        // void put(Entry<K,V> entry);

        //create function which takes in a vector of Entry and puts it in memtable
        //create function which takes the data in memcache after it's full add it to the levels. 

        std::vector<V> get(K key);
        //MemCache<K,V> createMemcache();
        void put(K key, std::vector<V> value);
        void update(K key, std::vector<V> value);
        void pointQuery(K key, std::vector<V>& result,bool& flag);
        void deleteQuery(K key);
        void rangeQuery(K minkey, K maxkey);
        void deleteRangeQuery(K minkey, K maxkey);
        
        

        

    };
}

#endif