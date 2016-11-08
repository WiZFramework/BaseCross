
// Fbx2BinVS2015.h : Fbx2BinVS2015 アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CFbx2BinVS2015App:
// このクラスの実装については、Fbx2BinVS2015.cpp を参照してください。
//

class CFbx2BinVS2015App : public CWinApp
{
	HWND m_DxHWnd;
public:
	CFbx2BinVS2015App();
	void CreateAppScene(HWND hWnd);


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);

// 実装

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CFbx2BinVS2015App theApp;
