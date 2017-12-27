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
		auto bkCamera = App::GetApp()->GetScene<Scene>()->GetBackupCamera();
		if (!bkCamera) {
			auto PtrLookAtCamera = ObjectFactory::Create<LookAtCamera>(10.0f);
			PtrView->SetCamera(PtrLookAtCamera);
			PtrLookAtCamera->SetEye(Vec3(0.0f, 5.0f, -20.0f));
			PtrLookAtCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		}
		else {
			PtrView->SetCamera(bkCamera);
		}
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
		//シャドウマップのライトの高さを調整(ステージが広いため)
		Shadowmap::SetLightHeight(50.0f);
	}


	//物理計算オブジェクトの作成
	void GameStage::CreatePhysicsObjects() {
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"SeekObjectGroup");
		//下の台
		AddGameObject<FixedPsBox>(Vec3(30.0f, 1.0f, 30.0f), Quat(), Vec3(0.0f, -0.5f, 0.0f));
		//凸面
		AddGameObject<ActivePsConvex>(Vec3(-10.0f, 1.0f, 0.0f));
		AddGameObject<ActivePsConvex>(Vec3(10.0f, 1.0f, 0.0f));
		//球体
		AddGameObject<ActivePsSphere>(1.0f, Vec3(0.0f, 1.0f, -10.f));
		AddGameObject<ActivePsSphere>(1.0f, Vec3(0.0f, 1.0f, 10.f));
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
			//物理計算有効
			SetPhysicsActive(true);
			//ビューとライトの作成
			CreateViewLight();
			//物理計算オブジェクトの作成
			CreatePhysicsObjects();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
