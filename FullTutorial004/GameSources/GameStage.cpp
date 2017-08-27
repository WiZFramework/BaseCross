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


	//プレートの作成
	void GameStage::CreatePlate() {
		//ステージへのゲームオブジェクトの追加
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(50.0f, 50.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

		auto ColPtr = Ptr->AddComponent<CollisionRect>();
		//描画コンポーネントの追加
		auto DrawComp = Ptr->AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		DrawComp->SetFogEnabled(true);
		//自分に影が映りこむようにする
		DrawComp->SetOwnShadowActive(true);

		//描画コンポーネントテクスチャの設定
		DrawComp->SetTextureResource(L"SKY_TX");
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
		//配列の初期化
		vector<Vec3> Vec = {
			{ 0, 0.25f, 10.0f },
			{ 10.0f, 0.25f, 0.0f },
			{ -10.0f, 0.25f, 0.0f },
			{ 0, 0.25f, -10.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<Enemy1>(v);
		}

		//配列の初期化
		vector<Vec3> Vec2 = {
			{ 15.0f, 0.25f, 0.0f },
			{ -15.0f, 0.25f, 0.0f },
			{ 0.0f, 0.25f, 15.0f },
			{ 0.0f, 0.25f, -15.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec2) {
			AddGameObject<Enemy2>(v);
		}

		//配列の初期化
		vector<Vec3> Vec3 = {
			{ 10.0f, 0.25f, 10.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec3) {
			AddGameObject<Enemy3>(v);
		}



	}





	void GameStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
			//プレートの作成
			CreatePlate();
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
