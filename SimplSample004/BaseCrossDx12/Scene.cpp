/*!
@file Scene.cpp
@brief シーンなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	void Scene::OnCreate() {
		//四角形スプライトグループの作成
		m_SquareSpriteGroup = ObjectFactory::Create<SquareSpriteGroup>();
	}

	void Scene::OnUpdate() {
		m_SquareSpriteGroup->OnUpdate();
	}
	
	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		m_SquareSpriteGroup->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}

}
//end basecross
