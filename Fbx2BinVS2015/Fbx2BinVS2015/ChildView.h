
// ChildView.h : CChildView クラスのインターフェイス
//


#pragma once
#include "DxWindow.h"


// CChildView ウィンドウ

class CChildView : public CWnd
{
	CDxWindow m_DxWindow;
// コンストラクション
public:
	CChildView();

// 属性
public:

// 操作
public:

// オーバーライド
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 実装
public:
	virtual ~CChildView();

	// 生成された、メッセージ割り当て関数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

