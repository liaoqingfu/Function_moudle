
int main()
{
    write(STDOUT_FILENO, "hello", 5);

    int fd = open("a.txt", O_CREAT|O_WRONLY);
    // 删除a.txt，没有真正删除a.txt，但是硬盘已经看不见
    unlink("a.txt");

    // 当进程退出时，a.txt会因为fd的关闭而自动删除
}
