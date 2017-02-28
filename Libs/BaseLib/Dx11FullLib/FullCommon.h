/*!
@file FullCommon.h
@brief Fullバージョン用ライブラリ（Dx11専用含む）用のヘッダをまとめる
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once

//共有リソース
#include "../SharedLib/SharedResources.h"
//コンポーネント
#include "../SharedLib/Components.h"
//アクション系コンポーネント
#include "../SharedLib/Action.h"
//衝突判定コンポーネント
#include "../SharedLib/Collision.h"
//物理計算コンポーネント
#include "../SharedLib/Rigidbody.h"
//描画コンポーネント
#include "DrawComponents.h"
//拡張描画コンポーネント
#include "DrawComponentsEx.h"
//文字列描画コンポーネント
#include "StringComponents.h"
//行動クラス
#include "../SharedLib/Behavior.h"
//行動クラス(Rigidbody関連)
#include "../SharedLib/BehaviorSteering.h"
//ゲームオブジェクト、ステージなど
#include "../SharedLib/GameObject.h"
//スプライトスタジオ５
#include "../SharedLib/SpriteStudio5.h"
