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
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
	}

	//ビューとライトの作成
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PtrCamera = PtrView->GetCamera();
		PtrCamera->SetEye(Vector3(0, 2.0, -5.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 0.0f));
		//シングルライトの作成
		auto PtrSingleLight = CreateLight<SingleLight>();
		//ライトの設定
		PtrSingleLight->GetLight().SetPositionToDirectional(-0.25f, 1.0f, -0.25f);
	}



	//ボックスの作成
	void GameStage::CreateNormalTextureBox() {
		AddGameObjectWithParam<NormalTextureBox>(Vector3(0.0f, 0.0f, .0f));
	}


	void GameStage::OnCreate() {
		try {
			//リソースの作成
			CreateResourses();
			//ビューとライトの作成
			CreateViewLight();
			//ボックスの作成
			CreateNormalTextureBox();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
