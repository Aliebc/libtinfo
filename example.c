#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<tinfo.h>

int main(){
    THUINFO_USER *exp=tinfo_user_init();
    char usr[1024];
    printf("--%s--\n请输入您的用户名:",tinfo_version);
    scanf("%s",usr);
    char * pwd=getpass("请输入您的密码:");
    int x=tinfo_setopt(exp,THUACCOUNT_USERNAME,usr);
    int y=tinfo_setopt(exp,THUACCOUNT_PASSWORD,pwd);
    if(x!=0||y!=0){
        printf("err:%d-%s\n",tinfo_errno,tinfo_strerrno());
    }
    THUINFO_ACCOUNT * s=tinfo_verify(exp);
    if(s==NULL){
        printf("Error:%d(%s)\n",tinfo_errno,tinfo_strerrno());
        return 1;
    }
    printf("%s",tinfo_stringfy(s));
    tinfo_clear(exp);
    #ifdef WIN32
    system("cls");
    #endif
    return 0;
}