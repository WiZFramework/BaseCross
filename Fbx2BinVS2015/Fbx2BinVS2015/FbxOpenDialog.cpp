// FbxOpenDialog.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Fbx2BinVS2015.h"
#include "FbxOpenDialog.h"
#include "afxdialogex.h"


// CFbxOpenDialog ダイアログ

IMPLEMENT_DYNAMIC(CFbxOpenDialog, CDialogEx)

CFbxOpenDialog::CFbxOpenDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_FILE_SELECT, pParent)
	, m_FullFileName(_T(""))
	, m_Scale(_T(""))
	, m_PosX(_T(""))
	, m_PosY(_T(""))
	, m_PosZ(_T(""))
	, m_IsReadStatic(FALSE)
{

}

CFbxOpenDialog::~CFbxOpenDialog()
{
}

void CFbxOpenDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_OPEN_FILENAME, m_FullFileName);
	DDX_Text(pDX, IDC_EDIT_SCALING, m_Scale);
	DDV_MaxChars(pDX, m_Scale, 5);
	DDX_Text(pDX, IDC_EDIT_POS_X, m_PosX);
	DDV_MaxChars(pDX, m_PosX, 5);
	DDX_Text(pDX, IDC_EDIT_POS_Y, m_PosY);
	DDV_MaxChars(pDX, m_PosY, 5);
	DDX_Text(pDX, IDC_EDIT_POS_Z, m_PosZ);
	DDV_MaxChars(pDX, m_PosZ, 5);
	DDX_Check(pDX, IDC_CHECK_READ_STATICMESH, m_IsReadStatic);
}


BEGIN_MESSAGE_MAP(CFbxOpenDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_FILEDIALOG_OPEN, &CFbxOpenDialog::OnBnClickedButtonFiledialogOpen)
	ON_BN_CLICKED(IDOK, &CFbxOpenDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CFbxOpenDialog メッセージ ハンドラー


void CFbxOpenDialog::OnBnClickedButtonFiledialogOpen()
{
	UpdateData(TRUE);

	CString         filter("FBX Files (*.fbx;*.FBX)|*.fbx; *.FBX||");
	CFileDialog     OpenDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, filter);

	if (OpenDlg.DoModal() == IDOK)
	{
		m_FullFileName = OpenDlg.GetPathName();
		m_FullFileDir = OpenDlg.GetFolderPath();
		m_FileName = OpenDlg.GetFileName();
		UpdateData(FALSE);
	}
}


void CFbxOpenDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (m_FullFileDir == L"" || m_FileName == L"") {
		AfxMessageBox(L"ファイルが選択されていません。");
		return;
	}
	float Scale = (float)_wtof(m_Scale);
	if (Scale <= 0.0f) {
		AfxMessageBox(L"スケールは0.0以下は設定できません。");
		return;

	}
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CDialogEx::OnOK();
}


BOOL CFbxOpenDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
