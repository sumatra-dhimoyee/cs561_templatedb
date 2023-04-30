#include "block.hpp"

using namespace templatedb;

template<typename K,typename V>
Build_Block<K, V>::Build_Block(std::vector<Entry<K,V> > _data, Bloomfilter& bf)
{
    
    int ret = 0;
    this->size = sizeof(size_t);
    for (int i = 0; i <  _data.size(); i++)
    {
        size_t block_size = this->size + sizeof(K)+sizeof(std::chrono::time_point<std::chrono::system_clock>) + sizeof(bool) + sizeof(V)*_data[i].value.size();
        
        if (block_size <= TARGET_SIZE)
        {
            this->data.push_back(_data[i]);
            this->size =  block_size;
            ret++;
            bf.program(std::to_string(_data[i].key));


        }
        else
        {
            std::cout<<"FINAL SIZE: "<<this->size<<std::endl;

            break;
        }

    }
    this->entries_kept = ret;
    
    
}

template<typename K,typename V>
bool Build_Block<K, V>::add_KV(Entry<K,V> entry)
{
    //check if size after adding will exceed target size
    //can change later according to the serialization implementation
    
    if (this->check_size(entry))
    {
        this->data.push_back(entry);
        // std::cout<<"SIZE BEFORE: "<<this->size<<std::endl;
        this->size = this->size + sizeof(K)+sizeof(std::chrono::time_point<std::chrono::system_clock>) + sizeof(bool) + sizeof(V)*entry.value.size();
        size_t temp = sizeof(K)+sizeof(std::chrono::time_point<std::chrono::system_clock>) + sizeof(bool) + sizeof(V)*entry.value.size();

        // std::cout<<"SIZE: "<<this->size<<std::endl; 
        // std::cout<<"TEMP SIZE: "<<temp<<std::endl; 
        return true;
    }

    
    return false;
    
}
template<typename K,typename V>
bool Build_Block<K, V>:: check_size(Entry<K,V>& entry)
{
    // size_t vec_size = keys.size()*sizeof(K);
    size_t vec_size = this->data.size()*(sizeof(K)+sizeof(std::chrono::time_point<std::chrono::system_clock>) + sizeof(bool));
    for (int i = 0; i < this->data.size(); i++)
    {
        vec_size = vec_size + this->data[i].value.size()*sizeof(V);
    }
    if (sizeof(K)+sizeof(std::chrono::time_point<std::chrono::system_clock>) + sizeof(bool) + sizeof(V)*entry.value.size()+ vec_size <= TARGET_SIZE)
    {
        return true;
    }
    else
    {
        return false;
    }

}
template<typename K, typename V>
int Build_Block<K,V>::enteries_kept_size()
{
    return this->entries_kept;
}
template<typename K,typename V>
Block<K,V> Build_Block<K, V>:: build()
{
    Block<K,V> block = Block<K,V>(this->data, this->size);
    return block;

}
template<typename K,typename V>
void Build_Block<K, V>::print_block()
{
    std::cout<<"PRINTING BLOCK ..."<<std::endl;
    for (int i = 0; i< this->data.size(); i++)
    {
        std::cout<<this->data[i].key<<" { ";
        for (int j = 0 ; j < this->data[i].value.size(); j++)
        {
            std::cout<< this->data[i].value[j]<<", ";
        }
        std::cout<<"}"<<std::endl;
    }

}

template<typename K,typename V>
int Build_Block<K, V>::add_to_block(Block<K,V>& block, std::vector<Entry<K,V> > _data)
{
    //need to enure it is sorted after adding
    //not incorporated yet
    //have to implement merge sort
    int ret = 0;
   
    for (int i = 0; i <  _data.size(); i++)
    {
        size_t block_size = block.offset + sizeof(K)+sizeof(std::chrono::time_point<std::chrono::system_clock>) + sizeof(bool) + sizeof(V)*_data[i].value.size();
         if(block.data.size()==0)
         {
            block_size = block_size + sizeof(size_t);
         }
        if (block_size <= TARGET_SIZE)
        {
            block.data.push_back(_data[i]);
            block.offset =  block_size;
            ret++;

        }
        else
        {
            // std::cout<<"FINAL SIZE: "<<block.offset<<std::endl;
            return ret;
        }

    }
    return ret;

}
template<typename K, typename V>
size_t Build_Block<K,V> ::current_size()
{
    return this->size;
}

template<typename K, typename V>
K  Build_Block::block_min()
{
    
    K min_key = this->data[0].key;
    for (auto entry : block.data)
    {
        if (this->key < min_key)
        {
            min_key = this->key;
        }
    }
    return min_key;

}
template<typename K, typename V>
K  Build_Block::block_max()
{
    
    K max_key = this->data[0].key;
    for (auto entry : this->data)
    {
        if (this->key > max_key)
        {
            max_key = this->key;
        }
    }
    return max_key;

}


Block<K,V> Block<K,V>::getBlock(){
    return this->data;
}

template<typename K, typename V>
bool Block<K,V>::compareEntries(const Entry<K,V>& a, const Entry<K,V>& b){
    return a.key<b.key;
}

template<typename K, typename V>
int Block<K,V>:: binarySearch(std::vector<Entry<K,V>> entries, int l, int r, K key){
    while(l<=r){
        int m = l+(r-1)/2;
        if(entries[m].key == key){
            return m;
        }
        if(entries[m].key < key){
            l= m+1;
        }else{
            r = m-1;
        }
    }
    return -1;
}

template<typename K, typename V>
Entry<K,V> Block<K,V>::getEntry(K key){

    std::vector<Entry<K,V>> entries=this->data;
    int n= sizeof(entries)/sizeof(entries[0]);
    int isFound= binarySearch(entries, 0, n-1, key);
    if(isFound!=-1){
        return entries[isFound];
    }else{
        throw std::runtime_error("Entry not found.");
    }   
}

template<typename K, typename V>
bool Block<K,V>::entryExist(K key){
    // rethink for space
    std::vector<Entry<K,V>> entries=this->data;
    int n= sizeof(entries)/sizeof(entries[0]);
    int isFound= binarySearch(entries, 0, n-1, key);
    if(isFound!=-1){
        return true;
    }else{
        return false;
    }
}
