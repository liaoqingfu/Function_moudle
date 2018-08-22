#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

//const修饰成员函数
class Person{
	friend void test();
public:
	Person(){
		this->mAge = 0;
		this->mID = 0;
	}
	//在函数括号后面加上const,修饰成员变量不可修改,除了mutable变量
	void sonmeOperate() const{
		//this->mAge = 200; //mAge不可修改
		this->mID = 10; //const Person* const tihs;
	}
	void ChangePerson() const{
		//mAge = 100;  //成员变量不可修改
		this->mID = 100;
	}

	void ShowPerson(){
		cout << "ID:" << mID << " mAge:" << mAge << endl;
	}
private:
	int mAge;
	mutable int mID;
};

void test(){
	const Person person;
	//1. 可访问数据成员
	cout << "Age:" << person.mAge << endl;
	//person.mAge = 300; //不可修改
	person.mID = 1001; //但是可以修改mutable修饰的成员变量
	//2. 只能访问const修饰的函数
	//person.ShowPerson();
	person.ChangePerson();
	
}


int main(){

	Person person;
	person.sonmeOperate();
	person.ShowPerson();
	test();
	person.ChangePerson();
	person.ShowPerson();
	system("pause");
	return EXIT_SUCCESS;
}
