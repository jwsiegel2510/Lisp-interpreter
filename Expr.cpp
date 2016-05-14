/* Author: Jonathan Siegel

Implements the Expression class of a Lisp Interpreter. */

#include "Expr.h"
#include <stdio.h>

namespace Expression {

  unordered_map<string,stack<Expr*> > var_table;
  unordered_map<string,bool> forced_eval; // used to indicate whether a variable's evaluation is forced by a surrounding lambda.

  long power(long a, long b);
  Expr* copy(Expr* E);

  void del_variables() {
    for (unordered_map<string,stack<Expr*> >::iterator it = var_table.begin(); it != var_table.end(); ++it) {
      while (!it->second.empty()) {delete it->second.top(); it->second.pop();}
    }
  }

  List_Expr::List_Expr(const List_Expr& L) : Expr() { // copy constructor for List_Expr
    for (list<Expr*>::const_iterator it = L.getElements().begin(); it != L.getElements().end(); ++it) {
      if ((*it)->is_int()) elements.push_back(new Int_Expr(*static_cast<Int_Expr*>(*it)));
      else if ((*it)->is_var()) elements.push_back(new Var_Expr(*static_cast<Var_Expr*>(*it)));
      else if ((*it)->is_op()) elements.push_back(new Op_Expr(*static_cast<Op_Expr*>(*it)));
      else if ((*it)->is_lambda()) elements.push_back(new Lambda_Expr(*static_cast<Lambda_Expr*>(*it)));
      else elements.push_back(new List_Expr(*static_cast<List_Expr*>(*it)));
    }
  }

  List_Expr::~List_Expr() {
    for (list<Expr*>::iterator it = elements.begin(); it != elements.end(); ++it) {
      if (*it != nullptr) delete (*it);
    }
  }

  Lambda_Expr::Lambda_Expr(const Lambda_Expr& I) : Expr() { // copy constructor for Lambda_Expr
    var = I.get_var();
    body_expr = copy(I.get_body());
  }

  Expr* copy(Expr* E) {
    if (E->is_int()) return new Int_Expr(*static_cast<Int_Expr*>(E));
    if (E->is_op()) return new Op_Expr(*static_cast<Op_Expr*>(E));
    if (E->is_var()) return new Var_Expr(*static_cast<Var_Expr*>(E));
    if (E->is_lambda()) return new Lambda_Expr(*static_cast<Lambda_Expr*>(E));
    return new List_Expr(*static_cast<List_Expr*>(E));
  }

  void check_arith(list<Expr*>::iterator it, list<Expr*>::iterator end) { // checks arguments for arithmetic operations.
    if ((++it) == end || !((*it = eval(*it))->is_int()) || (++it) == end || !((*it = eval(*it))->is_int())) throw Bad_Expr();
    if (++it != end) throw Bad_Expr();
  }

  void check_arith_red(list<Expr*>::iterator it, list<Expr*>::iterator end) { // checks arguments for reducing arithmetic operations.
    if ((++it) == end || ((*it = reduce(*it))->is_op()) || (++it) == end || ((*it = reduce(*it))->is_op())) throw Bad_Expr();
    if (++it != end) throw Bad_Expr();
    --it; --it;
    if ((*it)->is_lambda() || (*(++it))->is_lambda()) throw Bad_Expr();
  }

