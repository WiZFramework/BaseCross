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
		auto PtrCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(Vec3(0.0f, 2.0f, -3.0f));
		PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
		//シャドウマップのライトの高さを調整(ステージが広いため)
		Shadowmap::SetLightHeight(50.0f);
	}

	//物理計算オブジェクトの作成
	void GameStage::CreatePhysicsObjects() {
		//下の台
		AddGameObject<FixedPsBox>(Vec3(50.0f, 1.0f, 50.0f), Quat(), Vec3(0.0f, -0.5f, 0.0f));

		Quat Qt1, Qt2, Qt3, Qt4;
		Qt1.rotationZ(2.0f);
		Qt2.rotationX(0.7f);
		Qt3.rotationZ(-2.0f);
		Qt4.rotationX(-0.7f);
		//上から降ってくるボックス
		AddGameObject<ActivePsBox>(Vec3(2.0f, 1.0f, 1.0f), Quat(), Vec3(2.0f, 1.0f, 4.0f));
		AddGameObject<ActivePsBox>(Vec3(1.0f, 1.0f, 2.0f), Qt1, Vec3(2.5f, 12.0f, 9.0f));
		AddGameObject<ActivePsBox>(Vec3(1.0f, 1.0f, 1.0f), Qt2, Vec3(-3.0f, 13.0f, 2.0f));
		AddGameObject<ActivePsBox>(Vec3(2.0f, 1.0f, 1.0f), Qt4, Vec3(2.5f, 15.0f, 10.0f));
		//カプセル
		AddGameObject<ActivePsCapsule>(1.0f, 1.0f, Qt2, Vec3(-2.0f, 4.0f, 3.5f));
		AddGameObject<ActivePsCapsule>(1.0f, 1.0f, Qt4, Vec3(2.0f, 10.0f, 2.0f));

		//上から降ってくる球体
		AddGameObject<ActivePsSphere>(1.0f, Quat(), Vec3(-0.5f, 7.0f, 7.0f));
		AddGameObject<ActivePsSphere>(1.0f, Quat(), Vec3(0.0f, 8.0f, 4.5f));

	}


	//ボックスの作成
	void GameStage::CreateBox() {
		AddGameObject<Box>(Vec3(0.0f, 0.25f, 15.0f));
		AddGameObject<Box>(Vec3(-5.0f, 0.25f, 15.0f));
		AddGameObject<Box>(Vec3(5.0f, 0.25f, 15.0f));
		AddGameObject<Box>(Vec3(10.0f, 0.25f, 10.0f));
		AddGameObject<Box>(Vec3(-10.0f, 0.25f, 10.0f));
	}

	//スパークの作成
	void GameStage::CreateSpark() {
		auto MultiSparkPtr = AddGameObject<MultiSpark>();
		//共有オブジェクトにスパークを登録
		SetSharedGameObject(L"MultiSpark", MultiSparkPtr);
	}


	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>(Vec3(0,0.125f,0));
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
		//砲弾グループの作成
		CreateSharedObjectGroup(L"ShellGroup");

	}

	//BGMの再生
	void GameStage::PlayBGM() {
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		m_BGM = XAPtr->Start(L"Nanika", XAUDIO2_LOOP_INFINITE, 0.1f);
	}

	void GameStage::OnCreate() {
		try {
			//物理計算有効
			SetPhysicsActive(true);
			//ビューとライトの作成
			CreateViewLight();
			//物理計算オブジェクトの作成
			CreatePhysicsObjects();
			//ボックスの作成
			CreateBox();
			//スパークの作成
			CreateSpark();
			//プレーヤーの作成
			CreatePlayer();
			//BGMの再生
			PlayBGM();
		}
		catch (...) {
			throw;
		}
	}

	//更新
	void GameStage::OnUpdate() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Bボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
			}
		}
	}
	void GameStage::OnDestroy() {
		//BGMのストップ
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Stop(m_BGM);
	}

	//--------------------------------------------------------------------------------------
	//	タイトルステージクラス
	//--------------------------------------------------------------------------------------
	void TitleStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(Vec3(0.0f, 2.0f, -3.0f));
		PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();

	}

	//スプライトの作成
	void TitleStage::CreateTitleSprite() {
		AddGameObject<TitleSprite>(L"MESSAGE_TX", false,
			Vec2(256.0f, 64.0f), Vec2(0.0f, 100.0f));

	}

	//初期化
	void TitleStage::OnCreate() {
		CreateViewLight();
		//スプライトの作成
		CreateTitleSprite();
	}

	//更新
	void TitleStage::OnUpdate() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Bボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
			}
		}

	}



}
//end basecross
