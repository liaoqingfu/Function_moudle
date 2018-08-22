#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

int main()
{
	char buf[100];
	//跳过整型数据  %*s或%*d	跳过数据
	sscanf("1234 5678", "%*d %s", buf);
	printf("buf1=%s\n", buf);  //5678

	//读指定宽度的数据   %[width]s	读指定宽度的数据
	sscanf("12345678", "%4s", buf);
	printf("buf2=%s\n", buf); //1234

	//取仅包含指定字符集的字符串  %[a-z]	匹配a到z中任意字符(尽可能多的匹配)  %[aBc]	匹配a、B、c中一员，贪婪性
	sscanf("123abcABC", "%[1-9a-z]", buf);
	printf("buf3=%s\n", buf);  //123abc

	//取到指定字符集为止的字符串   %[^a-z]	表示读取除a-z以外的所有字符
	sscanf("123abcABC", "%[^A-Z]", buf);
	printf("buf4=%s\n", buf);  //123abc

	//取到指定字符为止的字符串  %[^a] 	匹配非a的任意字符，贪婪性
	sscanf("123456 abcdedf", "%[^ ]", buf);
	printf("buf5=%s\n", buf);  // 123456

	char src[] = "mike,jiang,250,sb";
	//提取,前面的内容，逗号不要
	sscanf(src, "%[^,]", buf);
	printf("buf = %s\n", buf); //mike

	//第一个逗号前面的内容跳过（不要）,逗号本身也不要   %*s或%*d	跳过数据
	//提取第二逗号前面的内容
	sscanf(src, "%*[^,],%[^,]", buf);
	printf("buf = %s\n", buf);  //jiang

	return 0;

}