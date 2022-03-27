#include <iostream>
#include <unistd.h>
#include <tinfo.h>
using namespace std;

int main(){
    char usr[1024];
    cout << "--libinfo(C++)登录测试--\n请输入INFO用户名:";
    cin.getline(usr,1024);
    char * pwd=getpass("请输入INFO密码:");
    TINFO_USER * testu=tinfo_user_init();
    tinfo_setopt(testu,THUACCOUNT_USERNAME,usr);
    tinfo_setopt(testu,THUACCOUNT_PASSWORD,pwd);
    TINFO_ACCOUNT * acco=tinfo_verify(testu);
    cout <<endl;
    if(acco == NULL){
        cout << tinfo_strerrno() <<endl;
    }else{
        cout << "验证成功!\n" <<tinfo_stringfy(acco);
    }
    tinfo_clear(acco);
    return 0;
}