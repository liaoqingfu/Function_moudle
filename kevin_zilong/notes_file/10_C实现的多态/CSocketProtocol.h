#pragma once

//函数指针类型
typedef void(*PINIT)(void **);
typedef void(*PSEND)(void *, char *, int );
typedef void(*PRECV)(void *, char *, int *);
typedef void(*PDESTROY)(void **);

typedef struct CSocketProtocol
{
	//内部成员是函数指针变量
	//初始化环境
	PINIT init;

	//发送信息
	PSEND send;

	//接收信息
	PRECV recv;

	//清理环境
	PDESTROY destroy;

}CSocketProtocol;

//给CSocketProtocol结构体的成员变量赋值
extern void SetSp(CSocketProtocol *p, PINIT pi, PSEND ps, PRECV pr, PDESTROY pd);
