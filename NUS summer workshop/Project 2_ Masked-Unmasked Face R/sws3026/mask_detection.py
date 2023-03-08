import os
import cv2 as cv
from skimage.feature import hog
from sklearn.preprocessing import normalize, StandardScaler
from sklearn.svm import *
from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix

nomask_path = "./dataset_nomask"
mask_path = "./dataset_mask"

# read the original data with and without mask & create labels
print("reading faces without masks")
X_nomask = []
for subject_name in os.listdir(nomask_path):
    subject_images_dir = os.path.join(nomask_path, subject_name)

    for img_name in os.listdir(subject_images_dir):
        img_path = os.path.join(subject_images_dir, img_name)
        img = cv.imread(img_path, cv.IMREAD_GRAYSCALE)
        X_nomask.append(img)

print("reading faces with masks")
X_mask = []
for subject_name in os.listdir(mask_path):
    subject_images_dir = os.path.join(mask_path, subject_name)

    for img_name in os.listdir(subject_images_dir):
        img_path = os.path.join(subject_images_dir, img_name)
        img = cv.imread(img_path, cv.IMREAD_GRAYSCALE)
        X_mask.append(img)

y = [0] * 750 + [1] * 750

# features extractiona & train-test split & preprocessing
print("extracting features")
X_feature = []
for x in (X_nomask + X_mask):
    x = hog(x, orientations=8, pixels_per_cell=(10, 10),
            cells_per_block=(1, 1), visualize=False, multichannel=False)
    X_feature.append(x)

X_feature = normalize(X_feature)
X_train, X_test, y_train, y_test = train_test_split(X_feature, y, 
    test_size=0.2, random_state=42)

# model training and evaluation
print("model fitting")
clf = SVC(C=1, kernel="linear")
clf.fit(X_train, y_train)

cm=confusion_matrix(clf.predict(X_test),y_test)
print(clf.score(X_test, y_test))