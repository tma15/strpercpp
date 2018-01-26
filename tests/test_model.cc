#include <stdio.h>

#include <strpercpp/early_update_perceptron.hpp>

#include "test.h"

using namespace strpercpp;

void test_early_update() {
  printf("model test\n");
//  GreedyEarlyUpdate perc;
  StructuredPerceptron* perc = new GreedyEarlyUpdate();
}
