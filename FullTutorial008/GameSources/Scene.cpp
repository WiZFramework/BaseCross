
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------

	void Scene::OnCreate(){
		try {
			//最初のアクティブステージの設定
			ResetActiveStage<GameStage>();
		}
		catch (...) {
			throw;
		}
	}



}
//end basecross
