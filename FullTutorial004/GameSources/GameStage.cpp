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
		//サンプルのためアセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(DataDir);
		//各ゲームは以下のようにデータディレクトリを取得すべき
		//App::GetApp()->GetDataDirectory(DataDir);

		//プレート用
		wstring strTexture = DataDir + L"gray.png";
		App::GetApp()->RegisterTexture(L"GRAY_TX", strTexture);
		strTexture = DataDir + L"normal3.png";
		App::GetApp()->RegisterTexture(L"NORMAL3_TX", strTexture);
		//プレイヤー用
		strTexture = DataDir + L"Brown.png";
		App::GetApp()->RegisterTexture(L"BROWN_TX", strTexture);
		strTexture = DataDir + L"normal1.png";
		App::GetApp()->RegisterTexture(L"NORMAL1_TX", strTexture);
		//Seekオブジェクト用
		strTexture = DataDir + L"Green.png";
		App::GetApp()->RegisterTexture(L"GREEN_TX", strTexture);
		strTexture = DataDir + L"normal2.png";
		App::GetApp()->RegisterTexture(L"NORMAL2_TX", strTexture);
		//Fixed用
		strTexture = DataDir + L"Blue.png";
		App::GetApp()->RegisterTexture(L"BLUE_TX", strTexture);
		//L"NORMAL2_TX"
		strTexture = DataDir + L"Wall2.png";
		App::GetApp()->RegisterTexture(L"WALL2_TX", strTexture);
		strTexture = DataDir + L"Wall2_normal.png";
		App::GetApp()->RegisterTexture(L"WALL2_NORMAL_TX", strTexture);
		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		strTexture = DataDir + L"wall_normal.png";
		App::GetApp()->RegisterTexture(L"WALL_NORMAL_TX", strTexture);
		//L"BROWN_TX"
		strTexture = DataDir + L"Floor_normal.png";
		App::GetApp()->RegisterTexture(L"FLOOR_NORMAL_TX", strTexture);
		//MoveBox用
		strTexture = DataDir + L"Red.png";
		App::GetApp()->RegisterTexture(L"RED_TX", strTexture);
		//FLOOR_NORMAL_TX
		//UnevenGround用
		strTexture = DataDir + L"Brown2.png";
		App::GetApp()->RegisterTexture(L"BROWN2_TX", strTexture);
		//WALL2_NORMAL_TX

		//その他
		strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"Floor.png";
		App::GetApp()->RegisterTexture(L"FLOOR_TX", strTexture);


		
	}



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

	//プレートのメッシュ作成
	void GameStage::CreatePlateMesh() {
		//プレート用のメッシュを作成
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionNormalTangentTexture> new_pntnt_vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSquare(1.0f, vertices, indices);
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionNormalTangentTexture new_pntnt_v;
			new_pntnt_v.position = vertices[i].position;
			new_pntnt_v.normal = vertices[i].normal;
			if (vertices[i].textureCoordinate.x == 1.0f) {
				new_pntnt_v.textureCoordinate.x = 20.0f;
			}
			if (vertices[i].textureCoordinate.y == 1.0f) {
				new_pntnt_v.textureCoordinate.y = 20.0f;
			}
			Vector3 n = Vector3EX::Cross(new_pntnt_v.normal, Vector3(0, 1, 0));
			new_pntnt_v.tangent = n;
			new_pntnt_v.tangent.w = 0.0f;
			new_pntnt_vertices.push_back(new_pntnt_v);
		}
		MeshUtill::SetNormalTangent(new_pntnt_vertices);
		App::GetApp()->RegisterResource(L"MY_PNTnT_SQUARE", MeshResource::CreateMeshResource(new_pntnt_vertices, indices, false));
	}

	//プレートの作成
	void GameStage::CreatePlate() {
		//メッシュ作成
		CreatePlateMesh();
		//ステージへのゲームオブジェクトの追加
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quaternion Qt(Vector3(1.0f,0,0), XM_PIDIV2);
		Qt.RotationRollPitchYawFromVector(Vector3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(50.0f, 50.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);
		auto ColPtr = Ptr->AddComponent<CollisionRect>();
		//描画コンポーネント
		auto DrawComp = Ptr->AddComponent<BcPNTnTStaticDraw>();
		DrawComp->SetMeshResource(L"MY_PNTnT_SQUARE");
		DrawComp->SetTextureResource(L"GRAY_TX");
		DrawComp->SetNormalMapTextureResource(L"NORMAL3_TX");
		//ラップモード
		DrawComp->SetSamplerState(SamplerState::LinearWrap);
		//スペキュラーなし
		DrawComp->DisableSpecular();
		//フォグはきつめに
		DrawComp->SetFogEnabled(true);
		DrawComp->SetFogColor(Color4(0.3f, 0.3f,0.3f, 1.0f));
		DrawComp->SetFogStart(-10.0f);
		DrawComp->SetFogEnd(-30.0f);
		//自分に影が映りこむようにする
		DrawComp->SetOwnShadowActive(true);
	}

	//追いかけるオブジェクトの作成
	void GameStage::CreateSeekObject() {
		//オブジェクトのグループを作成する
		auto Group = CreateSharedObjectGroup(L"ObjectGroup");
		//配列の初期化
		vector<Vector3> Vec = {
			{ 0, 0.25f, 10.0f },
			{ 10.0f, 0.25f, 0.0f },
			{ -10.0f, 0.25f, 0.0f },
			{ 0, 0.25f, -10.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<SeekObject>(v);
		}
	}


	struct FixedNormalBoxParam {
		wstring TexKey;
		wstring NormalTexKey;
		Vector3 Scale;
		Vector3 Rot;
		Vector3 Pos;
		FixedNormalBoxParam(const wstring& tk, const wstring& ntk, const Vector3& s, const Vector3& r, const Vector3& p) :
			TexKey(tk), NormalTexKey(ntk), Scale(s), Rot(r), Pos(p) {}
	};

	//固定の法線マップ処理ボックスの作成
	void GameStage::CreateFixedNormalBox() {
		//配列の初期化
		vector<FixedNormalBoxParam> Vec = {
			FixedNormalBoxParam(
				L"BLUE_TX",
				L"NORMAL2_TX",
				Vector3(5.0f, 0.5f, 5.0f),
				Vector3(0.0f, 0.0f, 0.0f),
				Vector3(10.0f, 0.25f, 10.0f)
			),
			FixedNormalBoxParam(
				L"WALL2_TX",
				L"WALL2_NORMAL_TX",
				Vector3(2.0f, 1.0f, 2.0f),
				Vector3(0, 0, 0),
				Vector3(10.0f, 0.5f, 10.0f)
			),
			FixedNormalBoxParam(
				L"WALL_TX",
				L"WALL_NORMAL_TX",
				Vector3(4.0f, 1.0f, 4.0f),
				Vector3(0, 0, 0),
				Vector3(-10.0f, 0.5f, 10.0f)
			),
			FixedNormalBoxParam(
				L"BROWN_TX",
				L"FLOOR_NORMAL_TX",
				Vector3(10.0f, 0.5f, 10.0f),
				Vector3(-0.5f, 0.0f, -0.5f),
				Vector3(-10.0f, 0.25f, 10.0f)
			)
		};
		//オブジェクトの作成
		for (auto& v : Vec) {
			AddGameObject<FixedNormalBox>(v.TexKey,v.NormalTexKey,v.Scale,v.Rot,v.Pos);
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
			Vector3(5.0f, 0.5f, 5.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, -0.5f, 20.0f)
			);
	}

	//ヒットする球体の作成
	void GameStage::CreateSphere() {
		//配列の初期化
		vector<Vector3> Vec = {
			{ 20.0f, 0, 15.0f },
			{ 20.0f, 0, 0.0f },
		};
		//配置オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<SphereObject>(v);
		}
	}

	//でこぼこ床の作成
	void GameStage::CreateUnevenGround() {
		//でこぼこ床のデータの作成
		AddGameObject<UnevenGroundData>();
		//配列の初期化
		vector< vector<Vector3> > Vec = {
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 5.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 5.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 7.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 7.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 9.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 9.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 11.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 11.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 13.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 13.0f)
			},

			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, XM_PIDIV2, 0.0f),
				Vector3(-4.0f, 0.0f, 15.0f)
			},
			{
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, -XM_PIDIV2, 0.0f),
				Vector3(-3.0f, 0.0f, 15.0f)
			},
		};
		//オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<UnevenGround>(v[0], v[1], v[2]);
		}

	}

	void GameStage::OnCreate() {
		try {
			//リソースの作成
			CreateResourses();
			//ビューとライトの作成
			CreateViewLight();
			//プレートの作成
			CreatePlate();
			//固定の法線マップ処理ボックスの作成
			CreateFixedNormalBox();
			//上下移動しているボックスの作成
			CreateMoveBox();
			//球体作成
			CreateSphere();
			//でこぼこ床の作成
			CreateUnevenGround();
			//プレーヤーの作成
			CreatePlayer();
			//追いかけるオブジェクトの作成
			CreateSeekObject();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
