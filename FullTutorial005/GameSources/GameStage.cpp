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
			new_pntnt_v.textureCoordinate = vertices[i].textureCoordinate;
			if (vertices[i].textureCoordinate.x == 1.0f) {
				new_pntnt_v.textureCoordinate.x = 20.0f;
			}
			if (vertices[i].textureCoordinate.y == 1.0f) {
				new_pntnt_v.textureCoordinate.y = 20.0f;
			}
			Vec3 n = cross((Vec3)new_pntnt_v.normal, Vec3(0, 1, 0));
			new_pntnt_v.tangent = Vec4(n,0.0f);
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
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(XM_PIDIV2, 0, 0));
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
		DrawComp->SetFogColor(Col4(0.3f, 0.3f,0.3f, 1.0f));
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
		vector<Vec3> Vec = {
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
		Vec3 Scale;
		Vec3 Rot;
		Vec3 Pos;
		FixedNormalBoxParam(const wstring& tk, const wstring& ntk, const Vec3& s, const Vec3& r, const Vec3& p) :
			TexKey(tk), NormalTexKey(ntk), Scale(s), Rot(r), Pos(p) {}
	};

	//固定の法線マップ処理ボックスの作成
	void GameStage::CreateFixedNormalBox() {
		//配列の初期化
		vector<FixedNormalBoxParam> Vec = {
			FixedNormalBoxParam(
				L"BLUE_TX",
				L"NORMAL2_TX",
				Vec3(5.0f, 0.5f, 5.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(10.0f, 0.25f, 10.0f)
			),
			FixedNormalBoxParam(
				L"WALL2_TX",
				L"WALL2_NORMAL_TX",
				Vec3(2.0f, 1.0f, 2.0f),
				Vec3(0, 0, 0),
				Vec3(10.0f, 0.5f, 10.0f)
			),
			FixedNormalBoxParam(
				L"WALL_TX",
				L"WALL_NORMAL_TX",
				Vec3(4.0f, 1.0f, 4.0f),
				Vec3(0, 0, 0),
				Vec3(-10.0f, 0.5f, 10.0f)
			),
			FixedNormalBoxParam(
				L"BROWN_TX",
				L"FLOOR_NORMAL_TX",
				Vec3(10.0f, 0.5f, 10.0f),
				Vec3(-0.5f, 0.0f, -0.5f),
				Vec3(-10.0f, 0.25f, 10.0f)
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

	void GameStage::OnCreate() {
		try {
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
			//追いかけるオブジェクトの作成
			CreateSeekObject();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
