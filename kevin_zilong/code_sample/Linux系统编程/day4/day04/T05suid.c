
#include "../header.h"
int main()
{
    uid_t uid = getuid(); // 进程所属用户:执行它的用户
    gid_t gid = getgid(); // 进程所属组：执行它的用户所属的组
    uid_t euid = geteuid(); // 进程所属有效用户：进程拥有的权限用户
    gid_t egid = getegid(); // 进程所属有效组：进程拥有的权限组

    printf("uid=%d, gid=%d, euid=%d, egid=%d\n", (int)uid,
            (int)gid, (int)euid, (int)egid);
}
