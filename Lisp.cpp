/* Author: Jonathan Siegel

Contains a simple Lisp Interpreter.*/

#include "Parser.h"
#include <cstdio>
#include <iostream>

using Lexer::input;
using Expression::Expr;
using Expression::Int_Expr;
using Expression::Var_Expr;
using Expression::Lambda_Expr;
using Expression::List_Expr;
using Expression::Op_Expr;
using Parser::expr;

int main() {
  std::cout << ">>> ";
  input = &std::cin;
  while (*input) {
    Expr* expression = expr(true);
    expression = eval(expression);
    std::cout << expression << "\n" << ">>> ";
    delete expression;
  }
}
