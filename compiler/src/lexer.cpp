#include "lexer.h"
#include "shared/error.h"
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <variant>

constexpr bool isDigit(char c) noexcept {
	if (c >= '0' && c <= '9') {
		return true;
	}
	return false;
}

constexpr bool isAlpha(char c) noexcept {
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_')) {
		return true;
	}
	return false;
}

constexpr bool isAlphaNumeric(char c) noexcept {
	if ((isAlpha(c)) || isDigit(c)) {
		return true;
	}
	return false;
}

std::vector<Token> Scanner::scanTokens() {
	while (!isAtEnd()) {
		start_ = current_;
		scanToken();
	}

	tokens_.emplace_back(TokenType::EOF_TOKEN, "", std::monostate(), line_);
	return tokens_;
}

void Scanner::addToken(TokenType type) {
	std::string text{ source_.substr(start_, current_ - start_) };
	tokens_.emplace_back(type, text, std::monostate{}, line_);
}

void Scanner::addToken(TokenType type, Literal literal) {
	std::string lexeme{ source_.substr(start_, current_ - start_) };
	tokens_.emplace_back(type, std::move(lexeme), literal, line_);
}

void Scanner::scanToken() {
	char c = advance();
	switch (c) {
	case ',': addToken(TokenType::COMMA); break;
	case ';': addToken(TokenType::SEMICOLON); break;
	case '(': addToken(TokenType::LEFT_PAREN); break;
	case ')': addToken(TokenType::RIGHT_PAREN); break;
	case '{': addToken(TokenType::LEFT_BRACE); break;
	case '}': addToken(TokenType::RIGHT_BRACE); break;
	case '+': addToken(TokenType::PLUS); break;
	case '-': addToken(TokenType::MINUS); break;
	case '*': addToken(TokenType::STAR); break;
	case '%': addToken(TokenType::PERCENT); break;
	case '/':
		if (match('/')) {
			while (peek() != '\n' && !isAtEnd()) {
				advance();
			}
			break;
		}
		else {
			addToken(TokenType::SLASH); break;
		}
	case '!':
		if (match('=')) {
			addToken(TokenType::EXCLAMATION_EQUAL); break;
		}
		else {
			addToken(TokenType::EXCLAMATION); break;
		}

	case '=':
		if (match('=')) {
			addToken(TokenType::EQUAL_EQUAL); break;
		}
		else {
			addToken(TokenType::EQUAL); break;
		}
	case '<':
		if (match('=')) {
			addToken(TokenType::LESS_EQUAL); break;
		}
		else {
			addToken(TokenType::LESS); break;
		}
	case '>':
		if (match('=')) {
			addToken(TokenType::GREATER_EQUAL); break;
		}
		else {
			addToken(TokenType::GREATER); break;
		}
	case ' ':
	case '\t':
	case '\r':
		break;
	case '\n': ++line_; break;
	case '"': string(); break;
	default:
		if (isDigit(c)) {
			number();
		}
		else if (isAlpha(c)) {
			identifier();
		}
		else {
			error_.error(line_, "Unexpected Charcter.");
		}
	}

}

void Scanner::string() {
	while (peek() != '"' && !isAtEnd()) {
		if (peek() == '\n') {
			++line_;
		}
		advance();
	}
	if (isAtEnd()) {
		error_.error(line_, "Unterminated String.");
		return;
	}
	advance();

	std::string stringLiteral{ source_.substr(start_ + 1, current_ - start_ - 2) };
	addToken(TokenType::STRING, stringLiteral);
}

void Scanner::number() {
	while (isDigit(peek()) && !isAtEnd()) {
		advance();
	}

	if (peek() == '.' && isDigit(peekNext())) {
		advance();
		while (isDigit(peek()) && !isAtEnd()) {
			advance();
		}
	}

	std::string numberLiteral{ source_.substr(start_, current_ - start_) };
	addToken(TokenType::NUMBER, numberLiteral);
}

const std::unordered_map<std::string, TokenType> Scanner::keywords = {
	{"if", TokenType::IF},
	{"else", TokenType::ELSE},
	{"while", TokenType::WHILE},
	{"float", TokenType::FLOAT},
	{"int", TokenType::INT},
	{"void", TokenType::VOID},
	{"char", TokenType::CHAR},
	{"print", TokenType::PRINT},
	{"return", TokenType::RETURN},
};

void Scanner::identifier() {
	while (isAlphaNumeric(peek()) && !isAtEnd()) {
		advance();
	}

	std::string text{ source_.substr(start_, current_ - start_) };
	auto word = keywords.find(text);
	
	if (word != keywords.end()) {
		addToken(word->second, text);
	}
	else {
		addToken(TokenType::IDENTIFIER, text);
	}
	
}

