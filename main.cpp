#include "src/Matrix.h"
#include "src/PCA.h"
#include <cassert>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <sstream>
#include <utility>

PYBIND11_MODULE(_PCA, m) {
  pybind11::class_<PCA>(m, "PCA")
      .def(pybind11::init<int, string, int, double, unsigned int>())
      // .def("__getitem__",
      //      [](Matrix<double> &m, pair<size_t, size_t> index) {
      //        return m(index.first, index.second);
      //      })
      // .def("__setitem__",
      //      [](Matrix<double> &m, pair<size_t, size_t> index, double value) {
      //        m(index.first, index.second) = value;
      //      })
      // .def("__str__",
      //      [](Matrix<double> &m) {
      //        stringstream ss;
      //        ss << m;

      //        return ss.str();
      //      })
      .def("fit",
           [](Matrix<double> &m) {
             stringstream ss;
             ss << m;

             return ss.str();
           })
      .def(pybind11::self == pybind11::self)
      .def_property_readonly("nrow", &Matrix<double>::rows)
      .def_property_readonly("ncol", &Matrix<double>::cols);

  m.def("multiply_naive", multiply_naive<double>,
        "Matrix multiplication with naive method.");
  m.def("multiply_tile", multiply_tile<double>,
        "Matrix multiplication with tile method.");
  m.def("multiply_mkl", multiply_mkl<double>,
        "Matrix multiplication with mkl method.");
  m.def("bytes", bytes, "MyAllocator bytes.");
  m.def("allocated", allocated, "MyAllocator allocated.");
  m.def("deallocated", deallocated, "MyAllocator deallocated.");
}