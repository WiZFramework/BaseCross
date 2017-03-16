/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		//ビューの作成
		void CreateViewLight();
		//半透明のスプライト作成
		void CreateTraceSprite();
		//壁模様のスプライト作成
		void CreateWallSprite();
		//スクロールするスプライト作成
		void CreateScrollSprite();
		//スコアスプライト作成
		void CreateScoreSprite();
		//トータル時間
		float m_TotalTime;
	public:
		//構築と破棄
		GameStage() :Stage(), m_TotalTime(0){}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
		//更新
		virtual void OnUpdate()override;
	};


}
//end basecross

