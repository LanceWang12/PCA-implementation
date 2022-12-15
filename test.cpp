#include "src/PCA.h"
#include "src/matrix.h"
#include <Eigen/Dense>
#include <iostream>
using namespace std;
// using namespace Eigen;

// -------- test Jacobi SVD & PCA --------
int main() {
  vector<vector<double>> vec{{0.1, 0.2, 0.3, 0.4},
                             {0.13, 0.26, 0.39, 0.46},
                             {0.27, 0.543, 0.679, 0.892},
                             {0.54, 1.23, 1.76, 2.18},
                             {0.18, 0.34, 2.17, 3.91}};

  Matrix<double> a(vec);
  Matrix<double> b = a.transpose(); //((size_t)6, (size_t)3, 2);
  Matrix<double> c(a.rows(), a.rows());
  c = a * b;

  cout << "a:\n" << a << endl;
  cout << "a.transpose() :\n" << b << endl;
  cout << "a * a^T =\n" << c << endl;

  // get eigenval & eigenvector
  Matrix<double> eigenVect;
  vector<double> eigenVal;
  get_eigenval_by_Jacobi(c, 1e-5, 1e8, eigenVect, eigenVal, 0);

  cout << "\nEigen value: ";
  for (size_t i = 0; i < a.rows(); i++) {
    cout << eigenVal[i] << ' ';
  }
  cout << "\n\n";

  // test PCA
  PCA alg(2, "Jakobi", 1e8, 1e-8, 1);
  alg.fit(a);
  auto result = alg.transform(a);
  cout << result << endl;

  return 0;
}
