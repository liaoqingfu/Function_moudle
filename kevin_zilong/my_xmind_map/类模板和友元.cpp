#include<iostream>
#include<string>

using namespace std;
template<class T1, class T2>class Maker;   //c.��ģ��Ҳ��Ҫ����
template<class T1, class T2>void print2(Maker<T1, T2> &p1); //b.���������߱��������������������ʵ��

template<class T1,class T2>
class Maker{
	//1.��Ԫ����������ʵ��
	friend void print(Maker<T1, T2> &p)
	{
		cout << "name:" << p.name << " age:" << p.age << endl;
	}
	
	//2.��Ԫ��������ʵ��
	friend void print2<>(Maker<T1, T2> &p1);  //a.�ÿղ����б�<>
public:
	Maker(T1 name, T2 age)
	{
		this->name = name;
		this->age = age;
	}
private:
	T1 name;
	T2 age;
};

void test1()
{
	string ab;
	cout << "��������Ҫ��������֣�";
	cin >> ab;
	int n;
	cout << "��������Ҫ��������䣺";
	cin >> n;

	Maker<string, int> Ma(ab, n);
	print(Ma);
}
template<class T1, class T2>
void print2(Maker<T1, T2> &p1)   //��Ԫ��������ʵ��
{
	cout << "name:" << p1.name << " age:" << p1.age << endl;
}

void test2()
{
	Maker<string, int> mm("long", 20);
	print2(mm);
}

int main()
{
	test1();
	test2();
	return EXIT_SUCCESS;
}