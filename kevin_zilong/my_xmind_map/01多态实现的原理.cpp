#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Animal{
public:
	
	virtual void speak()
	{
		cout << "Aniaml speak()" << endl;
	}
};

class Dog :public Animal{
public:
	 void speak()
	{
		cout << "Dog speak()" << endl;
	}
};

void test01()
{
	cout << sizeof(Animal) << endl;

	Animal *animal = new Dog;

	animal->speak();
}

int main()
{
	test01();
	system("pause");
	return EXIT_SUCCESS;
}

