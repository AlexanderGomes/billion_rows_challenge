
#include "manager.hpp"

void readChunk(Buffer_Manager &buffer_manager, int blockSize) {
  std::ifstream file("data/measurements.txt");

  if (!file.is_open()) {
    std::cerr << "Unable to open file measurements.txt" << std::endl;
    return;
  }

  while (true) {
    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);

    buffer_manager.need_data.wait(lck, [&buffer_manager] {
      return buffer_manager.num_blocks_in_memory < MAX_BLOCKS_IN_MEMORY;
    });

    bool EU_STARTED = false;

    if (buffer_manager.num_blocks_in_memory >= EU_MIN_BLOCKS) {
      if (!EU_STARTED) {
        buffer_manager.EU_start.notify_one();
        EU_STARTED = true;
      }
    }

    file.seekg(buffer_manager.curr_offset);

    if (!file.good()) {
      std::cerr << "Error seeking to offset" << buffer_manager.curr_offset;
      file.close();
      return;
    }

    std::vector<char> buffer(blockSize);

    file.read(buffer.data(), blockSize);

    if (file.gcount() == 0) {
      std::cerr << "No more data to read" << std::endl;
      file.close();
      return;
    }

    const int currBlockId = buffer_manager.last_processed_block_id + 1;

    buffer_manager.hash_map[currBlockId] = buffer;
    buffer_manager.curr_offset += blockSize;

    buffer_manager.mtx.lock();
    buffer_manager.num_blocks_in_memory++;
    buffer_manager.mtx.unlock();

    buffer_manager.last_processed_block_id = currBlockId;
    buffer_manager.need_data.notify_one();
  }

  file.close();
}

void printBufferManager(const Buffer_Manager &buffer_manager) {
  while (true) {
    std::cout << "Buffer Manager State:" << std::endl;
    std::cout << "Last Processed Block ID: "
              << buffer_manager.last_processed_block_id << std::endl;
    std::cout << "Number of Blocks in Memory: "
              << buffer_manager.num_blocks_in_memory << std::endl;
    std::cout << "Current Offset: " << buffer_manager.curr_offset << std::endl;
    std::cout << "Hash Map Contents:" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}