#pragma once


// CFbxOpenDialog ダイアログ

class CFbxOpenDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CFbxOpenDialog)

public:
	CFbxOpenDialog(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CFbxOpenDialog();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_FILE_SELECT };
#endif
	CString m_FullFileDir;
	CString m_FileName;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonFiledialogOpen();
	CString m_FullFileName;
	afx_msg void OnBnClickedOk();
	CString m_Scale;
	virtual BOOL OnInitDialog();
	CString m_PosX;
	CString m_PosY;
	CString m_PosZ;
	BOOL m_IsReadStatic;
	BOOL m_WithTangent;
	CString m_NormalmapFullFileName;
	afx_msg void OnBnClickedButtonFiledialogNormalmapOpen();
	BOOL m_TextureWrap;
};
