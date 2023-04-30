#ifndef _TEMPLATEDB_FP_H_
#define _TEMPLATEDB_FP_H_

#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <chrono>


namespace templatedb
{
    template<typename K>
    struct zone
    {
        K min;
        K max;
        int block_index;
        zone(K _min, K _max, int _block_index)
        {
            min = _min;
            max= _max;
            block_index = _block_index;
        }
    }

}