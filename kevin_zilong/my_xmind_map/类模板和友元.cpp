#include<iostream>
#include<string>

using namespace std;
template<class T1, class T2>class Maker;   //c.类模版也需要声明
template<class T1, class T2>void print2(Maker<T1, T2> &p1); //b.声明，告诉编译器我下面有这个函数实现

template<class T1,class T2>
class Maker{
	//1.友元函数在类内实现
	friend void print(Maker<T1, T2> &p)
	{
		cout << "name:" << p.name << " age:" << p.age << endl;
	}
	
	//2.友元函数类外实现
	friend void print2<>(Maker<T1, T2> &p1);  //a.用空参数列表<>
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
	cout << "请输入需要输入的名字：";
	cin >> ab;
	int n;
	cout << "请输入需要输入的年龄：";
	cin >> n;

	Maker<string, int> Ma(ab, n);
	print(Ma);
}
template<class T1, class T2>
void print2(Maker<T1, T2> &p1)   //友元函数类外实现
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