// ---------------- test PCA -----------------
#include "src/PCA.h"
#include "src/PCA_seq.h"
#include "src/matrix.h"
#include "src/matrix_seq.h"
#include "src/utils.h"
#include <Eigen/Dense>
#include <cstdio>
#include <iostream>
#include <time.h>
using namespace std;
// using namespace Eigen;

// -------- test Jacobi SVD & PCA --------
int main() {
  time_t start, end;
  double diff_seq, diff_paral;

  // -------- Hyperparameter --------
  int train_num = 20000, test_num = 10000, features_num = 128, random_seed = 0;
  int n_components = 2, n_jobs = 4, verbose = 0;
  string svd_solver = "Jakobi";
  double n_iter = 1e6, precision = 1e-5;

  // -------- Produce training and testing data--------
  Matrix<double> x_train =
      generate_test_matrix(train_num, features_num, random_seed);
  Matrix<double> x_test =
      generate_test_matrix(test_num, features_num, random_seed);

  seq::Matrix<double> x_train_seq(train_num, features_num);
  seq::Matrix<double> x_test_seq(test_num, features_num);
  for (int i = 0; i < train_num; i++)
    for (int j = 0; j < features_num; j++)
      x_train_seq(i, j) = x_train(i, j);

  for (int i = 0; i < test_num; i++)
    for (int j = 0; j < features_num; j++)
      x_test_seq(i, j) = x_test(i, j);

  // sequential pca.fit
  seq::PCA pca_seq(n_components, svd_solver, n_iter, precision, n_jobs,
                   verbose);
  start = clock();
  pca_seq.fit(x_train_seq);
  end = clock();
  diff_seq = (double)(end - start) / CLOCKS_PER_SEC;

  // parallel pca.fit
  PCA pca_parallel(n_components, svd_solver, n_iter, precision, n_jobs,
                   verbose);
  start = clock();
  pca_parallel.fit(x_train);
  end = clock();
  diff_paral = (double)(end - start) / CLOCKS_PER_SEC;

  // print time for parallel and sequential version
  cout << "---- fit() comparison ----" << endl;
  cout << "parallel: " << diff_paral << " sec." << endl;
  cout << "seq:      " << diff_seq << " sec.\n" << endl;

  // parallel pca.transform
  start = clock();
  Matrix<double> result_paral = pca_parallel.transform(x_test);
  end = clock();
  diff_paral = (double)(end - start) / CLOCKS_PER_SEC;

  // sequential pca.transform
  start = clock();
  seq::Matrix<double> result_seq = pca_seq.transform(x_test_seq);
  end = clock();
  diff_seq = (double)(end - start) / CLOCKS_PER_SEC;

  // print time for parallel and sequential version
  cout << "---- transform() comparison ----" << endl;
  cout << "parallel: " << diff_paral << " sec." << endl;
  cout << "seq:      " << diff_seq << " sec.\n" << endl;

  // test the output between seq an parallel
  cout << "---- test result ----" << endl;
  shape_info(x_test, "x_test_before_parallel");
  shape_info(x_test_seq, "x_test_before_seq");
  shape_info(result_paral, "x_test_after_parallel");
  seq::shape_info(result_seq, "x_test_after_seq");

  for (int i = 0; i < (int)x_test.rows(); i++) {
    for (int j = 0; j < n_components; j++)
      if (abs(result_paral(i, j) - result_seq(i, j)) > 1e-2) {
        printf("result_paral(%d, %d) != result_seq(%d, %d)\n", i, j, i, j);
        printf("result_paral(%d, %d) = %f\n", i, j, result_paral(i, j));
        printf("result_seq(%d, %d) = %f\n", i, j, result_seq(i, j));
        return 0;
      }
  }
  cout << "\n-------------- pass all test --------------" << endl;
  return 0;
}

// vector<vector<double>> vec{{0.1, 0.2, 0.3, 0.4},
//                            {0.13, 0.26, 0.39, 0.46},
//                            {0.27, 0.543, 0.679, 0.892},
//                            {0.54, 1.23, 1.76, 2.18},
//                            {0.18, 0.34, 2.17, 3.91}};

// get eigenval & eigenvector
//   Matrix<double> eigenVect;
//   vector<double> eigenVal;
// start = clock();
// get_eigenval_by_Jacobi(x_x_t, 1e-5, 1e6, eigenVect, eigenVal, 0);
// end = clock();
// cout << "get_eigenval_by_Jacobi() take "
//      << (double)(end - start) / CLOCKS_PER_SEC << " sec." << endl;

// cout << "\nEigen value: ";
// for (size_t i = 0; i < a.rows(); i++) {
//   cout << eigenVal[i] << ' ';
// }
// cout << "\n\n";