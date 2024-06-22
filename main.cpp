#include "execution_units/execution.hpp"
#include "manager/manager.hpp"

int main() {
  Buffer_Manager buffer{0, 0, 0};

  for (int i = 0; i < 10; i++) {
    readChunk(buffer);
  }

  for (int i = 0; i < 10; i++) {
    entry(buffer);
  }

  return 0;
}