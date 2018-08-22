#include<iostream>
#include<string>
using namespace std;

class Student{
	//friend Student operator+(Student &stu1, Student &stu2);
	//friend Student operator-(Student &stu1, Student &stu2);
	friend ostream &operator<<(ostream &out, Student &stu);
	friend istream &operator>>(istream &in, Student &stu);
public:
	Student(int age,int id)
	{
		this->age = age;
		this->id = id;
	}
	Student operator+(Student &stu)   //类内实现+加号运算符重载
	{
		Student tem(this->age + stu.age, this->id + stu.id);
		return tem;
	}
	Student operator-(Student &stu)   //类内实现-减号运算符重载
	{
		Student tem(this->age - stu.age, this->id - stu.id);
		return tem;
	}
	Student &operator=(Student &stu)  //类内实现=等号运算符重载
	{
		this->age = stu.age;
		this->id = stu.id;
		return *this;
	}

	Student &operator++()  //前置++ 无占位参数，可以做左值
	{
		++this->age;
		++this->id;
		return *this;  //返回引用
	}
	Student operator++(int)  //后置++ 有占位参数 c++规定有占位参数为后置加加，不能做左值
	{
		Student tem(*this);
		++this->age;
		++this->id;
		return tem;  //先返回对象本身,后加加
	}


	int AGE()
	{
		return age;
	}
	int Id()
	{
		return id;
	}
private:
	int age;
	int id;
};

//Student operator+(Student &stu1, Student &stu2)   //类外实现+加号运算符重载
//{
//	Student tem(stu1.age + stu2.age, stu1.id + stu2.id);
//	return tem;
//}

//Student operator-(Student &stu1, Student &stu2)   //类外实现-减号运算符重载
//{
//	Student tem(stu1.age - stu2.age, stu1.id - stu2.id);
//	return tem;
//}

//  << 和 >> 操作符最好通过友元函数进行重载

ostream &operator<<(ostream &out, Student &stu)  //重载<<左移操作符  类外实现
{
	out << stu.age << " " << stu.id << endl;
	return out;
}

istream &operator>>(istream &in, Student &stu)  //重载>>右移操作符  类外实现
{
	in >> stu.age;
	in >> stu.id;
	return in;
}

/*
ostream& operator<<(ostream& os, const MyString& str)   
{
os << "m_str:" << str.m_str << endl;
return os;
}
istream& operator>>(istream& is, MyString& str)   //指针字符时，重载>>右移操作符时注意需要接一下，不然会导致输入后，默认构造函数分配空间不够导致程序崩溃
{
char temp[1024] = { 0 };
is >> temp;
if (str.m_str != NULL)
{
delete[]str.m_str;
str.m_str = NULL;
}
str.m_str = new char[strlen(temp) + 1];
memset(str.m_str, 0, strlen(temp) + 1);
strcpy(str.m_str, temp);
return is;
}

Student & operator=(const Student &stu)
{
//1.释放原来空间
if (this->p != NULL)
{
delete[] this->p;
this->p = NULL;
}
//2.申请新空间
this->p = new char[strlen(stu.p) + 1];
strcpy(this->p, stu.p);

return *this;
}
*/


void test()
{
	//Student stu(10, 10);
	//Student stu1(20, 20);
	//Student stu3=stu + stu1;
	//cout << stu3.AGE() << " " << stu3.Id() << endl;
	//cout << "------------------------" << endl;
	//stu3 = stu1 - stu;
	//cout << stu3.AGE() << " " << stu3.Id() << endl;
	//cout << "------------------------" << endl;
	//cout << stu3;
	//cin >> stu3;  //输入age和id
	//cout << stu3;
	////Student stu4 = stu3;  //这是走拷贝构造，不会去赋值操作
	//Student stu4(0,0);
	//stu4 = stu3;
	//cout << stu4;
	//++stu4;
	//cout << stu4;
	Student stu5(100, 100);
	cout << ++stu5; //101 101
	cout << stu5++; //101 101
	Student stu6(0, 0);
	stu6 = stu5;
	cout << stu6;  //102  102

}

class Kevin{
public:
	Kevin()
	{
		cout << "Kevin构造函数" << endl;
	}
	void PrintKevin()
	{
		cout << "Kevin类" << endl;
	}
	~Kevin()
	{
		cout << "Kevin析构函数" << endl;
	}
};

class SmartPoint{
public:
	SmartPoint(Kevin *p)
	{
		cout << "SmartPoint构造函数" << endl;
		this->p = p;
	}

	Kevin *operator->()  //重载->
	{
		return this->p;
	}

	Kevin &operator*()   //重载*  
	{
		return *p;
	}

	~SmartPoint()
	{
		cout << "SmartPoint析构函数" << endl;
		if (this->p != NULL)
		{
			delete this->p;
			this->p = NULL;
		}
	}
private:
	Kevin *p;
};

void test01()
{
	Kevin *p1 = new Kevin;
	p1->PrintKevin();
	SmartPoint Sm(p1);
	Sm->PrintKevin();  //用SmartPointer的对象去调用Maker类中的成员函数，重载->
	(*Sm).PrintKevin();  //用SmartPointer的对象去调用Maker类中的成员函数，重载*

	Kevin kev;
	kev.PrintKevin();    //相当于kev == (*Sm)

}

class Person{
public:
	Person()
	{
		cout << "Person构造函数" << endl;
	}
	Person(string name)
	{
		this->name = name;
	}
	void printPerson()
	{
		cout << name + " hello" << endl;  //string类里面重载了+加号运算符
	}
	int operator()(int a=10,int b=10)  //函数调用符号重载
	{
		cout << "函数调用符号重载" << endl;
		return a + b;
	}
private:
	string name;
};


void test02()
{
	Person per("kevin");
	cout<<per(15, 15)<<endl;   ////函数调用符号重载  30
	per.printPerson();


	//Person per1(); //不会调用 符号重载函数  warning C4930: “Person per1(void)”: 未调用原型函数(是否是有意用变量定义的?)
	Person per1;
	per1();   //这样就会调用  符号重载函数
}


class water{
public:
	water()
	{
		num = 0;
	}
	void SetNum(int num)
	{
		this->num = num;
	}

	operator bool()  //重载bool，注意没有返回值
	{
		if (num <= 0)
		{
			return false;
		}
		return true;
	}
private:
	int num;
};

void test03()
{
	water wa;
	wa.SetNum(19);
	if (wa)
	{
		cout << "true" << endl;
	}
	else
	{
		cout << "false" << endl;
	}
}

int main()
{

	//test();
	//test01();
	//test02();
	test03();
	return EXIT_SUCCESS;
}