#include <gtest/gtest.h>
#include "log.h"
#include <sstream>

using namespace momo;

// Clean state before each test
class LoggerTest : public ::testing::Test {
protected:
	void SetUp() override {
		Logger::instance().clear();
		Logger::instance().setFilename("[test]");
		Logger::instance().setLine(0);
	}
	void TearDown() override {
		Logger::instance().clear();
	}
};

// No errors on fresh logger
TEST_F(LoggerTest, InitiallyNoErrors) {
	EXPECT_FALSE(Logger::instance().hasErrors());
}

// error() registers an error
TEST_F(LoggerTest, ErrorSetsHasErrors) {
	Logger::instance().error("test error");
	EXPECT_TRUE(Logger::instance().hasErrors());
}

// warning() does not count as error
TEST_F(LoggerTest, WarningIsNotError) {
	Logger::instance().warning("just a warning");
	EXPECT_FALSE(Logger::instance().hasErrors());
}

// debug() does not count as error
TEST_F(LoggerTest, DebugIsNotError) {
	Logger::instance().debug("info message");
	EXPECT_FALSE(Logger::instance().hasErrors());
}

// flush() outputs error with correct format
TEST_F(LoggerTest, FlushFormatsError) {
	Logger::instance().setLine(10);
	Logger::instance().error("something broke");
	std::ostringstream out;
	Logger::instance().flush(out);
	std::string output = out.str();
	EXPECT_NE(output.find("[GREŠKA]"), std::string::npos);
	EXPECT_NE(output.find("[test]:10"), std::string::npos);
	EXPECT_NE(output.find("something broke"), std::string::npos);
}

// flush() outputs warning with correct prefix
TEST_F(LoggerTest, FlushFormatsWarning) {
	Logger::instance().warning("careful");
	std::ostringstream out;
	Logger::instance().flush(out);
	EXPECT_NE(out.str().find("[POZOR]"), std::string::npos);
}

// flush() outputs debug with correct prefix
TEST_F(LoggerTest, FlushFormatsDebug) {
	Logger::instance().debug("just info");
	std::ostringstream out;
	Logger::instance().flush(out);
	EXPECT_NE(out.str().find("[INFO]"), std::string::npos);
}

// flush() clears entries
TEST_F(LoggerTest, FlushClearsEntries) {
	Logger::instance().error("err");
	std::ostringstream out;
	Logger::instance().flush(out);
	EXPECT_FALSE(Logger::instance().hasErrors());
}

// clear() removes all entries
TEST_F(LoggerTest, ClearRemovesAll) {
	Logger::instance().error("a");
	Logger::instance().warning("b");
	Logger::instance().clear();
	EXPECT_FALSE(Logger::instance().hasErrors());
	std::ostringstream out;
	Logger::instance().flush(out);
	EXPECT_TRUE(out.str().empty());
}

// advanceLine increments line counter
TEST_F(LoggerTest, AdvanceLine) {
	Logger::instance().setLine(5);
	Logger::instance().advanceLine();
	EXPECT_EQ(Logger::instance().getLine(), 6u);
}

// setFilename updates context
TEST_F(LoggerTest, SetFilename) {
	Logger::instance().setFilename("myfile.momo");
	EXPECT_EQ(Logger::instance().getFilename(), "myfile.momo");
}

// Free function wrappers delegate to singleton
TEST_F(LoggerTest, FreeFunctionWriteError) {
	write_error("via free func");
	EXPECT_TRUE(Logger::instance().hasErrors());
	std::ostringstream out;
	Logger::instance().flush(out);
	EXPECT_NE(out.str().find("via free func"), std::string::npos);
}

TEST_F(LoggerTest, FreeFunctionWriteWarning) {
	write_warning("warn free");
	std::ostringstream out;
	Logger::instance().flush(out);
	EXPECT_NE(out.str().find("[POZOR]"), std::string::npos);
}

TEST_F(LoggerTest, FreeFunctionWriteDebug) {
	write_debug("dbg free");
	std::ostringstream out;
	Logger::instance().flush(out);
	EXPECT_NE(out.str().find("[INFO]"), std::string::npos);
}

// Multiple entries flush in order
TEST_F(LoggerTest, MultipleEntriesInOrder) {
	Logger::instance().error("first");
	Logger::instance().warning("second");
	Logger::instance().debug("third");
	std::ostringstream out;
	Logger::instance().flush(out);
	std::string output = out.str();
	// "first" should appear before "second", "second" before "third"
	EXPECT_LT(output.find("first"), output.find("second"));
	EXPECT_LT(output.find("second"), output.find("third"));
}
