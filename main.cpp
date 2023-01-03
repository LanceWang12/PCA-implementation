#include "src/wrapper.h"
#include <cassert>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <sstream>
#include <utility>
namespace py = pybind11;

PYBIND11_MODULE(PCA, m) {
  pybind11::class_<np_PCA>(m, "PCA")
      .def(py::init<int, string, int, double, int, int>())
      .def("fit", &np_PCA::fit)
      .def("transform", &np_PCA::transform);
}