# 图形学大作业

该部分收录本人在本科期间选修计算机图形学课程所完成的大作业，主要包括以下四部分内容：

- 渲染管线
- 曲面Loop细分
- 物理模拟
- 曲面简化
其中，渲染管线为单独的代码文件，位于rendering code中；其余三部分都在Scotty3D中实现
> 本实验主要是在CMU的Scotty3D环境中实现的，Scotty3D项目地址移步https://github.com/CMU-Graphics/Scotty3D
> 本课程中实现的代码请在本仓库的Scotty3D环境中运行（课程上做了一定修改）

## 渲染管线

该部分实验主要完成了以下部分
- 设置相机位置，计算MVP矩阵  `./rendering code/shaders/vshader.glsl`
- 实现了Blinn-Phong光照模型  `./rendering code/shaders/fshader.glsl`

## 曲面Loop细分
该部分实验编写的代码位于`./Scotty3D/src/student/meshedit.cpp`，主要完成了以下部分：
- 实现Loop细分算法中边的翻转
函数`Halfedge_Mesh::flip_edge()`
- 实现Loop细分算法中边分裂，指定边分裂为两条边，在新增顶点和指定边相对的顶点之间增加一条新边`Halfedge_Mesh::split_edge()`
- 调用如上实现函数，实现Loop细分算法`Halfedge_Mesh::loop_subdivide()`

## 物理模拟

该部分实验编写的代码位于`./Scotty3D/src/student/meshedit.cpp`，主要完成了以下部分：



## 曲面简化

该部分实验采用的是基于二次误差度量 (QEM, Quadric Error Metrics) 和边折叠操作的简化算法。编写代码位于`./Scotty3D/src/student/meshedit.cpp`中函数`Halfedge_Mesh::simplify  `。具体详述请参看文档`./曲面简化实验报告`

