#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
//����һ
#define ADD(x,y) x+y

inline int add(int x, int y)//������������롿Ϊ��������
{
	return x + y;
}
//��������������һ������
//����ú�����Ϊ������������������������ͨ�����룬���ܲ�ͨ��
//�������������������������Գ�Ϊ������������Ҫ�����붼���ܳ�Ϊ��������
//��ĳ�Ա�����������˳�Ϊ��������

void test01()
{
	//10+20*10
	int ret = ADD(10, 20) * 10;//210
	cout << "ret=" << ret << endl;
	int ret2 = add(10, 20) * 10;
	cout << "ret2=" << ret2 << endl;  //300

}
//�����
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