  Expr* reduce(Expr* E) {
    if (E->is_int()) return E;
    if (E->is_op()) throw Bad_Expr();
    if (E->is_var()) {
      string s = static_cast<Var_Expr*>(E)->get_name();
      if (forced_eval.find(s) == forced_eval.end()) forced_eval[s] = false;
      if (forced_eval[s]) return eval(E);
      return E;
    }
    if (E->is_lambda()) {
      string s = static_cast<Lambda_Expr*>(E)->get_var();
      if (forced_eval.find(s) == forced_eval.end()) forced_eval[s] = false;
      if (forced_eval[s] == true) {
        forced_eval[s] = false;
        static_cast<Lambda_Expr*>(E)->set_body(reduce(static_cast<Lambda_Expr*>(E)->get_body()));
        forced_eval[s] = true;
      } else {
        static_cast<Lambda_Expr*>(E)->set_body(reduce(static_cast<Lambda_Expr*>(E)->get_body()));
      }
      return E;}
    if (E->is_list()) {
      // this part of the function gets called if the expr is a list.
      List_Expr* Q = static_cast<List_Expr*>(E);
      if (Q->getElements().empty()) return E;
      list<Expr*>::iterator it = Q->getElements().begin();
      list<Expr*>::iterator end = Q->getElements().end();
      if ((*it)->is_op()) {
        switch(static_cast<Op_Expr*>(*it)->get_type()) {
          case POW_OP: {
          check_arith_red(it,end); ++it;
          if (!(*it)->is_int() || !(*(++it))->is_int()) return E;
          --it;
          long temp = static_cast<Int_Expr*>(*it)->value(); ++it; temp = power(temp,static_cast<Int_Expr*>(*it)->value());
          delete E; return new Int_Expr(temp);}

          case PLUS_OP: {
          check_arith_red(it,end); ++it;
          if (!(*it)->is_int() || !(*(++it))->is_int()) return E;
          --it;
          long temp = static_cast<Int_Expr*>(*it)->value(); ++it; temp += static_cast<Int_Expr*>(*it)->value();
          delete E; return new Int_Expr(temp);}

          case TIMES_OP: {
          check_arith_red(it,end); ++it;
          if (!(*it)->is_int() || !(*(++it))->is_int()) return E;
          --it;
          long temp = static_cast<Int_Expr*>(*it)->value(); ++it; temp *= static_cast<Int_Expr*>(*it)->value();
          delete E; return new Int_Expr(temp);}

          case MINUS_OP: {
          check_arith_red(it,end); ++it;
          if (!(*it)->is_int() || !(*(++it))->is_int()) return E;
          --it;
          long temp = static_cast<Int_Expr*>(*it)->value(); ++it; temp -= static_cast<Int_Expr*>(*it)->value();
          delete E; return new Int_Expr(temp);}

          case DIV_OP: {
          check_arith_red(it,end); ++it;
          if (!(*it)->is_int() || !(*(++it))->is_int()) return E;
          --it;
          long temp = static_cast<Int_Expr*>(*it)->value(); ++it; temp /= static_cast<Int_Expr*>(*it)->value();
          delete E; return new Int_Expr(temp);}

          case MOD_OP: {
          check_arith_red(it,end); ++it;
          if (!(*it)->is_int() || !(*(++it))->is_int()) return E;
          --it;
          long temp = static_cast<Int_Expr*>(*it)->value(); ++it; temp %= static_cast<Int_Expr*>(*it)->value();
          delete E; return new Int_Expr(temp);}

          case CAR_OP: {
            if (++it == end || ((*it = reduce(*it))->is_int() && !(*it)->is_int())) throw Bad_Expr();
            if ((*it = reduce(*it))->is_int()) throw Bad_Expr(); if ((*it)->is_op()) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            return E;
          }

          case CDR_OP: {
            if (++it == end || (*it = reduce(*it))->is_int()) throw Bad_Expr(); if ((*it)->is_op()) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            return E;
          }

          case CONS_OP: {
            if (++it == end || ((*it = reduce(*it))->is_int() && !(*it)->is_int())) throw Bad_Expr();
            if (++it == end || (*it = reduce(*it))->is_int() || (*it)->is_op()) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            return E;
          }

          case CONC_OP: {
            if (++it == end || (*it = reduce(*it))->is_int() || (*it)->is_op()) throw Bad_Expr();
            if (++it == end || (*it = reduce(*it))->is_int() || (*it)->is_op()) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            return E;
          }

          case IF_OP: {
            if (++it == end) throw Bad_Expr();
            if (++it == end) throw Bad_Expr();
            if (++it == end) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            --it; --it; --it;
            if ((*it = reduce(*it))->is_int()) {
              ++it;
              Expr* out = reduce(*it); *it = nullptr;
              delete E; return out;
            }
            if ((*it)->is_list()) {
              List_Expr* tp = static_cast<List_Expr*>(*it);
              if (tp->getElements().size() == 0) {
                ++it; ++it;
                Expr* out = reduce(*it); *it = nullptr;
                delete E; return out;
              }
            }
            ++it; (*it) = reduce(*it);
            ++it; (*it) = reduce(*it);
            return E;
          }

          case LESS_OP: {
            if (++it == end || (*it = reduce(*it))->is_op()) throw Bad_Expr();
            if (++it == end || (*it = reduce(*it))->is_op()) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            --it; --it;
            if (!(*it)->is_int()) return E;
            long x1 = static_cast<Int_Expr*>(*it)->value();
            ++it;
            if (!(*it)->is_int()) return E;
            long x2 = static_cast<Int_Expr*>(*it)->value();
            if (x1 < x2) return E;
            return new List_Expr;
          }

          case EQ_OP: {
            if (++it == end || (*it = reduce(*it))->is_op()) throw Bad_Expr();
            if (++it == end || (*it = reduce(*it))->is_op()) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            --it; --it;
            if (!(*it)->is_int()) return E;
            long x1 = static_cast<Int_Expr*>(*it)->value();
            ++it;
            if (!(*it)->is_int()) return E;
            long x2 = static_cast<Int_Expr*>(*it)->value();
            if (x1 == x2) return E;
            return new List_Expr;
          }

          case GT_OP: {
            if (++it == end || (*it = reduce(*it))->is_op()) throw Bad_Expr();
            if (++it == end || (*it = reduce(*it))->is_op()) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            --it; --it;
            if (!(*it)->is_int()) return E;
            long x1 = static_cast<Int_Expr*>(*it)->value();
            ++it;
            if (!(*it)->is_int()) return E;
            long x2 = static_cast<Int_Expr*>(*it)->value();
            if (x1 > x2) return E;
            return new List_Expr;
          }

          case DEF_OP: {
            throw Bad_Expr();
          }
        }
      }
      if ((*it)->is_lambda()) { // deals with lambda expressions as the first element of a list.
          if ((++it) == end) throw Bad_Expr();
          if (++it != end) throw Bad_Expr();
          --it;
          Expr* T = reduce(*it); *it = nullptr; --it;
          var_table[static_cast<Lambda_Expr*>(*it)->get_var()].push(T);
          forced_eval[static_cast<Lambda_Expr*>(*it)->get_var()] = true;
          Expr* T2 = reduce(static_cast<Lambda_Expr*>(*it)->get_body());
          forced_eval[static_cast<Lambda_Expr*>(*it)->get_var()] = false;
          var_table[static_cast<Lambda_Expr*>(*it)->get_var()].pop();
          return T2;
      }
      for (list<Expr*>::iterator i = it; i != end; ++i) { // evalulate each element of the list otherwise
        *i = reduce(*i);
      } return E;
    }
  }

