/*！
 * Copyright (c) 2017 by ChengEn Shen
 * \file Dictionary.h
 * \brief 
 * \author ChenEn Shen
 */
#ifndef HUOGUONLP_DICTIONARY_H
#define HUOGUONLP_DICTIONARY_H

#include <cstdint>
#include <string>
#include <vector>
#include <istream>
#include <algorithm>
#include <random>
#include "args.h"
#include "real.h"

namespace huoguo {

struct entity {
  int64_t count;
  std::string words;
};
class Dictionary {
 public:
  Dictionary(std::shared_ptr<Args>);
  ~Dictionary();
  void readFromFile(std::istream& input);
  std::vector<int64_t> getCounts() const;
  int32_t getWords();
  int64_t getTokens();
  int32_t getLine(std::istream&, std::vector<int32_t>&, std::vector<int32_t>&,
                  std::vector<int32_t>&, std::minstd_rand&) const;
  int32_t getLine(std::istream&, std::vector<int32_t>&,
                  std::vector<int32_t>&, std::minstd_rand&) const;
  int32_t getId(const std::string& w, uint32_t h) const;
  int32_t getId(const std::string&) const;
  void initTableDiscard();
  bool discard(int32_t, float) const;
 private:
  void add(const std::string& word);
  bool readWord(std::istream& in, std::string& word) const;
  uint32_t hash(const std::string& str) const;
  int32_t find(const std::string&) const;
  int32_t find(const std::string&, uint32_t h) const;

  void threshold(int64_t t);
 private:
  static const int32_t MAX_VOCAB_SIZE = 30000000;
  static const int32_t MAX_LINE_SIZE = 1024;

  std::shared_ptr<Args> args_;
  std::vector<int32_t> word2int_;
  std::vector<entity> words_;

  int32_t size_;
  int32_t nwords_;
  int64_t ntokens_;
  static const std::string EOS;
  static const std::string BOW;
  static const std::string EOW;

  std::vector<real> pdiscard_;
};
}

#endif //HUOGUONLP_DICTIONARY_H
