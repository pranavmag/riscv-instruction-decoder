# pragma once

#include "shared/error.h"
#include <string>
#include <string_view>
#include <variant>
#include <vector>
#include <unordered_map>

enum TokenType {
	// Single-Character
	COMMA, SEMICOLON, LEFT_PAREN, RIGHT_PAREN,
	LEFT_BRACE, RIGHT_BRACE, PLUS, MINUS, STAR, SLASH, PERCENT,
	EXCLAMATION, EQUAL, GREATER, LESS, DOUBLE_QUOTE,

	// Double-Character
	EXCLAMATION_EQUAL, EQUAL_EQUAL,
	GREATER_EQUAL, LESS_EQUAL,
	AND, OR,

	// Literals
	STRING, NUMBER, IDENTIFIER,

	// keywords - Types
	INT, FLOAT, VOID, CHAR,

	// keywords - Control Flow/Loops
	IF, ELSE, WHILE, FOR, RETURN,

	//int x = 3;
	// a-z A-Z _
	//int int = 3;
	PRINT,

	EOF_TOKEN
};

using Literal = std::variant<std::monostate, int, float, std::string>;

struct Token {
	TokenType type;
	std::string lexeme;
	Literal literal;
	int line;

	
	Token(TokenType type, std::string lex, std::variant< std::monostate, int, float, std::string> literal, int line) :
		type(type), lexeme(std::move(lex)), literal(std::move(literal)), line(line) {
	}
};

inline std::string tokenTypeToString(TokenType type) {
	switch (type) {
	case TokenType::COMMA:              return "COMMA";
	case TokenType::SEMICOLON:          return "SEMICOLON";
	case TokenType::LEFT_PAREN:         return "LEFT_PAREN";
	case TokenType::RIGHT_PAREN:        return "RIGHT_PAREN";
	case TokenType::LEFT_BRACE:         return "LEFT_BRACE";
	case TokenType::RIGHT_BRACE:        return "RIGHT_BRACE";
	case TokenType::PLUS:               return "PLUS";
	case TokenType::MINUS:              return "MINUS";
	case TokenType::STAR:               return "STAR";
	case TokenType::SLASH:              return "SLASH";
	case TokenType::PERCENT:            return "PERCENT";
	case TokenType::EXCLAMATION:        return "EXCLAMATION";
	case TokenType::EQUAL:              return "EQUAL";
	case TokenType::GREATER:            return "GREATER";
	case TokenType::LESS:               return "LESS";
	case TokenType::EXCLAMATION_EQUAL:  return "EXCLAMATION_EQUAL";
	case TokenType::EQUAL_EQUAL:        return "EQUAL_EQUAL";
	case TokenType::GREATER_EQUAL:      return "GREATER_EQUAL";
	case TokenType::LESS_EQUAL:         return "LESS_EQUAL";
	case TokenType::STRING:             return "STRING";
	case TokenType::NUMBER:             return "NUMBER";
	case TokenType::IDENTIFIER:         return "IDENTIFIER";
	case TokenType::INT:                return "INT";
	case TokenType::FLOAT:              return "FLOAT";
	case TokenType::VOID:               return "VOID";
	case TokenType::CHAR:               return "CHAR";
	case TokenType::IF:                 return "IF";
	case TokenType::ELSE:               return "ELSE";
	case TokenType::WHILE:              return "WHILE";
	case TokenType::FOR:                return "FOR";
	case TokenType::RETURN:             return "RETURN";
	case TokenType::PRINT:              return "PRINT";
	case TokenType::EOF_TOKEN:          return "EOF";
	default:                            return "UNKNOWN";
	}
}

class Scanner {
private:
	std::string_view source_;
	std::vector<Token> tokens_;
	int start_ = 0;
	int current_ = 0;
	int line_ = 1;
	ErrorHandling& error_;

public:
	Scanner(std::string_view source, ErrorHandling& error) : source_(source), error_(error) {
		std::vector<Token> tokens{};
	}

	bool isAtEnd() const {
		return static_cast<std::string_view::size_type>(current_) >= source_.size();
	}

	char advance() {
		return source_[current_++];
	}

	char peek() const {
		if (isAtEnd()) {
			return '\0';
		}
		return source_[current_];
		
	}

	char peekNext() const {
		if (static_cast<std::string_view::size_type>(current_) + 1 >= source_.size()) {
			return '\0';
		}
		return source_[current_ + 1];
	}

	bool match(char expected) {
		if (isAtEnd() || source_[current_] != expected) {
			return false;
		}
		++current_;
		return true;
	}

	static const std::unordered_map<std::string, TokenType> keywords;

	void addToken(TokenType type);
	void addToken(TokenType type, Literal literal);
	std::vector<Token> scanTokens();
	void scanToken();
	void string();
	void number();
	void identifier();
};