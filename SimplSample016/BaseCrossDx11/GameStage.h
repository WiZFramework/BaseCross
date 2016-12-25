/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	メニューステージクラス
	//--------------------------------------------------------------------------------------
	class MenuStage : public Stage {
		//リソースの作成
		void CreateResourses();
		//ビューの作成
		void CreateViewLight();
		//壁模様のスプライト作成
		void CreateWallSprite();
		//スクロールするスプライト作成
		void CreateScrollSprite();
	public:
		//構築と破棄
		MenuStage() :Stage() {}
		virtual ~MenuStage() {}
		//初期化
		virtual void OnCreate()override;
		//更新
		virtual void OnUpdate() override;
	};


	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		//ビューの作成
		void CreateViewLight();
		//プレートの作成
		void CreatePlate();
		//セルマップの作成
		void CreateStageCellMap();
		//固定のボックスの作成
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
	};


}
//end basecross

