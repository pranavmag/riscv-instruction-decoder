#include "shared/error.h"
#include "lexer.h"
#include "parser.h"
#include "astprinter.h"
#include "irgenerator.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

ErrorHandling errorHandler;

void run(std::string_view source, ErrorHandling& errorHandler);

void runFile(const std::string& filePath, ErrorHandling& errHandler) {
	std::ifstream file(filePath);
	if (!file) {
		std::cerr << "Could not open file: " << filePath << '\n';
		std::exit(1);
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	run(buffer.str(), errHandler);
	if (errHandler.hasError) {
		std::exit(2);
	}
}

void runPrompt(ErrorHandling& errHandler) {
	std::string userInput{};
	while (true) {
		std::cout << "> ";
		if (!std::getline(std::cin >> std::ws, userInput)) {
			break;
		}
		run(userInput, errHandler);
		errHandler.hasError = false;
	}
}

void run(std::string_view source, ErrorHandling& errorhandling) {
	Scanner scanner(source, errorhandling);
	std::vector<Token> tokens = scanner.scanTokens();

	Parser parser(tokens, errorhandling);
	std::vector<std::unique_ptr<Stmt>> code = parser.parseCode();

	IRGenerator irGen;
	for (auto& stmt : code) {
		stmt->accept(irGen);
	}

	irGen.printIR();
}


int main(int argc, char* argv[]) {
	if (argc > 2) {
		std::cerr << "Too many arguments\n";
    }
	else if (argc == 2) {
		runFile(argv[1], errorHandler);
	}
	else {
		runPrompt(errorHandler);
	}
}