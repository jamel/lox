#include "str.h"

#include <stdlib.h>
#include <string.h>

lox_str_t* lox_str_new(u32 len) {
  u64 bytes = sizeof(lox_str_t) + len + 1;
  lox_str_t* str = (lox_str_t*)malloc(bytes);
  if (!str) {
    return NULL;
  }
  str->len = len;
  return str;
}

void lox_str_free(lox_str_t* str) {
  free(str);
}

lox_str_t* lox_str_copy(const char* src) {
  u32 len = strlen(src);
  lox_str_t* str = lox_str_new(len);
  if (!str) {
    return NULL;
  }
  memcpy(str->buf, src, len);
  str->buf[len] = '\0';
  return str;
}
