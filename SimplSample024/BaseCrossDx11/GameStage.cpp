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

	void GameStage::RegisterNormalBox() {
		if (!App::GetApp()->CheckResource<MeshResource>(L"NORMAL_BOX")) {
			vector<VertexPositionNormalTexture> vertices;
			vector<VertexPositionNormalTangentTexture> new_vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateCube(1.0f, vertices, indices);
			BcRenderer::ConvertToNormalVertex(vertices, new_vertices);
			App::GetApp()->RegisterResource(L"NORMAL_BOX",
				MeshResource::CreateMeshResource(new_vertices, indices, false));
		}
	}

	void GameStage::OnCreate() {

		//シャドウマップの描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->GetShadowMapRenderTarget(2048.0f);

		//複数使用する法線付きボックスの登録（リソース登録する）
		RegisterNormalBox();

		//平面の作成
		Quat Qt;
		Qt.rotationX(XM_PIDIV2);
		AddGameObject<SquareObject>(
			L"WALL_TX",
			L"WALL_NORMAL_TX",
			Vec3(50.0f, 50.0f, 1.0f),
			Qt,
			Vec3(0.0f, 0.0f, 0.0f)
			);

		//Static描画スタティックな3D平面(Faceing)
		AddGameObject<SimpleSquare>(
			L"SKY_TX",
			Vec3(4.0f, 4.0f, 1.0f),
			Vec3(-15.0f, 2.0f, 18.0f),
			SquareDrawOption::Faceing
			);

		//Static描画スタティックな3D平面(Billboard)
		AddGameObject<SimpleSquare>(
			L"SKY_TX",
			Vec3(4.0f, 4.0f, 1.0f),
			Vec3(-5.0f, 2.0f, 18.0f),
			SquareDrawOption::Billboard
			);

		//Static描画スタティックな3D平面(FaceingY)
		AddGameObject<SimpleSquare>(
			L"SKY_TX",
			Vec3(4.0f, 4.0f, 1.0f),
			Vec3(5.0f, 2.0f, 18.0f),
			SquareDrawOption::FaceingY
			);

		//Static描画スタティックな3D平面(何もしない。正規化のみ)
		AddGameObject<SimpleSquare>(
			L"SKY_TX",
			Vec3(4.0f, 4.0f, 1.0f),
			Vec3(15.0f, 2.0f, 18.0f),
			SquareDrawOption::Normal
			);




		//Basic描画スタティックなボックス
		AddGameObject<BcBoxObject>(
			L"BROWN_TX",
			L"NORMAL2_TX",
			Vec3(5.0f, 0.5f, 5.0f),
			Quat(),
			Vec3(5.0f, 0.25f, 12.0f)
			);

		//Basic描画スタティックなボックス
		AddGameObject<BcBoxObject>(
			L"BROWN_TX",
			L"NORMAL2_TX",
			Vec3(3.0f, 1.0f, 3.0f),
			Quat(),
			Vec3(5.0f, 0.5f, 12.0f)
			);

		//Basic描画スタティックなボックス
		AddGameObject<BcBoxObject>(
			L"BROWN_TX",
			L"NORMAL2_TX",
			Vec3(3.0f, 1.0f, 3.0f),
			Quat(),
			Vec3(-3.5f, 0.5f, 12.0f)
			);


		//Basic描画スタティックなボックス
		AddGameObject<BcBoxObject>(
			L"BROWN_TX",
			L"NORMAL2_TX",
			Vec3(5.0f, 0.5f, 5.0f),
			Quat(Vec3(0, 0, 1), -XM_PIDIV4),
			Vec3(-5.0f, 1.0f, 12.0f)
			);


		//Basic描画法線マップつきBoneキャラ(自己影なし)
		AddGameObject<BcBoneNormalChara>(
			Vec3(-1.0f, 0.5f, 2.5f),
			false);

		//Basic描画法線マップつきBoneキャラ(自己影あり)
		AddGameObject<BcBoneNormalChara>(
			Vec3(-3.0f, 0.5f, 2.5f),
			true);

		//Basic描画Boneキャラ(自己影なし)
		AddGameObject<BcBoneChara>(
			Vec3(-5.0f, 0.5f, 2.5f),
			false);

		//Basic描画Boneキャラ(自己影なし)
		AddGameObject<BcBoneChara>(
			Vec3(-7.0f, 0.5f, 2.5f),
			true);



		//Simple描画Boneキャラ(自己影なし)
		AddGameObject<BoneChara>(
			Vec3(1.0f, 0.5f, 2.5f),
			false);

		//Simple描画Boneキャラ(自己影あり)
		AddGameObject<BoneChara>(
			Vec3(3.0f, 0.5f, 2.5f),
			false);



		//スタティックなキャラ(自己影なし)
		AddGameObject<StaticChara>(
			Vec3(1.0f, 0.5f, 5.0f),
			false);
		//スタティックなキャラ(自己影あり)
		AddGameObject<StaticChara>(
			Vec3(3.0f, 0.5f, 5.0f),
			true);

		//Simple描画スタティックな球体(自己影なし)
		AddGameObject<SimpleSphereObject>(
			L"SKY_TX",
			Vec3(1.0f, 1.0f, 1.0f),
			Quat(),
			Vec3(1.0f, 0.5f, 8.0f),
			false);

		//Simple描画スタティックな球体(自己影あり)
		AddGameObject<SimpleSphereObject>(
			L"SKY_TX",
			Vec3(1.0f, 1.0f, 1.0f),
			Quat(),
			Vec3(3.0f, 0.5f, 8.0f),
			true);

		//Basic描画スタティックな球体(自己影なし)
		AddGameObject<BcSphereObject>(
			L"SKY_TX",
			Vec3(1.0f, 1.0f, 1.0f),
			Quat(),
			Vec3(-1.0f, 0.5f, 8.0f));

		//Basic描画法線マップ付きスタティックなキャラ(自己影なし)
		AddGameObject<BcStaticNormalChara>(
			Vec3(-1.0f, 0.5f, 5.0f),
			false);

		//Basic描画法線マップ付きスタティックなキャラ(自己影あり)
		AddGameObject<BcStaticNormalChara>(
			Vec3(-3.0f, 0.5f, 5.0f),
			true);

		//Basic描画スタティックなキャラ(自己影なし)
		AddGameObject<BcStaticChara>(
			Vec3(-5.0f, 0.5f, 5.0f),
			false);

		//Basic描画スタティックなキャラ(自己影あり)
		AddGameObject<BcStaticChara>(
			Vec3(-7.0f, 0.5f, 5.0f),
			true);


		//プレイヤーの作成
		AddGameObject<Player>(
			L"TRACE_TX", 
			true, 
			Vec3(0.0f, 0.125f, 0.0f)
			);

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

			if (CntlVec[0].fThumbRX != 0) {
				camera.m_CameraXZRad += CntlVec[0].fThumbRX * 0.02f;
				if (abs(camera.m_CameraXZRad) >= XM_2PI) {
					camera.m_CameraXZRad = 0;
				}
			}
			if (CntlVec[0].fThumbRY != 0) {
				camera.m_CameraYRad -= CntlVec[0].fThumbRY * 0.02f;
				if (camera.m_CameraYRad >= XM_PIDIV2 - 0.1f) {
					camera.m_CameraYRad = XM_PIDIV2 - 0.1f;
				}
				else if (camera.m_CameraYRad <= 0.2) {
					camera.m_CameraYRad = 0.2;
				}
			}

			camera.m_CamerAt = FindTagGameObject<GameObject>(L"Player")->GetPosition();
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
