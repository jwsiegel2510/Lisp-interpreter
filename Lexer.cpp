/* Author: Jonathan Siegel

Implements the Lexer for a simple Lisp Interpreter.*/

#include "Lexer.h"

namespace Lexer {
  Token getToken() {
    char ch;
    (*input) >> ch;
    switch(ch) {
      case 0:
        return curr_tok = END_T;
      case '+': case '*': case '-': case '/': case '%':
      case '(': case ')': case ';':
        return curr_tok = Token(ch);
      default:
        if (isdigit(ch)) {
          (*input).putback(ch);
          (*input) >> current_int;
          return curr_tok = INT_T;
        }
        throw Bad_Token();
    }
  }
}
