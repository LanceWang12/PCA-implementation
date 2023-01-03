#pragma once
#include "PCA.h"
#include "matrix.h"
#include <pybind11/numpy.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace py::literals;

// convert my Matrix class to numpy.ndarray(2d)
template <typename T> py::array_t<T> _Matrix2ndarray(const Matrix<T> &m);

// convert numpy.ndarray(2d) to my Matrix class
template <typename T> Matrix<T> _ndarray2Matrix(py::array_t<T> &arr);

// the wrapper of my PCA class
class np_PCA : public PCA {
public:
  // five of rules
  np_PCA(int n_components, string svd_solver, int n_iter, double precision,
         int n_jobs, int verbose)
      : PCA(n_components, svd_solver, n_iter, precision, n_jobs, verbose) {}

  // wrapper
  np_PCA fit(py::array_t<double> x);
  py::array_t<double> transform(py::array_t<double> x);
};

// ------------------------- Implementation ---------------------------
template <typename T> py::array_t<T> _Matrix2ndarray(const Matrix<T> &m) {
  size_t row = m.rows(), col = m.cols();
  auto result = py::array_t<T>(row * col);
  py::buffer_info buf = result.request();

  T *ptr = (T *)buf.ptr;

  for (size_t i = 0; i < row; i++)
    for (size_t j = 0; j < col; j++) {
      ptr[i * col + j] = m(i, j);
    }

  // convert 1d to 2d
  result.resize({row, col});
  return result;
}

template <typename T> Matrix<T> _ndarray2Matrix(py::array_t<T> &arr) {
  py::buffer_info buf = arr.request();
  if (buf.ndim != 2)
    throw std::runtime_error("numpy.ndarray dims must be 2!\n");

  auto row = buf.shape[0], col = buf.shape[1];
  T *ptr = (T *)buf.ptr;
  Matrix<T> result(row, col);

  for (int i = 0; i < row; i++)
    for (int j = 0; j < col; j++) {
      result(i, j) = ptr[i * col + j];
    }

  return result;
}

np_PCA np_PCA::fit(py::array_t<double> x) {
  Matrix<double> x_Matrix = _ndarray2Matrix(x);
  PCA::fit(x_Matrix);
  return *this;
}
py::array_t<double> np_PCA::transform(py::array_t<double> x) {
  Matrix<double> x_Matrix = _ndarray2Matrix(x);
  Matrix<double> result_Matrix = PCA::transform(x_Matrix);
  auto result_np = _Matrix2ndarray(result_Matrix);

  return result_np;
}
