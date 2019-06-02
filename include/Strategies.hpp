#ifndef STRATEGIES
#define STRATEGIES

#include <vector>
#include <map>
#include <string>

#include "Strategy.hpp"

using ::std::vector;
using ::std::map;
using ::std::string;

class Strategies {
public:
  vector < vector<int> > scores;
  map <string, Strategy> strats;
};

#endif
