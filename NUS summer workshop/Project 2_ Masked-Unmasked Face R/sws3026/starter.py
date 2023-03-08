import os
import cv2 as cv
import dlib
import numpy as np
from skimage.feature import hog
import sklearn

# T1  start _______________________________________________________________________________
# Read in Dataset

# change the dataset path here according to your folder structure
dataset_path = "C:/Users/Macondo/Desktop/NUS visual computing/Project 2_ Masked-Unmasked Face R/Dataset_1/Dataset_1"

X = []
y = []
for subject_name in os.listdir(dataset_path):
    y.append(subject_name)
    subject_images_dir = os.path.join(dataset_path, subject_name)

    temp_x_list = []
    for img_name in os.listdir(subject_images_dir):
        # write code to read each 'img'
        img_path = os.path.join(subject_images_dir, img_name)
        img = dlib.load_rgb_image(img_path)
        # add the img to temp_x_list
        temp_x_list.append(img)
    # add the temp_x_list to X
    X.append(temp_x_list)
# T1 end ____________________________________________________________________________________

# T2 start __________________________________________________________________________________
# Preprocessing
X_processed = []
detector = dlib.get_frontal_face_detector()
predictor_path = "D:/Data/shape_predictor_68_face_landmarks.dat"
predictor = dlib.shape_predictor(predictor_path)
nomask_data_path = "./dataset_nomask"
for i, x_list in enumerate(X):
    temp_X_processed = []
    dir_path = os.path.join(nomask_data_path, f"s{i}")
    os.makedirs(dir_path)
    for j, x in enumerate(x_list):
        # write the code to detect face in the image (x) using dlib facedetection library
        detector = dlib.get_frontal_face_detector()
        dets = detector(x, 2)
        det = dets[0]
        x = dlib.as_grayscale(x)
        shape = predictor(x, det)
        # write the code to crop the image (x) to keep only the face, resize the cropped image to 150x150
        temp_points = shape.parts()
        points = np.ndarray(shape=(0, 2), dtype=np.uint8)
        for p in temp_points:
            points = np.vstack([points, [p.x, p.y]])
        x_axis, y_axis = points[:, 0], points[:, 1]
        left, right = np.min(x_axis), np.max(x_axis)
        top, bottom = np.min(y_axis), np.max(y_axis)
        crp_x = x[top-5:bottom+5, left-5:right+5]
        crp_x = dlib.resize_image(crp_x, 150, 150)
        cv.imwrite(os.path.join(dir_path, f"{j}.jpg"), crp_x)
        # append the converted image into temp_X_processed
        temp_X_processed.append(crp_x)

    # append temp_X_processed into  X_processed
    X_processed.append(temp_X_processed)

# T2 end ____________________________________________________________________________________


# T3 start __________________________________________________________________________________
# Create masked face dataset
X_masked = []
shp_det = dlib.rectangle(0, 0, 149, 149)
nomask_data_path = "./dataset_mask"

for i, x_list in enumerate(X_processed):
    temp_X_masked = []
    dir_path = os.path.join(nomask_data_path, f"s{i}")
    os.makedirs(dir_path)
    for j, x in enumerate(x_list):
        # write the code to detect face in the image (x) using dlib facedetection library
        local_shape = predictor(x, shp_det)
        key_points = local_shape.parts()
        temp_points = key_points[2:15]
        temp_points.extend([key_points[31], key_points[35], key_points[27]])
        mask_points = np.ndarray(shape=(0, 2), dtype=np.uint8)
        for p in temp_points:
            mask_points = np.vstack([mask_points, [p.x, p.y]])
        # write the code to add synthetic mask as shown in the project problem description
        cv.fillPoly(x, [mask_points[:-1]], color=255)
        cv.fillPoly(x, [mask_points[-3:]], color=255)
        # append the converted image into temp_X_masked
        cv.imwrite(os.path.join(dir_path, f"{j}.jpg"), x)
        temp_X_masked.append(x)
    # append temp_X_masked into  X_masked
    X_masked.append(temp_X_masked)
# T3 end ____________________________________________________________________________________


# T4 start __________________________________________________________________________________
# Build a detector that can detect presence of facemask given an input image

# X_features = []
# for x_list in X_masked:
#     temp_X_features = []
#     for x in x_list:
#         x_feature = hog(x, orientations=8, pixels_per_cell=(10, 10),
#                         cells_per_block=(1, 1), visualize=False, multichannel=False)
#         temp_X_features.append(x_feature)
#     X_features.append(temp_X_features)


# write code to split the dataset into train-set and test-set

# write code to train and test the SVM classifier as the facemask presence detector

# T4 end ____________________________________________________________________________________
