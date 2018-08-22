C++中的内存泄露一般指堆中的内存泄露。堆内存是我们手动malloc/realloc/new申请的，程序不会自动回收，需要调用free或delete手动释放，否则就会造成内存泄露。内存泄露其实还应该包括系统资料的泄露，比如socket连接等，使用完后也要释放。
内存泄露的原因：
总结下来，内存泄露大概有一下几个原因：
1、编码错误：malloc、realloc、new申请的内存在堆上，需要手动显示释放，调用free或delete。申请和释放必须成对出现malloc/realloc对应free，new对应delete。前者不会运行构造/析构函数，后者会。对于C++内置数据类型可能没差别，但是对于自己构造的类，可能在析构函数中释放系统资源或释放内存，所以要对应使用。
2、“无主”内存：申请内存后，指针指向内存的起始地址，若丢失或修改这个指针，那么申请的内存将丢失且没有释放。
3、异常分支导致资源未释放：程序正常执行没有问题，但是如果遇到异常，正常执行的顺序或分支会被打断，得不到执行。所以在异常处理的代码中，要确保系统资源的释放。
4、隐式内存泄露：程序运行中不断申请内存，但是直到程序结束才释放。有些服务器会申请大量内存作为缓存，或申请大量Socket资源作为连接池，这些资源一直占用直到程序退出。服务器运行起来一般持续几个月，不及时释放可能会导致内存耗尽。
5、类的析构函数为非虚函数：析构函数为虚函数，利用多态来调用指针指向对象的析构函数，而不是基类的析构函数。

1. 分配完内存之后忘了回收； 
2. 程序Code有问题，造成没有办法回收； 
3. 某些API函数操作不正确，造成内存泄漏

内存泄露的检测
内存泄露的关键就是记录分配的内存和释放内存的操作，看看能不能匹配。跟踪每一块内存的声明周期，例如：每当申请一块内存后，把指向它的指针加入到List中，当释放时，再把对应的指针从List中删除，到程序最后检查List就可以知道有没有内存泄露了。Window平台下的Visual Studio调试器和C运行时（CRT）就是用这个原理来检测内存泄露。
在VS中使用时，需加上
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
crtdbg.h的作用是将malloc和free函数映射到它们的调试版本_malloc_dbg和_free_dbg，这两个函数将跟踪内存分配和释放（在Debug版本中有效）
_CrtDumpMemoryLeaks();
函数将显示当前内存泄露，也就是说程序运行到此行代码时的内存泄露，所有未销毁的对象都会报出内存泄露，因此要让这个函数尽量放到最后。
例如：
[cpp] view plain copy

	1. #define _CRTDBG_MAP_ALLOC  
	2. #include <crtdbg.h>  
	3. #include <iostream>  
	4. using namespace std;  
	5. int main(int argc,char** argv)  
	6. {  
	7.     char *str1 = NULL;  
	8.     char *str2 = NULL;  
	9.     str1=new char[100];  
	10.     str2=new char[50];  
	11.   
	12.     delete str1;  
	13.     _CrtDumpMemoryLeaks();  
	14.     return 0;  
	15. }  


上述代码中，内存申请了两块，但是只释放了一块，运行调试，会在output窗口输出：
Dumping objects ->
{136} normal block at 0x00084D70, 50 bytes long.
 Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD 
Object dump complete.
可以看到会检测到内存泄露。 但是并没有检测到泄露内存申请的位置，已经加了宏定义#define _CRTDBG_MAP_ALLOC。原因是申请内存用的是new，而刚刚包含头文件和加宏定义是重载了malloc函数，并没有重载new操作符，所以要自己定义重载new操作符才能检测到泄露内存的申请位置。修改如下：
[cpp] view plain copy

	1. #define _CRTDBG_MAP_ALLOC  
	2. #include <crtdbg.h>  
	3. #ifdef _DEBUG //重载new  
	4. #define new  new(_NORMAL_BLOCK, __FILE__, __LINE__)    
	5. #endif  
	6. #include <iostream>  
	7. using namespace std;  
	8. int main(int argc,char** argv)  
	9. {  
	10.     char *str1 = NULL;  
	11.     char *str2 = NULL;  
	12.     str1=(char*)malloc(100);  
	13.     str2=new char[50];  
	14.   
	15.     _CrtDumpMemoryLeaks();  
	16.     return 0;  
	17. }  

