# PCA implementation

This is a homework project for [Numerical Software Development(NYCU 2022 autumn)](https://yyc.solvcon.net/en/latest/nsd/schedule/22au_nycu/schedule.html)

## Basic Information

PCA (Principal Component Analysis) is an dimension reduction algorithm.  It can be used in visualizing high dimension  data and feature reduction.  **By the way, I will compare my implementation with sklearn.**

- github repository: 



## Problem to solve

In the Big Data analysis, data is not only becoming bigger and bigger; but also becoming more and more complicated.  It causes a spectacular increase of the dimensionality of the data.  For example, the dimensionality of a set of images is the number of pixels in any image, which ranges from thousands to millions.

Computer has no problem processing that many dimensions.  However, we are limited to three dimensions.  Computer still needs us, so we should find a way to effectively visualize high-dimensional data before handing it over to the computer.

There are many methods to solve this problem.  For example, PCA, LDA, Kernel method, Random projection and so on.  Today, I will implement PCA to solve the problem about dimension reduction.

![](https://i.imgur.com/cBQO9oV.gif)

(using MoviePy to produce animation)



## System Architecture

![](https://i.imgur.com/XOAw7qx.png)



## API Description

```c++
// Initialize hyperparameter for t-SNE object
int x[10][3] = [
  [1, 2, 3],
  [4, 5, 6],
  ...
]
PCA(
  int n_components=2, 
  int n_jobs=NULL,
)
  
X_embed = PCA().fit_transform(X)
// X_embed = [
//  [0.1, 0.2],
//  [0.3, 0.4],
//  ...
// ]
```


## Engineering Infrastructure

- Automatic build system: `cmake`
- Version control: `git`
- Testing framework: `pytest`
- Documentation: GitHub `README.md`

## Schedule

Planning phase : 

Week 1 (10/31): Implement the t-SNE algorithm with C++

Week 2 (11/7): Make python wrappers for C++ with `pybind`

Week 3 (11/14): Finish C++, and start visualizing some data in python

Week 4 (11/21): Implement features of visualization

Week 5 (11/28): Test all features with `pytest`

Week 6 (12/5): Finish up, debug, and write the documentation

Week 7 (12/12): Buffer time for further testing and debugging

Week 8 (12/19): Make slides and prepare for the presentation



## References

https://scikit-learn.org/stable/modules/generated/sklearn.decomposition.PCA.html

https://github.com/pybind/pybind11
