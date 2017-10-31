/*！
 * Copyright (c) 2017 by ChengEn Shen
 * \file vector.cc
 * \brief 
 * \author ChenEn Shen
 */

#include "vector.h"

#include <assert.h>

#include <iomanip>
#include <cmath>

#include "matrix.h"

namespace huoguo {

Vector::Vector(int64_t m) {
  m_ = m;
  data_ = new real[m];
}

Vector::~Vector() {
  delete[] data_;
}

int64_t Vector::size() const {
  return m_;
}

void Vector::zero() {
  for (int64_t i = 0; i < m_; i++) {
    data_[i] = 0.0;
  }
}

real Vector::norm() const {
  real sum = 0;
  for (int64_t i = 0; i < m_; i++) {
    sum += data_[i] * data_[i];
  }
  return std::sqrt(sum);
}

void Vector::mul(real a) {
  //对vector中的每一个元素都乘以a
  for (int64_t i = 0; i < m_; i++) {
    data_[i] *= a;
  }
}

void Vector::addVector(const Vector& source) {
  //对vector中的每一个元素都加上a
  assert(m_ == source.m_);
  for (int64_t i = 0; i < m_; i++) {
    data_[i] += source.data_[i];
  }
}

void Vector::addVector(const Vector& source, real s) {
  //向量相加
  assert(m_ == source.m_);
  for (int64_t i = 0; i < m_; i++) {
    data_[i] += s * source.data_[i];
  }
}

void Vector::addRow(const Matrix& A, int64_t i) {
  //将矩阵的第i行和向量相加
  assert(i >= 0);
  assert(i < A.m_);
  assert(m_ == A.n_);
  for (int64_t j = 0; j < A.n_; j++) {
    data_[j] += A.at(i, j);
  }
}

void Vector::addRow(const Matrix& A, int64_t i, real a) {
  //将矩阵的第i行和向量相加，并乘以a
  assert(i >= 0);
  assert(i < A.m_);
  assert(m_ == A.n_);
  for (int64_t j = 0; j < A.n_; j++) {
    data_[j] += a * A.at(i, j);
  }
}


void Vector::mul(const Matrix& A, const Vector& vec) {

  assert(A.m_ == m_);
  assert(A.n_ == vec.m_);
  for (int64_t i = 0; i < m_; i++) {
    data_[i] = A.dotRow(vec, i);
  }
}


int64_t Vector::argmax() {
  real max = data_[0];
  int64_t argmax = 0;
  for (int64_t i = 1; i < m_; i++) {
    if (data_[i] > max) {
      max = data_[i];
      argmax = i;
    }
  }
  return argmax;
}

real& Vector::operator[](int64_t i) {
  return data_[i];
}

const real& Vector::operator[](int64_t i) const {
  return data_[i];
}

std::ostream& operator<<(std::ostream& os, const Vector& v)
{
  os << std::setprecision(5);
  for (int64_t j = 0; j < v.m_; j++) {
    os << v.data_[j] << ' ';
  }
  return os;
}

}
