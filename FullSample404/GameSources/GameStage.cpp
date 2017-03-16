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
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(200.0f, 200.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

		//描画コンポーネントの追加
		auto DrawComp = Ptr->AddComponent<BcPNTStaticDraw>();
		//スペキュラーなし
		DrawComp->DisableSpecular();
		DrawComp->SetFogEnabled(true);
		DrawComp->SetDiffuse(Color4(0.3f, 0.7f, 1.0f, 1.0f));

		//描画コンポーネントに形状（メッシュ）を設定
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//自分に影が映りこむようにする
		DrawComp->SetOwnShadowActive(true);

		//描画コンポーネントテクスチャの設定
		DrawComp->SetTextureResource(L"SKY_TX");
	}


	//球の作成
	void GameStage::CreateSphere() {
		AddGameObject<PcSphere>(Vector3(-7.5f, 0.5f, 5.0f));
		AddGameObject<PtSphere>(Vector3(-5.0f, 0.5f, 5.0f));
		AddGameObject<PctSphere>(Vector3(-2.5f, 0.5f, 5.0f));
		AddGameObject<PntSphere>(Vector3(0.0f, 0.5f, 5.0f), true);
		AddGameObject<PntSphere>(Vector3(2.5f, 0.5f, 5.0f), false);
		AddGameObject<PntSpSphere>(Vector3(5.0f, 0.5f, 5.0f), true);
		AddGameObject<PntSpSphere>(Vector3(7.5f, 0.5f, 5.0f), false);

	}


	//キャラクターの作成
	void GameStage::CreateChara() {
		AddGameObject<StaticChara>(Vector3(2.5f, 0.0f, 0.0f),false);
		AddGameObject<StaticChara>(Vector3(5.0f, 0.0f, 0.0f),true);
		AddGameObject<BoneChara>(Vector3(-5.0f, 0.0f, 0.0f),false);
		AddGameObject<BoneChara>(Vector3(-2.5f, 0.0f, 0.0f),true);
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
			//球の作成
			CreateSphere();
			//キャラクターの作成
			CreateChara();
			//プレイヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}



}
//end basecross
