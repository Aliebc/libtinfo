#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <tinfo.h>
using namespace std;

int main(){
    #ifdef WIN32
    system("chcp 65001");
    #endif
    char usr[1024];
    cout << tinfo_version << "  (C++测试程序)\n请输入INFO用户名:";
    cin.getline(usr,1024);
    #ifndef WIN32
    char * pwd=getpass("请输入INFO密码:");
    #else
    char pwd[1024];
    cout << "由于本测试程序为Windows编译,密码会直接显示在控制台,请注意!\n请输入INFO密码:";
    cin.getline(pwd,1024);
    #endif
    tinfo test(usr,pwd);
    test.verify();
    cout << endl << test << endl;
    #ifdef WIN32
    system("pause");
    #endif
    return 0;
}