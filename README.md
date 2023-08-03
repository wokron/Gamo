# Gamo Engine
- [English](README.md)
- [中文](docs/README_zh.md)

## What is this?
Gamo is a 2D game engine written in C++, with Python used as a scripting language. The Gamo engine has implemented some basic functionalities, but there is still a long way to go before it's fully developed. If you are interested in this project, feel free to provide suggestions or contribute to its development. Your attention is the greatest recognition for me (wokron).

## How to Build?
This project is currently a work in progress, but it already provides some interfaces that can be used by Python. To build this project, make sure you have cmake installed, as well as the SDL2 and SDL2_image libraries.
```sh
$ apt-get install cmake
$ apt-get install libsdl2-dev
$ apt-get install libsdl2-image-dev
```

Then execute the following commands:
```sh
$ mkdir build
$ cd build
$ cmake ..
```

This will generate a shared library file named `gamo_core_py.cpython-*.so` that can be imported in Python. You can import this file in Python by executing `import gamo_core_py`.
```sh
$ cd ./build/py/gamo/core
$ python3
>>> import gamo_core_py
```

Alternatively, you can directly run the file `./script/build.sh`, which will perform the build operation and move the generated file to `py/gamo/core`. At this point, you can directly import the Python module named core.
```sh
$ bash ./script/build.sh
$ cd ./py/gamo
$ python3
>>> import core
```
