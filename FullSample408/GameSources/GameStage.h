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
		//プレイヤーの作成
		void CreatePlayer();
		//マスクスプライトの作成
		void CreateMaskSprite();
		bool m_MaskEnabled;
	public:
		//構築と破棄
		GameStage() :Stage(), m_MaskEnabled(true){}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
		virtual void OnUpdate()override;
	};


}
//end basecross

