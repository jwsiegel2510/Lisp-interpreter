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

  extern istream* input;

  namespace enumerations {
    enum Token {INT_T, END_T, POW_T, VAR_T, DEF_T, LAMBDA_T, PLUS_T = '+', TIMES_T = '*', MINUS_T = '-', DIV_T = '/', MOD_T = '%', LPAREN_T = '(', RPAREN_T = ')'};
  }
  using namespace enumerations;

  extern long current_int;
  extern string current_string;

  class Bad_Token {}; // used for throwing exceptions.

  extern Token curr_tok;

  Token getToken();
}

#endif
