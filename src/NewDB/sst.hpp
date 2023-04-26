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
        std::vector<Block<K,V>> data;
        size_t offset;
        int run;
        int level;

    }

    template<typename K,typename V>
    class Build_SST
    {
        private:
            std::vector<Block<K,V>> data;
            size_t size;
            int run;
            int level;
            //Add member bloom filter later on
        public:
        Build_SST(std::vector<K>& keys, std::vector<std::vector<V> values);
        bool add_KV_list(std::vector<K>& keys, std::vector<std::vector<V> values);
        static int add_flush(SST<K,V> sst, std::vector<K>& keys, std::vector<std::vector<V> values );
        void print_sst();
        SST<K,V> build();



    }
}




#endif