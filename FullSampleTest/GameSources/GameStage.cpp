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

	//ボックスの作成
	void GameStage::CreateNormalTextureBox() {
		AddGameObjectWithParam<NormalTextureBox>(Vector3(0.0f, 0.0f, .0f));
	}


	void GameStage::OnCreate() {
		try {
			//リソースの作成
			CreateResourses();
			//ボックスの作成
			CreateNormalTextureBox();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
