import os
# import more libraries as you need
import dlib
import cv2

# T1  start _______________________________________________________________________________
# Read in Dataset

# change the dataset path here according to your folder structure
dataset_path = "./Dataset_1/Dataset_1"

X = []
y = []
for subject_name in os.listdir(dataset_path):
    y.append(subject_name)
    subject_images_dir = os.path.join(dataset_path, subject_name)

    temp_x_list = []
    for img_name in os.listdir(subject_images_dir):
        # write code to read each 'img'
        img_path=os.path.join(subject_images_dir,img_name)
        img=cv2.imread(img_path)
        # add the img to temp_x_list
        temp_x_list.append(img)
    # add the temp_x_list to X
    X.append(temp_x_list)
# T1 end ____________________________________________________________________________________

# # T2 start __________________________________________________________________________________
# # Preprocessing
# X_processed = []
# for x_list in X:
#     temp_X_processed = []
#     for x in x_list:
#         # write the code to detect face in the image (x) using dlib facedetection library

#         # write the code to crop the image (x) to keep only the face, resize the cropped image to 150x150

#         # write the code to convert the image (x) to grayscale

#         # append the converted image into temp_X_processed

#     # append temp_X_processed into  X_processed

# # T2 end ____________________________________________________________________________________


# # T3 start __________________________________________________________________________________
# # Create masked face dataset
# X_masked = []
# for x_list in X_processed:
#     temp_X_masked = []
#     for x in x_list:
#         # write the code to detect face in the image (x) using dlib facedetection library

#         # write the code to add synthetic mask as shown in the project problem description

#         # append the converted image into temp_X_masked

#     # append temp_X_masked into  X_masked

# # T3 end ____________________________________________________________________________________


# # T4 start __________________________________________________________________________________
# # Build a detector that can detect presence of facemask given an input image
# X_features = []
# for x_list in X_masked:
#     temp_X_features = []
#     for x in x_list:
#         x_feature = hog(x, orientations=8, pixels_per_cell=(10, 10),
#                         cells_per_block=(1, 1), visualize=False, multichannel=False)
#         temp_X_features.append(x_feature)
#     X_features.append(temp_X_features)


# # write code to split the dataset into train-set and test-set

# # write code to train and test the SVM classifier as the facemask presence detector

# # T4 end ____________________________________________________________________________________
