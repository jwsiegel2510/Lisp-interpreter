/* Author: Jonathan Siegel

Implements the Expression class of a Lisp Interpreter. */

#include "Expr.h"

namespace Expression {
  void Expr::simplify() {
    switch(T) {
      case INT_EX: return;
      case UMINUS_EX: if (right == nullptr) throw Bad_Expr();
                right->simplify();
                val = -right->value(); delete right;
                return;
      default:  if (right == nullptr || left == nullptr) throw Bad_Expr();
                left->simplify();
                val = left->value(); delete left;
                right->simplify();
                break;
    }
    switch(T) {
      case '+': val += right->value(); delete right; return;
      case '*': val *= right->value(); delete right; return;
      case '-': val -= right->value(); delete right; return;
      case '/': val /= right->value(); delete right; return;
      case '%': val %= right->value(); delete right; return;
    }
  }

}
