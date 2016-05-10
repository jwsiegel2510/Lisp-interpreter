/* Author: Jonathan Siegel

Contains declarations for a Lisp Lexer. */

#ifndef _LISP_LEXER_H_
#define _LISP_LEXER_H_

#include "Expr.h"
#include <cstdio>
#include <iostream>
#include <string>

namespace Lexer {

  using std::string;
  using std::istream;

  istream* input;

  namespace enumerations {
    enum Token {INT_T, PLUS_T = '+', TIMES_T = '*', MINUS_T = '-', DIV_T = '/', MOD_T = '%', LPAREN_T = '(', RPAREN_T = ')', COLON_T = ';', END_T};
  }
  using namespace enumerations;

  long current_int;
  string current_string;

  class Bad_Token {}; // used for throwing exceptions.

  Token curr_tok;

  Token getToken();
}

#endif
