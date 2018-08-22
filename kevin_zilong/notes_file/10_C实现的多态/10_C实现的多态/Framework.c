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
	//��ʼ������
	p->init(&hd);

	//������Ϣ
	p->send(hd, send_buf, send_len);

	//������Ϣ
	p->recv(hd, recv_buf, recv_len);

	//������
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

	//��ʼ������
	p->init(&hd);

	//������Ϣǰ���ȼ���
	pe->enc(send_buf, send_len, buf, &len);

	p->send(hd, buf, len);

	//������Ϣ����Ҫ����
	memset(buf, 0, sizeof(buf));
	len = 0;
	p->recv(hd, buf, &len);

	//����
	pe->dec(buf, len, recv_buf, recv_len);

	//������
	p->destroy(&hd);
}