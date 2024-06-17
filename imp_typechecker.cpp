#include "imp_typechecker.hh"

ImpTypeChecker::ImpTypeChecker() {

}

ImpTypeChecker::~ImpTypeChecker() {

}

void ImpTypeChecker::typecheck(Program* p) {
  env.clear();
  p->accept(this);
  return;
}

void ImpTypeChecker::visit(Program* p) {
  p->body->accept(this);
  return;
}

void ImpTypeChecker::visit(Body* b) {
  env.add_level();
  b->var_decs->accept(this);
  b->slist->accept(this);
  env.remove_level();  
  return;
}

void ImpTypeChecker::visit(VarDecList* decs) {
  list<VarDec*>::iterator it;
  for (it = decs->vdlist.begin(); it != decs->vdlist.end(); ++it) {
    (*it)->accept(this);
  }  
  return;
}

void ImpTypeChecker::visit(VarDec* vd) {
  list<string>::iterator it;
  ImpType tt= ImpValue::get_basic_type(vd->type);
  //verificador de tipos de declaración de variables
  if (tt == NOTYPE) {
    cout << "Tipo No valido: " << vd->type << endl;
    exit(0);
  }
  for (it = vd->vars.begin(); it != vd->vars.end(); ++it) {
    env.add_var(*it, tt);
  } 
  return;
}


void ImpTypeChecker::visit(StatementList* s) {
  list<Stm*>::iterator it;
  for (it = s->slist.begin(); it != s->slist.end(); ++it) {
    (*it)->accept(this);
  }
  return;
}

void ImpTypeChecker::visit(AssignStatement* s) {
  ImpType type = s->rhs->accept(this);
  if (!env.check(s->id)){
    cout<< "Error: Variable" <<s->id << "no declarada"<<endl;
    exit(1);
  } 
  // verificador tipo de assignaciones
  ImpType lhs_type = env.lookup(s->id);
  if (lhs_type != type);
    cout<< "Error: Tipos no coinciden"<<endl;
    exit(1);
 
  return;
}

void ImpTypeChecker::visit(PrintStatement* s) {
  s->e->accept(this);
  return;
}

void ImpTypeChecker::visit(IfStatement* s) { 
  ImpType v= s->cond->accept(this); 
  //Asignación de tipo de condición
  if (v != TBOOL){
    cout<< "Error: Condición no booleana"<<endl;
    exit(0);
  }
  s->tbody->accept(this);
  if (s->fbody != NULL)
    s->fbody->accept(this);
  return;
}

void ImpTypeChecker::visit(WhileStatement* s) {
  ImpType tcond = s->cond->accept(this);
  if (tcond != TBOOL){
    cout<< "Error: Condición no booleana"<<endl;
    exit(0);
  }
  s->body->accept(this);
 return;
}

//void ImpTypeChecker::visit(DoWhileStatement* s) {} 

ImpType ImpTypeChecker::visit(BinaryExp* e) {
  ImpType t1 = e->left->accept(this);
  ImpType t2 = e->right->accept(this);
  ImpType result;

  if (t1 != t2) {
    cout << "Error: Tipos no coinciden" << endl;
    exit(0);
  }
  switch(e->op) {
  case PLUS: result = TINT; break; 
  case MINUS: result = TINT; break;
  case MULT: result = TINT; break;
  case DIV: result = TINT; break;
  case EXP: result = TINT; 
  break;
  case LT: result =TBOOL; break;
  case LTEQ: result = TBOOL; break;
  case EQ: result = TBOOL; 
  break;
  }
  return result;
}

ImpType ImpTypeChecker::visit(NumberExp* e) {
  ImpType t = TINT;
  return t;
}

ImpType ImpTypeChecker::visit(IdExp* e) {
  ImpType t = env.lookup(e->id);
  return t;
}

ImpType ImpTypeChecker::visit(ParenthExp* ep) {
  return ep->e->accept(this);
}

ImpType ImpTypeChecker::visit(CondExp* e) { 
  ImpType tcond = e->cond->accept(this);
  ImpType ttype = e->etrue->accept(this);
  ImpType ftype = e->efalse->accept(this);

  if (tcond != TBOOL){
    cout<< "Error: Condición no booleana"<<endl;
    exit(0);
  }

  if (ttype != ftype){
    cout<< "Error: Tipos no coinciden"<<endl;
    exit(0);
  }
  return ttype;
}

