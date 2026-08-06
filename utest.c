#include "utest.h"

#include <assert.h>
#include <stdio.h>

static test_desc_t* g_tests;

void register_test(test_desc_t* test) {
  assert(!test->next);

  test->next = g_tests;
  g_tests = test;
}

int main() {
  for (test_desc_t* test = g_tests; test; test = test->next) {
    printf("[RUN] %s\n", test->name);
    test->fn();
  }
}
