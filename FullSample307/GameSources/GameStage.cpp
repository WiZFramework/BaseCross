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
	}

	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	void GameStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
			//オブジェクトのグループを作成する
			auto Group = CreateSharedObjectGroup(L"EnemyGroup");
			//ゲームオブジェクトビルダー
			GameObjecttXMLBuilder Builder;
			//ゲームオブジェクトの登録
			Builder.Register<TilingPlate>(L"TilingPlate");
			Builder.Register<TilingFixedBox>(L"TilingFixedBox");
			Builder.Register<FrameBox>(L"FrameBox");
			wstring DataDir;
			App::GetApp()->GetDataDirectory(DataDir);
			//XMLからゲームオブジェクトの構築
			wstring XMLStr = DataDir + L"GameStage";
			XMLStr += Util::IntToWStr(m_StageNum);
			XMLStr += L".xml";
			Builder.Build(GetThis<Stage>(), XMLStr, L"GameStage/GameObject");
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
