# 📷 Industrial Vision & Data Analysis Platform (工业视觉采集与分析平台)

本项目是一个基于 Qt (C++) 构建的工业相机实时图像采集、视觉处理与数据分析的综合桌面软件平台，是作者学生时代学习搞出来的Demo，换电脑遂上传git留作纪念。系统底层集成了海康威视（Hikvision）机器视觉 SDK 以支持硬件交互，利用 OpenCV 进行核心图像处理，并通过 OpenGL 和 QCustomPlot 实现高性能的图像渲染与数据曲线可视化，最终支持将处理结果一键导出至 Excel。

- 作者：Gtree
- 日期2022.10
- 邮箱：gtree.cn@outlook.com

## ✨ 核心特性 (Key Features)

* **🎥 工业相机集成 (Hikvision SDK)**
* 基于海康 `MvCameraControl` SDK 深度封装 (`cmvcamera`, `camerathread`)。
* 支持相机的连接、初始化、实时图像拉流（Grab）以及曝光、增益等底层硬件参数的控制。


* **⚡ 高性能多线程架构**
* 将界面 UI、相机采集 (`CameraThread`) 与图像处理算法 (`ProcessingThread`) 分离为独立线程，确保在高帧率连续处理时界面依然流畅响应。


* **👁️ 机器视觉图像处理**
* 项目内置了 OpenCV 的完整开发包 (支持 MSVC 和 MinGW 编译环境)，可直接进行形态学处理、目标检测、缺陷识别、标定测量等视觉算法的二次开发。


* **📈 多维数据可视化**
* **QCustomPlot**: 集成强大的 Qt 绘图库，支持实时绘制工业检测数据曲线、直方图、散点图等二维统计图表。
* **OpenGL (`MyGLWidget`)**: 采用硬件加速的 OpenGL 视窗，支持海量点云显示、3D 渲染或超高分辨率图像的快速缩放与平移查看。


* **📊 Excel 数据报表管理**
* 提供独立的 Excel 读写操作模块 (`excelmanger`, `exceloperator`)，支持业务数据的结构化整理，并自动生成/导出 Excel 格式的检测报告或日志。



## 📁 目录结构 (Directory Structure)

```text
0328/
├── Camera.pro                # Qt 项目工程文件
├── main.cpp                  # 应用程序入口
├── mainwindow.h / .cpp / .ui # 主窗口及 UI 界面逻辑
├── HikSdk/                   # 核心视觉与相机控制模块
│   ├── SDK/                  # 海康 MvCameraControl 原生头文件与静态库
│   ├── OpenCV/               # OpenCV 包含目录及预编译库 (支持 Msvc/MinGW)
│   ├── cmvcamera.* # 海康相机操作控制封装类
│   ├── camerathread.* # 负责持续获取相机图像帧的独立线程
│   ├── processingthread.* # 负责执行 OpenCV 算法逻辑的处理线程
│   └── globalvar.* # 全局变量及跨线程共享数据定义
├── qcustomplot.h / .cpp      # QCustomPlot 第三方二维数据绘图库
├── myglwidget.h / .cpp       # OpenGL 图像/图形自定义渲染组件
└── excelmanger.h / .cpp      # Excel 报表生成与数据管理接口
    └── exceloperator.h/.cpp  # 底层 Excel 文件读写实现 (基于 ActiveQt/COM)

```

## 🛠️ 开发环境与依赖 (Dependencies)

* **开发框架**: Qt 5.x (建议使用 Qt 5.12+，需包含 `core`, `gui`, `widgets`, `axcontainer`(用于Excel), `printsupport` 等模块)
* **编译器**: MSVC 2017+ 64-bit 或 MinGW 64-bit (项目中已提供这两个环境的 OpenCV lib 库)
* **机器视觉 SDK**: [海康机器视觉 MVS 客户端](https://www.hikrobotics.com/) (运行该软件所在的主机必须安装海康的 MVS 客户端以提供底层驱动和 DLL 运行环境)
* **视觉算法库**: OpenCV 4.x (项目 `HikSdk/OpenCV` 下已内置)

## 🚀 编译与运行指南 (Build & Run)

1. **环境准备**：
* 确保您的电脑上安装了对应网口/USB 接口的海康工业相机，并能通过海康 MVS 官方软件正常预览图像。
* 确保已安装带有 `MSVC` 或 `MinGW` 编译器的 Qt Creator。


2. **配置工程**：
* 双击 `Camera.pro` 在 Qt Creator 中打开项目。
* 检查 `.pro` 文件中关于 `INCLUDEPATH` 和 `LIBS` 的路径。项目内带有相对路径配置，但若因环境不同导致找不到 `MvCameraControl.lib` 或 `opencv_world.lib`，请根据项目实际存放位置调整 `.pro` 文件中的链接路径。


3. **构建项目**：
* 在 Qt Creator 中选择构建套件（推荐 `Release` - `64bit`）。
* 点击 **构建 (Ctrl+B)**。


4. **运行程序**：
* 点击 **运行 (Ctrl+R)**。
* *注: 如果程序启动时提示缺失 DLL（如 `opencv_world4xx.dll` 或 `MvCameraControl.dll`），请将对应的 `.dll` 文件复制到编译输出目录（`.exe` 所在同级目录），或将库路径添加到系统的环境变量 PATH 中。*



## 💡 开发说明

* **如何添加新的视觉算法？**
找到 `HikSdk/processingthread.cpp`，该线程会在相机回调拿到 `cv::Mat` 后被触发，您可以在该线程的 `run()` 方法中添加自定义的 OpenCV 处理代码，并通过信号槽 (Signal & Slot) 将结果或数据发送回 `mainwindow` 进行界面更新。
* **数据如何写入 Excel？**
调用 `ExcelManger` 类的相关接口，在每次检测流程结束或用户点击“导出”时，传入整理好的数据模型即可。
