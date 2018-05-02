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

	//スパークの作成
	void GameStage::CreateSpark() {
		auto MultiSparkPtr = AddGameObject<MultiSpark>();
		//共有オブジェクトにスパークを登録
		SetSharedGameObject(L"MultiSpark", MultiSparkPtr);
	}

	//敵の作成
	void GameStage::CreateEnemy() {
		AddGameObject<EnemyBox>(Vec3(1.0), Vec3(0.0f), Vec3(2.5f,0.5f,5.0f));
	}

	//キャラクターの作成
	void GameStage::CreateChara() {
		auto Ptr = AddGameObject<BoneChara>(Vec3(-2.5f, 0.0f, 5.0f));
	}



	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		auto SwordPtr = AddGameObject<Sword>(
			Vec3(0.2, 0.8, 0.2),
			L"PlayerSword"
			);
		SetSharedGameObject(L"PlayerSword", SwordPtr);
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);

	}



	void GameStage::OnCreate() {
		try {
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			////CSVファイルそのBの読み込み
			m_GameStageCsvB.SetFileName(DataDir + L"GameStageB.csv");
			m_GameStageCsvB.ReadCsv();

			//ビューとライトの作成
			CreateViewLight();
			//プレートの作成
			CreatePlate();
			//ボックスの作成
			CreateFixedBox();
			//スパークの作成
			CreateSpark();
			//敵の作成
			CreateEnemy();
			//キャラクターの作成
			CreateChara();
			//プレーヤーの作成
			CreatePlayer();

		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
