#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//1.静态成员变量的生命周期是整个程序，作用域在类内
//2.静态成员变量必须在类内声明，类外初始化
//3.静态成员属于类，不属于对象，是所有对象共享的成员
//4.静态成员也有权限，如果为私有，类外是不能访问
//5.静态成员变量对象可以访问，类也可以访问
//6.静态成员函数是为了管理和封装静态成员变量,静态成员函数不能访问普通成员变量
//7.普通成员函数可以访问静态成员变量


class Maker{
public:
	static int a;//静态成员变量

	int b;

};

int Maker::a = 100;

void test01()
{
	Maker maker1;
	Maker maker2;

	cout << Maker::a << endl;
	cout << maker1.a << endl;
	cout << maker2.a << endl;
}

//静态成员也有权限
class Maker2{
private:
	static int a2;//静态成员变量

	int b2;

};


int Maker2::a2 = 200;
void test02()
{
	//cout<<Maker2::
}

//静态成员函数
class Maker3{
private:
	static int a3;//静态成员变量

	int b2;

	static void setA(int a)//静态成员函数
	{
		//b2;//静态成员函数不能访问普通成员变量
		a3 = a;
	}
	void getA()
	{
		a3;//普通成员函数可以访问静态成员变量
	}

};

int Maker3::a3 = 300;

void test03()
{
	Maker3 maker3;
}

class Maker4{
public:
	const static int a4=100;//const修饰的静态成员变量最好在类内初始化
	const static int a44;

	int b2;

};

const int Maker4::a44 = 500;

void test04()
{
	cout << Maker4::a4 << endl;
	cout << Maker4::a44 << endl;
}

int main()
{
	//test01();
	test04();
	system("pause");
	return EXIT_SUCCESS;
}

