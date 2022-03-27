# libtinfo

用于验证清华大学INFO账户的通用动态链接库

## 简介

本库是一个可以以动态链接库嵌入到其他程序中的清华大学INFO账户验证模块, 接受用户名和密码两个变量，本库可以可以返回目标用户的姓名、学号、院系等信息.

## 使用方式

##### 以下所有测试均在MacOS上进行

下载头文件**tinfo.h**和对应的动态链接库**build/libtinfo.dylib**后, 即可在你的项目中使用。 本仓库提供两个测试源码 **example.c**和**example.cpp**,分别显示如何在C/C++项目中调用本库。

## 测试程序构建

1. 下载**build/libtinfo.dylib**和**tinfo.h**,注意是一个动态链接库和一个快捷方式, 假定现在的目录是xxx, 现在把example.c和这两个文件放在同一个目录下

2. ```shell
   cd xxx
   gcc example.c -o example -I. -L. -ltinfo
   export DYLD_LIBRARY_PATH=xxx:${DYLD_LIBRARY_PATH}
   ./example
   ```

3. ```shell
   --LIBTINFO 1.0 on MacOS--
   请输入您的用户名:***
   请输入您的密码:***
   ----学生信息----
   #姓名:**
   #学院:**
   #用户名:**
   #学号:**
   #手机号:**
   ---------------
   ```

## 从源码构建动态链接库

您需要先从cJSON的仓库下载cJSON.c和cJSON.h，然后使用Makefile进行构建.

## 其他信息

本项目使用到了**libcurl**和**cJSON**
