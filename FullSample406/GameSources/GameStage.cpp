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


	//プレートの作成
	void GameStage::CreatePlate() {
		//ステージへのゲームオブジェクトの追加
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quaternion Qt(Vector3(1.0f, 0, 0), XM_PIDIV2);
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(50.0f, 50.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

		auto ColPtr = Ptr->AddComponent<CollisionRect>();
		//描画コンポーネントの追加
		auto DrawComp = Ptr->AddComponent<PNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//自分に影が映りこむようにする
		DrawComp->SetOwnShadowActive(true);

		//描画コンポーネントテクスチャの設定
		DrawComp->SetTextureResource(L"SKY_TX");
	}



	//固定の回転ボックスの作成
	void GameStage::CreateRollingBox() {
		auto GroupPtr = AddGameObject<RollingBoxGroup>();
		//シェアオブジェクトに設定
		SetSharedGameObject(L"RollingBoxGroup", GroupPtr);
		Vector3 Scale(1.25f, 0.5f, 1.25f);
		Vector3 Rot(0.0f, 0.0f, 0.0f);
		Vector3 Pos;

		//配列の初期化
		vector< vector<Vector3> > Vec;
		//奥側のオブジェクト
		for (int x = -20; x < 20; x+= 2) {
			for (int z = 5; z < 20; z+= 2) {
				Pos = Vector3((float)x, 0.25f, (float)z);
				vector<Vector3> temp = { Scale, Rot, Pos };
				Vec.push_back(temp);
			}
		}
		//手前側のオブジェクト
		for (int x = -20; x < 20; x += 2) {
			for (int z = -5; z > -20; z -= 2) {
				Pos = Vector3((float)x, 0.25f, (float)z);
				vector<Vector3> temp = { Scale, Rot, Pos };
				Vec.push_back(temp);
			}
		}
		//オブジェクトの作成
		float RotParam = 1.0f;
		for (auto v : Vec) {
			//乱数で回転係数を0.5から1.5の間に設定
			RotParam = Util::RandZeroToOne(true) + 0.5f;
			AddGameObject<RollingBox>(v[0], v[1], v[2], RotParam);
		}
	}

	//プレイヤーの作成
	void GameStage::CreatePlayer() {

		CreateSharedObjectGroup(L"AttackBall");
		//アタックボールは10個用意する
		for (int i = 0; i < 10; i++) {
			AddGameObject<AttackBall>();
		}

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
			//固定の回転ボックスの作成
			CreateRollingBox();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
