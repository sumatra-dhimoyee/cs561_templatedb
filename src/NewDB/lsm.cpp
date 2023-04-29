#include "level.hpp"

using namespace templatedb;

template<typename K, typename V>
LSM<K,V>::LSM(std::vector<Entry<K,V>> entries, size_t _mem_size, uint8_t _T_ratio, bool _leveled)
{

    this->mem_size = _mem_size;
    this-> T_ratio = _T_ratio;
    this->leveled = _leveled;
    if (this->leveled)
    {this->no_runs = 1;}
    else
    {this->no_runs = T_ratio;}
}

template<typename K, typename V>
void LSM<K,V>::add_buffer(std::vector<K,V> entries, leveled)
{
    size_t _level_size = this->mem_size * T_ratio;
    size_t _run_size;
    if(leveled)
        _run_size = _level_size
    else
        _run_size = (_level_size)/this->T_ratio


    Build_SST<K,V> sst_builder = Build_SST<K,V>(entries, _run_size, this->level);
    SST<K,V> sst = sst_builder.build();
        bool x = false;
        int i = 0;
        while (!x)
        {
            if(i < this->levels.size())
            {
                x = level[i].add_sst(sst, this->leveled);
                if(!x)
                {
                    sst = level[i].merge_runs();
                    level[i].clear();
                    i++;
                }
            }
            else{
                _level_size = this->mem_size * pow(this->T_ratio, i+1);
                Level<K,V> lvl = Level<K,V>(sst, this->no_runs, level_size, i);
                this->levels.push_back(lvl);
                x = true;
                
            }
        }

    
}