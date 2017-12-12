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
		//プレートの作成
		void CreatePlate();
		//物理計算オブジェクトの作成
		void CreatePhysicsObjects();
		//プレイヤーの作成
		void CreatePlayer();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
	};


}
//end basecross

