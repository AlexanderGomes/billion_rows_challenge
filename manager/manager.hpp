#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


struct Buffer_Manager {
  int last_processed_block_id;
  int num_blocks_in_memory;
  int64_t curr_offset;
  std::unordered_map<int, std::vector<char> > hash_map;
};


void readChunk(Buffer_Manager &buffer_manager);
void printBufferManager(const Buffer_Manager &buffer_manager);

#endif