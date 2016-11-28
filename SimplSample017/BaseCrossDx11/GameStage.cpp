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
	//リソースの作成
	void GameStage::CreateResourses() {
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		auto ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"Chara_R.bmf");
		App::GetApp()->RegisterResource(L"Chara_R_MESH", ModelMesh);
		auto StaticModelMesh = MeshResource::CreateStaticModelMesh(DataDir, L"Chara_Rst.bmf");
		App::GetApp()->RegisterResource(L"Chara_Rst_MESH", StaticModelMesh);
	}



	//ビューとライトの作成
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrLookAtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtrLookAtCamera);
		PtrLookAtCamera->SetEye(Vector3(0.0f, 5.0f, -5.0f));
		PtrLookAtCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
		//シングルライトの作成
		auto PtrSingleLight = CreateLight<SingleLight>();
		//ライトの設定
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
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

	//固定のモデルの作成
	void GameStage::CreateStaticModel() {
		AddGameObject<StaticModel>(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(5.0f, 0.0f, 5.0f)
			);

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
			//リソースの作成
			CreateResourses();
			//ビューとライトの作成
			CreateViewLight();
			//固定のモデルの作成
			CreateStaticModel();
			//プレートの作成
			CreatePlate();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
