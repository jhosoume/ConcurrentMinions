#include "helper.hpp"

int limited_rand(int min, int max) {
  return min + rand() % (max + 1 - min);
}
