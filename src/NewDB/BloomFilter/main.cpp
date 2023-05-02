#include "BloomFilter.h"
#include "BloomFilter.cpp"
#include "murmurhash.cpp"
#include "murmurhash.h"
#include <iostream>

using namespace BF;

int main() {
    int num = 100;
    int x = 22;
    int y = 33;
    std::string str = std::to_string(num);
    std::string str1 = std::to_string(x);
    std::string str2 = std::to_string(y);
    BloomFilter bf(1000, 10); // Create a BloomFilter with 1000 elements and 10 bits per element
    bf.program(str); // Program the filter with "hello"
    bf.program(str1);
    // bf.program(str2);
    bool query_result = bf.query(str2); // Query the filter with "hello"
    if (query_result) {
        std::cout << "The element may be in the filter." << std::endl;
    } else {
        std::cout << "The element is definitely not in the filter." << std::endl;
    }
    return 0;
}