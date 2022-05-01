#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<tinfo.h>

int main(){
    #ifdef WIN32
    system("chcp 65001");
    #endif
    THUINFO_USER *exp=tinfo_user_init();
    char usr[1024];
    printf("--%s--\n(tinfo示例程序)\n请输入您的用户名:",tinfo_version);
    scanf("%s",usr);
    #ifndef WIN32
    char * pwd=getpass("请输入您的密码:");
    #else 
    char pwd[1024];
    puts("(由于此版本针对Windows环境编译,您的密码会直接显示在控制台)");
    printf("请输入您的密码:");
    scanf("%s",pwd);
    #endif
    int x=tinfo_setopt(exp,THUACCOUNT_USERNAME,usr);
    int y=tinfo_setopt(exp,THUACCOUNT_PASSWORD,pwd);
    if(x!=0||y!=0){
        printf("err:%d-%s\n",tinfo_errno,tinfo_strerrno());
    }
    THUINFO_ACCOUNT * s=tinfo_verify(exp);
    if(s==NULL){
        printf("Error:%d(%s)\n",tinfo_errno,tinfo_strerrno());
        #ifdef WIN32
        system("pause");
        #endif
        return 1;
    }
    printf("%s",tinfo_stringfy(s));
    tinfo_clear(exp);
    #ifdef WIN32
    system("pause");
    #endif
    return 0;
}