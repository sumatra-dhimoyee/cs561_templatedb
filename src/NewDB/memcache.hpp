#ifndef _TEMPLATEDB_SST_H_
#define _TEMPLATEDB_SST_H_

#include <fstream>
#include <iostream>
#include <iterator>
#include "sst.hpp"
#include "block.hpp"



namespace templatedb {
    template<typename K, typename V>
    class MemCache {
        private:   
            static vector<Entry<K,V>> memcache;
            static size_t maxBufferSize;
            static size_t bufferSize;


        public:
            MemCache(size_t _bufferSize, vector<Entry<K, V>> _memcache){
                memcache=_memcache;
                maxBufferSize=_bufferSize;
                bufferSize=0;
                
            }
            bool put(K key, std::vector<V> value);
            Entry<K,V>& getEntry(K key);
            bool updateEntry(K key, std::vector<V> value);
            bool deleteEntry(K key);
            bool readRange(K startKey, K endKey);
            bool deleteRange(K startKey, K endKey);
            bool compareEntries (Entry<K,V> i,Entry<K,V> j);
            void sort();
            vector<Entry<K,V>>& getMemcache();
            int getBufferSize();
            bool isEmpty();
            vector<K>& buildKeyVector();
            vector<vector<V>>& buildValueVector();
            vector<V>& buildIntValueVector();

    };
}




#endif