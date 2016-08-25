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
		m_CubeObject = ObjectFactory::Create<CubeObject>();
	}

	void Scene::OnUpdate() {
		m_CubeObject->OnUpdate();
	}
	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefultViews(Color4(0, 0, 0, 1.0));
		//デフォルト描画の開始
		Dev->StartDefultDraw();
		m_CubeObject->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefultDraw();
	}

}
//end basecross
