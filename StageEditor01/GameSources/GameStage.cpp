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
		PtrLookAtCamera->SetMaxArm(100.0f);
		//シングルライトの作成
		auto PtrSingleLight = CreateLight<SingleLight>();
		//ライトの設定
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}

	//ラインの作成
	void GameStage::CreateActionLine() {
		//X軸
		AddGameObject<ActionLine>(Vec3(-100.0f,0,0), Vec3(100.0f, 0.0f, 0.0f),Col4(0.0f,1.0f,0.0f,1.0f));
		//Y軸
		AddGameObject<ActionLine>(Vec3(0.0f, -100.0f, 0), Vec3(0.0f, 100.0f, 0.0f), Col4(1.0f, 0.0f, 0.0f, 1.0f));
		//Z軸
		AddGameObject<ActionLine>(Vec3(0.0f, 0.0f, -100.0f), Vec3(0.0f, 0.0f, 100.0f), Col4(0.0f, 0.0f, 1.0f, 1.0f));
	}

	//タイリングプレートの作成
	void GameStage::CreateTilingPlate() {
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(XM_PIDIV2, 0, 0));
		Vec3 Scale(200.0f, 200.0f, 1.0f);
		Vec3 Pos(0.0f, 0.0f, 0.0f);
		AddGameObject<TilingPlate>(Scale,Qt,Pos,1.0f,1.0f);
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
			//ラインの作成
			CreateActionLine();
			//タイリングプレートの作成
			CreateTilingPlate();
			//プレーヤーの作成
			CreatePlayer();
		}
		catch (...) {
			throw;
		}
	}


}
//end basecross
