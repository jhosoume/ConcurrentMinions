#ifndef STRATEGY
#define STRATEGY

#include <string>
#include <map>

using ::std::string;
using ::std::map;
using ::std::pair;

class Strategy {
public:
  Strategy();
  Strategy(string, int);
  string name;
  int interaction_duration;
  map <string, int> payoff;

  int getPayoff(string);
  void addPayoff(string, int);

};

#endif
