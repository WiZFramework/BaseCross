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
		PtrLookAtCamera->SetEye(Vector3(0.0f, 5.0f, -5.0f));
		PtrLookAtCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
		//ライティング修正
	}


	//タイリングプレートの作成
	void GameStage::CreatePlate() {
		//CSVの行単位の配列
		vector<wstring> LineVec;
		//0番目のカラムがL"TilingPlate"である行を抜き出す
		m_GameStageCsv.GetSelect(LineVec, 0, L"TilingPlate");
		//1行も抽出できなければ作成しない
		if (!LineVec.empty()) {
			//トークン（カラム）の配列
			vector<wstring> Tokens;
			//0行目をトークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
			Util::WStrToTokenVector(Tokens, LineVec[0], L',');
			//各トークン（カラム）をスケール、回転、位置に読み込む
			Vector3 Scale(
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str()),
				(float)_wtof(Tokens[3].c_str())
			);
			Vector3 Rot;
			//回転は「XM_PIDIV2」の文字列になっている場合がある
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());
			Vector3 Pos(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			//プレートの回転の引数はクオータニオンになっているので変換
			Quaternion Qt;
			Qt.RotationRollPitchYawFromVector(Rot);
			//ステージへのゲームオブジェクトの追加
			AddGameObject<TilingPlate>(Scale, Qt, Pos, 1.0f, 1.0f);
		}
	}

	//セルマップの作成
	void GameStage::CreateStageCellMap() {
		float  PieceSize = 1.0f;
		auto Ptr = AddGameObject<StageCellMap>(Vector3(-5.0f, 0, -5.0f), PieceSize, 10, 10);
		//セルマップの区画を表示する場合は以下の設定
		//Ptr->SetDrawActive(true);
		//さらにセルのインデックスとコストを表示する場合は以下の設定
		//Ptr->SetCellStringActive(true);
		SetSharedGameObject(L"StageCellMap", Ptr);
	}


	//ボックスの作成
	void GameStage::CreateFixedBox() {
		//ボックスのグループを作成
		auto BoxGroup = CreateSharedObjectGroup(L"FixedBoxes");
		//CSVの行単位の配列
		vector<wstring> LineVec;
		//0番目のカラムがL"TilingFixedBox"である行を抜き出す
		m_GameStageCsv.GetSelect(LineVec, 0, L"TilingFixedBox");
		for (auto& v : LineVec) {
			//トークン（カラム）の配列
			vector<wstring> Tokens;
			//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
			Util::WStrToTokenVector(Tokens, v, L',');
			//各トークン（カラム）をスケール、回転、位置に読み込む
			Vector3 Scale(
				(float)_wtof(Tokens[1].c_str()), 
				(float)_wtof(Tokens[2].c_str()), 
				(float)_wtof(Tokens[3].c_str())
			);
			Vector3 Rot;
			//回転は「XM_PIDIV2」の文字列になっている場合がある
			Rot.x = (Tokens[4] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[4].c_str());
			Rot.y = (Tokens[5] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[5].c_str());
			Rot.z = (Tokens[6] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[6].c_str());
			Vector3 Pos(
				(float)_wtof(Tokens[7].c_str()),
				(float)_wtof(Tokens[8].c_str()),
				(float)_wtof(Tokens[9].c_str())
			);
			//各値がそろったのでオブジェクト作成
			auto BoxPtr = AddGameObject<TilingFixedBox>(Scale, Rot, Pos, 1.0f, 1.0f);
			//ボックスのグループに追加
			BoxGroup->IntoGroup(BoxPtr);
		}
		//最初の2つのセルマップへのボックスのコスト設定
		SetCellMapCost();
	}

	//固定のボックスのコストをセルマップに反映
	void GameStage::SetCellMapCost() {
		//セルマップ内にFixedBoxの情報をセット
		auto PtrCellmap = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		auto BoxGroup = GetSharedObjectGroup(L"FixedBoxes");
		//セルマップからセルの配列を取得
		auto& CellVec = PtrCellmap->GetCellVec();
		//ボックスグループからボックスの配列を取得
		auto& BoxVec = BoxGroup->GetGroupVector();
		vector<AABB> ObjectsAABBVec;
		for (auto& v : BoxVec) {
			auto FixedBoxPtr = dynamic_pointer_cast<TilingFixedBox>(v.lock());
			if (FixedBoxPtr) {
				auto ColPtr = FixedBoxPtr->GetComponent<CollisionObb>();
				//ボックスの衝突判定かラッピングするAABBを取得して保存
				ObjectsAABBVec.push_back(ColPtr->GetWrappingAABB());
			}
		}
		//セル配列からセルをスキャン
		for (auto& v : CellVec) {
			for (auto& v2 : v) {
				for (auto& vObj : ObjectsAABBVec) {
					if (HitTest::AABB_AABB_NOT_EQUAL(v2.m_PieceRange, vObj)) {
						//ボックスのABBとNOT_EQUALで衝突判定
						v2.m_Cost = -1;
						break;
					}
				}
			}
		}
	}


	//スパークの作成
	void GameStage::CreateSpark() {
		auto MultiSparkPtr = AddGameObject<MultiSpark>();
		//共有オブジェクトにスパークを登録
		SetSharedGameObject(L"MultiSpark", MultiSparkPtr);
	}
	//炎の作成
	void GameStage::CreateFire() {
		auto MultiFirePtr = AddGameObject<MultiFire>();
		//共有オブジェクトに炎を登録
		SetSharedGameObject(L"MultiFire", MultiFirePtr);
	}


	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		auto SwordPtr = AddGameObject<Sword>(
			Vector3(0.2, 0.5, 0.2),
			L"PlayerSword"
			);
		SetSharedGameObject(L"PlayerSword", SwordPtr);
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
		//HPバー
		AddGameObject<HPManeger<Player>>(PlayerPtr,Color4(0,1,0,1));
	}

	//敵の作成
	void GameStage::CreateEnemy() {
		auto MapPtr = GetSharedGameObject<StageCellMap>(L"StageCellMap");
		vector< vector<Vector3> > Vec1 = {
			{
				Vector3(0.5f, 0.5f, 0.5f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(-3.5f, 0.25f, 3.5f)
			},
		};
		//オブジェクトの作成
		for (auto v : Vec1) {
			auto SwordPtr = AddGameObject<Sword>(
				Vector3(0.2, 0.7, 0.2),
				L"EnemySword"
				);
			SetSharedGameObject(L"EnemySword", SwordPtr);
			auto EnemyPtr = AddGameObject<Enemy>(MapPtr, v[0], v[1], v[2]);
			//HPバー
			AddGameObject<HPManeger<Enemy>>(EnemyPtr, Color4(1, 0, 0, 1));
		}
	}


	void GameStage::OnCreate() {
		try {
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			////CSVファイルの読み込み
			m_GameStageCsv.SetFileName(DataDir + L"GameStage.csv");
			m_GameStageCsv.ReadCsv();

			//ビューとライトの作成
			CreateViewLight();
			//プレートの作成
			CreatePlate();
			//セルマップの作成
			CreateStageCellMap();
			//ボックスの作成
			CreateFixedBox();
			//スパークの作成
			CreateSpark();
			//炎の作成
			CreateFire();
			//プレーヤーの作成
			CreatePlayer();
			//敵の作成
			CreateEnemy();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
