#ifndef _TEMPLATEDB_BLOCK_H_
#define _TEMPLATEDB_BLOCK_H_

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <ctime>
#define TARGET_SIZE 4096



namespace templatedb
{
    template<typename K, typename V>
    class Entry
    {
        public:
            K key;
            std::vector<V> value;
            bool tomb;
            std::chrono::time_point<std::chrono::system_clock> TS;
            Entry(K _key, std::vector<V> _value, bool _tomb, std::chrono::time_point<std::chrono::system_clock> _TS)
            {
                key = _key;
                value = _value;
                tomb = _tomb;
                TS = _TS;
            }   
    };

    template<typename K,typename V>
    class Block
    {
        public:
        std::vector<Entry<K,V> > data;
        // std::vector<K> keys;
        // std::vector<std::vector<V>> values;
        size_t offset;
        
        //add fence pointers later
        Block(std::vector<Entry<K,V>> _data ,int _offset)
        {
            data = _data;
            // keys = _keys;
            // values = _values;
            offset = _offset;
        }

    };

    template<typename K,typename V>
    class Build_Block
    {
        private:
        std::vector<Entry<K,V> > data;
        int entries_kept;
        // std::vector<K> keys;
        // std::vector<std::vector<V>> values;
        size_t size;
        public:
        Build_Block(std::vector<Entry<K,V> > _data);
        bool add_KV(Entry<K,V> entry);
        static int add_to_block(Block<K,V>& block, std::vector<Entry<K,V> > _data);
        //bool add_KV_list(std::vector<K,vector<V>> kv_pairs);
        bool check_size(Entry<K,V>& entry);
        size_t current_size();
        int enteries_kept_size();
        void print_block();
        Block<K,V> build();

    };

}

#endif