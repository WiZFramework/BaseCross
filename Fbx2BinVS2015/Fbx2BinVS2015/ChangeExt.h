#pragma once


// CChangeExt ダイアログ

class CChangeExt : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeExt)

public:
	CChangeExt(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CChangeExt();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHANGE_EXT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	CString m_NewExt;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
