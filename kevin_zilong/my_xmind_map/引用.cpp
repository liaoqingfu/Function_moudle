#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


void test()
{
	int a = 10;
	int b = 20;
	//引用必须要引用一块合法的内存空间，引用必须初始化，除非做函数参数或者返回参数
	int &c = a; //引用，给a空间取别名为c   相当于 int * const c=&a  ,一个常指针，指向不能改变
	cout << "a=" << a << " c=" << c << endl; //10  10
	c = 100;   //改变空间内变量的值，a的值也改变了
	cout << "a=" << a << " c=" << c << endl; //100  100
	c = b; //只是赋值，将b的值给c，没有改变指向
	int &d = c;
	int &f = d;  //可以取多个别名
	f = 200;
	cout << "a=" << a << " c=" << c << endl;  //200  200
}

void test01()
{
	int a = 10;
	int *p = &a;
	int *&p1 = p;   //应用指针
	*p1 = 100;
	cout << a << " "<<*p <<" "<< *p1 << endl;  //100  100  100
	int arr[5] = { 1, 2, 3, 4, 5 };
	int(&buf)[5] = arr;   //数组的引用，方法一
	for (int i = 0; i < 5; ++i)
	{
		cout << buf[i]<<" ";  //1,2,3,4,5
	}
	cout << endl;
	typedef int(BUF)[5];  //给int[5]用typedef取别名
	BUF &buf2 = arr;    //方法二，数组的引用
	typedef int(&BUF3)[5];  //方法三，数组的引用
	BUF3 buf3 = arr;   //方法与上面是一样的

}

int &test02()
{
	//int a = 100;
	//int &p = a;
	//return p;  //返回局部引用，函数结束就释放了空间，返回了一个空的空间，不能返回局部引用
	static int a = 19;   //静态变量是全局的，这样返回才是可以的，返回后可以对空间进行操作
	int &p = a;
	return p;
}

void Swap(int a,int b)   //值传递，交换了a ，b ，但是程序调用完后，空间就释放了
{
	int tem = a;
	a = b;
	b = tem;
}

void Swap1(int *a, int *b)  //地址传递，直接操作内存空间，即使调用结束了，释放空间了，但是内存中的值已经间接交换了
{
	int tem = *a;
	*a = *b;
	*b = tem;
}
void Swap2(int &a, int &b)  //引用传递，给空间取别名，对空间内容直接交换
{
	int tem = a;
	a = b;
	b = tem;
}
void test03(char *&p) //指针引用
{
	p = (char *)malloc(64);
	memset(p, 0, 64);
	strcpy(p, "xiao");
}

void FuncFree(char *&p)   //记得释放申请的内存空间
{
	if (p != NULL)
	{
		free(p);
		p = NULL;
	}
	return;
}

int main()
{
	//test();
	//test01();
	int &x = test02();
	x = 18;
	cout << x << endl;  //18 
	cout << test02() << endl;  //18
	test02() = 188;   //返回引用后，函数可以当左值
	cout << x << " " << test02() << endl;  //188 188

	int k = 10;
	int bb = 12;
	const int &m = k;  //相当于 const int * const m=&k;
	//m = 20;  //m 不能被修改
	//m = bb;  //不能修改，也相当于赋值
	k = 100;
	cout << k << " " << m << endl;  //100  100
	int *p1 = (int *)&m;
	*p1 = 200;
	cout << k << " " << m << endl;  //可以通过指针间接修改

	//int &tem = 100;  //err 100是常量，不是合法的内存空间，不能把一个字面量赋给引用
	const int &tem1 = 100; //OK ,编译器会自动优化  相当于  int temp=100; int &tem1=temp;   
	//tem1 = 300;    //不能修改，const修饰的量为常量，不可修改，可以通过指针去间接修改
	int *p2 = (int *)&tem1;  //可以通过指针去间接修改
	*p2 = 120;
	cout << tem1 << endl;  //120

	int a = 15;
	int b = 16;
	Swap(a, b);
	cout << a << " "<<b << endl;   //15,16
	Swap1(&a, &b);
	cout << a << " " << b << endl;  //16,15
	Swap2(a, b);   //引用传递看起来更直观
	cout << a << " " << b << endl;  //15,16  ,交换回来了，上面是16，15

	char *p = NULL;
	test03(p);
	cout << p << endl;   //xiao
	FuncFree(p);
	return EXIT_SUCCESS;
}