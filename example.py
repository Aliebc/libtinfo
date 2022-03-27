#example.py
#for libtinfo
#by Aliebc

from getpass import *
from ctypes import *

try:
    tinfo=CDLL("./libtinfo.dylib")
except Exception as e:
    print(str(e))
    exit(1)

class TINFO_USER(Structure):
    _fields_ =[
        ('username',c_char_p),
        ('password',c_char_p)
    ]

class TINFO_ACCOUNT(Structure):
    _fields_ =[
        ('real_name',c_char_p),
        ('user_name',c_char_p),
        ('department',c_char_p),
        ('phone_number',c_char_p),
        ('user_id',c_char_p)
    ]

print("--libtinfo on python--")
usr=input("请输入INFO账户名:")
pwd=getpass(prompt="请输入INFO密码:")
test_u=TINFO_USER()
test_u.username=c_char_p(bytes(usr,"utf-8"))
test_u.password=c_char_p(bytes(pwd,"utf-8"))

acco=POINTER(TINFO_ACCOUNT)
verify=tinfo.tinfo_verify
verify.restype=POINTER(TINFO_ACCOUNT)
acco=verify(pointer(test_u))

if not acco:
    print("验证失败,错误原因:",end="")
    serr=tinfo.tinfo_strerrno
    serr.restype=c_char_p
    print(serr(None).decode("utf-8"))
else:
    print("验证成功!")
    stringfy=tinfo.tinfo_stringfy
    stringfy.restype=c_char_p
    res=stringfy(acco)
    print(res.decode("utf-8"))