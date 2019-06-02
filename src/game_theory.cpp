#include "game_theory.hpp"

Minion minions[NUM_MINIONS];
bool calculating = false;

void *fminion(void *identifier) {
  pthread_exit(0);
}

int main() {
  srand(time(NULL));

  pthread_t minions_threads[NUM_MINIONS];
  int *id;

  Strategies strats = Strategies();
  Generations generation = Generations(NUM_INTERACTIONS);

  for (int min_indx = 0; min_indx < NUM_MINIONS; ++min_indx) {
    id = (int *) malloc(sizeof(int));
    *id = min_indx;
    pthread_create(&minions_threads[min_indx], NULL, fminion, (void *) (id));
  }

  for (int min_indx = 0; min_indx < NUM_MINIONS; ++min_indx) {
    pthread_join(minions_threads[min_indx], NULL);
  }

  return 0;
}
