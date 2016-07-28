/*!
@file Common.h
@brief ライブラリ用のヘッダをまとめる
*/

#pragma once

//ユーティリティ基本クラス（削除テンプレート、例外処理など）
#include "../DxLib/BaseHelper.h"
//XML読み込み
#include "../DxLib/XmlDoc.h"
//ベクトル計算の計算クラス
#include "../DxLib/MathVector.h"
//行列、クオータニオン、カラーなどの計算クラス
#include "../DxLib/MathExt.h"
//ベクトルのスタティック計算
#include "../DxLib/MathVectorEX.h"
//行列、クオータニオン、カラーなどのスタティック計算
#include "../DxLib/MathExtEX.h"
//衝突判定、補間処理用ユーティリティ
#include "../DxLib/TransHelper.h"
//頂点定義、
#include "VertexHelper.h"
//プリミティブ作成関数等
#include "../DxLib/MeshHelper.h"


