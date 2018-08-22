#include<iostream>
using namespace std;

namespace A{      //?	命名空间只能全局范围内定义
	int a = 10;
	int test()   //可以有函数
	{
		int b = 100;
		return b;
	}
}
namespace B{   //?	命名空间只能全局范围内定义
	int a = 20;
	int test(int b)
	{
		return b;
	}
}

namespace C{   //命名空间可以嵌套命名空间，访问的时候作用域要加清楚
	int a = 30;
	namespace D{
		int a = 50;
	}
}
namespace D{
	void func(){ cout << "func()" << endl; }
	void func(int a){ cout << "func(int "<<a<<")" << endl; }
	void func(int a, int b){ cout << "func(int " << a << ",int " << b << ")" << endl; }
}
int main()
{
	//方法一：直接通过作用域访问命名空间里的成员
	cout << A::a << endl;   //10             //cout是c++中的标准输出流，endl是输出换行并刷新缓冲区。
	cout << A::test() << endl;   //100

	//方法二：可以通过声明访问空间内的成员,using关键字
	/*using B::a;  *///声明后可以直接访问a变量了,但是需要注意，函数内在定义a就会出现重定义,从声明开始以后的a都是B::a 里面的 a
	/*a = 123;*/  //也可以直接修改a的值
	//cout << a << endl;  //20

	//方法三：声明整个命名空间，这样就可以访问整个空间内的成员了
	//using namespace C;
	//using namespace D;
	//cout << a << endl;  //B::a 如果想在里面访问C中的话，声明B的时候需要增加作用域{}
	{
		using B::a;
		using B::test;    //函数命名空间声明，不需要加（）括号
		cout << a << endl;  //20 作用域内{} 即大括号内，声明有效，大括号外，声明无效
		cout << test(90) << endl;
	}            
	//using namespace C;
	//cout << a << endl;   //30
	{
		using namespace C;
		cout << a << endl;   //30
	}
	//using namespace D;  //这样就会报错，分不清是C里面的还是D里面的，这样声明是有问题的
	using namespace C::D;   //重名了，不明确的符号，如果想输出，前面声明就得加上{}大括号
	cout << a << endl;
	//命名空间可以修改别名
	namespace hk = B;   //将B命名改为hk命名，注意等号后面写改前的命名
	cout << hk::test(1000) << endl;  //1000

	//using声明碰到函数重载,只需要声明一次 如果命名空间包含一组用相同名字重载的函数，using声明就声明了这个重载函数的所有集合。
	using  D::func;
	func(); //打印结果 func()
	func(15);  //打印结果 func(int 15)
	func(55,55);  //打印结果 func(int 55, int 55)
	return EXIT_SUCCESS;    //EXIT_SUCCESS 就是0，转到定义查看 #define EXIT_SUCCESS    0   #define EXIT_FAILURE    1
}