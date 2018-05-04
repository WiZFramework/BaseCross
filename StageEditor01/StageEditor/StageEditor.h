
// StageEditor.h : StageEditor アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"       // メイン シンボル


// CStageEditorApp:
// このクラスの実装については、StageEditor.cpp を参照してください。
//

class CStageEditorApp : public CWinApp
{
	HWND m_DxHWnd;
public:
	CStageEditorApp();
	void CreateAppScene(HWND hWnd);


// オーバーライド
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 実装

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
	afx_msg void OnObjProp();
};

extern CStageEditorApp theApp;
