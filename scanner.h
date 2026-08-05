#pragma once

#include "str.h"
#include "types.h"

typedef enum {
  // Single character tokens.
  LOX_TT_LEFT_PAREN,
  LOX_TT_RIGHT_PAREN,
  LOX_TT_LEFT_BRACE,
  LOX_TT_RIGHT_BRACE,
  LOX_TT_COMMA,
  LOX_TT_DOT,
  LOX_TT_MINUS,
  LOX_TT_PLUS,
  LOX_TT_SEMICOLON,
  LOX_TT_SLASH,
  LOX_TT_STAR,

  // One or two character tokens.
  LOX_TT_BANG,
  LOX_TT_BANG_EQUAL,
  LOX_TT_EQUAL,
  LOX_TT_EQUAL_EQUAL,
  LOX_TT_GREATER,
  LOX_TT_GREATER_EQUAL,
  LOX_TT_LESS,
  LOX_TT_LESS_EQUAL,

  // Literals.
  LOX_TT_IDENTIFIER,
  LOX_TT_STRING,
  LOX_TT_NUMBER,

  // Keywords.
  LOX_TT_AND,
  LOX_TT_CLASS,
  LOX_TT_ELSE,
  LOX_TT_FALSE,
  LOX_TT_FUN,
  LOX_TT_FOR,
  LOX_TT_IF,
  LOX_TT_NIL,
  LOX_TT_OR,
  LOX_TT_PRINT,
  LOX_TT_RETURN,
  LOX_TT_SUPER,
  LOX_TT_THIS,
  LOX_TT_TRUE,
  LOX_TT_VAR,
  LOX_TT_WHILE,

  // End of file.
  LOX_TT_EOF,
} lox_token_type_t;

typedef struct {
  u32 row;
  u32 col;
} lox_pos_t;

typedef struct {
  lox_token_type_t type;
  u32 off;
  u32 len;
} lox_token_t;

typedef struct {
  lox_slice_t input;
  u32 off;
  lox_pos_t pos;
} lox_scanner_t;

void lox_scanner_init(lox_scanner_t* scanner, lox_slice_t input);

int lox_scanner_next_token(lox_scanner_t* scanner, lox_token_t* token);

int lox_scanner_parse_number(
    const lox_scanner_t* scanner,
    const lox_token_t* token,
    double* value
);

int lox_scanner_get_position(
    const lox_scanner_t* scanner,
    const lox_token_t* token,
    lox_pos_t* pos
);
