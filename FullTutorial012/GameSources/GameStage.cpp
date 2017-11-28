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
		auto PtrLookAtCamera = ObjectFactory::Create<LookAtCamera>(10.0f);
		PtrView->SetCamera(PtrLookAtCamera);
		PtrLookAtCamera->SetEye(Vec3(0.0f, 5.0f, -20.0f));
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

	//物理計算ボックスの作成
	void GameStage::CreatePhysicsBox() {
		//下の台
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeStatic, Vec3(6.0f, 0.5f, 6.0f), Quat(), Vec3(0.0f, 0.25f, 5.0f));
		//影響を受けるボックス
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive,Vec3(2.0f, 0.25f, 1.0f), Quat(), Vec3(0.0f, 0.625f, 5.0f));
		Quat Qt1,Qt2, Qt3, Qt4;
		Qt1.rotationZ(2.0f);
		Qt2.rotationY(0.7f);
		Qt3.rotationZ(-2.0f);
		Qt4.rotationY(-0.7f);
		//上から降ってくるボックス
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(2.0f, 0.25f, 1.0f), Qt2 * Qt1, Vec3(0.0f, 3.5f, 5.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt1 * Qt2, Vec3(0.0f, 6.0f, 5.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt1, Vec3(1.0f, 4.0f, 6.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt2, Vec3(-2.0f, 4.0f, 4.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt3, Vec3(2.0f, 4.0f, 3.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt4, Vec3(1.0f, 6.0f, 3.0f));

		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt1, Vec3(2.5f, 8.0f, 6.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt2, Vec3(-3.0f, 8.0f, 4.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt3, Vec3(-3.0f, 8.0f, 3.0f));
		AddGameObject<PhysicsBox>(PsMotionType::MotionTypeActive, Vec3(1.0f, 0.25f, 1.0f), Qt4, Vec3(2.5f, 8.0f, 7.0f));

	}

	void GameStage::OnCreate() {
		try {
			SetPhysicsManager(true);
			//ビューとライトの作成
			CreateViewLight();
			//プレートの作成
			CreatePlate();
			//物理計算ボックスの作成
			CreatePhysicsBox();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
