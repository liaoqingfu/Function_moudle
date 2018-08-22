 const 是一个限定符，它用来限定一个变量或者参数不允许改变，它将一个对象转换成一个常量。 

1. const 修饰的变量不能被直接修改，但是可以通过指针的方式通过地址空间去改变，所以定义的时候必须初始化。const int a = 10; a=100; //err
const char *a="hello" *a = "hhh" //err   a="hhh" Ok 不能改变指向里面的值，可以改变指针的指向
char * const a ="hello"  *a="kk" //ok a="hhhhh";//err
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