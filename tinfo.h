/*===---------------------------- tinfo.h ---------------------------------===//
//
// Libtinfo v1.1
// TINFO(THUINFO)库 头文件
// 本库中的函数可以进行清华大学INFO账户的验证
// Copyright (C) 2022 LIBTINFO 许可请见LICENSE
// 作者:Aliebc    E-mail:aliebcx@outlook.com
// 项目开发语言:C 标准:ISO-C99
//
//===----------------------------------------------------------------------===*/

/*宏定义部分*/

#ifndef TINFO_H
#define TINFO_H

#define tinfo_errno _TINFO_ERRNO
#define tinfo_version _TINFO_VERSION

#define TINFO_MAIN_VERSION 1
#define TINFO_SUB_VERSION 1
#define TINFO_MAIN_VERSION_STR "1"
#define TINFO_SUB_VERSION_STR "1"

#define THUACCOUNT_USERNAME 101
#define THUACCOUNT_PASSWORD 102

#define TINFO_USER THUINFO_USER
#define TINFO_ACCOUNT THUINFO_ACCOUNT

/*对Windows系统的动态链接库规则进行定义处理*/
#ifdef WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif
/*宏定义部分结束*/

/*TINFO标准错误码,如果出现异常,错误码将会出现在本变量中*/
EXPORT extern int _TINFO_ERRNO;
/*TINFO版本号,字符串常量*/
EXPORT extern const char _TINFO_VERSION[];

/*结构体定义部分*/

typedef unsigned long int PHONE;
typedef unsigned long int ID;
/*THUINFO_USER结构体,存放等待提交的用户名和密码,声明时请调用初始化函数进行初始化*/
typedef struct THUINFO_USER THUINFO_USER;
/*返回的THUINFO_ACCOUNT结构体,如果验证成功,结构体中将包含目标学生的信息*/
typedef struct THUINFO_ACCOUNT THUINFO_ACCOUNT;
/*THUINFO_USER结构体,存放等待提交的用户名和密码*/
struct THUINFO_USER
{
    char * username; /*目标用户名*/
    char * password; /*目标密码*/
};
/*返回的THUINFO_ACCOUNT结构体,如果验证成功,结构体中将包含目标学生的信息*/
struct THUINFO_ACCOUNT
{
    char * real_name; /*学生的真名*/
    char * user_name; /*学生的登录用户名*/
    char * department; /*学生的院系名称*/
    char * phone_number; /*学生的手机号*/
    char * user_id; /*学生的学号*/
};

/*结构体定义部分结束*/

/*库函数定义部分*/

/*tinfo标准错误文字化函数,用于输出错误的字符串形式,调用后会归零错误码*/
EXPORT char * tinfo_strerrno(void);
/*本函数用于关闭回显地获取密码,最长获取64位密码*/
EXPORT char * tinfo_getpass(const char * __prompt);
/*本函数用于初始化账户信息,为储存账户信息的变量分配内存*/
EXPORT struct THUINFO_USER * tinfo_user_init(void);
/*本函数用于设置账户的详细信息*/
EXPORT int tinfo_setopt(struct THUINFO_USER * ACCOUNT,int OPTION_CODE,char * OPTION_VALUE);
/*本函数为核心库函数,调用本函数将向清华服务器提交验证请求并处理返回值*/
EXPORT struct THUINFO_ACCOUNT * tinfo_verify(struct THUINFO_USER * ACCOUNT);
/*本函数可以字符串格式化输出ACCOUNT结构体*/
EXPORT char * tinfo_stringfy(struct THUINFO_ACCOUNT * ACCOUNT);
/*本函数用于回收空间,处理完毕后调用本函数释放内存*/
EXPORT int tinfo_clear(void * point);

#ifdef __cplusplus
}
#endif

/*库函数定义部分结束*/

/*C++部分*/

#ifdef __cplusplus

#include <iostream>
#include <stdlib.h>
#include <string.h>

#define ERROR_x1 "It hasn't been validated yet"
#define STR_SIZE 1024

class tinfo
{
private:
    THUINFO_USER user;
    THUINFO_ACCOUNT account;
    char * str;
public:
    int status;
    //tinfo();
    tinfo(char * usr,char * pwd);
    int verify();
    char * toString();
    char * getAttribute(const char * attribute);
    char * operator[](const char * attribute);
    friend std::ostream & operator<< (std::ostream & cout, const tinfo &ti);
    ~tinfo();
};

/*tinfo::tinfo()
{
    
}*/

/*构造函数,所需参数为用户名和密码*/
tinfo::tinfo(char * usr,char * pwd)
{
    this->status=-1;
    this->user.username=(char *)malloc((strlen(usr)+1)*sizeof(char));
    memcpy(this->user.username,usr,(strlen(usr)+1)*sizeof(char));
    this->user.password=(char *)malloc((strlen(pwd)+1)*sizeof(char));
    memcpy(this->user.password,pwd,(strlen(pwd)+1)*sizeof(char));
    this->str=(char *)malloc((STR_SIZE+1)*sizeof(char));
}

/*析构函数,自动释放内存*/
tinfo::~tinfo()
{
    free(this->user.username);
    free(this->user.password);
    free(this->str);
}

int tinfo::verify()
{
    TINFO_ACCOUNT * test=tinfo_verify(&this->user);
    if(test==NULL){
        this->status=-2;
    }else{
        this->account=*test;
        this->status=0;
    }
    return this->status;
}

char * tinfo::toString()
{
    switch (this->status)
    {
    case -1:
        strcpy(str,ERROR_x1);
        break;
    case -2:
        strcpy(str,tinfo_strerrno());
        break;
    case 0:
        strcpy(str,tinfo_stringfy(&this->account));
        break;
    default:
        break;
    }
    return this->str;
}

char * tinfo::getAttribute(const char * attribute)
{
    if(this->status!=0){
        return NULL;
    }
    if(strcmp(attribute,"real_name")==0){
        return this->account.real_name;
    }else if(strcmp(attribute,"phone_number")==0){
        return this->account.phone_number;
    }else if(strcmp(attribute,"department")==0){
        return this->account.department;
    }else if(strcmp(attribute,"user_id")==0){
        return this->account.user_id;
    }else if(strcmp(attribute,"user_name")==0){
        return this->account.user_name;
    }
    return NULL;
}

char * tinfo::operator[](const char * attribute)
{
    return this->getAttribute(attribute);
}

std::ostream & operator<< (std::ostream & cout, tinfo &ti)
{
    cout << ti.toString();
    return cout;
}

#endif

/*C++部分结束*/
/*以下为许可声明*/

/* 
==============================--LICENSE--===================================
================================--许可--=====================================
您被默认许可将本库用于以下用途:
1. 个人的使用、再开发、反汇编等
2. 在程设课大作业中使用本库
3. 保证完整的情况下复制本库中的内容

您不可将本库用于以下用途:
1. 使用本库对Tsinghua University的INFO账户批量攻击
2. 修改本库的版权声明(包括头文件中的一切声明)后以其他的名义重新发布给他人
3. 未经许可应用于商业活动
============================================================================
============================================================================
*/

/*头文件到此结束*/

#define TINFO
#endif
