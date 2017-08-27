/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------
	//ビューの作成
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//シャドウマップは使用しない
		SetShadowmapDraw(false);
	}

	//半透明のスプライト作成
	void GameStage::CreateTraceSprite() {
		AddGameObject<TraceSprite>(true,
			Vec2(320.0f, 320.0f), Vec3(-400.0f, 0.0f, 0.0f));
	}


	//壁模様のスプライト作成
	void GameStage::CreateWallSprite() {
		AddGameObject<WallSprite>(L"WALL_TX", false,
			Vec2(320.0f, 320.0f), Vec3(400.0f, 0.0f ,0.1f));
	}


	//スクロールするスプライト作成
	void GameStage::CreateScrollSprite() {
		AddGameObject<ScrollSprite>(L"TRACE_TX", true,
			Vec2(240.0f, 60.0f), Vec3(400.0f, 0.0f,0.0f));
	}

	//スコアスプライト作成
	void GameStage::CreateScoreSprite() {
		AddGameObject<ScoreSprite>(4,
			L"NUMBER_TX",
			true,
			Vec2(320.0f, 80.0f),
			Vec3(0.0f, 0.0f,0.0f));
	}



	void GameStage::OnCreate() {
		try {
			//ビューとライトの作成
			CreateViewLight();
			//半透明のスプライト作成
			CreateTraceSprite();
			//壁模様のスプライト作成
			CreateWallSprite();
			//スクロールするスプライト作成
			CreateScrollSprite();
			//スコアスプライト作成
			CreateScoreSprite();
		}
		catch (...) {
			throw;
		}
	}

	void GameStage::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= 10000.0f) {
			m_TotalTime = 0.0f;
		}
		//スコアを更新する
		auto ScorPtr = GetSharedGameObject<ScoreSprite>(L"ScoreSprite");
		ScorPtr->SetScore(m_TotalTime);
	}


}
//end basecross
