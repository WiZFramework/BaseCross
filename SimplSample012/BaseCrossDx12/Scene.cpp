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
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"sky.jpg";
		//球の作成
		m_SphereObject1 = ObjectFactory::Create<SphereObject>(18, strTexture, false, Vector3(0.0f, 0, 2.0f));
		strTexture = DataDir + L"trace.png";
		//球の作成
		m_SphereObject2 = ObjectFactory::Create<SphereObject>(18, strTexture, true, Vector3(0.0f, 0, 0.0f));
	}

	void Scene::OnUpdate() {
		m_SphereObject1->OnUpdate();
		m_SphereObject2->OnUpdate();
	}
	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefultViews(Color4(0, 0, 0, 1.0));
		//デフォルト描画の開始
		Dev->StartDefultDraw();
		m_SphereObject1->OnDraw();
		m_SphereObject2->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefultDraw();
	}

}
//end basecross
