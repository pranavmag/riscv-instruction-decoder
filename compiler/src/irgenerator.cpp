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
	case IROp::PARAM: return "PARAM";
	case IROp::CALL: return "CALL";
	case IROp::RET: return "RET";



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
	std::vector<int> arguments;
	for (auto& arg : n.args) {
		int param = arg->accept(*this);
		arguments.push_back(param);
	}

	for (auto& arg : arguments) {
		Quad paramQuad;
		paramQuad.dest = { OperandType::NONE, 0};
		paramQuad.src1 = { OperandType::VREG, arg };
		paramQuad.src2 = { OperandType::NONE, 0 };
		paramQuad.op = IROp::PARAM;
		emit(paramQuad);
	}

	int destReg = allocateVReg();

	// v4 CALL 3

	Quad callQuad;
	callQuad.dest = { OperandType::VREG, destReg };
	callQuad.src1 = { OperandType::IMM, static_cast<int>(arguments.size())};
	callQuad.src2 = { OperandType::NONE, 0 };
	callQuad.op = IROp::CALL;

	emit(callQuad);

	return destReg;
}

void IRGenerator::visitVarDecl(VarDeclStmt& n) {
	std::string varName = n.name.lexeme;

	// int x;
	// int x = 3;

	int initReg{};
	if (n.initializer) {
		initReg = n.initializer->accept(*this);
	}

	symbolTable_[varName] = initReg;
}

void IRGenerator::visitRet(RetStmt& n) {
	int retReg{};

	if (n.ret) {
		retReg = n.ret->accept(*this);
	}

	Quad quad;
	quad.dest = { OperandType::VREG, retReg };
	quad.src1 = { OperandType::NONE, 0 };
	quad.src2 = { OperandType::NONE, 0 };
	quad.op = IROp::RET;

	emit(quad);
}

void IRGenerator::visitIf(IfStmt& n) {
	/*
		some code

		if (x == y) {
			code
		}
		else {
			code
		}

		code
	
	
	
	
	
	*/
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

