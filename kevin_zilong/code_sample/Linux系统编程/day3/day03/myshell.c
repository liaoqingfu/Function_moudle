
#include "../header.h"

extern char *get_current_dir_name(void);

//char* builtins[] = {"cd", "exit", NULL};
//
typedef struct builtin_handle 
{
    char* cmd;
    void (*handle)(char* args[]);
} builtin_handle;

void handle_cd(char* args[])
{
    if(args[1] == NULL)
        chdir("~");


    printf("cd dest: %s\n", args[1]);
    if(chdir(args[1]) == -1)
    {
        perror("cd");
    }
}

void handle_exit(char* args[])
{
    exit(1);
}

builtin_handle handles[] = {
    {"cd", handle_cd},
    {"exit", handle_exit},
    {NULL, NULL}
};

char* get_cmd_output(const char* cmd)
{
    static char buf[4096];
    // fopen  <-->  popen
    FILE* fp = popen(cmd, "r");
    int ret = fread(buf, 1, 1024, fp);
    pclose(fp);
    buf[ret-1] = 0; // 字符串的结束标记
    return buf;
}

int handle_builtin(char* args[])
{
    int i;
    for(i=0; ;++i)
    {
        if(handles[i].cmd == NULL) break;

        if(strcmp(handles[i].cmd, args[0]) ==0)
        {
            handles[i].handle(args);
            return 1;
        }
    }

    return 0;
}


// ls
// ls -al
// cd /
// ls -a -l
void handle_cmd(char* buf)
{
    // 把命令按照空格拆分，保存字符串数组，原因是因为execvp需要这样的数组
    char* args[1024];
    int index = 0;
    args[index++] = strtok(buf, " ");

    while(args[index++] = strtok(NULL, "")) ;

    // 判断命令是否内置命令，如果是内置命令，则直接处理，不需要fork
    if(handle_builtin(args)) 
    {
        printf("handle builtin\n");
        return;
    }

    // 如果是外部命令，需要fork之后，再执行exec去执行
    pid_t pid = fork();
    if(pid == 0)
    {
        execvp(args[0], args);
        printf("cmd %s not found\n", args[0]);
        exit(1); // exit 
    }
    else
    {
        wait(NULL);
    }
}


int main()
{
    char buf[4096];
    // username@machine-name:current_work_path$ 
    while(1)
    {
        // 获取账户名，机器名，当前工作目录，为了显示命令行提示
        char* username = strdup(get_cmd_output("whoami"));
        char* hostname = strdup(get_cmd_output("cat /etc/hostname"));
        char* cwd = get_current_dir_name();
        printf("%s@%s:%s$ ", username, hostname, cwd);

        // 等待用户输入，fgets是阻塞函数，当用户不输入命令时，这个进程阻塞
        fgets(buf, sizeof(buf), stdin); 
        buf[strlen(buf)-1] = 0;
        if(strlen(buf) == 0) 
        {
            free(username);
            free(hostname);
            free(cwd);
            continue;
        }

        // ls
        // ls -al
        // cd /
        // ps aux | grep a.out  xxxxx
        // ls > a.txt           xxxxxx
        // 命令处理函数
        handle_cmd(buf);

        free(username);
        free(hostname);
        free(cwd);
    }
}
