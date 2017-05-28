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
		//CSVファイル
		CsvFile m_GameStageCsv;
		//ビューの作成
		void CreateViewLight();
		//プレートの作成
		void CreatePlate();
		//セルマップの作成
		void CreateStageCellMap();
		//ボックスの作成
		void CreateFixedBox();
		//スパークの作成
		void CreateSpark();
		//炎の作成
		void CreateFire();
		//プレイヤーの作成
		void CreatePlayer();
		//敵の作成
		void CreateEnemy();
	public:
		//固定のボックスのコストをセルマップに反映
		void SetCellMapCost();
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
	};


}
//end basecross

