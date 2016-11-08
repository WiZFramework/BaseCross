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
		//壁の作成
		void CreateTileWall();
		//くぐる壁の作成
		void CreateThroughWall();
		//固定のボックスの作成
		void CreateFixedBox();
		//頭の敵の作成
		void CreateHead();
		//もぞ敵の作成
		void CreateMozo();
		//固定のシリンダーの作成
		void CreateFixedCylinder();
		//プレイヤーの作成
		void CreatePlayer();
		//カメラマンの作成
		void CreateCameraman();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;
	};


}
//end basecross

