#include<iostream>
using namespace std;

namespace A{      //?	�����ռ�ֻ��ȫ�ַ�Χ�ڶ���
	int a = 10;
	int test()   //�����к���
	{
		int b = 100;
		return b;
	}
}
namespace B{   //?	�����ռ�ֻ��ȫ�ַ�Χ�ڶ���
	int a = 20;
	int test(int b)
	{
		return b;
	}
}

namespace C{   //�����ռ����Ƕ�������ռ䣬���ʵ�ʱ��������Ҫ�����
	int a = 30;
	namespace D{
		int a = 50;
	}
}
namespace D{
	void func(){ cout << "func()" << endl; }
	void func(int a){ cout << "func(int "<<a<<")" << endl; }
	void func(int a, int b){ cout << "func(int " << a << ",int " << b << ")" << endl; }
}
int main()
{
	//����һ��ֱ��ͨ����������������ռ���ĳ�Ա
	cout << A::a << endl;   //10             //cout��c++�еı�׼�������endl��������в�ˢ�»�������
	cout << A::test() << endl;   //100

	//������������ͨ���������ʿռ��ڵĳ�Ա,using�ؼ���
	/*using B::a;  *///���������ֱ�ӷ���a������,������Ҫע�⣬�������ڶ���a�ͻ�����ض���,��������ʼ�Ժ��a����B::a ����� a
	/*a = 123;*/  //Ҳ����ֱ���޸�a��ֵ
	//cout << a << endl;  //20

	//���������������������ռ䣬�����Ϳ��Է��������ռ��ڵĳ�Ա��
	//using namespace C;
	//using namespace D;
	//cout << a << endl;  //B::a ��������������C�еĻ�������B��ʱ����Ҫ����������{}
	{
		using B::a;
		using B::test;    //���������ռ�����������Ҫ�ӣ�������
		cout << a << endl;  //20 ��������{} ���������ڣ�������Ч���������⣬������Ч
		cout << test(90) << endl;
	}            
	//using namespace C;
	//cout << a << endl;   //30
	{
		using namespace C;
		cout << a << endl;   //30
	}
	//using namespace D;  //�����ͻᱨ���ֲ�����C����Ļ���D����ģ������������������
	using namespace C::D;   //�����ˣ�����ȷ�ķ��ţ�����������ǰ�������͵ü���{}������
	cout << a << endl;
	//�����ռ�����޸ı���
	namespace hk = B;   //��B������Ϊhk������ע��Ⱥź���д��ǰ������
	cout << hk::test(1000) << endl;  //1000

	//using����������������,ֻ��Ҫ����һ�� ��������ռ����һ������ͬ�������صĺ�����using������������������غ��������м��ϡ�
	using  D::func;
	func(); //��ӡ��� func()
	func(15);  //��ӡ��� func(int 15)
	func(55,55);  //��ӡ��� func(int 55, int 55)
	return EXIT_SUCCESS;    //EXIT_SUCCESS ����0��ת������鿴 #define EXIT_SUCCESS    0   #define EXIT_FAILURE    1
}