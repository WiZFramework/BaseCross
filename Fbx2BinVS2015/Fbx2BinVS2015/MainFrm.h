
// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once
#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
	//FBXメッシュ読み込み時の初期値
	CString m_MeshScale;
	CString m_MeshPosX;
	CString m_MeshPosY;
	CString m_MeshPosZ;
	BOOL m_IsReadStatic;
	BOOL m_WithTangent;
	//タンジェント使用時の法線マップファイル
	wstring m_NormalmapFullFileName;
	//テクスチャをラッピング処理するかどうか
	BOOL m_TextureWrap;


	//メッシュのフルパスディレクトリ（最後に/はつかない）
	wstring m_MeshDir;
	//メッシュのファイル名
	wstring m_MeshFilename;
	//表示・保存スケールの維持
	BOOL m_IsNowScale;
	//以下アニメーション関係
	//フレームレート
	UINT m_FrameRate;
	UINT m_StartSec;
	UINT m_EndSec;
	BOOL m_Looped;
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CChildView    m_wndView;

// 生成された、メッセージ割り当て関数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnOpenFbxFile();
	afx_msg void OnMoveFbxFile();
	afx_msg void OnUpdateMoveFbxFile(CCmdUI *pCmdUI);
	afx_msg void OnSaveBin();
	afx_msg void OnUpdateSaveBin(CCmdUI *pCmdUI);
	afx_msg void OnNewStage();
};


