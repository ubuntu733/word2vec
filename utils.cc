/*！
 * Copyright (c) 2017 by ChengEn Shen
 * \file utils.cc
 * \brief 
 * \author ChenEn Shen
 */
#include "utils.h"
namespace huoguo {
int64_t size(std::ifstream& ifs) {
  ifs.seekg(std::streamoff(0), std::ios::end);
  return ifs.tellg();
}

void seek(std::ifstream& ifs, int64_t pos) {
  ifs.clear();
  ifs.seekg(std::streampos(pos));
}
}