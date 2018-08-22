#include<iostream>
#include<string>
using namespace std;
//单列模式：将默认构造函数和默认拷贝函数都私有化
//定义一个静态的指针变量，类型为类的类型
//提供一个获取该类型的方法,需要公有权限，静态变量，只能静态函数来接
//要在类外初始化该静态变量
class Kevin{  //这样就出现了单列模式了
private:
	Kevin()
	{

	}
	Kevin(const Kevin &kev)
	{

	}
public:
	static Kevin *get()
	{
		cout << "sfsa" << endl;
		return p;
	}
private:
	static Kevin *p;
};

Kevin *Kevin::p = new Kevin;  //类内声明，类外定义

void test()
{
	Kevin *p = Kevin::get();
	Kevin *p1 = Kevin::get();
	p->get();
	if (p == p1)     //定义的指针都是一样的，只能实例化一个对象
	{
		cout << "ssdfsdf" << endl;
	}
}

class print{
private:
	print()
	{
		num = 0;
	}
	print(const print& pri)
	{

	}
public:
	void printp(string ch)
	{
		cout << "打印的内容:" << ch << endl;
		num++;
	}
	int GetU()
	{
		return num;
	}
	static print *prr()
	{
		return pri;
	}
private:
	static print *pri;
	int num;
};

print *print::pri = new print;

void test01()
{
	print *p1 = print::prr();
	print *p2 = print::prr();
	p1->printp("xiao");
	p2->printp("long");
}

int main()
{
	//test();
	test01();
	return EXIT_SUCCESS;
}