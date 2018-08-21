#ifndef _WIPN_API_H_
#define _WIPN_API_H_

#include <stdio.h>
#include <time.h>
#include <errno.h>

#include "NDT_Type.h"
#include "NDT_API.h"
#include "NDT_Error.h"
#include "WiPN_Error.h"
#include "WiPN_StringEncDec.h"

#define SNPRINTF	snprintf

// Whether to subscribe, cancel the subscription of API code to participate in the compilation
#define SUBSCRIBE_FLAG		1
// Whether will push API code in compilation
#define POST_FLAG			1

#ifdef __cplusplus
extern "C" {
#endif 

//// for Device && Client		
#define SIZE_DID                	(24)
#define SIZE_InitString         	(200)
#define SIZE_AES128Key          	(20)
//// for Device
#define SIZE_NDTLicense         	(12)
#define SIZE_iPNLicense         	(12)
//// for WiPN API Device&&Client 
#define SIZE_UTCTString         	(16)
#define SIZE_RETString				(32)
//// for WiPN API Device Post
#define SIZE_PostServerString   	(600)
#define SIZE_Title					(32)
#define SIZE_Content            	(128)
#define SIZE_Payload            	(128)
#define SIZE_SoundName          	(64)
//// for WiPN API Client Subscribe
#define SIZE_SubscribeServerString 	(600)
#define SIZE_DeviceToken            (200)		

//// Todo: Modify this for your own QueryServer Number
//// Each customer has its own fixed QueryServer DID and Number (Specified in the deployment WiPN server)
#define QUERY_SERVER_NUMBER  		(6)
//// Todo: Modify this for your own QueryServer DID


#define QUERY_SERVER_DID_1 		"SECCAMA-000003-WJTKZ"
#define QUERY_SERVER_DID_2 		"SECCAMA-000004-DJKCF"
#define QUERY_SERVER_DID_3 		"SECCAMA-000005-CUDDD"
#define QUERY_SERVER_DID_4 		"SECCAMA-000006-GJTJZ"
#define QUERY_SERVER_DID_5 		"SECCAMA-000007-FZFXK"
#define QUERY_SERVER_DID_6 		"SECCAMA-000008-YDKKL"

//// The WiPN key to encrypt/decrypt data required
#define	STRING_ENC_DEC_KEY		"ZXSecurity17Cam@"	
#define	PUSH_AES128_KEY		    "ZXSecurity17Cam@"		

//// Time delay unit: ms
//void mSecSleep(unsigned int ms);
//// get NDT/WiPN Error Code Info
const char *getErrorCodeInfo(int err);
//// Count Time of use
//unsigned int MyGetTickCount();

//// -------------------------------- WiPN API --------------------------------
////  WiPN_Query: Device&&Client
INT32 WiPN_Query(const CHAR *pDeviceDID, 			// The target device DID
				const CHAR *QueryServerDID[], 		// QueryServer DID
				CHAR *pPostServerString, 			// Save PostServerString
				UINT32 SizeOfPostServerString, 		// PostServerString Buf Size
				CHAR *pSubscribeServerString, 		// Save SubscribeServerString 
				UINT32 SizeOfSubscribeServerString, // SubscribeServerString Buf Size
				CHAR *pUTCTString, 					// Save UTCT From QueryServer response
				UINT32 SizeOfUTCTString);			// UTCTString Buf Size
				
//// WiPN_QueryByServer: Device&&Client
INT32 WiPN_QueryByServer(
				const CHAR *pServerString,			// cross-platform Initstring
				const CHAR *pAES128Key,				// cross-platform AES128Key
				const CHAR *pDeviceDID, 			// The target device DID
				const CHAR *QueryServerDID[], 		// QueryServer DID
				CHAR *pPostServerString, 			// Save PostServerString
				UINT32 SizeOfPostServerString, 		// PostServerString Buf Size
				CHAR *pSubscribeServerString, 		// Save SubscribeServerString 
				UINT32 SizeOfSubscribeServerString, // SubscribeServerString Buf Size
				CHAR *pUTCTString, 					// Save UTCT From QueryServer response
				UINT32 SizeOfUTCTString);			// UTCTString Buf Size
				
//// WiPN_Subscribe: Client
INT32 WiPN_Subscribe(
				const CHAR *pSubscribeServerString,	// Obtained by WiPN_Query
				const CHAR *pSubCmd,				// Subscribe Cmd
				CHAR *pRETString,					// Save SubscribeServer return info
				UINT32 SizeOfRETString,				// RETString Buf Size
				CHAR *pUTCTString,					// Save UTCT From SubscribeServer response
				UINT32 SizeOfUTCTString);			// UTCTString Buf Size

//// WiPN_SubscribeByServer
INT32 WiPN_SubscribeByServer(
				const CHAR *pServerString,			// cross-platform Initstring
				const CHAR *pAES128Key,				// cross-platform AES128Key
				const CHAR *pSubscribeServerString,	// Obtained by WiPN_Query
				const CHAR *pSubCmd,				// Subscribe Cmd
				CHAR *pRETString,					// Save SubscribeServer return info
				UINT32 SizeOfRETString,				// RETString Buf Size
				CHAR *pUTCTString,					// Save UTCT From SubscribeServer response
				UINT32 SizeOfUTCTString);			// UTCTString Buf Size			
				
//// WiPN_UnSubscribe: Client
INT32 WiPN_UnSubscribe(
				const CHAR *pSubscribeServerString,	// Obtained by WiPN_Query
				const CHAR *pSubCmd,				// Subscribe Cmd
				CHAR *pRETString,					// Save SubscribeServer return info
				UINT32 SizeOfRETString,				// RETString Buf Size
				CHAR *pUTCTString,					// Save UTCT From SubscribeServer response
				UINT32 SizeOfUTCTString);			// UTCTString Buf Size

//// WiPN_UnSubscribeByServer
INT32 WiPN_UnSubscribeByServer(
				const CHAR *pServerString,			// cross-platform Initstring
				const CHAR *pAES128Key,				// cross-platform AES128Key
				const CHAR *pSubscribeServerString,	// Obtained by WiPN_Query
				const CHAR *pSubCmd,				// Subscribe Cmd
				CHAR *pRETString,					// Save SubscribeServer return info
				UINT32 SizeOfRETString,				// RETString Buf Size
				CHAR *pUTCTString,					// Save UTCT From SubscribeServer response
				UINT32 SizeOfUTCTString);			// UTCTString Buf Size				
				
//// WiPN_Post: Device
INT32 WiPN_Post(const CHAR *pPostServerString, 		// Obtained by WiPN_Query
				const CHAR *pCmd,					// Push cmd
				CHAR *pRETString, 					// Save PostServer return info
				UINT32 SizeOfRETString, 			// RETString Buf Size
				CHAR *pUTCTString, 					// Save UTCT From PostServer response
				UINT32 SizeOfUTCTString);			// UTCTString Buf Size				
				
//// WiPN_PostByServer: Device
INT32 WiPN_PostByServer(
				const CHAR *pServerString,			// cross-platform Initstring
				const CHAR *pAES128Key,				// cross-platform AES128Key
				const CHAR *pPostServerString, 		// Obtained by WiPN_Query
				const CHAR *pCmd,					// Push cmd
				CHAR *pRETString, 					// Save PostServer return info
				UINT32 SizeOfRETString, 			// RETString Buf Size
				CHAR *pUTCTString, 					// Save UTCT From PostServer response
				UINT32 SizeOfUTCTString);			// UTCTString Buf Size

//// WiPN_ChkSubscribe				
INT32 WiPN_ChkSubscribe(
				const CHAR *pSubscribeServerString,	// Obtained by WiPN_Query
				const CHAR *pChkSubCmd,				// Check Subscribe Cmd
				CHAR *pListString,					// Save Server return List Info
				UINT32 SizeOfListString,			// ListString Buf Size
				CHAR *pUTCTString,					// Save UTCT From Server response
				UINT32 SizeOfUTCTString);			// UTCTString Buf Size

//// WiPN_ChkSubscribeByServer
INT32 WiPN_ChkSubscribeByServer(
				const CHAR *pServerString,			// cross-platform Initstring
				const CHAR *pAES128Key,				// cross-platform AES128Key
				const CHAR *pSubscribeServerString,	// Obtained by WiPN_Query
				const CHAR *pChkSubCmd,				// Check Subscribe Cmd
				CHAR *pListString,					// Save Server return ListInfo
				UINT32 SizeOfListString,			// ListString Buf Size
				CHAR *pUTCTString,					// Save UTCT From Server response
				UINT32 SizeOfUTCTString);			// UTCTString Buf Size
//// ------------------------------ WiPN API End ------------------------------

#ifdef __cplusplus
}
#endif 
#endif 	// _WIPN_API_H_
