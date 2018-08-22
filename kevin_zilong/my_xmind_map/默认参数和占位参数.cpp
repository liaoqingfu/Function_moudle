#include<iostream>
using namespace std;
//void test01(int a, int b , int c);  //没写默认参数，传参的时候得传三个参数
void test01(int a, int b = 10, int c = 9);
//函数的默认参数从左向右，如果一个参数设置了默认参数，那么这个参数之后的参数都必须设置默认参数。
//void test(int a, int b, int c = 0)
//void test(int a, int b, int c = 0,int d) //err 后面的参数必须要为默认参数,不设置参数的时候以默认参数为主，设置了参数就以设置的参数为准

void test(int a, int b, int c = 0)  //默认参数中函数重载的时候，注意函数的二义性
{
	int sum = a + b + c;
	cout << sum << endl;  
}

void test03(int a, int)  //占位参数也必须传值，函数内部无法使用占位参数
{
	cout << a << endl;
}
//void test03(int a, int b, int = 0)  //调用的时候会与上面test03（）函数造成二义性
//{
//
//}
void test04(int a, int b, int = 0)
{
	cout << a + b << endl;
}

int main()
{
	test(1, 2);  //3
	test(2, 3, 4);//9
	test01(0);  //19
	test01(1, 2); //12
	test01(1, 2, 3); //6

	test03(10,10);//10
	test04(12, 12); //24
	test04(1, 2, 3);//3
	return EXIT_SUCCESS;
}

//如果函数声明和函数定义分开写，函数声明和函数定义不能同时设置默认参数,默认参数最好写在声明里面，不然传参的时候必须传三个参数
//void test01(int a, int b = 10, int c = 9)  //需要声明
void test01(int a, int b, int c )  //需要声明
{
	int sum = a + b + c;
	cout << sum << endl;
}