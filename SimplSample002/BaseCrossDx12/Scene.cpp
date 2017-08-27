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
		//三角形の作成
		m_TriangleSprite = ObjectFactory::Create<TriangleSprite>();
	}

	void Scene::OnUpdate() {
		m_TriangleSprite->OnUpdate();
	}
	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0));
		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		//三角形の描画
		m_TriangleSprite->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}

}
//end basecross
