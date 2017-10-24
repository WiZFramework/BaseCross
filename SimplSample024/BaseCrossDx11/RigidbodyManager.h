/*!
@file RigidbodyManager.h
@brief 剛体と衝突判定
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	コリジョンタイプ
	//--------------------------------------------------------------------------------------
	enum class CollType {
		typeNone,
		typeSPHERE,
		typeCAPSULE,
		typeOBB,
	};

	struct Rigidbody;
	//--------------------------------------------------------------------------------------
	///	衝突情報
	//--------------------------------------------------------------------------------------
	struct CollisionState {
		Rigidbody* m_Src;
		Vec3 m_SrcHitNormal;
		Rigidbody* m_Dest;
		Vec3 m_DestHitNormal;
		float m_HitTime;
	};

	class GameObject;
	struct Camera;

	//--------------------------------------------------------------------------------------
	///	剛体
	//--------------------------------------------------------------------------------------
	struct Rigidbody {
		//描画用のメッシュ
		static shared_ptr<MeshResource> m_SPHEREMesh;
		static shared_ptr<MeshResource> m_CAPSULEMesh;
		static shared_ptr<MeshResource> m_OBBMesh;
		//オーナー
		weak_ptr<GameObject> m_Owner;
		//重力加速度
		Vec3 m_Gravity;
		//質量
		float m_Mass;
		//現在のフォース
		Vec3 m_Force;
		//速度
		Vec3 m_Velocity;
		//コリジョンのタイプ
		CollType m_CollType;
		//Fixedかどうか
		bool m_IsFixed;
		//衝突判定を行うかどうか
		bool m_IsCollisionActive;
		//デバッグ用描画を行うかどうか
		bool m_IsDrawActive;
		//スケール
		Vec3 m_Scale;
		//回転
		Quat m_Quat;
		//位置
		Vec3 m_Pos;
		//1つ前のスケール
		Vec3 m_BeforeScale;
		//1つ前の回転
		Quat m_BeforeQuat;
		//1つ前の位置
		Vec3 m_BeforePos;
		void SetToBefore() {
			m_BeforeScale = m_Scale;
			m_BeforeQuat = m_Quat;
			m_BeforePos = m_Pos;
		}
		Rigidbody() :
			m_Gravity(0, -9.8f, 0),
			m_Mass(0),
			m_Force(0),
			m_Velocity(0),
			m_CollType(CollType::typeNone),
			m_IsFixed(false),
			m_IsCollisionActive(true),
			m_IsDrawActive(false),
			m_Scale(1.0f),
			m_Quat(),
			m_Pos(0),
			m_BeforeScale(1.0f),
			m_BeforeQuat(),
			m_BeforePos(0)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief SPHEREを得る
		@return	SPHERE
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetSPHERE()const {
			SPHERE sp;
			sp.m_Center = m_Pos;
			sp.m_Radius = m_Scale.y * 0.5f;
			return sp;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CAPSULEを得る
		@return	CAPSULE
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE GetCAPSULE()const {
			Mat4x4 mat;
			mat.affineTransformation(m_Scale, Vec3(0, 0, 0),
				m_Quat, m_Pos);
			CAPSULE cap(0.5f, Vec3(0, -0.5f, 0), Vec3(0, 0.5f, 0), mat);
			return cap;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief OBBを得る
		@return	OBB
		*/
		//--------------------------------------------------------------------------------------
		OBB GetOBB()const {
			Mat4x4 mat;
			mat.affineTransformation(m_Scale, Vec3(0, 0, 0),
				m_Quat, m_Pos);
			OBB ob(Vec3(1.0f, 1.0f, 1.0f), mat);
			return ob;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief Velocityに従って、Spantimeだけ位置を変更する
		@param[in]	Spantime	位置変更タイム
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Move(float Spantime);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突判定(SrcがSPHERE)
		@param[in]	Dest	相手の参照
		@param[in]	ElapsedTime	ターン時間
		@param[out]	state	衝突ステート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		bool CollisionSPHERE(Rigidbody& Dest, float ElapsedTime, CollisionState& state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突判定(SrcがCAPSULE)
		@param[in]	Dest	相手の参照
		@param[in]	ElapsedTime	ターン時間
		@param[out]	state	衝突ステート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		bool CollisionCAPSULE(Rigidbody& Dest, float ElapsedTime, CollisionState& state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突判定(SrcがOBB)
		@param[in]	Dest	相手の参照
		@param[in]	ElapsedTime	ターン時間
		@param[out]	state	衝突ステート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		bool CollisionOBB(Rigidbody& Dest, float ElapsedTime, CollisionState& state);
		//--------------------------------------------------------------------------------------
		/*!
		@brief エスケープ(SrcがSPHERE)
		@param[in]	Dest	相手のポインタ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void EscapeSPHERE(Rigidbody* Dest);
		//--------------------------------------------------------------------------------------
		/*!
		@brief エスケープ(SrcがCAPSULE)
		@param[in]	Dest	相手のポインタ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void EscapeCAPSULE(Rigidbody* Dest);
		//--------------------------------------------------------------------------------------
		/*!
		@brief エスケープ(SrcがOBB)
		@param[in]	Dest	相手のポインタ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void EscapeOBB(Rigidbody* Dest);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ワイアフレーム描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void DrawWireFlame(const Camera& camera);
	};


	//--------------------------------------------------------------------------------------
	///	Rigidbodyマネージャ
	//--------------------------------------------------------------------------------------
	class RigidbodyManager : public GameObject {
		//Rigidbodyの配列
		vector<shared_ptr<Rigidbody>> m_RigidbodyVec;
		//衝突判定
		void CollisionDest(Rigidbody& Src);
		bool CollisionStateChk(Rigidbody* p1, Rigidbody* p2);
		bool CollisionTest(Rigidbody& Src, Rigidbody& Dest, CollisionState& state);
		//衝突ステートの配列
		vector<CollisionState> m_CollisionStateVec;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		*/
		//--------------------------------------------------------------------------------------
		RigidbodyManager(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RigidbodyManager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief Rigidbodyの配列を得る
		@return	Rigidbodyの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<shared_ptr<Rigidbody>>& GetRigidbodyVec()const {
			return m_RigidbodyVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突情報の配列を得る
		@return	衝突情報の配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<CollisionState>& GetCollisionStateVec()const {
			return m_CollisionStateVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 指定のオーナーのRigidbodyを得る
		@param[in]	OwnerPtr	オーナーのポインタ
		@return	指定のオーナーのRigidbodyのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Rigidbody> GetOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 指定のオーナーのRigidbodyを削除する（見つからなければ何もしない）
		@param[in]	OwnerPtr	オーナーのポインタ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void RemoveOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief フォースを初期化する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void InitRigidbody();
		//--------------------------------------------------------------------------------------
		/*!
		@brief Rigidbodyを登録する
		@return	追加したRigidbodyのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Rigidbody> AddRigidbody(const Rigidbody& body);
		//--------------------------------------------------------------------------------------
		/*!
		@brief SrcのDestからのエスケープ
		@param[in]	Src		Srcのポインタ
		@param[in]	Dest	Destのポインタ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Escape(Rigidbody* Src, Rigidbody* Dest);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 最終更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate2()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;

	};


}
//end basecross
