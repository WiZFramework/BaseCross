/*!
@file TransHelper.h
@brief 衝突判定、補間処理用ユーティリティ
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	Lerp 補間処理計算構造体
	//--------------------------------------------------------------------------------------
	struct Lerp{
		enum rate{
			Linear,	//1次関数（線形）
			Easein,	//2次関数ゆっくり上昇
			EaseOut,//2次関数最後がゆっくり
			Cube,	//3次関数
			Cos		//コサイン
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief	T型の補間計算を行う
		@tparam	T	計算する型
		@param[in]	Start	開始値
		@param[in]	End	終了値
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[in]	NowTime	現在の時間
		@param[in]	r	計算方法
		@return	T型の計算結果
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static T CalculateLerp(const T &Start, const T &End,
		float StartTime, float EndTime, float NowTime, rate r){
			float t = (NowTime - StartTime) / (EndTime - StartTime);
			if (t < 0.0f){
				t = 0;
			}
			if (t > 1.0f){
				t = 1.0f;
			}
			float cal_rate = t;   // 1次関数補間値に変換(デフォルト)
			switch (r){
			case Linear:
				cal_rate = t;
				break;
			case Easein:
				cal_rate = t * t;
				break;
			case EaseOut:
				cal_rate = t * (2.0f - t);
				break;
			case Cube:
				cal_rate = t * t * (3.0f - 2.0f * t);
				break;
			case Cos:
				cal_rate = (1.0f - cos(t * XM_PI)) / 2.0f;
				break;
			default:
				break;
			}
			return Start * (1.0f - cal_rate) + End * cal_rate;
		}
	};

	//--------------------------------------------------------------------------------------
	///	XZポイント(Z方向プラスの点)
	//--------------------------------------------------------------------------------------
	struct POINT_XZ {
		float m_X;
		float m_Z;
		POINT_XZ() :
			m_X(0.0f), m_Z(0.0f)
		{}
		POINT_XZ(float x,float z) :
			m_X(x), m_Z(z)
		{}
		POINT_XZ(const POINT_XZ& other) :
			m_X(other.m_X),
			m_Z(other.m_Z)
		{}
		POINT_XZ(const bsm::Vec2& other) :
			m_X(other.x),
			m_Z(other.y)
		{}
		POINT_XZ(const bsm::Vec3& other) :
			m_X(other.x),
			m_Z(other.z)
		{}
		operator bsm::Vec2() {
			return bsm::Vec2(m_X,m_Z);
		}
		POINT_XZ& operator=(const POINT_XZ& other) {
			//自己代入の抑制
			if (this != &other) {
				m_X = other.m_X;
				m_Z = other.m_Z;
			}
			return *this;
		}
		POINT_XZ& operator=(const bsm::Vec2& other) {
			m_X = other.x;
			m_Z = other.y;
			return *this;
		}
		POINT_XZ& operator=(const bsm::Vec3& other) {
			m_X = other.x;
			m_Z = other.z;
			return *this;
		}
	};

	//--------------------------------------------------------------------------------------
	///	XZ方向のセルに使用するインデックス
	//--------------------------------------------------------------------------------------
	struct CellIndex {
		int x;
		int z;
		CellIndex() :
			//無効値で初期化
			x(-1), z(-1)
		{}
		CellIndex(int xx, int zz) :
			x(xx), z(zz)
		{
		}
		bool operator==(const CellIndex& other)const {
			return (x == other.x && z == other.z);
		}
		bool operator!=(const CellIndex& other)const {
			return ((x != other.x) || (z != other.z));
		}
		static int GetHeuristic(const CellIndex& Cell1, const CellIndex& Cell2) {
			int spanX = abs(Cell1.x - Cell2.x);
			int spanZ = abs(Cell1.z - Cell2.z);
			//どちらか長いほうを返す
			return (spanX >= spanZ) ? spanX : spanZ;
		}
	};


	//--------------------------------------------------------------------------------------
	///	XZ矩形(Z方向プラスの矩形)
	//--------------------------------------------------------------------------------------
	struct RECT_XZ {
		float m_Left;
		float m_Near;
		float m_Right;
		float m_Far;
		RECT_XZ() :
			m_Left(0.0f), m_Near(0.0f),
			m_Right(0.0f), m_Far(0.0f)
		{}
		RECT_XZ(float l,float n,float r,float f) :
			m_Left(l), m_Near(n),
			m_Right(r), m_Far(f)
		{}

		POINT_XZ CenterPoint() const{
			return POINT_XZ((m_Left + m_Right) / 2.0f, (m_Near + m_Far) / 2.0f);
		}
		void operator+=(const POINT_XZ& point){
			m_Left += point.m_X;
			m_Right += point.m_X;
			m_Near += point.m_Z;
			m_Far += point.m_Z;
		}
		RECT_XZ operator+(const POINT_XZ& point) const {
			RECT_XZ rect(*this);
			rect += point;
			return rect;
		}
		void operator-=(const POINT_XZ& point) {
			m_Left -= point.m_X;
			m_Right -= point.m_X;
			m_Near -= point.m_Z;
			m_Far -= point.m_Z;
		}
		RECT_XZ operator-(const POINT_XZ& point) const {
			RECT_XZ rect(*this);
			rect -= point;
			return rect;
		}
		bool IsRectEmpty()const{
			if (m_Left >= m_Right || m_Near >= m_Far) {
				return true;
			}
			return false;
		}
		bool PtInRect(const POINT_XZ& point) const{
			if (point.m_X >= m_Left && point.m_X < m_Right
				&& point.m_Z >= m_Near && point.m_Z < m_Far) {
				return true;
			}
			return false;
		}
		bool PtInRect(const bsm::Vec2& point) const {
			POINT_XZ p(point);
			return PtInRect(p);
		}
		bool Intersect(const RECT_XZ& rect2) const{
			if (IsRectEmpty() || rect2.IsRectEmpty()) {
				//どちらかが空の場合はfalse
				return false;
			}
			if (m_Left > rect2.m_Right) {
				return false;
			}
			if (m_Right < rect2.m_Left) {
				return false;
			}
			if (m_Near > rect2.m_Far) {
				return false;
			}
			if (m_Far < rect2.m_Near) {
				return false;
			}
			return true;
		}


		RECT_XZ UnionRect(const RECT_XZ& rect1, const RECT_XZ& rect2) const{
			RECT_XZ ret;
			ret.m_Left = (rect1.m_Left <= rect2.m_Left) ? rect1.m_Left : rect2.m_Left;
			ret.m_Right = (rect1.m_Right >= rect2.m_Right) ? rect1.m_Right : rect2.m_Right;
			ret.m_Near = (rect1.m_Near <= rect2.m_Near) ? rect1.m_Near : rect2.m_Near;
			ret.m_Far = (rect1.m_Far >= rect2.m_Far) ? rect1.m_Far : rect2.m_Far;
			return ret;
		}
		RECT_XZ UnionRect(const RECT_XZ& other){
			RECT_XZ ret;
			ret.m_Left = (m_Left <= other.m_Left) ? m_Left : other.m_Left;
			ret.m_Right = (m_Right >= other.m_Right) ? m_Right : other.m_Right;
			ret.m_Near = (m_Near <= other.m_Near) ? m_Near : other.m_Near;
			ret.m_Far = (m_Far >= other.m_Far) ? m_Far : other.m_Far;
			*this = ret;
			return ret;
		}
		RECT_XZ operator|=(const RECT_XZ& other) {
			return UnionRect(other);
		}
		RECT_XZ operator|(const RECT_XZ& other) const{
			return UnionRect(*this,other);
		}
		float Width()const{
			return abs(m_Right - m_Left);
		}
		float Dipth()const{
			return abs(m_Far - m_Near);
		}
	};

	struct SPHERE;

	//--------------------------------------------------------------------------------------
	///	AABBボリューム境界
	//--------------------------------------------------------------------------------------
	struct AABB {
		bsm::Vec3 m_Min;     ///< 小さいほうの座標
		bsm::Vec3 m_Max;     ///< 大きいほうの座標
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		AABB():
			m_Min(0,0,0),
			m_Max(0, 0, 0){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Min	小さいほうの座標
		@param[in]	Max	大きいほうの座標
		*/
		//--------------------------------------------------------------------------------------
		AABB(const bsm::Vec3& Min, const bsm::Vec3& Max)
			:m_Min(Min), m_Max(Max) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Center	中心
		@param[in]	XSize	X方向サイズ
		@param[in]	YSize	Y方向サイズ
		@param[in]	ZSize	Z方向サイズ
		*/
		//--------------------------------------------------------------------------------------
		AABB(const bsm::Vec3& Center, float XSize, float YSize, float ZSize) {
			float xh = XSize / 2.0f;
			float yh = YSize / 2.0f;
			float zh = ZSize / 2.0f;

			m_Min.x = Center.x - xh;
			m_Max.x = Center.x + xh;

			m_Min.y = Center.y - yh;
			m_Max.y = Center.y + yh;

			m_Min.z = Center.z - zh;
			m_Max.z = Center.z + zh;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心を得る
		@param[out]	Pos	中心が代入されるベクトル
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetCenter(bsm::Vec3& Pos)const {
			Pos.x = (m_Min.x + m_Max.x) / 2.0f;
			Pos.y = (m_Min.y + m_Max.y) / 2.0f;
			Pos.z = (m_Min.z + m_Max.z) / 2.0f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心を得る
		@return	中心のベクトル
		*/
		//--------------------------------------------------------------------------------------
		bsm::Vec3 GetCenter() const{
			bsm::Vec3 ret;
			ret.x = (m_Min.x + m_Max.x) / 2.0f;
			ret.y = (m_Min.y + m_Max.y) / 2.0f;
			ret.z = (m_Min.z + m_Max.z) / 2.0f;
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	移動する
		@param[in]	MoveVec	移動ベクトル
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Move(const bsm::Vec3& MoveVec) {
			m_Min += MoveVec;
			m_Max += MoveVec;
		}
		bool PtInAABB(const bsm::Vec3& point) {
			if (
				point.x >= m_Min.x && point.x < m_Max.x
				&&
				point.y >= m_Min.y && point.y < m_Max.y
				&&
				point.z >= m_Min.z && point.z < m_Max.z
				) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	包み込むXZ矩形を得る
		@return	XZ矩形
		*/
		//--------------------------------------------------------------------------------------
		RECT_XZ GetWrappedRECT_XZ() const {
			return RECT_XZ(m_Min.x, m_Min.z, m_Max.x, m_Max.z);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	包み込むSPHEREを得る
		@return	SPHERE
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetWrappedSPHERE() const;

	};



	//--------------------------------------------------------------------------------------
	///	球ボリューム境界
	//--------------------------------------------------------------------------------------
	struct SPHERE {
		bsm::Vec3 m_Center;	///< 中心点の座標
		float m_Radius;			///< 半径
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		SPHERE() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Center	中心
		@param[in]	Radius	半径
		*/
		//--------------------------------------------------------------------------------------
		SPHERE(const bsm::Vec3& Center, float Radius) :
			m_Center(Center),
			m_Radius(Radius) {}
		AABB GetWrappedAABB() const {
			AABB ret;
			ret.m_Min = bsm::Vec3(m_Center.x - m_Radius, m_Center.y - m_Radius, m_Center.z - m_Radius);
			ret.m_Max = bsm::Vec3(m_Center.x + m_Radius, m_Center.y + m_Radius, m_Center.z + m_Radius);
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	包み込むXZ矩形を得る
		@return	XZ矩形
		*/
		//--------------------------------------------------------------------------------------
		RECT_XZ GetWrappedRECT_XZ() const {
			RECT_XZ ret;
			ret.m_Left = m_Center.x - m_Radius;
			ret.m_Near = m_Center.z - m_Radius;
			ret.m_Right = m_Center.x + m_Radius;
			ret.m_Far = m_Center.z + m_Radius;
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	包み込むSPHEREを得る
		@return	SPHERE
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetWrappedSPHERE() const {
			return *this;
		}

	};

	inline SPHERE AABB::GetWrappedSPHERE() const {
		SPHERE sp;
		auto Size = m_Max - m_Min;
		Size *= 0.5f;
		sp.m_Radius = bsm::length(Size);
		sp.m_Center = GetCenter();
		return sp;
	}



	//--------------------------------------------------------------------------------------
	///	OBBボリューム境界
	//--------------------------------------------------------------------------------------
	struct OBB{
		bsm::Vec3 m_Center;     ///< 中心点の座標
		bsm::Vec3 m_Rot[3];  ///< XYZ の各座標軸の傾きを表す方向ベクトル
		bsm::Vec3 m_Size;     ///< OBB の各座標軸に沿った長さの半分（中心点から面までの長さ）
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成されたサイズと変換行列から、OBBを作り直す
		@param[in]	Size	作成サイズ
		@param[in]	Matrix	変換行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CreateOBB(const bsm::Vec3& Size,const bsm::Mat4x4& Matrix){
			m_Center.x = Matrix._41;
			m_Center.y = Matrix._42;
			m_Center.z = Matrix._43;
			bsm::Vec3 VecX(Matrix._11,Matrix._12,Matrix._13);
			bsm::Vec3 VecY(Matrix._21,Matrix._22,Matrix._23);
			bsm::Vec3 VecZ(Matrix._31,Matrix._32,Matrix._33);
			m_Size = bsm::Vec3(
				Size.x * bsm::length(VecX),
				Size.y * bsm::length(VecY),
				Size.z * bsm::length(VecZ)
				);
			m_Size *= 0.5f;
			//回転を得る
			m_Rot[0] = bsm::normalize(VecX);
			m_Rot[1] = bsm::normalize(VecY);
			m_Rot[2] = bsm::normalize(VecZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		OBB(){
			//デフォルトは1辺1.0のOBB
			bsm::Vec3 SizeVec(1.0f,1.0f,1.0f);
			//行列はアイデンティティ
			bsm::Mat4x4 Matrix;
			CreateOBB(SizeVec,Matrix);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Size	サイズ
		@param[in]	Rot	回転ベクトル
		@param[in]	Center	中心位置
		*/
		//--------------------------------------------------------------------------------------
		OBB(const bsm::Vec3& Size,const bsm::Vec3& Rot,const bsm::Vec3& Center):
			m_Center(Center),
			m_Size(Size)
		{
			m_Size *= 0.5f;
			//回転を得る
			bsm::Vec3 LocaRot = Rot;
			LocaRot.normalize();
			bsm::Mat4x4 mRot(XMMatrixRotationRollPitchYawFromVector(LocaRot));
			m_Rot[0] = bsm::Vec3(mRot._11,mRot._12,mRot._13);
			m_Rot[1] = bsm::Vec3(mRot._21,mRot._22,mRot._23);
			m_Rot[2] = bsm::Vec3(mRot._31,mRot._32,mRot._33);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Size	サイズ
		@param[in]	Qt	回転クオータニオン
		@param[in]	Center	中心位置
		*/
		//--------------------------------------------------------------------------------------
		OBB(const bsm::Vec3& Size,const bsm::Quat& Qt,const bsm::Vec3& Center):
			m_Center(Center),
			m_Size(Size)
		{
			m_Size *= 0.5f;
			//回転を得る
			bsm::Quat LocalQt = Qt;
			//クオータニオンを正規化
			LocalQt.normalize();
			//クオータニオンから回転行列を作成
			bsm::Mat4x4 mRot(XMMatrixRotationQuaternion(LocalQt));
			m_Rot[0] = bsm::Vec3(mRot._11,mRot._12,mRot._13);
			m_Rot[1] = bsm::Vec3(mRot._21,mRot._22,mRot._23);
			m_Rot[2] = bsm::Vec3(mRot._31,mRot._32,mRot._33);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ（作成されたサイズと変換行列から、OBBを作成）
		@param[in]	Size	サイズ
		@param[in]	Matrix	変換行列
		*/
		//--------------------------------------------------------------------------------------
		OBB(const bsm::Vec3& Size,const bsm::Mat4x4& Matrix)
		{
			m_Center.x = Matrix._41;
			m_Center.y = Matrix._42;
			m_Center.z = Matrix._43;
			bsm::Vec3 VecX(Matrix._11,Matrix._12,Matrix._13);
			bsm::Vec3 VecY(Matrix._21,Matrix._22,Matrix._23);
			bsm::Vec3 VecZ(Matrix._31,Matrix._32,Matrix._33);
			m_Size = bsm::Vec3(
				Size.x * bsm::length(VecX),
				Size.y * bsm::length(VecY),
				Size.z * bsm::length(VecZ)
				);
			m_Size *= 0.5f;
			//回転を得る
			m_Rot[0] = bsm::normalize(VecX);
			m_Rot[1] = bsm::normalize(VecY);
			m_Rot[2] = bsm::normalize(VecZ);

		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	回転行列を得る
		@return	回転行列
		*/
		//--------------------------------------------------------------------------------------
		bsm::Mat4x4 GetRotMatrix()const{
			bsm::Mat4x4 ret;
			ret._11 = m_Rot[0].x;
			ret._12 = m_Rot[0].y;
			ret._13 = m_Rot[0].z;
			ret._21 = m_Rot[1].x;
			ret._22 = m_Rot[1].y;
			ret._23 = m_Rot[1].z;
			ret._31 = m_Rot[2].x;
			ret._32 = m_Rot[2].y;
			ret._33 = m_Rot[2].z;
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行列を得る
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
		bsm::Mat4x4 GetWorldMatrix()const {
			bsm::Mat4x4 ret,Pos;
			ret = XMMatrixScaling(m_Size.x * 2.0f, m_Size.y * 2.0f, m_Size.z * 2.0f);
			ret = XMMatrixMultiply(ret, GetRotMatrix());
			Pos = XMMatrixTranslation(m_Center.x, m_Center.y, m_Center.z);
			ret = XMMatrixMultiply(ret, Pos);
			return ret;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	一番大きい辺の半分長さを得る
		@return	一番大きい辺の半分の長さ
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxHalfSize()const {
			float ret = m_Size.x;
			if (ret < m_Size.y) {
				ret = m_Size.y;
			}
			if (ret < m_Size.z) {
				ret = m_Size.z;
			}
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	一番大きい辺の長さを得る
		@return	一番大きい辺の長さ
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxSize()const {
			return GetMaxHalfSize() * 2.0f;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	一番小さい辺の長さ半分を得る
		@return	一番小さい辺の長さの半分
		*/
		//--------------------------------------------------------------------------------------
		float GetMinHalfSize() const {
			float ret = m_Size.x;
			if (ret > m_Size.y) {
				ret = m_Size.y;
			}
			if (ret > m_Size.z) {
				ret = m_Size.z;
			}
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	一番小さい辺の長さを得る
		@return	一番小さい辺の長さ
		*/
		//--------------------------------------------------------------------------------------
		float GetMinSize() const {
			return  GetMinHalfSize() * 2.0f;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	ベクトルに一番近い回転を得る
		@param[in]	Check	チェックするベクトル
		@param[in]	Epsilon	誤差
		@param[out]	Ret	戻されるベクトル
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetNearNormalRot(const bsm::Vec3& Check, float Epsilon,bsm::Vec3& Ret){
			for (int i = 0; i < 3; i++){
				bsm::Vec4 VecEp(Epsilon);
				if (XMVector3NearEqual(Check, m_Rot[i], VecEp)) {
					Ret = m_Rot[i];
					return;
				}
				if (XMVector3NearEqual(Check, -m_Rot[i], VecEp)) {
					Ret = -m_Rot[i];
					return;
				}
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBBが回転してないかどうかを調べる（AABBとして使えるかどうかを調べられる）
		@return	回転してなければtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsRotIdentity() const{
			if (m_Rot[0] == bsm::Vec3(1.0f, 0.0f, 0.0f) 
				&& m_Rot[1] == bsm::Vec3(0.0f, 1.0f, 0.0f) 
				&& m_Rot[2] == bsm::Vec3(0.0f, 0.0f, 1.0f)){
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	包み込むXZ矩形を得る
		@return	XZ矩形
		*/
		//--------------------------------------------------------------------------------------
		RECT_XZ GetWrappedRECT_XZ() const {
			RECT_XZ ret;
			SPHERE sp;
			sp.m_Radius = bsm::length(m_Size);
			sp.m_Center = m_Center;
			return sp.GetWrappedRECT_XZ();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	包み込むSPHEREを得る
		@return	SPHERE
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetWrappedSPHERE() const {
			SPHERE sp;
			sp.m_Radius = bsm::length(m_Size);
			sp.m_Center = m_Center;
			return sp;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	包み込むAABBを得る
		@return	AABB
		*/
		//--------------------------------------------------------------------------------------
		AABB GetWrappedAABB() const {
			float Half = 0.5f;
			bsm::Vec3 Vec[] = {
				bsm::Vec3(-Half,-Half,-Half),
				bsm::Vec3(Half,-Half,-Half),
				bsm::Vec3(Half,-Half,Half),
				bsm::Vec3(-Half,-Half,Half),

				bsm::Vec3(-Half,Half,-Half),
				bsm::Vec3(Half,Half,-Half),
				bsm::Vec3(Half,Half,Half),
				bsm::Vec3(-Half,Half,Half),
			};
			bool First = true;
			AABB ret;
			auto m = GetWorldMatrix();
			for (auto& v : Vec) {
				v *= m;
				if (First) {
					ret.m_Min = v;
					ret.m_Max = v;
					First = false;
				}
				else {
					if (ret.m_Min.x > v.x) {
						ret.m_Min.x = v.x;
					}
					if (ret.m_Max.x < v.x) {
						ret.m_Max.x = v.x;
					}

					if (ret.m_Min.y > v.y) {
						ret.m_Min.y = v.y;
					}
					if (ret.m_Max.y < v.y) {
						ret.m_Max.y = v.y;
					}

					if (ret.m_Min.z > v.z) {
						ret.m_Min.z = v.z;
					}
					if (ret.m_Max.z < v.z) {
						ret.m_Max.z = v.z;
					}
				}
			}
			return ret;
		}

	};



	//--------------------------------------------------------------------------------------
	///	球配列データ
	//--------------------------------------------------------------------------------------
	struct SPHERE_ARR{
		vector<SPHERE> m_SphereArr;
	};

	//--------------------------------------------------------------------------------------
	///	平面
	//--------------------------------------------------------------------------------------
	struct PLANE{
		bsm::Vec3 m_Normal;	///< 法線
		float m_DotValue;	///< 内積値
		//--------------------------------------------------------------------------------------
		/*!
		@brief	3つの点から平面を作成
		@param[in]	PointA	点A
		@param[in]	PointB	点B
		@param[in]	PointC	点C
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Set(const bsm::Vec3& PointA,const bsm::Vec3& PointB,const bsm::Vec3& PointC){
			m_Normal = bsm::normalize(bsm::cross(PointB - PointA,PointC - PointA));
			m_DotValue = bsm::dot(m_Normal,PointA);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		PLANE(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ（3つの点から平面を作成）
		@param[in]	PointA	点A
		@param[in]	PointB	点B
		@param[in]	PointC	点C
		*/
		//--------------------------------------------------------------------------------------
		PLANE(const bsm::Vec3& PointA,const bsm::Vec3& PointB,const bsm::Vec3& PointC){
			Set(PointA,PointB,PointC);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ（bsm::Plane4型から平面を作成）
		@param[in]	plane	bsm::Plane4型の値
		*/
		//--------------------------------------------------------------------------------------
		PLANE(const bsm::Plane4& plane){
			m_Normal.x = plane.x;
			m_Normal.y = plane.x;
			m_Normal.z = plane.z;
			m_DotValue = -plane.w;
		}
	};


	//--------------------------------------------------------------------------------------
	///	シリンダーボリューム境界
	//--------------------------------------------------------------------------------------
	struct CYLINDER {
		float m_Radius;			///< 半径
		bsm::Vec3 m_PointBottom;		///< 中間部線分の開始点
		bsm::Vec3 m_PointTop;		///< 中間部線分の終了点
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		CYLINDER() :
			m_Radius(1.0f),
			m_PointBottom(0, -0.5f, 0),
			m_PointTop(0, 0.5f, 0)
		{
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Radius	半径
		@param[in]	PointBottom	中間部線分の開始点
		@param[in]	PointTop	中間部線分の終了点
		*/
		//--------------------------------------------------------------------------------------
		CYLINDER(float Radius, const bsm::Vec3& PointBottom, const bsm::Vec3& PointTop) :
			m_Radius(Radius),
			m_PointBottom(PointBottom),
			m_PointTop(PointTop)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心点を得る
		@return	中心点
		*/
		//--------------------------------------------------------------------------------------
		bsm::Vec3 GetCenter() const {
			return m_PointBottom + ((m_PointTop - m_PointBottom) * 0.5f);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	高さを得る
		@return	高さ
		*/
		//--------------------------------------------------------------------------------------
		float GetHeight() const {
			return bsm::length(m_PointTop - m_PointBottom);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	半径1.0、高さ1.0の基本形からのワールド行列を得る
		@return	ワールド行列
		*/
		//--------------------------------------------------------------------------------------
		bsm::Mat4x4 GetMatrix() const{
			bsm::Mat4x4 mat;
			bsm::Vec3 Pos = GetCenter();
			bsm::Vec3 Rot = m_PointTop - m_PointBottom;
			Rot.normalize();
			bsm::Quat Qt(Rot,0.0f);
			float Height = bsm::length(m_PointTop - m_PointBottom);
			bsm::Vec3 Scale(m_Radius, Height, m_Radius);
			mat.affineTransformation(Scale, bsm::Vec3(0, 0, 0), Qt, Pos);
			return mat;
		}
	};


	//--------------------------------------------------------------------------------------
	///	カプセルボリューム境界
	//--------------------------------------------------------------------------------------
	struct CAPSULE {
		float m_Radius;			///< 半径
		bsm::Vec3 m_PointBottom;		///< 中間部線分の開始点
		bsm::Vec3 m_PointTop;		///< 中間部線分の終了点
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE():
			m_Radius(0.5f),
			m_PointBottom(0,-0.5f,0),
			m_PointTop(0,0.5f,0)
		{
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Radius	半径
		@param[in]	PointBottom	中間部線分の開始点
		@param[in]	PointTop	中間部線分の終了点
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE(float Radius,const bsm::Vec3& PointBottom,const bsm::Vec3& PointTop):
			m_Radius(Radius),
			m_PointBottom(PointBottom),
			m_PointTop(PointTop)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	MakedRadius	作成時の半径
		@param[in]	MakedPointBottom	作成時の中間部線分の開始点
		@param[in]	MakedPointTop	作成時の中間部線分の終了点
		@param[in]	Matrix	変換行列
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE(float MakedRadius,const bsm::Vec3& MakedPointBottom,const bsm::Vec3& MakedPointTop,
			const bsm::Mat4x4& Matrix):
			m_Radius(MakedRadius),
			m_PointBottom(MakedPointBottom),
			m_PointTop(MakedPointTop)
		{
			m_PointBottom *= Matrix;
			m_PointTop *= Matrix;
			//スケーリングのみ1方向で計算
			bsm::Vec3 Scale = Matrix.scaleInMatrix();
			m_Radius *= Scale.x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心点を得る
		@return	中心点
		*/
		//--------------------------------------------------------------------------------------
		bsm::Vec3 GetCenter() const{
			return m_PointBottom + ((m_PointTop - m_PointBottom) * 0.5f);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心点を設定する
		@param[in]	Center	中心点
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetCenter(const bsm::Vec3& Center){
			bsm::Vec3 CenterToPointA = ((m_PointBottom - m_PointTop) * 0.5f);
			bsm::Vec3 CenterToPointB = ((m_PointTop - m_PointBottom) * 0.5f);
			m_PointBottom = Center + CenterToPointA;
			m_PointTop = Center + CenterToPointB;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	縦の半径を得る
		@return	縦の半径
		*/
		//--------------------------------------------------------------------------------------
		float GetHeightRadius()const{
			float PointLen = bsm::length(m_PointTop - m_PointBottom) * 0.5f;
			PointLen += m_Radius;
			return PointLen;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	包み込むXZ矩形を得る
		@return	XZ矩形
		*/
		//--------------------------------------------------------------------------------------
		RECT_XZ GetWrappedRECT_XZ() const {
			RECT_XZ ret;
			POINT_XZ P1(m_PointBottom);
			POINT_XZ P2(m_PointTop);
			if (bsm::length(bsm::Vec2(P1)) <= bsm::length(bsm::Vec2(P2))) {
				ret.m_Left = P1.m_X - m_Radius;
				ret.m_Right = P2.m_X + m_Radius;
				ret.m_Near = P1.m_Z - m_Radius;
				ret.m_Far = P2.m_Z + m_Radius;
			}
			else {
				ret.m_Left = P2.m_X - m_Radius;
				ret.m_Right = P1.m_X + m_Radius;
				ret.m_Near = P2.m_Z - m_Radius;
				ret.m_Far = P1.m_Z + m_Radius;
			}
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	包み込むSPHEREを得る
		@return	SPHERE
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetWrappedSPHERE() const {
			SPHERE sp;
			auto Size = bsm::length(m_PointTop - m_PointBottom);
			Size *= 0.5f;
			Size += m_Radius;
			sp.m_Radius = Size;
			sp.m_Center = GetCenter();
			return sp;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	包み込むAABBを得る
		@return	AABB
		*/
		//--------------------------------------------------------------------------------------
		AABB GetWrappedAABB() const;
	};

	//--------------------------------------------------------------------------------------
	///	衝突判定に使用する三角形（時計回り）
	//--------------------------------------------------------------------------------------
	struct TRIANGLE{
		bsm::Vec3 m_A;
		bsm::Vec3 m_B;
		bsm::Vec3 m_C;
		void Set(const bsm::Vec3& baseA, const bsm::Vec3& baseB, const bsm::Vec3& baseC,
			const bsm::Mat4x4& m) {
			m_A = m * baseA;
			m_B = m * baseB;
			m_C = m * baseC;
		}
		TRIANGLE(){}
		TRIANGLE(const bsm::Vec3& baseA, const bsm::Vec3& baseB, const bsm::Vec3& baseC,
			const bsm::Mat4x4& m) {
			Set(baseA,baseB,baseC,m);
		}
		bsm::Vec3 GetNormal()const {
			bsm::Vec3 Ret =
				bsm::cross(
					m_B - m_A,
					m_C - m_A
				);
			return Ret;
		}
		PLANE GetPLANE() const {
			//3点を使って面を作成
			PLANE ret(m_A, m_B, m_C);
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	包み込むXZ矩形を得る
		@return	XZ矩形
		*/
		//--------------------------------------------------------------------------------------
		RECT_XZ GetWrappedRECT_XZ() const {
			RECT_XZ ret;
			POINT_XZ P1(m_A);
			POINT_XZ P2(m_B);
			POINT_XZ P3(m_C);

			ret.m_Left = P1.m_X;
			if (ret.m_Left < P2.m_X) {
				ret.m_Left = P2.m_X;
			}
			if (ret.m_Left < P3.m_X) {
				ret.m_Left = P3.m_X;
			}

			ret.m_Right = P1.m_X;
			if (ret.m_Right > P2.m_X) {
				ret.m_Right = P2.m_X;
			}
			if (ret.m_Right > P3.m_X) {
				ret.m_Right = P3.m_X;
			}

			ret.m_Near = P1.m_Z;
			if (ret.m_Near < P2.m_Z) {
				ret.m_Near = P2.m_Z;
			}
			if (ret.m_Near < P3.m_Z) {
				ret.m_Near = P3.m_Z;
			}

			ret.m_Far = P1.m_Z;
			if (ret.m_Far > P2.m_Z) {
				ret.m_Far = P2.m_Z;
			}
			if (ret.m_Far > P3.m_Z) {
				ret.m_Far = P3.m_Z;
			}
			return ret;
		}
	};


	//--------------------------------------------------------------------------------------
	///	衝突判定に使用するRECT
	//--------------------------------------------------------------------------------------
	struct COLRECT {
		bsm::Vec3 m_Center;     //中心点の座標
		bsm::Vec3 m_Rot[2];  //XY の各座標軸の傾きを表す方向ベクトル
		float m_UVec[2];     //XY座標軸に沿った長さの半分（中心点から面までの長さ）
		float m_BaseXSize;	//制作時のサイズX（各種計算に使う）
		float m_BaseYSize;	//制作時のサイズY（各種計算に使う）
		bsm::Mat4x4 m_Matrix;	//行列（各種計算に使う）
		bsm::Vec3 m_Vertex[4];	//行列によって変換された頂点(各種計算に使う)
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ<br />
		横の基本大きさ、縦の基本大きさをと変換行列を与えてRECTを完成させる<br />
		COLRECTはデフォルトコンストラクタはない。
		@param[in]	BaseXSize	横方向の大きさ
		@param[in]	BaseYSize	縦方向の大きさ
		@param[in]	Matrix	変換行列
		*/
		//--------------------------------------------------------------------------------------
		COLRECT(float BaseXSize, float BaseYSize, const bsm::Mat4x4& Matrix):
			m_BaseXSize(BaseXSize),
			m_BaseYSize(BaseYSize),
			m_Matrix(Matrix)
			{
			m_Center.x = Matrix._41;
			m_Center.y = Matrix._42;
			m_Center.z = Matrix._43;
			bsm::Vec3 VecX(Matrix._11, Matrix._12, Matrix._13);
			bsm::Vec3 VecY(Matrix._21, Matrix._22, Matrix._23);
			m_UVec[0] = BaseXSize * bsm::length(VecX) * 0.5f;
			m_UVec[1] = BaseYSize * bsm::length(VecY) * 0.5f;
			//回転を得る
			m_Rot[0] = bsm::normalize(VecX);
			m_Rot[1] = bsm::normalize(VecY);

			//頂点の設定
			float HalfX = m_BaseXSize * 0.5f;
			float HalfY = m_BaseYSize * 0.5f;
			m_Vertex[0] = bsm::Vec3(-HalfX, HalfY, 0);
			m_Vertex[1] = bsm::Vec3(HalfX, HalfY, 0);
			m_Vertex[2] = bsm::Vec3(-HalfX, -HalfY, 0);
			m_Vertex[3] = bsm::Vec3(HalfX, -HalfY, 0);
			for (auto& v : m_Vertex) {
				v *= m_Matrix;
			}
		}
		PLANE GetPLANE() const {
			//表面上に3つの点を使ってPLANEを作成
			//1つ目の点は中心
			bsm::Vec3 Point0 = m_Center;
			float MakedHalfX = m_BaseXSize * 0.5f;
			float MakedHalfY = m_BaseYSize * 0.5f;
			//2つ目は-0.5,-0.5,0の点をワールド変換したもの
			bsm::Vec3 Point1(-MakedHalfX, -MakedHalfY,0);
			Point1 = m_Matrix * Point1;
			//3つ目は-0.5,0.5,0の点をワールド変換したもの
			bsm::Vec3 Point2(MakedHalfX, -MakedHalfY, 0);
			Point2 = m_Matrix * Point2;
			//3点を使って面を作成
			PLANE ret(Point0, Point1, Point2);
			return ret;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	包み込むXZ矩形を得る
		@return	XZ矩形
		*/
		//--------------------------------------------------------------------------------------
		RECT_XZ GetWrappedRECT_XZ() const {
			RECT_XZ ret;

			POINT_XZ P1(m_Vertex[0].x, m_Vertex[0].z);
			POINT_XZ P2(m_Vertex[1].x, m_Vertex[1].z);
			POINT_XZ P3(m_Vertex[2].x, m_Vertex[2].z);
			POINT_XZ P4(m_Vertex[3].x, m_Vertex[3].z);

			ret.m_Left = P1.m_X;
			if (ret.m_Left < P2.m_X) {
				ret.m_Left = P2.m_X;
			}
			if (ret.m_Left < P3.m_X) {
				ret.m_Left = P3.m_X;
			}
			if (ret.m_Left < P4.m_X) {
				ret.m_Left = P4.m_X;
			}


			ret.m_Right = P1.m_X;
			if (ret.m_Right > P2.m_X) {
				ret.m_Right = P2.m_X;
			}
			if (ret.m_Right > P3.m_X) {
				ret.m_Right = P3.m_X;
			}
			if (ret.m_Right > P4.m_X) {
				ret.m_Right = P4.m_X;
			}


			ret.m_Near = P1.m_Z;
			if (ret.m_Near < P2.m_Z) {
				ret.m_Near = P2.m_Z;
			}
			if (ret.m_Near < P3.m_Z) {
				ret.m_Near = P3.m_Z;
			}
			if (ret.m_Near < P4.m_Z) {
				ret.m_Near = P4.m_Z;
			}

			ret.m_Far = P1.m_Z;
			if (ret.m_Far > P2.m_Z) {
				ret.m_Far = P2.m_Z;
			}
			if (ret.m_Far > P3.m_Z) {
				ret.m_Far = P3.m_Z;
			}
			if (ret.m_Far > P4.m_Z) {
				ret.m_Far = P4.m_Z;
			}
			return ret;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	包み込むSPHEREを得る
		@return	SPHERE
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetWrappedSPHERE() const {
			SPHERE sp;
			auto Size = bsm::length(m_Vertex[1] - m_Vertex[2]);
			Size *= 0.5f;
			sp.m_Radius = Size;
			sp.m_Center = m_Center;
			return sp;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	包み込むAABBを得る
		@return	AABB
		*/
		//--------------------------------------------------------------------------------------
		AABB GetWrappedAABB() const {
			AABB ret;
			ret.m_Min = m_Vertex[0];
			ret.m_Max = m_Vertex[0];
			for (int i = 1; i < 4; i++) {
				if (ret.m_Min.x > m_Vertex[i].x) {
					ret.m_Min.x = m_Vertex[i].x;
				}
				if (ret.m_Max.x < m_Vertex[i].x) {
					ret.m_Max.x = m_Vertex[i].x;
				}

				if (ret.m_Min.y > m_Vertex[i].y) {
					ret.m_Min.y = m_Vertex[i].y;
				}
				if (ret.m_Max.y < m_Vertex[i].y) {
					ret.m_Max.y = m_Vertex[i].y;
				}

				if (ret.m_Min.z > m_Vertex[i].z) {
					ret.m_Min.z = m_Vertex[i].z;
				}
				if (ret.m_Max.z < m_Vertex[i].z) {
					ret.m_Max.z = m_Vertex[i].z;
				}
			}
			return ret;
		}
	};

	//--------------------------------------------------------------------------------------
	///	衝突判定用のユーティリティ構造体
	//--------------------------------------------------------------------------------------
	struct HitTest{
		HitTest(){}
		~HitTest(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	float型のクランプ
		@param[in]	n	クランプする値
		@param[in]	min	最小値
		@param[in]	max	最大値
		@return	クランプされたfloat
		*/
		//--------------------------------------------------------------------------------------
		static float floatClamp(float n,float min,float max){
			if(n < min) return min;
			if(n > max) return max;
			return n;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	chkをmin_vおよびmax_vと比較して最小または最大のベクトルを設定する
		@param[in]	chk	チェックするベクトル
		@param[in,out]	min_v	比較して最小値を設定するベクトル
		@param[in,out]	max_v	比較して最大値を設定するベクトル
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ChkSetMinMax(const bsm::Vec3& chk, bsm::Vec3& min_v, bsm::Vec3& max_v) {
			if (min_v.x > chk.x) {
				min_v.x = chk.x;
			}
			if (max_v.x < chk.x) {
				max_v.x = chk.x;
			}
			if (min_v.y > chk.y) {
				min_v.y = chk.y;
			}
			if (max_v.y < chk.y) {
				max_v.y = chk.y;
			}
			if (min_v.z > chk.z) {
				min_v.z = chk.z;
			}
			if (max_v.z < chk.z) {
				max_v.z = chk.z;
			}
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	点が面の裏側にあるかどうか判定
		@param[in]	p	検証する点
		@param[in]	pl	面
		@return	裏側ならtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool InsidePtPlane(const bsm::Vec3& p, const PLANE& pl){
			float dist = bsm::dot(p,pl.m_Normal) - pl.m_DotValue;
			return dist < 0.0f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	線分同士の最近接点を計算する（カプセル同士の判定に使用）
		@param[in]	p1	カプセル1の線分開始点
		@param[in]	q1	カプセル1の線分終了点
		@param[in]	p2	カプセル2の線分開始点
		@param[in]	q2	カプセル2の線分終了点
		@param[out]	s	線分1の最近接点のパラメータ
		@param[out]	t	線分2の最近接点のパラメータ
		@param[out]	c1	最近接点1
		@param[out]	c2	最近接点2
		@return	最近接点c1,c2を計算し、そのパラメータs,tを計算し線分同士の距離の平方を返す
		*/
		//--------------------------------------------------------------------------------------
		static float ClosestPtSegmentSegment(
			const bsm::Vec3& p1,const bsm::Vec3& q1,const bsm::Vec3& p2,const bsm::Vec3& q2,
			float& s,float& t,bsm::Vec3& c1,bsm::Vec3& c2){
			const float EPSILON = 1.175494e-37f;
			bsm::Vec3 d1 = q1 - p1;
			bsm::Vec3 d2 = q2 - p2;
			bsm::Vec3 r = p1 - p2;
			float a = bsm::dot(d1,d1);
			float e = bsm::dot(d2,d2);
			float f = bsm::dot(d2,r);

			if(f <= EPSILON && e <= EPSILON){
				s = t = 0.0f;
				c1 = p1;
				c2 = p2;
				return bsm::dot(c1 - c2,c1 - c2);
			}
			if(a <= EPSILON){
				s = 0.0f;
				t = f / e;
				t = floatClamp(t,0.0f,1.0f);
			}
			else{
				float c = bsm::dot(d1,r);
				if(e <= EPSILON){
					t = 0.0f;
					s = floatClamp(-c / a,0.0f,1.0f);
				}
				else{
					float b = bsm::dot(d1,d2);
					float denom = a * e - b * b;
					if(denom != 0.0f){
						s = floatClamp((b * f - c * e) / denom,0.0f,1.0f);
					}
					else{
						s = 0.0f;
					}
					t = (b * s + f) / e;
					if(t < 0.0f){
						t = 0.0f;
						s = floatClamp(-c / a,0.0f,1.0f);
					}
					else if(t > 1.0f){
						t = 1.0f;
						s = floatClamp((b - c) / a,0.0f,1.0f);
					}
				}
			}
			c1 = p1 + d1 * s;
			c2 = p2 + d2 * t;
			return bsm::dot(c1 - c2,c1 - c2);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	点cと線分abの距離の平方を返す
		@param[in]	a	線分開始点
		@param[in]	b	線分終了点
		@param[in]	c	計算点
		@return	点cと線分abの距離の平方
		*/
		//--------------------------------------------------------------------------------------
		static float SqDistPointSegment(const bsm::Vec3& a, const bsm::Vec3& b, const bsm::Vec3& c){
			bsm::Vec3 ab = b - a;
			bsm::Vec3 ac = c - a;
			bsm::Vec3 bc = c - b;
			float e = bsm::dot(ac,ab);
			if(e <= 0.0f){
				return bsm::dot(ac,ac);
			}
			float f = bsm::dot(ab,ab);
			if(e >= f){
				return bsm::dot(bc,bc);
			}
			return bsm::dot(ac,ac) - e * e / f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	点cと線分abの最近接点を返す
		@param[in]	c	もとになる点
		@param[in]	a	線分開始点
		@param[in]	b	線分終了点
		@param[out]	t	最近接点の線分上の位置の割合の戻り値
		@param[out]	d	最近接点の戻り値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ClosetPtPointSegment(const bsm::Vec3& c,
			const bsm::Vec3& a,const bsm::Vec3& b,
			float& t,bsm::Vec3& d){
			bsm::Vec3 ab = b - a;
			t = bsm::dot(c - a,ab) / bsm::dot(ab,ab);
			if(t < 0.0f){
				t = 0.0f;
			}
			if(t > 1.0f){
				t = 1.0f;
			}
			d = a + (ab * t);
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	点cとプレーンの最近接点を返す
		@param[in]	c	もとになる点
		@param[in]	c	もとになる点
		@param[out]	d	最近接点の戻り値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ClosetPtPointPlane(const bsm::Vec3& c,
			const PLANE& pl,bsm::Vec3& d) {
			float t = (bsm::dot(pl.m_Normal, c) - pl.m_DotValue) / bsm::dot(pl.m_Normal, pl.m_Normal);
			d = c - pl.m_Normal * t;
		}



		static SPHERE SphereEnclosingSphere(const SPHERE& s0, const SPHERE& s1) {
			const float EPSILON = 1.175494e-37f;
			SPHERE s;
			bsm::Vec3 d = s1.m_Center - s0.m_Center;
			float dist2 = bsm::dot(d,d);
			float f = s1.m_Radius - s0.m_Radius;
			bsm::Vec4 vec0(f);
			vec0 = XMVectorPow(vec0, bsm::Vec4(2.0f));
			if (vec0.x >= dist2) {
				if (s1.m_Radius >= s0.m_Radius) {
					s = s1;
				}
				else {
					s = s0;
				}
			}
			else {
				bsm::Vec4 vec(dist2);
				vec = XMVectorSqrt(vec);
				float dist = vec.x;
				s.m_Radius = (dist + s0.m_Radius + s1.m_Radius) * 0.5f;
				s.m_Center = s0.m_Center;
				if (dist > EPSILON) {
					s.m_Center += ((s.m_Radius - s0.m_Radius) / dist) * d;
				}

			}
			return s;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	球とカプセルとの衝突判定
		@param[in]	sp	球
		@param[in]	cap	カプセル
		@param[out]	d	最近接点の戻り値
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_CAPSULE(const SPHERE& sp, const CAPSULE& cap, bsm::Vec3& d){
			//まず最近接点を設定
			float t;
			bsm::Vec3 ClosetPoint;
			ClosetPtPointSegment(sp.m_Center, cap.m_PointBottom, cap.m_PointTop, t, ClosetPoint);
			//ClosetPointは、カプセル線分上の点である
			bsm::Vec3 Normal = sp.m_Center - ClosetPoint;
			Normal.normalize();
			Normal *= cap.m_Radius;
			//最近接点を設定
			d = ClosetPoint + Normal;
			//衝突しているか判別
			float dist2 = SqDistPointSegment(cap.m_PointBottom,cap.m_PointTop,sp.m_Center);
			float radius = sp.m_Radius + cap.m_Radius;
			return dist2 <= radius * radius;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	球と平面との衝突判定
		@param[in]	sp	球
		@param[in]	pl	平面
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_PLANE(const SPHERE& sp, const PLANE& pl) {
			float dist = bsm::dot(sp.m_Center, pl.m_Normal) - pl.m_DotValue;
			return dist <= sp.m_Radius;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	球とシリンダーとの衝突判定
		@param[in]	sp	球
		@param[in]	cy	シリンダー
		@param[out]	d	最近接点の戻り値
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_CYLINDER(const SPHERE& sp, const CYLINDER& cy, bsm::Vec3& d) {
			CAPSULE cap;
			cap.m_PointBottom = cy.m_PointBottom;
			cap.m_PointTop = cy.m_PointTop;
			cap.m_Radius = cy.m_Radius;
			//まずカプセルを作成して判定
			if (!SPHERE_CAPSULE(sp, cap, d)) {
				return false;
			}
			bsm::Vec3 CenterPole = cap.m_PointTop - cap.m_PointBottom;
			float CenterPoleLen = CenterPole.length();
			CenterPole.normalize();
			float f = bsm::dot(CenterPole, d - cap.m_PointBottom);
			if (f >= 0 && f <= CenterPoleLen) {
				return true;
			}

			bsm::Vec3 t0(0, 0.5f, 0);
			bsm::Vec3 t1(0.0f, 0.5f, 1.0);
			bsm::Vec3 t2(1.0, 0.5f, 0.0f);

			bsm::Vec3 b0(0, -0.5f, 0);
			bsm::Vec3 b1(1.0f, -0.5f, 0.0);
			bsm::Vec3 b2(0.0, -0.5f, 1.0f);

			bsm::Mat4x4 cyWorld = cy.GetMatrix();
			t0 *= cyWorld;
			t1 *= cyWorld;
			t2 *= cyWorld;

			b0 *= cyWorld;
			b1 *= cyWorld;
			b2 *= cyWorld;

			PLANE topPlane(t0,t1,t2);
			PLANE bottomPlane(b0, b1, b2);

			bool tb = SPHERE_PLANE(sp, topPlane);
			bool bb = SPHERE_PLANE(sp, bottomPlane);
			if (tb && bb) {
				bsm::Vec3 topV,bottomV;
				ClosetPtPointPlane(sp.m_Center, topPlane, topV);
				ClosetPtPointPlane(sp.m_Center, bottomPlane, bottomV);
				if (bsm::lengthSqr(sp.m_Center - topV) <= bsm::lengthSqr(sp.m_Center - bottomV)) {
					d = topV;
				}
				else {
					d = bottomV;
				}
				return true;
			}
			return false;
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	カプセルとカプセルとの衝突判定
		@param[in]	cap1	カプセル1
		@param[in]	cap2	カプセル2
		@param[out]	retvec1	カプセル1の最近接点を返す参照
		@param[out]	retvec2	カプセル2の最近接点を返す参照
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CAPSULE_CAPSULE(
			const CAPSULE& cap1,
			const CAPSULE& cap2,
			bsm::Vec3& retvec1,
			bsm::Vec3& retvec2){
			float s,t;
			float dist2 = ClosestPtSegmentSegment(cap1.m_PointBottom,cap1.m_PointTop,cap2.m_PointBottom,cap2.m_PointTop,
				s,t,retvec1,retvec2);
			float radius = cap1.m_Radius + cap2.m_Radius;
			return dist2 <= radius * radius;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	pointから見た三角形の最近接点を得る
		@param[in]	point	基準点
		@param[in]	t	三角形
		@param[out]	retvec	最近接点を返す参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ClosestPtPointTriangle(const bsm::Vec3& point,const TRIANGLE& t,bsm::Vec3& retvec) {
			bsm::Vec3 ab = t.m_B - t.m_A;
			bsm::Vec3 ac = t.m_C - t.m_A;
			bsm::Vec3 ap = point - t.m_A;
			float d1 = bsm::dot(ab,ap);
			float d2 = bsm::dot(ac,ap);
			if (d1 <= 0.0f && d2 <= 0.0f) {
				retvec = t.m_A;
				return;
			}

			bsm::Vec3 bp = point - t.m_B;
			float d3 = bsm::dot(ab,bp);
			float d4 = bsm::dot(ac,bp);
			if (d3 >= 0.0f && d4 <= d3) {
				retvec = t.m_B;
				return;
			}
			float vc = d1 * d4 - d3 * d2;
			if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
				float v = d1 / (d1 - d3);
				retvec = v * ab + t.m_A;
				return;
			}

			bsm::Vec3 cp = point - t.m_C;
			float d5 = bsm::dot(ab,cp);
			float d6 = bsm::dot(ac,cp);
			if (d6 >= 0.0f && d5 <= d6) {
				retvec = t.m_C;
				return;
			}
			float vb = d5 * d2 - d1 * d6;

			if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
				float w = d2 / (d2 - d6);
				retvec = w * ac + t.m_A;
				return;
			}

			float va = d3 * d6 - d5 * d4;
			if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
				float w = (d4 - d3) / ((d4 - d3) + (d5 -d6));
				retvec = w * (t.m_C - t.m_B) + t.m_B;
				return;
			}
			float denon = 1.0f / (va + vb + vc);
			float v = vb * denon;
			float w = vc * denon;
			retvec = ab * v + ac * w + t.m_A;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	pointから見たOBBの最近接点を得る
		@param[in]	point	基準点
		@param[in]	obb	OBB
		@param[out]	retvec	最近接点を返す参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ClosestPtPointOBB(const bsm::Vec3& point, const OBB& obb, bsm::Vec3& retvec){
			bsm::Vec3 d = point - obb.m_Center;
			retvec = obb.m_Center;
			float dist;
			for(int i = 0; i < 3; i++)
			{
				dist = bsm::dot(d,obb.m_Rot[i]);
				if(dist > obb.m_Size[i])
				{
					dist = obb.m_Size[i];
				}
				if(dist < -obb.m_Size[i])
				{
					dist = -obb.m_Size[i];
				}
				retvec +=  dist * obb.m_Rot[i];
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	球と球との衝突判定
		@param[in]	sp1	球1
		@param[in]	sp2	球2
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_SPHERE(const SPHERE& sp1, const SPHERE& sp2){
			bsm::Vec3 d = sp1.m_Center - sp2.m_Center;
			float dist2 = bsm::dot(d,d);
			float radiussum = sp1.m_Radius + sp2.m_Radius;
			return dist2 <= radiussum * radiussum;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	球と球配列との衝突判定
		@param[in]	sp1	球1
		@param[in]	sparr	球配列
		@param[out]	HitIndex	衝突していた場合の配列のインデックス
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_SPHERE_ARR(const SPHERE& sp1, const SPHERE_ARR& sparr, size_t& HitIndex){
			size_t sz = sparr.m_SphereArr.size();
			for(size_t i = 0;i < sz;i++){
				if(SPHERE_SPHERE(sp1,sparr.m_SphereArr[i])){
					HitIndex = i;
					return true;
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	球配列と球配列との衝突判定
		@param[in]	srcarr	球配列src
		@param[in]	destarr	球配列dest
		@param[out]	SrcHitIndex	衝突していた場合のSrc配列のインデックス
		@param[out]	DestHitIndex	衝突していた場合のDest配列のインデックス
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_ARR_SPHERE_ARR(const SPHERE_ARR& srcarr, const SPHERE_ARR& destarr,
			size_t& SrcHitIndex,size_t& DestHitIndex){
			size_t SrcSz = srcarr.m_SphereArr.size();
			size_t DestSz = destarr.m_SphereArr.size();
			for(size_t i = 0;i < SrcSz;i++){
				for(size_t j = 0;j < DestSz;j++){
					if(SPHERE_SPHERE(srcarr.m_SphereArr[i],destarr.m_SphereArr[j])){
						SrcHitIndex = i;
						DestHitIndex = j;
						return true;
					}
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	点とAABBとの最近接点を得る
		@param[in]	p	点
		@param[in]	b	AABB
		@param[out]	retvec	最近接点が代入される参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ClosestPtPointAABB(const bsm::Vec3& p, const AABB& b,
			bsm::Vec3& retvec){
			for(int i = 0;i < 3;i++){
				float v = p[i];
				if(v < b.m_Min[i]){
					v = b.m_Min[i];
				}
				if(v > b.m_Max[i]){
					v = b.m_Max[i];
				}
				retvec[i] = v;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	点とAABBとの距離の平方を測る
		@param[in]	p	点
		@param[in]	b	AABB
		@return	点とAABBとの距離の平方
		*/
		//--------------------------------------------------------------------------------------
		static float SqDistPointAABB(const bsm::Vec3& p, const AABB& b){
			float sqDist = 0.0f;
			for(int i = 0;i < 3;i++){
				float v = p[i];
				if(v < b.m_Min[i]){
					sqDist += (b.m_Min[i] - v) * (b.m_Min[i] - v);
				}
				if(v > b.m_Max[i]){
					sqDist += (v - b.m_Max[i]) * (v - b.m_Max[i]);
				}
			}
			return sqDist;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	球とAABBとの衝突判定
		@param[in]	sp	球
		@param[in]	b	AABB
		@param[out]	retvec	最近接点が代入される参照
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_AABB(const SPHERE& sp, const AABB& b,
			bsm::Vec3& retvec){
				//最近接点を得る
				ClosestPtPointAABB(sp.m_Center,b,retvec);
				//距離の平方が球の半径の平方内ならtrue
				if(SqDistPointAABB(sp.m_Center,b) <= (sp.m_Radius * sp.m_Radius)){
					return true;
				}
				return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	AABBとAABBとの衝突判定
		@param[in]	a	AABB1
		@param[in]	b	AABB2
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool AABB_AABB(const AABB& a, const AABB& b){
			if(a.m_Max.x < b.m_Min.x || a.m_Min.x > b.m_Max.x)
				return false;
			if(a.m_Max.y < b.m_Min.y || a.m_Min.y > b.m_Max.y)
				return false;
			if(a.m_Max.z < b.m_Min.z || a.m_Min.z > b.m_Max.z)
				return false;
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	AABBとAABBとの衝突判定(minもしくはmaxが同じ値だったら衝突してない)
		@param[in]	a	AABB1
		@param[in]	b	AABB2
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool AABB_AABB_NOT_EQUAL(const AABB& a, const AABB& b) {
			if (a.m_Max.x <= b.m_Min.x || a.m_Min.x >= b.m_Max.x)
				return false;
			if (a.m_Max.y <= b.m_Min.y || a.m_Min.y >= b.m_Max.y)
				return false;
			if (a.m_Max.z <= b.m_Min.z || a.m_Min.z >= b.m_Max.z)
				return false;
			return true;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	AABB aがAABB ｂに完全に含まれるか判定
		@param[in]	a	AABB1
		@param[in]	b	AABB2
		@return	完全に含まれればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool AABB_IN_AABB(const AABB& a, const AABB& b){
			if(a.m_Min.x < b.m_Min.x || a.m_Max.x > b.m_Max.x)
				return false;
			if(a.m_Min.y < b.m_Min.y || a.m_Max.y > b.m_Max.y)
				return false;
			if(a.m_Min.z < b.m_Min.z || a.m_Max.z > b.m_Max.z)
				return false;
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	AABB aとAABB ｂを包むAABBを返す
		@param[in]	a	AABB1
		@param[in]	b	AABB2
		@return	完全に含まれればtrue
		*/
		//--------------------------------------------------------------------------------------
		static AABB AABB_OR_AABB(const AABB& a, const AABB& b) {
			AABB ret;
			ret.m_Min.x = (a.m_Min.x <= b.m_Min.x) ? a.m_Min.x : b.m_Min.x;
			ret.m_Min.y = (a.m_Min.y <= b.m_Min.y) ? a.m_Min.y : b.m_Min.y;
			ret.m_Min.z = (a.m_Min.z <= b.m_Min.z) ? a.m_Min.z : b.m_Min.z;

			ret.m_Max.x = (a.m_Max.x >= b.m_Max.x) ? a.m_Max.x : b.m_Max.x;
			ret.m_Max.y = (a.m_Max.y >= b.m_Max.y) ? a.m_Max.y : b.m_Max.y;
			ret.m_Max.z = (a.m_Max.z >= b.m_Max.z) ? a.m_Max.z : b.m_Max.z;

			return ret;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	球とOBBとの衝突判定
		@param[in]	sp	球
		@param[in]	obb	OBB
		@param[out]	retvec	最近接点が代入される参照
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_OBB(const SPHERE& sp, const OBB& obb, bsm::Vec3& retvec){
			ClosestPtPointOBB(sp.m_Center,obb,retvec);
			bsm::Vec3 v = retvec - sp.m_Center;
			return bsm::dot(v,v) <= sp.m_Radius * sp.m_Radius;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBBとOBBとの衝突判定
		@param[in]	obb1	OBB1
		@param[in]	obb2	OBB2
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool OBB_OBB(const OBB& obb1, const OBB& obb2) {
			const float EPSILON = 1.175494e-37f;
			float R[3][3], AbsR[3][3];
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					R[i][j] = bsm::dot(obb1.m_Rot[i], obb2.m_Rot[j]);
					AbsR[i][j] = fabsf(R[i][j]) + EPSILON;
				}
			}
			bsm::Vec3 t = obb2.m_Center - obb1.m_Center;
			t = bsm::Vec3(bsm::dot(t, obb1.m_Rot[0]),
				bsm::dot(t, obb1.m_Rot[1]),
				bsm::dot(t, obb1.m_Rot[2]));
			//軸L=A0, L=A1, L=A2判定
			float ra, rb;
			for (int i = 0; i < 3; i++) {
				ra = obb1.m_Size[i];
				rb = obb2.m_Size[0] * AbsR[i][0] + obb2.m_Size[1] * AbsR[i][1] + obb2.m_Size[2] * AbsR[i][2];
				if (fabsf(t[i]) > ra + rb) {
					return false;
				}
			}
			//軸L=B0, L=B1, L=B2判定
			for (int i = 0; i < 3; i++) {
				ra = obb1.m_Size[0] * AbsR[0][i] + obb1.m_Size[1] * AbsR[1][i] + obb1.m_Size[2] * AbsR[2][i];
				rb = obb2.m_Size[i];
				if (fabsf(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb) {
					return false;
				}
			}
			//軸L=A0 X B0判定
			ra = obb1.m_Size[1] * AbsR[2][0] + obb1.m_Size[2] * AbsR[1][0];
			rb = obb2.m_Size[1] * AbsR[0][2] + obb2.m_Size[2] * AbsR[0][1];
			if (fabsf(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb) {
				return false;
			}
			//軸L=A0 X B1判定
			ra = obb1.m_Size[1] * AbsR[2][1] + obb1.m_Size[2] * AbsR[1][1];
			rb = obb2.m_Size[0] * AbsR[0][2] + obb2.m_Size[2] * AbsR[0][0];
			if (fabsf(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb) {
				return false;
			}
			//軸L=A0 X B2判定
			ra = obb1.m_Size[1] * AbsR[2][2] + obb1.m_Size[2] * AbsR[1][2];
			rb = obb2.m_Size[0] * AbsR[0][1] + obb2.m_Size[1] * AbsR[0][0];
			if (fabsf(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb) {
				return false;
			}
			//軸L=A1 X B0判定
			ra = obb1.m_Size[0] * AbsR[2][0] + obb1.m_Size[2] * AbsR[0][0];
			rb = obb2.m_Size[1] * AbsR[1][2] + obb2.m_Size[2] * AbsR[1][1];
			if (fabsf(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb) {
				return false;
			}
			//軸L=A1 X B1判定
			ra = obb1.m_Size[0] * AbsR[2][1] + obb1.m_Size[2] * AbsR[0][1];
			rb = obb2.m_Size[0] * AbsR[1][2] + obb2.m_Size[2] * AbsR[1][0];
			if (fabsf(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb) {
				return false;
			}
			//軸L=A1 X B2判定
			ra = obb1.m_Size[0] * AbsR[2][2] + obb1.m_Size[2] * AbsR[0][2];
			rb = obb2.m_Size[0] * AbsR[1][1] + obb2.m_Size[1] * AbsR[1][0];
			if (fabsf(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb) {
				return false;
			}
			//軸L=A2 X B0判定
			ra = obb1.m_Size[0] * AbsR[1][0] + obb1.m_Size[1] * AbsR[0][0];
			rb = obb2.m_Size[1] * AbsR[2][2] + obb2.m_Size[2] * AbsR[2][1];
			if (fabsf(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb) {
				return false;
			}
			//軸L=A2 X B1判定
			ra = obb1.m_Size[0] * AbsR[1][1] + obb1.m_Size[1] * AbsR[0][1];
			rb = obb2.m_Size[0] * AbsR[2][2] + obb2.m_Size[2] * AbsR[2][0];
			if (fabsf(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb) {
				return false;
			}
			//軸L=A2 X B2判定
			ra = obb1.m_Size[0] * AbsR[1][2] + obb1.m_Size[1] * AbsR[0][2];
			rb = obb2.m_Size[0] * AbsR[2][1] + obb2.m_Size[1] * AbsR[2][0];
			if (fabsf(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb) {
				return false;
			}
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	線分abと面判定
		@param[in]	a	線分開始点
		@param[in]	b	線分終了点
		@param[in]	p	面
		@param[out]	t	最近接点の線分上の位置の割合の戻り値
		@param[out]	q	最近接点の戻り値
		@return	交差していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SEGMENT_PLANE(const bsm::Vec3& a, const bsm::Vec3& b,
			const PLANE& p,float& t,bsm::Vec3& q){
			bsm::Vec3 ab = b - a;
			t = (p.m_DotValue - bsm::dot(p.m_Normal,a)) / bsm::dot(p.m_Normal,ab);
			if(t >= 0.0f && t <= 1.0f){
				q = a + (ab * t);
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	線分とAABBのレイ判定
		@param[in]	p0	線分開始点
		@param[in]	p1	線分終了点
		@param[in]	b	AABB
		@return	交差していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SEGMENT_AABB(const bsm::Vec3& p0, const bsm::Vec3& p1, const AABB& b){
			const float EPSILON = 1.175494e-37f;
			bsm::Vec3 c = (b.m_Min + b.m_Max) * 0.5f;
			bsm::Vec3 e = b.m_Max - c;
			bsm::Vec3 m = (p0 + p1) * 0.5f;
			bsm::Vec3 d = p1 - m;
			m = m - c;
			float adx = fabsf(d.x);
			if(fabsf(m.x) > e.x + adx){
				return false;
			}
			float ady = fabsf(d.y);
			if(fabsf(m.y) > e.y + ady){
				return false;
			}
			float adz = fabsf(d.z);
			if(fabsf(m.z) > e.z + adz){
				return false;
			}
			adx += EPSILON;
			ady += EPSILON;
			adz += EPSILON;
			if(fabsf(m.y * d.z - m.z * d.y) > e.y * adz + e.z * ady){
				return false;
			}
			if(fabsf(m.z * d.x - m.x * d.z) > e.x * adz + e.z * adx){
				return false;
			}
			if(fabsf(m.x * d.y - m.y * d.x) > e.x * ady + e.y * adx){
				return false;
			}
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	線分とOBBのレイ判定
		@param[in]	p0	線分開始点
		@param[in]	p1	線分終了点
		@param[in]	obb	OBB
		@return	交差していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SEGMENT_OBB(const bsm::Vec3& p0, const bsm::Vec3& p1, const OBB& obb){
			const float EPSILON = 1.175494e-37f;

			bsm::Vec3 m = (p0 + p1) * 0.5f;
			bsm::Vec3 d = p1 - m;
			m = m - obb.m_Center;
			m = bsm::Vec3(bsm::dot(obb.m_Rot[0], m), bsm::dot(obb.m_Rot[1], m), bsm::dot(obb.m_Rot[2], m));
			d = bsm::Vec3(bsm::dot(obb.m_Rot[0], d), bsm::dot(obb.m_Rot[1], d), bsm::dot(obb.m_Rot[2], d));

			float adx = fabsf(d.x);
			if(fabsf(m.x) > obb.m_Size.x + adx) return false;
			float ady = fabsf(d.y);
			if(fabsf(m.y) > obb.m_Size.y + ady) return false;
			float adz = fabsf(d.z);
			if(fabsf(m.z) > obb.m_Size.z + adz) return false;
			adx += EPSILON; 
			ady += EPSILON; 
			adz += EPSILON;
        
			if(fabsf(m.y * d.z - m.z * d.y) > obb.m_Size.y * adz + obb.m_Size.z * ady ) return false;
			if(fabsf(m.z * d.x - m.x * d.z) > obb.m_Size.x * adz + obb.m_Size.z * adx ) return false;
			if(fabsf(m.x * d.y - m.y * d.x) > obb.m_Size.x * ady + obb.m_Size.y * adx ) return false;

			return true;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	点とCOLRECTとの最近接点を得る
		@param[in]	p	点
		@param[in]	rect	COLRECT
		@param[out]	retvec	最近接点が代入される参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ClosestPtPointCOLRECT(const bsm::Vec3& point, const COLRECT& rect,
			bsm::Vec3& retvec) {
			bsm::Vec3 d = point - rect.m_Center;
			retvec = rect.m_Center;
			for (int i = 0; i < 2; i++) {
				float dist = bsm::dot(d, rect.m_Rot[i]);
				if (dist > rect.m_UVec[i]) {
					dist = rect.m_UVec[i];
				}
				if (dist < -rect.m_UVec[i]) {
					dist = -rect.m_UVec[i];
				}
				retvec += rect.m_Rot[i] * dist;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	球とCOLRECTとの衝突判定
		@param[in]	sp	球
		@param[in]	rect	COLRECT
		@param[out]	retvec	最近接点が代入される参照
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_COLRECT(const SPHERE& sp, const COLRECT& rect, bsm::Vec3& retvec) {
			//四角形との最近接点を得る
			ClosestPtPointCOLRECT(sp.m_Center, rect, retvec);
			//最近接点が半径以下なら衝突している
			if (bsm::length(sp.m_Center - retvec) <= sp.m_Radius) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphereと動かないRectの衝突判定
		@param[in]	SrcSp	Srcの球
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestRect	Dest矩形
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereRect(const SPHERE& SrcSp, const bsm::Vec3& SrcVelocity,
			const COLRECT& DestRect, float StartTime, float EndTime, float& HitTime) {
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			bsm::Vec3 RetVec;
			if (!HitTest::SPHERE_COLRECT(SrcSp2, DestRect, RetVec)) {
				return false;
			}
			if (EndTime - StartTime < m_EPSILON) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereRect(SrcSp, SrcVelocity, DestRect, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestSphereRect(SrcSp, SrcVelocity, DestRect, mid, EndTime, HitTime);
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBBとPLANEとの衝突判定
		@param[in]	obb	OBB
		@param[in]	plane	PLANE
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool OBB_PLANE(const OBB& obb, const PLANE& plane) {
			float r = obb.m_Size.x * abs(bsm::dot(plane.m_Normal, obb.m_Rot[0]))
				+ obb.m_Size.y * abs(bsm::dot(plane.m_Normal, obb.m_Rot[1]))
				+ obb.m_Size.z * abs(bsm::dot(plane.m_Normal, obb.m_Rot[2]));
			float s = bsm::dot(plane.m_Normal, obb.m_Center) - plane.m_DotValue;
			return abs(s) <= r;
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBBとCOLRECTとの衝突判定
		@param[in]	obb	OBB
		@param[in]	rect	COLRECT
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool OBB_COLRECT(const OBB& obb, const COLRECT& rect) {
			if (OBB_PLANE(obb, rect.GetPLANE())) {
				//平面と交差していた時のみOBBと調査
				OBB obb2(bsm::Vec3(rect.m_BaseXSize, rect.m_BaseYSize, 1.0f), rect.m_Matrix);
				return OBB_OBB(obb, obb2);
			}
			return false;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	SPHEREとTRIANGLEとの衝突判定
		@param[in]	sp	SPHERE
		@param[in]	tri	TRIANGLE
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_TRIANGLE(const SPHERE& sp, const TRIANGLE& tri, bsm::Vec3& retvec) {
			ClosestPtPointTriangle(sp.m_Center, tri, retvec);
			bsm::Vec3 v = retvec - sp.m_Center;
			return bsm::dot(v,v) <= sp.m_Radius * sp.m_Radius;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphereと動かないTriangleの衝突判定
		@param[in]	SrcSp	Srcの球
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestRect	Dest矩形
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereTriangle(const SPHERE& SrcSp, const bsm::Vec3& SrcVelocity,
			const TRIANGLE& DestTri, float StartTime, float EndTime, float& HitTime) {
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			bsm::Vec3 RetVec;
			if (!HitTest::SPHERE_TRIANGLE(SrcSp2, DestTri, RetVec)) {
				return false;
			}
			if (EndTime - StartTime < m_EPSILON) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereTriangle(SrcSp, SrcVelocity, DestTri, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestSphereTriangle(SrcSp, SrcVelocity, DestTri, mid, EndTime, HitTime);
		}




		//--------------------------------------------------------------------------------------
		/*!
		@brief	Obbと動かないRectの衝突判定
		@param[in]	SrcObb	SrcのObb
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestRect	Dest矩形
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestObbRect(const OBB& SrcObb, const bsm::Vec3& SrcVelocity,
			const COLRECT& DestRect, float StartTime, float EndTime, float& HitTime) {
			const float m_EPSILON = 0.005f;
			OBB SrcObb2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcObb2.m_Center = SrcObb.m_Center + SrcVelocity * mid;
			//OBBの各辺の長さを拡大
			SrcObb2.m_Size.x = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.x;
			SrcObb2.m_Size.y = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.y;
			SrcObb2.m_Size.z = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.z;
			if (!HitTest::OBB_COLRECT(SrcObb2, DestRect)) {
				return false;
			}
			if (EndTime - StartTime < m_EPSILON) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestObbRect(SrcObb, SrcVelocity, DestRect, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestObbRect(SrcObb, SrcVelocity, DestRect, mid, EndTime, HitTime);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphereと動かないSphereの衝突判定
		@param[in]	SrcSp	Srcの球
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestSp	Dest球
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereSphere(const SPHERE& SrcSp, const bsm::Vec3& SrcVelocity,
			const SPHERE& DestSp, float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			if (!HitTest::SPHERE_SPHERE(SrcSp2, DestSp)){
				return false;
			}
			if (EndTime - StartTime < m_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereSphere(SrcSp, SrcVelocity, DestSp, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestSphereSphere(SrcSp, SrcVelocity, DestSp, mid, EndTime, HitTime);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphereと動かないCylinderの衝突判定
		@param[in]	SrcSp	Srcの球
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestCy	Destシリンダー
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereCylinder(const SPHERE& SrcSp, const bsm::Vec3& SrcVelocity,
			const CYLINDER& DestCy,
			float StartTime, float EndTime, float& HitTime) {
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			bsm::Vec3 RetVec;
			if (!HitTest::SPHERE_CYLINDER(SrcSp2, DestCy, RetVec)) {
				return false;
			}
			if (EndTime - StartTime < m_EPSILON) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereCylinder(SrcSp, SrcVelocity, DestCy, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestSphereCylinder(SrcSp, SrcVelocity, DestCy, mid, EndTime, HitTime);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphereと動かないCapsuleの衝突判定
		@param[in]	SrcSp	Srcの球
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestCap	Destカプセル
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereCapsule(const SPHERE& SrcSp, const bsm::Vec3& SrcVelocity,
			const CAPSULE& DestCap,
			float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			bsm::Vec3 RetVec;
			if (!HitTest::SPHERE_CAPSULE(SrcSp2, DestCap, RetVec)){
				return false;
			}
			if (EndTime - StartTime < m_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereCapsule(SrcSp, SrcVelocity, DestCap, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestSphereCapsule(SrcSp, SrcVelocity, DestCap, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Capsuleと動かないCapsuleの衝突判定
		@param[in]	SrcCap	Srcのカプセル
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestCap	Destカプセル
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestCapsuleCapsule(const CAPSULE& SrcCapsule, const bsm::Vec3& SrcVelocity,
			const CAPSULE& DestCap,
			float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;

			CAPSULE SrcCapsule2 = SrcCapsule;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcCapsule2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcCapsule.m_Radius;
			float Scale = SrcCapsule2.m_Radius / SrcCapsule.m_Radius;

			//中心が原点の元のカプセルを作成
			CAPSULE SrcBaseCapsule = SrcCapsule;
			SrcBaseCapsule.SetCenter(bsm::Vec3(0, 0, 0));
			//原点カプセルでスケーリング
			//スケーリング行列の作成
			bsm::Mat4x4 ScalMat;
			ScalMat.scale(bsm::Vec3(Scale));
			//各頂点をスケーリング
			SrcCapsule2.m_PointBottom = SrcBaseCapsule.m_PointBottom * ScalMat;
			SrcCapsule2.m_PointTop = SrcBaseCapsule.m_PointTop * ScalMat;
			//中心を移動
			SrcCapsule2.SetCenter(SrcCapsule.GetCenter() + SrcVelocity * mid);

			bsm::Vec3 RetVec1, RetVec2;
			if (!HitTest::CAPSULE_CAPSULE(SrcCapsule2, DestCap, RetVec1, RetVec2)){
				return false;
			}
			if (EndTime - StartTime < m_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestCapsuleCapsule(SrcCapsule, SrcVelocity, DestCap, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestCapsuleCapsule(SrcCapsule, SrcVelocity, DestCap, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphereと動かないAabbの衝突判定
		@param[in]	SrcSp	Srcの球
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestAabb	DestのAABB
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereAabb(const SPHERE& SrcSp, const bsm::Vec3& SrcVelocity,
			const AABB& DestAabb,
			float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			bsm::Vec3 RetVec;
			if (!HitTest::SPHERE_AABB(SrcSp2, DestAabb, RetVec)){
				return false;
			}
			if (EndTime - StartTime < m_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereAabb(SrcSp, SrcVelocity, DestAabb, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestSphereAabb(SrcSp, SrcVelocity, DestAabb, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Sphereと動かないObbの衝突判定
		@param[in]	SrcSp	Srcの球
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestObb	DestのOBB
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestSphereObb(const SPHERE& SrcSp, const bsm::Vec3& SrcVelocity,
			const OBB& DestObb,
			float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcSp.m_Radius;
			bsm::Vec3 RetVec;
			if (!HitTest::SPHERE_OBB(SrcSp2, DestObb, RetVec)){
				return false;
			}
			if (EndTime - StartTime < m_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestSphereObb(SrcSp, SrcVelocity, DestObb, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestSphereObb(SrcSp, SrcVelocity, DestObb, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カプセルとObbの最近接点を得る
		@param[in]	cp	カプセル
		@param[in]	obb	OBB
		@param[out]	flg	2つ球のとの位置関係
		@return	最近接点
		*/
		//--------------------------------------------------------------------------------------
		static bsm::Vec3 ClosestPtCapsuleOBB(const CAPSULE& cp, const OBB& obb, int& flg){
			SPHERE Sp;
			Sp.m_Center = cp.m_PointBottom;
			Sp.m_Radius = cp.m_Radius;
			bsm::Vec3 retvec;
			//スタート位置で最近接点を得る
			HitTest::SPHERE_OBB(Sp, obb, retvec);
			//内積を図る
			bsm::Vec3 Base = cp.m_PointTop - cp.m_PointBottom;
			Base.normalize();
			bsm::Vec3 Dest = retvec - cp.m_PointBottom;
			float dot = bsm::dot(Base,Dest);
			if (dot < 0){
				//スタート位置の球体の外側
				//retvecは有効
				flg = -1;
				return retvec;
			}
			float  size = bsm::length(cp.m_PointTop - cp.m_PointBottom);
			if (dot > size){
				//終点より先にある
				Sp.m_Center = cp.m_PointTop;
				HitTest::SPHERE_OBB(Sp, obb, retvec);
				//終点で最近接点をとる
				flg = 1;
				return retvec;
			}
			//中心とobbの最近接点を得る
			HitTest::ClosestPtPointOBB(cp.GetCenter(), obb, retvec);
			float t;
			bsm::Vec3 SegPoint;
			HitTest::ClosetPtPointSegment(retvec, cp.m_PointBottom, cp.m_PointTop, t, SegPoint);
			bsm::Vec3 Span = retvec - SegPoint;
			Span.normalize();
			Span *= cp.m_Radius;
			SegPoint += Span;
			retvec = SegPoint;
			flg = 0;
			return retvec;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	カプセルとAabbの最近接点を得る
		@param[in]	cp	カプセル
		@param[in]	aabb	AABB
		@param[out]	flg	位置関係
		@return	最近接点
		*/
		//--------------------------------------------------------------------------------------
		static bsm::Vec3 ClosestPtCapsuleAABB(const CAPSULE& cp, const AABB& aabb, int& flg) {
			SPHERE Sp;
			Sp.m_Center = cp.m_PointBottom;
			Sp.m_Radius = cp.m_Radius;
			bsm::Vec3 retvec;
			//スタート位置で最近接点を得る
			HitTest::SPHERE_AABB(Sp, aabb, retvec);
			//内積を図る
			bsm::Vec3 Base = cp.m_PointTop - cp.m_PointBottom;
			Base.normalize();
			bsm::Vec3 Dest = retvec - cp.m_PointBottom;
			float dot = bsm::dot(Base,Dest);
			if (dot < 0) {
				//スタート位置の球体の外側
				//retvecは有効
				flg = -1;
				return retvec;
			}
			float  size = bsm::length(cp.m_PointTop - cp.m_PointBottom);
			if (dot > size) {
				//終点より先にある
				Sp.m_Center = cp.m_PointTop;
				HitTest::SPHERE_AABB(Sp, aabb, retvec);
				//終点で最近接点をとる
				flg = 1;
				return retvec;
			}
			//中心とaabbの最近接点を得る
			HitTest::ClosestPtPointAABB(cp.GetCenter(), aabb, retvec);
			float t;
			bsm::Vec3 SegPoint;
			HitTest::ClosetPtPointSegment(retvec, cp.m_PointBottom, cp.m_PointTop, t, SegPoint);
			bsm::Vec3 Span = retvec - SegPoint;
			Span.normalize();
			Span *= cp.m_Radius;
			SegPoint += Span;
			retvec = SegPoint;
			flg = 0;
			return retvec;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	カプセルとAABBの衝突判定
		@param[in]	cp	カプセル
		@param[in]	aabb	AABB
		@param[out]	retvec	最近接点
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CAPSULE_AABB(const CAPSULE& cp, const AABB& aabb, bsm::Vec3& retvec) {
			//スィープさせる球
			SPHERE StartSp, EndSp;
			StartSp.m_Center = cp.m_PointBottom;
			StartSp.m_Radius = cp.m_Radius;
			EndSp.m_Center = cp.m_PointTop;
			EndSp.m_Radius = cp.m_Radius;
			//各点とaabbの最近接点を得る
			//カプセルとAABBの最近接点を得る（衝突してるかどうかは関係ない）
			int flg;
			retvec = ClosestPtCapsuleAABB(cp, aabb, flg);
			float HitTime;
			bsm::Vec3 Velocity = EndSp.m_Center - StartSp.m_Center;
			if (CollisionTestSphereAabb(StartSp, Velocity, aabb, 0, 1.0f, HitTime)) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カプセルと動かないAABBの衝突判定
		@param[in]	SrcCapsule	Srcのカプセル
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestAabb	DestのAABB
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestCapsuleAabb(const CAPSULE& SrcCapsule, const bsm::Vec3& SrcVelocity,
			const AABB& DestAabb, float StartTime, float EndTime, float& HitTime) {
			const float m_EPSILON = 0.005f;
			CAPSULE SrcCapsule2 = SrcCapsule;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcCapsule2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcCapsule.m_Radius;
			float Scale = SrcCapsule2.m_Radius / SrcCapsule.m_Radius;
			//中心が原点の元のカプセルを作成
			CAPSULE SrcBaseCapsule = SrcCapsule;
			SrcBaseCapsule.SetCenter(bsm::Vec3(0, 0, 0));
			//原点カプセルでスケーリング
			//スケーリング行列の作成
			bsm::Mat4x4 ScalMat;
			ScalMat.scale(bsm::Vec3(Scale));
			//各頂点をスケーリング
			SrcCapsule2.m_PointBottom = SrcBaseCapsule.m_PointBottom * ScalMat;
			SrcCapsule2.m_PointTop = SrcBaseCapsule.m_PointTop * ScalMat;
			//中心を移動
			SrcCapsule2.SetCenter(SrcCapsule.GetCenter() + SrcVelocity * mid);
			bsm::Vec3 RetVec;
			if (!HitTest::CAPSULE_AABB(SrcCapsule2, DestAabb, RetVec)) {
				return false;
			}
			if (EndTime - StartTime < m_EPSILON) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestCapsuleAabb(SrcCapsule, SrcVelocity, DestAabb, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestCapsuleAabb(SrcCapsule, SrcVelocity, DestAabb, mid, EndTime, HitTime);
		}



		//--------------------------------------------------------------------------------------
		/*!
		@brief	カプセルとObbの衝突判定
		@param[in]	cp	カプセル
		@param[in]	obb	OBB
		@param[out]	retvec	最近接点
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CAPSULE_OBB(const CAPSULE& cp, const OBB& obb, bsm::Vec3& retvec){
			//スィープさせる球
			SPHERE StartSp, EndSp;
			StartSp.m_Center = cp.m_PointBottom;
			StartSp.m_Radius = cp.m_Radius;
			EndSp.m_Center = cp.m_PointTop;
			EndSp.m_Radius = cp.m_Radius;
			//各点とobbの最近接点を得る
			//カプセルとOBBの最近接点を得る（衝突してるかどうかは関係ない）
			int flg;
			retvec = ClosestPtCapsuleOBB(cp, obb,flg);
			float HitTime;
			bsm::Vec3 Velocity = EndSp.m_Center - StartSp.m_Center;
			if (CollisionTestSphereObb(StartSp, Velocity,obb, 0,1.0f, HitTime)){
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カプセルと動かないObbの衝突判定
		@param[in]	SrcCapsule	Srcのカプセル
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestObb	DestのOBB
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestCapsuleObb(const CAPSULE& SrcCapsule, const bsm::Vec3& SrcVelocity,
			const OBB& DestObb, float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			CAPSULE SrcCapsule2 = SrcCapsule;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcCapsule2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcCapsule.m_Radius;
			float Scale = SrcCapsule2.m_Radius / SrcCapsule.m_Radius;
			//中心が原点の元のカプセルを作成
			CAPSULE SrcBaseCapsule = SrcCapsule;
			SrcBaseCapsule.SetCenter(bsm::Vec3(0, 0, 0));
			//原点カプセルでスケーリング
			//スケーリング行列の作成
			bsm::Mat4x4 ScalMat;
			ScalMat.scale(bsm::Vec3(Scale));
			//各頂点をスケーリング
			SrcCapsule2.m_PointBottom = SrcBaseCapsule.m_PointBottom * ScalMat;
			SrcCapsule2.m_PointTop = SrcBaseCapsule.m_PointTop * ScalMat;
			//中心を移動
			SrcCapsule2.SetCenter(SrcCapsule.GetCenter() + SrcVelocity * mid);
			bsm::Vec3 RetVec;
			if (!HitTest::CAPSULE_OBB(SrcCapsule2, DestObb, RetVec)){
				return false;
			}
			if (EndTime - StartTime < m_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestCapsuleObb(SrcCapsule, SrcVelocity, DestObb, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestCapsuleObb(SrcCapsule, SrcVelocity, DestObb, mid, EndTime, HitTime);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	CAPSULEとTRIANGLEとの衝突判定
		@param[in]	cp	CAPSULE
		@param[in]	rect	COLRECT
		@param[in]	retvec	最近接点
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CAPSULE_TRIANGLE(const CAPSULE& cp, const TRIANGLE& tri, bsm::Vec3& retvec) {
			//スィープさせる球
			SPHERE StartSp, EndSp;
			//下から上
			StartSp.m_Center = cp.m_PointBottom;
			StartSp.m_Radius = cp.m_Radius;
			EndSp.m_Center = cp.m_PointTop;
			EndSp.m_Radius = cp.m_Radius;

			PLANE p = tri.GetPLANE();
			float t;
			bsm::Vec3 q;
			SEGMENT_PLANE(StartSp.m_Center, EndSp.m_Center, p, t, q);
			//仮に下の点で初期化
			bsm::Vec3 Centor = StartSp.m_Center;
			if (t <= 0) {
				Centor = StartSp.m_Center;
			}
			else if (t >= 1.0f) {
				Centor = EndSp.m_Center;
			}
			else {
				Centor = q;
			}
			//Centerは、線上の面との最近接点
			//点とTRIANGLEの最近接点を得る（衝突してるかどうかは関係ない）
			ClosestPtPointTriangle(Centor, tri, retvec);
			float HitTime;
			bsm::Vec3 Velocity = EndSp.m_Center - StartSp.m_Center;
			if (CollisionTestSphereTriangle(StartSp, Velocity, tri, 0, 1.0f, HitTime)) {
				return true;
			}
			return false;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Capsuleと動かないTriangleの衝突判定
		@param[in]	SrcCapsule	Srcのカプセル
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestTri		Dest三角形
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestCapsuleTriangle(const CAPSULE& SrcCapsule, const bsm::Vec3& SrcVelocity,
			const TRIANGLE& DestTri, float StartTime, float EndTime, float& HitTime) {
			const float m_EPSILON = 0.005f;
			CAPSULE SrcCapsule2 = SrcCapsule;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcCapsule2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcCapsule.m_Radius;
			float Scale = SrcCapsule2.m_Radius / SrcCapsule.m_Radius;
			//中心が原点の元のカプセルを作成
			CAPSULE SrcBaseCapsule = SrcCapsule;
			SrcBaseCapsule.SetCenter(bsm::Vec3(0, 0, 0));
			//原点カプセルでスケーリング
			//スケーリング行列の作成
			bsm::Mat4x4 ScalMat;
			ScalMat.scale(bsm::Vec3(Scale));
			//各頂点をスケーリング
			SrcCapsule2.m_PointBottom = SrcBaseCapsule.m_PointBottom * ScalMat;
			SrcCapsule2.m_PointTop = SrcBaseCapsule.m_PointTop * ScalMat;
			//中心を移動
			SrcCapsule2.SetCenter(SrcCapsule.GetCenter() + SrcVelocity * mid);
			bsm::Vec3 RetVec;
			if (!HitTest::CAPSULE_TRIANGLE(SrcCapsule2, DestTri, RetVec)) {
				return false;
			}
			if (EndTime - StartTime < m_EPSILON) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestCapsuleTriangle(SrcCapsule, SrcVelocity, DestTri, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestCapsuleTriangle(SrcCapsule, SrcVelocity, DestTri, mid, EndTime, HitTime);
		}




		//--------------------------------------------------------------------------------------
		/*!
		@brief	CAPSULEとCOLRECTとの衝突判定
		@param[in]	cp	CAPSULE
		@param[in]	rect	COLRECT
		@param[in]	retvec	最近接点
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CAPSULE_COLRECT(const CAPSULE& cp, const COLRECT& rect, bsm::Vec3& retvec) {
			//スィープさせる球
			SPHERE StartSp, EndSp;
			//下から上
			StartSp.m_Center = cp.m_PointBottom;
			StartSp.m_Radius = cp.m_Radius;
			EndSp.m_Center = cp.m_PointTop;
			EndSp.m_Radius = cp.m_Radius;

			PLANE p = rect.GetPLANE();
			float t;
			bsm::Vec3 q;
			SEGMENT_PLANE(StartSp.m_Center, EndSp.m_Center, p, t, q);
			//仮に下の点で初期化
			bsm::Vec3 Centor = StartSp.m_Center;
			if (t <= 0) {
				Centor = StartSp.m_Center;
			}
			else if (t >= 1.0f) {
				Centor = EndSp.m_Center;
			}
			else {
				Centor = q;
			}
			//Centerは、線上の面との最近接点
			//球ととCOLRECTの最近接点を得る（衝突してるかどうかは関係ない）
			ClosestPtPointCOLRECT(Centor, rect, retvec);
			float HitTime;
			bsm::Vec3 Velocity = EndSp.m_Center - StartSp.m_Center;
			if (CollisionTestSphereRect(StartSp, Velocity, rect, 0, 1.0f, HitTime)) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Capsuleと動かないRectの衝突判定
		@param[in]	SrcCapsule	Srcの球
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestRect	Dest矩形
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestCapsuleRect(const CAPSULE& SrcCapsule, const bsm::Vec3& SrcVelocity,
			const COLRECT& DestRect, float StartTime, float EndTime, float& HitTime) {
			const float m_EPSILON = 0.005f;
			CAPSULE SrcCapsule2 = SrcCapsule;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcCapsule2.m_Radius = (mid - StartTime) * bsm::length(SrcVelocity) + SrcCapsule.m_Radius;
			float Scale = SrcCapsule2.m_Radius / SrcCapsule.m_Radius;
			//中心が原点の元のカプセルを作成
			CAPSULE SrcBaseCapsule = SrcCapsule;
			SrcBaseCapsule.SetCenter(bsm::Vec3(0, 0, 0));
			//原点カプセルでスケーリング
			//スケーリング行列の作成
			bsm::Mat4x4 ScalMat;
			ScalMat.scale(bsm::Vec3(Scale));
			//各頂点をスケーリング
			SrcCapsule2.m_PointBottom = SrcBaseCapsule.m_PointBottom * ScalMat;
			SrcCapsule2.m_PointTop = SrcBaseCapsule.m_PointTop * ScalMat;
			//中心を移動
			SrcCapsule2.SetCenter(SrcCapsule.GetCenter() + SrcVelocity * mid);
			bsm::Vec3 RetVec;
			if (!HitTest::CAPSULE_COLRECT(SrcCapsule2, DestRect, RetVec)) {
				return false;
			}
			if (EndTime - StartTime < m_EPSILON) {
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestCapsuleRect(SrcCapsule, SrcVelocity, DestRect, StartTime, mid, HitTime)) {
				return true;
			}
			return CollisionTestCapsuleRect(SrcCapsule, SrcVelocity, DestRect, mid, EndTime, HitTime);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBBと動かないOBBの衝突判定（サブ関数として実装）
		@param[in]	SrcObb	SrcのObb
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestObb	DestのOBB
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestObbObbSub(const OBB& SrcObb, const bsm::Vec3& SrcVelocity,
			const OBB& DestObb, float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			OBB SrcObb2 = SrcObb;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcObb2.m_Center = SrcObb.m_Center + SrcVelocity * mid;
			//OBBの各辺の長さを拡大
			SrcObb2.m_Size.x = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.x;
			SrcObb2.m_Size.y = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.y;
			SrcObb2.m_Size.z = (mid - StartTime) * bsm::length(SrcVelocity) + SrcObb.m_Size.z;
			if (!HitTest::OBB_OBB(SrcObb2, DestObb)){
				return false;
			}
			if (EndTime - StartTime < m_EPSILON){
				HitTime = StartTime;
				return true;
			}
			if (CollisionTestObbObbSub(SrcObb, SrcVelocity, DestObb, StartTime, mid, HitTime)){
				return true;
			}
			return CollisionTestObbObbSub(SrcObb, SrcVelocity, DestObb, mid, EndTime, HitTime);

		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBBと動かないOBBの衝突判定（SrcObbはSPHERE化できるOBB）
		@param[in]	SrcObb	SrcのObb
		@param[in]	SrcVelocity	ソース速度
		@param[in]	DestObb	DestのOBB
		@param[in]	StartTime	開始時間
		@param[in]	EndTime	終了時間
		@param[out]	HitTime	ヒット時間
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool CollisionTestObbObb(const OBB& SrcObb, const bsm::Vec3& SrcVelocity,
			const OBB& DestObb, float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE BeforeSrcSphere;
			BeforeSrcSphere.m_Center = SrcObb.m_Center;
			BeforeSrcSphere.m_Radius = bsm::length(SrcObb.m_Size);
			if (!CollisionTestSphereObb(BeforeSrcSphere, SrcVelocity, DestObb, StartTime, EndTime, HitTime)){
				//衝突がなければOBBも衝突なし
				return false;
			}
			return CollisionTestObbObbSub(SrcObb, SrcVelocity,DestObb, StartTime, EndTime,HitTime);
		}
	};

	inline AABB CAPSULE::GetWrappedAABB() const {
		SPHERE sp_bottom, sp_top;
		sp_bottom.m_Center = m_PointBottom;
		sp_bottom.m_Radius = m_Radius;
		sp_top.m_Center = m_PointBottom;
		sp_top.m_Radius = m_Radius;
		return HitTest::AABB_OR_AABB(sp_bottom.GetWrappedAABB(), sp_top.GetWrappedAABB());

	}


}
//end of namespace basecross.


