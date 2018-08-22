#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stack>
#include<queue>
using namespace std;

//（STL）两个栈实现⼀个队列。
//要求：只能使用栈的pop(), top()和push()，以及测试栈是否为空
//empty()四个操作.来实现队列的clear(), push(), pop(), back(), front()等操作。

template<class T>
class MyQueue
{
private:
	stack<T> m_stackIn;//一个进的栈
	stack<T> m_stackOut;//一个出的栈
	int m_size;
public:
	MyQueue(){ m_size = 0; }//构造函数
	void push(T t); //往队尾添加元素
	T front();//返回第一个元素
	T back();//返回最后一个元素
	void pop();//从队头移除第一个元素
	void clear();//清空容器
	bool empty();//判断容器是否为空
	int size(){ return m_size; }//得到值的多少
};
//push
template<class T>
void MyQueue<T>::push(T t)
{
	m_stackIn.push(t);//往进的里面放1个数据
	m_size++;//数值自增
}
//front
template<class T>
T MyQueue<T>::front()//返回第一个数据
{
	if (m_stackOut.empty())//判断出是否为空
	{
		while (!m_stackIn.empty())//不等于空则循环把(进的栈)的内容放到(出的栈)里面.
		{
			m_stackOut.push(m_stackIn.top());
			m_stackIn.pop();
		}
	}
	return m_stackOut.top();//然后返回栈顶元素
}
//back
template<class T>
T MyQueue<T>::back()//返回最后一个数据
{
	if (m_stackIn.empty())//判断出是否为空
	{
		while (!m_stackOut.empty())//不等于空则循环把(出的栈)的内容放到(进的栈)里面.
		{
			m_stackIn.push(m_stackOut.top());
			m_stackOut.pop();
		}
	}
	return m_stackIn.top();//然后返回栈顶元素
}
//pop
template<class T>
void MyQueue<T>::pop()//弹出数据
{
	if (m_stackOut.empty())
	{
		while (!m_stackIn.empty())//判断出是否为空
		{
			m_stackOut.push(m_stackIn.top());//不等于空则循环把(进的栈)的内容放到(出的栈)里面.
			m_stackIn.pop();
		}
	}
	m_stackOut.pop();//弹出(出的栈)的栈顶元素
	m_size--;
}
//clear
template<class T>
void MyQueue<T>::clear()//移除容器的所有数据
{
	while (!m_stackIn.empty())
	{
		m_stackIn.pop();
		m_size--;
	}
	while (!m_stackOut.empty())
	{
		m_stackOut.pop();
		m_size--;
	}
}
//clear
template<class T>
bool MyQueue<T>::empty()//判断容器是否为空
{
	if (m_stackIn.empty() && m_stackOut.empty())
	{
		return true;
	}
	return false;
}
void test01()
{
	MyQueue<int> qq;									//定义1个队列
	qq.push(1);											//放入数据1
	qq.push(2);											//放入数据2
	qq.push(3);											//放入数据3
	cout << "qq.front():" << qq.front() << endl;//1
	cout << "qq.back():" << qq.back() << endl;//3
	qq.push(4);											//放入数据4
	cout << "qq.back():" << qq.back() << endl;//4
	qq.pop();
	cout << "qq.front():" << qq.front() << endl;//2
	qq.pop();
	cout << "qq.front():" << qq.front() << endl;//3
	cout << "qq.size():" << qq.size() << endl;
	qq.pop();
	qq.pop();
	//qq.pop();
	//清空
	qq.clear();
	if (qq.empty())
	{
		cout << "队列为空" << endl;
	}
	else
	{
		cout << "队列不为空" << endl;
	}
}
int main()
{
	test01();
	/*queue<int> q;
	q.push(1);
	q.push(2);
	q.push(3);

	q.pop();
	q.pop();
	q.pop();
	cout << q.front();*/

	system("pause");
	return EXIT_SUCCESS;
}
