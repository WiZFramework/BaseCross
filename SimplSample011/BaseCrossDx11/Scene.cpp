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
		//サンプルのためアセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(DataDir);
		//各ゲームは以下のようにデータディレクトリを取得すべき
		//App::GetApp()->GetDataDirectory(DataDir);

		wstring strTexture = DataDir + L"sky.jpg";
		//立方体の作成
		m_CubeObject1 = ObjectFactory::Create<CubeObject>(strTexture, false, Vec3(-1.0f, 0, 2.0f), true);
		m_CubeObject2 = ObjectFactory::Create<CubeObject>(strTexture, false, Vec3(1.0f, 0, 2.0f), false);
		strTexture = DataDir + L"trace.png";
		//立方体の作成
		m_CubeObject3 = ObjectFactory::Create<CubeObject>(strTexture, true, Vec3(-2.0f, 0, 0), true);
		m_CubeObject4 = ObjectFactory::Create<CubeObject>(strTexture, true, Vec3(2.0f, 0, 0), false);
	}

	void Scene::OnUpdate() {
		m_CubeObject1->OnUpdate();
		m_CubeObject2->OnUpdate();
		m_CubeObject3->OnUpdate();
		m_CubeObject4->OnUpdate();
	}
	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Col4(0, 0, 0, 1.0f));
		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		m_CubeObject1->OnDraw();
		m_CubeObject2->OnDraw();
		m_CubeObject3->OnDraw();
		m_CubeObject4->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}

}
//end basecross
