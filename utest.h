#pragma once

typedef void (*test_fn_t)();

typedef struct test_desc {
  struct test_desc* next;
  const char* name;
  test_fn_t fn;
} test_desc_t;

#define ATTR_CONSTRUCTOR __attribute__((constructor))

void register_test(test_desc_t* test);

#define TEST(test_name)                             \
  void test_name();                                 \
  ATTR_CONSTRUCTOR static void test_name##_init() { \
    static test_desc_t test = {                     \
        .next = NULL,                               \
        .name = #test_name,                         \
        .fn = test_name,                            \
    };                                              \
    register_test(&test);                           \
  }                                                 \
  void test_name()
