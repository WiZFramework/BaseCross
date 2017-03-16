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

		//四角形の作成
		wstring strTexture = DataDir + L"sky.jpg";
		m_SquareSprite1 = ObjectFactory::Create<SquareSprite>(strTexture, false, Vector2(-100.0f, 0));
		strTexture = DataDir + L"trace.png";
		m_SquareSprite2 = ObjectFactory::Create<SquareSprite>(strTexture, true, Vector2(100.0f, 0));
	}

	void Scene::OnUpdate() {
		m_SquareSprite1->OnUpdate();
		m_SquareSprite2->OnUpdate();
	}
	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Color4(0, 0, 0, 1.0));
		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		m_SquareSprite1->OnDraw();
		m_SquareSprite2->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}

}
//end basecross
