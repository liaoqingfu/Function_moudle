

// int open(const char* filename, int flag, ...)

int main()
{
    int fd = open("a.txt", O_WRONLY);

    int fd = open("b.txt", O_WRONLY | O_CREAT, 0777);
}
