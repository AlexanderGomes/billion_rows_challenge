#include "execution.hpp"

int next_block_id = 1;

void entry(Buffer_Manager& buffer_manager) {
  while (true) {
    std::mutex mtx;
    std::unique_lock<std::mutex> lck(mtx);

    buffer_manager.EU_start.wait(lck, [&buffer_manager] {
      return buffer_manager.num_blocks_in_memory >= EU_MIN_BLOCKS;
    });

    auto it = buffer_manager.hash_map.find(next_block_id);

    if (it != buffer_manager.hash_map.end()) {
      std::vector<char>& block = it->second;
      parsing(block);

      buffer_manager.hash_map.erase(it);

      buffer_manager.mtx.lock();
      buffer_manager.num_blocks_in_memory--;
      buffer_manager.mtx.unlock();
    }

    if (buffer_manager.num_blocks_in_memory == 1) {
      buffer_manager.need_data.notify_one();
    }
    
    next_block_id++;
  }
}

std::string broken_station_name;
std::string broken_temperature_str;
std::string last_cycle_name;

void parsing(std::vector<char>& block) {
  std::string station_name;
  double station_temperature;
  std::vector<Station> stations;
  std::string current_field;

  if (!broken_station_name.empty()) {
    current_field = broken_station_name;
    broken_station_name.clear();
  }

  if (!broken_temperature_str.empty()) {
    current_field = broken_temperature_str;
    station_name = last_cycle_name;
    broken_temperature_str.clear();
    last_cycle_name.clear();
  }

  for (int i = 0; i < block.size(); i++) {
    char curr_char = block[i];
    if (curr_char == ';') {
      station_name = current_field;
      current_field.clear();
    } else if (curr_char == '\n') {
      station_temperature = std::stod(current_field);
      stations.push_back(Station(station_name, station_temperature));
      current_field.clear();
      station_name.clear();
      station_temperature = std::numeric_limits<double>::quiet_NaN();
    } else {
      current_field += curr_char;
    }
  }

  if (!current_field.empty()) {
    if (station_name.empty()) {
      broken_station_name = current_field;
    } else {
      broken_temperature_str = current_field;
      last_cycle_name = station_name;
    }
  }

  computation(stations);
}

// min // mean // max
// sorted inputs
void computation(std::vector<Station>& stations) {
  std::cout << "next_block_id: " << next_block_id << '\n' << std::endl;
}