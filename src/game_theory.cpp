#include "game_theory.hpp"

Minion minions[NUM_MINIONS];
Strategies strats = Strategies();
Generations generation = Generations(NUM_INTERACTIONS, strats);
bool calculating = false;
bool gen_calc = false;
int interactions = 0;
pthread_mutex_t lock_minions = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_interactions = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gru_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t gen_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t minion_done = PTHREAD_COND_INITIALIZER;
bool gru_recruiting = false;
int to_recruit = 0;
bool gru_generation = false;
bool end = false;
sem_t gru_recruited;
list <int> minions_recruited;
int done_minions = 0;

void *fgen(void *identifier) {
  int id = *((int *) identifier);
  free((int *)identifier);
  while (generation.num_gen < NUM_GEN) {
    generation.startGeneration();
    pthread_mutex_lock(&lock_minions);
    pthread_cond_wait(&minion_done, &lock_minions);
    pthread_mutex_unlock(&lock_minions);
    cout << "Done with one generation, gen: " << generation.num_gen << endl;
    cout << "Score A = " << generation.scores.at(generation.num_gen - 1).at(0) << endl;
    cout << "Score B = " << generation.scores.at(generation.num_gen - 1).at(1) << endl;
    generation.endGen();
  }
  cout << "Finish Here!" << endl;
  pthread_exit(0);
}

void *fgru(void *identifier) {
  int id = *((int *) identifier);
  free((int *)identifier);
  int min_minions = NUM_MINIONS/5;
  int max_minions = NUM_MINIONS/2;

  while(1) {
    sleep(GRU_WAIT);
    to_recruit = limited_rand(min_minions, max_minions);

    pthread_mutex_lock(&lock_minions);
    while (!gru_generation) {
      pthread_cond_wait(&gru_cond, &lock_minions);
    }
    cout << "!!! Gru is recruiting " << to_recruit << " minions!" << endl;
    gru_recruiting = true;
    pthread_mutex_unlock(&lock_minions);

    sem_wait(&gru_recruited);
    pthread_mutex_lock(&lock_minions);
    gru_recruiting = false;
    cout << "!!! Recruiting number reached!" << endl;
    pthread_mutex_unlock(&lock_minions);

    sleep(limited_rand(5, 7));

    pthread_mutex_lock(&lock_minions);
    cout << "!!! Releasing Minions!" << endl;
    for (auto minion_indx : minions_recruited) {
    cout << "!!! Released Minion : " << minion_indx << endl;
      minions[minion_indx].occupied = false;
      pthread_cond_broadcast(&minions[minion_indx].mcond);
    }
    minions_recruited.clear();
    pthread_mutex_unlock(&lock_minions);
  }

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
    while (!gen_calc) {
      cout << "Waiting for generation to start" << endl;
      pthread_cond_wait(&gen_cond, &lock_minions);
    }
    while (minions[id].occupied ||
           minions[partner].occupied ||
           (gru_recruiting && (to_recruit > 0))) {
      if (minions[id].occupied) {
        cout << "Minion " << id << " is already occupied." << endl;
        pthread_cond_wait(&minions[id].mcond, &lock_minions);
      } else if (gru_recruiting && (to_recruit > 0)) {
        minions[id].occupied = true;
        --to_recruit;
        minions_recruited.push_back(id);
        cout << "!!! Minion " << id << " RECRUITED!" << endl;
        if (to_recruit <= 0) {
          cout << "!!! Minion " << id << " REC FIN!" << endl;
          sem_post(&gru_recruited);
        }
        pthread_cond_wait(&minions[id].mcond, &lock_minions);
      } else if (minions[partner].occupied) {
        cout << "Minion " << id << " is waiting partner " << partner << "." << endl;
        pthread_cond_wait(&minions[partner].mcond, &lock_minions);
      }
    }
    if (interactions >= NUM_INTERACTIONS) {
      gru_generation = false;
      ++done_minions;
      cout << "DONE!" << endl;
      if (done_minions == NUM_MINIONS) {
        pthread_cond_signal(&minion_done);
      }
      pthread_cond_wait(&gen_cond, &lock_minions);
      pthread_mutex_unlock(&lock_minions);
      continue;
    }
    ++interactions;
    minions[id].occupied = true;
    minions[partner].occupied = true;
    sleep_time = minions[id].strategy.interaction_duration;
    cout << "Minion " << id << " is interacting with " << partner << endl;
    cout << "Num interactions = " << interactions << endl;
    generation.addScore(minions[id].strategy, minions[partner].strategy);
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

  pthread_exit(0);
}

int main() {
  srand(time(NULL));

  pthread_t minions_threads[NUM_MINIONS];
  pthread_t gru;
  pthread_t gen;
  int *id;
  sem_init(&gru_recruited, 0, 0);

  generation.initializeMinions();

  id = (int *) malloc(sizeof(int));
  *id = 0;
  pthread_create(&gru, NULL, fgru, (void *) (id));
  id = (int *) malloc(sizeof(int));
  *id = 0;
  pthread_create(&gen, NULL, fgen, (void *) (id));

  for (int min_indx = 0; min_indx < NUM_MINIONS; ++min_indx) {
    id = (int *) malloc(sizeof(int));
    *id = min_indx;
    pthread_create(&minions_threads[min_indx], NULL, fminion, (void *) (id));
  }

  // for (int min_indx = 0; min_indx < NUM_MINIONS; ++min_indx) {
  //   pthread_join(minions_threads[min_indx], NULL);
  // }
  // pthread_join(gru, NULL);
  pthread_join(gen, NULL);

  return 0;
}
