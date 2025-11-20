# hw3d: A DirectX11-based GPU Renderer

![DirectX11](https://img.shields.io/badge/DirectX-11-9cf?logo=directx)  
![C++](https://img.shields.io/badge/C++-20-blue?logo=cplusplus)  
![Visual Studio](https://img.shields.io/badge/IDE-Visual%20Studio%202022-purple?logo=visualstudio)

## 📖 项目简介

**hw3d**（Hello World 3D）是一个基于 **DirectX11** 开发的 GPU 渲染器，实现了现代实时渲染管线，并集成了**基于物理的渲染（PBR）** 系统，支持多种高级渲染特性，以增强视觉真实感。

该项目旨在深入理解图形 API 底层原理、现代渲染管线架构，并提升 C++ 与图形编程的工程实践能力。

---

## ✨ 功能特性

### 🎨 现代渲染管线
- 使用 DirectX11 从零搭建应用程序窗口、设备与交换链
- 高效管理渲染资源，构建动态资源调用系统
- 优化 CPU-GPU 协作，理解 API 开销与性能瓶颈

### 🌟 基于物理的渲染（PBR）
- 实现 Phong 光照模型
- 支持多种纹理贴图：漫反射、法线、镜面反射等

### 🧩 动态分级模型加载
- 使用 **Assimp** 导入外部模型
- 构建树状结构管理模型节点
- 支持对节点进行旋转、缩放、平移等动态操作

### 🖥️ 实时调试 UI 系统
- 集成 **Dear ImGui** 组件
- 实时调整 ConstantBuffer 参数
- 便于渲染效果的测试与优化

---

## 🛠️ 技术栈

- **编程语言**: C++ (C++20 标准)
- **图形 API**: DirectX11
- **着色器语言**: HLSL
- **开发环境**: Visual Studio 2022
- **版本控制**: Git
- **第三方库**: Assimp, Dear ImGui

---

## 🚀 构建与运行

### 环境要求
- Windows 10/11
- Visual Studio 2022
- DirectX11 运行时环境

### 编译步骤
1. 克隆本仓库：
   ```bash
   git clone https://github.com/oldmikestpark/hw3d.git
