#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//问题一
#define ADD(x,y) x+y

inline int add(int x, int y)//向编译器【申请】为内联函数
{
	return x + y;
}
//内联函数本身是一个函数
//如果让函数变为内联函数，编译器可能让你通过申请，可能不通过
//如果编译器觉得你这个函数可以成为内联函数，不要你申请都可能成为内联函数
//类的成员函数都申请了成为内联函数

void test01()
{
	//10+20*10
	int ret = ADD(10, 20) * 10;//210
	cout << "ret=" << ret << endl;
	int ret2 = add(10, 20) * 10;
	cout << "ret2=" << ret2 << endl;  //300

}
//问题二
#define FUNC(x,y) ((x)<(y)?(x):(y))



void test02()
{
	int a = 1;
	int b = 3;
	//((++a)<(b)?(++a):(b))
	cout << "func=" << FUNC(++a, b) << endl;// 3
}

int main()
{
	//test01();
	test02();
	system("pause");
	return EXIT_SUCCESS;
}

