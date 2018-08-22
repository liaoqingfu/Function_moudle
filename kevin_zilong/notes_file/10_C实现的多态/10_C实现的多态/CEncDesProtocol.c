#include "CEncDesProtocol.h"
#include <stdio.h>

void SetEd(CEncDesProtocol *p, PENC pe, PDEC pd)
{
	if (NULL == p || NULL == pe || NULL == pd)
	{
		printf("NULL == p || NULL == pe || NULL == pd\n");
		return;
	}

	p->dec = pd;
	p->enc = pe;
}