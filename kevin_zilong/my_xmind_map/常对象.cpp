#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

//const���γ�Ա����
class Person{
	friend void test();
public:
	Person(){
		this->mAge = 0;
		this->mID = 0;
	}
	//�ں������ź������const,���γ�Ա���������޸�,����mutable����
	void sonmeOperate() const{
		//this->mAge = 200; //mAge�����޸�
		this->mID = 10; //const Person* const tihs;
	}
	void ChangePerson() const{
		//mAge = 100;  //��Ա���������޸�
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
	//1. �ɷ������ݳ�Ա
	cout << "Age:" << person.mAge << endl;
	//person.mAge = 300; //�����޸�
	person.mID = 1001; //���ǿ����޸�mutable���εĳ�Ա����
	//2. ֻ�ܷ���const���εĺ���
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
