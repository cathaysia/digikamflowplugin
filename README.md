# 截图

![1.gif](screenshot/1.gif)
![缩放.gif](./screenshot/缩放.gif)
![节省内存.gif](./screenshot/节省内存.gif)


# 构建

此项目依赖于以下组件（Fedora）：

- spdlog-devel
- digikam-devel
- qt5-qtbase-devel
- c++ 17
- cmake

要构建并安装，请执行以下指令：

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
sudo cmake --install build
```
