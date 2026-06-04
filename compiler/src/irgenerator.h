# pragma once

#include "ast.h"
#include "visitor.h"
#include "shared/ir.h"
#include <vector>

class IRGenerator : public ExprVisitor, public StmtVisitor {
private:
	int vreg_counter_ = 1;

	std::vector<Quad> instructions_;

	int allocateVReg();
	void emit(Quad& quad);

public:
	IROp mapOperator(TokenType type);
	int visitBinaryOpNode(BinaryOpNode& n) override;
	int visitUnaryOpNode(UnaryOpNode& n) override;
	int visitLiteralNode(LiteralNode& n) override;
	int visitIdentifierNode(IdentifierNode& n) override;
	int visitFuncCallNode(FuncCallNode& n) override;
	void visitVarDecl(VarDeclStmt& n) override;
	void visitRet(RetStmt& n) override;
	void visitIf(IfStmt& n) override;
	void visitWhile(WhileStmt& n) override;
	void visitBlock(BlockStmt& n) override;
	void visitExpr(ExprStmt& n) override;

	void printIR();
	std::string operandToString(Operand operand);
	std::string operatorToString(IROp operand);
};



