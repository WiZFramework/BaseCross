/*!
@file MathVectorEX.h
@brief ベクトルののnamespaceによる計算<br />
XNAMATH のラッピング関数群
@copyright Copyright (c) 2016 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once

#include "stdafx.h"

namespace basecross{

	void Vector2::Transform(const Matrix4X4& m) {
		*this = XMVector2Transform(XMVECTOR(*this), XMMATRIX(m));

	}

	//--------------------------------------------------------------------------------------
	///	Vector2EX（2次元ベクトル）ネームスペース
	//--------------------------------------------------------------------------------------
	namespace Vector2EX {
		//--------------------------------------------------------------------------------------
		/*!
		@brief	文字列から作成する
		@param[in]	x_str	X数値に変換できる文字列
		@param[in]	y_str	Y数値に変換できる文字列
		@return	作成したベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline Vector2 WStr2ToVec2(wstring& x_str, wstring& y_str) {
			return
				Vector2(
					(float)_wtof(x_str.c_str()),
					(float)_wtof(y_str.c_str())
					);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルと等しいかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	等しければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool Equal(const Vector2& Vec, const Vector2& other) {
			return XMVector2Equal(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルと等しいかどうかを検証する。<br />
		それぞれの要素は符号なし整数として扱う
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	等しければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool EqualInt(const Vector2& Vec, const Vector2& other) {
			return XMVector2EqualInt(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルより大きいかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	大きければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool Greater(const Vector2& Vec, const Vector2& other) {
			return XMVector2Greater(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトル以上の大きさかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	以上の大きさならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool GreaterOrEqual(const Vector2& Vec, const Vector2& other) {
			return XMVector2GreaterOrEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルが値のいずれかが正か負の無限大かどうかを検証する
		@param[in]	Vec	ベクトル
		@return	値のいずれかが正か負の無限大ならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool IsInfinite(const Vector2& Vec) {
			return XMVector2IsInfinite(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの値のいずれかが非数（無効値）かどうかを検証する
		@param[in]	Vec	ベクトル
		@return	値のいずれかが非数（無効値）ならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool IsNaN(const Vector2& Vec) {
			return XMVector2IsNaN(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルより小さいかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	小さければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool Less(const Vector2& Vec, const Vector2& other) {
			return XMVector2Less(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトル以下の大きさかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	以下の大きさならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool LessOrEqual(const Vector2& Vec, const Vector2& other) {
			return XMVector2LessOrEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルが近い大きさかどうかを検証する。
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@param[in]	Epsilon	比較する最大許容差
		@return	近い大きさならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool NearEqual(const Vector2& Vec, const Vector2& other, float Epsilon) {
			float temp = Epsilon;
			XMVECTOR VecEp = XMLoadFloat(&temp);
			return XMVector2NearEqual(XMVECTOR(Vec), XMVECTOR(other), VecEp);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルと等しくないかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	等しくなければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool NotEqual(const Vector2& Vec, const Vector2& other) {
			return XMVector2NotEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルと等しくないかどうかを検証する<br />
		それぞれの要素は符号なし整数として扱う
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	等しくなければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool NotEqualInt(const Vector2& Vec, const Vector2& other) {
			return XMVector2NotEqualInt(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	正規化したベクトル同士のラジアン角度を計算する。正規化はしないので、引数ははあらかじめ正規化しておく<br />
		ベクトルのラジアン角計算では、正規化しない分速い
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@return	ラジアン角
		*/
		//--------------------------------------------------------------------------------------
		inline float AngleBetweenNormals(const Vector2& Vec1, const Vector2& Vec2) {
			return ((Vector2)XMVector2AngleBetweenNormals(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	正規化したベクトル同士のラジアン角度を計算予測する。正規化はしないので、引数ははあらかじめ正規化しておく<br />
		ベクトルのラジアン角計算では、正規化しない分速い
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@return	ラジアン角
		*/
		//--------------------------------------------------------------------------------------
		inline float AngleBetweenNormalsEst(const Vector2& Vec1, const Vector2& Vec2) {
			return ((Vector2)XMVector2AngleBetweenNormalsEst(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	2つのベクトル間のラジアン角を計算する
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@return	ラジアン角
		*/
		//--------------------------------------------------------------------------------------
		inline float AngleBetweenVectors(const Vector2& Vec1, const Vector2& Vec2) {
			return ((Vector2)XMVector2AngleBetweenVectors(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さを指定した大きさにクランプする
		@param[in]	Vec	ベクトル
		@param[in]	LengthMin	長さの最小値
		@param[in]	LengthMax	長さの最大値
		@return	クランプした結果ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline Vector2 ClampLength(const Vector2& Vec, float LengthMin, float LengthMax) {
			return (Vector2)XMVector2ClampLength(XMVECTOR(Vec), LengthMin, LengthMax);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さを指定した大きさにクランプする
		@param[in]	Vec	ベクトル
		@param[in]	LengthMinV	長さの最小値(ベクトル)
		@param[in]	LengthMaxV	長さの最大値(ベクトル)
		@return	クランプした結果ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline Vector2 ClampLengthV(const Vector2& Vec, const Vector2& LengthMinV, const Vector2& LengthMaxV) {
			return (Vector2)XMVector2ClampLengthV(XMVECTOR(Vec), XMVECTOR(LengthMinV), XMVECTOR(LengthMaxV));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	外積を計算して返す
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@return	外積
		*/
		//--------------------------------------------------------------------------------------
		inline float Cross(const Vector2& Vec1, const Vector2& Vec2) {
			return ((Vector2)XMVector2Cross(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	内積を計算して返す
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@return	内積
		*/
		//--------------------------------------------------------------------------------------
		inline float Dot(const Vector2& Vec1, const Vector2& Vec2) {
			return ((Vector2)XMVector2Dot(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	すべての要素が、設定された境界ベクトル内にある場合はtrueを返す
		@param[in]	Vec1	比較元ベクトル
		@param[in]	Vec2	比較するベクトル
		@return	境界内にある場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool InBounds(const Vector2& Vec1, const Vector2& Vec2) {
			return XMVector2InBounds(XMVECTOR(Vec1), XMVECTOR(Vec2));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さを返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さ
		*/
		//--------------------------------------------------------------------------------------
		inline float Length(const Vector2& Vec) {
			return ((Vector2)XMVector2Length(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さを予想して返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さの予想
		*/
		//--------------------------------------------------------------------------------------
		inline float LengthEst(const Vector2& Vec) {
			return ((Vector2)XMVector2LengthEst(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さの2乗を返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さの2乗
		*/
		//--------------------------------------------------------------------------------------
		inline float LengthSq(const Vector2& Vec) {
			return ((Vector2)XMVector2LengthSq(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルを正規化する
		@param[in]	Vec	ベクトル
		@return	正規化したベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline Vector2 Normalize(const Vector2& Vec) {
			return (Vector2)XMVector2Normalize(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	正規化の予想ベクトルを返す
		@param[in]	Vec	ベクトル
		@return	正規化の予想ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline Vector2 NormalizeEst(const Vector2& Vec) {
			return (Vector2)XMVector2NormalizeEst(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	垂直なベクトルを返す
		@param[in]	Vec	ベクトル
		@return	垂直なベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline Vector2 Orthogonal(const Vector2& Vec) {
			return (Vector2)XMVector2Orthogonal(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さの逆数を返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さの逆数
		*/
		//--------------------------------------------------------------------------------------
		inline float ReciprocalLength(const Vector2& Vec) {
			return ((Vector2)XMVector2ReciprocalLength(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さの逆数の予測値を返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さの逆数の予測値
		*/
		//--------------------------------------------------------------------------------------
		inline  float ReciprocalLengthEst(const Vector2& Vec) {
			return ((Vector2)XMVector2ReciprocalLengthEst(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルを法線ベクトルによって反射させたベクトル(入射ベクトル)を返す
		@param[in]	Vec	ベクトル
		@param[in]	Normal	法線ベクトル
		@return	反射させたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector2 Reflect(const Vector2& Vec, const Vector2& Normal) {
			return (Vector2)XMVector2Reflect(XMVECTOR(Vec), XMVECTOR(Normal));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルを法線ベクトルによって屈折率で反射させたベクトル(入射ベクトル)を返す
		@param[in]	Vec	ベクトル
		@param[in]	Normal	屈折させる法線ベクトル
		@param[in]	RefractionIndex	屈折率
		@return	反射させたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector2 Refract(const Vector2& Vec, const Vector2& Normal, float RefractionIndex) {
			return (Vector2)XMVector2Refract(XMVECTOR(Vec), XMVECTOR(Normal), RefractionIndex);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルを法線ベクトルによって屈折率ベクトル（すべての要素が同じ値）で反射させたベクトル(入射ベクトル)を返す
		@param[in]	Vec	ベクトル
		@param[in]	Normal	屈折させる法線ベクトル
		@param[in]	RefractionIndex	屈折率ベクトル
		@return	反射させたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector2 RefractV(const Vector2& Vec, const Vector2& Normal, const Vector2& RefractionIndex) {
			return (Vector2)XMVector2RefractV(XMVECTOR(Vec), XMVECTOR(Normal),
				XMVECTOR(RefractionIndex));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルをmによってトランスフォームさせたベクトルを返す
		@param[in]	Vec	ベクトル
		@param[in]	m	トランスフォームする行列
		@return	トランスフォームさせたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector2 Transform(const Vector2& Vec, const XMMATRIX& m) {
			return (Vector2)XMVector2Transform(XMVECTOR(Vec), m);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルをmによってトランスフォームさせたベクトルを返す
		@param[in]	Vec	ベクトル
		@param[in]	m	トランスフォームする行列
		@return	トランスフォームさせたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector2 Transform(const Vector2& Vec, const Matrix4X4& m) {
			return (Vector2)XMVector2Transform(XMVECTOR(Vec), XMMATRIX(m));
		}
	};
	// end Vector2EX

	void Vector3::Transform(const Matrix4X4& m) {
		*this = XMVector3Transform(XMVECTOR(*this), XMMATRIX(m));
	}

	//--------------------------------------------------------------------------------------
	///	Vector3EX（3次元ベクトル）ネームスペース
	//--------------------------------------------------------------------------------------
	namespace Vector3EX {
		//--------------------------------------------------------------------------------------
		/*!
		@brief	文字列から作成する
		@param[in]	x_str	X数値に変換できる文字列
		@param[in]	y_str	Y数値に変換できる文字列
		@param[in]	z_str	Z数値に変換できる文字列
		@return	作成したベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline Vector3 WStr3ToVec3(wstring& x_str, wstring& y_str, wstring& z_str) {
			return
				Vector3(
					(float)_wtof(x_str.c_str()),
					(float)_wtof(y_str.c_str()),
					(float)_wtof(z_str.c_str())
					);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルと等しいかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	等しければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool Equal(const Vector3& Vec, const Vector3& other) {
			return XMVector3Equal(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルと等しいかどうかを検証する。<br />
		それぞれの要素は符号なし整数として扱う
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	等しければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool EqualInt(const Vector3& Vec, const Vector3& other) {
			return XMVector3EqualInt(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルより大きいかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	大きければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool Greater(const Vector3& Vec, const Vector3& other) {
			return XMVector3Greater(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトル以上の大きさかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	以上の大きさならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool GreaterOrEqual(const Vector3& Vec, const Vector3& other) {
			return XMVector3GreaterOrEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルが値のいずれかが正か負の無限大かどうかを検証する
		@param[in]	Vec	ベクトル
		@return	値のいずれかが正か負の無限大ならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool IsInfinite(const Vector3& Vec) {
			return XMVector3IsInfinite(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの値のいずれかが非数（無効値）かどうかを検証する
		@param[in]	Vec	ベクトル
		@return	値のいずれかが非数（無効値）ならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool IsNaN(const Vector3& Vec) {
			return XMVector3IsNaN(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルより小さいかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	小さければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool Less(const Vector3& Vec, const Vector3& other) {
			return XMVector3Less(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトル以下の大きさかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	以下の大きさならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool LessOrEqual(const Vector3& Vec, const Vector3& other) {
			return XMVector3LessOrEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルが近い大きさかどうかを検証する。
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@param[in]	Epsilon	比較する最大許容差
		@return	近い大きさならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool NearEqual(const Vector3& Vec, const Vector3& other, float Epsilon) {
			float temp = Epsilon;
			XMVECTOR VecEp = XMLoadFloat(&temp);
			return XMVector3NearEqual(XMVECTOR(Vec), XMVECTOR(other), VecEp);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルと等しくないかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	等しくなければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool NotEqual(const Vector3& Vec, const Vector3& other) {
			return XMVector3NotEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルと等しくないかどうかを検証する<br />
		それぞれの要素は符号なし整数として扱う
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	等しくなければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool NotEqualInt(const Vector3& Vec, const Vector3& other) {
			return XMVector3NotEqualInt(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	正規化したベクトル同士のラジアン角度を計算する。正規化はしないので、引数ははあらかじめ正規化しておく<br />
		ベクトルのラジアン角計算では、正規化しない分速い
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@return	ラジアン角
		*/
		//--------------------------------------------------------------------------------------
		inline  float AngleBetweenNormals(const Vector3& Vec1, const Vector3& Vec2) {
			return ((Vector3)XMVector3AngleBetweenNormals(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	正規化したベクトル同士のラジアン角度を計算予測する。正規化はしないので、引数ははあらかじめ正規化しておく<br />
		ベクトルのラジアン角計算では、正規化しない分速い
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@return	ラジアン角
		*/
		//--------------------------------------------------------------------------------------
		inline  float AngleBetweenNormalsEst(const Vector3& Vec1, const Vector3& Vec2) {
			return ((Vector3)XMVector3AngleBetweenNormalsEst(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	2つのベクトル間のラジアン角を計算する
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@return	ラジアン角
		*/
		//--------------------------------------------------------------------------------------
		inline  float AngleBetweenVectors(const Vector3& Vec1, const Vector3& Vec2) {
			return ((Vector3)XMVector3AngleBetweenVectors(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さを指定した大きさにクランプする
		@param[in]	Vec	ベクトル
		@param[in]	LengthMin	長さの最小値
		@param[in]	LengthMax	長さの最大値
		@return	クランプした結果ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 ClampLength(const Vector3& Vec, float LengthMin, float LengthMax) {
			return (Vector3)XMVector3ClampLength(XMVECTOR(Vec), LengthMin, LengthMax);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さを指定した大きさにクランプする
		@param[in]	Vec	ベクトル
		@param[in]	LengthMinV	長さの最小値(ベクトル)
		@param[in]	LengthMaxV	長さの最大値(ベクトル)
		@return	クランプした結果ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 ClampLengthV(const Vector3& Vec, const Vector3& LengthMinV, const Vector3& LengthMaxV) {
			return (Vector3)XMVector3ClampLengthV(XMVECTOR(Vec), XMVECTOR(LengthMinV), XMVECTOR(LengthMaxV));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	外積を計算して返す
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@return	外積
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 Cross(const Vector3& Vec1, const Vector3& Vec2) {
			return (Vector3)XMVector3Cross(XMVECTOR(Vec1), XMVECTOR(Vec2));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	内積を計算して返す
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@return	内積
		*/
		//--------------------------------------------------------------------------------------
		inline  float Dot(const Vector3& Vec1, const Vector3& Vec2) {
			return ((Vector3)XMVector3Dot(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	すべての要素が、設定された境界ベクトル内にある場合はtrueを返す
		@param[in]	Vec1	比較元ベクトル
		@param[in]	Vec2	比較するベクトル
		@return	境界内にある場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool InBounds(const Vector3& Vec1, const Vector3& Vec2) {
			return XMVector3InBounds(XMVECTOR(Vec1), XMVECTOR(Vec2));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さを返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さ
		*/
		//--------------------------------------------------------------------------------------
		inline float Length(const Vector3& Vec) {
			return ((Vector3)XMVector3Length(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さを予想して返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さの予想
		*/
		//--------------------------------------------------------------------------------------
		inline  float LengthEst(const Vector3& Vec) {
			return ((Vector3)XMVector3LengthEst(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さの2乗を返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さの2乗
		*/
		//--------------------------------------------------------------------------------------
		inline  float LengthSq(const Vector3& Vec) {
			return ((Vector3)XMVector3LengthSq(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルを正規化する
		@param[in]	Vec	ベクトル
		@return	正規化したベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 Normalize(const Vector3& Vec) {
			return (Vector3)XMVector3Normalize(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	正規化の予想ベクトルを返す
		@param[in]	Vec	ベクトル
		@return	正規化の予想ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 NormalizeEst(const Vector3& Vec) {
			return (Vector3)XMVector3NormalizeEst(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	垂直なベクトルを返す
		@param[in]	Vec	ベクトル
		@return	垂直なベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 Orthogonal(const Vector3& Vec) {
			return (Vector3)XMVector3Orthogonal(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さの逆数を返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さの逆数
		*/
		//--------------------------------------------------------------------------------------
		inline  float ReciprocalLength(const Vector3& Vec) {
			return ((Vector3)XMVector3ReciprocalLength(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さの逆数の予測値を返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さの逆数の予測値
		*/
		//--------------------------------------------------------------------------------------
		inline  float ReciprocalLengthEst(const Vector3& Vec) {
			return ((Vector3)XMVector3ReciprocalLengthEst(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルを法線ベクトルによって反射させたベクトル(入射ベクトル)を返す
		@param[in]	Vec	ベクトル
		@param[in]	Normal	法線ベクトル
		@return	反射させたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 Reflect(const Vector3& Vec, const Vector3& Normal) {
			return (Vector3)XMVector3Reflect(XMVECTOR(Vec), XMVECTOR(Normal));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルを法線ベクトルによって屈折率で反射させたベクトル(入射ベクトル)を返す
		@param[in]	Vec	ベクトル
		@param[in]	Normal	屈折させる法線ベクトル
		@param[in]	RefractionIndex	屈折率
		@return	反射させたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 Refract(const Vector3& Vec, const Vector3& Normal, float RefractionIndex) {
			return (Vector3)XMVector3Refract(XMVECTOR(Vec), XMVECTOR(Normal), RefractionIndex);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルを法線ベクトルによって屈折率ベクトル（すべての要素が同じ値）で反射させたベクトル(入射ベクトル)を返す
		@param[in]	Vec	ベクトル
		@param[in]	Normal	屈折させる法線ベクトル
		@param[in]	RefractionIndex	屈折率ベクトル
		@return	反射させたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 RefractV(const Vector3& Vec, const Vector3& Normal, const Vector3& RefractionIndex) {
			return (Vector3)XMVector3RefractV(XMVECTOR(Vec), XMVECTOR(Normal),
				XMVECTOR(RefractionIndex));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルをmによってトランスフォームさせたベクトルを返す
		@param[in]	Vec	ベクトル
		@param[in]	m	トランスフォームする行列
		@return	トランスフォームさせたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 Transform(const Vector3& Vec, const XMMATRIX& m) {
			return (Vector3)XMVector3Transform(XMVECTOR(Vec), m);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルをmによってトランスフォームさせたベクトルを返す
		@param[in]	Vec	ベクトル
		@param[in]	m	トランスフォームする行列
		@return	トランスフォームさせたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 Transform(const Vector3& Vec, const Matrix4X4& m) {
			return (Vector3)XMVector3Transform(XMVECTOR(Vec), XMMATRIX(m));
		}
	};
	//end Vector3EX

	void Vector4::Transform(const Matrix4X4& m) {
		*this = XMVector4Transform(XMVECTOR(*this), XMMATRIX(m));
	}


	//--------------------------------------------------------------------------------------
	///	Vector4EX（4次元ベクトル）ネームスペース
	//--------------------------------------------------------------------------------------
	namespace Vector4EX {
		//--------------------------------------------------------------------------------------
		/*!
		@brief	文字列から作成する
		@param[in]	x_str	X数値に変換できる文字列
		@param[in]	y_str	Y数値に変換できる文字列
		@param[in]	z_str	Z数値に変換できる文字列
		@param[in]	w_str	W数値に変換できる文字列
		@return	作成したベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector4 WStr4ToVec4(wstring& x_str, wstring& y_str, wstring& z_str, wstring& w_str) {
			return
				Vector4(
					(float)_wtof(x_str.c_str()),
					(float)_wtof(y_str.c_str()),
					(float)_wtof(z_str.c_str()),
					(float)_wtof(w_str.c_str())
					);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルと等しいかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	等しければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool Equal(const Vector4& Vec, const Vector4& other) {
			return XMVector4Equal(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルと等しいかどうかを検証する。<br />
		それぞれの要素は符号なし整数として扱う
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	等しければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool EqualInt(const Vector4& Vec, const Vector4& other) {
			return XMVector4EqualInt(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルより大きいかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	大きければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool Greater(const Vector4& Vec, const Vector4& other) {
			return XMVector4Greater(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトル以上の大きさかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	以上の大きさならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool GreaterOrEqual(const Vector4& Vec, const Vector4& other) {
			return XMVector4GreaterOrEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルが値のいずれかが正か負の無限大かどうかを検証する
		@param[in]	Vec	ベクトル
		@return	値のいずれかが正か負の無限大ならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool IsInfinite(const Vector4& Vec) {
			return XMVector4IsInfinite(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの値のいずれかが非数（無効値）かどうかを検証する
		@param[in]	Vec	ベクトル
		@return	値のいずれかが非数（無効値）ならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool IsNaN(const Vector4& Vec) {
			return XMVector4IsNaN(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルより小さいかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	小さければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool Less(const Vector4& Vec, const Vector4& other) {
			return XMVector4Less(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトル以下の大きさかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	以下の大きさならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool LessOrEqual(const Vector4& Vec, const Vector4& other) {
			return XMVector4LessOrEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルが近い大きさかどうかを検証する。
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@param[in]	Epsilon	比較する最大許容差
		@return	近い大きさならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool NearEqual(const Vector4& Vec, const Vector4& other, float Epsilon) {
			float temp = Epsilon;
			XMVECTOR VecEp = XMLoadFloat(&temp);
			return XMVector4NearEqual(XMVECTOR(Vec), XMVECTOR(other), VecEp);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルと等しくないかどうかを検証する
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	等しくなければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool NotEqual(const Vector4& Vec, const Vector4& other) {
			return XMVector4NotEqual(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルがもう一つのベクトルと等しくないかどうかを検証する<br />
		それぞれの要素は符号なし整数として扱う
		@param[in]	Vec	ベクトル
		@param[in]	other	比較するベクトル
		@return	等しくなければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool NotEqualInt(const Vector4& Vec, const Vector4& other) {
			return XMVector4NotEqualInt(XMVECTOR(Vec), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	正規化したベクトル同士のラジアン角度を計算する。正規化はしないので、引数ははあらかじめ正規化しておく<br />
		ベクトルのラジアン角計算では、正規化しない分速い
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@return	ラジアン角
		*/
		//--------------------------------------------------------------------------------------
		inline  float AngleBetweenNormals(const Vector4& Vec1, const Vector4& Vec2) {
			return ((Vector4)XMVector4AngleBetweenNormals(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	正規化したベクトル同士のラジアン角度を計算予測する。正規化はしないので、引数ははあらかじめ正規化しておく<br />
		ベクトルのラジアン角計算では、正規化しない分速い
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@return	ラジアン角
		*/
		//--------------------------------------------------------------------------------------
		inline  float AngleBetweenNormalsEst(const Vector4& Vec1, const Vector4& Vec2) {
			return ((Vector4)XMVector4AngleBetweenNormalsEst(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	2つのベクトル間のラジアン角を計算する
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@return	ラジアン角
		*/
		//--------------------------------------------------------------------------------------
		inline  float AngleBetweenVectors(const Vector4& Vec1, const Vector4& Vec2) {
			return ((Vector4)XMVector4AngleBetweenVectors(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さを指定した大きさにクランプする
		@param[in]	Vec	ベクトル
		@param[in]	LengthMin	長さの最小値
		@param[in]	LengthMax	長さの最大値
		@return	クランプした結果ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector4 ClampLength(const Vector4& Vec, float LengthMin, float LengthMax) {
			return (Vector4)XMVector4ClampLength(XMVECTOR(Vec), LengthMin, LengthMax);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さを指定した大きさにクランプする
		@param[in]	Vec	ベクトル
		@param[in]	LengthMinV	長さの最小値(ベクトル)
		@param[in]	LengthMaxV	長さの最大値(ベクトル)
		@return	クランプした結果ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector4 ClampLengthV(const Vector4& Vec, const Vector4& LengthMinV, const Vector4& LengthMaxV) {
			return (Vector4)XMVector4ClampLengthV(XMVECTOR(Vec), XMVECTOR(LengthMinV), XMVECTOR(LengthMaxV));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	外積を計算して返す
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@param[in]	Vec3	ベクトル3
		@return	外積
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector4 Cross(const Vector4& Vec1, const Vector4& Vec2, const Vector4& Vec3) {
			return (Vector4)XMVector4Cross(XMVECTOR(Vec1), XMVECTOR(Vec2), XMVECTOR(Vec3));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	内積を計算して返す
		@param[in]	Vec1	ベクトル1
		@param[in]	Vec2	ベクトル2
		@return	内積
		*/
		//--------------------------------------------------------------------------------------
		inline  float Dot(const Vector4& Vec1, const Vector4& Vec2) {
			return ((Vector4)XMVector4Dot(XMVECTOR(Vec1), XMVECTOR(Vec2))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	すべての要素が、設定された境界ベクトル内にある場合はtrueを返す
		@param[in]	Vec1	比較元ベクトル
		@param[in]	Vec2	比較するベクトル
		@return	境界内にある場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool InBounds(const Vector4& Vec1, const Vector4& Vec2) {
			return XMVector4InBounds(XMVECTOR(Vec1), XMVECTOR(Vec2));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さを返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さ
		*/
		//--------------------------------------------------------------------------------------
		inline  float Length(const Vector4& Vec) {
			return ((Vector4)XMVector4Length(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さを予想して返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さの予想
		*/
		//--------------------------------------------------------------------------------------
		inline  float LengthEst(const Vector4& Vec) {
			return ((Vector4)XMVector4LengthEst(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さの2乗を返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さの2乗
		*/
		//--------------------------------------------------------------------------------------
		inline  float LengthSq(const Vector4& Vec) {
			return ((Vector4)XMVector4LengthSq(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルを正規化する
		@param[in]	Vec	ベクトル
		@return	正規化したベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector4 Normalize(const Vector4& Vec) {
			return (Vector4)XMVector4Normalize(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	正規化の予想ベクトルを返す
		@param[in]	Vec	ベクトル
		@return	正規化の予想ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector4 NormalizeEst(const Vector4& Vec) {
			return (Vector4)XMVector4NormalizeEst(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	垂直なベクトルを返す
		@param[in]	Vec	ベクトル
		@return	垂直なベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector4 Orthogonal(const Vector4& Vec) {
			return (Vector4)XMVector4Orthogonal(XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さの逆数を返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さの逆数
		*/
		//--------------------------------------------------------------------------------------
		inline  float ReciprocalLength(const Vector4& Vec) {
			return ((Vector4)XMVector4ReciprocalLength(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルの長さの逆数の予測値を返す
		@param[in]	Vec	ベクトル
		@return	ベクトルの長さの逆数の予測値
		*/
		//--------------------------------------------------------------------------------------
		inline  float ReciprocalLengthEst(const Vector4& Vec) {
			return ((Vector4)XMVector4ReciprocalLengthEst(XMVECTOR(Vec))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルを法線ベクトルによって反射させたベクトル(入射ベクトル)を返す
		@param[in]	Vec	ベクトル
		@param[in]	Normal	法線ベクトル
		@return	反射させたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector4 Reflect(const Vector4& Vec, const Vector4& Normal) {
			return (Vector4)XMVector4Reflect(XMVECTOR(Vec), XMVECTOR(Normal));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルを法線ベクトルによって屈折率で反射させたベクトル(入射ベクトル)を返す
		@param[in]	Vec	ベクトル
		@param[in]	Normal	屈折させる法線ベクトル
		@param[in]	RefractionIndex	屈折率
		@return	反射させたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector4 Refract(const Vector4& Vec, const Vector4& Normal, float RefractionIndex) {
			return (Vector4)XMVector4Refract(XMVECTOR(Vec), XMVECTOR(Normal), RefractionIndex);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルを法線ベクトルによって屈折率ベクトル（すべての要素が同じ値）で反射させたベクトル(入射ベクトル)を返す
		@param[in]	Vec	ベクトル
		@param[in]	Normal	屈折させる法線ベクトル
		@param[in]	RefractionIndex	屈折率ベクトル
		@return	反射させたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector4 RefractV(const Vector4& Vec, const Vector4& Normal, const Vector4& RefractionIndex) {
			return (Vector4)XMVector4RefractV(XMVECTOR(Vec), XMVECTOR(Normal),
				XMVECTOR(RefractionIndex));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルをmによってトランスフォームさせたベクトルを返す
		@param[in]	Vec	ベクトル
		@param[in]	m	トランスフォームする行列
		@return	トランスフォームさせたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector4 Transform(const Vector4& Vec, const XMMATRIX& m) {
			return (Vector4)XMVector4Transform(XMVECTOR(Vec), m);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルをmによってトランスフォームさせたベクトルを返す
		@param[in]	Vec	ベクトル
		@param[in]	m	トランスフォームする行列
		@return	トランスフォームさせたベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector4 Transform(const Vector4& Vec, const Matrix4X4& m) {
			return (Vector4)XMVector4Transform(XMVECTOR(Vec), XMMATRIX(m));
		}


	};
	//end Vector4EX


}
//end of namespace basecross.



