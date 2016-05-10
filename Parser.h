/* Author: Jonathan Siegel

Contains declarations for a Parser for the Lisp Interpreter. */

#ifndef _LISP_PARSER_H_
#define _LISP_PARSER_H_

#include "Lexer.h"

namespace Parser {
  using namespace Lexer::enumerations;
  using Lexer::current_int;
  using Lexer::current_string;
  using Lexer::curr_tok;
  using Lexer::getToken;

  using Expression::Expr;
  using namespace Expression::enumerations;

  class ParseError {}; // used for throwing exceptions

  Expr* expr(bool); // boolean tells whether to eat the current token and move on to the next.

}

#endif
