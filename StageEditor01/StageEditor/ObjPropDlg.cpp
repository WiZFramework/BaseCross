// ObjPropDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "StageEditor.h"
#include "ObjPropDlg.h"
#include "afxdialogex.h"


// CObjPropDlg ダイアログ

IMPLEMENT_DYNAMIC(CObjPropDlg, CDialogEx)

CObjPropDlg::CObjPropDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_OBJ_PROP, pParent),
	m_IsNew(false)
{

}

CObjPropDlg::~CObjPropDlg()
{
}

void CObjPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CObjPropDlg, CDialogEx)
END_MESSAGE_MAP()


// CObjPropDlg メッセージ ハンドラー


BOOL CObjPropDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: ここに初期化を追加してください
	if (m_IsNew) {
		SetWindowText(L"新規");
	}
	else {
		SetWindowText(L"プロパティ");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
