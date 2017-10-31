/*！
 * Copyright (c) 2017 by ChengEn Shen
 * \file args.cc
 * \brief 
 * \author ChenEn Shen
 */
#include "args.h"

namespace huoguo {
Args::Args() {}

Args::Args(std::string input,
           std::string output,
           bool hs,  //是否使用hs
           bool ng,
           int model) {
  this->input = input;
  this->output = output;
  this->hs = hs;
  this->ng = ng;
  this->model = model;
  lr = 0.05;
  dim = 100;
  ws = 5;
  epoch = 2;
  minCount = 5;
  neg = 5;
  thread = 12;
  lrUpdateRate = 100;
  t = 1e-4;
}

Args::~Args() {

}
}