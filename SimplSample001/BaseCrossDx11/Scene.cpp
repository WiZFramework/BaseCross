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
		//立方体の作成
		m_TriangleObject = ObjectFactory::Create<TriangleObject>();
	}

	void Scene::OnUpdate() {
		m_TriangleObject->OnUpdate();
	}
	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		m_TriangleObject->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}

}
//end basecross
