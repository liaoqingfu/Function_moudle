
#include <unistd.h>
#include <stdio.h>
int main(int argc, char* argv[])
{
    pid_t pid = fork();
    if(pid > 0) // parent  
    {

    }
    else if(pid == 0) // child
    {
        execl("./helloworld", "./helloworld", NULL);
    }
    else // pid < 0 error
    {
        
    }
        
    printf("hello world1\n");
}
