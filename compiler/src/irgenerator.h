# pragma once

#include "ast.h"
#include "visitor.h"
#include "shared/ir.h"
#include <vector>
#include <unordered_map>

class IRGenerator : public ExprVisitor, public StmtVisitor {
private:
	int vreg_counter_ = 1;
	int block_counter_ = 1;

	std::vector<Quad> instructions_;

	int allocateVReg();
	void emit(Quad& quad);

	std::unordered_map<std::string, int> symbolTable_;

	std::vector<std::unique_ptr<BasicBlock>> cfg;

	BasicBlock* createBlock() {
		auto newBlock = std::make_unique<BasicBlock>();
		newBlock->id = block_counter_++;
		BasicBlock* currentBlock = newBlock.get();
		cfg.push_back(std::move(newBlock));
		return currentBlock;
	}

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



