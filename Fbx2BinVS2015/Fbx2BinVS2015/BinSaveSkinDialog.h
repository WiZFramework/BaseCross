#pragma once


// CBinSaveSkinDialog ダイアログ

class CBinSaveSkinDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CBinSaveSkinDialog)

public:
	CBinSaveSkinDialog(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CBinSaveSkinDialog();
	CString m_FileDir;
	CString m_FileNameNoExt;
	CString m_FileExt;

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SAVE_BINFILE_SKIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CString m_FullBinFileName;
	CString m_Scale;
	BOOL m_IsNowScale;
	CString m_FrameRate;
	CString m_AnimeStartSec;
	CString m_AnimeEndSec;
	afx_msg void OnBnClickedButtonUpdateExtSkin();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
