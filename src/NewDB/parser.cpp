#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include "block.hpp"
#include "block.cpp"
#include "sst.hpp"
#include "sst.cpp"
#include "level.hpp"
#include "level.cpp"
#include "lsm.hpp"
#include "lsm.cpp"
#include "BloomFilter/BloomFilter.h"
#include "BloomFilter/BloomFilter.cpp"
#include "BloomFilter/murmurhash.cpp"
#include "BloomFilter/murmurhash.h"

using namespace std;

int main() {

    size_t mem_size = 4*1024;
    uint8_t T_ratio = 3;
    int bf_num_elem = 1024;
    int bf_num_bits_per_elem = 10;
    bool leveled = true;
    templatedb::LSM<int,string> LeveledLSM = templatedb::LSM<int,int>(mem_size, T_ratio, leveled, bf_num_elem, bf_num_bits_per_elem);
    // Open the input file
    ifstream infile("workload.txt");

    // Create a hash table to store the key-value pairs
    

    // Read each line from the input file
    string line;
    while (getline(infile, line)) {
        // Parse the line into its operation, key, and value (if applicable)
        
        char op = line[0];
        string key, value;
        line = line.substr(2, line.length()-1);
        
        size_t pos = line.find(' ');
        // cout<<"POSSS "<< pos<<endl;
        key = line.substr(2, pos -1);
        // cout<<"KEYYY "<<key<<endl;
        if (op == 'I' || op == 'U') {
            value = line.substr(pos + 1);
             }

    
        // Perform the corresponding operation on the hash table
        switch (op) {
            case 'I':
                // table[key] = value;
                cout<<op<< " "<<key<<" "<<value<<endl;

                break;
            case 'U':
                cout<<op<< " "<<key<<" "<< value<<endl;
                // if (table.count(key)) {
                //     table[key] = value;
                // } else {
                //     cout << "Error: key not found for update: " << key << endl;
                // }
                break;
            case 'D':
                cout<<op<< " "<<key<<" "<< endl;
                // if (table.erase(key)) {
                //     cout << "Deleted key: " << key << endl;
                // } else {
                //     cout << "Error: key not found for delete: " << key << endl;
                // }
                // break;
            case 'Q':
                cout<<op<< " "<<key<<endl;
                // if (table.count(key)) {
                //     cout << "Value for key " << key << ": " << table[key] << endl;
                // } else {
                //     cout << "Error: key not found for query: " << key << endl;
                // }
                break;
            default:
                cout << "Error: unrecognized operation: " << op << endl;
                break;
        }
    }

    // Close the input file
    infile.close();

    return 0;
}
