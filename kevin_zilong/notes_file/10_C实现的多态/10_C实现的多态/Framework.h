#pragma once
#include "CSocketProtocol.h"
#include "CEncDesProtocol.h"

extern void Framework(CSocketProtocol *p, char *send_buf, int send_len, char *recv_buf, int *recv_len);

extern void FrameworkPro(CSocketProtocol *p, CEncDesProtocol *pe, char *send_buf, int send_len, char *recv_buf, int *recv_len);