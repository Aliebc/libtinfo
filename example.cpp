#include <iostream>
#include <tinfo.h>
using namespace std;

int main(){
    char usr[1024];
    char pwd[1024];
    cout << "请输入INFO用户名:";
    cin.getline(usr,1024);
    cout << "请输入INFO密码:";
    cin.getline(pwd,1024);
    TINFO_USER * testu=tinfo_user_init();
    tinfo_setopt(testu,THUACCOUNT_USERNAME,usr);
    tinfo_setopt(testu,THUACCOUNT_PASSWORD,pwd);
    TINFO_ACCOUNT * acco=tinfo_verify(testu);
    cout <<endl;
    if(acco == NULL){
        cout << tinfo_strerrno() <<endl;
    }else{
        cout << tinfo_stringfy(acco);
    }
    tinfo_clear(acco);
    return 0;
}