#include<iostream>
using namespace std;
class A{
public:
	int a;
	A()
	{
		this->a = 0;
		this->b = 0;
		this->c = 0;
	}
	void set(int a, int b, int c)
	{
		this->a = a;
		this->b = b;
		this->c = c;
	}
protected:
	int b;
private:
	int c;
};
class B :public A{   //公有继承
	//当类的继承方式为公有继承时,基类的公有和保护成员的访问属性在派生
	//类中  不变, 而基类的私有成员不可访问。
public:
	void print()
	{
		a = 15;
		b = 16;//类内可以访问公有和保护成员
		//c = 12;  //类内也不能访问私有成员，不可访问
		cout << "a=" << a << endl;
		cout << "b=" << b << endl;
		//cout << c << endl;  //私有成员无法继承
	}
};

class C :protected A{ //保护继承
	//保护继承中,基类的公有成员和私有成员都以保护成员的身份出现在派生
	//类  中, 而基类的私有成员不可访问。
public:
	void print()
	{
		a = 12;
		b = 15;  //类内可以访问公有和保护成员
		//c = 15;  //类内也不能访问私有成员，不可访问
		cout << "a=" << a << endl;
		cout << "b=" << b << endl;
		//cout << c << endl;  //私有成员无法继承
	}
};
class D :private A{   //私有继承  
	//,基类中的公有成员和保护成员都以私有成
	//员身  份出现在派生类中, 而基类的私有成员在派生类中不可访问
public:
	void print()
	{
		a = 100;   //类内可以访问公有和保护成员
		b = 10;
		//c = 19;  ////类内也不能访问私有成员,不可访问
		cout << "a="<<a << endl;
		cout << "b="<<b << endl;
		//cout << c << endl;  //私有成员无法继承
	}
};

int main()
{
	A aa;
	B bb;
	C cc;
	D dd;
	//aa.c=100 //类外不能访问私有成员
	aa.a = 10;
	//aa.b;  //类外不能访问保护成员

	bb.a = 100;  //公有继承可以访问继承的公有成员
	//bb.c=100 //类外不能访问私有成员
	//bb.b;  //类外不能访问保护成员

	//cc.a = 1000;//保护继承后，基类里的成员都是以保护的权限在派生类里，类外无访问权限,类外不能访问保护成员
	//cc.c; //类外不能访问保护成员
	//cc.b;//类外不能访问保护成员

	//dd.a;  //私有继承后，基类里的成员都是以私有的权限在派生类里面，类外无访问权限，类外不能访问私有成员
	//dd.b;//类外不能访问私有成员
	//dd.c;//类外不能访问私有成员

	aa.set(1, 2, 3);
	bb.set(10, 20, 30);
	//cc.set(20, 30, 40);  //保护继承后，基类里的成员函数也是以保护的权限在派生类里，类外无访问权限,类外不能访问保护成员函数
	//dd.set(5, 6, 7); // 私有继承后，基类里的成员函数也是以私有的权限在派生类里面，类外无访问权限，类外不能访问私有成员函数

	bb.print();
	cc.print();
	dd.print();
	system("pause");
	return EXIT_SUCCESS;

}