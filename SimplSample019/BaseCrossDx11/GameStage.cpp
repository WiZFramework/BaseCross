/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	現在の理科室ステージクラス実体
	//--------------------------------------------------------------------------------------
	//固定のボックスの作成
	void ScienceRoom::CreateFixedBox() {
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

	//入口ドアの作成
	void ScienceRoom::CreateRoomDoor() {
		//オブジェクトの作成
		AddGameObject<RoomDoor>(
			L"TRACE_TX",
			Vector3(0.5f, 1.0f, 0.1f), 
			Vector3(0.0f, XM_PI, 0.0f), 
			Vector3(0.0f, 0.5f, -5.0f)
			);

	}

	//ウインドウの作成
	void ScienceRoom::CreateRoomWindow() {
		//オブジェクトの作成
		AddGameObject<RoomWindow>(
			Vector3(1.0f, 1.0f, 0.1f),
			Vector3(0.0f, XM_PIDIV2, 0.0f),
			Vector3(5.0f, 1.0f, -2.0f)
			);


	}

	//頭の作成
	void ScienceRoom::CreateFixHead() {
		//オブジェクトの作成
		AddGameObject<FixedHead>(
			Vector3(3.0f, 1.0f, -3.0f)
			);

	}


	//壁の作成
	void ScienceRoom::CreateTileWall() {
		auto Grpup = CreateSharedObjectGroup(L"WallGroup");

		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(0, 0, 0));
		auto Ptr = AddGameObject<TileWall>(
			L"WALL2_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, 5.0f)
			);
		Grpup->IntoGroup(Ptr);
		Qt.RotationRollPitchYawFromVector(Vector3(0, XM_PIDIV2, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL2_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(5.0f, 2.5f, 0.0f)
			);
		Grpup->IntoGroup(Ptr);

		Qt.RotationRollPitchYawFromVector(Vector3(0, -XM_PIDIV2, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL2_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(-5.0f, 2.5f, 0.0f)
			);
		Grpup->IntoGroup(Ptr);

		Qt.RotationRollPitchYawFromVector(Vector3(0, XM_PI, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL2_TX",
			20, 5,
			Vector3(10.0f, 5.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, -5.0f)
			);
		Grpup->IntoGroup(Ptr);

		Qt.RotationRollPitchYawFromVector(Vector3(-XM_PIDIV2, 0, 0));
		Ptr = AddGameObject<TileWall>(
			L"WALL2_TX",
			10, 10,
			Vector3(10.0f, 10.0f, 1.0f),
			Qt,
			Vector3(0.0f, 5.0f, 0.0f)
			);
		Grpup->IntoGroup(Ptr);


	}



	//--------------------------------------------------------------------------------------
	//	現在の理科室ステージクラス
	//--------------------------------------------------------------------------------------
	//プレートの作成
	void NowScienceRoom::CreatePlate() {

		//ステージへのゲームオブジェクトの追加
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(10.0f, 10.0f, 1.0f);
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
		DrawComp->SetTextureResource(L"WALL_TX");

	}

	//ビューとライトの作成
	void NowScienceRoom::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtCamera);
		{
			//LookAtCameraの場合
			PtCamera->SetMaxArm(2.0f);
			PtCamera->SetMinArm(1.0f);
			PtCamera->SetAt(Vector3(0.0f, 0.125f, -3.0f));
			PtCamera->SetEye(Vector3(0.0f, 0.75f, -4.5f));
			PtCamera->UpdateArmLengh();
			PtCamera->SetToTargetLerp(0.1f);
			PtCamera->SetTargetToAt(Vector3(0, 0.125f, 0));
		}
		//シングルライトの作成
		auto PtrSingleLight = CreateLight<SingleLight>();
		//ライトの設定
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}

	//くぐる壁の作成
	void NowScienceRoom::CreateThroughWall() {
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(0, 0, 0));

		auto Ptr = AddGameObject<TileWall>(
			L"SKY_TX",
			1, 1,
			Vector3(2.0f, 2.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, 4.99f)
			);
		SetSharedGameObject(L"Goal", Ptr);


		auto ThroughPtr = AddGameObject<ThroughWall>(
			L"SKY_TX",
			50, 50,
			false,
			Vector3(2.0f, 2.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, 4.8f)
			);

	}

	//頭の敵の作成
	void NowScienceRoom::CreateHead() {
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//配列の初期化
		vector<Vector3> Vec = {
			{ 0.0f, 0.125f, 0.0f },
		};

		//配置オブジェクトの作成
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<HeadEnemy>(Vec[count]);
		}

	}

	//もぞ敵の作成
	void NowScienceRoom::CreateMozo() {
		//配列の初期化
		vector<Vector3> Vec = {
			{ -2.6f, 5.0f, 2.25f },
			{ -2.5f, 5.0f, 3.25f },
			{ -1.5f, 5.0f, 3.0f },
			{ -1.3f, 5.0f, 3.25f },
			{ -1.7f, 5.0f, 2.6f },
			{ -2.0f, 5.0f, 2.3f },
			{ -2.4f, 5.0f, 2.8f },
			{ -3.3f, 5.0f, 2.2f },
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
	void NowScienceRoom::CreateFixedCylinder() {
		//オブジェクトの作成
		auto PtrCylinder = AddGameObject<FixedCylinder>(
			Vector3(1.0f, 0.5f, 0.5f),
			Vector3(0, XM_PIDIV4, 0),
			Vector3(3.0f, 0.25f, -3.0f)
			);
		//シェア配列に追加
		SetSharedGameObject(L"FixedCylinder", PtrCylinder);
	}

	//平和の固定のシリンダーの作成
	void NowScienceRoom::CreatteClearFixedCylinder() {
		//オブジェクトの作成
		auto PtrCylinder = AddGameObject<FixedCylinder>(
			Vector3(0.5f, 1.0f, 0.5f),
			Vector3(0, XM_PIDIV4, 0),
			Vector3(3.0f, 0.5f, -3.0f)
			);
		//シェア配列に追加
		SetSharedGameObject(L"FixedCylinder", PtrCylinder);

	}


	//シリンダーに付属の吹きだしの作成
	void NowScienceRoom::CreatePopupBalloon() {
		auto PtrBalloon = AddGameObject<PopupBalloon>(L"BALLOON_0_TX", Vector3(1.0f, 0.5f, 1.0f));
		//シェア配列に追加
		SetSharedGameObject(L"PopupBalloon", PtrBalloon);
	}

	//プレイヤーの作成
	void NowScienceRoom::CreatePlayer() {
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>(
			Vector3(0, 0.125, -3.0f), Vector3(0, 0, 0));
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	void NowScienceRoom::OnCreate() {
		try {
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
			//入口ドアの作成
			CreateRoomDoor();
			//ウインドウの作成
			CreateRoomWindow();
			auto PtrParam = App::GetApp()->GetScene<Scene>()->GetGameParamaters();
			if (!PtrParam.m_IsHeadTaskClear) {
				//もぞ敵の作成
				CreateMozo();
				//頭の敵の作成
				CreateHead();
				//固定のシリンダーの作成
				CreateFixedCylinder();
			}
			else {
				//敵ではない頭
				CreateFixHead();
				//平和な固定のシリンダーの作成
				CreatteClearFixedCylinder();
			}
			//シリンダーに付属の吹きだしの作成
			CreatePopupBalloon();
			//プレーヤーの作成
			CreatePlayer();

		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	過去の理科室ステージクラス
	//--------------------------------------------------------------------------------------
	//プレートの作成
	void PastScienceRoom::CreatePlate() {

		//ステージへのゲームオブジェクトの追加
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(10.0f, 10.0f, 1.0f);
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
		DrawComp->SetTextureResource(L"FLOOR_TX");

	}
	//ビューとライトの作成
	void PastScienceRoom::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtCamera);
		{
			//LookAtCameraの場合
			PtCamera->SetMaxArm(2.0f);
			PtCamera->SetMinArm(1.0f);
			PtCamera->SetAt(Vector3(0.0f, 0.125f, -3.0f));
			PtCamera->SetEye(Vector3(0.0f, 0.75f, -4.5f));
			PtCamera->UpdateArmLengh();
			PtCamera->SetToTargetLerp(0.1f);
			PtCamera->SetTargetToAt(Vector3(0, 0.125f, 0));
		}
		//シングルライトの作成
		auto PtrSingleLight = CreateLight<SingleLight>();
		//ライトの設定
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}

	//くぐる壁の作成
	void PastScienceRoom::CreateThroughWall() {
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Vector3(0, 0, 0));

		auto Ptr = AddGameObject<TileWall>(
			L"SKY_TX",
			1, 1,
			Vector3(2.0f, 2.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, 4.99f)
			);
		SetSharedGameObject(L"Goal", Ptr);


		auto ThroughPtr = AddGameObject<ThroughWall>(
			L"SKY_TX",
			50, 50,
			false,
			Vector3(2.0f, 2.0f, 1.0f),
			Qt,
			Vector3(0.0f, 2.5f, 4.8f)
			);

	}

	//頭の敵の作成
	void PastScienceRoom::CreateHead() {
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//配列の初期化
		vector<Vector3> Vec = {
			{ 0.0f, 0.125f, 0.0f },
		};

		//配置オブジェクトの作成
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<HeadEnemy>(Vec[count]);
		}

	}

	//固定のシリンダーの作成
	void PastScienceRoom::CreateFixedCylinder() {
		//オブジェクトの作成
		auto PtrCylinder = AddGameObject<FixedCylinder>(
			Vector3(0.5f, 1.0f, 0.5f),
			Vector3(0, XM_PIDIV4, 0),
			Vector3(3.0f, 0.5f, -3.0f)
			);
		//シェア配列に追加
		SetSharedGameObject(L"FixedCylinder", PtrCylinder);
	}

	//プレイヤーの作成
	void PastScienceRoom::CreatePlayer() {
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>(
			Vector3(0, 0.125, -3.0f), Vector3(0, 0, 0));
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	void PastScienceRoom::OnCreate() {
		try {
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
			//入口ドアの作成
			CreateRoomDoor();
			//ウインドウの作成
			CreateRoomWindow();
			//頭の作成
			CreateFixHead();
			//固定のシリンダーの作成
			CreateFixedCylinder();
			//プレーヤーの作成
			CreatePlayer();

		}
		catch (...) {
			throw;
		}
	}


	//--------------------------------------------------------------------------------------
	//	クリアステージクラス
	//--------------------------------------------------------------------------------------
	void ClearStage::OnCreate() {
		try {
			auto PtrView = CreateView<SingleView>();
			auto PtrStr = AddGameObject<StringGameObject>();
			auto& param = App::GetApp()->GetScene<Scene>()->GetGameParamaters();
			auto LifesStr = Util::UintToWStr(param.m_PlayerLife);
			wstring ClearStr = L"クリア\n残りのライフ: ";
			ClearStr += LifesStr;
			ClearStr += L"\nもう一度やるにはAボタン";

			PtrStr->SetText(ClearStr);
		}
		catch (...) {
			throw;
		}
	}

	void ClearStage::OnUpdate() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Aボタンが押された瞬間なら、初期化に戻る
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
				auto& param = App::GetApp()->GetScene<Scene>()->GetGameParamaters();
				param.m_BalloonOpenTask = false;
				param.m_IsHeadTaskClear = false;
				param.m_PlayerLife = 5;

				auto ScenePtr = App::GetApp()->GetScene<Scene>();
				PostEvent(0.0f, GetThis<ObjectInterface>(), ScenePtr, L"ToNowScienceRoom");
			}
		}

	}

	//--------------------------------------------------------------------------------------
	//	ゲームオーバーステージクラス
	//--------------------------------------------------------------------------------------
	void GameOverStage::OnCreate() {
		try {
			auto PtrView = CreateView<SingleView>();
			auto PtrStr = AddGameObject<StringGameObject>();
			PtrStr->SetText(L"ゲームオーバー\nもう一度やるにはAボタン");
		}
		catch (...) {
			throw;
		}
	}

	void GameOverStage::OnUpdate() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Aボタンが押された瞬間なら、初期化に戻る
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
				auto& param = App::GetApp()->GetScene<Scene>()->GetGameParamaters();
				param.m_BalloonOpenTask = false;
				param.m_IsHeadTaskClear = false;
				param.m_PlayerLife = 5;

				auto ScenePtr = App::GetApp()->GetScene<Scene>();
				PostEvent(0.0f, GetThis<ObjectInterface>(), ScenePtr, L"ToNowScienceRoom");
			}
		}

	}


}
//end basecross
