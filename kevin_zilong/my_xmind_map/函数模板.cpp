#include<iostream>
#include<string>
using namespace std;

template<class T>  //û�зֺţ�templateģ��ؼ��֣�class��ʾ�κ�����,T���Լ�ȡ�����֣�����ȡ������֣�T�õĶ�
//template<typename T>  //��typename��class��һ����
//���߱��������溯������T�Ļ���Ҫ������������ĵ�һ������Ϊ����ģ��
void mySwap(T &a, T &b)
{
	T tem = a;
	a = b;
	b = tem;
}

void test()
{
	int a = 10;
	int b = 20;
	mySwap(a, b);
	cout << "a=" << a << " b=" << b << endl;  //a=20,b=10

	char c = 'A';
	char d = 'X';
	mySwap(c, d);   //���Դ���������ı���,������������ͱ����ָ����һ��
	cout << "c=" << c << " d=" << d << endl;  //c=X,d=A
	//mySwap(a, c);  //�ᱨ�����Ĳ�������Ҫһ������Ϊ������ģ�������һ���ģ����ú���ģ�壬�ϸ�ƥ�����ͣ�����Ҫͳһ����Ȼ�Զ��Ƶ������ж�����
	//mySwap<int>(c, d); //�ᱨ����ʾ��ָ������Ĳ������ͱ����봫��Ĳ�������һ��
	double x1 = 10.3;
	double x5 = 16.8;
	//mySwap<int>(x1, x5); //�ᱨ����ʾ��ָ������Ĳ������ͱ����봫��Ĳ�������һ��
	
}

//���ù�����������,�Ƚ�����ģ��ʵ�ٻ�Ϊ����,Ȼ���ٷ����������á�
template<class T>
void func(T a,T b)
{
	cout << "ģ�庯��" << endl;
}
//����ģ�岻�����Զ�����ת���������ϸ�ƥ������
//��ͨ�������Խ����Զ�����ת��

void func(int a,int b)
{
	cout << "��ͨ����" << endl;
}

void Max(int a, int b)
{
	cout << "void Max(int a, int b)" << endl;
}
template<class T>
void Max(T a, T b)
{
	cout << "void Max(T a, T b)" << endl;
}
template<class T>
void Max(T a, T b, T c)
{
	cout << "void Max(T a, T b, T c)" << endl;
}
template<class T1,class T2>   //���Դ���ͬ�Ĳ���
void funcc3(T1 a,T2 b)
{
	cout << "a=" << a << " b=" << b << endl;
}

int main()
{
	int a = 10;
	int b = 20;
	test();
	func(a,b);   //���ȵ�����ͨ����
	func<>(a, b);  //ǿ�Ƶ��ú���ģ�壬ʹ�ÿղ����б�
	char c1 = 'a';
	char c2 = 'b';
	func(c1, c2);  //�������ģ����Բ������õ�ƥ�䣬��ôʹ�ú���ģ��

	Max(3.0, 4.0);
	Max(5.0, 6.0,7.0); //����ģ��Ҳ��������
	Max('a', 100);  //������ͨ������������������ת��

	int k = 19;
	string ch = "xiao";
	funcc3(ch,k);    //��ӡa=xiao,b=19  ���Դ���ͬ�Ĳ�����ֻҪ����ú���ƥ�����
	return EXIT_SUCCESS;
}