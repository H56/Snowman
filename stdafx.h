// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�: 
#include <windows.h>
#include <shellapi.h>


// C ����ʱͷ�ļ�
#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <crtdbg.h>


// C++ ��
#include <vector>
#include <fstream>
#include <string>
#include <map>

// DirectX ͷ�ļ�
#if defined(DEBUG) || defined(_DEBUG)
#define D3D_DEBUG_INFO
#endif
#pragma warning( disable:4995 )
#include <d3d9.h>
#include <d3dx9.h>
#include <D3DCommon.h>
#include <DXGI.h>



// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
