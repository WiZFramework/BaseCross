/*!
@file MathExtEX.h
@brief 行列、クオータニオン、カラーなどのnamespaceによる計算<br />
XNAMATH のラッピング関数群
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once

#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	PlaneEX（平面）ネームスペース
	//--------------------------------------------------------------------------------------
	namespace PlaneEX {
		//--------------------------------------------------------------------------------------
		/*!
		@brief	planeとVector4の内積を返す。平面と同次座標の関係を判定するときに役立つ。<br />
		たとえば、この関数を使用してある座標が特定の平面上に存在するか、<br />
		または座標が平面のどちら側に存在するかなどを判定できる。
		@param[in]	plane	計算元の平面
		@param[in]	other	計算するVector4
		@return	内積
		*/
		//--------------------------------------------------------------------------------------
		inline float Dot(const Plane& plane, const Vector4& other) {
			return ((Vector4)XMPlaneDot(XMVECTOR(plane), XMVECTOR(other))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	planeとVector3の内積を計算し返す<br/>
		あるポイントから平面までの符号付き距離を求めるのに便利である
		@param[in]	plane	計算元の平面
		@param[in]	other	計算するVector3
		@return	内積
		*/
		//--------------------------------------------------------------------------------------
		inline float DotCoord(const Plane& plane, const Vector3& other) {
			return  ((Vector4)XMPlaneDotCoord(XMVECTOR(plane), XMVECTOR(other))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	planeの法線ベクトルと 3D ベクトルの間の内積を計算し返す<br/>
		平面の法線ベクトルと別の法線ベクトルとの角度を算出するときに役立つ
		@param[in]	plane	計算元の平面
		@param[in]	other	計算するVector3
		@return	内積
		*/
		//--------------------------------------------------------------------------------------
		inline float DotNormal(const Plane& plane, const Vector3& other) {
			return ((Vector4)XMPlaneDotNormal(XMVECTOR(plane), XMVECTOR(other))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	planeと相手の平面が同じかどうかを調べる
		@param[in]	plane	計算元の平面
		@param[in]	other	相手のPlane
		@return	同じならtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool Equal(const Plane& plane, const Plane& other) {
			return XMPlaneEqual(XMVECTOR(plane), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	点と法線から平面を作成して、返す
		@param[in]	Point	平面上の点
		@param[in]	Normal	法線
		@return	作成したPlane
		*/
		//--------------------------------------------------------------------------------------
		inline  Plane FromPointNormal(const Vector3& Point, const Vector3& Normal) {
			return (Plane)XMPlaneFromPointNormal(XMVECTOR(Point), XMVECTOR(Normal));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	３点から平面を作成して、返す
		@param[in]	Point1	頂点1
		@param[in]	Point2	頂点2
		@param[in]	Point3	頂点3
		@return	作成したPlane
		*/
		//--------------------------------------------------------------------------------------
		inline  Plane FromPoints(const Vector3& Point1, const Vector3& Point2, const Vector3& Point3) {
			return (Plane)XMPlaneFromPoints(XMVECTOR(Point1), XMVECTOR(Point2), XMVECTOR(Point3));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	planeと２つの頂点が作成する直線の交差する頂点を返す<br />
		平面と直線が平行の場合は、戻る各値はQNaN（非数） になるので、<br />
		Vector3::IsNaN()などでチェックする
		@param[in]	plane	計算元の平面
		@param[in]	LinePoint1	頂点1
		@param[in]	LinePoint2	頂点2
		@return	planeと交差する３Ｄベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 IntersectLine(const Plane& plane,
			const Vector3& LinePoint1, const Vector3& LinePoint2) {
			return (Vector3)XMPlaneIntersectLine(XMVECTOR(plane),
				XMVECTOR(LinePoint1), XMVECTOR(LinePoint2));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	planeともう一つの面が交差する2つの頂点を計算する<br />
		RetLinePoint1とRetLinePoint2に交差する頂点を返す<br />
		2つの平面が平行の場合は、戻る各値はQNaN（非数） になるので、<br />
		Vector3::IsNaN()などでチェックする
		@param[out]	RetLinePoint1	返される頂点1
		@param[out]	RetLinePoint2	返される頂点2
		@param[in]	plane	計算元の平面
		@param[in]	other	planeと比べられるもう一つの面
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		inline  void IntersectPlane(Vector3& RetLinePoint1, Vector3& RetLinePoint2,
			const Plane& plane, const Plane& other) {
			XMVECTOR Temp1, Temp2;
			XMPlaneIntersectPlane(&Temp1, &Temp2, XMVECTOR(plane), XMVECTOR(other));
			RetLinePoint1 = (Vector3)Temp1;
			RetLinePoint2 = (Vector3)Temp2;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	planeの要素のいずれかが正または負の無限大かどうかを調べる
		@param[in]	plane	計算元の平面
		@return	正または負の無限大ならtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool IsInfinite(const Plane& plane) {
			return XMPlaneIsInfinite(XMVECTOR(plane));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Planeの要素のいずれかが NaNかどうかを調べる
		@param[in]	plane	計算元の平面
		@return	NaNならtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool IsNaN(const Plane& plane) {
			return XMPlaneIsNaN(XMVECTOR(plane));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	planeともう一つの平面がほぼ等しいかどうか調べる
		@param[in]	plane	計算元の平面
		@param[in]	other	planeと比べられるもう一つの面
		@param[in]	Epsilon	各要素の許容範囲
		@return	ほぼ等しければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool NearEqual(const Plane& plane, const Plane& other, const Vector4& Epsilon) {
			return XMPlaneNearEqual(XMVECTOR(plane), XMVECTOR(other), Epsilon);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	planeを正規化する
		@param[in]	plane	計算元の平面
		@return	正規化した平面
		*/
		//--------------------------------------------------------------------------------------
		inline  Plane Normalize(const Plane& plane) {
			return (Plane)XMPlaneNormalize(XMVECTOR(plane));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	planeをx,y,zの係数が単位の法線ベクトルを形成するように、平面の係数を予測
		@param[in]	plane	計算元の平面
		@return	予測した平面
		*/
		//--------------------------------------------------------------------------------------
		inline  Plane NormalizeEst(const Plane& plane) {
			return (Plane)XMPlaneNormalizeEst(XMVECTOR(plane));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	planeと相手の平面が等しくないかどうかを調べる
		@param[in]	plane	計算元の平面
		@param[in]	other	相手の平面
		@return	等しくないならtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool NotEqual(const Plane& plane, const Plane& other) {
			return XMPlaneNotEqual(XMVECTOR(plane), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	planeをmでトランスフォームする
		@param[in]	plane	計算元の平面
		@param[in]	m	トランスフォームする行列
		@return	トランスフォームした平面
		*/
		//--------------------------------------------------------------------------------------
		inline  Plane Transform(const Plane& plane, const XMMATRIX& m) {
			return (Plane)XMPlaneTransform(XMVECTOR(plane), m);
		}
		//　XMPlaneTransformStream関数は、ラッピング関数は用意しない
		//　理由：パラメータに配列を渡す必要があり、ラッピングによって、最適化が損なわれる
	}
	//end PlaneEX

	//--------------------------------------------------------------------------------------
	///	Color4EX（カラー）ネームスペース
	//--------------------------------------------------------------------------------------
	namespace Color4EX {
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カラーがもう一つのカラーと等しいかどうかを検証する
		@param[in]	Col	計算元のカラー
		@param[in]	other	比較するカラー
		@return	等しければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool Equal(const Color4& Col, const Color4& other) {
			return XMColorEqual(XMVECTOR(Col), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カラーがもう一つのカラーと等しくないかどうかを検証する
		@param[in]	Col	計算元のカラー
		@param[in]	other	比較するカラー
		@return	等しくなければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool NotEqual(const Color4& Col, const Color4& other) {
			return XMColorNotEqual(XMVECTOR(Col), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カラーがもう一つのカラーより大きいかどうかを検証する
		@param[in]	Col	計算元のカラー
		@param[in]	other	比較するカラー
		@return	大きければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool Greater(const Color4& Col, const Color4& other) {
			return XMColorGreater(XMVECTOR(Col), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カラーがもう一つのカラー以上かどうかを検証する
		@param[in]	Col	計算元のカラー
		@param[in]	other	比較するカラー
		@return	以上であればtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool GreaterOrEqual(const Color4& Col, const Color4& other) {
			return XMColorGreaterOrEqual(XMVECTOR(Col), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カラーがもう一つのカラー以下かどうかを検証する
		@param[in]	Col	計算元のカラー
		@param[in]	other	比較するカラー
		@return	以下であればtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool LessOrEqual(const Color4& Col, const Color4& other) {
			return XMColorLessOrEqual(XMVECTOR(Col), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カラーがもう一つのカラーより小さいかどうかを検証する
		@param[in]	Col	計算元のカラー
		@param[in]	other	比較するカラー
		@return	より小さければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool Less(const Color4& Col, const Color4& other) {
			return XMColorLess(XMVECTOR(Col), XMVECTOR(other));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カラーのいずれかの成分が正または負の無限大かどうかをテストする
		@param[in]	Col	計算元のカラー
		@return	カラーのいずれかの成分が正または負の無限大ならtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool IsInfinite(const Color4& Col) {
			return XMColorIsInfinite(XMVECTOR(Col));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カラーのいずれかの成分がNaNかどうかをテストする
		@param[in]	Col	計算元のカラー
		@return	カラーのいずれかの成分がNaNならtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool IsNaN(const Color4& Col) {
			return XMColorIsNaN(XMVECTOR(Col));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カラーを0.0から1.0の範囲に調整する。範囲外の要素は0.0か1.0に設定される
		@param[in]	Col	計算元のカラー
		@return	調整したカラー
		*/
		//--------------------------------------------------------------------------------------
		inline  Color4 AdjustRange(const Color4& Col) {
			Color4 temp = Col;
			if (temp.x <= 0) {
				temp.x = 0;
			}
			if (temp.y <= 0) {
				temp.y = 0;
			}
			if (temp.z <= 0) {
				temp.z = 0;
			}
			if (temp.w <= 0) {
				temp.w = 0;
			}
			if (temp.x >= 1.0f) {
				temp.x = 1.0f;
			}
			if (temp.y >= 1.0f) {
				temp.y = 1.0f;
			}
			if (temp.z >= 1.0f) {
				temp.z = 1.0f;
			}
			if (temp.w >= 1.0f) {
				temp.w = 1.0f;
			}
			return temp;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カラーのコントラストを調節する。カラーは0.0から1.0の範囲に調整され、計算される
		@param[in]	Col	計算元のカラー
		@param[in]	Contrast	コントラスト範囲
		@return	調整したカラー
		*/
		//--------------------------------------------------------------------------------------
		inline  Color4 AdjustContrast(const Color4& Col, float Contrast) {
			Color4 temp = Col;
			temp.AdjustRange();
			return (Color4)XMColorAdjustContrast(XMVECTOR(temp), Contrast);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カラーの彩度を調節する。カラーは0.0から1.0の範囲に調整され、計算される。<br />
		彩度値が0の場合はグレースケール、1.0の場合はもとの色になる
		@param[in]	Col	計算元のカラー
		@param[in]	Saturation	彩度値
		@return	調整したカラー
		*/
		//--------------------------------------------------------------------------------------
		inline  Color4 AdjustSaturation(const Color4& Col, float Saturation) {
			Color4 temp = Col;
			temp.AdjustRange();
			return (Color4)XMColorAdjustSaturation(XMVECTOR(temp), Saturation);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	対応する成分を乗算して2つのカラーをブレンドする。
		@param[in]	Col1	カラー1
		@param[in]	Col2	カラー2
		@return	ブレンドしたカラー
		*/
		//--------------------------------------------------------------------------------------
		inline  Color4 Modulate(const Color4& Col1, const Color4& Col2) {
			return (Color4)XMColorModulate(XMVECTOR(Col1), XMVECTOR(Col2));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カラーの負の RGB カラー値を求める。カラーは0.0から1.0の範囲に調整され、計算される。
		@param[in]	Col	計算元のカラー
		@return	負のRGBカラー値
		*/
		//--------------------------------------------------------------------------------------
		inline  Color4 Negative(const Color4& Col) {
			Color4 temp = Col;
			temp.AdjustRange();
			return (Color4)XMColorNegative(XMVECTOR(temp));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	グレーのカラーを得る。
		@return	グレーのカラー値（0.5,0.5,0.5,1.0）
		*/
		//--------------------------------------------------------------------------------------
		inline  Color4 GrayColor() {
			return Color4(0.5f, 0.5f, 0.5f, 1.0f);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	白のカラーを得る。
		@return	白のカラー値（1.0,1.0,1.0,1.0）
		*/
		//--------------------------------------------------------------------------------------
		inline  Color4 WhiteColor() {
			return Color4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	//end Color4EX

	//--------------------------------------------------------------------------------------
	///	QuaternionEX（クオータニオン）ネームスペース
	//--------------------------------------------------------------------------------------
	namespace QuaternionEX {
		//--------------------------------------------------------------------------------------
		/*!
		@brief	2つのクオータニオンが等しいかどうかを検証する
		@param[in]	Qt1	クオータニオン1
		@param[in]	Qt2	クオータニオン2
		@return	等しければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool Equal(const Quaternion& Qt1, const Quaternion& Qt2) {
			return XMQuaternionEqual(XMVECTOR(Qt1), XMVECTOR(Qt2));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	2つのクオータニオンが等しくないかどうかを検証する
		@param[in]	Qt1	クオータニオン1
		@param[in]	Qt2	クオータニオン2
		@return	等しくなければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool NotEqual(const Quaternion& Qt1, const Quaternion& Qt2) {
			return XMQuaternionNotEqual(XMVECTOR(Qt1), XMVECTOR(Qt2));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンが 単位クオータニオン(0, 0, 0, 1)かどうかを検証する。
		@param[in]	Qt	計算元のクオータニオン
		@return	単位クオータニオンならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool IsIdentity(const Quaternion& Qt) {
			return XMQuaternionIsIdentity(XMVECTOR(Qt));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンの値のいずれかが正か負の無限大かどうかを検証する。
		@param[in]	Qt	計算元のクオータニオン
		@return	値のいずれかが正か負の無限大ならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool IsInfinite(const Quaternion& Qt) {
			return XMQuaternionIsInfinite(XMVECTOR(Qt));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンの値のいずれかがNaNかどうかを検証する。
		@param[in]	Qt	計算元のクオータニオン
		@return	値のいずれかがNaNならばtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool IsNaN(const Quaternion& Qt) {
			return XMQuaternionIsNaN(XMVECTOR(Qt));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンを回転ベクトルにより追加回転する
		@param[in]	Qt	計算元のクオータニオン
		@param[in]	other	追加回転する回転ベクトル
		@return	計算結果のクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion AddRotation(const Quaternion& Qt, const Vector3& other) {
			Quaternion Span;
			Span.RotationRollPitchYawFromVector(other);
			return (Quaternion)XMQuaternionMultiply(XMVECTOR(Qt), XMVECTOR(Span));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンから回転ベクトルに変換して返す
		@param[in]	Qt	計算元のクオータニオン
		@return	回転ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 GetRotation(const Quaternion& Qt) {
			Quaternion Temp = Qt;
			Temp.Normalize();
			Matrix4X4 mt;
			mt.RotationQuaternion(Temp);
			Vector3 Rot;
			if (mt._32 == 1.0f) {
				Rot.x = XM_PI / 2.0f;
				Rot.y = 0;
				Rot.z = -atan2(mt._21, mt._11);
			}
			else if (mt._32 == -1.0f) {
				Rot.x = -XM_PI / 2.0f;
				Rot.y = 0;
				Rot.z = -atan2(mt._21, mt._11);
			}
			else {
				Rot.x = -asin(mt._32);
				Rot.y = -atan2(-mt._31, mt._33);
				Rot.z = atan2(mt._12, mt._11);
			}
			return Rot;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定されたクオータニオンを使用して、重心座標のポイントを返す
		@param[in]	Q0	三角形の1つ目のクオータニオン
		@param[in]	Q1	三角形の2つ目のクオータニオン
		@param[in]	Q2	三角形の3つ目のクオータニオン
		@param[in]	f	加重係数
		@param[in]	g	加重係数
		@return	重心座標のクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion BaryCentric(const Quaternion& Q0, const Quaternion& Q1, const Quaternion& Q2, float f, float g) {
			return (Quaternion)XMQuaternionBaryCentric(XMVECTOR(Q0), XMVECTOR(Q1), XMVECTOR(Q2), f, g);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定されたクオータニオンとベクトルを使用して、重心座標のポイントを返す
		@param[in]	Q0	三角形の1つ目のクオータニオン
		@param[in]	Q1	三角形の2つ目のクオータニオン
		@param[in]	Q2	三角形の3つ目のクオータニオン
		@param[in]	f	加重係数ベクトル（それぞれ要素が同じ値にする）
		@param[in]	g	加重係数ベクトル（それぞれ要素が同じ値にする）
		@return	重心座標のクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion BaryCentricV(const Quaternion& Q0, const Quaternion& Q1, const Quaternion& Q2,
			const Vector4& f, const Vector4& g) {
			return (Quaternion)XMQuaternionBaryCentricV(XMVECTOR(Q0), XMVECTOR(Q1), XMVECTOR(Q2), XMVECTOR(f), XMVECTOR(g));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンの共役を返す
		@param[in]	Qt	計算元のクオータニオン
		@return	クオータニオンの共役
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion Conjugate(const Quaternion& Qt) {
			return (Quaternion)XMQuaternionConjugate(XMVECTOR(Qt));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	2つのクオータニオンの内積を返す
		@param[in]	Qt1	クオータニオン1
		@param[in]	Qt2	クオータニオン2
		@return	クオータニオンの内積
		*/
		//--------------------------------------------------------------------------------------
		inline  float Dot(const Quaternion& Qt1, const Quaternion& Qt2) {
			return ((Vector4)XMQuaternionDot(XMVECTOR(Qt1), XMVECTOR(Qt2))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンの指数を返す。入力のw要素は無視される
		@param[in]	Qt	計算元のクオータニオン
		@return	クオータニオンの指数
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion Exp(const Quaternion& Qt) {
			return (Quaternion)XMQuaternionExp(XMVECTOR(Qt));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単位クオータニオンを返す
		@return	単位クオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion Identity() {
			return (Quaternion)XMQuaternionIdentity();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンの反転を返す
		@param[in]	Qt	計算元のクオータニオン
		@return	反転したクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion Inverse(const Quaternion& Qt) {
			return (Quaternion)XMQuaternionInverse(XMVECTOR(Qt));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンの長さを返す
		@param[in]	Qt	計算元のクオータニオン
		@return	クオータニオンの長さ
		*/
		//--------------------------------------------------------------------------------------
		inline  float Length(const Quaternion& Qt) {
			return ((Vector4)XMQuaternionLength(XMVECTOR(Qt))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンの長さの2乗を返す
		@param[in]	Qt	計算元のクオータニオン
		@return	クオータニオンの長さの2乗
		*/
		//--------------------------------------------------------------------------------------
		inline  float LengthSq(const Quaternion& Qt) {
			return ((Vector4)XMQuaternionLengthSq(XMVECTOR(Qt))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンの自然対数を返す（入力は正規化しておく）。
		@param[in]	Qt	計算元のクオータニオン
		@return	クオータニオンの自然対数
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion Ln(const Quaternion& Qt) {
			return (Quaternion)XMQuaternionLn(XMVECTOR(Qt));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	2つのクオータニオンを乗算して返す
		@param[in]	Qt1	計算元のクオータニオン
		@param[in]	Qt2	乗算するクオータニオン
		@return	クオータニオンを乗算した結果
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion Multiply(const Quaternion& Qt1, const Quaternion& Qt2) {
			return (Quaternion)XMQuaternionMultiply(XMVECTOR(Qt1), XMVECTOR(Qt2));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンを正規化して返す
		@param[in]	Qt	計算元のクオータニオン
		@return	正規化したクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion Normalize(const Quaternion& Qt) {
			return (Quaternion)XMQuaternionNormalize(XMVECTOR(Qt));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンを正規化バージョンの予測値を返す
		@param[in]	Qt	計算元のクオータニオン
		@return	正規化バージョンの予測値
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion NormalizeEst(const Quaternion& Qt) {
			return (Quaternion)XMQuaternionNormalizeEst(XMVECTOR(Qt));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンの長さの逆数を返す
		@param[in]	Qt	計算元のクオータニオン
		@return	クオータニオンの長さの逆数
		*/
		//--------------------------------------------------------------------------------------
		inline  float ReciprocalLength(const Quaternion& Qt) {
			return ((Vector4)XMQuaternionReciprocalLength(XMVECTOR(Qt))).x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	回転軸と角度からクオータニオンを作成する
		@param[in]	Vec	回転軸を表す３Ｄベクトル
		@param[in]	Angle	回転角度（時計回りラジアン）
		@return	作成したクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion RotationAxis(const Vector3& Vec, float Angle) {
			return (Quaternion)XMQuaternionRotationAxis(XMVECTOR(Vec), Angle);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	回転行列からクオータニオンを作成する
		@param[in]	m	回転行列
		@return	作成したクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion RotationMatrix(const XMMATRIX& m) {
			return (Quaternion)XMQuaternionRotationMatrix(m);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	正規化された回転軸と角度からクオータニオンを作成する
		@param[in]	Vec	回転軸を表す正規化された３Ｄベクトル
		@param[in]	Angle	回転角度（時計回りラジアン）
		@return	作成したクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion RotationNormal(const Vector3& Vec, float Angle) {
			return (Quaternion)XMQuaternionRotationNormal(XMVECTOR(Vec), Angle);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	各軸の回転からクオータニオンを作成する。回転はRollPitchYaw（Z,Y,X）の順で行われる。
		@param[in]	Pitch	X軸回転（ラジアン）
		@param[in]	Yaw		Y軸回転（ラジアン）
		@param[in]	Roll	Z軸回転（ラジアン）
		@return	作成したクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion RotationRollPitchYaw(float Pitch, float Yaw, float Roll) {
			return (Quaternion)XMQuaternionRotationRollPitchYaw(Pitch, Yaw, Roll);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	3D回転ベクトルからクオータニオンを作成する。回転はRollPitchYaw（Z,Y,X）の順で行われる。
		@param[in]	Vec	回転軸を表す３Ｄベクトル
		@return	作成したクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion RotationRollPitchYawFromVector(const Vector3& Vec) {
			return (Quaternion)XMQuaternionRotationRollPitchYawFromVector(Vec);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	2つのクオータニオンから係数で補間されたクオータニオンを作成する。<br />
		係数が0の場合はQ0、1.0ならQ1と同じものを返す
		@param[in]	Qt0	補間元（正規化されたクオータニオン）
		@param[in]	Qt1	補間先（正規化されたクオータニオン）
		@param[in]	t	補間係数（0.0fから1.0fの間を指定）
		@return	作成したクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion Slerp(const Quaternion& Qt0, const Quaternion& Qt1, float t) {
			return (Quaternion)XMQuaternionSlerp(XMVECTOR(Qt0), XMVECTOR(Qt1), t);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	2つのクオータニオンから係数(ベクトル)で補間されたクオータニオンを作成する。<br />
		係数が0の場合はQ0、1.0ならQ1と同じものを返す
		@param[in]	Qt0	補間元（正規化されたクオータニオン）
		@param[in]	Qt1	補間先（正規化されたクオータニオン）
		@param[in]	Vec	補間係数（すべての要素を0.0fから1.0fの間の同じ値を指定）
		@return	作成したクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion SlerpV(const Quaternion& Qt0, const Quaternion& Qt1, const Vector4& Vec) {
			return (Quaternion)XMQuaternionSlerpV(XMVECTOR(Qt0), XMVECTOR(Qt1), XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	球状平方補間を使用して、4 つの単位クオータニオン間を補間する<br />
		Q1、Q2、Q3はQuaternionEX::SquadSetup関数で作成する
		@param[in]	Qt0	正規化されたクオータニオン0
		@param[in]	Qt1	正規化されたクオータニオン1
		@param[in]	Qt2	正規化されたクオータニオン2
		@param[in]	Qt3	正規化されたクオータニオン3
		@param[in]	t	補間制御係数
		@return	作成したクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion Squad(const Quaternion& Qt0, const Quaternion& Qt1, const Quaternion& Qt2, const Quaternion& Qt3, float t) {
			return (Quaternion)XMQuaternionSquad(XMVECTOR(Qt0), XMVECTOR(Qt1), XMVECTOR(Qt2), XMVECTOR(Qt3), t);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	球状平方補間のためのセットアップを行う<br />
		pA、pB、pCはそれぞれSquad()、SquadV()関数のQt1、Qt2、Qt3に入力する
		@param[out]	pA	結果を受けるクオータニオンA
		@param[out]	pB	結果を受けるクオータニオンB
		@param[out]	pC	結果を受けるクオータニオンC
		@param[in]	Qt0	正規化されたクオータニオン0
		@param[in]	Qt1	正規化されたクオータニオン1
		@param[in]	Qt2	正規化されたクオータニオン2
		@param[in]	Qt3	正規化されたクオータニオン2
		@return	作成したクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  void SquadSetup(Quaternion* pA, Quaternion* pB, Quaternion* pC,
			const Quaternion& Qt0, const Quaternion& Qt1, const Quaternion& Qt2, const Quaternion& Qt3) {
			XMVECTOR A(*pA);
			XMVECTOR B(*pB);
			XMVECTOR C(*pC);
			XMQuaternionSquadSetup(&A, &B, &C,
				XMVECTOR(Qt0), XMVECTOR(Qt1), XMVECTOR(Qt2), XMVECTOR(Qt3));
			*pA = A;
			*pB = B;
			*pC = C;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	球状平方補間を使用して、4 つの単位クオータニオン間を補間する<br />
		Q1、Q2、Q3はQuaternionEX::SquadSetup関数で作成する
		@param[in]	Qt0	正規化されたクオータニオン0
		@param[in]	Qt1	正規化されたクオータニオン1
		@param[in]	Qt2	正規化されたクオータニオン2
		@param[in]	Qt3	正規化されたクオータニオン3
		@param[in]	Vec	補間制御係数（すべての要素を0.0fから1.0fの間の同じ値を指定）
		@return	作成したクオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion SquadV(const Quaternion& Qt0, const Quaternion& Qt1, const Quaternion& Qt2, const Quaternion& Qt3, const Vector4& Vec) {
			return (Quaternion)XMQuaternionSquadV(XMVECTOR(Qt0), XMVECTOR(Qt1), XMVECTOR(Qt2), XMVECTOR(Qt3), XMVECTOR(Vec));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンから、回転軸と回転角度を得る<br />
		pVecには回転軸ベクトル、pAngleには角度が設定される
		@param[out]	pVec	回転軸を得る３Ｄベクトル
		@param[out]	pAngle	回転角度を得る変数
		@param[in]	Qt	計算元のクオータニオン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		inline  void ToAxisAngle(Vector3* pVec, float* pAngle, const Quaternion& Qt) {
			XMVECTOR Vec(*pVec);
			XMQuaternionToAxisAngle(&Vec, pAngle, XMVECTOR(Qt));
			*pVec = Vec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンから、回転軸と回転角度を得る<br />
		rVecには回転軸ベクトル、rAngleには角度が設定される
		@param[out]	rVec	回転軸を得る３Ｄベクトル
		@param[out]	rAngle	回転角度を得る変数
		@param[in]	Qt	計算元のクオータニオン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		inline  void ToAxisAngle(Vector3& rVec, float& rAngle, const Quaternion& Qt) {
			XMVECTOR Vec(rVec);
			XMQuaternionToAxisAngle(&Vec, &rAngle, XMVECTOR(Qt));
			rVec = Vec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Lineに対してビルボード角度を設定したクオータニオンを得る
		@param[in]	Line	計算元となる線
		@return	クオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion Billboard(const Vector3& Line) {
			Vector3 Temp = Line;
			Matrix4X4 RotMatrix;
			Vector4 dammi(0, 0, 0, 0);
			Vector3 DefUp(0, 1.0f, 0);
			Vector2 TempVec2(Temp.x, Temp.z);
			if (TempVec2.Length() < 0.1f) {
				DefUp = Vector3(0, 0, 1.0f);
			}
			Temp.Normalize();
			RotMatrix.LookAtLH(Vector3(0, 0, 0), Temp, DefUp);
			RotMatrix.Inverse(&dammi);
			Quaternion Qt;
			Qt = RotMatrix.QtInMatrix();
			Qt.Normalize();
			return Qt;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Lineに対してフェイシング角度を設定したクオータニオンを得る
		@param[in]	Line	計算元となる線(オブジェクトからカメラ位置を引いたもの)
		@return	クオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion Faceing(const Vector3& Line) {
			Vector3 Temp = Line;
			Matrix4X4 RotMatrix;
			Vector4 dammi(0, 0, 0, 0);
			Vector3 DefUp(0, 1.0f, 0);
			Vector2 TempVec2(Temp.x, Temp.z);
			if (TempVec2.Length() < 0.1f) {
				DefUp = Vector3(0, 0, 1.0f);
			}
			RotMatrix.LookAtLH(Vector3(0, 0, 0), Temp, DefUp);
			RotMatrix.Inverse(&dammi);
			Quaternion Qt;
			Qt = RotMatrix.QtInMatrix();
			Qt.Normalize();
			return Qt;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Lineに対してY軸フェイシング角度を設定したクオータニオンを得る
		@param[in]	Line	計算元となる線(オブジェクトからカメラ位置を引いたもの)
		@return	クオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion FaceingY(const Vector3& Line) {
			Vector3 Temp = Line;
			Temp.Normalize();
			Quaternion Qt;
			Qt.Identity();
			Qt.RotationRollPitchYaw(0, atan2(Temp.x, Temp.z), 0);
			Qt.Normalize();
			return Qt;
		}
	}
	//end QuaternionEX


	//--------------------------------------------------------------------------------------
	///	Matrix4X4EX（4X4行列）ネームスペース
	//--------------------------------------------------------------------------------------
	namespace Matrix4X4EX {
		//--------------------------------------------------------------------------------------
		/*!
		@brief	2つの行列が等しいかどうかを検証する
		@param[in]	m1	比較元
		@param[in]	m2	比較先
		@return	等しければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool Equal(const Matrix4X4& m1, const Matrix4X4& m2){
			return (m1 == m2);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	2つの行列が等しくないかどうかを検証する
		@param[in]	m1	比較元
		@param[in]	m2	比較先
		@return	等しくなければtrue
		*/
		//--------------------------------------------------------------------------------------
		inline bool NotEqual(const Matrix4X4& m1, const Matrix4X4& m2){
			return (m1 != m2);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列内の位置情報を取得する(直接行列から取り出す)
		@param[in]	m	計算元となる行列
		@return	位置ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 PosInMatrixSt(const Matrix4X4& m) {
			Vector3 Vec;
			Vec.x = m._41;
			Vec.y = m._42;
			Vec.z = m._43;
			return Vec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列内の位置情報を取得する(XMMAth関数を使用)
		@param[in]	m	計算元となる行列
		@return	位置ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 PosInMatrix(const Matrix4X4& m) {
			Vector3 Vec(0, 0, 0);
			XMVECTOR Scale;
			XMVECTOR Qt;
			XMVECTOR Translation;
			if (XMMatrixDecompose(&Scale, &Qt, &Translation, XMMATRIX(m))) {
				Vec = Translation;
			}
			return Vec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列内のスケーリングを取得する
		@param[in]	m	計算元となる行列
		@return	スケーリングベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 ScaleInMatrix(const Matrix4X4& m) {
			Vector3 Vec(1.0f, 1.0f, 1.0f);
			XMVECTOR Scale;
			XMVECTOR Qt;
			XMVECTOR Translation;
			if (XMMatrixDecompose(&Scale, &Qt, &Translation, XMMATRIX(m))) {
				Vec = Scale;
			}
			return Vec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列内の回転を取得する
		@param[in]	m	計算元となる行列
		@return	回転クオータニオン
		*/
		//--------------------------------------------------------------------------------------
		inline  Quaternion QtInMatrix(const Matrix4X4& m) {
			Quaternion RetQt;
			XMVECTOR Scale;
			XMVECTOR Qt;
			XMVECTOR Translation;
			if (XMMatrixDecompose(&Scale, &Qt, &Translation, XMMATRIX(m))) {
				RetQt = Qt;
			}
			return RetQt;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列内のX軸回転を取得する
		@param[in]	m	計算元となる行列
		@return	X軸の回転ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 RotXInMatrix(const Matrix4X4& m) {
			Vector3 ret(m._11, m._12, m._13);
			Vector3 Scale = ScaleInMatrix(m);
			Matrix4X4::GetSafeScale(Scale);
			ret.x /= Scale.x;
			ret.y /= Scale.x;
			ret.z /= Scale.x;
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列内のY軸回転を取得する
		@param[in]	m	計算元となる行列
		@return	Y軸の回転ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 RotYInMatrix(const Matrix4X4& m) {
			Vector3 ret(m._21, m._22, m._23);
			Vector3 Scale = ScaleInMatrix(m);
			Matrix4X4::GetSafeScale(Scale);
			ret.x /= Scale.y;
			ret.y /= Scale.y;
			ret.z /= Scale.y;
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列内のZ軸回転を取得する
		@param[in]	m	計算元となる行列
		@return	Z軸の回転ベクトル
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector3 RotZInMatrix(const Matrix4X4& m) {
			Vector3 ret(m._31, m._32, m._33);
			Vector3 Scale = ScaleInMatrix(m);
			Matrix4X4::GetSafeScale(Scale);
			ret.x /= Scale.z;
			ret.y /= Scale.z;
			ret.z /= Scale.z;
			return ret;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	2つの行列を比べ移動のみの違いであればtrueを返す(全く同じ場合もtrue)
		@param[in]	m1	比較元
		@param[in]	m2	比較先
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		inline bool IsNotTranslationOnly(const Matrix4X4& m1, const Matrix4X4& m2){
			return m1.IsNotTranslationOnly(m2);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	アフィン変換（スケーリング、回転の中心座標、クオータニオン、平行移動）から行列を作成する
		@param[in]	Scaling	スケーリング
		@param[in]	RotOrigin	回転の中心
		@param[in]	RotationQuaternion	クオータニオン
		@param[in]	Translation	平行移動
		@return	作成した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 AffineTransformation(const Vector3& Scaling, const Vector3& RotOrigin,
			const Quaternion& RotationQuaternion, const Vector3& Translation) {
			return (Matrix4X4)XMMatrixAffineTransformation(XMVECTOR(Scaling), XMVECTOR(RotOrigin),
				XMVECTOR(RotationQuaternion), XMVECTOR(Translation));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	アフィン変換（スケーリング、回転の中心座標、回転値（ラジアン）、平行移動）から2D行列をを作成する
		@param[in]	Scaling	スケーリング
		@param[in]	RotOrigin	回転の中心
		@param[in]	Rotation	回転値（ラジアン）
		@param[in]	Translation	平行移動
		@return	作成した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 AffineTransformation2D(const Vector2& Scaling,
			const Vector2& RotOrigin, float Rotation, const Vector2& Translation) {
			return (Matrix4X4)XMMatrixAffineTransformation2D(XMVECTOR(Scaling), XMVECTOR(RotOrigin),
				Rotation, XMVECTOR(Translation));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列からスケーリング、回転、平行移動を取り出す
		@param[out]	rScaling	スケーリングを受け取る変数
		@param[out]	rQt	クオータニオンを受け取る変数
		@param[out]	rTranslation	平行移動を受け取る変数
		@param[in]	m	計算元となる行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		inline  void Decompose(Vector3& rScaling, Quaternion& rQt,
			Vector3& rTranslation, const Matrix4X4& m) {
			XMVECTOR Scale;
			XMVECTOR Qt;
			XMVECTOR Translation;
			if (XMMatrixDecompose(&Scale, &Qt, &Translation, XMMATRIX(m))) {
				rScaling = Scale;
				rQt = Qt;
				rTranslation = Translation;
			}
			else {
				throw BaseException(
					L"行列のデコンポーズに失敗しました",
					L"if(!XMMatrixDecompose(&Scale, &Qt, &Translation, XMMATRIX(m)))",
					L"Matrix4X4EX::Decompose()"
					);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列式を返す
		@param[in]	m	計算元となる行列
		@return	作成した行列式（Vector4の各要素に入る）
		*/
		//--------------------------------------------------------------------------------------
		inline  Vector4 Determinant(const Matrix4X4& m) {
			return (Vector4)XMMatrixDeterminant(XMMATRIX(m));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単位行列を返す
		@return	単位行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 Identity() {
			return (Matrix4X4)XMMatrixIdentity();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	逆行列を返す。pVecには行列式が入る
		@param[out]	pVec	行列式を受け取るベクトル
		@param[in]	m	計算元となる行列
		@return	逆行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 Inverse(Vector4* pVec, const Matrix4X4& m) {
			XMVECTOR Vec(*pVec);
			Matrix4X4 ret = (Matrix4X4)XMMatrixInverse(&Vec, XMMATRIX(m));
			*pVec = Vec;
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	逆行列を返す。pVecには行列式が入る
		@param[out]	rVec	行列式を受け取るベクトル
		@param[in]	m	計算元となる行列
		@return	逆行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 Inverse(Vector4& rVec, const Matrix4X4& m) {
			XMVECTOR Vec(rVec);
			Matrix4X4 ret = (Matrix4X4)XMMatrixInverse(&Vec, XMMATRIX(m));
			rVec = Vec;
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単位行列かどうかを検証する
		@param[in]	m	計算元となる行列
		@return	単位行列ならtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool IsIdentity(const Matrix4X4& m) {
			return XMMatrixIsIdentity(XMMATRIX(m));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列のいずれかの要素が正または負の無限大かどうかを検証する
		@param[in]	m	計算元となる行列
		@return	正または負の無限大ならtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool IsInfinite(const Matrix4X4& m) {
			return XMMatrixIsInfinite(XMMATRIX(m));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列のいずれかの要素がNaNかどうかを検証する
		@param[in]	m	計算元となる行列
		@return	NaNならtrue
		*/
		//--------------------------------------------------------------------------------------
		inline  bool IsNaN(const Matrix4X4& m) {
			return XMMatrixIsNaN(XMMATRIX(m));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	左手座標ビュー行列を作成する
		@param[in]	Eye	カメラ位置
		@param[in]	At	カメラ視点
		@param[in]	Up	カメラ傾き（通常<0,1,0>）
		@return	左手座標ビュー行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 LookAtLH(const Vector3& Eye, const Vector3& At, const Vector3& Up) {
			return (Matrix4X4)XMMatrixLookAtLH(
				XMVECTOR(Eye),
				XMVECTOR(At),
				XMVECTOR(Up));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	右手座標ビュー行列を作成する
		@param[in]	Eye	カメラ位置
		@param[in]	At	カメラ視点
		@param[in]	Up	カメラ傾き（通常<0,1,0>）
		@return	右手座標ビュー行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 LookAtRH(const Vector3& Eye, const Vector3& At, const Vector3& Up) {
			return (Matrix4X4)XMMatrixLookAtRH(
				XMVECTOR(Eye),
				XMVECTOR(At),
				XMVECTOR(Up));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カメラ位置、向き、傾きから左手座標ビュー行列を作成する
		@param[in]	Eye	カメラ位置
		@param[in]	EyeDirection	カメラ向き
		@param[in]	Up	カメラ傾き（通常<0,1,0>）
		@return	左手座標ビュー行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 LookToLH(const Vector3& Eye, const Vector3& EyeDirection, const Vector3& Up) {
			return (Matrix4X4)XMMatrixLookToLH(
				XMVECTOR(Eye),
				XMVECTOR(EyeDirection),
				XMVECTOR(Up));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カメラ位置、向き、傾きから右手座標ビュー行列を作成する
		@param[in]	Eye	カメラ位置
		@param[in]	EyeDirection	カメラ向き
		@param[in]	Up	カメラ傾き（通常<0,1,0>）
		@return	右手座標ビュー行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 LookToRH(const Vector3& Eye, const Vector3& EyeDirection, const Vector3& Up) {
			return (Matrix4X4)XMMatrixLookToRH(
				XMVECTOR(Eye),
				XMVECTOR(EyeDirection),
				XMVECTOR(Up));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列の積を返す
		@param[in]	m1	計算元となる行列
		@param[in]	m2	演算先になる行列
		@return	積の結果行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 Multiply(const Matrix4X4& m1, const Matrix4X4& m2) {
			return  (Matrix4X4)XMMatrixMultiply(XMMATRIX(m1), XMMATRIX(m2));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	積の転置行列を返す
		@param[in]	m1	計算元となる行列
		@param[in]	m2	演算先になる行列
		@return	積の転置行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 MultiplyTranspose(const Matrix4X4& m1, const Matrix4X4& m2) {
			return  (Matrix4X4)XMMatrixMultiplyTranspose(XMMATRIX(m1), XMMATRIX(m2));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	左手座標系の正射影行列を返す
		@param[in]	ViewWidth	幅
		@param[in]	ViewHeight	高さ
		@param[in]	NearZ	奥行手前
		@param[in]	FarZ	奥行奥
		@return	左手座標系の正射影行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 OrthographicLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ) {
			return  (Matrix4X4)XMMatrixOrthographicLH(ViewWidth, ViewHeight, NearZ, FarZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	右手座標系の正射影行列を返す
		@param[in]	ViewWidth	幅
		@param[in]	ViewHeight	高さ
		@param[in]	NearZ	奥行手前
		@param[in]	FarZ	奥行奥
		@return	右手座標系の正射影行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 OrthographicRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ) {
			return  (Matrix4X4)XMMatrixOrthographicRH(ViewWidth, ViewHeight, NearZ, FarZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	左手座標系のカスタム正射影行列を返す
		@param[in]	ViewLeft	左
		@param[in]	ViewRight	右
		@param[in]	ViewBottom	下
		@param[in]	ViewTop	上
		@param[in]	NearZ	奥行手前
		@param[in]	FarZ	奥行奥
		@return	左手座標系のカスタム正射影行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 OrthographicOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ) {
			return  (Matrix4X4)XMMatrixOrthographicOffCenterLH(
				ViewLeft, ViewRight, ViewBottom, ViewTop, NearZ, FarZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	右手座標系のカスタム正射影行列を返す
		@param[in]	ViewLeft	左
		@param[in]	ViewRight	右
		@param[in]	ViewBottom	下
		@param[in]	ViewTop	上
		@param[in]	NearZ	奥行手前
		@param[in]	FarZ	奥行奥
		@return	右手座標系のカスタム正射影行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 OrthographicOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop, float NearZ, float FarZ) {
			return  (Matrix4X4)XMMatrixOrthographicOffCenterRH(
				ViewLeft, ViewRight, ViewBottom, ViewTop, NearZ, FarZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	視野に基づいて、左手座標系のパースペクティブ射影行列を返す
		@param[in]	FovAngleY	トップダウン視野角度
		@param[in]	AspectHByW	ビュー空間の高さと幅のアスペクト比
		@param[in]	NearZ	奥行手前
		@param[in]	FarZ	奥行奥
		@return	左手座標系のパースペクティブ射影行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 PerspectiveFovLH(float FovAngleY, float AspectHByW, float NearZ, float FarZ) {
			return  (Matrix4X4)XMMatrixPerspectiveFovLH(FovAngleY, AspectHByW, NearZ, FarZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	視野に基づいて、右手座標系のパースペクティブ射影行列を返す
		@param[in]	FovAngleY	トップダウン視野角度
		@param[in]	AspectHByW	ビュー空間の高さと幅のアスペクト比
		@param[in]	NearZ	奥行手前
		@param[in]	FarZ	奥行奥
		@return	右手座標系のパースペクティブ射影行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 PerspectiveFovRH(float FovAngleY, float AspectHByW, float NearZ, float FarZ) {
			return  (Matrix4X4)XMMatrixPerspectiveFovRH(FovAngleY, AspectHByW, NearZ, FarZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	左手座標系のパースペクティブ射影行列を返す
		@param[in]	ViewWidth	近くのクリップ面の視錐台の幅
		@param[in]	ViewHeight	近くのクリップ面の視錐台の高さ
		@param[in]	NearZ	奥行手前
		@param[in]	FarZ	奥行奥
		@return	左手座標系のパースペクティブ射影行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 PerspectiveLH(float ViewWidth, float ViewHeight, float NearZ, float FarZ) {
			return  (Matrix4X4)XMMatrixPerspectiveLH(ViewWidth, ViewHeight, NearZ, FarZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	右手座標系のパースペクティブ射影行列を返す
		@param[in]	ViewWidth	近くのクリップ面の視錐台の幅
		@param[in]	ViewHeight	近くのクリップ面の視錐台の高さ
		@param[in]	NearZ	奥行手前
		@param[in]	FarZ	奥行奥
		@return	右手座標系のパースペクティブ射影行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 PerspectiveRH(float ViewWidth, float ViewHeight, float NearZ, float FarZ) {
			return  (Matrix4X4)XMMatrixPerspectiveRH(ViewWidth, ViewHeight, NearZ, FarZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カスタム バージョンの左手座標系パースペクティブ射影行列を返す
		@param[in]	ViewLeft	クリップ視錐台の左側のx座標
		@param[in]	ViewRight	クリップ視錐台の右側のx座標
		@param[in]	ViewBottom	クリップ視錐台の下側のy座標
		@param[in]	ViewTop	クリップ視錐台の上側のy座標
		@param[in]	NearZ	奥行手前
		@param[in]	FarZ	奥行奥
		@return	カスタム バージョンの左手座標系パースペクティブ射影行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 PerspectiveOffCenterLH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop,
			float NearZ, float FarZ) {
			return  (Matrix4X4)XMMatrixPerspectiveOffCenterLH(ViewLeft, ViewRight, ViewBottom, ViewTop, NearZ, FarZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カスタム バージョンの右手座標系パースペクティブ射影行列を返す
		@param[in]	ViewLeft	クリップ視錐台の左側のx座標
		@param[in]	ViewRight	クリップ視錐台の右側のx座標
		@param[in]	ViewBottom	クリップ視錐台の下側のy座標
		@param[in]	ViewTop	クリップ視錐台の上側のy座標
		@param[in]	NearZ	奥行手前
		@param[in]	FarZ	奥行奥
		@return	カスタム バージョンの右手座標系パースペクティブ射影行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 PerspectiveOffCenterRH(float ViewLeft, float ViewRight, float ViewBottom, float ViewTop,
			float NearZ, float FarZ) {
			return  (Matrix4X4)XMMatrixPerspectiveOffCenterRH(ViewLeft, ViewRight, ViewBottom, ViewTop, NearZ, FarZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定された平面を介してベクトルを反映するトランスフォーム行列を作成し返す
		@param[in]	Pln	平面
		@return	ベクトルを反映するトランスフォーム行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 Reflect(const Plane& Pln) {
			return  (Matrix4X4)XMMatrixReflect(XMVECTOR(Pln));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	任意の軸を回転軸にして、回転した行列を作成し返す
		@param[in]	Axis	回転軸を表すベクトル
		@param[in]	Angle	回転角度（ラジアン）
		@return	任意の軸を回転軸にして、回転した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 RotationAxis(const Vector3& Axis, float Angle) {
			return  (Matrix4X4)XMMatrixRotationAxis(XMVECTOR(Axis), Angle);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	任意の法線ベクトルを回転軸にして、回転した行列を作成し返す
		@param[in]	NormalAxis	回転軸を記述する法線ベクトル
		@param[in]	Angle	回転角度（ラジアン）
		@return	任意の法線ベクトルを回転軸にして、回転した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 RotationNormal(const Vector3& NormalAxis, float Angle) {
			return  (Matrix4X4)XMMatrixRotationNormal(XMVECTOR(NormalAxis), Angle);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クオータニオンから回転行列を作成し返す
		@param[in]	Qt	回転を記述するクオータニオン
		@return	クオータニオンから作成した回転行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 RotationQuaternion(const Quaternion& Qt) {
			return  (Matrix4X4)XMMatrixRotationQuaternion(XMVECTOR(Qt));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オイラー角に基づいて回転行列を作成する。回転はRollPitchYaw（Z、X、Y）の順で行われる
		@param[in]	Pitch	X軸回転
		@param[in]	Yaw		Y軸回転
		@param[in]	Roll	Z軸回転
		@return	オイラー角に基づいて回転した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 RotationRollPitchYaw(float Pitch, float Yaw, float Roll) {
			return  (Matrix4X4)XMMatrixRotationRollPitchYaw(Pitch, Yaw, Roll);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オイラー角(ベクトル)に基づいて回転行列を作成する。回転はRollPitchYaw（Z、X、Y）の順で行われる
		@param[in]	Angles	回転を記述するベクトル
		@return	オイラー角(ベクトル)に基づいて回転した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 RotationRollPitchYawFromVector(const Vector3& Angles) {
			return  (Matrix4X4)XMMatrixRotationRollPitchYawFromVector(XMVECTOR(Angles));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	X軸回転に基づいて回転行列を作成する
		@param[in]	Angle	X軸回転
		@return	X軸回転に基づいて回転した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 RotationX(float Angle) {
			return  (Matrix4X4)XMMatrixRotationX(Angle);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Y軸回転に基づいて回転行列を作成する
		@param[in]	Angle	Y軸回転
		@return	Y軸回転に基づいて回転した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 RotationY(float Angle) {
			return  (Matrix4X4)XMMatrixRotationY(Angle);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Z軸回転に基づいて回転行列を作成する
		@param[in]	Angle	Z軸回転
		@return	Z軸回転に基づいて回転した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 RotationZ(float Angle) {
			return  (Matrix4X4)XMMatrixRotationZ(Angle);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	x軸、y軸、およびz軸に沿ってスケーリングした行列を作成する
		@param[in]	ScaleX	Xに沿ったスケーリング
		@param[in]	ScaleY	Yに沿ったスケーリング
		@param[in]	ScaleZ	Zに沿ったスケーリング
		@return	x軸、y軸、およびz軸に沿ってスケーリングした行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 Scaling(float ScaleX, float ScaleY, float ScaleZ) {
			return  (Matrix4X4)XMMatrixScaling(ScaleX, ScaleY, ScaleZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルに沿ってスケーリングした行列を作成する
		@param[in]	Scale	スケーリングを記述するベクトル
		@return	ベクトルに沿ってスケーリングした行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 ScalingFromVector(const Vector3& Scale) {
			return  (Matrix4X4)XMMatrixScalingFromVector(XMVECTOR(Scale));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	各値を設定した行列を作成する
		@param[in]	m00	設定元00
		@param[in]	m01	設定元01
		@param[in]	m02	設定元02
		@param[in]	m03	設定元03
		@param[in]	m10	設定元10
		@param[in]	m11	設定元11
		@param[in]	m12	設定元12
		@param[in]	m13	設定元13
		@param[in]	m20	設定元20
		@param[in]	m21	設定元21
		@param[in]	m22	設定元22
		@param[in]	m23	設定元23
		@param[in]	m30	設定元30
		@param[in]	m31	設定元31
		@param[in]	m32	設定元32
		@param[in]	m33	設定元33
		@return	各値を設定した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 Set(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
			) {
			return  (Matrix4X4)XMMatrixSet(
				m00, m01, m02, m03,
				m10, m11, m12, m13,
				m20, m21, m22, m23,
				m30, m31, m32, m33
				);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ジオメトリを平面に射影するトランスフォーム行列を作成する。<br />
		ライトのw要素は、0.0でディレクショナルライト、1.0でポイントライトとなる
		@param[in]	ShadowPlane	基準面
		@param[in]	LightPosition	ライトの位置
		@return	ジオメトリを平面に射影するトランスフォーム行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 Shadow(const Plane& ShadowPlane, const Vector4& LightPosition) {
			return  (Matrix4X4)XMMatrixShadow(XMVECTOR(ShadowPlane), XMVECTOR(LightPosition));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	トランスフォーム行列を作成する。
		@param[in]	ScalingOrigin	スケーリングの中心を記述する3Dベクトル
		@param[in]	ScalingOrientationQuaternion	スケーリングの向きを記述するクオータニオン
		@param[in]	Scaling		x軸、y軸、z軸のスケーリング係数を含む3Dベクトル
		@param[in]	RotationOrigin	回転の中心を記述する3Dベクトル
		@param[in]	RotationQuaternion	RotationOriginによって示される原点を中心とする回転を記述するクオータニオン
		@param[in]	Translation		x軸、y軸、z軸に沿った平行移動を記述する3Dベクトル
		@return	トランスフォーム行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 Transformation(const Vector3& ScalingOrigin, const Quaternion& ScalingOrientationQuaternion, const Vector3& Scaling,
			const Vector3& RotationOrigin, const Quaternion& RotationQuaternion,
			const Vector3& Translation) {
			return  (Matrix4X4)XMMatrixTransformation(
				XMVECTOR(ScalingOrigin), XMVECTOR(ScalingOrientationQuaternion), XMVECTOR(Scaling),
				XMVECTOR(RotationOrigin), XMVECTOR(RotationQuaternion),
				XMVECTOR(Translation)
				);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	2Dトランスフォーム行列をxy平面に作成する。
		@param[in]	ScalingOrigin	スケーリングの中心を記述する2Dベクトル
		@param[in]	ScalingOrientation	スケーリング回転係数
		@param[in]	Scaling		x軸、y軸のスケーリング係数を含む2Dベクトル
		@param[in]	RotationOrigin	回転の中心を記述する2Dベクトル
		@param[in]	Rotation	回転の角度 (ラジアン単位)
		@param[in]	Translation		平行移動を記述する2Dベクトル
		@return	xy平面の2Dトランスフォーム行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 Transformation2D(const Vector2& ScalingOrigin, float ScalingOrientation, const Vector2& Scaling,
			const Vector2& RotationOrigin, float Rotation,
			const Vector2& Translation) {
			return  (Matrix4X4)XMMatrixTransformation2D(XMVECTOR(ScalingOrigin), ScalingOrientation, XMVECTOR(Scaling),
				XMVECTOR(RotationOrigin), Rotation, XMVECTOR(Translation));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定されたオフセットから平行移動行列を作成する
		@param[in]	OffsetX	X軸に沿った平行移動
		@param[in]	OffsetY	Y軸に沿った平行移動
		@param[in]	OffsetZ	Z軸に沿った平行移動
		@return	指定されたオフセットで平行移動した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 Translation(float OffsetX, float OffsetY, float OffsetZ) {
			return  (Matrix4X4)XMMatrixTranslation(OffsetX, OffsetY, OffsetZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルから平行移動行列を作成する
		@param[in]	Offset	x軸、y軸、z軸に沿った平行移動を記述する3Dベクトル
		@return	ベクトルから平行移動した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 TranslationFromVector(const Vector3& Offset) {
			return  (Matrix4X4)XMMatrixTranslationFromVector(XMVECTOR(Offset));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列を転置する
		@param[in]	m	計算元となる行列
		@return	転置した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 Transpose(const Matrix4X4& m) {
			return  (Matrix4X4)XMMatrixTranspose(XMMATRIX(m));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スケーリング、回転、移動から行列を作成する<br />
		回転位置を中心に置いたアフィン変換である。
		@param[in]	Scaling	x軸、y軸、z軸のスケーリング係数を含む3Dベクトル
		@param[in]	RotationQt	回転を記述するクオータニオン
		@param[in]	Translation	x軸、y軸、z軸に沿った平行移動を記述する3Dベクトル
		@return	作成した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 DefTransformation(const Vector3& Scaling, const Quaternion& RotationQt, const Vector3& Translation) {
			return AffineTransformation(Scaling, Vector3(0, 0, 0), RotationQt, Translation);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スケーリング、回転ベクトル、移動から行列を作成する<br />
		回転位置を中心に置いたアフィン変換である。
		@param[in]	Scaling	x軸、y軸、z軸のスケーリング係数を含む3Dベクトル
		@param[in]	Rotation	回転を記述するベクトル
		@param[in]	Translation	x軸、y軸、z軸に沿った平行移動を記述する3Dベクトル
		@return	作成した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 DefTransformation(const Vector3& Scaling, const Vector3& Rotation, const Vector3& Translation) {
			Quaternion Qt;
			Qt.RotationRollPitchYawFromVector(Rotation);
			Qt.Normalize();
			return AffineTransformation(Scaling, Vector3(0, 0, 0), Qt, Translation);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スケーリング、移動、回転から行列を作成する。回転より移動を先に行う。
		@param[in]	Scaling	x軸、y軸、z軸のスケーリング係数を含む3Dベクトル
		@param[in]	Translation	x軸、y軸、z軸に沿った平行移動を記述する3Dベクトル
		@param[in]	RotationQt	回転を記述するクオータニオン
		@return	作成した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 STRTransformation(const Vector3& Scaling, const Vector3& Translation, const Quaternion& RotationQt) {
			Matrix4X4 Scale = ScalingFromVector(Scaling);
			Matrix4X4 Trans = TranslationFromVector(Translation);
			Matrix4X4 Qt = RotationQuaternion(RotationQt);
			Matrix4X4 ret = Scale * Trans * Qt;
			return  ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スケーリング、移動、回転ベクトルから行列を作成する。回転より移動を先に行う。
		@param[in]	Scaling	x軸、y軸、z軸のスケーリング係数を含む3Dベクトル
		@param[in]	Translation	x軸、y軸、z軸に沿った平行移動を記述する3Dベクトル
		@param[in]	Rotation	回転を記述するベクトル
		@return	作成した行列
		*/
		//--------------------------------------------------------------------------------------
		inline  Matrix4X4 STRTransformation(const Vector3& Scaling, const Vector3& Translation, const Vector3& Rotation) {
			Matrix4X4 Scale = ScalingFromVector(Scaling);
			Matrix4X4 Trans = TranslationFromVector(Translation);
			Matrix4X4 Rot = RotationRollPitchYawFromVector(Rotation);
			Matrix4X4 ret = Scale * Trans * Rot;
			return  ret;
		}
	};
	//end Matrix4X4EX


}
//end of namespace basecross.

