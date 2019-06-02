#include "Strategies.hpp"

Strategies::Strategies() {
  Strategy funny =  Strategy("Funny", 1);
  funny.addPayoff("Funny", 3);
  funny.addPayoff("Evil", 0);
  addStrategy("Funny", funny);

  Strategy evil =   Strategy("Evil",  2);
  evil.addPayoff("Funny", 5);
  evil.addPayoff("Evil", 1);
  addStrategy("Evil", evil);
}

Strategy Strategies::getStrategy(string strat) {
  if (strategies.count(strat) > 0) {
    return strategies.at(strat);
  }
  return Strategy();
}

void Strategies::addStrategy(string strat_name, Strategy strat) {
  strategies.insert( pair<string, Strategy> (strat_name, strat));
}

void Strategies::listTable() {
  cout << "Listing Stategies Table" << endl;
  for (const auto &pair_strat : strategies) {
    cout << pair_strat.first << endl;
    for (const auto &pair_payoff : pair_strat.second.payoff) {
        cout << "     " << pair_payoff.first << " : " << pair_payoff.second << endl;
    }
    cout << endl;
  }
}
