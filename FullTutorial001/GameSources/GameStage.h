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
		//ビューとライトの作成
		void CreateViewLight();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
	};


}
//end basecross

