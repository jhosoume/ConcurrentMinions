#ifndef MINION
#define MINION

#include <pthread.h>

class Minion {
public:
  int id;
  pthread_cond_t minion_cond;
};

#endif
