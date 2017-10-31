/*！
 * Copyright (c) 2017 by ChengEn Shen
 * \file args.h
 * \brief 
 * \author ChenEn Shen
 */
#ifndef HUOGUONLP_ARGS_H
#define HUOGUONLP_ARGS_H

#include <string>

namespace huoguo {
class Args {
 public:
  Args();
  Args(std::string input,  //输入
       std::string output,  //输出
       bool hs,  //是否使用hs
       bool ng,  //是否使用ng
       int model);
  ~Args();

 public:
  std::string input;
  std::string output;
  double lr;
  int lrUpdateRate;
  int dim;   //词向量纬度
  int ws;
  int epoch;
  int minCount;
  int neg;
  int thread;
  bool hs;
  bool ng;
  int model; //1.cbow, 2.skip-gram
  int32_t vocab_size;
  double t;
};

}

#endif //HUOGUONLP_ARGS_H
