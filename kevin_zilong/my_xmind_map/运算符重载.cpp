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
	Student operator+(Student &stu)   //����ʵ��+�Ӻ����������
	{
		Student tem(this->age + stu.age, this->id + stu.id);
		return tem;
	}
	Student operator-(Student &stu)   //����ʵ��-�������������
	{
		Student tem(this->age - stu.age, this->id - stu.id);
		return tem;
	}
	Student &operator=(Student &stu)  //����ʵ��=�Ⱥ����������
	{
		this->age = stu.age;
		this->id = stu.id;
		return *this;
	}

	Student &operator++()  //ǰ��++ ��ռλ��������������ֵ
	{
		++this->age;
		++this->id;
		return *this;  //��������
	}
	Student operator++(int)  //����++ ��ռλ���� c++�涨��ռλ����Ϊ���üӼӣ���������ֵ
	{
		Student tem(*this);
		++this->age;
		++this->id;
		return tem;  //�ȷ��ض�����,��Ӽ�
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

//Student operator+(Student &stu1, Student &stu2)   //����ʵ��+�Ӻ����������
//{
//	Student tem(stu1.age + stu2.age, stu1.id + stu2.id);
//	return tem;
//}

//Student operator-(Student &stu1, Student &stu2)   //����ʵ��-�������������
//{
//	Student tem(stu1.age - stu2.age, stu1.id - stu2.id);
//	return tem;
//}

//  << �� >> ���������ͨ����Ԫ������������

ostream &operator<<(ostream &out, Student &stu)  //����<<���Ʋ�����  ����ʵ��
{
	out << stu.age << " " << stu.id << endl;
	return out;
}

istream &operator>>(istream &in, Student &stu)  //����>>���Ʋ�����  ����ʵ��
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
istream& operator>>(istream& is, MyString& str)   //ָ���ַ�ʱ������>>���Ʋ�����ʱע����Ҫ��һ�£���Ȼ�ᵼ�������Ĭ�Ϲ��캯������ռ䲻�����³������
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
//1.�ͷ�ԭ���ռ�
if (this->p != NULL)
{
delete[] this->p;
this->p = NULL;
}
//2.�����¿ռ�
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
	//cin >> stu3;  //����age��id
	//cout << stu3;
	////Student stu4 = stu3;  //�����߿������죬����ȥ��ֵ����
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
		cout << "Kevin���캯��" << endl;
	}
	void PrintKevin()
	{
		cout << "Kevin��" << endl;
	}
	~Kevin()
	{
		cout << "Kevin��������" << endl;
	}
};

class SmartPoint{
public:
	SmartPoint(Kevin *p)
	{
		cout << "SmartPoint���캯��" << endl;
		this->p = p;
	}

	Kevin *operator->()  //����->
	{
		return this->p;
	}

	Kevin &operator*()   //����*  
	{
		return *p;
	}

	~SmartPoint()
	{
		cout << "SmartPoint��������" << endl;
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
	Sm->PrintKevin();  //��SmartPointer�Ķ���ȥ����Maker���еĳ�Ա����������->
	(*Sm).PrintKevin();  //��SmartPointer�Ķ���ȥ����Maker���еĳ�Ա����������*

	Kevin kev;
	kev.PrintKevin();    //�൱��kev == (*Sm)

}

class Person{
public:
	Person()
	{
		cout << "Person���캯��" << endl;
	}
	Person(string name)
	{
		this->name = name;
	}
	void printPerson()
	{
		cout << name + " hello" << endl;  //string������������+�Ӻ������
	}
	int operator()(int a=10,int b=10)  //�������÷�������
	{
		cout << "�������÷�������" << endl;
		return a + b;
	}
private:
	string name;
};


void test02()
{
	Person per("kevin");
	cout<<per(15, 15)<<endl;   ////�������÷�������  30
	per.printPerson();


	//Person per1(); //������� �������غ���  warning C4930: ��Person per1(void)��: δ����ԭ�ͺ���(�Ƿ��������ñ��������?)
	Person per1;
	per1();   //�����ͻ����  �������غ���
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

	operator bool()  //����bool��ע��û�з���ֵ
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