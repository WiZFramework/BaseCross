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
		//リソースの作成
		void CreateResourses();
		//ビューの作成
		void CreateViewLight();
		//プレートの作成
		void CreatePlate();
		//追いかけるオブジェクトの作成
		void CreateSeekObject();
		//固定のボックスの作成
		void CreateFixedBox();
		//上下移動しているボックスの作成
		void CreateMoveBox();
		//ヒットする球体の作成
		void CreateSphere();
		//半透明のスプライト作成
		void CreateTraceSprite();
		//壁模様のスプライト作成
		void CreateWallSprite();
		//スクロールするスプライト作成
		void CreateScrollSprite();
		//スコアスプライト作成
		void CreateScoreSprite();
		//左上で回転する立方体
		void CreateRollingCube();
		//形状が変わる球体
		void CreateTransSphere();
		//でこぼこ床の作成
		void CreateUnevenGround();
		//スパークの作成
		void CreateSpark();
		//炎の作成
		void CreateFire();
		//プレイヤーの作成
		void CreatePlayer();

		float m_TotalTime;
	public:
		//構築と破棄
		GameStage() :Stage(), m_TotalTime(0){}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;

		virtual void OnUpdate()override;

	};


}
//end basecross

