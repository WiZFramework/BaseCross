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


		auto PtrView = CreateView<MultiView>();
		//メインビューのビューポートとカメラの設定
		auto PtrLookAtCamera = ObjectFactory::Create<LookAtCamera>();
		PtrLookAtCamera->SetEye(Vector3(0.0f, 5.0f, -5.0f));
		PtrLookAtCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
		Viewport MainViewport;
		float Height = 200;
		MainViewport.Width = static_cast<float>(App::GetApp()->GetGameWidth());
		MainViewport.Height = static_cast<float>(App::GetApp()->GetGameHeight()) - Height;
		MainViewport.TopLeftX = 0;
		MainViewport.TopLeftY = 0;
		MainViewport.MinDepth = 0.5f;
		MainViewport.MaxDepth = 1.0f;
		//比率が変わるのでカメラにビューポートを伝える
		PtrLookAtCamera->SetViewPort(MainViewport);
		PtrView->AddView(MainViewport, PtrLookAtCamera);
		
		//サブ(固定カメラ)
		auto PtrSubCamera = ObjectFactory::Create<Camera>();
		PtrSubCamera->SetEye(Vector3(0.0f, 5.0f,-5.0f));
		PtrSubCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));

		Viewport SubViewport;
		SubViewport.Width = static_cast<float>(320.0f);
		SubViewport.Height = static_cast<float>(256.0f);
		SubViewport.TopLeftX = 32;
		SubViewport.TopLeftY = 32;
		SubViewport.MinDepth = 0.0f;
		SubViewport.MaxDepth = 0.5f;
		//比率が変わるのでカメラにビューポートを伝える
		PtrSubCamera->SetViewPort(SubViewport);
		PtrView->AddView(SubViewport, PtrSubCamera);
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
			//子供ステージの作成
			AddChileStage<ChildStage>();

		}
		catch (...) {
			throw;
		}
	}


	//--------------------------------------------------------------------------------------
	//	チャイルドステージクラス
	//--------------------------------------------------------------------------------------
	void ChildStage::OnCreate() {
		try {
			auto PtrView = CreateView<SingleView>();

			Viewport ChileViewport;
			float Height = 200;
			ChileViewport.Width = static_cast<float>(App::GetApp()->GetGameWidth());
			ChileViewport.Height = Height;
			ChileViewport.TopLeftX = 0;
			ChileViewport.TopLeftY = static_cast<float>(App::GetApp()->GetGameHeight()) - Height;
			ChileViewport.MinDepth = 0.0f;
			ChileViewport.MaxDepth = 1.0f;
			PtrView->SetViewport(ChileViewport);

			auto PtrStr = AddGameObject<StringGameObject>();
			wstring ClearStr = L"チャイルドステージ";
			PtrStr->SetText(ClearStr);
		}
		catch (...) {
			throw;
		}
	}

	void ChildStage::OnUpdate() {
	}


}
//end basecross
