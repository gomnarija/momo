#include "momo.h"
#include "tokenizer.h"
#include "reader.h"

#include <iostream>
#include <string>

static void run_file(const std::string& path) {
	momo::moEnv env;
	auto& logger = momo::Logger::instance();

	momo::moListPtr res = momo::readFile(path);
	for (size_t i = 0; i < res->size(); i++) {
		momo::moValPtr eres = momo::eval(res->at(i), env);
		if (eres->getType() != momo::MO_TYPE::MO_NIL) {
			std::cout << eres->print() << "\n";
		}
		if (logger.hasErrors()) {
			logger.flush();
		}
	}
}

static void run_repl() {
	momo::moEnv env;
	auto& logger = momo::Logger::instance();
	logger.setFilename("[repl]");

	std::cout << "momo repl" << std::endl;

	std::string line;
	std::string buffer;
	int depth = 0;

	while (true) {
		std::cout << (depth == 0 ? ">> " : ".. ");

		if (!std::getline(std::cin, line)) {
			std::cout << std::endl;
			break;
		}

		// Track bracket depth (naive — ignores brackets inside strings)
		for (char c : line) {
			if (c == '[') depth++;
			else if (c == ']') depth--;
		}

		buffer += line + "\n";

		if (depth > 0) {
			continue;
		}

		if (depth < 0) {
			logger.error("višak zatvorenih zagrada.");
			logger.flush();
			buffer.clear();
			depth = 0;
			continue;
		}

		// Balanced — parse and eval
		if (!buffer.empty()) {
			logger.advanceLine();
			momo::moValPtr ast = momo::readString(buffer);
			if (ast != nullptr) {
				momo::moValPtr result = momo::eval(ast, env);
				if (result->getType() != momo::MO_TYPE::MO_NIL) {
					std::cout << result->print() << std::endl;
				}
			}
			if (logger.hasErrors()) {
				logger.flush();
			}
			logger.clear();
		}

		buffer.clear();
		depth = 0;
	}
}

int main(int argc, char* argv[]) {
	if (argc > 1) {
		run_file(argv[1]);
	} else {
		run_repl();
	}
	return 0;
}
