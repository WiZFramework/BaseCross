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
	}

	void GameStage::OnCreate() {
		try {
			//リソースの作成
			CreateResourses();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
