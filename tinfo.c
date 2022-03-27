#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<curl/curl.h>
#include<tinfo.h>
#include<cJSON.h>

/*目标平台检测*/
#ifdef __MACH__
#define TINFO_PLATFORM "MacOS"
#endif
#ifdef linux
#define TINFO_PLATFORM "Linux"
#endif
#ifdef WIN32
#define TINFO_PLATFORM "Windows"
#endif
#ifndef TINFO_PLATFORM
#define TINFO_PLATFORM "Unknown System"
#endif

/*调试函数宏定义*/
#ifdef DEBUG
#define TEST(exp,fmt,...)\
    if(!(exp)){fprintf(stderr,fmt,##__VA_ARGS__);exit(1);}
#define CERR(exp,fmt,...)\
    if(!(exp)){fprintf(stderr,fmt,##__VA_ARGS__);exit(1);}
#else
#define TEST(exp,fmt,...)\
    void(0);
#define CERR(exp,fmt,...)\
    void(0);
#endif

/*字符串常量宏定义*/
#define TINFO_VERSION_STR "LIBTINFO " TINFO_MAIN_VERSION_STR "." TINFO_SUB_VERSION_STR " on " TINFO_PLATFORM

#define STRING_STDSIZE 257
#define MEMORY_STDSIZE 1024*1024
#define TINFO_ERRNO_200 "Cannot initialize CURL point."
#define TINFO_ERRNO_201 "Cannot malloc from memory."
#define TINFO_ERRNO_202 "Tsinghua Host unreachable."
#define TINFO_ERRNO_203 "Username or password is incorrect."
#define TINFO_ERRNO_204 "Option code not exists."
#define TINFO_ERRNO_205 "Cannot operate memory."
#define TINFO_ERRNO_206 "Out of memory!"
#define TINFO_ERRNO_207 "Format analyse failed!"

#define SECURITY_CHECK "https://id.tsinghua.edu.cn/security_check"
#define THU_SETTINGS "https://id.tsinghua.edu.cn/f/account/settings"
#define USER_AGENT "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.104 Safari/537.36 Core/1.53.2141.400 QQBrowser/9.5.10219.400"

int _TINFO_ERRNO=0;
const char _TINFO_VERSION[]=TINFO_VERSION_STR;

/*结构体定义*/
/*TINFO MEMORY结构,存放一些内存*/
struct tinfo_memory
{
    size_t size;
    size_t max_size;
    char * memory;
};

/*函数部分开始*/
/*内存分配函数*/
void * TINFO_MALLOC(int SIZE){
    void * ret=malloc(SIZE);
    if(ret==NULL){
        _TINFO_ERRNO = 201;
        return NULL;
    }else{
        return ret;
    }
}

/*内存重分配函数*/
void * TINFO_REALLOC(void *ptr,int SIZE){
    void * ret=realloc(ptr,SIZE);
    if(ret==NULL){
        _TINFO_ERRNO = 206;
        return NULL;
    }else{
        return ret;
    }
}

struct THUINFO_USER * tinfo_user_init(){
    struct THUINFO_USER * ret=(struct THUINFO_USER *)TINFO_MALLOC(sizeof(struct THUINFO_USER));
    if(ret==NULL){
        return NULL;
    }else{
        ret->username=(char *)TINFO_MALLOC(STRING_STDSIZE);
        ret->password=(char *)TINFO_MALLOC(STRING_STDSIZE);
        if(ret->username==NULL||ret->password==NULL){
            return NULL;
        }else{
            return ret;
        }
    }
}

/*THUINFO_USER操作函数*/
int tinfo_setopt(struct THUINFO_USER * ACCOUNT,int OPTION_CODE,char * OPTION_VALUE){
    switch (OPTION_CODE)
    {
    case 101:
        if(strlen(OPTION_VALUE)<STRING_STDSIZE){
            strcpy(ACCOUNT->username,OPTION_VALUE);
            return 0;
        }else{
            _TINFO_ERRNO=206;
            return 1;
        }
        break;
    case 102:
        if(strlen(OPTION_VALUE)<STRING_STDSIZE){
            strcpy(ACCOUNT->password,OPTION_VALUE);
            return 0;
        }else{
            _TINFO_ERRNO=206;
            return 1;
        }
        break;
    default:
        _TINFO_ERRNO=204;
        return 1;
        break;
    }
}

char * tinfo_strerrno(){
    int t_errno=_TINFO_ERRNO;
    char * errstr[]={
        TINFO_ERRNO_200,
        TINFO_ERRNO_201,
        TINFO_ERRNO_202,
        TINFO_ERRNO_203,
        TINFO_ERRNO_204,
        TINFO_ERRNO_205,
        TINFO_ERRNO_206,
        TINFO_ERRNO_207
    };
    _TINFO_ERRNO=0;
    if(t_errno<208 && t_errno>=200){
        return errstr[t_errno-200];
    }else{
        return NULL;
    }
}

struct THUINFO_ACCOUNT * tinfo_account_init(){
    struct THUINFO_ACCOUNT * ret=(struct THUINFO_ACCOUNT *)TINFO_MALLOC(sizeof(struct THUINFO_ACCOUNT));
    if(ret==NULL){
        return NULL;
    }else{
        ret->real_name=(char *)TINFO_MALLOC(STRING_STDSIZE);
        ret->department=(char *)TINFO_MALLOC(STRING_STDSIZE);
        ret->user_name=(char *)TINFO_MALLOC(STRING_STDSIZE);
        ret->phone_number=(char *)TINFO_MALLOC(STRING_STDSIZE);
        ret->user_id=(char *)TINFO_MALLOC(STRING_STDSIZE);
        if(ret->real_name==NULL||ret->department==NULL||ret->user_name==NULL){
            return NULL;
        }else{
            return ret;
        }
    }
}

char * URLEncode(char * from){
    char * ret=(char *)TINFO_MALLOC(STRING_STDSIZE*2);
    if(ret==NULL){
        return NULL;
    }else{
    }
    return NULL;
}

static int tinfo_write_data(void * ptr,size_t size,size_t nmemb,void * stream){
    size_t written = fwrite(ptr,size,nmemb,(FILE *)stream);
    return written;
}

size_t tinfo_curl2memory(char *buffer,size_t size,size_t nitems,void *userdata){
    size_t true_size=size*nitems;
    struct tinfo_memory * uss=(struct tinfo_memory *)userdata;
    memcpy(&uss->memory[uss->size],buffer,true_size);
    uss->size+=true_size;
    uss->memory[uss->size]='\0';
    return true_size;
}

size_t tinfo_curl2memory_findcookie(char *buffer,size_t size,size_t nitems,void *userdata){
    size_t true_size=size*nitems;
    struct tinfo_memory * uss=(struct tinfo_memory *)userdata;
    if(buffer[0]=='S'&&buffer[1]=='e'&&buffer[2]=='t'){
        memcpy(&uss->memory[uss->size],&buffer[4],true_size-4);
        uss->size+=true_size;
        uss->memory[uss->size]='\0';
    }
    return true_size;
}

char * tinfo_body2json(char * src){
    int p1=0;
    int p2=0;
    int count=0;
    char key_start[]="<script>";
    char key_end[]="</script>";
    while(count<2&&p1<strlen(src)){
        while(*(src+p1)!='<'){
            p1++;
        }
        if(memcmp(key_start,&src[p1],8)==0){
            count++;
        }
        p1++;
    }
    if(p1==strlen(src)){
        _TINFO_ERRNO=203;
        return NULL;
    }
    p2=p1;
    while(p2<strlen(src)){
        while(*(src+p2)!='<'){
            p2++;
        }
        if(memcmp(key_end,&src[p2],9)==0){
            break;
        }
        p2++;
    }
    if(p2==strlen(src)){
        _TINFO_ERRNO=203;
        return NULL;
    }
    p1=p1+8+16;
    p2=p2-3;
    char * ret=(char*)TINFO_MALLOC(sizeof(char)*(p2-p1+1));
    if(ret==NULL){
        return NULL;
    }
    memcpy(ret,&src[p1],p2-p1);
    ret[p2-p1]='\0';
    return ret;
}

struct THUINFO_ACCOUNT * json2account(char * src,struct THUINFO_ACCOUNT * ACCOUNT){
    cJSON * tinfo_yjson=cJSON_Parse(src);
    if(tinfo_yjson==NULL){
        return NULL;
    }else{
        cJSON * tinfo_yjson_ss=cJSON_GetObjectItem(tinfo_yjson,"ss");
        if(tinfo_yjson_ss==NULL){
            return NULL;
        }
        cJSON_Print(tinfo_yjson_ss);
        cJSON * tinfo_yjson_account=cJSON_GetObjectItem(tinfo_yjson_ss,"account");
        strcpy(ACCOUNT->real_name,cJSON_GetStringValue(cJSON_GetObjectItem(tinfo_yjson_account,"realName")));
        strcpy(ACCOUNT->department,cJSON_GetStringValue(cJSON_GetObjectItem(tinfo_yjson_account,"deptString")));
        strcpy(ACCOUNT->user_name,cJSON_GetStringValue(cJSON_GetObjectItem(tinfo_yjson_account,"username")));
        strcpy(ACCOUNT->user_id,cJSON_GetStringValue(cJSON_GetObjectItem(tinfo_yjson_account,"userId")));
        strcpy(ACCOUNT->phone_number,cJSON_GetStringValue(cJSON_GetObjectItem(tinfo_yjson_account,"phone")));
        cJSON_Delete(tinfo_yjson);
    }
    return ACCOUNT;
}

struct THUINFO_ACCOUNT * tinfo_verify(struct THUINFO_USER * ACCOUNT){
    struct THUINFO_ACCOUNT * ret=tinfo_account_init();
    struct tinfo_memory tinfo1_header={0};
    struct tinfo_memory tinfo2_body={0};
    if(ret==NULL){
        return NULL;
    }
    tinfo1_header.memory=(char*)TINFO_MALLOC(MEMORY_STDSIZE);
    tinfo2_body.memory=(char*)TINFO_MALLOC(MEMORY_STDSIZE);
    if(tinfo1_header.memory==NULL||tinfo2_body.memory==NULL){
        return NULL;
    }
    CURL *tinfo1=curl_easy_init();
    struct curl_slist * headers=NULL;
    if(tinfo1==NULL){
        _TINFO_ERRNO=200;
        return NULL;
    }
    curl_easy_setopt(tinfo1,CURLOPT_URL,SECURITY_CHECK);
    curl_easy_setopt(tinfo1,CURLOPT_USERAGENT,USER_AGENT);
    curl_easy_setopt(tinfo1,CURLOPT_POST,1);
    char pf[STRING_STDSIZE*2];
    sprintf(pf,"username=%s&password=%s",curl_easy_escape(tinfo1,ACCOUNT->username,0),curl_easy_escape(tinfo1,ACCOUNT->password,0));
    curl_easy_setopt(tinfo1,CURLOPT_POSTFIELDS,pf);
    curl_easy_setopt(tinfo1,CURLOPT_SSL_VERIFYPEER,0);
    curl_easy_setopt(tinfo1,CURLOPT_HEADERFUNCTION,tinfo_curl2memory_findcookie);
    curl_easy_setopt(tinfo1,CURLOPT_HEADERDATA,(void *)&tinfo1_header);
    curl_easy_setopt(tinfo1,CURLOPT_TIMEOUT,4L);
    CURLcode st;
    int count=0;
    while(count<3){
        st=curl_easy_perform(tinfo1);
        if(st!=CURLE_OK){
            _TINFO_ERRNO=202;
            count++;
        }else{
            _TINFO_ERRNO=0;
            break;
        }
        if(count==3){
            return NULL;
        }
    }
    curl_easy_cleanup(tinfo1);
    CURL * tinfo2=curl_easy_init();
    curl_easy_setopt(tinfo2,CURLOPT_URL,THU_SETTINGS);
    curl_easy_setopt(tinfo2,CURLOPT_USERAGENT,USER_AGENT);
    headers=curl_slist_append(headers,tinfo1_header.memory);
    curl_easy_setopt(tinfo2,CURLOPT_HTTPHEADER,headers);
    curl_easy_setopt(tinfo2, CURLOPT_WRITEFUNCTION,tinfo_curl2memory);
    curl_easy_setopt(tinfo2, CURLOPT_WRITEDATA, (void *)&tinfo2_body);
    st=curl_easy_perform(tinfo2);
    if(st!=CURLE_OK){
        _TINFO_ERRNO=202;
        return NULL;
    }
    if(strlen(tinfo2_body.memory)<10){
        _TINFO_ERRNO=203;
        return NULL;
    }
    char * tinfo_json=tinfo_body2json(tinfo2_body.memory);
    if(tinfo_json==NULL){
        _TINFO_ERRNO=207;
        return NULL;
    }
    json2account(tinfo_json,ret);
    curl_easy_cleanup(tinfo2);
    
    return ret;
}

int tinfo_clear(void * point){
    free(point);
    return 0;
}

char * tinfo_stringfy(struct THUINFO_ACCOUNT * ACCOUNT){
    char *ret=(char*)TINFO_MALLOC(STRING_STDSIZE);
    if(ret==NULL){
        return NULL;
    }
    sprintf(ret,"----学生信息----\n#姓名:%s\n#学院:%s\n#用户名:%s\n#学号:%s\n#手机号:%s\n---------------\n",
    ACCOUNT->real_name,ACCOUNT->department,ACCOUNT->user_name,ACCOUNT->user_id,ACCOUNT->phone_number);
    return ret;
}