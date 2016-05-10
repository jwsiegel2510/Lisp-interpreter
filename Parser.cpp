/* Author: Jonathan Siegel

Implements the parser for a simple Lisp Interpreter. */

#include "Parser.h"

namespace Parser {

  Expr* expr(bool b) {
    if (b) getToken();
    Expr* out;
    switch(curr_tok) {
      case LPAREN_T:
        switch(getToken()) {
          case '+':
            out = new Expr(PLUS_EX); break;
          case '*':
            out = new Expr(TIMES_EX); break;
          case '-':
            out = new Expr(MINUS_EX); break;
          case '/':
            out = new Expr(DIV_EX); break;
          case '%':
            out = new Expr(MOD_EX); break;
          case INT_T:
            out = new Expr(current_int);
            if (getToken() != RPAREN_T) throw ParseError();
            return out;
          case LPAREN_T:
            out = expr(false);
            if (getToken() != RPAREN_T) throw ParseError();
        }
        switch(curr_tok) {
          case '+': case '*': case '/': case '%':
            out->setLeft(expr(true)); out->setRight(expr(true));
            if (getToken() != RPAREN_T) throw ParseError();
            return out;
          case '-':
            out->setLeft(expr(true));
            if (getToken() == RPAREN_T) {out->setType(UMINUS_EX); return out;}
            out->setRight(expr(false));
            if (getToken() != RPAREN_T) throw ParseError();
            return out;
        }
      case INT_T:
        out = new Expr(current_int); return out;
      default:
        throw ParseError();
    }
  }

}
