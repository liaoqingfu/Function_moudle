#pragma once

//����ָ������
typedef void(*PINIT)(void **);
typedef void(*PSEND)(void *, char *, int );
typedef void(*PRECV)(void *, char *, int *);
typedef void(*PDESTROY)(void **);

typedef struct CSocketProtocol
{
	//�ڲ���Ա�Ǻ���ָ�����
	//��ʼ������
	PINIT init;

	//������Ϣ
	PSEND send;

	//������Ϣ
	PRECV recv;

	//������
	PDESTROY destroy;

}CSocketProtocol;

//��CSocketProtocol�ṹ��ĳ�Ա������ֵ
extern void SetSp(CSocketProtocol *p, PINIT pi, PSEND ps, PRECV pr, PDESTROY pd);
