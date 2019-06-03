#include "game_theory.hpp"

Minion minions[NUM_MINIONS];
bool calculating = false;
int interactions = 0;
pthread_mutex_t lock_minions = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_interactions = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gru_cond = PTHREAD_COND_INITIALIZER;
bool gru_recruiting = false;

void *fgru(void *identifier) {
  pthread_exit(0);
}


void *fminion(void *identifier) {
  int id = *((int *) identifier);
  free((int *)identifier);
  int partner = id;
  int sleep_time;

  // cout << "Created Minion id = " << id << " | Strategy = "
    // << minions[id].strategy.name << endl;
  sleep(limited_rand(0, 3));
  while (1) {
    partner = limited_rand(0, NUM_MINIONS - 1);
    while (partner == id) {
      partner = limited_rand(0, NUM_MINIONS - 1);
    }
    pthread_mutex_lock(&lock_minions);
    if (interactions >= NUM_INTERACTIONS) {
      pthread_mutex_unlock(&lock_minions);
      break;
    }
    ++interactions;
    while (minions[id].occupied || minions[partner].occupied) {
      if (minions[id].occupied) {
        cout << "Minion " << id << " is already in an interaction." << endl;
        pthread_cond_wait(&minions[id].mcond, &lock_minions);
      } else if (minions[partner].occupied) {
        cout << "Minion " << id << " is waiting partner " << partner << "." << endl;
        pthread_cond_wait(&minions[partner].mcond, &lock_minions);
      }
    }
    minions[id].occupied = true;
    minions[partner].occupied = true;
    sleep_time = minions[id].strategy.interaction_duration;
    cout << "Minion " << id << " is interacting with " << partner << endl;
    cout << "Num interactions = " << interactions << endl;
    pthread_mutex_unlock(&lock_minions);

    sleep(sleep_time);

    pthread_mutex_lock(&lock_minions);
    minions[id].occupied = false;
    minions[partner].occupied = false;
    pthread_cond_broadcast(&minions[id].mcond);
    pthread_cond_broadcast(&minions[partner].mcond);
    cout << "Minion " << id << " is done with " << partner << endl;
    pthread_mutex_unlock(&lock_minions);
    // cout << "Interactions = " << interactions << endl;
    sleep(5);

  }
  cout << "DONE!" << endl;

  pthread_exit(0);
}

int main() {
  srand(time(NULL));

  pthread_t minions_threads[NUM_MINIONS];
  pthread_t gru;
  int *id;

  Strategies strats = Strategies();
  Generations generation = Generations(NUM_INTERACTIONS);
  generation.initializeMinions(strats);

  for (int min_indx = 0; min_indx < NUM_MINIONS; ++min_indx) {
    id = (int *) malloc(sizeof(int));
    *id = min_indx;
    pthread_create(&minions_threads[min_indx], NULL, fminion, (void *) (id));
  }
  id = (int *) malloc(sizeof(int));
  *id = 0;
  pthread_create(&gru, NULL, fgru, (void *) (id));
  for (int min_indx = 0; min_indx < NUM_MINIONS; ++min_indx) {
    pthread_join(minions_threads[min_indx], NULL);
  }
  pthread_join(gru, NULL);

  return 0;
}
