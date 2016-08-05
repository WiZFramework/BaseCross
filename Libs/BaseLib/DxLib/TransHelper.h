/*!
@file TransHelper.h
@brief 衝突判定、補間処理用ユーティリティ
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
	///	衝突判定ボリュームの基底クラス
	//--------------------------------------------------------------------------------------
	struct CollisionVolume{
		CollisionVolume(){}
		virtual ~CollisionVolume(){}
	};
	//--------------------------------------------------------------------------------------
	///	OBBボリューム境界
	//--------------------------------------------------------------------------------------
	struct OBB : public CollisionVolume{
		Vector3 m_Center;     ///< 中心点の座標
		Vector3 m_Rot[3];  ///< XYZ の各座標軸の傾きを表す方向ベクトル
		Vector3 m_Size;     ///< OBB の各座標軸に沿った長さの半分（中心点から面までの長さ）
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成されたサイズと変換行列から、OBBを作り直す
		@param[in]	Size	作成サイズ
		@param[in]	Matrix	変換行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CreateOBB(const Vector3& Size,const Matrix4X4& Matrix){
			m_Center.x = Matrix._41;
			m_Center.y = Matrix._42;
			m_Center.z = Matrix._43;
			Vector3 VecX(Matrix._11,Matrix._12,Matrix._13);
			Vector3 VecY(Matrix._21,Matrix._22,Matrix._23);
			Vector3 VecZ(Matrix._31,Matrix._32,Matrix._33);
			m_Size = Vector3(
				Size.x * VecX.Length(),
				Size.y * VecY.Length(),
				Size.z * VecZ.Length()
				);
			m_Size *= 0.5f;
			//回転を得る
			m_Rot[0] = Vector3EX::Normalize(VecX);
			m_Rot[1] = Vector3EX::Normalize(VecY);
			m_Rot[2] = Vector3EX::Normalize(VecZ);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		OBB(){
			//デフォルトは1辺1.0のOBB
			Vector3 SizeVec(1.0f,1.0f,1.0f);
			//行列はアイデンティティ
			Matrix4X4 Matrix;
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
		OBB(const Vector3& Size,const Vector3& Rot,const Vector3& Center):
			m_Center(Center),
			m_Size(Size)
		{
			m_Size *= 0.5f;
			//回転を得る
			Vector3 LocaRot = Rot;
			LocaRot.Normalize();
			Matrix4X4 mRot;
			mRot.Identity();
			mRot.RotationRollPitchYawFromVector(LocaRot);
			m_Rot[0] = Vector3(mRot._11,mRot._12,mRot._13);
			m_Rot[1] = Vector3(mRot._21,mRot._22,mRot._23);
			m_Rot[2] = Vector3(mRot._31,mRot._32,mRot._33);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Size	サイズ
		@param[in]	Qt	回転クオータニオン
		@param[in]	Center	中心位置
		*/
		//--------------------------------------------------------------------------------------
		OBB(const Vector3& Size,const Quaternion& Qt,const Vector3& Center):
			m_Center(Center),
			m_Size(Size)
		{
			m_Size *= 0.5f;
			//回転を得る
			Quaternion LocalQt = Qt;
			//クオータニオンを正規化
			LocalQt.Normalize();
			//クオータニオンから回転行列を作成
			Matrix4X4 mRot;
			mRot.Identity();
			mRot.RotationQuaternion(LocalQt);
			m_Rot[0] = Vector3(mRot._11,mRot._12,mRot._13);
			m_Rot[1] = Vector3(mRot._21,mRot._22,mRot._23);
			m_Rot[2] = Vector3(mRot._31,mRot._32,mRot._33);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ（作成されたサイズと変換行列から、OBBを作成）
		@param[in]	Size	サイズ
		@param[in]	Matrix	変換行列
		*/
		//--------------------------------------------------------------------------------------
		OBB(const Vector3& Size,const Matrix4X4& Matrix)
		{
			m_Center.x = Matrix._41;
			m_Center.y = Matrix._42;
			m_Center.z = Matrix._43;
			Vector3 VecX(Matrix._11,Matrix._12,Matrix._13);
			Vector3 VecY(Matrix._21,Matrix._22,Matrix._23);
			Vector3 VecZ(Matrix._31,Matrix._32,Matrix._33);
			m_Size = Vector3(
				Size.x * VecX.Length(),
				Size.y * VecY.Length(),
				Size.z * VecZ.Length()
				);
			m_Size *= 0.5f;
			//回転を得る
			m_Rot[0] = Vector3EX::Normalize(VecX);
			m_Rot[1] = Vector3EX::Normalize(VecY);
			m_Rot[2] = Vector3EX::Normalize(VecZ);

		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	回転行列を得る
		@return	回転行列
		*/
		//--------------------------------------------------------------------------------------
		Matrix4X4 GetRotMatrix(){
			Matrix4X4 ret;
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
		@brief	一番大きい辺の長さを得る
		@return	一番大きい辺の長さ
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxSize(){
			float ret = m_Size.x;
			if(ret < m_Size.y){
				ret = m_Size.y;
			}
			if(ret < m_Size.z){
				ret = m_Size.z;
			}
			return ret * 2.0f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	一番小さい辺の長さを得る
		@return	一番小さい辺の長さ
		*/
		//--------------------------------------------------------------------------------------
		float GetMinSize(){
			float ret = m_Size.x;
			if(ret > m_Size.y){
				ret = m_Size.y;
			}
			if(ret > m_Size.z){
				ret = m_Size.z;
			}
			return ret;
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
		void GetNearNormalRot(const Vector3& Check, float Epsilon,Vector3& Ret){
			for (int i = 0; i < 3; i++){
				if (Vector3EX::NearEqual(Check, m_Rot[i], Epsilon)){
					Ret = m_Rot[i];
					return;
				}
				if (Vector3EX::NearEqual(Check, -m_Rot[i], Epsilon)){
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
			if (m_Rot[0] == Vector3(1.0f, 0.0f, 0.0f) && m_Rot[1] == Vector3(0.0f, 1.0f, 0.0f) && m_Rot[2] == Vector3(0.0f, 0.0f, 1.0f)){
				return true;
			}
			return false;
		}
	};

	//--------------------------------------------------------------------------------------
	///	AABBボリューム境界
	//--------------------------------------------------------------------------------------
	struct AABB : public CollisionVolume{
		Vector3 m_Min;     ///< 小さいほうの座標
		Vector3 m_Max;     ///< 大きいほうの座標
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		AABB(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Min	小さいほうの座標
		@param[in]	Max	大きいほうの座標
		*/
		//--------------------------------------------------------------------------------------
		AABB(const Vector3& Min,const Vector3& Max)
			:m_Min(Min),m_Max(Max){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Center	中心
		@param[in]	XSize	X方向サイズ
		@param[in]	YSize	Y方向サイズ
		@param[in]	ZSize	Z方向サイズ
		*/
		//--------------------------------------------------------------------------------------
		AABB(const Vector3& Center,float XSize,float YSize,float ZSize){
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
		void GetCenter(Vector3& Pos){
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
		Vector3 GetCenter(){
			Vector3 ret;
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
		void Move(const Vector3& MoveVec){
			m_Min += MoveVec;
			m_Max += MoveVec;
		}
	};


	//--------------------------------------------------------------------------------------
	///	球ボリューム境界
	//--------------------------------------------------------------------------------------
	struct SPHERE : public CollisionVolume
	{
		Vector3 m_Center;	///< 中心点の座標
		float m_Radius;			///< 半径
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		SPHERE(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Center	中心
		@param[in]	Radius	半径
		*/
		//--------------------------------------------------------------------------------------
		SPHERE(const Vector3& Center,float Radius):
			m_Center(Center),
			m_Radius(Radius){}
	};

	//--------------------------------------------------------------------------------------
	///	球配列データ
	//--------------------------------------------------------------------------------------
	struct SPHERE_ARR : public CollisionVolume{
		vector<SPHERE> m_SphereArr;
	};

	//--------------------------------------------------------------------------------------
	///	平面
	//--------------------------------------------------------------------------------------
	struct PLANE : public CollisionVolume{
		Vector3 m_Normal;	///< 法線
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
		void Set(const Vector3& PointA,const Vector3& PointB,const Vector3& PointC){
			m_Normal = Vector3EX::Normalize(Vector3EX::Cross(PointB - PointA,PointC - PointA));
			m_DotValue = Vector3EX::Dot(m_Normal,PointA);
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
		PLANE(const Vector3& PointA,const Vector3& PointB,const Vector3& PointC){
			Set(PointA,PointB,PointC);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ（Plane型から平面を作成）
		@param[in]	plane	Plane型の値
		*/
		//--------------------------------------------------------------------------------------
		PLANE(const Plane& plane){
			m_Normal.x = plane.x;
			m_Normal.y = plane.x;
			m_Normal.z = plane.z;
			m_DotValue = -plane.w;
		}
	};


	//--------------------------------------------------------------------------------------
	///	カプセルボリューム境界
	//--------------------------------------------------------------------------------------
	struct CAPSULE  : public CollisionVolume{
		float m_Radius;			///< 半径
		Vector3 m_PointA;		///< 中間部線分の開始点
		Vector3 m_PointB;		///< 中間部線分の終了点
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE():
			m_Radius(0.5f),
			m_PointA(0,-0.5f,0),
			m_PointB(0,0.5f,0)
		{
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Radius	半径
		@param[in]	PointA	中間部線分の開始点
		@param[in]	PointB	中間部線分の終了点
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE(float Radius,const Vector3& PointA,const Vector3& PointB):
			m_Radius(Radius),
			m_PointA(PointA),
			m_PointB(PointB)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	MakedRadius	作成時の半径
		@param[in]	MakedPointA	作成時の中間部線分の開始点
		@param[in]	MakedPointB	作成時の中間部線分の終了点
		@param[in]	Matrix	変換行列
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE(float MakedRadius,const Vector3& MakedPointA,const Vector3& MakedPointB,
			const Matrix4X4& Matrix):
			m_Radius(MakedRadius),
			m_PointA(MakedPointA),
			m_PointB(MakedPointB)
		{
			m_PointA.Transform(Matrix);
			m_PointB.Transform(Matrix);
			//スケーリングのみ1方向で計算
			Vector3 Scale = Matrix.ScaleInMatrix();
			m_Radius *= Scale.x;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心点を得る
		@return	中心点
		*/
		//--------------------------------------------------------------------------------------
		Vector3 GetCenter() const{
			return m_PointA + ((m_PointB - m_PointA) * 0.5f);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心点を設定する
		@param[in]	Center	中心点
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetCenter(const Vector3& Center){
			Vector3 CenterToPointA = ((m_PointA - m_PointB) * 0.5f);
			Vector3 CenterToPointB = ((m_PointB - m_PointA) * 0.5f);
			m_PointA = Center + CenterToPointA;
			m_PointB = Center + CenterToPointB;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	縦の半径を得る
		@return	縦の半径
		*/
		//--------------------------------------------------------------------------------------
		float GetHeightRadius()const{
			float PointLen = Vector3EX::Length(m_PointB - m_PointA) * 0.5f;
			PointLen += m_Radius;
			return PointLen;
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
		@brief	点が面の裏側にあるかどうか判定
		@param[in]	p	検証する点
		@param[in]	pl	面
		@return	裏側ならtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool InsidePtPlane(const Vector3& p, const PLANE& pl){
			float dist = Vector3EX::Dot(p,pl.m_Normal) - pl.m_DotValue;
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
			const Vector3& p1,const Vector3& q1,const Vector3& p2,const Vector3& q2,
			float& s,float& t,Vector3& c1,Vector3& c2){
			const float EPSILON = 1.175494e-37f;
			Vector3 d1 = q1 - p1;
			Vector3 d2 = q2 - p2;
			Vector3 r = p1 - p2;
			float a = Vector3EX::Dot(d1,d1);
			float e = Vector3EX::Dot(d2,d2);
			float f = Vector3EX::Dot(d2,r);

			if(f <= EPSILON && e <= EPSILON){
				s = t = 0.0f;
				c1 = p1;
				c2 = p2;
				return Vector3EX::Dot(c1 - c2,c1 - c2);
			}
			if(a <= EPSILON){
				s = 0.0f;
				t = f / e;
				t = floatClamp(t,0.0f,1.0f);
			}
			else{
				float c = Vector3EX::Dot(d1,r);
				if(e <= EPSILON){
					t = 0.0f;
					s = floatClamp(-c / a,0.0f,1.0f);
				}
				else{
					float b = Vector3EX::Dot(d1,d2);
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
			return Vector3EX::Dot(c1 - c2,c1 - c2);
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
		static float SqDistPointSegment(const Vector3& a, const Vector3& b, const Vector3& c){
			Vector3 ab = b - a;
			Vector3 ac = c - a;
			Vector3 bc = c - b;
			float e = Vector3EX::Dot(ac,ab);
			if(e <= 0.0f){
				return Vector3EX::Dot(ac,ac);
			}
			float f = Vector3EX::Dot(ab,ab);
			if(e >= f){
				return Vector3EX::Dot(bc,bc);
			}
			return Vector3EX::Dot(ac,ac) - e * e / f;
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
		static void ClosetPtPointSegment(const Vector3& c,
			const Vector3& a,const Vector3& b,
			float& t,Vector3& d){
			Vector3 ab = b - a;
			t = Vector3EX::Dot(c - a,ab) / Vector3EX::Dot(ab,ab);
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
		@brief	球とカプセルとの衝突判定
		@param[in]	sp	球
		@param[in]	cap	カプセル
		@param[out]	d	最近接点の戻り値
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_CAPSULE(const SPHERE& sp, const CAPSULE& cap, Vector3& d){
			//まず最近接点を設定
			float t;
			Vector3 ClosetPoint;
			ClosetPtPointSegment(sp.m_Center, cap.m_PointA, cap.m_PointB, t, ClosetPoint);
			//ClosetPointは、カプセル線分上の点である
			Vector3 Normal = sp.m_Center - ClosetPoint;
			Normal.Normalize();
			Normal *= cap.m_Radius;
			//最近接点を設定
			d = ClosetPoint + Normal;
			//衝突しているか判別
			float dist2 = SqDistPointSegment(cap.m_PointA,cap.m_PointB,sp.m_Center);
			float radius = sp.m_Radius + cap.m_Radius;
			return dist2 <= radius * radius;
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
			Vector3& retvec1,
			Vector3& retvec2){
			float s,t;
			float dist2 = ClosestPtSegmentSegment(cap1.m_PointA,cap1.m_PointB,cap2.m_PointA,cap2.m_PointB,
				s,t,retvec1,retvec2);
			float radius = cap1.m_Radius + cap2.m_Radius;
			return dist2 <= radius * radius;
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
		static void ClosestPtPointOBB(const Vector3& point, const OBB& obb, Vector3& retvec){
			Vector3 d = point - obb.m_Center;
			retvec = obb.m_Center;
			float dist;
			for(int i = 0; i < 3; i++)
			{
				dist = Vector3EX::Dot(d,obb.m_Rot[i]);
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
			Vector3 d = sp1.m_Center - sp2.m_Center;
			float dist2 = Vector3EX::Dot(d,d);
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
		static void ClosestPtPointAABB(const Vector3& p, const AABB& b,
			Vector3& retvec){
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
		static float SqDistPointAABB(const Vector3& p, const AABB& b){
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
			Vector3& retvec){
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
		@brief	球とOBBとの衝突判定
		@param[in]	sp	球
		@param[in]	obb	OBB
		@param[out]	retvec	最近接点が代入される参照
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool SPHERE_OBB(const SPHERE& sp, const OBB& obb, Vector3& retvec){
			ClosestPtPointOBB(sp.m_Center,obb,retvec);
			Vector3 v = retvec - sp.m_Center;
			return Vector3EX::Dot(v,v) <= sp.m_Radius * sp.m_Radius;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OBBとOBBとの衝突判定
		@param[in]	obb1	OBB1
		@param[in]	obb2	OBB2
		@return	衝突していればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool OBB_OBB(const OBB& obb1, const OBB& obb2){
			const float EPSILON = 1.175494e-37f;
			float R[3][3], AbsR[3][3];
			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++){
					R[i][j] = Vector3EX::Dot(obb1.m_Rot[i], obb2.m_Rot[j]);
					AbsR[i][j] = fabsf(R[i][j]) + EPSILON;
				}
			}
			Vector3 t = obb2.m_Center - obb1.m_Center;
			t = Vector3(Vector3EX::Dot(t, obb1.m_Rot[0]),
							Vector3EX::Dot(t, obb1.m_Rot[1]),
							Vector3EX::Dot(t, obb1.m_Rot[2]));
			//軸L=A0, L=A1, L=A2判定
			float ra, rb;
			for(int i = 0; i < 3; i++){
				ra = obb1.m_Size[i];
				rb = obb2.m_Size[0] * AbsR[i][0] + obb2.m_Size[1] * AbsR[i][1] + obb2.m_Size[2] * AbsR[i][2];
				if(fabsf(t[i]) > ra + rb){
					return false;
				}
			}
			//軸L=B0, L=B1, L=B2判定
			for(int i = 0; i < 3; i++){
				ra = obb1.m_Size[0] * AbsR[0][i] + obb1.m_Size[1] * AbsR[1][i] + obb1.m_Size[2] * AbsR[2][i];
				rb = obb2.m_Size[i];
				if(fabsf(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb){
					return false;
				}
			}
			//軸L=A0 X B0判定
			ra = obb1.m_Size[1] * AbsR[2][0] + obb1.m_Size[2] * AbsR[1][0];
			rb = obb2.m_Size[1] * AbsR[0][2] + obb2.m_Size[2] * AbsR[0][1];
			if(fabsf(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb){
				return false;
			}
			//軸L=A0 X B1判定
			ra = obb1.m_Size[1] * AbsR[2][1] + obb1.m_Size[2] * AbsR[1][1];
			rb = obb2.m_Size[0] * AbsR[0][2] + obb2.m_Size[2] * AbsR[0][0];
			if(fabsf(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb){
				return false;
			}
			//軸L=A0 X B2判定
			ra = obb1.m_Size[1] * AbsR[2][2] + obb1.m_Size[2] * AbsR[1][2];
			rb = obb2.m_Size[0] * AbsR[0][1] + obb2.m_Size[1] * AbsR[0][0];
			if(fabsf(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb){
				return false;
			}
			//軸L=A1 X B0判定
			ra = obb1.m_Size[0] * AbsR[2][0] + obb1.m_Size[2] * AbsR[0][0];
			rb = obb2.m_Size[1] * AbsR[1][2] + obb2.m_Size[2] * AbsR[1][1];
			if(fabsf(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb){
				return false;
			}
			//軸L=A1 X B1判定
			ra = obb1.m_Size[0] * AbsR[2][1] + obb1.m_Size[2] * AbsR[0][1];
			rb = obb2.m_Size[0] * AbsR[1][2] + obb2.m_Size[2] * AbsR[1][0];
			if(fabsf(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb){
				return false;
			}
			//軸L=A1 X B2判定
			ra = obb1.m_Size[0] * AbsR[2][2] + obb1.m_Size[2] * AbsR[0][2];
			rb = obb2.m_Size[0] * AbsR[1][1] + obb2.m_Size[1] * AbsR[1][0];
			if(fabsf(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb){
				return false;
			}
			//軸L=A2 X B0判定
			ra = obb1.m_Size[0] * AbsR[1][0] + obb1.m_Size[1] * AbsR[0][0];
			rb = obb2.m_Size[1] * AbsR[2][2] + obb2.m_Size[2] * AbsR[2][1];
			if(fabsf(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb){
				return false;
			}
			//軸L=A2 X B1判定
			ra = obb1.m_Size[0] * AbsR[1][1] + obb1.m_Size[1] * AbsR[0][1];
			rb = obb2.m_Size[0] * AbsR[2][2] + obb2.m_Size[2] * AbsR[2][0];
			if(fabsf(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb){
				return false;
			}
			//軸L=A2 X B2判定
			ra = obb1.m_Size[0] * AbsR[1][2] + obb1.m_Size[1] * AbsR[0][2];
			rb = obb2.m_Size[0] * AbsR[2][1] + obb2.m_Size[1] * AbsR[2][0];
			if(fabsf(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb){
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
		static bool SEGMENT_PLANE(const Vector3& a, const Vector3& b,
			const PLANE& p,float& t,Vector3& q){
			Vector3 ab = b - a;
			t = (p.m_DotValue - Vector3EX::Dot(p.m_Normal,a)) / Vector3EX::Dot(p.m_Normal,ab);
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
		static bool SEGMENT_AABB(const Vector3& p0, const Vector3& p1, const AABB& b){
			const float EPSILON = 1.175494e-37f;
			Vector3 c = (b.m_Min + b.m_Max) * 0.5f;
			Vector3 e = b.m_Max - c;
			Vector3 m = (p0 + p1) * 0.5f;
			Vector3 d = p1 - m;
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
		static bool SEGMENT_OBB(const Vector3& p0, const Vector3& p1, const OBB& obb){
			const float EPSILON = 1.175494e-37f;

			Vector3 m = (p0 + p1) * 0.5f;
			Vector3 d = p1 - m;
			m = m - obb.m_Center;
			m = Vector3(Vector3EX::Dot(obb.m_Rot[0], m), Vector3EX::Dot(obb.m_Rot[1], m), Vector3EX::Dot(obb.m_Rot[2], m));
			d = Vector3(Vector3EX::Dot(obb.m_Rot[0], d), Vector3EX::Dot(obb.m_Rot[1], d), Vector3EX::Dot(obb.m_Rot[2], d));

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
		static bool CollisionTestSphereSphere(const SPHERE& SrcSp, const Vector3& SrcVelocity,
			const SPHERE& DestSp, float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * SrcVelocity.Length() + SrcSp.m_Radius;
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
		static bool CollisionTestSphereCapsule(const SPHERE& SrcSp, const Vector3& SrcVelocity,
			const CAPSULE& DestCap,
			float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * SrcVelocity.Length() + SrcSp.m_Radius;
			Vector3 RetVec;
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
		static bool CollisionTestCapsuleCapsule(const CAPSULE& SrcCapsule, const Vector3& SrcVelocity,
			const CAPSULE& DestCap,
			float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;

			CAPSULE SrcCapsule2 = SrcCapsule;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcCapsule2.m_Radius = (mid - StartTime) * SrcVelocity.Length() + SrcCapsule.m_Radius;
			float Scale = SrcCapsule2.m_Radius / SrcCapsule.m_Radius;

			//中心が原点の元のカプセルを作成
			CAPSULE SrcBaseCapsule = SrcCapsule;
			SrcBaseCapsule.SetCenter(Vector3(0, 0, 0));
			//原点カプセルでスケーリング
			//スケーリング行列の作成
			Matrix4X4 ScalMat;
			ScalMat.Scaling(Scale, Scale, Scale);
			//各頂点をスケーリング
			SrcCapsule2.m_PointA = Vector3EX::Transform(SrcBaseCapsule.m_PointA, ScalMat);
			SrcCapsule2.m_PointB = Vector3EX::Transform(SrcBaseCapsule.m_PointB, ScalMat);
			//中心を移動
			SrcCapsule2.SetCenter(SrcCapsule.GetCenter() + SrcVelocity * mid);

			Vector3 RetVec1, RetVec2;
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
		static bool CollisionTestSphereAabb(const SPHERE& SrcSp, const Vector3& SrcVelocity,
			const AABB& DestAabb,
			float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * SrcVelocity.Length() + SrcSp.m_Radius;
			Vector3 RetVec;
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
		static bool CollisionTestSphereObb(const SPHERE& SrcSp, const Vector3& SrcVelocity,
			const OBB& DestObb,
			float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE SrcSp2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcSp2.m_Center = SrcSp.m_Center + SrcVelocity * mid;
			SrcSp2.m_Radius = (mid - StartTime) * SrcVelocity.Length() + SrcSp.m_Radius;
			Vector3 RetVec;
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
		static Vector3 ClosestPtCapsuleOBB(const CAPSULE& cp, const OBB& obb, int& flg){
			SPHERE Sp;
			Sp.m_Center = cp.m_PointA;
			Sp.m_Radius = cp.m_Radius;
			Vector3 retvec;
			//スタート位置で最近接点を得る
			HitTest::SPHERE_OBB(Sp, obb, retvec);
			//内積を図る
			Vector3 Base = cp.m_PointB - cp.m_PointA;
			Base.Normalize();
			Vector3 Dest = retvec - cp.m_PointA;
			float dot = Base.Dot(Dest);
			if (dot < 0){
				//スタート位置の球体の外側
				//retvecは有効
				flg = -1;
				return retvec;
			}
			float  size = Vector3EX::Length(cp.m_PointB - cp.m_PointA);
			if (dot > size){
				//終点より先にある
				Sp.m_Center = cp.m_PointB;
				HitTest::SPHERE_OBB(Sp, obb, retvec);
				//終点で最近接点をとる
				flg = 1;
				return retvec;
			}
			//中心とobbの最近接点を得る
			HitTest::ClosestPtPointOBB(cp.GetCenter(), obb, retvec);
			float t;
			Vector3 SegPoint;
			HitTest::ClosetPtPointSegment(retvec, cp.m_PointA, cp.m_PointB, t, SegPoint);
			Vector3 Span = retvec - SegPoint;
			Span.Normalize();
			Span *= cp.m_Radius;
			SegPoint += Span;
			retvec = SegPoint;
			flg = 0;
			return retvec;
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
		static bool CAPSULE_OBB(const CAPSULE& cp, const OBB& obb, Vector3& retvec){
			//スィープさせる球
			SPHERE StartSp, EndSp;
			//各点とobbの最近接点を得る
			Vector3 ToObb;
			HitTest::ClosestPtPointOBB(cp.m_PointA, obb, ToObb);
			float LenA = Vector3EX::Length(cp.m_PointA - ToObb);
			HitTest::ClosestPtPointOBB(cp.m_PointB, obb, ToObb);
			float LenB = Vector3EX::Length(cp.m_PointB - ToObb);
			if (LenA < LenB){
				//スタートはA側
				StartSp.m_Center = cp.m_PointA;
				StartSp.m_Radius = cp.m_Radius;
				EndSp.m_Center = cp.m_PointB;
				EndSp.m_Radius = cp.m_Radius;
			}
			else{
				//スタートはB側
				StartSp.m_Center = cp.m_PointB;
				StartSp.m_Radius = cp.m_Radius;
				EndSp.m_Center = cp.m_PointA;
				EndSp.m_Radius = cp.m_Radius;
			}
			//カプセルとOBBの最近接点を得る（衝突してるかどうかは関係ない）
			int flg;
			retvec = ClosestPtCapsuleOBB(cp, obb,flg);
			float HitTime;
			Vector3 Velocity = EndSp.m_Center - StartSp.m_Center;
			if (CollisionTestSphereObb(StartSp, Velocity,obb, 0, 1.0f, HitTime)){
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
		static bool CollisionTestCapsuleObb(const CAPSULE& SrcCapsule, const Vector3& SrcVelocity,
			const OBB& DestObb, float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			CAPSULE SrcCapsule2 = SrcCapsule;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcCapsule2.m_Radius = (mid - StartTime) * SrcVelocity.Length() + SrcCapsule.m_Radius;
			float Scale = SrcCapsule2.m_Radius / SrcCapsule.m_Radius;
			//中心が原点の元のカプセルを作成
			CAPSULE SrcBaseCapsule = SrcCapsule;
			SrcBaseCapsule.SetCenter(Vector3(0, 0, 0));
			//原点カプセルでスケーリング
			//スケーリング行列の作成
			Matrix4X4 ScalMat;
			ScalMat.Scaling(Scale, Scale, Scale);
			//各頂点をスケーリング
			SrcCapsule2.m_PointA = Vector3EX::Transform(SrcBaseCapsule.m_PointA, ScalMat);
			SrcCapsule2.m_PointB = Vector3EX::Transform(SrcBaseCapsule.m_PointB, ScalMat);
			//中心を移動
			SrcCapsule2.SetCenter(SrcCapsule.GetCenter() + SrcVelocity * mid);
			Vector3 RetVec;
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
		static bool CollisionTestObbObbSub(const OBB& SrcObb, const Vector3& SrcVelocity,
			const OBB& DestObb, float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			OBB SrcObb2;
			float mid = (StartTime + EndTime) * 0.5f;
			SrcObb2.m_Center = SrcObb.m_Center + SrcVelocity * mid;
			//OBBの各辺の長さを拡大
			SrcObb2.m_Size.x = (mid - StartTime) * SrcVelocity.Length() + SrcObb.m_Size.x;
			SrcObb2.m_Size.y = (mid - StartTime) * SrcVelocity.Length() + SrcObb.m_Size.y;
			SrcObb2.m_Size.z = (mid - StartTime) * SrcVelocity.Length() + SrcObb.m_Size.z;
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
		static bool CollisionTestObbObb(const OBB& SrcObb, const Vector3& SrcVelocity,
			const OBB& DestObb, float StartTime, float EndTime, float& HitTime){
			const float m_EPSILON = 0.005f;
			SPHERE BeforeSrcSphere;
			BeforeSrcSphere.m_Center = SrcObb.m_Center;
			BeforeSrcSphere.m_Radius = SrcObb.m_Size.Length();
			if (!CollisionTestSphereObb(BeforeSrcSphere, SrcVelocity, DestObb, StartTime, EndTime, HitTime)){
				//衝突がなければOBBも衝突なし
				return false;
			}
			return CollisionTestObbObbSub(SrcObb, SrcVelocity,DestObb, StartTime, EndTime,HitTime);
		}
	};


}
//end of namespace basecross.