运行结果：
Detected memory leaks!
Dumping objects ->
e:\c++\test\内存泄露检测2\main.cpp(13) : {62} normal block at 0x001714F8, 50 bytes long.
 Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD
e:\c++\test\内存泄露检测2\main.cpp(12) : {61} normal block at 0x00171458, 100 bytes long.
 Data: <                > CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD CD
Object dump complete.
可以看到
main.cpp()括号里面的数字就是泄露内存的起始位置。那么后面的{62} normal block at 0x001714F8, 50 bytes long.
代表什么？
大括号{}里面的数字表示第几次申请内存操作；0x001714F8表示泄露内存的起始地址，CD CD表示泄露内存的内容。
为什么是第62次申请内存，因为在初始化操作时也申请了内存。通过这个信息，可以设置断点。调用long _CrtSetBreakAlloc(long nAllocID)可以再第nAllocID次申请内存是中断，在中断时获取的信息比在程序终止时获取的信息要多，你可以调试，查看变量状态，对函数调用调试分析，解决内存泄露。
block分为3中类型，此处为normal，表示普通，此外还有client表示客户端（专门用于MFC），CRT表示运行时（有CRT库来管理，一般不会泄露），free表示已经释放掉的块，igore表示要忽略的块。
在上面程序中，调用_CrtDumpMemoryLeaks()来检测内存泄露，如果程序可能在多个地方终止，必须在多个地方调用这个函数，这样比较麻烦，可以在程序起始位置调用_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF )，这样无论程序何时终止，都会在终止前调用_CrtDumpMemoryLeaks()。
 
除此之外，还可以在某时刻设置检查点，获取当时内存状态的快照。比较不同时刻内存状态的差异。
[cpp] view plain copy

	1. #define _CRTDBG_MAP_ALLOC  
	2. #include <crtdbg.h>  
	3. #ifdef _DEBUG //重载new  
	4. #define new  new(_NORMAL_BLOCK, __FILE__, __LINE__)    
	5. #endif  
	6. #include <iostream>  
	7. using namespace std;  
	8. int main(int argc,char** argv)  
	9. {  
	10.     _CrtMemState s1, s2, s3;  
	11.     char *str1 = NULL;  
	12.     char *str2 = NULL;  
	13.     str1=(char*)malloc(100);  
	14.     _CrtMemCheckpoint( &s1 );//记录内存快照  
	15.     _CrtMemDumpStatistics( &s1 );//输出  
	16.     str2=new char[50];  
	17.     _CrtMemCheckpoint( &s2 );  
	18.     _CrtMemDumpStatistics( &s2 );  
	19.   
	20.     if ( _CrtMemDifference( &s3, &s1, &s2) )//比较s1和s2，把比较结果输出到s3  
	21.         _CrtMemDumpStatistics( &s3 );// dump 差异结果  
	22.   
	23.     return 0;  
	24. }  


输出结果为：
0 bytes in 0 Free Blocks.
100 bytes in 1 Normal Blocks.
8434 bytes in 54 CRT Blocks.
0 bytes in 0 Ignore Blocks.
0 bytes in 0 Client Blocks.
Largest number used: 8963 bytes.
Total allocations: 14003 bytes.
0 bytes in 0 Free Blocks.
150 bytes in 2 Normal Blocks.
8434 bytes in 54 CRT Blocks.
0 bytes in 0 Ignore Blocks.
0 bytes in 0 Client Blocks.
Largest number used: 8963 bytes.
Total allocations: 14053 bytes.
0 bytes in 0 Free Blocks.
50 bytes in 1 Normal Blocks.
0 bytes in 0 CRT Blocks.
0 bytes in 0 Ignore Blocks.
0 bytes in 0 Client Blocks.
Largest number used: 0 bytes.
Total allocations: 50 bytes.

也可以用此法更复杂检测内存泄露，例如设置检查点，检查检查点之间的内存泄露。
在Linux下也有类似的方法，具体可以参考：http://en.wikipedia.org/wiki/Mtrace


