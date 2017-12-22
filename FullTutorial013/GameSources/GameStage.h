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
		//エフェクトのインターフェイス
		shared_ptr<EfkInterface> m_EfkInterface;
		//CSVファイルそのA
		CsvFile m_GameStageCsvA;
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
		//敵の作成
		void CreateEnemy();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
		//エフェクトのインターフェイスの取得
		shared_ptr<EfkInterface> GetEfkInterface() const {
			return m_EfkInterface;
		}
		//更新
		virtual void OnUpdate() override;
		//描画
		virtual void OnDraw() override;

	};


}
//end basecross

