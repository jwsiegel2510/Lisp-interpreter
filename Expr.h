/* Author: Jonathan Siegel

Contains an expressions class for a Lisp Interpreter. */

#ifndef _LISP_OPPR_H_
#define _LISP_OPPR_H_

#include<string>
#include<list>
#include<unordered_map>
#include<stack>
#include <iostream>

namespace Expression {

  using std::string;
  using std::stack;
  using std::list;
  using std::unordered_map;
  using std::ostream;

  namespace enumerations {
    enum op_type {POW_OP, DEF_OP, CAR_OP, CDR_OP, CONS_OP, CONC_OP, IF_OP, LESS_OP = '<', EQ_OP = '=', GT_OP = '>', PLUS_OP = '+', TIMES_OP = '*', MINUS_OP = '-', DIV_OP = '/', MOD_OP = '%'};
  }
  using namespace enumerations;

  class Bad_Expr {}; // used for throwing exceptions

  class Expr { // an expression can be an operator, a variable, an int, or a list of expressions.
  public:
    Expr() {}
    virtual ~Expr() {}

    virtual bool is_int() = 0;
    virtual bool is_op() = 0;
    virtual bool is_var() = 0;
    virtual bool is_lambda() = 0;
    virtual bool is_list() = 0;
  };

  extern unordered_map<string,stack<Expr*> > var_table;
  extern unordered_map<string,bool> forced_eval;

  void del_variables();

  class Op_Expr : public Expr {
  private:
    op_type type;
  public:
    Op_Expr() : Expr() {type = PLUS_OP;}
    Op_Expr(op_type T) : Expr() {type = T;}
    Op_Expr(const Op_Expr& O) : Expr() {type = O.get_type();}
    virtual ~Op_Expr() {}

    void set_type(op_type T) {type = T;}
    op_type get_type() const {return type;}

    virtual bool is_int() {return false;}
    virtual bool is_op() {return true;}
    virtual bool is_var() {return false;}
    virtual bool is_lambda() {return false;}
    virtual bool is_list() {return false;}
  };

  class Int_Expr : public Expr {
  private:
    long val;
  public:
    Int_Expr() : Expr() {val = 0;}
    Int_Expr(long v) : Expr() {val = v;}
    Int_Expr(const Int_Expr& I) : Expr() {val = I.value();}
    virtual ~Int_Expr() {}

    const long& value() const {return val;}
    long& value() {return val;}

    virtual bool is_int() {return true;}
    virtual bool is_op() {return false;}
    virtual bool is_var() {return false;}
    virtual bool is_lambda() {return false;}
    virtual bool is_list() {return false;}
  };

  class Var_Expr : public Expr {
  private:
    string name;
  public:
    Var_Expr() : Expr() {}
    Var_Expr(string s) : Expr() {name = s;}
    Var_Expr(const Var_Expr& I) : Expr() {name = I.get_name();}
    virtual ~Var_Expr() {}

    string get_name() const {return name;}
    void set_name(string s) {name = s;}

    virtual bool is_int() {return false;}
    virtual bool is_op() {return false;}
    virtual bool is_var() {return true;}
    virtual bool is_lambda() {return false;}
    virtual bool is_list() {return false;}
  };

  class Lambda_Expr : public Expr {
  private:
    string var;
    Expr* body_expr;
  public:
    Lambda_Expr() : Expr() {}
    Lambda_Expr(string s, Expr* E) : Expr() {var = s; body_expr = E;}
    Lambda_Expr(const Lambda_Expr& I);
    virtual ~Lambda_Expr() {if (body_expr != nullptr) delete body_expr;}

    string get_var() const {return var;}
    void set_var(string s) {var = s;}
    Expr* get_body() const {return body_expr;}
    void set_body(Expr* b) {body_expr = b;}

    virtual bool is_int() {return false;}
    virtual bool is_op() {return false;}
    virtual bool is_var() {return false;}
    virtual bool is_lambda() {return true;}
    virtual bool is_list() {return false;}
  };

  class List_Expr : public Expr {
  private:
    list<Expr*> elements;
  public:
    List_Expr() : Expr() {}
    List_Expr(const List_Expr& L);
    virtual ~List_Expr();

    const list<Expr*>& getElements() const {return elements;}
    list<Expr*>& getElements() {return elements;}

    virtual bool is_int() {return false;}
    virtual bool is_op() {return false;}
    virtual bool is_var() {return false;}
    virtual bool is_lambda() {return false;}
    virtual bool is_list() {return true;}
  };

  Expr* reduce(Expr*); // reduces an expression according to the rules given by the operators.
  Expr* eval(Expr*); // the main difference is that eval doesn't allow partial lambdas or undefined variables.

  ostream& operator<<(ostream& os, Expr* E); // functions for printing Expressions.
}

#endif
