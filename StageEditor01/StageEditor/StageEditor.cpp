
// StageEditor.cpp : アプリケーションのクラス動作を定義します。
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


// CStageEditorApp コンストラクション

CStageEditorApp::CStageEditorApp()
{
	// TODO: 下のアプリケーション ID 文字列を一意の ID 文字列で置換します。推奨される
	// 文字列の形式は CompanyName.ProductName.SubProduct.VersionInformation です
	SetAppID(_T("StageEditor.AppID.NoVersion"));

	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}

// 唯一の CStageEditorApp オブジェクトです。

CStageEditorApp theApp;


void CStageEditorApp::CreateAppScene(HWND hWnd) {
	//アプリケーションクラスとデバイスの構築
	////アプリケーションクラスの構築
	try {
		m_DxHWnd = hWnd;
		App::CreateApp(AfxGetInstanceHandle(), hWnd, false, 1280, 800);
		App::GetApp()->CreateScene<Scene>();
	}
	catch (...) {
		throw;
	}
}



// CStageEditorApp 初期化

BOOL CStageEditorApp::InitInstance()
{
	// アプリケーション マニフェストが visual スタイルを有効にするために、
	// ComCtl32.dll Version 6 以降の使用を指定する場合は、
	// Windows XP に InitCommonControlsEx() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// アプリケーションで使用するすべてのコモン コントロール クラスを含めるには、
	// これを設定します。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// OLE ライブラリを初期化します。
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit コントロールを使用するには AfxInitRichEdit2() が必要です	
	// AfxInitRichEdit2();

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("アプリケーション ウィザードで生成されたローカル アプリケーション"));


	// メイン ウィンドウを作成するとき、このコードは新しいフレーム ウィンドウ オブジェクトを作成し、
	// それをアプリケーションのメイン ウィンドウにセットします
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;

	// フレームをリソースからロードして作成します
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	return TRUE;
}

int CStageEditorApp::ExitInstance()
{
	//TODO: 追加したリソースがある場合にはそれらも処理してください
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CStageEditorApp メッセージ ハンドラー


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
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

// ダイアログを実行するためのアプリケーション コマンド
void CStageEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CStageEditorApp メッセージ ハンドラー


BOOL CStageEditorApp::OnIdle(LONG lCount)
{
	try {
		// attempt any additional idle loop processing.
		if (CWinApp::OnIdle(lCount))
			return TRUE;
		//ここに設定したキーボード入力を得る
		vector<DWORD> UseKeyVec = {};
		App::GetApp()->ResetInputState(m_DxHWnd, UseKeyVec);
		//更新描画処理
		App::GetApp()->UpdateDraw(1);
	}
	catch (BaseException& e) {
		AfxMessageBox(e.what_w().c_str());
	}
	catch (BaseMBException& e) {
		MessageBoxA(nullptr, e.what_m().c_str(), "エラー", MB_OK);
	}
	catch (exception& e) {
		MessageBoxA(nullptr, e.what(), "エラー", MB_OK);
	}
	catch (...) {
		//原因不明失敗した
		AfxMessageBox(L"原因不明のエラーです");
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
