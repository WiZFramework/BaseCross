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
		auto PlateNode = m_XmlDocReader->GetSelectSingleNode(L"GameStage/GameObjects/TilingPlate");
		if (!PlateNode) {
			throw BaseException(
				L"GameStage/GameObjects/TilingPlateが見つかりません",
				L"if (!PlateNode)",
				L"GameStage::CreatePlate()"
			);
		}
		//トークン（カラム）の配列
		vector<wstring> Tokens;
		auto ScaleNode = XmlDocReader::GetSelectSingleNode(PlateNode, L"Scale");
		auto RotNode = XmlDocReader::GetSelectSingleNode(PlateNode, L"Rot");
		auto PosNode = XmlDocReader::GetSelectSingleNode(PlateNode, L"Pos");
		wstring ScaleStr = XmlDocReader::GetText(ScaleNode);
		wstring RotStr = XmlDocReader::GetText(RotNode);
		wstring PosStr = XmlDocReader::GetText(PosNode);
		//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, ScaleStr, L',');
		//各トークン（カラム）をスケール、回転、位置に読み込む
		Vec3 Scale(
			(float)_wtof(Tokens[0].c_str()),
			(float)_wtof(Tokens[1].c_str()),
			(float)_wtof(Tokens[2].c_str())
		);
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, RotStr, L',');
		Vec3 Rot;
		//回転は「XM_PIDIV2」の文字列になっている場合がある
		Rot.x = (Tokens[0] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[0].c_str());
		Rot.y = (Tokens[1] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[1].c_str());
		Rot.z = (Tokens[2] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[2].c_str());
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, PosStr, L',');
		Vec3 Pos(
			(float)_wtof(Tokens[0].c_str()),
			(float)_wtof(Tokens[1].c_str()),
			(float)_wtof(Tokens[2].c_str())
		);
		//プレートの回転の引数はクオータニオンになっているので変換
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Rot);
		//ステージへのゲームオブジェクトの追加
		AddGameObject<TilingPlate>(Scale, Qt, Pos, 1.0f, 1.0f);
	}

	//ボックスの作成
	void GameStage::CreateFixedBox() {
		auto BoxNodes = m_XmlDocReader->GetSelectNodes(L"GameStage/GameObjects/TilingFixedBox");
		if (!BoxNodes) {
			throw BaseException(
				L"GameStage/GameObjects/TilingFixedBoxが見つかりません",
				L"if (!BoxNodes)",
				L"GameStage::CreateFixedBox()"
			);
		}
		long CountNode = XmlDocReader::GetLength(BoxNodes);
		for (long i = 0; i < CountNode; i++) {
			auto Node = XmlDocReader::GetItem(BoxNodes, i);
			auto ScaleNode = XmlDocReader::GetSelectSingleNode(Node, L"Scale");
			auto RotNode = XmlDocReader::GetSelectSingleNode(Node, L"Rot");
			auto PosNode = XmlDocReader::GetSelectSingleNode(Node,L"Pos");
			wstring ScaleStr = XmlDocReader::GetText(ScaleNode);
			wstring RotStr = XmlDocReader::GetText(RotNode);
			wstring PosStr = XmlDocReader::GetText(PosNode);
			//トークン（カラム）の配列
			vector<wstring> Tokens;
			//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
			Util::WStrToTokenVector(Tokens, ScaleStr, L',');
			//各トークン（カラム）をスケール、回転、位置に読み込む
			Vec3 Scale(
				(float)_wtof(Tokens[0].c_str()),
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str())
			);
			Tokens.clear();
			Util::WStrToTokenVector(Tokens, RotStr, L',');
			Vec3 Rot;
			//回転は「XM_PIDIV2」の文字列になっている場合がある
			Rot.x = (Tokens[0] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[0].c_str());
			Rot.y = (Tokens[1] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[1].c_str());
			Rot.z = (Tokens[2] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[2].c_str());
			Tokens.clear();
			Util::WStrToTokenVector(Tokens, PosStr, L',');
			Vec3 Pos(
				(float)_wtof(Tokens[0].c_str()),
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str())
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
		//セルマップのノードを取得
		auto CellmapNode = m_XmlDocReader->GetSelectSingleNode(L"GameStage/CellMap");
		if (!CellmapNode) {
			throw BaseException(
				L"GameStage/CellMapが見つかりません",
				L"if (!CellmapNode)",
				L"GameStage::CreateEnemy()"
			);
		}
		//内容の文字列を取得
		wstring MapStr = XmlDocReader::GetText(CellmapNode);
		vector<wstring> LineVec;
		//最初に「改行」をデリミタとした文字列の配列にする
		Util::WStrToTokenVector(LineVec, MapStr, L'\n');
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
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			//XMLの読み込み
			m_XmlDocReader.reset(new XmlDocReader(DataDir + L"GameStage.xml"));
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

}
//end basecross
