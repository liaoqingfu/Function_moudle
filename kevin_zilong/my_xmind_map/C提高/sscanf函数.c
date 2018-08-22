#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>

int main()
{
	char buf[100];
	//������������  %*s��%*d	��������
	sscanf("1234 5678", "%*d %s", buf);
	printf("buf1=%s\n", buf);  //5678

	//��ָ����ȵ�����   %[width]s	��ָ����ȵ�����
	sscanf("12345678", "%4s", buf);
	printf("buf2=%s\n", buf); //1234

	//ȡ������ָ���ַ������ַ���  %[a-z]	ƥ��a��z�������ַ�(�����ܶ��ƥ��)  %[aBc]	ƥ��a��B��c��һԱ��̰����
	sscanf("123abcABC", "%[1-9a-z]", buf);
	printf("buf3=%s\n", buf);  //123abc

	//ȡ��ָ���ַ���Ϊֹ���ַ���   %[^a-z]	��ʾ��ȡ��a-z����������ַ�
	sscanf("123abcABC", "%[^A-Z]", buf);
	printf("buf4=%s\n", buf);  //123abc

	//ȡ��ָ���ַ�Ϊֹ���ַ���  %[^a] 	ƥ���a�������ַ���̰����
	sscanf("123456 abcdedf", "%[^ ]", buf);
	printf("buf5=%s\n", buf);  // 123456

	char src[] = "mike,jiang,250,sb";
	//��ȡ,ǰ������ݣ����Ų�Ҫ
	sscanf(src, "%[^,]", buf);
	printf("buf = %s\n", buf); //mike

	//��һ������ǰ���������������Ҫ��,���ű���Ҳ��Ҫ   %*s��%*d	��������
	//��ȡ�ڶ�����ǰ�������
	sscanf(src, "%*[^,],%[^,]", buf);
	printf("buf = %s\n", buf);  //jiang

	return 0;

}