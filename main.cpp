#include <thread>

#include "execution_units/execution.hpp"
#include "manager/manager.hpp"

int main() {
  Buffer_Manager buffer{0, 0, 0};

  std::thread Reader(readChunk, std::ref(buffer), BLOCK_SIZE);

  std::thread EUs(entry, std::ref(buffer));

  std::thread Print(printBufferManager, std::ref(buffer));

  Reader.join();
  EUs.join();
  Print.join();

  return 0;
}