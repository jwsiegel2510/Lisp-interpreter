/* Author: Jonathan Siegel

Implements the Expression class of a Lisp Interpreter. */

#include "Expr.h"

namespace Expr {
  void Expr::simplify() {
    switch(T) {
      case INT: return;
      default:  left->simplify();
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
