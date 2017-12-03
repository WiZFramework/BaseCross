/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ゲームステージ
	//--------------------------------------------------------------------------------------
	GameStage::GameStage()
	{
	}

	void GameStage::RegisterPctlBox() {
		if (!App::GetApp()->CheckResource<MeshResource>(L"DEFAULT_PCT_BOX")) {
			vector<VertexPositionNormalTexture> vertices;
			vector<VertexPositionColorTexture> new_vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateCube(1.0f, vertices, indices);
			for (auto&v : vertices) {
				VertexPositionColorTexture vertex;
				vertex.position = v.position;
				vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				vertex.textureCoordinate = v.textureCoordinate;
				if (vertex.textureCoordinate.x == 1.0f) {
					vertex.textureCoordinate.x = 50.0f;
				}
				new_vertices.push_back(vertex);
			}
			App::GetApp()->RegisterResource(L"DEFAULT_PCT_BOX",
				MeshResource::CreateMeshResource(new_vertices, indices, false));
		}
	}

	void GameStage::OnCreate() {

		//シャドウマップの描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->GetShadowMapRenderTarget(2048.0f);

		//PCTボックスの登録（リソース登録する）
		RegisterPctlBox();

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				float x = -15.0f + 10.0f * (float)i + (float)j * 2.0f;
				float z = 1.0f - (float)j * 0.1f;
				//Static描画スタティックな3D平面(何もしない。正規化のみ)
				AddGameObject<SimpleSquare>(
					L"SKY_TX",
					Vec3(1.0f, 0.5f, 2.0f),
					Vec3(x, 0.25f, z),
					SquareDrawOption::Normal
					);
			}
		}

		AddGameObject<SimpleBox>(
			L"SKY_TX",
			Vec3(50.0f, 1.0f, 50.0f),
			Vec3(0, -0.5f, 0)
			);


		//プレイヤーの作成
		AddGameObject<Player>(
			L"TRACE_TX", 
			true, 
			Vec3(0.0f, 0.125f, 0.0f)
			);

		wstring DataDir;
		//サンプルのためアセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(DataDir);
		//Chaera1の作成
		AddGameObject<Chara1>(DataDir + L"character_template_sample1\\");

		//スパークエフェクト
		AddGameObject<MultiSpark>();
		//ファイアエフェクト
		AddGameObject<MultiFire>();

		//描画オブジェクトの追加
		CreateDrawObjects();


		//回転するスプライトの作成
		AddGameObject<RotateSprite>(
			L"TRACE_TX",
			Vec2(160, 160),
			0.0f,
			Vec2(-480, 260),
			4, 4
			);


		//メッセージを表示するスプライトの作成
		AddGameObject<MessageSprite>(
			L"MESSAGE_TX",
			Vec2(256, 64),
			0.0f,
			Vec2(480, 260),
			1, 1
			);

		//文字列描画オブジェクトの作成
		AddGameObject<StringDrawObject>();


	}

	//描画オブジェクトの追加
	void GameStage::CreateDrawObjects() {
		//シャドウマップ描画オブジェクトの作成
		AddGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");

		//SimplePCTStaticRenderer描画オブジェクトの作成
		AddGameObject<SimplePCTStaticRenderer>(L"SimplePCTStaticRenderer");

		//SimplePNTStaticRenderer描画オブジェクトの作成
		AddGameObject<SimplePNTStaticRenderer>(L"SimplePNTStaticRenderer");
		//SimplePNTStaticRenderer2描画オブジェクトの作成
		AddGameObject<SimplePNTStaticRenderer2>(L"SimplePNTStaticRenderer2");

		//SimplePNTStaticModelRenderer描画オブジェクトの作成
		AddGameObject<SimplePNTStaticModelRenderer>(L"SimplePNTStaticModelRenderer");
		//SimplePNTStaticModelRenderer2描画オブジェクトの作成
		AddGameObject<SimplePNTStaticModelRenderer2>(L"SimplePNTStaticModelRenderer2");

		//SimplePNTBoneModelRenderer描画オブジェクトの作成
		AddGameObject<SimplePNTBoneModelRenderer>(L"SimplePNTBoneModelRenderer");
		//SimplePNTBoneModelRenderer2描画オブジェクトの作成
		AddGameObject<SimplePNTBoneModelRenderer2>(L"SimplePNTBoneModelRenderer2");


		//BcPNTStaticDrawObject描画オブジェクトの作成
		AddGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
		//BcPNTnTStaticDrawObject描画オブジェクトの作成
		AddGameObject<BcPNTnTStaticRenderer>(L"BcPNTnTStaticRenderer");

		//BcPNTStaticModelDrawObject描画オブジェクトの作成
		AddGameObject<BcPNTStaticModelRenderer>(L"BcPNTStaticModelRenderer");
		//BcPNTnTStaticModelDrawObject描画オブジェクトの作成
		AddGameObject<BcPNTnTStaticModelRenderer>(L"BcPNTnTStaticModelRenderer");

		//BcPNTBoneModelDrawObject描画オブジェクトの作成
		AddGameObject<BcPNTBoneModelRenderer>(L"BcPNTBoneModelRenderer");
		//BcPNTnTBoneModelDrawObject描画オブジェクトの作成
		AddGameObject<BcPNTnTBoneModelRenderer>(L"BcPNTnTBoneModelRenderer");

		//Particle描画オブジェクトの作成(加算処理しない)
		AddGameObject<ParticleDrawObject>(false);
		//Particle描画オブジェクトの作成(加算処理する)
		AddGameObject<ParticleDrawObject>(true);

	}


	void GameStage::OnUpdateStage() {
		//ターン毎の初期化
		GetRigidbodyManager()->InitRigidbody();
		for (auto& v : GetGameObjectVec()) {
			//各オブジェクトの更新
			v->OnUpdate();
		}
		//Rigidbodyマネージャの更新（衝突判定など）
		GetRigidbodyManager()->OnUpdate();
		for (auto& v : GetGameObjectVec()) {
			//各オブジェクトの最終更新
			v->OnUpdate2();
		}
		//自分自身の更新(カメラ)
		this->OnUpdate();
		//Rigidbodyマネージャの最終更新（衝突判定情報のクリア）
		GetRigidbodyManager()->OnUpdate2();
	}


	void GameStage::OnUpdate() {
		auto& camera = GetCamera();
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {

			//Dパッド下
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				//カメラ位置を引く
				camera.m_CameraArmLen += 0.1f;
				if (GetCamera().m_CameraArmLen >= 50.0f) {
					GetCamera().m_CameraArmLen = 50.0f;
				}
			}
			//Dパッド上
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				//カメラ位置を寄る
				camera.m_CameraArmLen -= 0.1f;
				if (GetCamera().m_CameraArmLen <= 2.0f) {
					camera.m_CameraArmLen = 2.0f;
				}
			}

			camera.m_CamerAt = FindTagGameObject<GameObject>(L"Player")->GetPosition();
			camera.m_CamerAt.y += 0.25f;
			Vec3 CameraLocalEye =
				Vec3(
					sin(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad),
					cos(camera.m_CameraYRad) * camera.m_CameraArmLen,
					-cos(camera.m_CameraXZRad) * camera.m_CameraArmLen * sin(camera.m_CameraYRad)
				);
			camera.m_CamerEye = camera.m_CamerAt + CameraLocalEye;
			//Bボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToEmptyStage");
			}
		}

		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\nElapsedTime: ";
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		FPS += Util::FloatToWStr(ElapsedTime);
		FPS += L"\n";
		if (!m_StringDrawObject) {
			m_StringDrawObject = FindTagGameObject<StringDrawObject>(L"StringDrawObject");
		}
		m_StringDrawObject->SetText(FPS);
	}

	void GameStage::OnDrawStage() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));

		//シャドオウマップ描画の開始
		Dev->ClearShadowmapViews();
		Dev->StartShadowmapDraw();
		for (auto& v : GetGameObjectVec()) {
			//各オブジェクトの描画
			v->OnDrawShadowmap();
		}
		Dev->EndShadowmapDraw();

		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		for (auto& v : GetGameObjectVec()) {
			//各オブジェクトの描画
			v->OnDraw();
		}
		//自分自身の描画
		this->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}


	void GameStage::OnDraw() {
		m_RigidbodyManager->OnDraw();
	}

	//--------------------------------------------------------------------------------------
	///	空のステージ（メッセージのみある）
	//--------------------------------------------------------------------------------------
	void EmptyStage::OnCreate() {
		//メッセージスプライト
		m_MessageSprite = ObjectFactory::Create<MessageSprite>(
			GetThis<Stage>(),
			L"MESSAGE_TX",
			Vec2(256, 64),
			0.0f,
			Vec2(0, 0),
			1, 1);
	}
	void EmptyStage::OnUpdateStage() {
		//スプライトの更新
		m_MessageSprite->OnUpdate();
		//自分自身の更新
		this->OnUpdate();
	}
	void EmptyStage::OnUpdate() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Bボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
			}
		}
	}

	void EmptyStage::OnDrawStage() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		//スプライト描画
		m_MessageSprite->OnDraw();
		//自分自身の描画
		this->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}


	void EmptyStage::OnDraw() {
		//何もしない
	}


}
//end basecross
