// BinSaveSkinDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Fbx2BinVS2015.h"
#include "BinSaveSkinDialog.h"
#include "ChangeExt.h"
#include "afxdialogex.h"


// CBinSaveSkinDialog ダイアログ

IMPLEMENT_DYNAMIC(CBinSaveSkinDialog, CDialogEx)

CBinSaveSkinDialog::CBinSaveSkinDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SAVE_BINFILE_SKIN, pParent)
	, m_FullBinFileName(_T(""))
	, m_Scale(_T(""))
	, m_IsNowScale(FALSE)
	, m_FrameRate(_T(""))
	, m_AnimeStartSec(_T(""))
	, m_AnimeEndSec(_T(""))
{

}

CBinSaveSkinDialog::~CBinSaveSkinDialog()
{
}

void CBinSaveSkinDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_SAVE_BINFILENAME_SKIN, m_FullBinFileName);
	DDX_Text(pDX, IDC_STATIC_SCALING_SKIN, m_Scale);
	DDX_Check(pDX, IDC_CHECK_NOW_SCALE_SKIN, m_IsNowScale);
	DDX_Text(pDX, IDC_EDIT_FRAME_RATE_SKIN, m_FrameRate);
	DDV_MaxChars(pDX, m_FrameRate, 2);
	DDX_Text(pDX, IDC_EDIT_ANIME_START_SEC_SKIN, m_AnimeStartSec);
	DDV_MaxChars(pDX, m_AnimeStartSec, 4);
	DDX_Text(pDX, IDC_EDIT_ANIME_END_SEC_SKIN, m_AnimeEndSec);
	DDV_MaxChars(pDX, m_AnimeEndSec, 4);
}


BEGIN_MESSAGE_MAP(CBinSaveSkinDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_EXT_SKIN, &CBinSaveSkinDialog::OnBnClickedButtonUpdateExtSkin)
	ON_BN_CLICKED(IDOK, &CBinSaveSkinDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CBinSaveSkinDialog メッセージ ハンドラー


void CBinSaveSkinDialog::OnBnClickedButtonUpdateExtSkin()
{
	CChangeExt ChangeExt;
	ChangeExt.m_NewExt = m_FileExt;
	ChangeExt.m_NewExt.Replace(L".", L"");
	if (ChangeExt.DoModal() == IDOK) {
		ChangeExt.m_NewExt.Replace(L".", L"");
		m_FileExt = L".";
		m_FileExt += ChangeExt.m_NewExt;

		m_FullBinFileName = m_FileDir;
		m_FullBinFileName += m_FileNameNoExt;
		m_FullBinFileName += m_FileExt;
		UpdateData(FALSE);
	}
}


void CBinSaveSkinDialog::OnBnClickedOk()
{
	UpdateData(TRUE);
	if (_wtoi(m_FrameRate) <= 0) {
		AfxMessageBox(L"フレームレートは0以下は設定できません。");
		return;
	}

	if (_wtoi(m_AnimeStartSec) < 0) {
		AfxMessageBox(L"スタートは0未満は設定できません。");
		return;
	}
	if (_wtoi(m_AnimeStartSec) >= _wtoi(m_AnimeEndSec)) {
		AfxMessageBox(L"スタートより終了は大きくなければなりません。");
		return;
	}

	CDialogEx::OnOK();
}


BOOL CBinSaveSkinDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
