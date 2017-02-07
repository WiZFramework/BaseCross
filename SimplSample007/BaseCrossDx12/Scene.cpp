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
		wstring strTexture = DataDir + L"wall.jpg";
		m_WallSprite = ObjectFactory::Create<WallSprite>(strTexture, false, Vector2(0, 0));
		strTexture = DataDir + L"trace.png";
		m_SquareSprite = ObjectFactory::Create<SquareSprite>(strTexture, true, Vector2(0, 0));
	}

	void Scene::OnUpdate() {
		m_WallSprite->OnUpdate();
		m_SquareSprite->OnUpdate();
	}
	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefultViews(Color4(0, 0, 0, 1.0));
		//デフォルト描画の開始
		Dev->StartDefultDraw();
		m_WallSprite->OnDraw();
		m_SquareSprite->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefultDraw();
	}

}
//end basecross
