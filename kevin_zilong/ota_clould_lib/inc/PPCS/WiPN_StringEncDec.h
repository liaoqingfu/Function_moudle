/*
//  CS2 Network LIMITED All right reserved.
//
//  WiPN_StringEncDec.h: The Web iPN String Enc / Dec include file
//  Author: Charlie Chang (sengfu@gmail.com)
*/
#ifndef _zx_WIPN_STRINGENC_DEC_H_
#define _zx_WIPN_STRINGENC_DEC_H_

#ifdef __cplusplus
extern "C" {
#endif 

int iPN_StringEnc(const char *keystr, const char *src, char *dest, unsigned int maxsize);
int iPN_StringDnc(const char *keystr, const char *src, char *dest, unsigned int maxsize);
void Base64_Decode(const char *b64src, char *dststr);
void Base64_Encode(const char *scrstr, char *b64dst);

#ifdef __cplusplus
}
#endif 
#endif 	// _zx_WIPN_STRINGENC_DEC_H_
