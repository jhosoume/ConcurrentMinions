#include "Generations.hpp"

Generations::Generations(int num_interactions)
  : num_interactions{num_interactions}
  {}

void Generations::initializeMinions(Strategies strat) {
  int indx = 0;
  while(indx < NUM_MINIONS) {
    for(;indx < NUM_S_A; ++indx) {
      minions[indx] = Minion(indx, strat.getStrategy("Funny"));
    }
    for(;indx < NUM_S_A + NUM_S_B; ++indx) {
      minions[indx] = Minion(indx, strat.getStrategy("Evil"));
    }
  }
}
