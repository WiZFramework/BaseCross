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
		m_CubeObject1 = ObjectFactory::Create<CubeObject>(Vector3(-1.0f, 0, 0), false);
		m_CubeObject2 = ObjectFactory::Create<CubeObject>(Vector3(1.0f, 0, 0), true);
	}

	void Scene::OnUpdate() {
		m_CubeObject1->OnUpdate();
		m_CubeObject2->OnUpdate();
	}
	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Color4(0, 0, 0, 1.0));
		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		m_CubeObject1->OnDraw();
		m_CubeObject2->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}

}
//end basecross
