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
  bool not_exit = true;
  while (*input && not_exit) {
      try {
      Expr* expression = expr(true);
      expression = eval(expression);
      std::cout << expression << "\n" << ">>> ";
      delete expression;
    }
    catch(Expression::Bad_Expr e) {
      printf("Dude, your expression isn't valid.");
      std::cout << "\n" << ">>> ";
    }
    catch (Lexer::Bad_Token e) {
      printf("C'mon you've gotta give my lexer something it can work with.");
      std::cout << "\n" << ">>> ";
    }
    catch (Parser::ParseError e) {
      printf("I can't quite seem to parse that ... please try again.");
      std::cout << "\n" << ">>> ";
    }
    catch (Lexer::Exit_Interpreter e) {
      not_exit = false;
    }
  }
  Expression::del_variables();
}
