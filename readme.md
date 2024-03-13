### 0. 编译过程

```
mkdir build
cd build
cmake ..  // 将上一级目录的cmake文件，编译到当前目录
cmake -G "Visual Studio 16 2019" -A Win32 .. // 选择 win32
cmake -G "Visual Studio 16 2019" -A x64 ..   // 选择 x64
cmake --build .  //编译当前目录项目
```



















