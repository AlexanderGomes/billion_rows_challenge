#ifndef EXECUTION_HPP
#define EXECUTION_HPP

#include <limits>
#include <mutex>
#include <string>
#include <vector>

#include "../manager/manager.hpp"

struct Station {
  std::string name;
  double temperature;

  Station(std::string& name, double temperature)
      : name(name), temperature(temperature) {}
};


void entry(Buffer_Manager& buffer_Manager);
void parsing(std::vector<char>& block);
void computation(std::vector<Station>& stations);

#endif