#include "visitor.h"

// Expressions
int BinaryOpNode::accept(ExprVisitor& v) {
	v.visitBinaryOpNode(*this);
	return 0;
}

int UnaryOpNode::accept(ExprVisitor& v) {
	v.visitUnaryOpNode(*this);
	return 0;
}

int LiteralNode::accept(ExprVisitor& v) {
	v.visitLiteralNode(*this);
	return 0;
}

int IdentifierNode::accept(ExprVisitor& v) {
	v.visitIdentifierNode(*this);
	return 0;
}

int FuncCallNode::accept(ExprVisitor& v) {
	v.visitFuncCallNode(*this);
	return 0;
}


// Statements
void VarDeclStmt::accept(StmtVisitor& v) {
	v.visitVarDecl(*this);
}

void RetStmt::accept(StmtVisitor& v) {
	v.visitRet(*this);
}

void IfStmt::accept(StmtVisitor& v) {
	v.visitIf(*this);
}

void WhileStmt::accept(StmtVisitor& v) {
	v.visitWhile(*this);
}

void BlockStmt::accept(StmtVisitor& v) {
	v.visitBlock(*this);
}

void ExprStmt::accept(StmtVisitor& v) {
	v.visitExpr(*this);
}







