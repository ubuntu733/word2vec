/*！
 * Copyright (c) 2017 by ChengEn Shen
 * \file vector.h
 * \brief 
 * \author ChenEn Shen
 */
#ifndef HUOGUONLP_VECTOR_H
#define HUOGUONLP_VECTOR_H

#include <cstdint>
#include <ostream>

#include "real.h"

namespace huoguo {

class Matrix;
class QMatrix;

class Vector {

 public:
  int64_t m_;
  real* data_;

  explicit Vector(int64_t);
  ~Vector();

  real& operator[](int64_t);
  const real& operator[](int64_t) const;

  int64_t size() const;
  void zero();
  void mul(real);
  real norm() const;
  void addVector(const Vector& source);
  void addVector(const Vector&, real);
  void addRow(const Matrix&, int64_t);
  void addRow(const QMatrix&, int64_t);
  void addRow(const Matrix&, int64_t, real);
  void mul(const QMatrix&, const Vector&);
  void mul(const Matrix&, const Vector&);
  int64_t argmax();
};

std::ostream& operator<<(std::ostream&, const Vector&);

}


#endif //HUOGUONLP_VECTOR_H
