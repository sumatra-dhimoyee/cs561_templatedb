#ifndef _TEMPLATEDB_LEVEL_H_
#define _TEMPLATEDB_LEVEL_H_

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "sst.hpp"



namespace templatedb
{
    template<typename K>
    struct zone
    {
        K min;
        K max;
        int block_index;
    }
    template<typename K, typename V>
    class Level
    {
        private:
        std::vector<SST<K,V> sst_vector;
        uint8_t no_runs;
        size_t run_size;
        size_t level_size;
        uint8_t level;
        std::vector<vector<zone<K>> FP;
        std::vector<BloomFilter> BF;

    
        public:

        Level(SST<K,V> sst, int _no_runs, size_t _level_size, uint8_t level, std::vector<zone<K>>& fp, BloomFilter& bf);
        bool add_sst(std::vector<Entry<K,V>>, bool leveled, std::vector<zone<K>>& fp, BloomFilter& bf);
        SST<K,V> merge_runs(std::vector<zone<K>>& fp,  BloomFilter& bf);
        int get_block_index(int sorted_run, K key);
        int lookup(int sorted_run, int block_index, K key);
        int bloom_lookup(K key);



    };


}


#endif