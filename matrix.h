/*！
 * Copyright (c) 2017 by ChengEn Shen
 * \file matrix.h
 * \brief 
 * \author ChenEn Shen
 */
#ifndef HUOGUONLP_MATRIX_H
#define HUOGUONLP_MATRIX_H

#include <cstdint>
#include <istream>
#include <ostream>
#include "real.h"
#include "vector.h"

namespace huoguo {
class Matrix {
 public:
  real* data_; //以数组形式存储矩阵的所有元素
  int64_t m_; //行数
  int64_t n_; //列数

  Matrix();
  Matrix(int64_t, int64_t);
  Matrix(const Matrix&);
  Matrix& operator=(const Matrix&);
  ~Matrix();

  inline const real& at(int64_t i, int64_t j) const {return data_[i * n_ + j];};
  inline real& at(int64_t i, int64_t j) {return data_[i * n_ + j];};


  void zero();
  void uniform(real);
  real dotRow(const Vector&, int64_t) const;
  void addRow(const Vector&, int64_t, real);

  void multiplyRow(const Vector& nums, int64_t ib = 0, int64_t ie = -1);
  void divideRow(const Vector& denoms, int64_t ib = 0, int64_t ie = -1);

  real l2NormRow(int64_t i) const;
  void l2NormRow(Vector& norms) const;

  void save(std::ostream&);
  void load(std::istream&);
};
}

#endif //HUOGUONLP_MATRIX_H
