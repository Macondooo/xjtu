import numpy as np
import dlib
import cv2 as cv


class Preprocessor:
    def __init__(self, predictor_path: str) -> None:
        self.detector = dlib.get_frontal_face_detector()
        self.predictor = dlib.shape_predictor(predictor_path)

    def preprocess(self, imgs: list[np.ndarray]):
        ans = []
        for i, x in enumerate(imgs):
            dets = self.detector(x, 2)
            det = dets[0]
            x_gray = dlib.as_grayscale(x)
            shape = self.predictor(x_gray, det)
            # write the code to crop the image (x) to keep only the face, resize the cropped image to 150x150
            temp_points = np.array(shape.parts())
            points: np.ndarray = np.ndarray(shape=(0, 2), dtype=np.uint8)
            for p in temp_points:
                points = np.vstack([points, [p.x, p.y]])
            x_axis, y_axis = points[:, 0], points[:, 1]

            # rotate face by feature points
            # the feature point chosen are No.8 (the mid point of jaw line) and No.27 (the top point of nose)
            center = (points[27] + points[8]) / 2
            orient_vec: np.ndarray = points[27] - points[8]
            angle = np.sign(orient_vec[0]) * np.arccos(-orient_vec[1] /
                                                       np.linalg.norm(orient_vec))
            # perform transform
            rotation_matrix = cv.getRotationMatrix2D(
                center=center, angle=angle * 180 / np.pi, scale=1)
            x = cv.warpAffine(
                src=x, M=rotation_matrix, dsize=(x.shape[1], x.shape[0]))
            points = np.vstack([points.T, [1] * points.shape[0]])
            points = (rotation_matrix @ points).T

            # Cut the image to square to avoid distortion
            x_axis, y_axis = points[:, 0], points[:, 1]
            left, right = np.min(x_axis), np.max(x_axis)
            top, bottom = np.min(y_axis), np.max(y_axis)
            w, h = right - left, bottom - top
            if w > h:
                top -= (w - h) / 2
                bottom += (w - h) / 2
            else:
                left -= (h - w) / 2
                right += (h - w) / 2
            margin_ratio = 0.05  # keep margin or not
            resized_x = x[int(top - margin_ratio * w):int(bottom + margin_ratio * w),
                          int(left - margin_ratio * w):int(right + margin_ratio * w)]
            resized_x = dlib.resize_image(resized_x, 150, 150)
            ans.append(resized_x)
            print(f'{i + 1} / {len(imgs)} preprocessed')
        return np.array(ans)