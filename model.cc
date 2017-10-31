/*！
 * Copyright (c) 2017 by ChengEn Shen
 * \file model.cc
 * \brief 
 * \author ChenEn Shen
 */
#include <thread>
#include <iostream>
#include "model.h"
#include "logging.h"
#include "utils.h"

namespace huoguo {

Model::Model(std::shared_ptr<Args> args):hidden(args->dim),grad(args->dim) {
  args_ = args;
  dict_ = std::make_shared<Dictionary>(args);
  loss = 0.0;
  negative_num = args->neg;
  std::ifstream ifs(args_->input);
  if (!ifs.is_open()) {
    ERROR("Input file cannot be opened!")
    exit(EXIT_FAILURE);
  }
  dict_->readFromFile(ifs);
  ifs.clear();
  dict_->initTableDiscard();
  vocab_size = dict_->getWords();
  std::vector<int64_t> counts = dict_->getCounts();
  hidden.zero();
  grad.zero();
  wi = std::make_shared<Matrix>(dict_->getWords(), args->dim);
  wh = std::make_shared<Matrix>(dict_->getWords(), args->dim);
  wn = std::make_shared<Matrix>(dict_->getWords(), args->dim);
  wi->uniform(1.0/args->dim);
  wh->zero();
  wn->zero();
  buildTree(counts);
  initTableNegatives(counts);
  initSigmoid();
}

Model::~Model() {}

void Model::train() {


  std::ifstream ifs(args_->input);
  const int32_t ntokens = dict_->getTokens();
  int64_t localTokenCount = 0;
  std::vector<int32_t> line, labels;
  std::uniform_int_distribution<> uniform(1, args_->ws);
  while (tokenCount<args_->epoch*ntokens) {
    real progress = real(tokenCount) / (args_->epoch * ntokens);
    real lr = args_->lr * (1.0 - progress);
    localTokenCount += dict_->getLine(ifs, line, labels, rng);
    if (args_->model==1) {
      //cbow
      real local_loss = 0.0;
      std::vector<int32_t > input;
      for (int32_t w = 0; w < line.size(); w++) {
        int32_t boundary = uniform(rng);
        input.clear();
        for (int32_t c = -boundary; c <= boundary; c++) {
          if (c != 0 && w + c >= 0 && w + c < line.size()) {
            input.emplace_back(line[w+c]);
          }
        }
        computeHidden(input);
        grad.zero();
        if (args_->hs) {
          const std::vector<bool>& binaryCode = codes[line[w]];
          const std::vector<int32_t>& path = paths[line[w]];
          for (int32_t i = 0; i < path.size(); i++) {
            real score = sigmoid(wh->dotRow(hidden,path[i]));
            real g = lr * (1-binaryCode[i]-score);
            grad.addRow(*wh,path[i],g);
            wh->addRow(hidden, path[i],g);
          }
        }
        if (args_->ng) {
          real score = sigmoid(wn->dotRow(hidden,line[w]));
          real g = lr * (1-true-score);
          grad.addRow(*wn,line[w],g);
          wn->addRow(hidden, line[w], g);

          vector<int32_t> samples;
          getNegative(line[w], samples);
          for (size_t i=0; i<samples.size(); ++i) {
            score = sigmoid(wn->dotRow(hidden, samples[i]));
            g = lr * (1-false-score);
            grad.addRow(*wn, samples[i], g);
            wn->addRow(hidden, samples[i], g);
          }
        }
        for (auto iter=input.begin(); iter!=input.end(); ++iter) {
          wi->addRow(grad, *iter, 1.0);
        }
        std::cerr <<"更新词语:"<<line[w]<<"的周边词"<<std::endl;
      }
    } else if (args_->model==2){
      //skip-gram
      std::vector<int32_t > input;
      for (int32_t w = 0; w < line.size(); w++) {
        int32_t boundary = uniform(rng);
        for (int32_t c = -boundary; c <= boundary; c++) {
          input.clear();
          input.emplace_back(line[w+c]);
          computeHidden(input);
          if (c != 0 && w + c >= 0 && w + c < line.size()) {
            if (args_->hs) {
              const std::vector<bool>& binaryCode = codes[line[w]];
              const std::vector<int32_t>& path = paths[line[w]];
              for (int32_t i = 0; i < path.size(); i++) {
                real score = sigmoid(wh->dotRow(hidden,path[i]));
                real g = lr * (1-binaryCode[i]-score);
                grad.addRow(*wh,path[i],g);
                wh->addRow(hidden, path[i],g);
              }
            }
            if (args_->ng) {
              real score = sigmoid(wn->dotRow(hidden,w));
              real g = lr * (1-true-score);
              grad.addRow(*wn,w,g);
              wn->addRow(hidden, w, g);

              vector<int32_t> samples;
              getNegative(w, samples);
              for (size_t i=0; i<samples.size(); ++i) {
                score = sigmoid(wn->dotRow(hidden, samples[i]));
                g = lr * (1-false-score);
                grad.addRow(*wn, samples[i], g);
                wn->addRow(hidden, samples[i], g);
              }
            }
            wi->addRow(grad, line[w+c], 1.0);
            std::cerr <<"更新词语:"<<line[w+c]<<std::endl;
          }
        }
      }

    } else {
      ERROR("模型错误")
      exit(EXIT_FAILURE);
    }

    if (localTokenCount > args_->lrUpdateRate) {
      tokenCount += localTokenCount;
      localTokenCount = 0;
    }
  }
}

void Model::computeHidden(const std::vector<int32_t> &input) {
  hidden.zero();
  for (auto it = input.cbegin(); it != input.cend(); ++it) {
    hidden.addRow(*wi, *it);
  }
  hidden.mul(1.0 / input.size());
}

void Model::buildTree(const std::vector<int64_t> counts) {
  tree.resize(2*vocab_size-1);
  for (int32_t i=0; i<2*vocab_size-1; ++i) {
    tree[i].parent = -1;
    tree[i].left = -1;
    tree[i].right = -1;
    tree[i].count = 1e15;
    tree[i].binary = false;
  }

  for (int32_t i=0; i<vocab_size; ++i) {
    tree[i].count = counts[i];
  }
  int32_t leaf = vocab_size-1 ;
  int32_t node = vocab_size;

  for (int32_t i=vocab_size; i<(2*vocab_size-1); ++i) {
    int32_t min[2];
    for (int32_t j=0; j<2; ++j) {
      if (leaf>=0 && tree[leaf].count<tree[node].count) {
        min[j] = leaf--;
      } else {
        min[j] = node++;
      }
    }
    tree[i].left = min[0];
    tree[i].right = min[1];
    tree[min[0]].parent = i;
    tree[min[1]].parent = i;
    tree[i].count = tree[min[0]].count + tree[min[1]].count;
    tree[min[1]].binary = true;
  }

  for (int32_t i=0; i<vocab_size; ++i) {
    std::vector<int32_t> path;
    std::vector<bool> code;
    int32_t node = i;
    while (tree[node].parent!=-1) {
      path.push_back(tree[node].parent-vocab_size);
      code.push_back(tree[node].binary);
      node = tree[node].parent;
    }
    paths.push_back(path);
    codes.push_back(code);
  }
}

void Model::getNegative(int32_t word, std::vector<int32_t> &samples) {
  samples.clear();
  int32_t negative;
  for (int32_t i=0; i<negative_num; ++i) {
    do {
      negative = negatives[negpos];
      negpos = (negpos+1)%negatives.size();
    }while (negative == word);
    samples.emplace_back(negative);
  }
}

void Model::initTableNegatives(const std::vector<int64_t > &counts) {
  float z = 0.0;
  for (int32_t i=0; i<counts.size(); ++i) {
    z += pow(counts[i],0.5);
  }

  for (int32_t i=0; i<counts.size(); ++i) {
    float c = pow(counts[i],0.5);
    for (int32_t j=0; j<c*NEGATIVE_TABLE_SIZE/z; ++j) {
      negatives.emplace_back(i);
    }
  }
  std::shuffle(negatives.begin(), negatives.end(), rng);
}

void Model::initSigmoid() {
  t_sigmoid = new real[SIGMOID_TABLE_SIZE + 1];
  for (int i = 0; i < SIGMOID_TABLE_SIZE + 1; i++) {
    real x = real(i * 2 * MAX_SIGMOID) / SIGMOID_TABLE_SIZE - MAX_SIGMOID;
    t_sigmoid[i] = 1.0 / (1.0 + std::exp(-x));
  }
}

real Model::sigmoid(real x) const {
  if (x < -MAX_SIGMOID) {
    return 0.0;
  } else if (x > MAX_SIGMOID) {
    return 1.0;
  } else {
    int i = int((x + MAX_SIGMOID) * SIGMOID_TABLE_SIZE / MAX_SIGMOID / 2);
    return t_sigmoid[i];
  }
}

}