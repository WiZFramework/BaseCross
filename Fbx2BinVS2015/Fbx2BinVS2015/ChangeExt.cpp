// ChangeExt.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Fbx2BinVS2015.h"
#include "ChangeExt.h"
#include "afxdialogex.h"


// CChangeExt ダイアログ

IMPLEMENT_DYNAMIC(CChangeExt, CDialogEx)

CChangeExt::CChangeExt(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CHANGE_EXT, pParent)
	, m_NewExt(_T(""))
{

}

CChangeExt::~CChangeExt()
{
}

void CChangeExt::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NEW_EXT, m_NewExt);
	DDV_MaxChars(pDX, m_NewExt, 5);
}


BEGIN_MESSAGE_MAP(CChangeExt, CDialogEx)
	ON_BN_CLICKED(IDOK, &CChangeExt::OnBnClickedOk)
END_MESSAGE_MAP()


// CChangeExt メッセージ ハンドラー


BOOL CChangeExt::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}


void CChangeExt::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_NewExt == L"") {
		AfxMessageBox(L"拡張子に空白は指定できません。");
		return;
	}

	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}
