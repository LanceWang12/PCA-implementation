from sklearn import datasets
from sklearn import decomposition
from sklearn.model_selection import train_test_split
from sklearn.datasets import make_multilabel_classification
import matplotlib.pyplot as plt
import numpy as np
import time
import PCA


def plot(x, y, label, imgname):
    plt.figure(figsize=(8, 6), dpi=150)
    plt.scatter(x[:, 0], x[:, 1], c=y, cmap=plt.cm.Set1,
                edgecolor='k', label=label)
    plt.xlabel("feature_01")
    plt.ylabel("feature_02")
    plt.legend()
    plt.grid(True)
    plt.savefig(f"./plot_img/{imgname}")


def test_PCA():
    # ---- load dataset ----
    X, Y = make_multilabel_classification(
        n_samples=40000, n_features=128, n_classes=3)
    # X = np.array(data.data)
    # Y = np.array(data.target)
    x_train, x_test, y_train, y_test = train_test_split(
        X, Y, test_size=0.5, random_state=0)
    print(f'Before PCA: x.shape = {x_test.shape}')

    # ---- dimension reduction ----
    n_components = 2
    pca = PCA.PCA(n_components, 'Jacobi', int(1e8), 1e-8, 8, 0)
    start = time.time()
    pca.fit(x_train)
    end = time.time()
    x_test_mypca = pca.transform(x_test)
    assert(x_test_mypca.shape[1] == n_components)
    assert(type(x_test_mypca) == type(np.ones(1)))
    print(f'After PCA: x.shape = {x_test_mypca.shape}')
    print(f'My PCA.fit() take {end - start:.5f} sec.')

    # ---- plot image ----
    label = [f"class{i}" for i in range(3)]
    plot(x_test_mypca, y_test, label, "my_pca.png")

    sklearn_pca = decomposition.PCA(n_components=n_components)
    start = time.time()
    sklearn_pca.fit(x_train)
    end = time.time()
    print(f'Sklearn PCA.fit() take {end - start:.5f} sec.')
    x_test_sklearn = sklearn_pca.transform(x_test)
    plot(x_test_sklearn, y_test, label, "sklearn_pca.png")


if __name__ == '__main__':
    test_PCA()
