#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct TMP
{
	char *buf;
	int len;
	char *ip;
	int aaaa;
}TMP;

void get_men(void * * p1)
{
	TMP ** p = (TMP **)p1;

	TMP *tmp = (TMP *)malloc(sizeof(TMP));
	memset(tmp, 0, sizeof(TMP));

	*p = tmp;
}

void set_buf(void *p1, char *tmp, int len)
{
	TMP * p = (TMP *)p1;
	p->buf = (char *)malloc(len + 1);
	strcpy(p->buf, tmp);
	p->len = len;
}

void get_buf(void *p1, char *str, int *len)
{
	TMP * p = (TMP *)p1;

	strcpy(str, p->buf);
	*len = p->len;
}

void free_mem(void ** p1)
{
	TMP ** p = (TMP **)p1;

	//先释放成员，再释放结构体变量
	TMP * tmp = *p;
	free(tmp->buf);
	free(tmp);
	*p = NULL;
}
