#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//�д��麯���������������
class rule{
public:
	//virtual int getRusult(int a, int b){ return 0; }//�麯��
	~rule()
	{
		cout << "rule" << endl;
	}

	virtual int getRusult(int a, int b) = 0;//���麯��
	//�д��麯�����࣬���������࣬�����಻��ʵ��������
	
};

class plus_rule:public rule{//�̳�
public:
	virtual int getRusult(int a, int b)//��д������麯��
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
	virtual int getRusult(int a, int b)//��д������麯��
	{
		return a - b;
	}
};

class Maker :public rule{
	//����̳��˳����࣬��ûȥ���³������еĴ��麯������ô����Ҳ��ɳ����࣬����ʵ��������

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

	//rule rull;//�д��麯�����࣬���������࣬�����಻��ʵ��������
}



int main()
{
	test();
	system("pause");
	return EXIT_SUCCESS;
}

