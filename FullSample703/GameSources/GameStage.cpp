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
		//下の台
		AddGameObject<FixedPsBox>(Vec3(30.0f, 1.0f, 30.0f), Quat(), Vec3(0.0f, -0.5f, 0.0f));
		//ボールジョイント用
		auto BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(-2.0f, 1.5f, 4.0f), 5.0f, true);
		SetSharedGameObject(L"Brick01", BrickPtr);
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(-2.0f, 1.5f, 3.0f), 5.0f, false);
		SetSharedGameObject(L"Brick02", BrickPtr);
		//球関節ジョイント用
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(-1.0f, 1.5f, 4.0f), 5.0f, true);
		SetSharedGameObject(L"Brick03", BrickPtr);
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(-1.0f, 1.5f, 3.0f), 5.0f, false);
		SetSharedGameObject(L"Brick04", BrickPtr);
		//蝶番ジョイント用
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(0.0f, 1.5f, 4.0f),5.0f,true);
		SetSharedGameObject(L"Brick05", BrickPtr);
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(0.0f, 1.5f, 3.0f), 5.0f, false);
		SetSharedGameObject(L"Brick06", BrickPtr);
		//スライダージョイント用
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(1.0f, 1.5f, 4.0f), 5.0f, true);
		SetSharedGameObject(L"Brick07", BrickPtr);
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(1.0f, 1.5f, 3.0f), 5.0f, false);
		SetSharedGameObject(L"Brick08", BrickPtr);
		//固定ジョイント用
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(2.0f, 1.5f, 4.0f), 5.0f, true);
		SetSharedGameObject(L"Brick09", BrickPtr);
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(2.0f, 1.5f, 3.0f), 5.0f, false);
		SetSharedGameObject(L"Brick10", BrickPtr);
		//ユニバーサルジョイント用
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(3.0f, 1.5f, 4.0f), 5.0f, true);
		SetSharedGameObject(L"Brick11", BrickPtr);
		BrickPtr = AddGameObject<PsBrick>(Vec3(0.25f, 0.7f, 1.0f), Quat(), Vec3(3.0f, 1.5f, 3.0f), 5.0f, false);
		SetSharedGameObject(L"Brick12", BrickPtr);

		//ジョイントマネージャの作成
		auto MngPtr = AddGameObject<PsJointManager>();
		SetSharedGameObject(L"PsJointManager", MngPtr);
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
