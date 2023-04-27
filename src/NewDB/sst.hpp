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

namespace templatedb
{
    template<typename K, typename V>
    class SST
    {
        std::vector<Block<K,V>> block_vector;
        size_t offset;
        uint8_t run;
        uint8_t level;
        size_t max_size;
        bool overflow;
        //Add member bloom filter later on
        SST(std::vector<Block<K,V>> _block_vector, size_t _offset, uint8_t _run, uint8_t _level , size_t _max_size, bool _overflow)
        {
            block_vector = _block_vector;
            offset = _offset;
            run = _run;
            level = _level;
            max_size = _max_size;
            overflow = _overflow;

        }

    };

    template<typename K,typename V>
    class Build_SST
    {
        private:
            std::vector<Block<K,V>> block_vector;
            size_t size;
            uint8_t run;
            uint8_t level;
            size_t max_size;
            bool overflow;
            //Add member bloom filter later on
        public:
        Build_SST(std::vector<Entry<K,V>> _data,size_t max_size, uint8_t _run, uint8_t level, bool overflow);
        // bool add_KV_list(std::vector<K>& keys, std::vector<std::vector<V> values);
        bool merge_flush(SST<K,V>& second_sst);

        // void print_sst();
        SST<K,V> build();



    };
}




#endif