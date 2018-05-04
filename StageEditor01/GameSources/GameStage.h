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
	//タイリングプレートの作成
	void CreateTilingPlate();
	//ラインの作成
	void CreateActionLine();
	//プレイヤーの作成
	void CreatePlayer();
public:
	//構築と破棄
	GameStage() :Stage(){}
	virtual ~GameStage() {}
	//初期化
	virtual void OnCreate()override;
};


}
//end basecross

