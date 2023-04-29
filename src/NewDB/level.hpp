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

#define LEVELS 4

namespace templatedb
{

    template<typename K, typename V>
    class Level
    {
        private:
        std::vector<SST<K,V> sst_vector;
        int no_runs;
        size_t run_size;
        uint8_t level;
    
        public:

        Level(std::vector<SST<K,V>> _sst_vector, int _no_runs, size_t _run_size, uint8_t _level);
        bool add_sst(std::vector<Entry<K,V>>, uint8_t level);
        SST<K,V> merge_sst(SST<K,V> first_sst, SST<K,V> second sst);



    };


}


#endif