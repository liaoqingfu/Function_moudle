#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Test
{
	char *buf;
	int len;
}Test;

void get_men(void * * p1)
{
	Test ** p = (Test **)p1;

	Test *tmp = (Test *)malloc(sizeof(Test));
	memset(tmp, 0, sizeof(Test));

	*p = tmp;
}

void set_buf(void *p1, char *tmp, int len)
{
	Test * p = (Test *)p1;
	p->buf = (char *)malloc(len + 1);
	strcpy(p->buf, tmp);
	p->len = len;
}

void get_buf(void *p1, char *str, int *len)
{
	Test * p = (Test *)p1;

	strcpy(str, p->buf);
	*len = p->len;
}

void free_mem(void ** p1)
{
	Test ** p = (Test **)p1;

	//先释放成员，再释放结构体变量
	Test * tmp = *p;
	free(tmp->buf);
	free(tmp);
	*p = NULL;
}
