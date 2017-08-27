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
		PtrTrans->SetScale(20.0f, 20.0f, 1.0f);
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

	//固定のボックスの作成
	void GameStage::CreateFixedBox() {
		//配列の初期化
		vector< vector<Vec3> > Vec = {
			{
				Vec3(1.0f, 0.5f, 20.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(9.0f, 0.25f, 0.0f)
			},
			{
				Vec3(1.0f, 0.5f, 20.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-9.0f, 0.25f, 0.0f)
			},
			{
				Vec3(20.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.25f, 9.0f)
			},
			{
				Vec3(20.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(0.0f, 0.25f, -9.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}


	//ボールの作成
	void GameStage::CreateBalls() {
		//ボールの作成
		auto Ball1 = AddGameObject<Ball>(Vec3(-2.5f, 0.125f, 5.0f), true);
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Ball1", Ball1);
		auto Ball2 = AddGameObject<Ball>(Vec3(2.5f, 0.125f, 5.0f), false);
		SetSharedGameObject(L"Ball2", Ball2);
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
			//プレートの作成
			CreatePlate();
			//固定のボックスの作成
			CreateFixedBox();
			//ボールの作成
			CreateBalls();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}


}
//end basecross
