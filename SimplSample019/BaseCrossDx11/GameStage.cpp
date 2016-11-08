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
		wstring strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"trace2.png";
		App::GetApp()->RegisterTexture(L"TRACE2_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		strTexture = DataDir + L"mozo.png";
		App::GetApp()->RegisterTexture(L"MOZO_TX", strTexture);

	}



	//ビューとライトの作成
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtCamera = ObjectFactory::Create<Camera>();
//		auto PtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtCamera);
		{
			//LookAtCameraの場合
			//PtCamera->SetEye(Vector3(0.0f, 5.0f, -5.0f));
			//PtCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
			//PtCamera->SetMaxArm(20.0f);
			//PtCamera->SetMinArm(1.5f);
			//PtCamera->SetToTargetLerp(0.1f);
			//PtCamera->SetTargetToAt(Vector3(0, 1.0f, 0));
		}
		{
			//カメラマンによるカメラの場合
			PtCamera->SetEye(Vector3(0.0f, 2.0f, -2.0f));
			PtCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
		}
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
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		Matrix4X4 WorldMat;
		WorldMat.DefTransformation(
			Vector3(200.0f, 200.0f, 1.0f),
			Qt,
			Vector3(0.0f, 0.0f, 0.0f)
		);
		PtrTrans->SetScale(200.0f, 200.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

		//描画コンポーネントの追加
		auto DrawComp = Ptr->AddComponent<PNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//自分に影が映りこむようにする
		DrawComp->SetOwnShadowActive(true);

		//描画コンポーネントテクスチャの設定
		DrawComp->SetTextureResource(L"SKY_TX");


	}

	//固定のボックスの作成
	void GameStage::CreateFixedBox() {
		//配列の初期化
		vector< vector<Vector3> > Vec = {
			{
				Vector3(1.0f, 0.25f, 1.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(0.0f, 1.5f, 4.0f)
			},
			{
				Vector3(1.5f, 1.0f, 1.5f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(-2.0f, 0.5f, 3.0f)
			},
			{
				Vector3(1.5f, 0.5f, 1.5f),
				Vector3(0.0f,0, 0),
				Vector3(-3.0f, 0.25f, 2.5f)
			},
		};
		//オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//頭の敵の作成
	void GameStage::CreateHead() {
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//配列の初期化
		vector<Vector3> Vec = {
			{ 3.0f, 0.125f, -2.0f },
		};

		//配置オブジェクトの作成
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<HeadEnemy>(Vec[count]);
		}

	}

	//もぞ敵の作成
	void GameStage::CreateMozo() {
		//配列の初期化
		vector<Vector3> Vec = {
			{ -2.6f, 5.0f, 2.25f },
			{ -2.5f, 5.0f, 3.25f },
			{ -1.5f, 5.0f, 3.0f },
			{ -1.3f, 5.0f, 3.25f },
			{ -1.7f, 5.0f, 2.6f },
			{ -2.0f, 5.0f, 2.3f },
			{ -2.4f, 5.0f, 2.8f },
			{ -2.2f, 5.0f, 2.7f },
			{ -3.5f, 5.0f, 2.0f },
			{ 0.0f, 5.0f, 4.0f },
			{ 0.2f, 5.0f, 4.2f },
			{ -0.2f, 5.0f, 3.8f },
		};

		//配置オブジェクトの作成
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<MozoEnemy>(Vec[count]);
		}

	}


	//固定のシリンダーの作成
	void GameStage::CreateFixedCylinder() {
		//配列の初期化
		vector< vector<Vector3> > Vec = {
			{
				Vector3(1.0f, 0.5f, 0.5f),
				Vector3(0, XM_PIDIV4,0),
				Vector3(3.0f, 0.25f, -3.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<FixedCylinder>(v[0], v[1], v[2]);
		}
	}


	//壁の作成
	void GameStage::CreateTileWall() {
		auto Grpup = CreateSharedObjectGroup(L"WallGroup");

		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(0, 0, 0));
		auto Ptr = AddGameObject<TileWall>(
			L"WALL_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, 5.0f)
			);
		Grpup->IntoGroup(Ptr);
		Qt.RotationRollPitchYawFromVector(Vector3(0, XM_PIDIV2, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(5.0f, 2.5f, 0.0f)
			);
		Grpup->IntoGroup(Ptr);

		Qt.RotationRollPitchYawFromVector(Vector3(0, -XM_PIDIV2, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(-5.0f, 2.5f, 0.0f)
			);
		Grpup->IntoGroup(Ptr);

		Qt.RotationRollPitchYawFromVector(Vector3(0, XM_PI, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, -5.0f)
			);
		Grpup->IntoGroup(Ptr);

		Qt.RotationRollPitchYawFromVector(Vector3(-XM_PIDIV2, 0, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL_TX",
			10, 10,
			Vector3(10.0f, 10.0f, 1.0f),
			Qt,
			Vector3(0.0f, 5.0f, 0.0f)
			);
		Grpup->IntoGroup(Ptr);


	}

	//くぐる壁の作成
	void GameStage::CreateThroughWall() {
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(0, 0, 0));
		auto Ptr = AddGameObject<ThroughWall>(
			L"SKY_TX",
			50, 50,
			false,
			Vector3(2.0f, 2.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, 4.9f)
			);

	}


	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>();
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//カメラマンの作成
	void GameStage::CreateCameraman() {
		//カメラマンの作成
		AddGameObject<Cameraman>();
	}



	void GameStage::OnCreate() {
		try {
			//リソースの作成
			CreateResourses();
			//ビューとライトの作成
			CreateViewLight();
			//プレートの作成
			CreatePlate();
			//壁の作成
			CreateTileWall();
			//くぐる壁の作成
			CreateThroughWall();
			//固定のボックスの作成
			CreateFixedBox();
			//頭の敵の作成
			CreateHead();
			//もぞ敵の作成
			CreateMozo();
			//固定のシリンダーの作成
			CreateFixedCylinder();
			//プレーヤーの作成
			CreatePlayer();
			//カメラマンの作成
			CreateCameraman();

		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
