#ifndef PARAMETERS
#define PARAMETERS

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>

#include "Minion.hpp"

#define NUM_MINIONS 50
#define NUM_S_A 25
#define NUM_S_B 25
#define NUM_INTERACTIONS 50
#define GRU_WAIT 8
#define NUM_GEN 4

extern Minion minions[NUM_MINIONS];
extern bool calculating;
extern bool gen_calc;
extern int interactions;
extern pthread_mutex_t lock_minions;
extern pthread_mutex_t lock_interactions;
extern pthread_cond_t gru_cond;
extern pthread_cond_t gen_cond;
extern bool gru_recruiting;
extern int to_recruit;
extern bool gru_generation;
extern sem_t gru_recruited;
extern int done_minions;


#endif
