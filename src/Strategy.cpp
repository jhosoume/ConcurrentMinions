#include "Strategy.hpp"

Strategy::Strategy()
  : name{"Default"}, interaction_duration{1}
  {}

Strategy::Strategy(string name, int time)
  : name{name}, interaction_duration{time}
  {}

int Strategy::getPayoff(string other_strat) {
  if (payoff.count(other_strat) > 0) {
    return payoff.at(other_strat);
  }
  return 0;
}

void Strategy::addPayoff(string strat, int score) {
  payoff.insert( pair<string, int> (strat, score));
}
