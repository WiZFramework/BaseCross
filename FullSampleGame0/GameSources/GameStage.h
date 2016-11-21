/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	理科室ステージクラス
	//--------------------------------------------------------------------------------------
	class ScienceRoom : public Stage {
	protected:
		//壁の作成
		void CreateTileWall();
		//固定のボックスの作成
		void CreateFixedBox();
		//入口ドアの作成
		void CreateRoomDoor();
		//ウインドウの作成
		void CreateRoomWindow();
		//頭の作成
		void CreateFixHead();
		//構築と破棄
		ScienceRoom() :Stage()
		{}
		virtual ~ScienceRoom() {}
	public:
		//初期化
	};

	//--------------------------------------------------------------------------------------
	//	現在の理科室ステージクラス
	//--------------------------------------------------------------------------------------
	class NowScienceRoom : public ScienceRoom {
		//プレートの作成
		void CreatePlate();
		//ビューの作成
		void CreateViewLight();
		//くぐる壁の作成
		void CreateThroughWall();
		//頭の敵の作成
		void CreateHead();
		//もぞ敵の作成
		void CreateMozo();
		//固定のシリンダーの作成
		void CreateFixedCylinder();
		//平和の固定のシリンダーの作成
		void CreatteClearFixedCylinder();
		//シリンダーに付属の吹きだしの作成
		void CreatePopupBalloon();
		//プレイヤーの作成
		void CreatePlayer();
	public:
		//構築と破棄
		NowScienceRoom() :ScienceRoom()
		{}
		virtual ~NowScienceRoom() {}
		//初期化
		virtual void OnCreate()override;
	};

	//--------------------------------------------------------------------------------------
	//	過去の理科室ステージクラス
	//--------------------------------------------------------------------------------------
	class PastScienceRoom : public ScienceRoom {
		//プレートの作成
		void CreatePlate();
		//ビューの作成
		void CreateViewLight();
		//くぐる壁の作成
		void CreateThroughWall();
		//頭の敵の作成
		void CreateHead();
		//固定のシリンダーの作成
		void CreateFixedCylinder();
		//プレイヤーの作成
		void CreatePlayer();
	public:
		//構築と破棄
		PastScienceRoom() :ScienceRoom()
		{}
		virtual ~PastScienceRoom() {}
		//初期化
		virtual void OnCreate()override;
	};

	//--------------------------------------------------------------------------------------
	//	クリアステージクラス
	//--------------------------------------------------------------------------------------
	class ClearStage : public Stage {
	public:
		//構築と破棄
		ClearStage() :Stage()
		{}
		virtual ~ClearStage() {}
		//初期化
		virtual void OnCreate()override;
		//更新
		virtual void OnUpdate()override;
	};

	//--------------------------------------------------------------------------------------
	//	ゲームオーバーステージクラス
	//--------------------------------------------------------------------------------------
	class GameOverStage : public Stage {
	public:
		//構築と破棄
		GameOverStage() :Stage()
		{}
		virtual ~GameOverStage() {}
		//初期化
		virtual void OnCreate()override;
		//更新
		virtual void OnUpdate()override;
	};

}
//end basecross

