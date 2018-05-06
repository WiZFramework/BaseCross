/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------

	//ビューとライトの作成
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrLookAtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtrLookAtCamera);
		PtrLookAtCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
		PtrLookAtCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}


	//タイリングプレートの作成
	void GameStage::CreatePlate() {
		//CSVの行単位の配列
		vector<wstring> LineVec;
		//0番目のカラムがL"TilingPlate"である行を抜き出す
		m_GameStageCsvB.GetSelect(LineVec, 0, L"TilingPlate");
		//1行も抽出できなければ作成しない
		if (!LineVec.empty()) {
			//トークン（カラム）の配列
			vector<wstring> Tokens;
			//0行目をトークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
			Util::WStrToTokenVector(Tokens, LineVec[0], L',');
			//各トークン（カラム）をスケール、回転、位置に読み込む
			Vec3 Scale(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);
			Vec3 Rot;
			//回転は「XM_PIDIV2」の文字列になっている場合がある
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());
			Vec3 Pos(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			//プレートの回転の引数はクオータニオンになっているので変換
			Quat Qt;
			Qt.rotationRollPitchYawFromVector(Rot);
			//ステージへのゲームオブジェクトの追加
			AddGameObject<TilingPlate>(Scale, Qt, Pos, 1.0f, 1.0f);
		}
	}

	//ボックスの作成
	void GameStage::CreateFixedBox() {
		//CSVの行単位の配列
		vector<wstring> LineVec;
		//0番目のカラムがL"TilingFixedBox"である行を抜き出す
		m_GameStageCsvB.GetSelect(LineVec, 0, L"TilingFixedBox");
		for (auto& v : LineVec) {
			//トークン（カラム）の配列
			vector<wstring> Tokens;
			//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
			Util::WStrToTokenVector(Tokens, v, L',');
			//各トークン（カラム）をスケール、回転、位置に読み込む
			Vec3 Scale(
				(float)_wtof(Tokens[1].c_str()), 
				(float)_wtof(Tokens[2].c_str()), 
				(float)_wtof(Tokens[3].c_str())
			);
			Vec3 Rot;
			//回転は「XM_PIDIV2」の文字列になっている場合がある
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());
			Vec3 Pos(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			//各値がそろったのでオブジェクト作成
			AddGameObject<TilingFixedBox>(Scale, Rot, Pos, 1.0f, 1.0f);
		}
	}



	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//敵の作成
	void GameStage::CreateEnemy() {
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"EnemyGroup");
		//CSVの全体の配列
		//CSVからすべての行を抜き出す
		vector<wstring>& LineVec = m_GameStageCsv.GetCsvVec();
		for (size_t i = 0; i < LineVec.size();i++) {
			//トークン（カラム）の配列
			vector<wstring> Tokens;
			//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
			Util::WStrToTokenVector(Tokens, LineVec[i], L',');
			for (size_t j = 0; j < Tokens.size(); j++) {
				//XとZの位置を計算
				float XPos = (float)((int)j - 19);
				float ZPos = (float)(19 - (int)i);
				if (Tokens[j] == L"1") {
					AddGameObject<Enemy1>(Vec3(XPos,0.25f,ZPos));
				}
				else if (Tokens[j] == L"2") {
					AddGameObject<Enemy2>(Vec3(XPos, 0.25f, ZPos));

				}
				else if (Tokens[j] == L"3") {
					AddGameObject<Enemy3>(Vec3(XPos, 0.25f, ZPos));

				}
			}
		}
	}


	void GameStage::OnCreate() {
		try {
			//シーンの取得
			auto PtrScene = App::GetApp()->GetScene<Scene>();
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			switch (PtrScene->GetStageNum()) {
			case 1:
				//CSVファイルの確定
				m_GameStageCsv.SetFileName(DataDir + L"GameStage1.csv");
				break;
			case 2:
				//CSVファイルの確定
				m_GameStageCsv.SetFileName(DataDir + L"GameStage2.csv");
				break;
			case 3:
				//CSVファイルの確定
				m_GameStageCsv.SetFileName(DataDir + L"GameStage3.csv");
				break;
			default:
				//それ以外はステージ1
				//CSVファイルの確定
				m_GameStageCsv.SetFileName(DataDir + L"GameStage1.csv");
				break;
			}
			m_GameStageCsv.ReadCsv();
			////CSVファイルそのBの読み込み
			m_GameStageCsvB.SetFileName(DataDir + L"GameStageB.csv");
			m_GameStageCsvB.ReadCsv();
			//ビューとライトの作成
			CreateViewLight();
			//プレートの作成
			CreatePlate();
			//ボックスの作成
			CreateFixedBox();
			//プレーヤーの作成
			CreatePlayer();
			//敵の作成
			CreateEnemy();
		}
		catch (...) {
			throw;
		}
	}

	//更新
	void GameStage::OnUpdate() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Bボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				//シーンの取得
				auto PtrScene = App::GetApp()->GetScene<Scene>();
				PostEvent(0.0f, GetThis<GameStage>(), PtrScene, L"ToSelectStage");
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	セレクトステージクラス
	//--------------------------------------------------------------------------------------
	void SelectStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(Vec3(0.0f, 2.0f, -3.0f));
		PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();

	}

	//スプライトの作成
	void SelectStage::CreateAnimeSprite() {
		auto PtrSp = AddGameObject<AnimeSprite>(L"STAGE1_TX", true,
			Vec2(256.0f, 64.0f), Vec2(0.0f, 100.0f));
		PtrSp->SetSelect(true);
		m_SpVec[0] = PtrSp;
		PtrSp = AddGameObject<AnimeSprite>(L"STAGE2_TX", true,
			Vec2(256.0f, 64.0f), Vec2(0.0f, 0.0f));
		m_SpVec[1] = PtrSp;
		PtrSp = AddGameObject<AnimeSprite>(L"STAGE3_TX", true,
			Vec2(256.0f, 64.0f), Vec2(0.0f, -100.0f));
		m_SpVec[2] = PtrSp;
	}

	//初期化
	void SelectStage::OnCreate() {
		CreateViewLight();
		//スプライトの作成
		CreateAnimeSprite();
	}

	void SelectStage::ChangeSelect(int num) {
		for (int i = 0; i < 3; i++) {
			shared_ptr<AnimeSprite> shptr = m_SpVec[i].lock();
			if (shptr) {
				if ((i + 1) == num) {
					shptr->SetSelect(true);
				}
				else {
					shptr->SetSelect(false);
				}
			}
		}
	}


	//更新
	void SelectStage::OnUpdate() {
		//シーンの取得
		auto PtrScene = App::GetApp()->GetScene<Scene>();
		int StageNum = PtrScene->GetStageNum();
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//ゲームステージへ
			//Aボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
				PostEvent(0.0f, GetThis<SelectStage>(), PtrScene, L"ToGameStage");
				return;
			}
			if (!m_CntrolLock) {
				if (CntlVec[0].fThumbLY >= 0.8f) {
					StageNum--;
					if (StageNum < 1) {
						StageNum = 3;
					}
					m_CntrolLock = true;
					PtrScene->SetStageNum(StageNum);
					ChangeSelect(StageNum);
				}
				else if (CntlVec[0].fThumbLY <= -0.8f) {
					StageNum++;
					if (StageNum > 3) {
						StageNum = 1;
					}
					m_CntrolLock = true;
					PtrScene->SetStageNum(StageNum);
					ChangeSelect(StageNum);
				}
			}
			else {
				if (CntlVec[0].fThumbLY == 0.0f) {
					m_CntrolLock = false;
				}
			}
		}
	}


}
//end basecross
