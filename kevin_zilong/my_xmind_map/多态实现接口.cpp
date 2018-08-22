/*
请写出一个Cat类和Dog类继承与Animal类,
并封装一个架构函数实现多态，并写出测试案例。
class Animal
{
public:
Animal();
virtual void voice() = 0;
};
*/

#include<iostream>
using namespace std;

class Animal{
public:
	Animal(){}
	virtual void voice() = 0;
};
class Cat :public Animal{
public:
	void voice()
	{
		cout << "Cat voice" << endl;
	}
};

class Dog :public Animal{
public:
	void voice()
	{
		cout << " Dog voice" << endl;
	}
};

void func(Animal &an)   //就是接口
{
	an.voice();
}

void test()
{
	Animal *p1 = new Cat;
	Animal *p2 = new Dog;
	func(*p1);
	func(*p2);
}

int main()
{
	test();
	return EXIT_SUCCESS;
}