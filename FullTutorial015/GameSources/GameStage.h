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
		//ステージ数は3個
		GameStage() :Stage(){}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
		//更新
		virtual void OnUpdate()override;
	};

	class AnimeSprite;
	//--------------------------------------------------------------------------------------
	//	セレクトステージクラス
	//--------------------------------------------------------------------------------------
	class SelectStage : public Stage {
		//ビューの作成
		void CreateViewLight();
		//スプライトの作成
		void CreateAnimeSprite();
		bool m_CntrolLock;
		//スプライトのweak_ptrの配列
		vector<weak_ptr<AnimeSprite>> m_SpVec;
		//選択がチェンジされた
		void ChangeSelect(int num);
	public:
		//構築と破棄
		SelectStage() :Stage(),  m_CntrolLock(false),
			m_SpVec(3)
		{
		}
		virtual ~SelectStage() {}
		//初期化
		virtual void OnCreate()override;
		//更新
		virtual void OnUpdate()override;
	};



}
//end basecross

