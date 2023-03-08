import sklearn.svm
import numpy as np


class Classifier:

    u: np.ndarray
    sigma: np.ndarray
    v: np.ndarray
    svm: sklearn.svm.SVC

    def __init__(self) -> None:
        self.svm = sklearn.svm.SVC()

    def fit(self, x, y, num_features) -> None:
        temp_u, temp_sigma, temp_v = np.linalg.svd(x.T, full_matrices=False)
        self.u = temp_u[:, :num_features]
        self.sigma = temp_sigma[:num_features]
        self.v = temp_v[:num_features, :]

        self.svm.fit(self.v.T, y)

    def predict(self, x: np.ndarray) -> np.ndarray:
        test_v = (self.u.T @ x.T) / \
            self.sigma.reshape((self.sigma.shape[0], 1))
        return self.svm.predict(test_v.T)