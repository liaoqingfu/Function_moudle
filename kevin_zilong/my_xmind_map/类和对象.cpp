#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Student{
public:
	Student()   //�޲ι��캯��
	{
		cout << "���캯��" << endl;
	}
	Student(int a) //�вι���
	{
		cout << "�вι���" << endl;
	}
	Student(const Student &stu)  //�������캯��
	{
		cout << "�������캯��" << endl;
	}
	Student &operator=(const Student &stu)  //��ֵ����
	{
		cout << "��ֵ����" << endl;
		return *this;
	}
	~Student()  //��������
	{
		cout << "��������" << endl;
	}
};
//���ݵĲ�������ͨ���󣬺�������Ҳ����ͨ���󣬴��ݽ�����ÿ�������
void fun(Student stu)   //���ÿ�������
{
	cout << "fun����" << endl;
}
Student kevi()
{
	Student stu(10);
	cout << "kevi����" << endl;
	return stu;
}

void test()
{
	//Student();  //�������󣬵��ù��캯����һ�������������������������������������,������������������ڵ�ǰ��
	
	//Student stu; //������󣬵��ù��캯������������
	//Student(stu);   //�����൱��Student stu ��������һ�л���ʾ�ض��壬���ܵ��ÿ������캯��ȥ��ʼ����������
	//Student stu1(stu); //��stu�����ʼ��stu1���󣬵��ÿ������캯��������������������������˳��͹��캯�����õ�˳�����෴�ģ��ȹ���ĺ�����
	//Student stu1 = stu;  //���������Ż����൱��stu1(stu) ,���������һ����,��������
	//Student stu3 = Student(stu);   //��������һ���� Student(stu)���൱������������
	//stu1 = stu;  //��ֵ����
	//Student stu2;  //���캯��
	//stu2 = stu;  //�����Ż�ȥ����ֵ����
	//Student stu5 = 10; //�൱��Student stu5=stu(10);  //�����вι��캯��
	//Student stu6 = Student(10);//����ͨ���캯��
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
private:          //�������ĵ��ù���Ĺ����ȵ��ó�Ա����Ĺ��캯������Ա����ĵ���˳��Ͷ���˳���йء�
                  //�ڵ��ñ���Ĺ��캯�������������ĵ���˳��͹��캯���෴
	Myclass c1;   //�������涨��������Ķ��󣬱���Ҫ��ʼ��������󣬸��Ŷ������Ĺ��캯����ʼ������Ȼ���캯���ᱨ��
	Myclass c2;
	int mSize;
	
};

class stu{
public:
	stu(int age)
	{
		this->age = age;
		cout << "stu���캯��" << endl;
	}
	~stu()
	{
		cout << "stu����������" << endl;
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
		cout << "kev�Ĺ��캯��" << endl;
	}
	~kev()
	{
		cout << "kev����������" << endl;
	}
private:
	int a;
	int b;
};

class Kevin{
public:
	Kevin(int a, int b, int c) :s(a), k(b, c) //�������ඨ��Ķ�����Ҫ��ʼ���б�
	{
		this->n = a;
		cout << "Kevin�Ĺ��캯��" << endl;
	}
	~Kevin()
	{
		cout << "Kevin����������" << endl;
	}
private:
	stu s;
	kev k;
	int n;
};

class home{
public:
	home(int a, int b, int age, char c) :ma(a), mb(b), age(age), mc(c){}   //��������ֱ�ӳ�ʼ���б���ʼ����Ա�б�(�����б�)ֻ���ڹ��캯��ʹ�á�
	
private:
	int ma;
	int mb;
	int age;
	char mc;
};

class school{
public:
	explicit school(int a)    //��ֹ���캯�����е���ʽת��
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
	//Kevin(1, 2, 3);  //�������󣬵��ù��캯����һ�������������������������������������,������������������ڵ�ǰ��
	//Kevin ke(1, 2, 3);
	//Kevin ke1(1, 2, 3);
	//�������ĵ��ù���Ĺ����ȵ��ó�Ա����Ĺ��캯������Ա����ĵ���˳��Ͷ���˳���йء�
	//�ڵ��ñ���Ĺ��캯�������������ĵ���˳��͹��캯���෴

	//school sch = 10; //�ֲ����Ǹ�ֵ�����ǳ�ʼ�������ñ������Ż������
	stu s = 20;  //û�м�explicit�ؼ��֣��������ͻ��Ż�  stu s=s(10);
	school sch(10); //OK ,��ʼ����
}


int main()
{
	//test();

	test01();
	return EXIT_SUCCESS;
}