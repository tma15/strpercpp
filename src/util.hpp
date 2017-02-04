#ifndef STPERCPP_UTIL_H_
#define STPERCPP_UTIL_H_

#include <stdio.h>
#include <string>

void save_string(FILE* fp, const std::string& line);

void read_string(FILE* fp, std::string* line);

#endif
