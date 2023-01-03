#pragma once
#include "matrix.h"
#include "matrix_seq.h"
#include <stdlib.h>
#include <time.h>

// ----------- Definition ------------
Matrix<double> generate_test_matrix(int row = 20000, int col = 256,
                                    int seed = 0);
template <typename T> void shape_info(Matrix<T> mat, string mat_name);

namespace seq {
seq::Matrix<double> generate_test_matrix(int row = 20000, int col = 256,
                                         int seed = 0);
template <typename T> void shape_info(seq::Matrix<T> mat, string mat_name);
} // namespace seq

// ----------- Implementation --------------
template <typename T> void shape_info(Matrix<T> mat, string mat_name) {
  cout << mat_name << ".shape = (" << mat.rows() << ", " << mat.cols() << ")"
       << endl;
}

Matrix<double> generate_test_matrix(int row, int col, int seed) {
  srand(seed);
  Matrix<double> mat(row, col);
  double max = 0, min = 2;

  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++)
      mat(i, j) = (max - min) * rand() / (RAND_MAX + 1.0) + min;

  return mat;
}

template <typename T>
void seq::shape_info(seq::Matrix<T> mat, string mat_name) {
  cout << mat_name << ".shape = (" << mat.rows() << ", " << mat.cols() << ")"
       << endl;
}

seq::Matrix<double> seq::generate_test_matrix(int row, int col, int seed) {
  srand(seed);
  seq::Matrix<double> mat(row, col);
  double max = 0, min = 2;

  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++)
      mat(i, j) = (max - min) * rand() / (RAND_MAX + 1.0) + min;

  return mat;
}