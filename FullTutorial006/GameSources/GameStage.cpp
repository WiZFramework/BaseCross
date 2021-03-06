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
		PtrLookAtCamera->SetEye(Vec3(0.0f, 5.0f, -5.0f));
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
		Quat Qt(Vec3(1.0f, 0, 0), XM_PIDIV2);
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

	//追いかけるオブジェクトの作成
	void GameStage::CreateSeekObject() {
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//配列の初期化
		vector<Vec3> Vec = {
			{ 0, 0.125f, 10.0f },
			{ 10.0f, 0.125f, 0.0f },
			{ -10.0f, 0.125f, 0.0f },
			{ 0, 0.125f, -10.0f },
		};

		//配置オブジェクトの作成
		//ナンバースクエアの作成
		for (size_t count = 0; count < Vec.size(); count++) {
			auto Ptr = AddGameObject<SeekObject>(Vec[count]);
			//ナンバースクエアを作成して関連させる
			AddGameObject<NumberSquare>(Ptr, count);
		}

	}



	//固定のボックスの作成
	void GameStage::CreateFixedBox() {
		//配列の初期化
		vector< vector<Vec3> > Vec = {
			{
				Vec3(5.0f, 0.5f, 5.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(10.0f, 0.25f, 10.0f)
			},
			{
				Vec3(5.0f, 0.5f, 5.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(14.0f, 0.25f, 10.0f)
			},

			{
				Vec3(2.0f, 1.0f, 2.0f),
				Vec3(0, 0, 0),
				Vec3(10.0f, 0.5f, 10.0f)
			},
			{
				Vec3(4.0f, 1.0f, 4.0f),
				Vec3(0, 0, 0),
				Vec3(-10.0f, 0.5f, 10.0f)
			},
			{
				Vec3(10.0f, 0.5f, 10.0f),
				Vec3(-0.5f, 0.0f, -0.5f),
				Vec3(-10.0f, 0.25f, 10.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
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

	//上下移動しているボックスの作成
	void GameStage::CreateMoveBox() {
		CreateSharedObjectGroup(L"MoveBox");
		AddGameObject<MoveBox>(
			Vec3(5.0f, 0.5f, 5.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, -0.5f, 20.0f)
			);
	}

	//ヒットする球体の作成
	void GameStage::CreateSphere() {
		//配列の初期化
		vector<Vec3> Vec = {
			{ 20.0f, 0, 25.0f },
			{ 20.0f, 0, 0.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<SphereObject>(v);
		}
	}


	//半透明のスプライト作成
	void GameStage::CreateTraceSprite() {
		AddGameObject<TraceSprite>( true,
			Vec2(200.0f, 200.0f), Vec3(-500.0f, -280.0f,0.0f));
	}


	//壁模様のスプライト作成
	void GameStage::CreateWallSprite() {
		AddGameObject<WallSprite>(L"WALL_TX", false,
			Vec2(200.0f, 200.0f), Vec2(500.0f, -280.0f));
	}


	//スクロールするスプライト作成
	void GameStage::CreateScrollSprite() {
		AddGameObject<ScrollSprite>(L"TRACE_TX",true,
			Vec2(160.0f, 40.0f),Vec2(500.0f,-280.0f));
	}

	//左上で回転する立方体
	void GameStage::CreateRollingCube() {
		Quat Qt(Vec3(0.0f, 0.0, 1.0), XM_PIDIV4);
		AddGameObject<RollingCube>(true,
			Vec3(64.0f,64.0f,64.0f), 
			Qt,
			Vec3(-440,320,100.0f)
			);

	}

	//左上で回転するWall立方体
	void GameStage::CreateRollingWallCube() {
		Quat Qt(Vec3(0.0f, 0.0, 1.0), XM_PIDIV4);
		AddGameObject<RollingWallCube>(
			L"WALL_TX",
			false,
			Vec3(64.0f, 64.0f, 64.0f),
			Qt,
			Vec3(-320, 320, 100.0f)
			);

	}
	//白い立方体
	void GameStage::CreateWhiteCube() {
		Quat Qt(Vec3(0.0f, 1.0, 1.0), 0);
		AddGameObject<WhiteCube>(
			Vec3(1.0f, 1.0f, 1.0f),
			Qt,
			Vec3(0.0f,1.0f, 10.0f)
			);

	}
	//形状が変わる球体
	void GameStage::CreateTransSphere() {
		AddGameObject<TransSphere>(
			L"WALL_TX",
			false,
			Vec3(1.0f, 1.0f, 1.0f),
			Quat(),
			Vec3(10.0f, 2.0f, 10.0f)
			);
	}

	//でこぼこ床の作成
	void GameStage::CreateUnevenGround() {
		//でこぼこ床のデータの作成
		AddGameObject<UnevenGroundData>();
		//配列の初期化
		vector< vector<Vec3> > Vec = {
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(-4.0f, 0.0f, 5.0f)
			},
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-3.0f, 0.0f, 5.0f)
			},

			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(-4.0f, 0.0f, 7.0f)
			},
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-3.0f, 0.0f, 7.0f)
			},

			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(-4.0f, 0.0f, 9.0f)
			},
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-3.0f, 0.0f, 9.0f)
			},

			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(-4.0f, 0.0f, 11.0f)
			},
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-3.0f, 0.0f, 11.0f)
			},

			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(-4.0f, 0.0f, 13.0f)
			},
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-3.0f, 0.0f, 13.0f)
			},

			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, XM_PIDIV2, 0.0f),
				Vec3(-4.0f, 0.0f, 15.0f)
			},
			{
				Vec3(1.0f, 1.0f, 1.0f),
				Vec3(0.0f, -XM_PIDIV2, 0.0f),
				Vec3(-3.0f, 0.0f, 15.0f)
			},


		};
		//オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<UnevenGround>(v[0], v[1], v[2]);
		}

	}


	//スパークの作成
	void GameStage::CreateSpark() {
		auto MultiSparkPtr = AddGameObject<MultiSpark>();
		//共有オブジェクトにスパークを登録
		SetSharedGameObject(L"MultiSpark", MultiSparkPtr);
	}
	//炎の作成
	void GameStage::CreateFire() {
		auto MultiFirePtr = AddGameObject<MultiFire>();
		//共有オブジェクトに炎を登録
		SetSharedGameObject(L"MultiFire", MultiFirePtr);
	}





	void GameStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
			//プレートの作成
			CreatePlate();
			//固定のボックスの作成
			CreateFixedBox();
			//上下移動しているボックスの作成
			CreateMoveBox();
			//球体作成
			CreateSphere();
			//追いかけるオブジェクトの作成
			CreateSeekObject();
			//半透明のスプライト作成
			CreateTraceSprite();
			//壁模様のスプライト作成
			CreateWallSprite();
			//スクロールするスプライト作成
			CreateScrollSprite();
			//左上で回転する立方体
			CreateRollingCube();
			//左上で回転するWall立方体
			CreateRollingWallCube();
			//白い立方体
			CreateWhiteCube();
			//左上で形状が変わる球体
			CreateTransSphere();
			//でこぼこ床の作成
			CreateUnevenGround();
			//スパークの作成
			CreateSpark();
			//炎の作成
			CreateFire();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
