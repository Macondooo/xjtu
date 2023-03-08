import numpy as np
from skimage.feature import hog
from matplotlib import pyplot as plt
from sklearn.preprocessing import normalize
from sklearn.svm import *
from sklearn.neighbors import KNeighborsClassifier as knc
from sklearn.metrics import confusion_matrix
from sklearn.decomposition import PCA
import cv2

def load_data(data_path):
    X = []
    y = []
    for i in range(50):
        for j in range(15):
            filepath = f"{data_path}/s{i}/{j}.jpg"
            img = plt.imread(filepath)
            X.append(img)
            y.append(i)
    X, y = np.array(X), np.array(y)
    return X,y

def extract_feature(X):
    X_feature = []
    for x in X:
        x = hog(x, orientations=8, pixels_per_cell=(10, 10),
                cells_per_block=(1, 1), visualize=False, multichannel=False)
        X_feature.append(x)
    X_feature = normalize(X_feature)
    return X_feature

X_train,y_train=load_data("./dataset_nomasked_eyes")
X_test, y_test=load_data("./dataset_masked_eyes")

dimensions, width, height =X_train.shape
X_train = X_train.reshape((dimensions, width * height))
dimensions, width, height = X_test.shape
X_test = X_test.reshape((dimensions, width * height))

pca=PCA()
pca.fit(X_train)

#pca.components_=pca.components_[2:-1]

X_train=pca.transform(X_train)
X_test=pca.transform(X_test)

# for i,x in enumerate(pca.components_):
#     x=x.reshape(width,height)
#     plt.imsave(f'./eigeneyes/{i}.jpg',x)

# model training and evaluation
print("model fitting")
clf = SVC(C=1,kernel="linear")
clf.fit(X_train, y_train)
cm=confusion_matrix(clf.predict(X_test),y_test)
print(clf.score(X_test, y_test))
#0.6