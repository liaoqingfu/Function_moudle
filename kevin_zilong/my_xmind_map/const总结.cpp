#include<iostream>
using namespace std;
#define PARAM 128
const short param = 128;

namespace MySpace{   
#define num 1024    //宏常量没有命名空间之说，
}
void test(){
	//cout << MySpace::NUM << endl; //错误
	//int num = 100; //命名冲突
	cout << num << endl;
}


void func(short a){
	cout << "short!" << endl;
}
void func(int a){
	cout << "int" << endl;
}

void func1(){
	const int a = 10;
	#define A 20    //宏常量不重视作用域  #define作用域从定义到文件结束或者到#undef之前都可访问
	//#undef A  //卸载宏常量A，取消A宏定义
}
void func2(){
	//cout << "a:" << a << endl; //不可访问，超出了const int a作用域   
	cout << "A:" << A << endl; //#define作用域从定义到文件结束或者到#undef之前都可访问
}
struct Person{
	int age = 18;
};


//宏常量没有类型，所以调用了int类型重载的函数。const有类型，所以调用的short类型的函数
int main()
{
	func(PARAM);    //没法调用void func(short a)函数，打印int
	func(param);    //修改类型就可以任意调用函数，打印short
	func2();   //20
	test();   //1024

	const int con = 100;   //const修饰的量是常量，不可修改，但是可以通过指针间接修改
	int *p = (int *) &con;
	*p = 120;
	cout << con << endl;   //会发生常量折叠   打印100
	cout << &con << endl;   //有地址
	cout << *p << endl;    //内容已经修改了的，打印120
	volatile const int con1 = 150;   //增加了volatile关键字，告诉编译器不要优化 ，防止出现常量折叠
	int *p1 = (int *)&con1;
	*p1 = 200;
	cout << con1 << endl;    // 打印200 增加了volatile关键字，告诉编译器不要优化 ，防止出现常量折叠

	int con2 = 300;
	const int con3 = con2;
	int *p3 = (int *)&con3;
	*p3 = 350;
	cout << con3 << endl;  //打印350  ，接了一下，编译器就不会发生常量折叠现象

	Person person;   //定义一个结构体
	person.age = 19;  //可以修改结构体成员的值
	cout << person.age << endl;   //打印19；

	const Person person1;
	//person1.age = 20;    //不能修改，这样会报错，产量不可修改
	Person *p6 = (Person *)&person1;
	p6->age = 25;    //可以通过指针间接修改
	cout << person1.age << endl;   //打印25
	return EXIT_SUCCESS;
}


 const 是一个限定符，它用来限定一个变量或者参数不允许改变，它将一个对象转换成一个常量。 

1. const 修饰的变量不能被直接修改，但是可以通过指针的方式通过地址空间去改变，所以定义的时候必须初始化。const int a = 10; a=100; //err
const char *a="hello" *a = "hhh" //err   a="hhh" Ok 不能改变指向里面的值，可以改变指针的指向
char * const a ="hello"  *a="kk" //ok a="hhhhh";//err  //可以改变指向，但是不能改变里面的值
“引用传递”有可能改变参数a，这是我们不期望的。解决这个问题很容易，加const修饰即可，因此函数最终成为voidFunc(const A &a)。
是否应将voidFunc(int x) 改写为voidFunc(const int&x)，以便提高效率？完全没有必要，因为内部数据类型的参数不存在构造、析构的过程，而复制也非常快，“值传递”和“引用传递”的效率几乎相当。

将“const&”修饰输入参数的用法总结一下。
对于非内部数据类型的输入参数，应该将“值传递”的方式改为“const引用传递”，目的是提高效率

2. const 修饰的函数的参数，参数的值也就不能直接被改变，修饰函数参数的时候就不用去初始化了。

3. const 可以修饰函数的返回值，（值传递没啥意义，一般都是指针传递）就是在函数前面加上了const，这样的函数就不能做左值了，只能做右值，即：const int test();  test() = 10; //err 这是错的，只能做右值，可以给别的量赋值，主要是C++中用
如果返回const data,non-const pointer，返回值也必须赋给const data,non-const pointer。因为指针指向的数据是常量不能修改。

 1 const int * mallocA(){  ///const data,non-const pointer
 2     int *a=new int(2);
 3     return a;
 4 }
 5 
 6 int main()
 7 {
 8     const int *a = mallocA();
 9     ///int *b = mallocA();  ///编译错误
10     return 0;
11 }

4. const 被const修饰的函数定义体的函数能被const或者非const对象调用，但是const对象只能调用被const修饰过定义体的函数

a.const对象只能访问const成员函数,而非const对象可以访问任意的成员函数,包括const成员函数.
b.const对象的成员是不可修改的,然而const对象通过指针维护的对象却是可以修改的.
c.const成员函数不可以修改对象的数据,不管对象是否具有const性质.它在编译时,以是否修改成员数据为依据,进行检查.
e.然而加上mutable修饰符的数据成员,对于任何情况下通过任何手段都可修改,自然此时的const成员函数是可以修改它

 //4和5是一样的
5 . const修饰的成员函数不能修改任何的成员变量(mutable修饰的变量除外)
 #include <iostream>
 2 using namespace std;
 3 class Point{
 4     public :
 5     Point(int _x):x(_x){}
 6 
 7     void testConstFunction(int _x) const{
 8 
 9         ///错误，在const成员函数中，不能修改任何类成员变量
10         x=_x;
11 
12         ///错误，const成员函数不能调用非onst成员函数，因为非const成员函数可以会修改成员变量
13         modify_x(_x);
14     }
15 
16     void modify_x(int _x){
17         x=_x;
18     }
19 
20     int x;
21 };

6. 尽量以const替换#define
因为：
1．const有类型，可进行编译器类型安全检查。#define无类型，不可进行类型检查.
2．const有作用域，而#define不重视作用域