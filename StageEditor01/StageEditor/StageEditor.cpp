
// StageEditor.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "StageEditor.h"
#include "MainFrm.h"
#include "ObjPropDlg.h"


#include "Project.h"
using namespace basecross;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStageEditorApp

BEGIN_MESSAGE_MAP(CStageEditorApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CStageEditorApp::OnAppAbout)
	ON_COMMAND(ID_32771, &CStageEditorApp::OnObjProp)
END_MESSAGE_MAP()


// CStageEditorApp �R���X�g���N�V����

CStageEditorApp::CStageEditorApp()
{
	// TODO: ���̃A�v���P�[�V���� ID ���������ӂ� ID ������Œu�����܂��B���������
	// ������̌`���� CompanyName.ProductName.SubProduct.VersionInformation �ł�
	SetAppID(_T("StageEditor.AppID.NoVersion"));

	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

// �B��� CStageEditorApp �I�u�W�F�N�g�ł��B

CStageEditorApp theApp;


void CStageEditorApp::CreateAppScene(HWND hWnd) {
	//�A�v���P�[�V�����N���X�ƃf�o�C�X�̍\�z
	////�A�v���P�[�V�����N���X�̍\�z
	try {
		m_DxHWnd = hWnd;
		App::CreateApp(AfxGetInstanceHandle(), hWnd, false, 1280, 800);
		App::GetApp()->CreateScene<Scene>();
	}
	catch (...) {
		throw;
	}
}



// CStageEditorApp ������

BOOL CStageEditorApp::InitInstance()
{
	// �A�v���P�[�V���� �}�j�t�F�X�g�� visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll Version 6 �ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �� InitCommonControlsEx() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �A�v���P�[�V�����Ŏg�p���邷�ׂẴR���� �R���g���[�� �N���X���܂߂�ɂ́A
	// �����ݒ肵�܂��B
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// OLE ���C�u���������������܂��B
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit �R���g���[�����g�p����ɂ� AfxInitRichEdit2() ���K�v�ł�	
	// AfxInitRichEdit2();

	// �W��������
	// �����̋@�\���g�킸�ɍŏI�I�Ȏ��s�\�t�@�C����
	// �T�C�Y���k���������ꍇ�́A�ȉ�����s�v�ȏ�����
	// ���[�`�����폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ��Ж��܂��͑g�D���Ȃǂ̓K�؂ȕ������
	// ���̕������ύX���Ă��������B
	SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));


	// ���C�� �E�B���h�E���쐬����Ƃ��A���̃R�[�h�͐V�����t���[�� �E�B���h�E �I�u�W�F�N�g���쐬���A
	// ������A�v���P�[�V�����̃��C�� �E�B���h�E�ɃZ�b�g���܂�
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;

	// �t���[�������\�[�X���烍�[�h���č쐬���܂�
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CStageEditorApp::ExitInstance()
{
	//TODO: �ǉ��������\�[�X������ꍇ�ɂ͂������������Ă�������
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CStageEditorApp ���b�Z�[�W �n���h���[


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �_�C�A���O�����s���邽�߂̃A�v���P�[�V���� �R�}���h
void CStageEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CStageEditorApp ���b�Z�[�W �n���h���[


BOOL CStageEditorApp::OnIdle(LONG lCount)
{
	try {
		// attempt any additional idle loop processing.
		if (CWinApp::OnIdle(lCount))
			return TRUE;
		//�����ɐݒ肵���L�[�{�[�h���͂𓾂�
		vector<DWORD> UseKeyVec = {};
		App::GetApp()->ResetInputState(m_DxHWnd, UseKeyVec);
		//�X�V�`�揈��
		App::GetApp()->UpdateDraw(1);
	}
	catch (BaseException& e) {
		AfxMessageBox(e.what_w().c_str());
	}
	catch (BaseMBException& e) {
		MessageBoxA(nullptr, e.what_m().c_str(), "�G���[", MB_OK);
	}
	catch (exception& e) {
		MessageBoxA(nullptr, e.what(), "�G���[", MB_OK);
	}
	catch (...) {
		//�����s�����s����
		AfxMessageBox(L"�����s���̃G���[�ł�");
	}
	return TRUE;

//	return CWinApp::OnIdle(lCount);
}




void CStageEditorApp::OnObjProp()
{
	CObjPropDlg objpropDlg;
	objpropDlg.SetNew(true);
	objpropDlg.DoModal();
}