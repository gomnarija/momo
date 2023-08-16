#ifndef READER_H
#define READER_H

#include <momo.h>
#include <fstream>

namespace momo{

moValPtr     read_string(std::string &);
moListPtr    read_file(const std::string &);
moListPtr    read_file(std::ifstream &);

}



#endif
