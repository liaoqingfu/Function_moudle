#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


void test()
{
	int a = 10;
	int b = 20;
	//���ñ���Ҫ����һ��Ϸ����ڴ�ռ䣬���ñ����ʼ���������������������߷��ز���
	int &c = a; //���ã���a�ռ�ȡ����Ϊc   �൱�� int * const c=&a  ,һ����ָ�룬ָ���ܸı�
	cout << "a=" << a << " c=" << c << endl; //10  10
	c = 100;   //�ı�ռ��ڱ�����ֵ��a��ֵҲ�ı���
	cout << "a=" << a << " c=" << c << endl; //100  100
	c = b; //ֻ�Ǹ�ֵ����b��ֵ��c��û�иı�ָ��
	int &d = c;
	int &f = d;  //����ȡ�������
	f = 200;
	cout << "a=" << a << " c=" << c << endl;  //200  200
}

void test01()
{
	int a = 10;
	int *p = &a;
	int *&p1 = p;   //Ӧ��ָ��
	*p1 = 100;
	cout << a << " "<<*p <<" "<< *p1 << endl;  //100  100  100
	int arr[5] = { 1, 2, 3, 4, 5 };
	int(&buf)[5] = arr;   //��������ã�����һ
	for (int i = 0; i < 5; ++i)
	{
		cout << buf[i]<<" ";  //1,2,3,4,5
	}
	cout << endl;
	typedef int(BUF)[5];  //��int[5]��typedefȡ����
	BUF &buf2 = arr;    //�����������������
	typedef int(&BUF3)[5];  //�����������������
	BUF3 buf3 = arr;   //������������һ����

}

int &test02()
{
	//int a = 100;
	//int &p = a;
	//return p;  //���ؾֲ����ã������������ͷ��˿ռ䣬������һ���յĿռ䣬���ܷ��ؾֲ�����
	static int a = 19;   //��̬������ȫ�ֵģ��������ز��ǿ��Եģ����غ���ԶԿռ���в���
	int &p = a;
	return p;
}

void Swap(int a,int b)   //ֵ���ݣ�������a ��b �����ǳ��������󣬿ռ���ͷ���
{
	int tem = a;
	a = b;
	b = tem;
}

void Swap1(int *a, int *b)  //��ַ���ݣ�ֱ�Ӳ����ڴ�ռ䣬��ʹ���ý����ˣ��ͷſռ��ˣ������ڴ��е�ֵ�Ѿ���ӽ�����
{
	int tem = *a;
	*a = *b;
	*b = tem;
}
void Swap2(int &a, int &b)  //���ô��ݣ����ռ�ȡ�������Կռ�����ֱ�ӽ���
{
	int tem = a;
	a = b;
	b = tem;
}
void test03(char *&p) //ָ������
{
	p = (char *)malloc(64);
	memset(p, 0, 64);
	strcpy(p, "xiao");
}

void FuncFree(char *&p)   //�ǵ��ͷ�������ڴ�ռ�
{
	if (p != NULL)
	{
		free(p);
		p = NULL;
	}
	return;
}

int main()
{
	//test();
	//test01();
	int &x = test02();
	x = 18;
	cout << x << endl;  //18 
	cout << test02() << endl;  //18
	test02() = 188;   //�������ú󣬺������Ե���ֵ
	cout << x << " " << test02() << endl;  //188 188

	int k = 10;
	int bb = 12;
	const int &m = k;  //�൱�� const int * const m=&k;
	//m = 20;  //m ���ܱ��޸�
	//m = bb;  //�����޸ģ�Ҳ�൱�ڸ�ֵ
	k = 100;
	cout << k << " " << m << endl;  //100  100
	int *p1 = (int *)&m;
	*p1 = 200;
	cout << k << " " << m << endl;  //����ͨ��ָ�����޸�

	//int &tem = 100;  //err 100�ǳ��������ǺϷ����ڴ�ռ䣬���ܰ�һ����������������
	const int &tem1 = 100; //OK ,���������Զ��Ż�  �൱��  int temp=100; int &tem1=temp;   
	//tem1 = 300;    //�����޸ģ�const���ε���Ϊ�����������޸ģ�����ͨ��ָ��ȥ����޸�
	int *p2 = (int *)&tem1;  //����ͨ��ָ��ȥ����޸�
	*p2 = 120;
	cout << tem1 << endl;  //120

	int a = 15;
	int b = 16;
	Swap(a, b);
	cout << a << " "<<b << endl;   //15,16
	Swap1(&a, &b);
	cout << a << " " << b << endl;  //16,15
	Swap2(a, b);   //���ô��ݿ�������ֱ��
	cout << a << " " << b << endl;  //15,16  ,���������ˣ�������16��15

	char *p = NULL;
	test03(p);
	cout << p << endl;   //xiao
	FuncFree(p);
	return EXIT_SUCCESS;
}