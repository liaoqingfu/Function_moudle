#pragma once

typedef int(*PENC)(unsigned char *, int, unsigned char *, int *);
typedef int(*PDEC)(unsigned char *, int, unsigned char *, int *);

typedef struct CEncDesProtocol
{
	//º”√‹
	PENC enc;

	//Ω‚√‹
	PDEC dec;

}CEncDesProtocol;

extern void SetEd(CEncDesProtocol *p, PENC pe, PDEC pd);