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


	//ラグドールの作成
	void GameStage::CreateRagdoll() {
		Quat Qt;
		Qt.rotationX(0.3f);
		//ラグドール
		auto RagdollPtr = AddGameObject<Ragdoll>(Vec3(0.0f, 3.0f, 5.0f), Qt);
		//シェア配列にラグドールを追加
		SetSharedGameObject(L"Ragdoll", RagdollPtr);
		auto RagMngPtr = AddGameObject<RagdollManager>(RagdollPtr);
		//シェア配列にラグドールマネージャを追加
		SetSharedGameObject(L"RagdollManager", RagMngPtr);
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
			//下の台
			AddGameObject<FixedPsBox>(Vec3(30.0f, 1.0f, 30.0f), Quat(), Vec3(0.0f, -0.5f, 0.0f));
			//ラグドール
			CreateRagdoll();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
