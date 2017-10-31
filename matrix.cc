/*！
 * Copyright (c) 2017 by ChengEn Shen
 * \file matrix.cc
 * \brief 
 * \author ChenEn Shen
 */
#include "matrix.h"

#include <assert.h>

#include <random>

#include "utils.h"
#include "vector.h"
namespace huoguo {
Matrix::Matrix() {
  m_ = 0;
  n_ = 0;
  data_ = nullptr;
}

Matrix::Matrix(int64_t m, int64_t n) {
  m_ = m;
  n_ = n;
  data_ = new real[m * n];
}

Matrix::Matrix(const Matrix& other) {
  m_ = other.m_;
  n_ = other.n_;
  data_ = new real[m_ * n_];
  for (int64_t i = 0; i < (m_ * n_); i++) {
    data_[i] = other.data_[i];
  }
}

Matrix& Matrix::operator=(const Matrix& other) {
  Matrix temp(other);
  m_ = temp.m_;
  n_ = temp.n_;
  std::swap(data_, temp.data_);
  return *this;
}

Matrix::~Matrix() {
  delete[] data_;
}

void Matrix::zero() {
  for (int64_t i = 0; i < (m_ * n_); i++) {
    data_[i] = 0.0;
  }
}

void Matrix::uniform(real a) {
  std::minstd_rand rng(1);
  std::uniform_real_distribution<> uniform(-a, a);
  for (int64_t i = 0; i < (m_ * n_); i++) {
    data_[i] = uniform(rng);
  }
}

real Matrix::dotRow(const Vector& vec, int64_t i) const {
  //对矩阵在第i行，和vec进行相成，返回一个数值
  //点乘
  assert(i >= 0);
  assert(i < m_);
  assert(vec.size() == n_);
  real d = 0.0;
  for (int64_t j = 0; j < n_; j++) {
    d += at(i, j) * vec.data_[j];
  }
  return d;
}

void Matrix::addRow(const Vector& vec, int64_t i, real a) {
  //对矩阵在第i行，和vec*a进行相加,操作对象是矩阵本身
  assert(i >= 0);
  assert(i < m_);
  assert(vec.size() == n_);
  for (int64_t j = 0; j < n_; j++) {
    data_[i * n_ + j] += a * vec.data_[j];
  }
}

void Matrix::multiplyRow(const Vector& nums, int64_t ib, int64_t ie) {
  //对矩阵从第ib-ie行进行操作，将nums中的第ib-ie中的元素，和矩阵中的相对行数进行相乘
  if (ie == -1) {ie = m_;}
  assert(ie <= nums.size());
  for (auto i = ib; i < ie; i++) {
    real n = nums[i-ib];
    if (n != 0) {
      for (auto j = 0; j < n_; j++) {
        at(i, j) *= n;
      }
    }
  }
}

void Matrix::divideRow(const Vector& denoms, int64_t ib, int64_t ie) {
  if (ie == -1) {ie = m_;}
  assert(ie <= denoms.size());
  for (auto i = ib; i < ie; i++) {
    real n = denoms[i-ib];
    if (n != 0) {
      for (auto j = 0; j < n_; j++) {
        at(i, j) /= n;
      }
    }
  }
}

real Matrix::l2NormRow(int64_t i) const {
  //对第i行进行正则化项
  auto norm = 0.0;
  for (auto j = 0; j < n_; j++) {
    const real v = at(i,j);
    norm += v * v;
  }
  return std::sqrt(norm);
}

void Matrix::l2NormRow(Vector& norms) const {
  //对每一行进行正则化项，保存在norms中，norms中每一列对应矩阵的每一行的正则化值
  assert(norms.size() == m_);
  for (auto i = 0; i < m_; i++) {
    norms[i] = l2NormRow(i);
  }
}

void Matrix::save(std::ostream& out) {
  out.write((char*) &m_, sizeof(int64_t));
  out.write((char*) &n_, sizeof(int64_t));
  out.write((char*) data_, m_ * n_ * sizeof(real));
}

void Matrix::load(std::istream& in) {
  in.read((char*) &m_, sizeof(int64_t));
  in.read((char*) &n_, sizeof(int64_t));
  delete[] data_;
  data_ = new real[m_ * n_];
  in.read((char*) data_, m_ * n_ * sizeof(real));
}
}