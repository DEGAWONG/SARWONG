#pragma once

// 包括 SDKDDKVer.h 将定义可用的最高版本的 Windows 平台。

// 如果要为以前的 Windows 平台生成应用程序，请包括 WinSDKVer.h，并将
// WIN32_WINNT 宏设置为要支持的平台，然后再包括 SDKDDKVer.h。

#include <SDKDDKVer.h>

# define  MAX(a,b) (((a)>(b))?(a):(b))
# define  MIN(a,b) (((a)<(b))?(a):(b))
# define  SIGN(a,b) ((b)>=0.0 ? fabs(a) : -fabs(a))
#define MAX_ITERA 60
#define MIN_DOUBLE (1e-15)
#define BMP_HEADER_MARKER	((WORD) ('M' << 8) | 'B') 
#define WNDNUM 256