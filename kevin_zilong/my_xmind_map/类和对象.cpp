#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Student{
public:
	Student()   //无参构造函数
	{
		cout << "构造函数" << endl;
	}
	Student(int a) //有参构造
	{
		cout << "有参构造" << endl;
	}
	Student(const Student &stu)  //拷贝构造函数
	{
		cout << "拷贝构造函数" << endl;
	}
	Student &operator=(const Student &stu)  //赋值函数
	{
		cout << "赋值函数" << endl;
		return *this;
	}
	~Student()  //析构函数
	{
		cout << "析构函数" << endl;
	}
};
//传递的参数是普通对象，函数参数也是普通对象，传递将会调用拷贝构造
void fun(Student stu)   //调用拷贝构造
{
	cout << "fun函数" << endl;
}
Student kevi()
{
	Student stu(10);
	cout << "kevi函数" << endl;
	return stu;
}

void test()
{
	//Student();  //匿名对象，调用构造函数，一调用立马析构，不会等整个程序结束后才析构,匿名对象的生命周期在当前行
	
	//Student stu; //定义对象，调用构造函数，析构函数
	//Student(stu);   //就是相当于Student stu ，与上面一行会显示重定义，不能调用拷贝构造函数去初始化匿名对象
	//Student stu1(stu); //用stu对象初始化stu1对象，调用拷贝构造函数，析构函数，析构函数调用顺序和构造函数调用的顺序是相反的，先构造的后析构
	//Student stu1 = stu;  //编译器会优化，相当于stu1(stu) ,和上面的是一样的,拷贝函数
	//Student stu3 = Student(stu);   //和上面是一样的 Student(stu)就相当于是匿名对象
	//stu1 = stu;  //赋值函数
	//Student stu2;  //构造函数
	//stu2 = stu;  //这样才会去调赋值函数
	//Student stu5 = 10; //相当于Student stu5=stu(10);  //调用有参构造函数
	//Student stu6 = Student(10);//调普通构造函数
	//fun(stu);
	Student stu7=kevi();
}

class Myclass{
public:
	//Myclass()
	//{
	//	this->name = new char[128];
	//	memset(name, 0, 128);
	//	this->num = 0;
	//	this->mcapacity = 10;
	//}
	Myclass(char *name,int num, int mcapacity)
	{
		this->name = new char[strlen(name)+1];
		memset(this->name, 0, strlen(name) + 1);
		strcpy(this->name, name);
		this->num = num;
		this->mcapacity = mcapacity;
	}
	void print()
	{
		cout << name << endl;
		cout << num << endl;
		cout << mcapacity << endl;
	}
	~Myclass()
	{
		delete[]this->name;
		this->name = NULL;
	}
private:
	char *name;
	int num;
	int mcapacity;
};
class MYgrad{
public:
	MYgrad(int a) :c1("xiao", 10, 20), c2("long",15,25)
	{
		this->mSize = a;
	}
private:          //多个对象的调用构造的规则：先调用成员对象的构造函数，成员对象的调用顺序和定义顺序有关。
                  //在调用本身的构造函数，析构函数的调用顺序和构造函数相反
	Myclass c1;   //在类里面定义其他类的对象，必须要初始化定义对象，跟着定义对象的构造函数初始化，不然构造函数会报错
	Myclass c2;
	int mSize;
	
};

class stu{
public:
	stu(int age)
	{
		this->age = age;
		cout << "stu构造函数" << endl;
	}
	~stu()
	{
		cout << "stu的析构函数" << endl;
	}
private:
	int age;
};

class kev{
public:
	kev(int a, int b)
	{
		this->a = a;
		this->b = b;
		cout << "kev的构造函数" << endl;
	}
	~kev()
	{
		cout << "kev的析构函数" << endl;
	}
private:
	int a;
	int b;
};

class Kevin{
public:
	Kevin(int a, int b, int c) :s(a), k(b, c) //里面有类定义的对象，需要初始化列表
	{
		this->n = a;
		cout << "Kevin的构造函数" << endl;
	}
	~Kevin()
	{
		cout << "Kevin的析构函数" << endl;
	}
private:
	stu s;
	kev k;
	int n;
};

class home{
public:
	home(int a, int b, int age, char c) :ma(a), mb(b), age(age), mc(c){}   //这样可以直接初始化列表，初始化成员列表(参数列表)只能在构造函数使用。
	
private:
	int ma;
	int mb;
	int age;
	char mc;
};

class school{
public:
	explicit school(int a)    //防止构造函数进行的隐式转换
	{
		this->a = a;
	}
private:
	int a;
};

void test01()
{
	//Myclass();
	//Myclass mca = Myclass("xiao", 15, 20);
	/*Myclass mca("xiao", 15, 19);*/
	//mca.print();
	//Kevin(1, 2, 3);  //匿名对象，调用构造函数，一调用立马析构，不会等整个程序结束后才析构,匿名对象的生命周期在当前行
	//Kevin ke(1, 2, 3);
	//Kevin ke1(1, 2, 3);
	//多个对象的调用构造的规则：先调用成员对象的构造函数，成员对象的调用顺序和定义顺序有关。
	//在调用本身的构造函数，析构函数的调用顺序和构造函数相反

	//school sch = 10; //分不清是赋值，还是初始化，不让编译器优化这代码
	stu s = 20;  //没有加explicit关键字，编译器就会优化  stu s=s(10);
	school sch(10); //OK ,初始化，
}


int main()
{
	//test();

	test01();
	return EXIT_SUCCESS;
}