# libtinfo

用于验证清华大学INFO账户的通用C**动态链接库**

## 简介

本库是一个可以以动态链接库嵌入到其他程序中的清华大学INFO账户验证模块, 接受用户名和密码两个变量，本库可以可以返回目标用户的姓名、学号、院系等信息.

## 使用方式

本仓库提供一个头文件和一个C动态链接库,您可以在任意的其他程序中调用本库.

## 测试程序

1. ```shell
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

如果你是Windows平台，由于编译比较麻烦，可以直接下载预编译版本，如果你是Linux/MacOS平台，可以基于下述描述从源码构建，在三个平台上均已通过编译测试.

首先确保您的环境中安装了**libcurl和gcc**, 验证方式为命令行运行**curl-config --prefix**, 并且确保该curl版本是可以用于开发的(即curl-devel). 假设你的curl地址位于/usr/local,目标安装目录是/usr/local

在MacOS上, 默认编译工具也可以选择为clang

编译:

```shell
$ make CURL_DIR=/usr/local CC=gcc
```

安装:

```shell
$ make install INSTALL_DIR=/usr/local/ CC=gcc
```

卸载:

```shell
$ make remove INSTALL_DIR=/usr/local/ CC=gcc
```

打包:

```shell
$ make dist CC=gcc
```

如果你在交叉编译(比如在MacOS上使用x86_64-w64-mingw32编译windows的版本), 则把上述的gcc全部更换为x86_64-w64-mingw32即可,并且CURL_DIR也需要更换到相应的目录.

## 下载预编译的二进制动态链接库

本仓库提供一个基于Windows x64平台的预编译版本,你可以直接从右边的Release列表中点击最新版本的发布包,并在其中下载(名字为libtinfo-*-x86_64-w64-mingw32)

本预编译版本在MacOS上使用mingw-w64进行交叉编译所得，具体参数在文件的share目录中可以看到

## 其他信息

本项目使用到了**libcurl**和**cJSON**
