/*
��д��һ��Cat���Dog��̳���Animal��,
����װһ���ܹ�����ʵ�ֶ�̬����д�����԰�����
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

void func(Animal &an)   //���ǽӿ�
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