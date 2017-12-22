/*!
@file Common.h
@brief ライブラリ用のヘッダをまとめる
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once

//ユーティリティ基本クラス（削除テンプレート、例外処理など）
#include "../DxLib/BaseHelper.h"
//XML読み込み
#include "../DxLib/XmlDoc.h"
//シンプルな計算クラス
#include "../DxLib/BaseMath.h"
//物理計算
#include "../DxLib/physics_effects/include/physics_effects.h"
//衝突判定、補間処理用ユーティリティ
#include "../DxLib/TransHelper.h"
//頂点定義、
#include "VertexHelper.h"
//プリミティブ作成関数等
#include "../DxLib/MeshHelper.h"
//アプリケーションクラスなど
#include "../DxLib/App.h"
//デバイスなど
#include "DeviceResources.h"
//物理計算インターフェイス
#include "../DxLib/BasePhysics.h"





