
#include "../header.h"
void* thread_func(void* ptr)
{
    while(1)
    {
        int ret = getchar();
        printf("%c\n", ret);
    }
}
int main()
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread_func, NULL);

//    pthread_exit(0);
    return 0;
}
