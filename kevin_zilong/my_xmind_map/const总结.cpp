#include<iostream>
using namespace std;
#define PARAM 128
const short param = 128;

namespace MySpace{   
#define num 1024    //�곣��û�������ռ�֮˵��
}
void test(){
	//cout << MySpace::NUM << endl; //����
	//int num = 100; //������ͻ
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
	#define A 20    //�곣��������������  #define������Ӷ��嵽�ļ��������ߵ�#undef֮ǰ���ɷ���
	//#undef A  //ж�غ곣��A��ȡ��A�궨��
}
void func2(){
	//cout << "a:" << a << endl; //���ɷ��ʣ�������const int a������   
	cout << "A:" << A << endl; //#define������Ӷ��嵽�ļ��������ߵ�#undef֮ǰ���ɷ���
}
struct Person{
	int age = 18;
};


//�곣��û�����ͣ����Ե�����int�������صĺ�����const�����ͣ����Ե��õ�short���͵ĺ���
int main()
{
	func(PARAM);    //û������void func(short a)��������ӡint
	func(param);    //�޸����;Ϳ���������ú�������ӡshort
	func2();   //20
	test();   //1024

	const int con = 100;   //const���ε����ǳ����������޸ģ����ǿ���ͨ��ָ�����޸�
	int *p = (int *) &con;
	*p = 120;
	cout << con << endl;   //�ᷢ�������۵�   ��ӡ100
	cout << &con << endl;   //�е�ַ
	cout << *p << endl;    //�����Ѿ��޸��˵ģ���ӡ120
	volatile const int con1 = 150;   //������volatile�ؼ��֣����߱�������Ҫ�Ż� ����ֹ���ֳ����۵�
	int *p1 = (int *)&con1;
	*p1 = 200;
	cout << con1 << endl;    // ��ӡ200 ������volatile�ؼ��֣����߱�������Ҫ�Ż� ����ֹ���ֳ����۵�

	int con2 = 300;
	const int con3 = con2;
	int *p3 = (int *)&con3;
	*p3 = 350;
	cout << con3 << endl;  //��ӡ350  ������һ�£��������Ͳ��ᷢ�������۵�����

	Person person;   //����һ���ṹ��
	person.age = 19;  //�����޸Ľṹ���Ա��ֵ
	cout << person.age << endl;   //��ӡ19��

	const Person person1;
	//person1.age = 20;    //�����޸ģ������ᱨ�����������޸�
	Person *p6 = (Person *)&person1;
	p6->age = 25;    //����ͨ��ָ�����޸�
	cout << person1.age << endl;   //��ӡ25
	return EXIT_SUCCESS;
}


 const ��һ���޶������������޶�һ���������߲���������ı䣬����һ������ת����һ�������� 

1. const ���εı������ܱ�ֱ���޸ģ����ǿ���ͨ��ָ��ķ�ʽͨ����ַ�ռ�ȥ�ı䣬���Զ����ʱ������ʼ����const int a = 10; a=100; //err
const char *a="hello" *a = "hhh" //err   a="hhh" Ok ���ܸı�ָ�������ֵ�����Ըı�ָ���ָ��
char * const a ="hello"  *a="kk" //ok a="hhhhh";//err  //���Ըı�ָ�򣬵��ǲ��ܸı������ֵ
�����ô��ݡ��п��ܸı����a���������ǲ������ġ���������������ף���const���μ��ɣ���˺������ճ�ΪvoidFunc(const A &a)��
�Ƿ�Ӧ��voidFunc(int x) ��дΪvoidFunc(const int&x)���Ա����Ч�ʣ���ȫû�б�Ҫ����Ϊ�ڲ��������͵Ĳ��������ڹ��졢�����Ĺ��̣�������Ҳ�ǳ��죬��ֵ���ݡ��͡����ô��ݡ���Ч�ʼ����൱��

����const&����������������÷��ܽ�һ�¡�
���ڷ��ڲ��������͵����������Ӧ�ý���ֵ���ݡ��ķ�ʽ��Ϊ��const���ô��ݡ���Ŀ�������Ч��

2. const ���εĺ����Ĳ�����������ֵҲ�Ͳ���ֱ�ӱ��ı䣬���κ���������ʱ��Ͳ���ȥ��ʼ���ˡ�

3. const �������κ����ķ���ֵ����ֵ����ûɶ���壬һ�㶼��ָ�봫�ݣ������ں���ǰ�������const�������ĺ����Ͳ�������ֵ�ˣ�ֻ������ֵ������const int test();  test() = 10; //err ���Ǵ�ģ�ֻ������ֵ�����Ը��������ֵ����Ҫ��C++����
�������const data,non-const pointer������ֵҲ���븳��const data,non-const pointer����Ϊָ��ָ��������ǳ��������޸ġ�

 1 const int * mallocA(){  ///const data,non-const pointer
 2     int *a=new int(2);
 3     return a;
 4 }
 5 
 6 int main()
 7 {
 8     const int *a = mallocA();
 9     ///int *b = mallocA();  ///�������
10     return 0;
11 }

4. const ��const���εĺ���������ĺ����ܱ�const���߷�const������ã�����const����ֻ�ܵ��ñ�const���ι�������ĺ���

a.const����ֻ�ܷ���const��Ա����,����const������Է�������ĳ�Ա����,����const��Ա����.
b.const����ĳ�Ա�ǲ����޸ĵ�,Ȼ��const����ͨ��ָ��ά���Ķ���ȴ�ǿ����޸ĵ�.
c.const��Ա�����������޸Ķ��������,���ܶ����Ƿ����const����.���ڱ���ʱ,���Ƿ��޸ĳ�Ա����Ϊ����,���м��.
e.Ȼ������mutable���η������ݳ�Ա,�����κ������ͨ���κ��ֶζ����޸�,��Ȼ��ʱ��const��Ա�����ǿ����޸���

 //4��5��һ����
5 . const���εĳ�Ա���������޸��κεĳ�Ա����(mutable���εı�������)
 #include <iostream>
 2 using namespace std;
 3 class Point{
 4     public :
 5     Point(int _x):x(_x){}
 6 
 7     void testConstFunction(int _x) const{
 8 
 9         ///������const��Ա�����У������޸��κ����Ա����
10         x=_x;
11 
12         ///����const��Ա�������ܵ��÷�onst��Ա��������Ϊ��const��Ա�������Ի��޸ĳ�Ա����
13         modify_x(_x);
14     }
15 
16     void modify_x(int _x){
17         x=_x;
18     }
19 
20     int x;
21 };

6. ������const�滻#define
��Ϊ��
1��const�����ͣ��ɽ��б��������Ͱ�ȫ��顣#define�����ͣ����ɽ������ͼ��.
2��const�������򣬶�#define������������