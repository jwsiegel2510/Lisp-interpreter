/* Author: Jonathan Siegel

Implements the Lexer for a simple Lisp Interpreter.*/

#include "Lexer.h"

namespace Lexer {

  istream* input;

  long current_int;
  string current_string;

  Token curr_tok;

  Token getToken() {
    char ch;
    (*input) >> ch;
    switch(ch) {
      case 0:
        return curr_tok = END_T;
      case '+': case '*': case '/': case '%':
      case '(': case ')':
        return curr_tok = Token(ch);
      case '-':
        (*input).get(ch);
        if (isdigit(ch)) {
          (*input).putback(ch); (*input).putback('-');
          (*input) >> current_int;
          return curr_tok = INT_T;
        }
        (*input).putback(ch);
        return curr_tok = Token('-');
      default:
        if (isdigit(ch)) {
          (*input).putback(ch);
          (*input) >> current_int;
          return curr_tok = INT_T;
        }
        if (islower(ch)) {
          string s; s.push_back(ch);
          while (((*input).get(ch), islower(ch))) s.push_back(ch);
          if (!isspace(ch) && ch != ')') throw Bad_Token();
          if (ch == ')') (*input).putback(ch);
          if (s == "pow") return curr_tok = POW_T;
          current_string = s;
          return curr_tok = VAR_T;
        }
        if (isupper(ch)) {
          string s; s.push_back(ch);
          while (((*input).get(ch), isupper(ch))) s.push_back(ch);
          if (!isspace(ch) && ch != ')' && ch != '(') throw Bad_Token();
          if (ch == ')' || ch == '(') (*input).putback(ch);
          if (s == "DEF") return curr_tok = DEF_T;
          if (s == "LAMBDA") return curr_tok = LAMBDA_T;
        }
        throw Bad_Token();
    }
  }
}
