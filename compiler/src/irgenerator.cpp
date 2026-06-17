#include "irgenerator.h"
#include <vector>
#include <iostream>
#include <stdexcept>

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
	case TokenType::PERCENT: return IROp::REM;
	default: return IROp::UNKNOWN;



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
	case IROp::REM: return "REM";
	case IROp::NOT: return "NOT";
	case IROp::NEG: return "NEG";
	case IROp::LOAD_IMM: return "LOAD_IMM";
	default: return "Unknown OP";


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
	quad.src1 = { OperandType::VREG, leftReg };
	quad.src2 = { OperandType::VREG, rightReg };
	quad.op = op;

	emit(quad);

	return destReg;
}

int IRGenerator::visitUnaryOpNode(UnaryOpNode& n) {
	// op, operand
	// !, -

	int childReg = n.operand->accept(*this);

	int destReg = allocateVReg();

	IROp op;
	if (n.op.type == TokenType::MINUS) {
		op = IROp::NEG;
	}
	else if (n.op.type == TokenType::EXCLAMATION) {
		op = IROp::NOT;
	}
	else {
		throw std::runtime_error("Unknown Operator");
	}

	Quad quad;
	quad.dest = { OperandType::VREG, destReg };
	quad.src1 = { OperandType::VREG, childReg };
	quad.src2 = { OperandType::NONE, 0 };
	quad.op = op;

	emit(quad);

	return destReg;
}

int IRGenerator::visitLiteralNode(LiteralNode& n) {
	int destReg = allocateVReg();
	int literal = 0;

	if (std::holds_alternative<int>(n.value)) {
		literal = std::get<int>(n.value);
	}
	else if (std::holds_alternative<float>(n.value)) {
		literal = static_cast<int>(std::get<float>(n.value));
	}
	else if (std::holds_alternative<std::string>(n.value)) {
		literal = std::stoi(std::get<std::string>(n.value));
	}

	Quad quad;
	quad.dest = { OperandType::VREG, destReg };
	quad.src1 = { OperandType::IMM, literal };
	quad.src2 = { OperandType::NONE, 0 };
	quad.op = IROp::LOAD_IMM;

	emit(quad);

	return destReg;
}

int IRGenerator::visitIdentifierNode(IdentifierNode& n) {
	std::string varName = n.name.lexeme;

	if (symbolTable_.find(varName) == symbolTable_.end()) {
		throw std::runtime_error("Error in visitIdentifierNode");
	}

	return symbolTable_[varName];
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

