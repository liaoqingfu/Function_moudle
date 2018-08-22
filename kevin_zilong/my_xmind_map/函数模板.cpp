#include<iostream>
#include<string>
using namespace std;

template<class T>  //没有分号，template模板关键字，class表示任何类型,T是自己取的名字，可以取别的名字，T用的多
//template<typename T>  //用typename和class是一样的
//告诉编译器下面函数中有T的话不要报错，下面紧跟的第一个函数为函数模板
void mySwap(T &a, T &b)
{
	T tem = a;
	a = b;
	b = tem;
}

void test()
{
	int a = 10;
	int b = 20;
	mySwap(a, b);
	cout << "a=" << a << " b=" << b << endl;  //a=20,b=10

	char c = 'A';
	char d = 'X';
	mySwap(c, d);   //可以传任意参数的变量,传入的数据类型必须和指定的一致
	cout << "c=" << c << " d=" << d << endl;  //c=X,d=A
	//mySwap(a, c);  //会报错，传的参数必须要一样，因为函数的模板参数是一样的，调用函数模板，严格匹配类型，类型要统一，不然自动推导类型有二义性
	//mySwap<int>(c, d); //会报错，显示的指定传入的参数类型必须与传入的参数类型一致
	double x1 = 10.3;
	double x5 = 16.8;
	//mySwap<int>(x1, x5); //会报错，显示的指定传入的参数类型必须与传入的参数类型一致
	
}

//调用过程是这样的,先将函数模板实再化为函数,然后再发生函数调用。
template<class T>
void func(T a,T b)
{
	cout << "模板函数" << endl;
}
//函数模板不允许自动类型转换，必须严格匹配类型
//普通函数可以进行自动类型转换

void func(int a,int b)
{
	cout << "普通函数" << endl;
}

void Max(int a, int b)
{
	cout << "void Max(int a, int b)" << endl;
}
template<class T>
void Max(T a, T b)
{
	cout << "void Max(T a, T b)" << endl;
}
template<class T>
void Max(T a, T b, T c)
{
	cout << "void Max(T a, T b, T c)" << endl;
}
template<class T1,class T2>   //可以传不同的参数
void funcc3(T1 a,T2 b)
{
	cout << "a=" << a << " b=" << b << endl;
}

int main()
{
	int a = 10;
	int b = 20;
	test();
	func(a,b);   //优先调用普通函数
	func<>(a, b);  //强制调用函数模板，使用空参数列表
	char c1 = 'a';
	char c2 = 'b';
	func(c1, c2);  //如果函数模版可以产生更好的匹配，那么使用函数模版

	Max(3.0, 4.0);
	Max(5.0, 6.0,7.0); //函数模板也可以重载
	Max('a', 100);  //调用普通函数，可以隐形类型转换

	int k = 19;
	string ch = "xiao";
	funcc3(ch,k);    //打印a=xiao,b=19  可以传不同的参数，只要与调用函数匹配就行
	return EXIT_SUCCESS;
}