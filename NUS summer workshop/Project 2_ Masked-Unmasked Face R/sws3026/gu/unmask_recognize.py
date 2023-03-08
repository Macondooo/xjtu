import numpy as np
from matplotlib import pyplot as plt
from Classifier import Classifier
from Preprocessor import Preprocessor
from sklearn.metrics import accuracy_score
import joblib


data_path = "Dataset_1"

if __name__ == "__main__":
    x = []
    y = []
    subject_prefix = "s"
    for i in range(50):
        subject_name = f"{subject_prefix}{(i + 1) // 10}{(i + 1) % 10}"
        for j in range(14, 15):
            file_name = f"{(j + 1) // 10}{(j + 1) % 10}.jpg"
            x.append(plt.imread(data_path + '/' +
                     subject_name + '/' + file_name))
            y.append(i)

    preprocessor = Preprocessor('shape_predictor_68_face_landmarks.dat')
    model: Classifier = joblib.load("unmask_model.joblib")

    x = preprocessor.preprocess(x)
    dimensions, width, height, channels = x.shape
    processed = x.reshape((dimensions, width * height * channels))

    y_pred = model.predict(processed)
    print(accuracy_score(y, y_pred))