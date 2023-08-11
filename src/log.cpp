#include "momo.h"



namespace momo{

std::string                 LOG_FILENAME = "[nepoznato]";
uint32_t                    LOG_LINE;
std::stringstream			LOG_STREAM;


/*
    todo:
        add lines, time...
*/
void write_error(const std::string &msg){
    LOG_STREAM << "[GREŠKA] " <<  LOG_FILENAME <<  ":" << LOG_LINE << " "  << msg << "\n";
}

void write_warning(const std::string &msg){
    LOG_STREAM << "[POZOR] " <<  LOG_FILENAME <<  ":" << LOG_LINE << " "  << msg << "\n";
}

void write_debug(const std::string &msg){
    LOG_STREAM << "[INFO] " <<  LOG_FILENAME <<  ":" << LOG_LINE << " "  << msg << "\n";
}

}