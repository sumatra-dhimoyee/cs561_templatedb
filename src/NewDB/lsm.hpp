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

namespace templatedb
{

    template<typename K, typename V>
    class LSM
    {
        private:
        std::vector<level<K,V>> levels;
        MemCache memcache;
        uint8_t no_levels;
        size_t mem_size;
        uint8_t T_ratio;
        bool leveled;

        public:

        LSM(std::vector<K,V> entries, size_t _mem_size, uint8_T T_ratio, bool leveled);
        void add_buffer(std::vector<K,V> entries, levelled);

        // void put(Entry<K,V> entry);

        //create function which takes in a vector of Entry and puts it in memtable
        //create function which takes the data in memcache after it's full add it to the levels. 
        

    };
}

#endif