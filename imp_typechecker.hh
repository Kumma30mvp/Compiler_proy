#ifndef IMP_TYPECHECKER
#define IMP_TYPECHECKER


#include "imp.hh"
#include "type_visitor.hh"
#include "environment.hh"
#include <unordered_map>
#include <string>

using namespace std;

class ImpTypeChecker : public TypeVisitor {
public:
  ImpTypeChecker();
  ~ImpTypeChecker(); //agregamos destructor
private:
  Environment<ImpType> env;

public:
  void typecheck(Program*);
  void visit(Program*);
  void visit(Body*);
  void visit(VarDecList*);
  void visit(VarDec*);
  void visit(StatementList*);
  void visit(AssignStatement*);
  void visit(PrintStatement*);
  void visit(IfStatement*);
  void visit(WhileStatement*);
 // void visit(DoWhileStatement*); añadimos visit para DoWhileStatement
  
  ImpType visit(BinaryExp* e);
  ImpType visit(NumberExp* e);
  ImpType visit(IdExp* e);
  ImpType visit(ParenthExp* e);
  ImpType visit(CondExp* e);
};


#endif

