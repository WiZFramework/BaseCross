
// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "Fbx2BinVS2015.h"
#include "FbxOpenDialog.h"
#include "AnimeMoveDialog.h"
#include "BinSaveDialog.h"
#include "BinSaveSkinDialog.h"
#include "afxdialogex.h"


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
	ON_COMMAND(ID_32771, &CMainFrame::OnOpenFbxFile)
	ON_COMMAND(ID_32772, &CMainFrame::OnMoveFbxFile)
	ON_UPDATE_COMMAND_UI(ID_32772, &CMainFrame::OnUpdateMoveFbxFile)
	ON_COMMAND(ID_32773, &CMainFrame::OnSaveBin)
	ON_UPDATE_COMMAND_UI(ID_32773, &CMainFrame::OnUpdateSaveBin)
	ON_COMMAND(ID_32774, &CMainFrame::OnNewStage)
END_MESSAGE_MAP()

// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
{
	//FBXメッシュ読み込み時の初期値
	m_MeshScale = L"1.0";
	m_MeshPosX = L"0.0";
	m_MeshPosY = L"0.0";
	m_MeshPosZ = L"0.0";
	//メッシュのフルパスディレクトリ（最後に/はつかない）
	m_MeshDir = L"";
	//メッシュのファイル名
	m_MeshFilename = L"";
	//スタティックとして読み込む
	m_IsReadStatic = FALSE;

	m_IsNowScale = TRUE;

	//以下アニメーション関係
	//フレームレート
	m_FrameRate = 10;
	m_StartSec = 0;
	m_EndSec = 1;
	m_Looped = TRUE;


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
	try{
		((CFbx2BinVS2015App*)AfxGetApp())->CreateAppScene(m_wndView.GetSafeHwnd());
	}
	catch (...) {
		TRACE0("シーンの作成に失敗しました。\n");
		return -1;
	}
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

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



void CMainFrame::OnOpenFbxFile()
{
	CFbxOpenDialog FbxOpenDialog;
	FbxOpenDialog.m_FullFileDir = m_MeshDir.c_str();
	FbxOpenDialog.m_FileName = m_MeshFilename.c_str();
	if (m_MeshDir != L"" && m_MeshFilename != L"") {
		FbxOpenDialog.m_FullFileName = FbxOpenDialog.m_FullFileDir;
		FbxOpenDialog.m_FullFileName += L"\\";
		FbxOpenDialog.m_FullFileName += FbxOpenDialog.m_FileName;
	}
	else {
		FbxOpenDialog.m_FullFileName = L"";
	}
	FbxOpenDialog.m_Scale = m_MeshScale;
	FbxOpenDialog.m_PosX = m_MeshPosX;
	FbxOpenDialog.m_PosY = m_MeshPosY;
	FbxOpenDialog.m_PosZ = m_MeshPosZ;
	FbxOpenDialog.m_IsReadStatic = m_IsReadStatic;

	if(FbxOpenDialog.DoModal() == IDOK){
		wchar_t RelativePath[MAX_PATH];
		m_MeshDir = FbxOpenDialog.m_FullFileDir;
		m_MeshFilename = FbxOpenDialog.m_FileName;
		m_MeshScale = FbxOpenDialog.m_Scale;
		m_MeshPosX = FbxOpenDialog.m_PosX;
		m_MeshPosY = FbxOpenDialog.m_PosY;
		m_MeshPosZ = FbxOpenDialog.m_PosZ;
		m_IsReadStatic = FbxOpenDialog.m_IsReadStatic;

		wstring ModDIr;
		App::GetApp()->GetModuleDirectory(ModDIr);

		if (PathRelativePathTo(
			RelativePath,
			ModDIr.c_str(),
			FILE_ATTRIBUTE_DIRECTORY,
			m_MeshDir.c_str(),
			FILE_ATTRIBUTE_DIRECTORY
		)) {
			//ファイルのオープン
			wstring Dir = RelativePath;
			Dir += L"\\";
			float Scale = (float)_wtof(m_MeshScale);
			Vector3 Pos(
				(float)_wtof(m_MeshPosX),
				(float)_wtof(m_MeshPosY),
				(float)_wtof(m_MeshPosZ)
				);
			try{
				App::GetApp()->GetScene<Scene>()->ReadFbxFile(Dir, m_MeshFilename, 0, Scale, Pos, m_IsReadStatic == TRUE);
			}
			catch (...) {
				AfxMessageBox(L"FBXファイルの読み込みに失敗しました。");
			}
		}
	}

}


void CMainFrame::OnMoveFbxFile()
{
	if (!App::GetApp()->GetScene<Scene>()->CheckSkinMesh()) {
		AfxMessageBox(L"メッシュが読まれてないか、スキンメッシュではありません。");
	}
	else {
		CAnimeMoveDialog AnimeMoveDialog;
		AnimeMoveDialog.m_FrameRate.Format(L"%d", m_FrameRate);
		AnimeMoveDialog.m_AnimeStartSec.Format(L"%d", m_StartSec);
		AnimeMoveDialog.m_AnimeEndSec.Format(L"%d", m_EndSec);
		AnimeMoveDialog.m_IsLooped = m_Looped;
		if (AnimeMoveDialog.DoModal() == IDOK) {
			m_FrameRate = (UINT)_wtoi(AnimeMoveDialog.m_FrameRate);
			m_StartSec = (UINT)_wtoi(AnimeMoveDialog.m_AnimeStartSec);
			m_EndSec = (UINT)_wtoi(AnimeMoveDialog.m_AnimeEndSec);
			m_Looped = (AnimeMoveDialog.m_IsLooped == TRUE);
			App::GetApp()->GetScene<Scene>()->MoveFbxFile(m_FrameRate, m_StartSec, m_EndSec, m_Looped == TRUE);
		}
	}
}


void CMainFrame::OnUpdateMoveFbxFile(CCmdUI *pCmdUI)
{
	if (!App::GetApp()->GetScene<Scene>()->CheckSkinMesh()) {
		pCmdUI->Enable(FALSE);
	}
	else {
		pCmdUI->Enable(TRUE);
	}
}

//保存
void CMainFrame::OnSaveBin()
{
	if (App::GetApp()->GetScene<Scene>()->CheckMesh()) {
		wchar_t Drivebuff[_MAX_DRIVE];
		wchar_t Dirbuff[_MAX_DIR];
		wchar_t FileNamebuff[_MAX_FNAME];
		wchar_t Extbuff[_MAX_EXT];

		::ZeroMemory(Drivebuff, sizeof(Drivebuff));
		::ZeroMemory(Dirbuff, sizeof(Dirbuff));
		::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
		::ZeroMemory(Extbuff, sizeof(Extbuff));

		wstring FullPath = m_MeshDir;
		FullPath += L"\\";
		FullPath += m_MeshFilename;

		//フルパスから、各ブロックに分ける
		_wsplitpath_s(FullPath.c_str(),
			Drivebuff, _MAX_DRIVE,
			Dirbuff, _MAX_DIR,
			FileNamebuff, _MAX_FNAME,
			Extbuff, _MAX_EXT);

		if (App::GetApp()->GetScene<Scene>()->CheckSkinMesh()) {
			CBinSaveSkinDialog BinSaveSkinDialog;
			BinSaveSkinDialog.m_FileDir = Drivebuff;
			BinSaveSkinDialog.m_FileDir += Dirbuff;
			BinSaveSkinDialog.m_FileNameNoExt = FileNamebuff;
			BinSaveSkinDialog.m_FileExt = L".bmf";

			BinSaveSkinDialog.m_FullBinFileName = BinSaveSkinDialog.m_FileDir;
			BinSaveSkinDialog.m_FullBinFileName += BinSaveSkinDialog.m_FileNameNoExt;
			BinSaveSkinDialog.m_FullBinFileName += BinSaveSkinDialog.m_FileExt;

			BinSaveSkinDialog.m_FrameRate.Format(L"%d", m_FrameRate);
			BinSaveSkinDialog.m_AnimeStartSec.Format(L"%d", m_StartSec);
			BinSaveSkinDialog.m_AnimeEndSec.Format(L"%d", m_EndSec);


			BinSaveSkinDialog.m_Scale = m_MeshScale;
			BinSaveSkinDialog.m_IsNowScale = m_IsNowScale;

			if (BinSaveSkinDialog.DoModal() == IDOK) {
				wstring SaveFileDir = BinSaveSkinDialog.m_FileDir;
				wstring SaveFileName = BinSaveSkinDialog.m_FileNameNoExt;
				SaveFileName += BinSaveSkinDialog.m_FileExt;
				m_FrameRate = (UINT)_wtoi(BinSaveSkinDialog.m_FrameRate);
				m_StartSec = (UINT)_wtoi(BinSaveSkinDialog.m_AnimeStartSec);
				m_EndSec = (UINT)_wtoi(BinSaveSkinDialog.m_AnimeEndSec);
				float Scale = (float)_wtof(m_MeshScale);
				if (!BinSaveSkinDialog.m_IsNowScale) {
					Scale = 1.0f;
				}
				try {
					App::GetApp()->GetScene<Scene>()->SaveSkinBinFile(SaveFileDir, SaveFileName, 0, Scale,
						m_FrameRate, m_StartSec, m_EndSec);
					AfxMessageBox(L"スキンバイナリファイルを保存しました。");
				}
				catch (...) {
					AfxMessageBox(L"スキンバイナリファイルの保存に失敗しました。");
				}
			}


		}
		else {



			CBinSaveDialog BinSaveDialog;
			BinSaveDialog.m_FileDir = Drivebuff;
			BinSaveDialog.m_FileDir += Dirbuff;
			BinSaveDialog.m_FileNameNoExt = FileNamebuff;
			BinSaveDialog.m_FileExt = L".bmf";

			BinSaveDialog.m_FullBinFileName = BinSaveDialog.m_FileDir;
			BinSaveDialog.m_FullBinFileName += BinSaveDialog.m_FileNameNoExt;
			BinSaveDialog.m_FullBinFileName += BinSaveDialog.m_FileExt;

			BinSaveDialog.m_Scale = m_MeshScale;
			BinSaveDialog.m_IsNowScale = m_IsNowScale;

			if (BinSaveDialog.DoModal() == IDOK) {
				wstring SaveFileDir = BinSaveDialog.m_FileDir;
				wstring SaveFileName = BinSaveDialog.m_FileNameNoExt;
				SaveFileName += BinSaveDialog.m_FileExt;
				float Scale = (float)_wtof(m_MeshScale);
				if (!BinSaveDialog.m_IsNowScale) {
					Scale = 1.0f;
				}
				try {
					App::GetApp()->GetScene<Scene>()->SaveStaticBinFile(SaveFileDir, SaveFileName, 0, Scale);
					AfxMessageBox(L"スタティックバイナリファイルを保存しました。");
				}
				catch (...) {
					AfxMessageBox(L"スタティックバイナリファイルの保存に失敗しました。");
				}
			}
		}
	}
	else {
		AfxMessageBox(L"メッシュが読まれてせん。");
	}


}


void CMainFrame::OnUpdateSaveBin(CCmdUI *pCmdUI)
{
	if (!App::GetApp()->GetScene<Scene>()->CheckMesh()) {
		pCmdUI->Enable(FALSE);
	}
	else {
		pCmdUI->Enable(TRUE);
	}
}


void CMainFrame::OnNewStage()
{
	if (App::GetApp()->GetScene<Scene>()->CheckMesh()) {
		if (AfxMessageBox(L"現在のステージは削除してよろしいですか？", MB_OKCANCEL) != IDOK) {
			return;
		}
	}
	App::GetApp()->GetScene<Scene>()->ResetNewStage();
}
