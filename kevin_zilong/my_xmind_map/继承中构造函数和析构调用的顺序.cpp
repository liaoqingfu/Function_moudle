#include<iostream>
using namespace std;

class D{
public:
	D(){
		cout << "D�๹�캯��!" << endl;
	}
	~D(){
		cout << "D����������!" << endl;
	}
};
class A{
public:
	A(){
		cout << "A�๹�캯��!" << endl;
	}
	~A(){
		cout << "A����������!" << endl;
	}
};
class B : public A{
public:
	B(){
		cout << "B�๹�캯��!" << endl;
	}
	~B(){
		cout << "B����������!" << endl;
	}
};
class C : public B{
public:
	C(){
		cout << "C�๹�캯��!" << endl;
	}
	~C(){
		cout << "C����������!" << endl;
	}
public:
	D c;
};
void test(){
	C c;
}

int main()
{
	test();   //A  B  D C  C D B A 
	return EXIT_SUCCESS;
}
