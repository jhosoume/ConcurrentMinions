#ifndef GENERATION
#define GENERATION

#include <vector>
#include <iostream>
#include "parameters.hpp"
#include "Strategies.hpp"

using ::std::vector;
using ::std::cout;
using ::std::endl;

class Generations {
public:
  Generations(int);
  void initializeMinions(Strategies);
  void startGeneration();
  void endGen();
  int num_interactions;
  vector < vector<int> > scores;


};

#endif
