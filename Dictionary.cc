/*！
 * Copyright (c) 2017 by ChengEn Shen
 * \file Dictionary.cc
 * \brief 
 * \author ChenEn Shen
 */
#include <cassert>
#include <iostream>
#include "Dictionary.h"

namespace huoguo {
const std::string Dictionary::EOS = "</s>";
const std::string Dictionary::BOW = "<";
const std::string Dictionary::EOW = ">";

Dictionary::Dictionary(std::shared_ptr<Args> args) : args_(args),
                                                     ntokens_(0),nwords_(0),
                                                     size_(0),word2int_(MAX_VOCAB_SIZE,-1) {}

Dictionary::~Dictionary() {}

void Dictionary::readFromFile(std::istream &input) {
  std::string word;
  int64_t minThreshold = 1;
  while (readWord(input,word)) {
    add(word);
    if (ntokens_ % 1000000 == 0 ) {
      std::cerr << "\rRead " << ntokens_  / 1000000 << "M words" << std::flush;
    }
    if (size_ > 0.75*MAX_VOCAB_SIZE) {
      ++minThreshold;
      threshold(++minThreshold);
    }
  }
  threshold(args_->minCount);
}
void Dictionary::add(const std::string &word) {
  int32_t id = find(word);
  ++ntokens_;
  if (word2int_[id]==-1) {
    entity entity1;
    entity1.words = word;
    entity1.count = 1;
    words_.push_back(entity1);
    word2int_[id] = size_++;
  } else {
    words_[word2int_[id]].count++;
  }
}

int32_t Dictionary::find(const std::string &word) const {
  return find(word,hash(word));
}

int32_t Dictionary::find(const std::string &word, uint32_t h) const {
  int32_t id = h % MAX_VOCAB_SIZE;
  while (word2int_[id]!=-1 && words_[word2int_[id]].words!=word) {
    id = (id+1) % MAX_VOCAB_SIZE;
  }
  return id;
}

uint32_t Dictionary::hash(const std::string& str) const {
  uint32_t h = 2166136261;
  for (size_t i = 0; i < str.size(); i++) {
    h = h ^ uint32_t(str[i]);
    h = h * 16777619;
  }
  return h;
}

bool Dictionary::readWord(std::istream &in, std::string &word) const {
  char c;
  std::streambuf& sb = *in.rdbuf();
  word.clear();
  while ((c = sb.sbumpc()) != EOF) {
    if (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v' ||
        c == '\f' || c == '\0') {
      if (word.empty()) {
        if (c == '\n') {
          word += EOS;
          return true;
        }
        continue;
      } else {
        if (c == '\n')
          sb.sungetc();
        return true;
      }
    }
    word.push_back(c);
  }
  // trigger eofbit
  in.get();
  return !word.empty();
}
void Dictionary::threshold(int64_t t) {
  std::sort(words_.begin(), words_.end(), [](const entity& e1, const entity& e2) {
    return e1.count > e2.count;
  });
  words_.erase(remove_if(words_.begin(), words_.end(), [&](const entity& e) {
    return (e.count < t);
  }), words_.end());
  words_.shrink_to_fit();
  size_ = 0;
  nwords_ = 0;
  std::fill(word2int_.begin(), word2int_.end(), -1);
  for (auto it = words_.begin(); it != words_.end(); ++it) {
    int32_t h = find(it->words);
    word2int_[h] = size_++;
    ++nwords_;
  }
}

std::vector<int64_t> Dictionary::getCounts() const {
  std::vector<int64_t> counts;
  for (auto& w : words_) {
    counts.emplace_back(w.count);
  }
  return counts;
}

int32_t Dictionary::getWords() {
  return nwords_;
}

int64_t Dictionary::getTokens() {
  return ntokens_;
  return ntokens_;
}

int32_t Dictionary::getLine(std::istream& in,
                            std::vector<int32_t>& words,
                            std::vector<int32_t>& word_hashes,
                            std::vector<int32_t>& labels,
                            std::minstd_rand& rng) const {
  std::uniform_real_distribution<> uniform(0, 1);

  if (in.eof()) {
    in.clear();
    in.seekg(std::streampos(0));
  }

  words.clear();
  labels.clear();
  word_hashes.clear();
  int32_t ntokens = 0;
  std::string token;
  while (readWord(in, token)) {
    int32_t h = hash(token);
    int32_t wid = getId(token, h);
    if (wid < 0) {
      word_hashes.push_back(h);
      continue;
    }
    ntokens++;
    if (!discard(wid, uniform(rng))) {
      words.push_back(wid);
      word_hashes.push_back(hash(token));
    }
    if (token == EOS) break;
    if (ntokens > MAX_LINE_SIZE ) break;
  }
  return ntokens;
}


int32_t Dictionary::getLine(std::istream& in,
                            std::vector<int32_t>& words,
                            std::vector<int32_t>& labels,
                            std::minstd_rand& rng) const {
  std::vector<int32_t> word_hashes;
  int32_t ntokens = getLine(in, words, word_hashes, labels, rng);
  return ntokens;
}

int32_t Dictionary::getId(const std::string& w, uint32_t h) const {
  int32_t id = find(w, h);
  return word2int_[id];
}

int32_t Dictionary::getId(const std::string& w) const {
  int32_t h = find(w);
  return word2int_[h];
}

bool Dictionary::discard(int32_t id, float rng) const {
  assert(id >= 0);
  assert(id < nwords_);
  return rng > pdiscard_[id];
}

void Dictionary::initTableDiscard() {
  pdiscard_.resize(size_);
  for (size_t i=0; i<size_; ++i) {
    real f = real(words_[i].count)/ real(ntokens_);
    pdiscard_[i] = std::sqrt(args_->t / f) + args_->t / f;
  }
}

}