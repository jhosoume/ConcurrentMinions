#include "Minion.hpp"

Minion::Minion(int identifier, Strategy strat)
  : id{identifier}, strategy{strat}
  {}
Minion::Minion()
  : id{0}
  {}
