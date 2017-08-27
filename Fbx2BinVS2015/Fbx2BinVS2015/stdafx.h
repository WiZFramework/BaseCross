
// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Windows ヘッダーから使用されていない部分を除外します。
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 一部の CString コンストラクターは明示的です。

// 一般的で無視しても安全な MFC の警告メッセージの一部の非表示を解除します。
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分


#include <afxdisp.h>        // MFC オートメーション クラス



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC の Internet Explorer 4 コモン コントロール サポート
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC におけるリボンとコントロール バーのサポート


// C ランタイム ヘッダー ファイル
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <wrl.h>

#include <initguid.h>
#include <assert.h>
#include <wchar.h>
#include <mmsystem.h>
#include <atlbase.h>
#include <commctrl.h> // for InitCommonControls() 
#include <shellapi.h> // for ExtractIcon()
#include <new.h>      // for placement new
#include <shlobj.h>
#include <math.h>
#include <limits.h>
#include <time.h>
#include <Winhttp.h>


#include <xaudio2.h>	//サウンド
#include <xaudio2fx.h>
#include <mmreg.h>
#include <mfidl.h>
#include <mfapi.h>
#include <mfreadwrite.h>
#include <msxml6.h>


// STL
#include <vector>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <functional>
#include <algorithm>
#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <sstream>
#include <string>
#include <new>
#include <memory>
#include <exception>
#include <stdexcept>
#include <type_traits>
#include <typeindex>

using namespace std;

#include <d3d11_2.h>
#include <d2d1_2.h>
#include <d2d1effects_1.h>
#include <dwrite_2.h>
#include <wincodec.h>


#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <DirectXTex.h>
#define FBXSDK_NEW_API
#include <fbxsdk.h>

// XInput includes
#include <xinput.h>


#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dwrite.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "comctl32.lib" )

//DirectXTx
#pragma comment( lib, "DirectXTex.lib" )
//FBX
#pragma comment(lib, "libfbxsdk-mt.lib")
// XInput
#pragma comment( lib, "XInput.lib" )
//オーディオ
#pragma comment( lib, "xaudio2.lib" )
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
//WinHTTP
#pragma comment( lib, "Winhttp.lib" )


#pragma warning(disable:4192)
#import "msxml6.dll" named_guids raw_interfaces_only
#pragma warning(default:4192)



namespace DirectX
{
#if (DIRECTXMATH_VERSION < 305) && !defined(XM_CALLCONV)
#define XM_CALLCONV __fastcall
	typedef const XMVECTOR& HXMVECTOR;
	typedef const XMMATRIX& FXMMATRIX;
#endif
}

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Details;

#include "../../Libs/BaseLib/Dx11Lib/Common.h"
#include "../../Libs/BaseLib/Dx11FullLib/FullCommon.h"
using namespace basecross::bsm;





#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


