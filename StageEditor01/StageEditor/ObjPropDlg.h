#pragma once


// CObjPropDlg ダイアログ

class CObjPropDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CObjPropDlg)
	bool m_IsNew;
public:
	CObjPropDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~CObjPropDlg();
	void SetNew(bool b) {
		m_IsNew = b;
	}
// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_OBJ_PROP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
