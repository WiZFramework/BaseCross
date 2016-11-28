/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	メニューステージクラス実体
	//--------------------------------------------------------------------------------------
	//リソースの作成
	void MenuStage::CreateResourses() {
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		wstring CursorWav = App::GetApp()->m_wstrRelativeDataPath + L"cursor.wav";
		App::GetApp()->RegisterWav(L"Cursor", CursorWav);
	}

	//ビューとライトの作成
	void MenuStage::CreateViewLight() {
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

	//壁模様のスプライト作成
	void MenuStage::CreateWallSprite() {
		float w = static_cast<float>(App::GetApp()->GetGameWidth()) / 2.0f;
		float h = static_cast<float>(App::GetApp()->GetGameHeight()) / 2.0f;

		AddGameObject<WallSprite>(L"WALL_TX", false,
			Vector2(w, h), Vector2(0,0));
	}


	//スクロールするスプライト作成
	void MenuStage::CreateScrollSprite() {
		AddGameObject<ScrollSprite>(L"TRACE_TX", true,
			Vector2(512.0f, 128.0f), Vector2(0,0));
	}


	void MenuStage::OnCreate() {
		try {
			//リソースの作成
			CreateResourses();
			//ビューとライトの作成
			CreateViewLight();
			//壁模様のスプライト作成
			CreateWallSprite();
			//スクロールするスプライト作成
			CreateScrollSprite();
		}
		catch (...) {
			throw;
		}
	}

	//更新
	void MenuStage::OnUpdate() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Bボタンが押された瞬間ならステージ推移
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				auto ScenePtr = App::GetApp()->GetScene<Scene>();
				PostEvent(0.0f, GetThis<ObjectInterface>(), ScenePtr,L"ToGameStage");
			}
		}
	}





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


	//プレイヤーの作成
	void GameStage::CreatePlayer() {
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
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
