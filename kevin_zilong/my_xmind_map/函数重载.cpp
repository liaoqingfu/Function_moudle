#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

//函数重载是一种关系
//不同的函数有相同的函数名
//函数重载是为了解决取名字的烦恼
int add(int a, int b)
{
	return a + b;
}
double add(double a, double b)
{
	return a + b;
}

void test01()
{
	int a = 10;
	int b = 20;
	int ret = add(a, b);
}

/*
	前提：同一个作用域
	参数个数不同
	参数类型不同
	参数顺序不同
	三个条件只需满足一个条件，就可以进行重载
*/

//1.参数个数不同
void func(int a)
{
	cout << "func(int a)" << endl;
}
void func(int a, int b)
{
	cout << "func(int a, int b)" << endl;
}

void test001()
{
	//根据你传入的参数个数来决定调用哪个函数
	int a = 10;
	int b = 20;
	func(a);
	func(a, b);
}
//2.参数类型不同

void func2(int a)
{
	cout << "func2(int a)" << endl;
}
void func2(double a)
{

	cout << "func2(double a)" << endl;
}
void test002()
{
	//根据你传入的参数类型来决定调用哪个函数
	int a = 10;
	double b = 10.10;
	func2(a);
	func2(b);
}
//3.参数顺序不同
void func3(int a,double b)
{
	cout << "func3(int a,double b)" << endl;
}
void func3(double a, int b)
{
	cout << "func3(double a, int b)" << endl;

}
void test003()
{
	//根据你传入的参数类型来决定调用哪个函数
	int a = 10;
	double b = 20.20;
	func3(a, b); //(int,double)
	func3(b, a);//(double,int)

}

void func4(int a)
{
	cout << "func4(int a)" << endl;
}
void func4(double a)
{

	cout << "func4(double a)" << endl;
}
//void func4(char a)
//{
//	cout << "func4(char a)" << endl;
//}
void test004()
{
	//函数调用的时候，先严格的匹配，如果没有匹配的函数
	//在试着转换，转换成功就调用，不成功就报错
	char a = 'b';
	string name = "aaa";
	func4(a);//默认调用参数int类型的函数
}

void test02()
{
	func3(10,10.22);//为什么返回值不能做为函数重载的条件
					//因为在调用函数时，并没有函数的返回值
}

void myfunc(int a,int b=0)
{

}

void myfunc(int a)
{

}
void test03()
{
	//函数重载和函数有默认参数时的二义性问题
	int a = 10;
	//myfunc(a);
}

int main()
{
	test004();
	system("pause");
	return EXIT_SUCCESS;
}

