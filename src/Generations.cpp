#include "Generations.hpp"

Generations::Generations(int num_interactions, Strategies strat)
  : num_interactions{num_interactions}, strat{strat}
  {}

void Generations::initializeMinions() {
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

void Generations::startGeneration() {
  vector<int> vec {0, 0};
  ++num_gen;
  scores.push_back(vec);
  pthread_mutex_lock(&lock_minions);
  gen_calc = true;
  gru_generation = true;
  interactions = 0;
  done_minions = 0;
  pthread_cond_broadcast(&gen_cond);
  pthread_cond_signal(&gru_cond);
  pthread_mutex_unlock(&lock_minions);
}

void Generations::addScore(Strategy stgy1, Strategy stgy2) {
    if (stgy1.name == "Funny") {
      scores.at(num_gen - 1).at(0) += stgy1.getPayoff(stgy2.name);
    } else {
      scores.at(num_gen - 1).at(1) += stgy1.getPayoff(stgy2.name);
    }
    if (stgy2.name == "Funny") {
      scores.at(num_gen - 1).at(0) += stgy2.getPayoff(stgy1.name);
    } else {
      scores.at(num_gen - 1).at(1) += stgy2.getPayoff(stgy1.name);
    }
}

void Generations::endGen() {
  int indx = 0;
  double total_score = scores.at(num_gen - 1).at(0) + scores.at(num_gen - 1).at(1);
  double num_a = round((scores.at(num_gen - 1).at(0) / total_score) * NUM_MINIONS);
  double num_b = round((scores.at(num_gen - 1).at(1) / total_score) * NUM_MINIONS);
  while(indx < NUM_MINIONS) {
    for(;indx < num_a; ++indx) {
      minions[indx].strategy = strat.getStrategy("Funny");
    }
    for(;indx < (num_a + num_b); ++indx) {
      minions[indx].strategy = strat.getStrategy("Evil");
    }
  }
}
