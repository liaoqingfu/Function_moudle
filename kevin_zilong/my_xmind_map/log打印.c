#include <stdio.h>

int main(void)
{
	printf("%s\n%s\n%d\n%s\n%s\n", __func__, __FILE__,__LINE__,__DATE__,__TIME__);  //函数名，函数页名字，行号，日期，时间

	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include<stdarg.h>

#define	TRACE()		do { fprintf(stderr, "%s     ******  %s() <%d> \n", __FILE__,__func__, __LINE__ ); }while(0)

#define mylog(fmt,...) test03(__FILE__,__func__,__LINE__,fmt,__VA_ARGS__)  //宏定义一个多参数函数
#define mylog1(fmt,...) test02(__FILE__,__func__,__LINE__,fmt)  //宏定义一个多参数函数
#define	log(fmt)		do { fprintf(stderr, "%s     ******  %s() <%d> :%s\n", __FILE__,__func__, __LINE__,fmt); }while(0)

void test01(int count,...)//多参数函数
{
    int temp=0;
    int i=0;
    va_list ap;  //定义一个参数列表
    va_start(ap,count); //参数列表从count参数开始，但是不包括count参数
    for(i=0;i<count;++i)//循环count次
    {
        temp+=va_arg(ap,int);//将参数列表后面传入的不定参数拿出来，然后加起来，第一个参数不计入
    }
    printf("%d\n",temp);//25
    va_end(ap);//结束列表
}

void test02(const char *p,...)//参数列表不定义死，传参随意
{
    //const char *arg = p;
    va_list ap; //定义一个参数列表
    va_start(ap,p);//参数列表从p参数开始，不包括p参数列表从

    while(1)
    {
        p=va_arg(ap,const char *);//获取p后面的参数
        printf("%s",p);//然后打印参数
        if(p == NULL)//如果参数为空，跳出循环
        {
            break;
        }
    }
    printf("\n");
    va_end(ap);//结束参数列表
}

void test03(const char *filename,const char *funcname,int fileline,const char *fmt,...)
{
    printf("[%s:%s:%d] ",filename,funcname,fileline);//打印文件名字和行号
    va_list ap; //定义一个参数列表
    va_start(ap,fmt); //指定开始位置,不包括fmt参数位置
    vprintf(fmt,ap); //打印参数列表
    va_end(ap);//结束参数列表
	printf("\n");
}

int main()
{
	printf("hello oceawing\n");
	log("main start");
	test01(3,8,8,9);//25
	test02(NULL,"kevin-","zi ","long",NULL);
	int a = 10;
	int b = 9;
	mylog("main end :%d",(a+ b));
	printf("a + b = %d\n",(a+ b));
	mylog("10 + 9 = %",(a + b));
	log("main end");
	
	return 0;
}