linux下检查内存泄露的工具--mtrace
最令linux程序员头疼的莫过于内存泄露了，即使你是在优秀的程序员，你也不能保证所以的malloc操作都有对应的free，那必要的工具就是必不可少的了。在一般的linux发行版中，有一个自带的工具可以很方便的替你完成这些事，这个工具就是mtrace。
 
下面是它的用法
[cpp] view plain copy

	1. #include <stdio.h>  
	2. #include <stdlib.h>  
	3. #include <string.h>  
	4.    
	5. #include <mcheck.h>  
	6.    
	7.    
	8. int main(){  
	9.     setenv("MALLOC_TRACE", "output", 1);  
	10.     mtrace();  
	11.    
	12.     char * text = ( char * ) malloc (sizeof(char) * 100);  
	13.     memset(text,'/0',100);  
	14.     memcpy(text,"hello,world!",12);  
	15.    
	16.     printf("%s/n",text);  
	17.     return 0;  
	18. }  

 
可以看出，只需要在你的程序中插入三行代码，就行。
第一句，#include <mcheck.h>，包含头文件
第二句，setenv("MALLOC_TRACE", "output", 1);output表示输出的中间文件
第三句，mtrace()，调用mtrace.
 
将这个文件编译，注意，编译的时候一地要加上gcc的-g选项。
gcc mtrace_test.c -g -o mtrace_test
接着执行可执行文件，然后你会发现当前目录下多了一个output的文件。
这个文件自然不是文本文件，所以需要工具来查看。
 
$ mtrace mtrace_test output
 
这时会打印出一串信息
- 0x000000000129a010 Free 3 was never alloc'd 0x7f62844d89ba
- 0x000000000129a100 Free 4 was never alloc'd 0x7f6284583a1d
- 0x000000000129a120 Free 5 was never alloc'd 0x7f62845d20ec
 
Memory not freed:
-----------------
           Address     Size     Caller
0x000000000129a5a0     0x64  at /home/dianping/peter/mtrace/mtrace_test.c:12
 
其它别的直接忽略，注意那句要命的 Memory not freed，caller表示那段代码对应的malloc操作没有释放。顺便说一句，如果你编译的时候没有使用-g的选项，那么Caller这个地方就不会出现代码的信息，而是一串二进制的地址信息。
 
另外还有两个工具据说也不错dmalloc和memwatch

使用mtrace进行内存泄漏检查
原创 2014年11月16日 10:16:50

1.确保你的系统是支持GNU的头文件<mcheck.h>
[html] view plain copy

	1. #include <mcheck.h>  


2. 代码中添加mtrace(), muntrace() 函数
[html] view plain copy

	1. int main(int argc, char* argv[])  
	2. {  
	3. #ifdef  MEMORY_CHECK  
	4.     mtrace();  
	5. #endif   
	6.   
	7. ... // other codes  
	8.   
	9. #ifdef  MEMORY_CHECK  
	10.     muntrace();  
	11. #endif   
	12. }  


3. 编译代码
[html] view plain copy

	1. gcc -g  xxx.c  -o xxx  

必须使用 -g 选项，否则不能打开 mtrace()

4. 设置内存跟踪的保存路径，并运行代码
[html] view plain copy

	1. export MALLOC_TRACE=xxx.log  

[html] view plain copy

	1. ./xxx  


5. 分析程序运行时保留的结果
[html] view plain copy

	1. mtrace xxx  xxx.log  


实例如下
sean@ubuntu:~$ cat test_mcheck.c 
#include <mcheck.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    void * ptr;

    mtrace();
    ptr = malloc(100);
    muntrace();

    return 0;
}
sean@ubuntu:~$ 
sean@ubuntu:~$ gcc -g test_mcheck.c -o test
sean@ubuntu:~$ export MALLOC_TRACE=./test_log
sean@ubuntu:~$ ./test 
sean@ubuntu:~$ mtrace test test_log 


Memory not freed:
-----------------
           Address     Size     Caller
0x0000000001457460     0x64  at /home/sean/test_mcheck.c:8
sean@ubuntu:~$ 



