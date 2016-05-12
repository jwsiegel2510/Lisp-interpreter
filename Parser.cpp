/* Author: Jonathan Siegel

Implements the parser for a simple Lisp Interpreter. */

#include "Parser.h"
#include <stdio.h>

namespace Parser {

  Expr* expr(bool b) {
    if (b) getToken();
    switch(curr_tok) {
      case LPAREN_T: {
        List_Expr* out = new List_Expr;
        while (getToken() != RPAREN_T) {
          out->getElements().push_back(expr(false));
        } return out;
      }
      case '+': return new Op_Expr(PLUS_OP);
      case '-': return new Op_Expr(MINUS_OP);
      case '*': return new Op_Expr(TIMES_OP);
      case '/': return new Op_Expr(DIV_OP);
      case '%': return new Op_Expr(MOD_OP);
      case POW_T: return new Op_Expr(POW_OP);
      case DEF_T: return new Op_Expr(DEF_OP);
      case VAR_T: return new Var_Expr(current_string);
      case LAMBDA_T: {
        if (getToken() != LPAREN_T) throw ParseError();
        if (getToken() != VAR_T) throw ParseError();
        std::string var = current_string;
        if (getToken() != RPAREN_T) throw ParseError();
        return new Lambda_Expr(var, expr(true));
      }
      case INT_T:
        return new Int_Expr(current_int);
      default:
         throw ParseError();
    }
  }

}
