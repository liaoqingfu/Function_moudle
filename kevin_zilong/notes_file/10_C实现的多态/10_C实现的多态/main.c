#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "CSocketProtocol.h"
#include "Framework.h"
#include "sock2.h"
#include "des.h"

int main(void)
{
	CSocketProtocol sp;
	SetSp(&sp, get_men, set_buf, get_buf, free_mem); //ͨ�Žӿڳ�ʼ��

	CEncDesProtocol ed;
	SetEd(&ed, DesEnc, DesDec); //�ӽ��ܽӿڳ�ʼ��

	char send_buf[] = "are u ok?";
	int send_len = strlen(send_buf);
	char recv_buf[100];
	int recv_len;

	FrameworkPro(&sp, &ed, send_buf, send_len, recv_buf, &recv_len); //����ͨ�Žӿ�
	recv_buf[recv_len] = '\0';
	printf("recv_buf = %s, recv_len = %d\n", recv_buf, recv_len);


	printf("\n");
	system("pause");
	return 0;
}


int main01(void)
{
	CSocketProtocol sp;
	SetSp(&sp, get_men, set_buf, get_buf, free_mem);

	char send_buf[] = "are u ok?";
	int send_len = strlen(send_buf);
	char recv_buf[100];
	int recv_len;

	Framework(&sp, send_buf, send_len, recv_buf, &recv_len); //����ͨ�Žӿ�
	printf("recv_buf = %s, recv_len = %d\n", recv_buf, recv_len);


	printf("\n");
	system("pause");
	return 0;
}
