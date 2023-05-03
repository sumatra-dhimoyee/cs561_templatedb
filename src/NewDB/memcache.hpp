#ifndef _TEMPLATEDB_MEM_H_
#define _TEMPLATEDB_MEM_H_

#include <fstream>
#include <iostream>
#include <iterator>
#include "sst.hpp"
#include "block.hpp"

using namespace std;

namespace templatedb {
    template<typename K, typename V>
    class MemCache {
        private:   
            vector<Entry<K,V>> memcache;
            size_t maxBufferSize;
            size_t bufferSize;


        public:
            MemCache(size_t _bufferSize, vector<Entry<K, V>> _memcache){
                memcache=_memcache;
                maxBufferSize=_bufferSize;
                bufferSize=0;
                
            }
            bool put(K key, std::vector<V> value);
            Entry<K,V>& getEntry(K key);
            bool entryExist(K key);
            bool updateEntry(K key, std::vector<V> value);
            bool deleteEntry(K key);
            bool readRange(K startKey, K endKey);
            bool deleteRange(K startKey, K endKey);
            bool compareEntries (Entry<K,V>& i,Entry<K,V>& j);
            int binarySearch(std::vector<Entry<K,V>> entries, int l, int r, K key);
            void sortEntries();
            vector<Entry<K,V>>& getMemcache();
            int getBufferSize();
            bool isEmpty();
            vector<K>& buildKeyVector();
            vector<vector<V>>& buildValueVector();
            vector<V>& buildIntValueVector();
            void clearMemcache();
            void printMemcache();

    };
}




#endif