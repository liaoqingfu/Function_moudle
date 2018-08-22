#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "CSocketProtocol.h"

void Framework(CSocketProtocol *p, char *send_buf, int send_len, char *recv_buf, int *recv_len)
{
	if (p == NULL)
	{
		printf("p == NULL\n");
		return;
	}
	void * hd;
	//初始化环境
	p->init(&hd);

	//发送信息
	p->send(hd, send_buf, send_len);

	//接收信息
	p->recv(hd, recv_buf, recv_len);

	//清理环境
	p->destroy(&hd);
}

int main(void)
{
	CSocketProtocol sp;
	SetSp(&sp, NULL, NULL, NULL, NULL);

	char send_buf[] = "are u ok?";
	int send_len = strlen(send_buf);
	char recv_buf[100];
	int recv_len;

	Framework(NULL, send_buf, send_len, recv_buf, &recv_len); //测试通信接口
	//printf("recv_buf = %s, recv_len = %d\n", recv_buf, recv_len);


	printf("\n");
	system("pause");
	return 0;
}
