#ifndef GENERATION
#define GENERATION

#include <vector>
#include <iostream>
#include <cmath>

#include "parameters.hpp"
#include "Strategies.hpp"
#include "Minion.hpp"

using ::std::vector;
using ::std::cout;
using ::std::endl;

class Generations {
public:
  Generations(int, Strategies);
  void initializeMinions();
  void startGeneration();
  void addScore(Strategy, Strategy);
  void endGen();
  int num_interactions;
  vector < vector<int> > scores;
  Strategies strat;
  int num_gen = 0;

};

#endif
