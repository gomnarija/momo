#include "log.h"

namespace momo {

Logger& Logger::instance() {
	static Logger logger;
	return logger;
}

void Logger::setFilename(const std::string& fname) {
	this->filename = fname;
}

void Logger::setLine(uint32_t ln) {
	this->line = ln;
}

void Logger::advanceLine() {
	this->line++;
}

std::string Logger::getFilename() const {
	return this->filename;
}

uint32_t Logger::getLine() const {
	return this->line;
}

void Logger::error(const std::string& msg) {
	entries.push_back({LogLevel::ERROR, filename, line, msg});
}

void Logger::warning(const std::string& msg) {
	entries.push_back({LogLevel::WARNING, filename, line, msg});
}

void Logger::debug(const std::string& msg) {
	entries.push_back({LogLevel::DEBUG, filename, line, msg});
}

bool Logger::hasErrors() const {
	for (const auto& entry : entries) {
		if (entry.level == LogLevel::ERROR)
			return true;
	}
	return false;
}

void Logger::flush(std::ostream& out) {
	for (const auto& entry : entries) {
		switch (entry.level) {
			case LogLevel::ERROR:
				out << "[GREŠKA] ";
				break;
			case LogLevel::WARNING:
				out << "[POZOR] ";
				break;
			case LogLevel::DEBUG:
				out << "[INFO] ";
				break;
		}
		out << entry.filename << ":" << entry.line << " " << entry.message << "\n";
	}
	clear();
}

void Logger::clear() {
	entries.clear();
}

// Free function wrappers
void write_error(const std::string& msg) {
	Logger::instance().error(msg);
}

void write_warning(const std::string& msg) {
	Logger::instance().warning(msg);
}

void write_debug(const std::string& msg) {
	Logger::instance().debug(msg);
}

}