#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//1.��̬��Ա������������������������������������
//2.��̬��Ա�������������������������ʼ��
//3.��̬��Ա�����࣬�����ڶ��������ж�����ĳ�Ա
//4.��̬��ԱҲ��Ȩ�ޣ����Ϊ˽�У������ǲ��ܷ���
//5.��̬��Ա����������Է��ʣ���Ҳ���Է���
//6.��̬��Ա������Ϊ�˹���ͷ�װ��̬��Ա����,��̬��Ա�������ܷ�����ͨ��Ա����
//7.��ͨ��Ա�������Է��ʾ�̬��Ա����


class Maker{
public:
	static int a;//��̬��Ա����

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

//��̬��ԱҲ��Ȩ��
class Maker2{
private:
	static int a2;//��̬��Ա����

	int b2;

};


int Maker2::a2 = 200;
void test02()
{
	//cout<<Maker2::
}

//��̬��Ա����
class Maker3{
private:
	static int a3;//��̬��Ա����

	int b2;

	static void setA(int a)//��̬��Ա����
	{
		//b2;//��̬��Ա�������ܷ�����ͨ��Ա����
		a3 = a;
	}
	void getA()
	{
		a3;//��ͨ��Ա�������Է��ʾ�̬��Ա����
	}

};

int Maker3::a3 = 300;

void test03()
{
	Maker3 maker3;
}

class Maker4{
public:
	const static int a4=100;//const���εľ�̬��Ա������������ڳ�ʼ��
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

