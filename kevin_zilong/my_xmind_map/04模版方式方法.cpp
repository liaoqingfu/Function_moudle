#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//������
class Drink{
public:
	//��ˮ
	virtual void Boil() = 0;
	//����
	virtual void Brew() = 0;
	//���뱭��
	virtual void PourInCup() = 0;
	//��������
	virtual void AddSometing() = 0;


	//ģ�淽����ȷ��˳��
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
	//��ˮ
	virtual void Boil()
	{
		cout << "����Ȫˮ" << endl;
	}
	//����
	virtual void Brew()
	{
		cout << "����Ħ��" << endl;
	}
	//���뱭��
	virtual void PourInCup()
	{
		cout << "�������" << endl;
	}
	//��������
	virtual void AddSometing()
	{
		cout << "�ӵ���Ȼ" << endl;
	}
};

class Tea :public Drink
{
public:
	virtual void Boil()
	{
		cout << "�������ˮ" << endl;
	}
	//����
	virtual void Brew()
	{
		cout << "����������" << endl;
	}
	//���뱭��
	virtual void PourInCup()
	{
		cout << "����豭" << endl;
	}
	//��������
	virtual void AddSometing()
	{
		cout << "�ӵ�ţ��" << endl;
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

