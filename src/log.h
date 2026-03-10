#ifndef LOG_H
#define LOG_H

#include <string>
#include <vector>
#include <iostream>

namespace momo {

enum class LogLevel {
	ERROR,
	WARNING,
	DEBUG
};

struct LogEntry {
	LogLevel level;
	std::string filename;
	uint32_t line;
	std::string message;
};

class Logger {
public:
	static Logger& instance();

	void setFilename(const std::string& filename);
	void setLine(uint32_t line);
	void advanceLine();

	std::string getFilename() const;
	uint32_t getLine() const;

	void error(const std::string& msg);
	void warning(const std::string& msg);
	void debug(const std::string& msg);

	bool hasErrors() const;
	void flush(std::ostream& out = std::cerr);
	void clear();

private:
	Logger() = default;
	std::vector<LogEntry> entries;
	std::string filename = "[nepoznato]";
	uint32_t line = 0;
};

// Convenience free functions — delegate to Logger::instance()
void write_error(const std::string& msg);
void write_warning(const std::string& msg);
void write_debug(const std::string& msg);

}

#endif
