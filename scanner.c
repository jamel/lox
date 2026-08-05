#include "scanner.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

#include "str.h"

#define LOX_TOKEN(type_, off_, len_)                 \
  (lox_token_t) {                                    \
    .type = LOX_TT_##type_, .off = off_, .len = len_ \
  }

static inline bool
_lox_token_is_valid_off(const lox_token_t* token, lox_slice_t str) {
  return (token->off < str.len) && ((token->off + token->len) <= str.len);
}

static inline bool _lox_scanner_at_end(lox_scanner_t* scanner) {
  return scanner->off == scanner->input.len;
}

static inline char _lox_scanner_advance(lox_scanner_t* scanner) {
  return scanner->input.buf[scanner->off++];
}

void lox_scanner_init(lox_scanner_t* scanner, lox_slice_t input) {
  memset(scanner, 0, sizeof(*scanner));
  scanner->input = input;
}

int lox_scanner_next_token(lox_scanner_t* scanner, lox_token_t* token) {
  if (_lox_scanner_at_end(scanner)) {
    *token = LOX_TOKEN(EOF, scanner->off, 0);
    return 0;
  }

  u32 start = scanner->off;
  char ch = _lox_scanner_advance(scanner);

  switch (ch) {
    case '(':
      *token = LOX_TOKEN(LEFT_PAREN, start, 1);
      break;
    case ')':
      *token = LOX_TOKEN(RIGHT_PAREN, start, 1);
      break;
    case '{':
      *token = LOX_TOKEN(LEFT_BRACE, start, 1);
      break;
    case '}':
      *token = LOX_TOKEN(RIGHT_BRACE, start, 1);
      break;
    case ',':
      *token = LOX_TOKEN(COMMA, start, 1);
      break;
    case '.':
      *token = LOX_TOKEN(DOT, start, 1);
      break;
    case '-':
      *token = LOX_TOKEN(MINUS, start, 1);
      break;
    case '+':
      *token = LOX_TOKEN(PLUS, start, 1);
      break;
    case '*':
      *token = LOX_TOKEN(STAR, start, 1);
      break;
    case ';':
      *token = LOX_TOKEN(SEMICOLON, start, 1);
      break;

    default:
      // TODO: use logger or return better error type.
      fprintf(
          stderr,
          "lox: unexpected character '%c' at %u:%u\n",
          ch,
          scanner->pos.row,
          scanner->pos.col
      );
      return EINVAL;
  }

  return 0;
}

int lox_scanner_parse_number(
    const lox_scanner_t* scanner,
    const lox_token_t* token,
    double* value
) {
  if ((token->type != LOX_TT_NUMBER) ||
      !_lox_token_is_valid_off(token, scanner->input)) {
    return EINVAL;
  }

  const char* num_str_beg = scanner->input.buf + token->off;
  char* num_str_end;

  double num = strtod(num_str_beg, &num_str_end);
  if (num_str_end != (num_str_beg + token->len)) {
    return EINVAL;
  }

  *value = num;
  return 0;
}

int lox_scanner_get_position(
    const lox_scanner_t* scanner,
    const lox_token_t* token,
    lox_pos_t* pos
) {
  if (!_lox_token_is_valid_off(token, scanner->input)) {
    return EINVAL;
  }

  u32 row = 0;
  u32 off = 0;

  const char* start = scanner->input.buf;
  char* end;

  while ((end = strchr(start, '\n')) != NULL) {
    off += (u32)(end - start) + 1;
    if (off > token->off) {
      break;
    }

    row++;
    start = end + 1;
  }

  *pos = (lox_pos_t){
      .row = row,
      .col = off - token->off,
  };

  return 0;
}
