#ifndef _MURMUR_H_
#define _MURMUR_H_


#include <cstdint> 

unsigned int MurmurHash2 ( const void * key, int len, unsigned int seed );

#endif