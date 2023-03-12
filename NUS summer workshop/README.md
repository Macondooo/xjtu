# NUS summer workshop

欢迎来到该目录😊，本目录记录了2022年夏作者在新加坡国立大学所完成的项目`Masked-Unmasked Face Recognition`
## 项目内容速览
速览请戳`./Showcase.pptx`
为项目验收时，作者所在小组的展示PPT

该项目主要分为一下四部分内容：
-  依据戴口罩的数据集，创造一个戴口罩的数据集
    代码详见`stater.py`
    主要实现步骤为：
  - 从原始数据中读取图片
  - 使用人脸特征点标记模型`shape_predictor_68_face_landmarks.dat`,在图片上标记出人脸的68特征点；依据已标记的特征点求出最小的外界矩形；并重新设定图片的大小，裁剪掉多余的区域
  - 同样利用人脸特征点标记模型`shape_predictor_68_face_landmarks.dat`，连接下颚线，鼻子等特征点，建立一个伪戴口罩的人脸图片。

- 给定一个人脸图片，判断图片中的人脸是否佩戴口罩
    代码详见`mask_detection.py`
    主要实现步骤为：
  - 使用HOG算法，提取图像特征
  - 依据提取的特征，使用SVM线性核进行分类
  - 计算confusion_matrix，来评价分类的效果

- 对未带口罩的人脸，进行人脸识别
	代码详见`feature_extract.py`
  - 使用SVD分解，提取特征人脸（基向量U），获得人脸在特征向量上的分量（$V^T$）
  - 将$V^T$和人脸标签y作为SVM的输入，训练对未带口罩的人脸识别

- 类似于上一个，输入仍是未带口罩的人脸，但需要对带口罩的人脸进行识别
	这里主要考虑了2种方法，第一种考虑是，对于已经带口罩的人脸，大部分有效信息都被遮蔽，能否只通过人脸眼睛的信息来识别。代码详见`extract_eyes.ipynb, eyes_pca.py`。经过试验这种方法准确性并不高，只有0.74左右
	第二种考虑是，只使用眼部信息来处理，仍丢失了一部分信息。仍然希望使用特征人脸的信息来处理这件事。具体处理方法如下
	- 在对人脸的预处理上，除了传统的裁剪之外，还依据特征点对人脸进行旋转（`Preprocessor.py`）
	- 


## 代码速览
代码目录结构如下所示：
```
Project 2_ Masked-Unmasked Face R:
├─Dataset_1				//项目给定的原始人脸数据，包含50组人脸，每组15张不同角度的照片
├─sws3026				//代码以及代码所需数据所在的目录
   ├─4_final    		//戴口罩人脸识别（部分4）最终代码
      ├─Classifier.py
      ├─mask_model.py
      ├─Preprocessor.py
      ├─unmask_build_model.py
      ├─unmask_model.joblib
      ├─unmask_recognize.py
   ├─dataset_mask      		 //裁剪后人脸图片人为加上口罩
   ├─dataset_nomask   		 //原始人脸图片的裁剪
   ├─dataset_nomasked_eyes   //原始人脸图片眼睛部分的裁剪
   ├─eigeneyes               //眼睛部分的PCA提取图片
   ├─eigenfaces_nomask       //未带口罩人脸PCA提取
   |
   ├─extract_eyes.ipynb		 //对于未带口罩的人脸，试图截取眼睛部分用PCA提取特征
   ├─eyes_pca.py             //同上
   ├─feature_extract.py 	 //训练PCA模型识别未带口罩的人脸
   ├─mask_detection.py  	 //训练判断图片中的人脸是否佩戴口罩的模型
   ├─stater.py               //读入原始数据，并建立伪带口罩的人脸图片

├─MaskedUnmaskedFaceRecognition //项目指导文档
```
