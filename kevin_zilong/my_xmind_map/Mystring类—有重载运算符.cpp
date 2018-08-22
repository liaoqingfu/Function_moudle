/*

自实现string类：并进行测试

class MyString
{
public:
MyString()
MyString(const char* str="");  //普通构造函数
MyString(const MyString  & other);    //拷贝构造函数
MyString& operator=(const MyString& other);    //赋值运算符重载
MyString& operator=(const char* str);    //赋值运算符重载
~MyString();    //析构函数
char& operator[](unsigned int index);
friend ostream& operator<<(ostream& os, const MyString& str);
friend istream& operator>>(istream& is, MyString& str);
operator+(const MyString &str)
operator==(const MyString &str)
operator!=(const MyString &str)
private:
char* m_str;
};
*/
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

class MyString
{
public:
	MyString();
	MyString(char* str);  //普通构造函数
	MyString(const MyString  & other);    //拷贝构造函数
	MyString& operator=(const MyString& other);    //赋值运算符重载
	MyString& operator=(const char* str);    //赋值运算符重载
	~MyString();    //析构函数
	char& operator[](unsigned int index);
	friend ostream& operator<<(ostream& os, const MyString& str);
	friend istream& operator>>(istream& is, MyString& str);
	MyString operator + (const MyString &str);
	bool operator == (const MyString &str);
	bool operator != (const MyString &str);
private:
	char* m_str;
};


MyString::MyString()
{
	m_str = new char[128];
	memset(m_str, 0, 128);
}


MyString::MyString(char* str)  //普通构造函数
{
	m_str = new char[strlen(str) + 1];
	memset(m_str, 0, strlen(str) + 1);
	strcpy(m_str, str);
}
MyString::MyString(const MyString  & other)    //拷贝构造函数
{
	if (this->m_str != NULL)
	{
		delete[]this->m_str;
		this->m_str = NULL;
	}
	m_str = new char[strlen(other.m_str) + 1];
	memset(m_str, 0, strlen(other.m_str) + 1);
	strcpy(m_str, other.m_str);
}
MyString& MyString::operator=(const MyString& other)    //赋值运算符重载
{
	if (this->m_str != NULL)
	{
		delete[]this->m_str;
		this->m_str = NULL;
	}
	this->m_str = new char[strlen(other.m_str) + 1];
	memset(m_str, 0, strlen(other.m_str) + 1);
	strcpy(this->m_str, other.m_str);
	return *this;
}
MyString& MyString::operator=(const char* str)    //赋值运算符重载
{
	if (this->m_str != NULL)
	{
		delete[]this->m_str;
		this->m_str = NULL;
	}
	this->m_str = new char[strlen(str) + 1];
	memset(m_str, 0, strlen(str) + 1);
	strcpy(this->m_str, str);
	return *this;
}
MyString::~MyString()    //析构函数
{
	if (this->m_str != NULL)
	{
		delete[]this->m_str;
		this->m_str = NULL;
	}
}
char& MyString::operator[](unsigned int index)
{
	return m_str[index];
}
ostream& operator<<(ostream& os, const MyString& str)
{
	os << "m_str:" << str.m_str << endl;
	return os;
}
istream& operator>>(istream& is, MyString& str)
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
MyString MyString::operator + (const MyString &str)
{
	int n = strlen(str.m_str) + strlen(this->m_str)+1;
	MyString temp;
	if (temp.m_str != NULL)
	{
		delete[]temp.m_str;
		temp.m_str = NULL;
	}
	temp.m_str = new char[n];
	memset(temp.m_str, 0, n);
	strcat(temp.m_str,this->m_str);
	strcat(temp.m_str, str.m_str);
	return temp;
}
bool MyString::operator == (const MyString &str)
{
	if (strcmp(this->m_str, str.m_str) == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool MyString::operator != (const MyString &str)
{
	if (strcmp(this->m_str, str.m_str) != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void test()
{
	MyString str1("kevin");
	MyString str2 = str1;
	cout << str2;
	cout << str1;
	MyString str3;
	cout << "请输入字符串" << endl;
	cin >> str3;
	cout << str3;
	cout << str3[0] << endl;
	MyString str4=str1 + str2;
	cout << str4;
	if (str1 == str4)
	{
		cout << "str1和str4相等" << endl;
	}
	else
	{
		cout << "str1和str4不相等" << endl;
	}
}

int main()
{
	test();
	return EXIT_SUCCESS;
}