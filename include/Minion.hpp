#ifndef MINION
#define MINION

#include <pthread.h>

#include "Strategy.hpp"
#include "helper.hpp"

class Minion {
public:
  int id;
  pthread_cond_t minion_cond = PTHREAD_COND_INITIALIZER;
  bool occupied = false;
  Strategy strat = Strategy();
};

#endif
