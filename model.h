/*！
 * Copyright (c) 2017 by ChengEn Shen
 * \file model.h
 * \brief 
 * \author ChenEn Shen
 */
#ifndef HUOGUONLP_MODEL_H
#define HUOGUONLP_MODEL_H

#include <random>
#include "matrix.h"
#include "vector.h"
#include "args.h"
#include "Dictionary.h"

namespace huoguo {
struct Node {
  int32_t parent;
  int32_t left;
  int32_t right;
  int64_t count;
  bool binary;
};
#define SIGMOID_TABLE_SIZE 512
#define MAX_SIGMOID 8
class Model {
 public:
  Model(std::shared_ptr<Args> args);
  ~Model();

  void train();

  void trainThread(int32_t thread_id);

  void computeHidden(const std::vector<int32_t> &input);

  void buildTree(const std::vector<int64_t> counts);

  void getNegative(int32_t word, std::vector<int32_t>& samples);

  void initTableNegatives(const std::vector<int64_t>& counts);

  void save();

  void load(const std::string& modelFile);

  real sigmoid(real) const;

  void initSigmoid();
 private:
  std::shared_ptr<Dictionary> dict_;
  std::shared_ptr<Args> args_;

  std::shared_ptr<Matrix> wi; //词向量
  std::shared_ptr<Matrix> wh; //huffman树参数
  std::shared_ptr<Matrix> wn; //ns 参数
  Vector hidden;
  Vector grad;
  float loss;

  int32_t negative_num;
  std::vector<int32_t> negatives;
  size_t negpos;
  real* t_sigmoid;

  std::vector< std::vector<int32_t> > paths; //每个词哈夫曼树对应的路径
  std::vector< std::vector<bool> > codes;  //每个词哈夫曼树的编码 0101011
  std::vector<Node> tree; //哈夫曼树

  int vocab_size;
  static const int32_t NEGATIVE_TABLE_SIZE = 10000000;
  std::minstd_rand rng;
  std::atomic<int64_t> tokenCount;
};
}

#endif //HUOGUONLP_MODEL_H
