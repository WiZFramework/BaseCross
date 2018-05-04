
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------

	void Scene::CreateResourses() {
		wstring DataDir;
		//各ゲームは以下のようにデータディレクトリを取得すべき
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"plate.png";
		App::GetApp()->RegisterTexture(L"PLATE_TX", strTexture);

	}


	void Scene::OnCreate() {
		try {
			//リソース作成
			CreateResourses();
			//最初のアクティブステージの設定
			ResetActiveStage<GameStage>();
		}
		catch (...) {
			throw;
		}
	}



}
//end basecross
