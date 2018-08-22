
#include "../header.h"
int main()
{
    int uid = getuid();   // 真实账户
    int gid = getgid();     // 真实组
    int euid = geteuid();   // 有效账户
    int egid = getegid();   // 有效组
    printf("uid =%d, gid=%d, euid=%d, egid=%d\n", 
            uid, gid, euid, egid);

    int ret = setuid(1000);
    printf("ret is %d\n", ret);
    setgid(1000);
    seteuid(1000);
    setegid(1000);
}
