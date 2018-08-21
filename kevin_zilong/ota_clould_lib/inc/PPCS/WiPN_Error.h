#ifndef _WIPN_ERROR_H_
#define _WIPN_ERROR_H_

//// for WiPN API
#define WiPN_ERROR_InvalidParameter                 	-1000 // Invalid Parameter
#define WiPN_ERROR_iPNStringEncFailed               	-1001 // Encryption failed
#define WiPN_ERROR_iPNStringDncFailed               	-1002 // Decryption failed
#define WiPN_ERROR_GetPostServerStringItemFailed    	-1003 // Failed to get PostServerString Item
#define WiPN_ERROR_GetSubscribeServerStringItemFailed  	-1004 // Failed to get SubscribeServerString Item
#define WiPN_ERROR_GetUTCTStringItemFailed             	-1005 // Failed to get UTCTString Item
#define WiPN_ERROR_GetNumberFromPostServerStringFailed  	-1006 // Failed to get Number From PostServerString
#define WiPN_ERROR_GetNumberFromSubscribeServerStringFailed -1007 // // Failed to get Number From SubscribeServerString
#define WiPN_ERROR_GetDIDFromPostServerStringFailed        	-1008 // Failed to get DID From PostServerString
#define WiPN_ERROR_GetDIDFromSubscribeServerStringFailed    -1009 // Failed to get DID From SubscribeServer
#define WiPN_ERROR_GetRETStringItemFailed           	-1010 // Failed to get RETString Item
#define WiPN_ERROR_MallocFailed                     	-1011 // Allocates memory Failed
#define WiPN_ERROR_ExceedMaxSize                		-1012 // More than most Byte size, Possible title, content or cmd length exceeds the limit

//// for WiPN Web API 
#define WiPN_ERROR_TimeOut                 				-1013 // TimeOut
#define WiPN_ERROR_SocketCreateFailed					-1014 // Create socket Failed
#define WiPN_ERROR_SocketConnectFailed					-1015 // TCP Connect Failed
#define WiPN_ERROR_InvalidSocketID						-1016 // 无效的 socket 
#define WiPN_ERROR_SetSockOptFailed						-1017 // SetSockOpt error 
#define WiPN_ERROR_SocketWriteFailed					-1018 // TCP Write/Send Failed
#define WiPN_ERROR_SocketReadFailed						-1019 // TCP Read/Recv Failed
#define WiPN_ERROR_RemoteSocketClosed					-1020 // remote close Socket
#define WiPN_ERROR_SelectError							-1021 // select return -1
#define WiPN_ERROR_GetContentFromHttpRetDataFailed		-1022 // Get HTTP Body data failed
#define WiPN_ERROR_GetNDTRETItemFailed           		-1023 // Get NDTRET failed
//// 2016.08.30
#define WiPN_ERROR_DeviceTokenIsEmpty					-1024 // DeviceToken is empty 
#endif 	// _WIPN_ERROR_H_