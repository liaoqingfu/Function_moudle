#include<iostream>
#include<string>
using namespace std;
//����ģʽ����Ĭ�Ϲ��캯����Ĭ�Ͽ���������˽�л�
//����һ����̬��ָ�����������Ϊ�������
//�ṩһ����ȡ�����͵ķ���,��Ҫ����Ȩ�ޣ���̬������ֻ�ܾ�̬��������
//Ҫ�������ʼ���þ�̬����
class Kevin{  //�����ͳ����˵���ģʽ��
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

Kevin *Kevin::p = new Kevin;  //�������������ⶨ��

void test()
{
	Kevin *p = Kevin::get();
	Kevin *p1 = Kevin::get();
	p->get();
	if (p == p1)     //�����ָ�붼��һ���ģ�ֻ��ʵ����һ������
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
		cout << "��ӡ������:" << ch << endl;
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