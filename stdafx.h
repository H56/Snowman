// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件: 
#include <windows.h>
#include <shellapi.h>


// C 运行时头文件
#define _CRTDBG_MAP_ALLOC
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <crtdbg.h>


// C++ 库
#include <vector>
#include <fstream>
#include <string>
#include <map>

// DirectX 头文件
#if defined(DEBUG) || defined(_DEBUG)
#define D3D_DEBUG_INFO
#endif
#pragma warning( disable:4995 )
#include <d3d9.h>
#include <d3dx9.h>
#include <D3DCommon.h>
#include <DXGI.h>



// TODO:  在此处引用程序需要的其他头文件
