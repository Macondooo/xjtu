import sklearn.svm
import numpy as np
from matplotlib import pyplot as plt
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
from sklearn.metrics import confusion_matrix
data_path = "./dataset_nomasked_eyes"
num_features = 29


def pca(x):
    u, sigma, v = np.linalg.svd(x.T, full_matrices=False)
    return u[:, :num_features], sigma[:num_features], v[:num_features, :]


def compose(u, sigma, v, idx):
    return (u @ np.diag(sigma) @
            v[:, idx]).reshape((width, height, channels)).astype(np.uint8)

if __name__ == "__main__":
    # load dataset
    x = []
    y = []
    for i in range(50):
        for j in range(15):
            filepath = f"{data_path}/s{i}/{j}.jpg"
            img = plt.imread(filepath)
            x.append(img)
            y.append(i)
    
    x, y = np.array(x), np.array(y)
    #dimensions, width, height, channels = x.shape
    # x = x.reshape((dimensions, width * height * channels)) #rgb or rgba pictures
    dimensions, width, height = x.shape
    x = x.reshape((dimensions, width * height))

    # do SVD decomposition of image idx
    idx = 2
    u, sigma, v = pca(x)

    # # plot the power plot
    # plt.subplot(121), plt.plot(range(num_features), np.log(
    #     sigma)), plt.title("Log of singular values")
    # plt.subplot(122), plt.plot(range(num_features), [np.log(1 - np.sum(sigma[:i] ** 2) / np.sum(
    #     sigma ** 2)) for i in range(num_features)]), plt.title("Log error of eigenfaces")
    # plt.show()

    # Saving eigenfaces
    i = 0
    for face in u.T:
        ef = (face - np.min(face)) / (np.max(face) - np.min(face))  # normalize
        plt.imsave(f"eigeneyes/{i}.jpg",
                   #ef.reshape((width, height, channels))) #rgb or rgba pictures
                   ef.reshape((width, height)))  #gray pictures
        i += 1

    # split train and test
    x_train, x_test, y_train, y_test = train_test_split(
        v.T, y, train_size=0.8, test_size=0.2)

    # fit model
    svm = sklearn.svm.SVC()

    svm.fit(x_train, y_train)
    y_pred = svm.predict(x_test)
    cm=confusion_matrix(svm.predict(x_test),y_test)
    print(accuracy_score(y_test, y_pred))