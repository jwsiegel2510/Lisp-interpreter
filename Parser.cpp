/* Author: Jonathan Siegel

Implements the parser for a simple Lisp Interpreter. */

#include "Parser.h"
#include <stdio.h>

namespace Parser {

  Expr* expr(bool b) {
    if (b) getToken();
    switch(curr_tok) {
      case LPAREN_T: {
        if (getToken() == LAMBDA_T) {
          if (getToken() != LPAREN_T) throw ParseError();
          if (getToken() != VAR_T) throw ParseError();
          std::string var = current_string;
          if (getToken() != RPAREN_T) throw ParseError();
          Lambda_Expr* l_out = new Lambda_Expr(var, expr(true));
          if (getToken() != RPAREN_T) throw ParseError();
          return l_out;
        }
        List_Expr* out = new List_Expr;
        if (curr_tok == RPAREN_T) return out;
        out->getElements().push_back(expr(false));
        while (getToken() != RPAREN_T) {
          out->getElements().push_back(expr(false));
        } return out;
      }
      case '+': return new Op_Expr(PLUS_OP);
      case '-': return new Op_Expr(MINUS_OP);
      case '*': return new Op_Expr(TIMES_OP);
      case '/': return new Op_Expr(DIV_OP);
      case '%': return new Op_Expr(MOD_OP);
      case '<': return new Op_Expr(LESS_OP);
      case '=': return new Op_Expr(EQ_OP);
      case '>': return new Op_Expr(GT_OP);
      case POW_T: return new Op_Expr(POW_OP);
      case DEF_T: return new Op_Expr(DEF_OP);
      case CAR_T: return new Op_Expr(CAR_OP);
      case CDR_T: return new Op_Expr(CDR_OP);
      case CONS_T: return new Op_Expr(CONS_OP);
      case CONC_T: return new Op_Expr(CONC_OP);
      case IF_T: return new Op_Expr(IF_OP);
      case VAR_T: return new Var_Expr(current_string);
      case INT_T:
        return new Int_Expr(current_int);
      default:
         throw ParseError();
    }
  }

}
