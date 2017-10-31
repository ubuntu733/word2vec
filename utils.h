/*！
 * Copyright (c) 2017 by ChengEn Shen
 * \file utils.h
 * \brief 
 * \author ChenEn Shen
 */
#ifndef HUOGUONLP_UTILS_H
#define HUOGUONLP_UTILS_H

#include <fstream>
namespace huoguo {
int64_t size(std::ifstream&);
void seek(std::ifstream&, int64_t);
}
#endif //HUOGUONLP_UTILS_H
