
// 坏处：编译出来的程序大
int c[1024] = {1} // data : DATA

int b[1024] // data : BSS
int main()
{
    for(int i=0; i<1024; ++i)
        b[i] = 1;

    char* q = "hello world"; // "hello world"
    q[0] = 'H';

    int* p = 0;
    *p = 100; // 对0地址进行写操作，段错误
}
