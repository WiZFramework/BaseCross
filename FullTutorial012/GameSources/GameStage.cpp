/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	using namespace sce::PhysicsEffects;


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

	//物理計算オブジェクトの作成
	void GameStage::CreatePhysicsObjects() {
		Quat Qt1, Qt2, Qt3, Qt4;
		Qt1.rotationZ(2.0f);
		Qt2.rotationX(0.7f);
		Qt3.rotationZ(-2.0f);
		Qt4.rotationX(-0.7f);
		//下の台
		AddGameObject<FixedPsBox>(Vec3(15.0f, 0.25f, 15.0f), Quat(), Vec3(0.0f, 0.125f, 10.0f));

		//上から降ってくるボックス
		AddGameObject<ActivePsBox>(Vec3(2.0f, 1.0f, 1.0f), Quat(), Vec3(2.0f, 1.0f, 9.0f));
		AddGameObject<ActivePsBox>(Vec3(2.0f, 1.0f, 1.0f), Qt2, Vec3(1.0f, 3.0f, 8.0f));
		AddGameObject<ActivePsBox>(Vec3(2.0f, 1.0f, 1.0f), Qt3, Vec3(-2.0f, 4.0f, 8.5f));
		AddGameObject<ActivePsBox>(Vec3(1.0f, 1.0f, 2.0f), Qt3, Vec3(2.0f, 10.0f, 7.0f));
		AddGameObject<ActivePsBox>(Vec3(1.0f, 1.0f, 1.0f), Qt4, Vec3(1.0f, 11.0f, 13.0f));
		AddGameObject<ActivePsBox>(Vec3(1.0f, 1.0f, 2.0f), Qt1, Vec3(2.5f, 12.0f, 14.0f));
		AddGameObject<ActivePsBox>(Vec3(1.0f, 1.0f, 1.0f), Qt2, Vec3(-3.0f, 13.0f, 7.0f));
		AddGameObject<ActivePsBox>(Vec3(1.0f, 1.0f, 1.0f), Qt3, Vec3(-3.0f, 14.0f, 6.0f));
		AddGameObject<ActivePsBox>(Vec3(2.0f, 1.0f, 1.0f), Qt4, Vec3(2.5f, 15.0f, 15.0f));
		//上から降ってくる球体
		AddGameObject<ActivePsSphere>(1.0f, Quat(), Vec3(0.0f, 6.0f, 10.0f));
		AddGameObject<ActivePsSphere>(1.0f, Quat(), Vec3(-0.5f, 7.0f, 11.0f));
		AddGameObject<ActivePsSphere>(1.0f, Quat(), Vec3(0.0f, 8.0f, 10.5f));

	}

	void GameStage::OnCreate() {
		try {
			//物理計算有効
			SetPhysicsActive(true);
			//アップデートを無効にするには以下コメント外す
			//GetPhysicsManager()->SetUpdateActive(false);
			//ワイアフレーム描画を無効にするには以下コメント外す
			//GetPhysicsManager()->SetDrawActive(false);
			//ビューとライトの作成
			CreateViewLight();
			//プレートの作成
			CreatePlate();
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
