#include "CSocketProtocol.h"
#include "CEncDesProtocol.h"
#include <stdio.h>
#include <string.h>

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

void FrameworkPro(CSocketProtocol *p, CEncDesProtocol *pe, char *send_buf, int send_len, char *recv_buf, int *recv_len)
{
	if (p == NULL || pe == NULL)
	{
		printf("p == NULL\n");
		return;
	}
	void * hd;
	unsigned char buf[128] = { 0 };
	int len = 0;

	//初始化环境
	p->init(&hd);

	//发送信息前，先加密
	pe->enc(send_buf, send_len, buf, &len);

	p->send(hd, buf, len);

	//接收信息后，需要解密
	memset(buf, 0, sizeof(buf));
	len = 0;
	p->recv(hd, buf, &len);

	//解密
	pe->dec(buf, len, recv_buf, recv_len);

	//清理环境
	p->destroy(&hd);
}