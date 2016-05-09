/* Author: Jonathan Siegel

Contains an expressions class for a Lisp Interpreter. */

#ifndef _LISP_EXPR_H_
#define _LISP_EXPR_H_

namespace Expr {
  enum type {INT, PLUS = '+', TIMES = '*', MINUS = '-', DIV = '/', MOD = '%'};

  class Expr {
  private:
    type T;
    long val;
    Expr* left;
    Expr* right;

  public:
    Expr() {}
    Expr(type Ty) {T = Ty;}
    Expr(long i) {T = INT; val = i;}

    virtual ~Expr() {
      if (left != nullptr) delete left;
      if (right != nullptr) delete right;
    }

    void setLeft(Expr* E) {left = E;}
    void setRight(Expr* E) {right = E;}

    long value() {return val;}

    void simplify();

  };
}

#endif
