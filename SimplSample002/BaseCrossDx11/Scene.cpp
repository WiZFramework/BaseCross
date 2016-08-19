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
		//三角形スプライトの作成
		m_TriangleSprite = ObjectFactory::Create<TriangleSprite>();
	}

	void Scene::OnUpdate() {
		m_TriangleSprite->OnUpdate();
	}
	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefultViews(Color4(0, 0, 0, 1.0f));
		//デフォルト描画の開始
		Dev->StartDefultDraw();
		m_TriangleSprite->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefultDraw();
	}

}
//end basecross
