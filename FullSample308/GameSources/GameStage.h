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
		//CSVファイルそのB
		CsvFile m_GameStageCsvB;
		//ビューの作成
		void CreateViewLight();
		//プレートの作成
		void CreatePlate();
		//ボックスの作成
		void CreateFixedBox();
		//スパークの作成
		void CreateSpark();
		//敵の作成
		void CreateEnemy();
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

