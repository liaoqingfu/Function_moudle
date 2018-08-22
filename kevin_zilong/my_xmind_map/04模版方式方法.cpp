#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//抽象类
class Drink{
public:
	//煮水
	virtual void Boil() = 0;
	//冲泡
	virtual void Brew() = 0;
	//导入杯中
	virtual void PourInCup() = 0;
	//加入佐料
	virtual void AddSometing() = 0;


	//模版方法，确定顺序
	void Make()
	{
		Boil();
		Brew();
		PourInCup();
		AddSometing();
	}
	

};

class Coffee :public Drink{
public:
	//煮水
	virtual void Boil()
	{
		cout << "煮点矿泉水" << endl;
	}
	//冲泡
	virtual void Brew()
	{
		cout << "冲泡摩卡" << endl;
	}
	//导入杯中
	virtual void PourInCup()
	{
		cout << "倒入缸中" << endl;
	}
	//加入佐料
	virtual void AddSometing()
	{
		cout << "加点孜然" << endl;
	}
};

class Tea :public Drink
{
public:
	virtual void Boil()
	{
		cout << "煮点自来水" << endl;
	}
	//冲泡
	virtual void Brew()
	{
		cout << "冲泡铁观音" << endl;
	}
	//导入杯中
	virtual void PourInCup()
	{
		cout << "倒入茶杯" << endl;
	}
	//加入佐料
	virtual void AddSometing()
	{
		cout << "加点牛奶" << endl;
	}
};

void test()
{
	Drink *dri = NULL;
	dri = new Coffee;

	dri->Make();
	delete dri;

	cout << "------------" << endl;

	dri = new Tea;
	dri->Make();
	delete dri;

}


int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

