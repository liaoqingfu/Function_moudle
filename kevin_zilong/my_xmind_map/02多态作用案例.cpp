#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//有纯虚函数的类叫做抽象类
class rule{
public:
	//virtual int getRusult(int a, int b){ return 0; }//虚函数
	~rule()
	{
		cout << "rule" << endl;
	}

	virtual int getRusult(int a, int b) = 0;//纯虚函数
	//有纯虚函数的类，叫做抽象类，抽象类不能实例化对象
	
};

class plus_rule:public rule{//继承
public:
	virtual int getRusult(int a, int b)//重写父类的虚函数
	{
		return a + b;
	}
	~plus_rule()
	{
		cout << "plus_rule" << endl;
	}
};

class minus_rule :public rule{
public:
	virtual int getRusult(int a, int b)//重写父类的虚函数
	{
		return a - b;
	}
};

class Maker :public rule{
	//如果继承了抽象类，而没去重新抽象类中的纯虚函数，那么子类也变成抽象类，不能实例化对象

};

void test01()
{
	//Maker m;
}

int func(rule *rul)
{
	int a = 10;
	int b = 20;
	int ret = rul->getRusult(a, b);

	return ret;
}

void test()
{
	rule *rul = NULL;
	rul = new plus_rule;

	cout << func(rul) << endl;

	delete rul;


	rul = new minus_rule;


	cout << func(rul) << endl;

	delete rul;

	//rule rull;//有纯虚函数的类，叫做抽象类，抽象类不能实例化对象
}



int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

