#ifndef MINION
#define MINION

#include <pthread.h>

#include "Strategy.hpp"
#include "helper.hpp"

class Minion {
public:
  Minion();
  Minion(int, Strategy);
  int id;
  pthread_cond_t mcond = PTHREAD_COND_INITIALIZER;
  bool occupied = false;
  Strategy strategy = Strategy();
};

#endif
