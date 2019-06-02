#ifndef STRATEGIES
#define STRATEGIES

#include <vector>
#include <map>
#include <string>
#include <iostream>

#include "Strategy.hpp"

using ::std::vector;
using ::std::map;
using ::std::string;
using ::std::cout;
using ::std::endl;

class Strategies {
public:
  Strategies();
  map <string, Strategy> strategies;

  Strategy getStrategy(string);
  void addStrategy(string, Strategy);
  void listTable();
};

#endif
