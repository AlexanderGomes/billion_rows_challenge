
#include "manager.hpp"

void readChunk(Buffer_Manager &buffer_manager) {
  std::ifstream file("data/measurements.txt");

  if (!file.is_open()) {
    std::cerr << "Unable to open file measurements.txt" << std::endl;
    return;
  }

  file.seekg(buffer_manager.curr_offset);

  if (!file.good()) {
    std::cerr << "Error seeking to offset" << buffer_manager.curr_offset;
    file.close();
    return;
  }

  const int blockSize = 512;
  std::vector<char> buffer(blockSize);

  file.read(buffer.data(), blockSize);

  const int currBlockId = buffer_manager.last_processed_block_id + 1;

  buffer_manager.hash_map[currBlockId] = buffer;
  buffer_manager.curr_offset += blockSize;
  buffer_manager.num_blocks_in_memory++;
  buffer_manager.last_processed_block_id = currBlockId;

  file.close();
}

void printBufferManager(const Buffer_Manager &buffer_manager) {
  std::cout << "Buffer Manager State:" << std::endl;
  std::cout << "Last Processed Block ID: "
            << buffer_manager.last_processed_block_id << std::endl;
  std::cout << "Number of Blocks in Memory: "
            << buffer_manager.num_blocks_in_memory << std::endl;
  std::cout << "Current Offset: " << buffer_manager.curr_offset << std::endl;
  std::cout << "Hash Map Contents:" << std::endl;

  for (const auto &pair : buffer_manager.hash_map) {
    std::cout << "Block ID: " << pair.first << ", Size: " << pair.second.size()
              << " bytes" << std::endl;
  }
}