#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

struct Buffer_Manager {
  int last_processed_block_id;
  int num_blocks_in_memory;
  int64_t curr_offset;
  std::condition_variable EU_start;
  std::condition_variable need_data;
  std::unordered_map<int, std::vector<char> > hash_map;
  std::mutex mtx;
};

constexpr size_t BLOCK_SIZE = 128 * 1024 * 1024;
constexpr size_t MAX_BLOCKS_IN_MEMORY = 8;
constexpr size_t EU_MIN_BLOCKS = 1;

void readChunk(Buffer_Manager &buffer_manager, int readChunk);
void printBufferManager(const Buffer_Manager &buffer_manager);

#endif