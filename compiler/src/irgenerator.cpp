#include "irgenerator.h"
#include <vector>
#include <iostream>

int IRGenerator::allocateVReg() {
	return vreg_counter_++;
}

void IRGenerator::emit(Quad& quad) {
	instructions_.push_back(quad);
}

IROp IRGenerator::mapOperator(TokenType type) {
	switch (type) {
	case TokenType::PLUS: return IROp::ADD;
	case TokenType::MINUS: return IROp::SUB;
	case TokenType::STAR: return IROp::MUL;
	case TokenType::SLASH: return IROp::DIV;


	}
}

std::string IRGenerator::operandToString(Operand operand) {
	switch (operand.type) {
	case OperandType::VREG: 
		return "v" + std::to_string(operand.value);
	case OperandType::IMM:
		return std::to_string(operand.value);
	case OperandType::LABEL:
		return "L" + std::to_string(operand.value);
	case OperandType::NONE:
		return "";
	default:
		return "operandToString failure";
	}
}

std::string IRGenerator::operatorToString(IROp operand) {
	switch (operand) {
	case IROp::ADD: return "ADD";
	case IROp::SUB: return "SUB";
	case IROp::MUL: return "MUL";
	case IROp::DIV: return "DIV";
	}
}

int IRGenerator::visitBinaryOpNode(BinaryOpNode& n) {
	// two operands, operator
	// 1 + 2 * 3
	// result stored into virtual register

	int leftReg = n.left->accept(*this);
	int rightReg = n.right->accept(*this);

	// destination register = 2 * 3
	int destReg = allocateVReg();

	IROp op = mapOperator(n.op.type);

	Quad quad;
	quad.dest = { OperandType::VREG, destReg };
	quad.src1 = { OperandType::IMM, leftReg };
	quad.src2 = { OperandType::IMM, rightReg };
	quad.op = op;

	emit(quad);

	return destReg;
}

int IRGenerator::visitUnaryOpNode(UnaryOpNode& n) {
	//// op, operand
	//// !, -

	//int childReg = n.operand->accept(*this);

	//if (n.op.type == TokenType::EXCLAMATION) {

	//}

	return 0;
}

int IRGenerator::visitLiteralNode(LiteralNode& n) {
	return 0;
}

int IRGenerator::visitIdentifierNode(IdentifierNode& n) {
	return 0;
}

int IRGenerator::visitFuncCallNode(FuncCallNode& n) {
	return 0;
}

void IRGenerator::visitVarDecl(VarDeclStmt& n) {

}

void IRGenerator::visitRet(RetStmt& n) {

}

void IRGenerator::visitIf(IfStmt& n) {

}

void IRGenerator::visitWhile(WhileStmt& n) {

}

void IRGenerator::visitBlock(BlockStmt& n) {

}

void IRGenerator::visitExpr(ExprStmt& n) {
	n.expr->accept(*this);
}

void IRGenerator::printIR() {
	for (const auto& quad : instructions_) {
		std::cout << operandToString(quad.dest) << " ";
		std::cout << operatorToString(quad.op) << " ";
		std::cout << operandToString(quad.src1) << " ";
		std::cout << operandToString(quad.src2) << " \n";
	}
}

