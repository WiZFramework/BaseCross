
// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "StageEditor.h"

#include "MainFrm.h"
#include "Project.h"
using namespace basecross;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
{
	// TODO: メンバー初期化コードをここに追加してください。
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// フレームのクライアント領域全体を占めるビューを作成します。
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("ビュー ウィンドウを作成できませんでした。\n");
		return -1;
	}

	//アプリケーションクラスとデバイスシーンの作成
	try {
		((CStageEditorApp*)AfxGetApp())->CreateAppScene(m_wndView.GetSafeHwnd());
	}
	catch (BaseException& e) {
		AfxMessageBox(e.what_w().c_str());
		return -1;
	}
	catch (BaseMBException& e) {
		MessageBoxA(nullptr, e.what_m().c_str(), "エラー", MB_OK);
		return -1;
	}
	catch (exception& e) {
		MessageBoxA(nullptr, e.what(), "エラー", MB_OK);
		return -1;
	}
	catch (...) {
		//原因不明失敗した
		AfxMessageBox(L"原因不明のエラーです");
		return -1;
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。
	CRect rect = { 0, 0,1280, 800 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);
	cs.cx = rect.Width();
	cs.cy = rect.Height();

	//cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
	//	 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU;

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		|  WS_MINIMIZEBOX | WS_SYSMENU;


	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame メッセージ ハンドラー

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// ビュー ウィンドウにフォーカスを与えます。
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// ビューに最初にコマンドを処理する機会を与えます。
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// それ以外の場合は、既定の処理を行います。
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

