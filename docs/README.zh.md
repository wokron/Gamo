# Gamo Engine
- [English](README.md)
- [中文](docs/README.zh.md)

## 这是什么？
Gamo 是一个 2D 游戏引擎，采用 c++ 编写，并使用 python 作为脚本语言。目前 Gamo 引擎已经实现了一些基本的功能，但离完善还有很远。如果您对本项目感兴趣，欢迎您提出建议或参与本项目的开发，您的关注是对我（wokron）最大的认可。

## 构建过程
本项目目前并不完善，但已经能够提供一些可供 python 调用的接口。想要构建本项目，请确保已安装 cmake 以及 SDL2 和 SDL2_image 库
```sh
$ apt-get install install cmake
$ apt-get install libsdl2-dev
$ apt-get install libsdl2-image-dev
```

随后执行如下命令
```sh
$ mkdir build
$ cd build
$ cmake ..
```

这将生成可供 python 导入的共享目标文件 `gamo_core_py.cpython-*.so`，可以在 python 终端中执行 `import gamo_core_py` 导入该文件。
```sh
$ cd ./build/py/gamo/core
$ python3
>>> import gamo_core_py
```

或者直接执行文件 `./script/build.sh`，这将执行构建操作，并将生成的共享目标文件移动到 `py/gamo/core `下。此时可以直接导入 python 模块 core。
```sh
$ bash ./script/build.sh
$ cd ./py/gamo
$ python3
>>> import core
```
