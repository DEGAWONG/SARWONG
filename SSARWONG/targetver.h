#pragma once

// ���� SDKDDKVer.h ��������õ���߰汾�� Windows ƽ̨��

// ���ҪΪ��ǰ�� Windows ƽ̨����Ӧ�ó�������� WinSDKVer.h������
// WIN32_WINNT ������ΪҪ֧�ֵ�ƽ̨��Ȼ���ٰ��� SDKDDKVer.h��

#include <SDKDDKVer.h>

# define  MAX(a,b) (((a)>(b))?(a):(b))
# define  MIN(a,b) (((a)<(b))?(a):(b))
# define  SIGN(a,b) ((b)>=0.0 ? fabs(a) : -fabs(a))
#define MAX_ITERA 60
#define MIN_DOUBLE (1e-15)
#define BMP_HEADER_MARKER	((WORD) ('M' << 8) | 'B') 
#define WNDNUM 256