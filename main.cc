/*！
 * Copyright (c) 2017 by ChengEn Shen
 * \file main.cc
 * \brief 
 * \author ChenEn Shen
 */
#include "args.h"
#include "model.h"
#include <fstream>
#include <iostream>

int main() {
  std::string input("/Users/chenchengen/CLionProjects/testWord2vec/fil9");
  std::string output("");
  std::shared_ptr<huoguo::Args> a =std::make_shared<huoguo::Args>();
  a.reset(new huoguo::Args(input,output,true, false,1));
  std::shared_ptr<huoguo::Dictionary> dict = std::make_shared<huoguo::Dictionary>(a);
  huoguo::Model model(a);
  model.train();

  return 0;
}