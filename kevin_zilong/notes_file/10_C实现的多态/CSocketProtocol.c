#include "CSocketProtocol.h"
#include <stdio.h>

void SetSp(CSocketProtocol *p, PINIT pi, PSEND ps, PRECV pr, PDESTROY pd)
{
	if (p == NULL || pi == NULL || ps == NULL || pr == NULL || pd == NULL)
	{
		printf("p == NULL || pi == NULL || ps == NULL || pr == NULL || pd == NULL\n");
		return;
	}
}