/* Author: Jonathan Siegel

Contains an expressions class for a Lisp Interpreter. */

#ifndef _LISP_EXPR_H_
#define _LISP_EXPR_H_

namespace Expression {

  namespace enumerations {
    enum type {INT_EX, PLUS_EX = '+', TIMES_EX = '*', MINUS_EX = '-', DIV_EX = '/', MOD_EX = '%', UMINUS_EX};
  }
  using namespace enumerations;

  class Bad_Expr {}; // used for throwing exceptions

  class Expr {
  private:
    type T;
    long val;
    Expr* left;
    Expr* right;

  public:
    Expr() {}
    Expr(type Ty) {T = Ty;}
    Expr(long i) {T = INT_EX; val = i;}

    virtual ~Expr() {
      if (left != nullptr) delete left;
      if (right != nullptr) delete right;
    }

    type getType() {return T;}
    void setType(type Q) {T = Q;}

    void setLeft(Expr* E) {left = E;}
    void setRight(Expr* E) {right = E;}

    long value() {return val;}

    void simplify();

  };
}

#endif
