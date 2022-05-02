#include <iostream>
#include <stdlib.h>
#include <tinfo.h>
using namespace std;

int main(){
    #ifdef WIN32
    system("chcp 65001");
    system("cls");
    #endif
    char usr[1024];
    cout << tinfo_version << "  (C++测试程序)\n请输入INFO用户名:";
    cin.getline(usr,1024);
    char * pwd=tinfo_getpass("请输入INFO密码:");
    tinfo test(usr,pwd);
    test.verify();
    cout << endl << test << endl;
    #ifdef WIN32
    system("pause");
    #endif
    return 0;
}