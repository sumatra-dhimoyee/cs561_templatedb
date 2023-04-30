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

    template<typename K, typename V>
    class Level
    {
        private:
        std::vector<SST<K,V> sst_vector;
        uint8_t no_runs;
        size_t run_size;
        size_t level_size;
        uint8_t level;
    
        public:

        Level(std::vector<Entry<K,V>> _sst_vector, int _no_runs, size_t _level_size, uint8_t level);
        bool add_sst(std::vector<Entry<K,V>>, bool merge);
        SST<K,V> merge_runs();



    };


}


#endif