  Expr* eval(Expr* E) {
    if (E->is_int()) return E;
    if (E->is_op()) throw Bad_Expr();
    if (E->is_var()) { // handling variables.
      if (var_table[static_cast<Var_Expr*>(E)->get_name()].empty()) throw Bad_Expr();
      Expr* temp = var_table[static_cast<Var_Expr*>(E)->get_name()].top();
      delete E;
      return copy(temp);
    }
    if (E->is_lambda()) {return reduce(E);}
    if (E->is_list()) {
      // this part of the function gets called if the expr is a list.
      List_Expr* Q = static_cast<List_Expr*>(E);
      if (Q->getElements().empty()) return E;
      list<Expr*>::iterator it = Q->getElements().begin();
      list<Expr*>::iterator end = Q->getElements().end();
      if ((*it)->is_op()) { // deal with operators if they appear as the first element of the list.
        long temp;
        switch(static_cast<Op_Expr*>(*it)->get_type()) {
          case POW_OP: {
          check_arith(it,end); ++it;
          long temp = static_cast<Int_Expr*>(*it)->value(); ++it; temp = power(temp,static_cast<Int_Expr*>(*it)->value());
          delete E; return new Int_Expr(temp);}

          case PLUS_OP: {
          check_arith(it,end); ++it;
          long temp = static_cast<Int_Expr*>(*it)->value(); ++it; temp += static_cast<Int_Expr*>(*it)->value();
          delete E; return new Int_Expr(temp);}

          case TIMES_OP: {
          check_arith(it,end); ++it;
          long temp = static_cast<Int_Expr*>(*it)->value(); ++it; temp *= static_cast<Int_Expr*>(*it)->value();
          delete E; return new Int_Expr(temp);}

          case MINUS_OP: {
          check_arith(it,end); ++it;
          long temp = static_cast<Int_Expr*>(*it)->value(); ++it; temp -= static_cast<Int_Expr*>(*it)->value();
          delete E; return new Int_Expr(temp);}

          case DIV_OP: {
          check_arith(it,end); ++it;
          long temp = static_cast<Int_Expr*>(*it)->value(); ++it; temp /= static_cast<Int_Expr*>(*it)->value();
          delete E; return new Int_Expr(temp);}

          case MOD_OP: {
          check_arith(it,end); ++it;
          long temp = static_cast<Int_Expr*>(*it)->value(); ++it; temp %= static_cast<Int_Expr*>(*it)->value();
          delete E; return new Int_Expr(temp);}

          case CAR_OP: {
            if (++it == end || !(*it = eval(*it))->is_list()) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            --it;
            List_Expr* temp = static_cast<List_Expr*>(*it);
            if (temp->getElements().empty()) throw Bad_Expr();
            Expr* out = *temp->getElements().begin();
            *temp->getElements().begin() = nullptr;
            delete E; return out;
          }

          case CDR_OP: {
            if (++it == end || !(*it = eval(*it))->is_list()) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            --it;
            List_Expr* temp = static_cast<List_Expr*>(*it);
            *it = nullptr;
            if (temp->getElements().empty()) throw Bad_Expr();
            Expr* out = *temp->getElements().begin();
            temp->getElements().pop_front();
            delete E; delete out; return temp;
          }

          case CONS_OP: {
            if (++it == end) throw Bad_Expr();
            if (++it == end || !(*it = eval(*it))->is_list()) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            --it; --it;
            Expr* elem = eval(*it);
            *it = nullptr; ++it;
            List_Expr* temp = static_cast<List_Expr*>(*it);
            *it = nullptr;
            temp->getElements().push_front(elem);
            delete E; return temp;
          }

          case CONC_OP: {
            if (++it == end || !(*it = eval(*it))->is_list()) throw Bad_Expr();
            if (++it == end || !(*it = eval(*it))->is_list()) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            --it; --it;
            List_Expr* elems = static_cast<List_Expr*>(*it); ++it;
            List_Expr* temp = static_cast<List_Expr*>(*it);
            *it = nullptr;
            temp->getElements().splice(temp->getElements().begin(),elems->getElements());
            delete E; return temp;
          }

          case IF_OP: {
            if (++it == end) throw Bad_Expr();
            if (++it == end) throw Bad_Expr();
            if (++it == end) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            --it; --it; --it;
            if (!(*it = eval(*it))->is_list()) { // if the condition is not a list, it's not the empty list, so true.
              ++it;
              Expr* out = eval(*it); *it = nullptr;
              delete E; return out;
            }
            List_Expr* temp = static_cast<List_Expr*>(*it);
            if (temp->getElements().size() == 0) { // if the condition is the empty list, so false.
              ++it; ++it;
              Expr* out = eval(*it); *it = nullptr;
              delete E; return out;
            }
            ++it;
            Expr* out = eval(*it); *it = nullptr;
            delete E; return out;
          }

          case LESS_OP: {
            if (++it == end || !(*it = eval(*it))->is_int()) throw Bad_Expr();
            if (++it == end || !(*it = eval(*it))->is_int()) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            --it; --it;
            long x1 = static_cast<Int_Expr*>(*it)->value();
            ++it;
            long x2 = static_cast<Int_Expr*>(*it)->value();
            if (x1 < x2) return E;
            return new List_Expr;
          }

          case EQ_OP: {
            if (++it == end || !(*it = eval(*it))->is_int()) throw Bad_Expr();
            if (++it == end || !(*it = eval(*it))->is_int()) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            --it; --it;
            long x1 = static_cast<Int_Expr*>(*it)->value();
            ++it;
            long x2 = static_cast<Int_Expr*>(*it)->value();
            if (x1 == x2) return E;
            return new List_Expr;
          }

          case GT_OP: {
            if (++it == end || !(*it = eval(*it))->is_int()) throw Bad_Expr();
            if (++it == end || !(*it = eval(*it))->is_int()) throw Bad_Expr();
            if (++it != end) throw Bad_Expr();
            --it; --it;
            long x1 = static_cast<Int_Expr*>(*it)->value();
            ++it;
            long x2 = static_cast<Int_Expr*>(*it)->value();
            if (x1 > x2) return E;
            return new List_Expr;
          }

          case DEF_OP: {
          if ((++it) == end || !((*it)->is_var()) || (++it) == end ) throw Bad_Expr();
          if (++it != end) throw Bad_Expr();
          --it; --it;
          list<Expr*>::iterator itemp = it++;
          Expr* T = eval(*it); *it = nullptr;
          if (!var_table[static_cast<Var_Expr*>(*itemp)->get_name()].empty()) { // remove previous declaration.
            Expr* old = var_table[static_cast<Var_Expr*>(*itemp)->get_name()].top();
            var_table[static_cast<Var_Expr*>(*itemp)->get_name()].pop();
            delete old;
          }
          var_table[static_cast<Var_Expr*>(*itemp)->get_name()].push(copy(T));
          delete E; return T;}
        }
      }
      if ((*it)->is_lambda()) { // deals with lambda expressions as the first element of a list.
          if ((++it) == end) throw Bad_Expr();
          if (++it != end) throw Bad_Expr();
          --it;
          Expr* T = eval(*it); *it = nullptr; --it;
          var_table[static_cast<Lambda_Expr*>(*it)->get_var()].push(T);
          forced_eval[static_cast<Lambda_Expr*>(*it)->get_var()] = true;
          Expr* T2 = eval(static_cast<Lambda_Expr*>(*it)->get_body());
          forced_eval[static_cast<Lambda_Expr*>(*it)->get_var()] = false;
          var_table[static_cast<Lambda_Expr*>(*it)->get_var()].pop();
          return T2;
      }
      if ((*it)->is_var()) { // if the first element is a variable, then we must evaluate it as it could be a lambda.
        *it = eval(*it);
        return eval(E);
      }
      for (list<Expr*>::iterator i = it; i != end; ++i) { // evalulate each element of the list otherwise
        *i = eval(*i);
      }
      if ((*it)->is_lambda()) return eval(E); // if the first element evaluated to a lambda, we must now recursively evaluate.
      return E;
    }
  }

  long power(long a, long b) {
    long q = 1;
    if (b > 0) while (((b%2 == 1)?q*=a:0,a*=a,b/=2));
    return q;
  }

  ostream& operator<<(ostream& os, Expr* E) {
    if (E->is_int()) {os << static_cast<Int_Expr*>(E)->value(); return os;}
    if (E->is_op()) {os << "OP"; return os;}
    if (E->is_var()) {os << static_cast<Var_Expr*>(E)->get_name(); return os;}
    if (E->is_lambda()) {os << "LAMBDA(" << static_cast<Lambda_Expr*>(E)->get_var() << ") " << static_cast<Lambda_Expr*>(E)->get_body(); return os;}
    List_Expr* Q = static_cast<List_Expr*>(E);
    list<Expr*>::iterator end = Q->getElements().end();
    os << "(";
    for (list<Expr*>::iterator it = Q->getElements().begin(); it != end;++it) {
      os << *it << " ";
    }
    if (!Q->getElements().empty()) os << "\b";
    os << ")"; return os;
  }

}
