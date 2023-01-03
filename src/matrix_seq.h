#pragma once
#include <assert.h>
#include <iomanip>
#include <iostream>
#include <mkl.h>
#include <omp.h>
#include <utility>
#include <vector>
using namespace std;

namespace seq {
template <typename T> class Matrix {
private:
  size_t m_rows, m_cols;
  vector<T> m_matrix;

public:
  pair<size_t, size_t> shape;

  // -------- Five of rules --------
  Matrix() : m_rows(0), m_cols(0) { this->shape = make_pair(m_rows, m_cols); }

  Matrix(const vector<vector<T>> vec)
      : m_rows(vec.size()), m_cols(vec[0].size()),
        shape(make_pair(m_rows, m_cols)) {
    m_matrix.resize(m_rows * m_cols, 0);
    for (size_t i = 0; i < m_rows; i++)
      for (size_t j = 0; j < m_cols; j++)
        m_matrix[i * m_cols + j] = vec[i][j];
  }

  Matrix(size_t rows, size_t cols) : m_rows(rows), m_cols(cols) {
    this->m_matrix.resize(this->m_rows * this->m_cols, 0);
    this->shape = make_pair(m_rows, m_cols);
  }

  Matrix(size_t rows, size_t cols, T val) : m_rows(rows), m_cols(cols) {
    this->m_matrix.resize(this->m_rows * this->m_cols, val);
    this->shape = make_pair(m_rows, m_cols);
  }

  Matrix(Matrix const &rhs)
      : m_rows(rhs.m_rows), m_cols(rhs.m_cols), m_matrix(rhs.m_matrix) {
    this->shape = make_pair(m_rows, m_cols);
  }

  Matrix(Matrix &&rhs)
      : m_rows(rhs.m_rows), m_cols(rhs.m_cols), m_matrix(move(rhs.m_matrix)) {
    this->shape = make_pair(m_rows, m_cols);
  }

  ~Matrix() {
    this->m_matrix.clear();
    this->m_matrix.shrink_to_fit();
  }

  // -------- Operator overloading --------
  Matrix &operator=(Matrix const &rhs) {
    if (this != &rhs) {
      this->m_rows = rhs.m_rows;
      this->m_cols = rhs.m_cols;
      this->m_matrix = rhs.m_matrix;
      this->shape = make_pair(m_rows, m_cols);
    }

    return *this;
  }

  Matrix &operator=(Matrix &&rhs) {
    if (this != &rhs) {
      this->m_rows = rhs.m_rows;
      this->m_cols = rhs.m_cols;
      this->m_matrix = move(rhs.m_matrix);
      this->shape = make_pair(m_rows, m_cols);
    }

    return *this;
  }

  T const &operator()(size_t i, size_t j) const {
    return this->m_matrix[i * this->m_cols + j];
  }

  T &operator()(size_t i, size_t j) {
    return this->m_matrix[i * this->m_cols + j];
  }

  constexpr bool operator==(const Matrix &rhs) const {
    return (this->m_matrix == rhs.m_matrix);
  }

  template <typename J>
  friend ostream &operator<<(ostream &os, const Matrix<J> &matrix) {
    for (size_t row = 0; row < matrix.m_rows; row++) {
      for (size_t col = 0; col < matrix.m_cols; col++)
        os << std::setprecision(4) << std::fixed << matrix(row, col) << " ";

      os << "\n";
    }

    os << "\n";

    return os;
  }

  Matrix<T> operator*(const Matrix<T> &m2) {
    assert(this->m_cols == m2.m_rows);

    Matrix<T> m3(this->m_rows, m2.m_cols);

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, this->m_rows,
                m2.m_cols, this->m_cols, 1, this->data(), this->m_cols,
                m2.data(), m2.m_cols, 0, m3.data(), m3.m_cols);

    return m3;
  }

  // -------- Other functions --------
  constexpr size_t rows() const { return this->m_rows; }

  constexpr size_t cols() const { return this->m_cols; }

  const T *data() const { return &(this->m_matrix[0]); }

  T *data() { return &(this->m_matrix[0]); }

  void resize(size_t i, size_t j) {
    m_rows = i;
    m_cols = j;
    shape = make_pair(i, j);
    this->m_matrix.resize(m_rows * m_cols, 0);
  }

  // ------- Linear algebra --------
  Matrix<T> transpose() {
    Matrix<T> m_t(this->m_cols, this->m_rows);
    for (size_t row = 0; row < this->m_cols; row++) {
      for (size_t col = 0; col < this->m_rows; col++)
        m_t(row, col) = this->operator()(col, row);
    }
    return m_t;
  }
};

} // namespace seq