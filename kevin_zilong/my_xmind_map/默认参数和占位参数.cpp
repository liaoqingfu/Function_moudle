#include<iostream>
using namespace std;
//void test01(int a, int b , int c);  //ûдĬ�ϲ��������ε�ʱ��ô���������
void test01(int a, int b = 10, int c = 9);
//������Ĭ�ϲ����������ң����һ������������Ĭ�ϲ�������ô�������֮��Ĳ�������������Ĭ�ϲ�����
//void test(int a, int b, int c = 0)
//void test(int a, int b, int c = 0,int d) //err ����Ĳ�������ҪΪĬ�ϲ���,�����ò�����ʱ����Ĭ�ϲ���Ϊ���������˲����������õĲ���Ϊ׼

void test(int a, int b, int c = 0)  //Ĭ�ϲ����к������ص�ʱ��ע�⺯���Ķ�����
{
	int sum = a + b + c;
	cout << sum << endl;  
}

void test03(int a, int)  //ռλ����Ҳ���봫ֵ�������ڲ��޷�ʹ��ռλ����
{
	cout << a << endl;
}
//void test03(int a, int b, int = 0)  //���õ�ʱ���������test03����������ɶ�����
//{
//
//}
void test04(int a, int b, int = 0)
{
	cout << a + b << endl;
}

int main()
{
	test(1, 2);  //3
	test(2, 3, 4);//9
	test01(0);  //19
	test01(1, 2); //12
	test01(1, 2, 3); //6

	test03(10,10);//10
	test04(12, 12); //24
	test04(1, 2, 3);//3
	return EXIT_SUCCESS;
}

//������������ͺ�������ֿ�д�����������ͺ������岻��ͬʱ����Ĭ�ϲ���,Ĭ�ϲ������д���������棬��Ȼ���ε�ʱ����봫��������
//void test01(int a, int b = 10, int c = 9)  //��Ҫ����
void test01(int a, int b, int c )  //��Ҫ����
{
	int sum = a + b + c;
	cout << sum << endl;
}