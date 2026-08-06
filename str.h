#pragma once

#include "types.h"

typedef struct {
  u32 len;
  const char* buf;
} lox_slice_t;

typedef struct {
  u32 len;
  char buf[];
} lox_str_t;

lox_str_t* lox_str_new(u32 len);
void lox_str_free(lox_str_t* str);
lox_str_t* lox_str_copy(const char* src);

static inline lox_slice_t lox_str_slice(const lox_str_t* str) {
  return (lox_slice_t){
      .len = str->len,
      .buf = str->buf,
  };
}
