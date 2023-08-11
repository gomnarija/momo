#ifndef READER_H
#define READER_H

#include <momo.h>


namespace momo{

moValPtr    readString(std::string &);
moListPtr    readFile(const std::string &);

}



#endif
