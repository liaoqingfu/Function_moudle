#ifndef _WIPN_WEBAPI_H_
#define _WIPN_WEBAPI_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#if defined WINDOWS 
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <windows.h>
//#include <ws2tcpip.h>
#elif defined LINUX
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

#include "NDT_Error.h"
#include "WiPN_Error.h"
#include "WiPN_StringEncDec.h"
#include "Debug_log.h"

#ifdef WINDOWS
#define SNPRINTF	_snprintf
#else
#define SNPRINTF	snprintf
#endif

// Whether to subscribe, cancel the subscription of API code to participate in the compilation
#define SUBSCRIBE_FLAG		1
// Whether will push API code in compilation
#define POST_FLAG			1

#ifdef __cplusplus
extern "C" {
#endif 			

//// Device&&Client
#define SIZE_DID                	24
//// Device
#define SIZE_iPNLicense         	12
#define SIZE_Title					32
#define SIZE_Content            	128
#define SIZE_Payload            	128
#define SIZE_SoundName          	64
#define SIZE_PostServerString   	600
//// Device&&Client
#define SIZE_UTCTString         	16
#define SIZE_RETString				32
//// Client
#define SIZE_SubscribeServerString 	600
#define SIZE_DeviceToken            200

//// Todo: Modify this for your own Web Server IP
#define SERVER_IP		"120.24.6.102"
//// Todo: Modify this for your own Web Server Port
#define SERVER_PORT		80
//// Todo: Modify this for your own URL
#define URL				"/iot/WiPN/WiPN.php"
//// -> http://WebServerIP/iot/WiPN/WiPN.php?A=iPN_StringEnc(DID)&B=iPN_StringEnc(DATA)

//// Todo: Modify this for your own QueryServer Number
//// Each customer has its own fixed QueryServer DID and Number (Specified in the deployment WiPN server)
#define QUERY_SERVER_NUMBER  		2
//// Todo: Modify this for your own QueryServer DID
#define QUERY_SERVER_DID_1 		"PPCS-014143-SBKHR"
#define QUERY_SERVER_DID_2 		"PPCS-014144-RVDKK"
//// The WiPN key to encrypt/decrypt data required
#define	STRING_ENC_DEC_KEY		"WiPN@CS2-Network"				

//// Time delay unit: ms
void mSecSleep(unsigned int ms);
//// get NDT/WiPN Error Code Info
const char *getErrorCodeInfo(int err);

// ---------------------------- WiPN Web API ---------------------------
//  WiPN_WebAPI_Query  
int WiPN_WebAPI_Query(
					const char *pDeviceDID, 				// The target device DID
					const char *QueryServerDID[], 			// QueryServer DID
					char *pPostServerString, 				// Save PostServerString
					unsigned int SizeOfPostServerString, 	// PostServerString Buf Size
					char *pSubscribeServerString, 			// Save SubscribeServerString 
					unsigned int SizeOfSubscribeServerString,// SubscribeServerString Size
					char *pUTCTString, 						// Save UTCTString 
					unsigned int SizeOfUTCTString);			// UTCTString Buf Size

// WiPN_WebAPI_Subscribe 
int WiPN_WebAPI_Subscribe(
					const char *pSubscribeServerString,	// Obtained by WiPN_WebAPI_Query
					const char *pSubCmd,			// Subscribe Cmd
					char *pRETString,				// Save SubscribeServer return info
					unsigned int SizeOfRETString,	// RETString Buf Size
					char *pUTCTString,				// Save UTCTString From SubscribeServer response
					unsigned int SizeOfUTCTString);	// UTCTString Buf Size

// WiPN_WebAPI_UnSubscribe 
int WiPN_WebAPI_UnSubscribe(
					const char *pSubscribeServerString,	// Obtained by WiPN_WebAPI_Query
					const char *pSubCmd,				// Subscribe Cmd
					char *pRETString,				// Save SubscribeServer return info
					unsigned int SizeOfRETString,	// RETString Buf Size
					char *pUTCTString,				// Save UTCTString
					unsigned int SizeOfUTCTString);	// UTCTString Buf Size
					
// WiPN_WebAPI_Post 
int WiPN_WebAPI_Post(const char *pPostServerString, // Obtained by WiPN_WebAPI_Query
					const char *pCmd,				// Push cmd
					char *pRETString, 				// Save PostServer return info
					unsigned int SizeOfRETString, 	// RETString Buf Size
					char *pUTCTString, 				// Save UTCTString From PostServer response
					unsigned int SizeOfUTCTString);	// UTCTString Buf Size
					
int WiPN_WebAPI_ChkSubscribe(
					const char *pSubscribeServerString,	// Obtained by WiPN_WebAPI_Query
					const char *pChkSubCmd,				// Check Subscribe Cmd
					char *pListString,					// Save Server return List Info
					unsigned int SizeOfListString,		// ListString Buf Size
					char *pUTCTString,					// Save UTCT From Server response
					unsigned int SizeOfUTCTString);		// UTCTString Buf Size
#ifdef __cplusplus
}
#endif 
#endif 	// _WIPN_WEBAPI_H_