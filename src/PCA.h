#pragma once
#include "matrix.h"
#include <assert.h>
#include <cmath>
#include <cstdio>
#include <float.h>
#include <map>
// #include <Eigen/Dense>

template <typename T>
bool get_eigenval_by_Jacobi(Matrix<T> a, double precision, int n_iter,
                            Matrix<T> &eigenVect, vector<T> &eigenValue,
                            unsigned int verbose = 0);

class PCA {
  // protected:
private:
  int n_components;
  string svd_solver;
  int n_iter;
  double precision;
  unsigned int verbose;

  // store the principal component: v_reduct(n * n_components)
  Matrix<double> v_reduct;

public:
  // -------- five of rules --------
  PCA()
      : n_components(2), svd_solver("Jakobi"), n_iter(1e8), precision(1e-8),
        verbose(0) {}
  PCA(int N_Components, string SVD_Solver, int N_Iter, double Precision,
      unsigned int Verbose)
      : n_components(N_Components), svd_solver(SVD_Solver), n_iter(N_Iter),
        precision(Precision), verbose(Verbose) {}

  // -------- functions --------
  PCA fit(Matrix<double> x);
  Matrix<double> transform(Matrix<double> x);
};

PCA PCA::fit(Matrix<double> x) {
  assert(n_components < (int)x.cols());
  // get the eigen value and vactor about x^T * x (feature_dim x feature_dim)
  Matrix<double> x_t = x.transpose();
  Matrix<double> a = x_t * x;
  Matrix<double> eigenVect;
  vector<double> eigenVal;
  get_eigenval_by_Jacobi(a, this->precision, this->n_iter, eigenVect, eigenVal,
                         this->verbose);
  size_t n = eigenVect.rows(), k = (size_t)n_components;
  this->v_reduct.resize(n, k);
  for (size_t i = 0; i < n; i++)
    for (size_t j = 0; j < k; j++)
      this->v_reduct(i, j) = eigenVect(i, j);
  return *this;
}

Matrix<double> PCA::transform(Matrix<double> x) {
  assert(this->v_reduct.rows() == x.cols());
  Matrix<double> result(x.rows(), this->n_components);
  result = x * this->v_reduct;
  return result;
}

template <typename T>
bool get_eigenval_by_Jacobi(Matrix<T> a, double precision, int n_iter,
                            Matrix<T> &eigenVect, vector<T> &eigenValue,
                            unsigned int verbose) {
  assert(a.rows() == a.cols());
  size_t dim = a.rows();
  eigenVect.resize(dim, dim);
  eigenValue.resize(dim);

  // produce I matrix
  for (size_t i = 0; i < dim; i++)
    eigenVect(i, i) = 1.;

  // cout << "error msg 1" << endl; // error msg
  int nCount = 0;
  // the max element which is not in diagonal
  double maxElement;
  // (row, col ) of the maxElement
  size_t nRow, nCol;
  double tmp;
  double dbApp, dbApq, dbAqq, dbAngle, dbSin, dbCos, dbSin2, dbCos2;
  while (1) {
    // find the max element which is not in diagonal
    maxElement = a(0, 1);
    if (verbose > 0) {
      printf("Iteration_%03d: error(%.10f)\n", nCount + 1, fabs(maxElement));
    }
    nRow = 0;
    nCol = 1;
    for (size_t i = 0; i < dim; i++)
      for (size_t j = 0; j < dim; j++) {
        tmp = fabs(a(i, j));
        if (i != j && tmp > maxElement) {
          maxElement = tmp;
          nRow = i;
          nCol = j;
        }
      }
    // cout << "error msg 2" << endl; // error msg
    if (maxElement < precision)
      break;
    if (nCount > n_iter)
      break;

    nCount++;

    dbApp = a(nRow, nRow);
    dbApq = a(nRow, nCol);
    dbAqq = a(nCol, nCol);

    // compute theta(angle)
    dbAngle = 0.5 * atan2(-2 * dbApq, dbAqq - dbApp);
    dbSin = sin(dbAngle);
    dbCos = cos(dbAngle);
    dbSin2 = sin(2 * dbAngle);
    dbCos2 = cos(2 * dbAngle);

    a(nRow, nRow) = dbApp * dbCos * dbCos + dbAqq * dbSin * dbSin +
                    2 * dbApq * dbCos * dbSin;
    a(nCol, nCol) = dbApp * dbSin * dbSin + dbAqq * dbCos * dbCos -
                    2 * dbApq * dbCos * dbSin;
    a(nRow, nCol) = 0.5 * (dbAqq - dbApp) * dbSin2 + dbApq * dbCos2;
    a(nCol, nRow) = a(nRow, nCol);
    // cout << "error msg 3" << endl; // error msg
    for (size_t i = 0; i < dim; i++)
      if (i != nCol && i != nRow) {
        maxElement = a(i, nRow);
        a(i, nRow) = a(i, nCol) * dbSin + maxElement * dbCos;
        a(i, nCol) = a(i, nCol) * dbCos - maxElement * dbSin;
      }

    for (size_t j = 0; j < dim; j++)
      if (j != nCol && j != nRow) {
        maxElement = a(nRow, j);
        a(nRow, j) = a(nCol, j) * dbSin + maxElement * dbCos;
        a(nCol, j) = a(nCol, j) * dbCos - maxElement * dbSin;
      }

    // compute eigen vector
    for (size_t i = 0; i < dim; i++) {
      maxElement = eigenVect(i, nRow);
      eigenVect(i, nRow) = eigenVect(i, nCol) * dbSin + maxElement * dbCos;
      eigenVect(i, nCol) = eigenVect(i, nCol) * dbCos - maxElement * dbSin;
    }
  }

  // reorder eigen vector by eigen value
  map<double, int> mapEigen;
  for (size_t i = 0; i < dim; i++) {
    eigenValue[i] = a(i, i);
    mapEigen[eigenValue[i]] = i;
  }

  Matrix<T> tmpVec(dim, dim);
  auto iter = mapEigen.rbegin();
  for (size_t j = 0; iter != mapEigen.rend(), j < dim; j++, iter++) {
    for (size_t i = 0; i < dim; i++)
      tmpVec(i, j) = eigenVect(i, iter->second);
    eigenValue[j] = iter->first;
  }

  // set positive or negative
  double sumVec = 0;
  for (size_t i = 0; i < dim; i++) {
    sumVec = 0;
    for (size_t j = 0; j < dim; j++)
      sumVec += tmpVec(j, i);
    if (sumVec < 0)
      for (size_t j = 0; j < dim; j++)
        tmpVec(j, i) *= (-1);
  }

  eigenVect = move(tmpVec);
  if (verbose > 0) {
    cout << "Jacobi success!!!" << endl;
  }

  return true;
}

template <typename T> bool pca(const Matrix<T> &X, Matrix<T> &X_reduced) {

  return true;
}