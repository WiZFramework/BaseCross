/*!
@file BasePhysics.h
@brief 物理計算インターファイス
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	モーションタイプ
	//--------------------------------------------------------------------------------------
	enum PsMotionType {
		MotionTypeFixed = 0,
		MotionTypeActive,
		MotionTypeKeyframe,
		MotionTypeOneWay,
		MotionTypeTrigger,
		MotionTypeCount
	};

	//--------------------------------------------------------------------------------------
	///	ボックス作成のパラメータ
	//--------------------------------------------------------------------------------------
	struct PsBoxParam {
		PsMotionType m_MotionType;
		bsm::Vec3 m_HalfSize;
		bsm::Quat m_Quat;
		bsm::Vec3 m_Pos;
		bsm::Vec3 m_Force;
		bsm::Vec3 m_Torque;
		bsm::Vec3 m_Velocity;
		float m_Mass;
		PsBoxParam():
			m_HalfSize(1.0f),
			m_Quat(),
			m_Pos(0),
			m_Force(0),
			m_Torque(0),
			m_Velocity(0),
			m_Mass(1.0f)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	球体作成のパラメータ
	//--------------------------------------------------------------------------------------
	struct PsSphereParam {
		PsMotionType m_MotionType;
		float m_Radius;
		bsm::Quat m_Quat;
		bsm::Vec3 m_Pos;
		bsm::Vec3 m_Force;
		bsm::Vec3 m_Torque;
		bsm::Vec3 m_Velocity;
		float m_Mass;
		PsSphereParam():
			m_Radius(1.0f),
			m_Quat(),
			m_Pos(0),
			m_Force(0),
			m_Torque(0),
			m_Velocity(0),
			m_Mass(1.0f)
		{}
	};

	//--------------------------------------------------------------------------------------
	///	剛体のステータス
	//--------------------------------------------------------------------------------------
	struct PsBodyStatus {
		bsm::Vec3	m_Position;
		bsm::Quat	m_Orientation;
		bsm::Vec3	m_LinearVelocity;
		bsm::Vec3	m_AngularVelocity;
	};


	//--------------------------------------------------------------------------------------
	///	物理オブジェクトの親
	//--------------------------------------------------------------------------------------
	class PhysicsObject : public ObjectInterface {
	protected:
		/// 剛体インデックス
		uint16_t m_Index;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsObject();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体インデックスの取得
		*/
		//--------------------------------------------------------------------------------------
		uint16_t GetIndex() const {
			return m_Index;
		}
	};

	//--------------------------------------------------------------------------------------
	///	ボックス物理オブジェクト
	//--------------------------------------------------------------------------------------
	class PhysicsBox : public PhysicsObject {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	param	作成のパラメータ
		@param[in]	index	インデックス
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsBox(const PsBoxParam& param, uint16_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsBox();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	球体物理オブジェクト
	//--------------------------------------------------------------------------------------
	class PhysicsSphere : public PhysicsObject {
		wstring m_MappedKey;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	param	作成のパラメータ
		@param[in]	index	インデックス
		*/
		//--------------------------------------------------------------------------------------
		explicit PhysicsSphere(const PsSphereParam& param,uint16_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsSphere();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	物理計算用のインターフェイス
	//--------------------------------------------------------------------------------------
	class BasePhysics{
		map< wstring , uint16_t > m_ConstIndexMap;
		uint16_t GetMappedIndex(const wstring& key);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		explicit BasePhysics();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BasePhysics();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Update();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化（すべてのオブジェクトが無くなる）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Reset();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単体のボックスの追加
		@param[in]	param	作成のパラメータ
		@param[in]	indexKey	インデックスのキー（オブジェクトを再利用する場合）
		@return	オブジェクトのポインタ（バックアップはしないので呼び出し側で保存すること）
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsBox> AddSingleBox(const PsBoxParam& param, const wstring& indexKey = L"");
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単体の球体の追加
		@param[in]	param	作成のパラメータ
		@param[in]	indexKey	インデックスのキー（オブジェクトを再利用する場合）
		@return	オブジェクトのポインタ（バックアップはしないので呼び出し側で保存すること）
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<PhysicsSphere> AddSingleSphere(const PsSphereParam& param,const wstring& indexKey = L"");
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定のキーのボディインデックスをチェックする
		@return	キーのインデックスがあればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool CheckBodyIndex(const wstring& indexKey) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のボディ数を得る
		@return	現在のボディ数
		*/
		//--------------------------------------------------------------------------------------
		uint16_t GetNumBodies() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	剛体のステータスを得る
		@param[in]	index	ボディID
		@param[out]	st	ステータスを得る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetBodyStatus(uint16_t index, PsBodyStatus& st);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	形状の数を得る
		@param[in]	index	ボディID
		@return	形状数
		*/
		//--------------------------------------------------------------------------------------
		uint16_t GetNumShapes(uint16_t body_index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	形状のオフセット回転とオフセット位置を得る
		@param[in]	index	ボディID
		@param[in]	shape_index	形状ID
		@param[out]	qt	回転を受ける参照
		@param[out]	pos	位置を受ける参照
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		void GetShapeOffsetQuatPos(uint16_t body_index, uint16_t shape_index, bsm::Quat& qt, bsm::Vec3& pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	形状のタイプを得る
		@param[in]	index	ボディID
		@param[in]	shape_index	形状ID
		@return　形状のタイプ
		*/
		//--------------------------------------------------------------------------------------
		sce::PhysicsEffects::ePfxShapeType GetShapeType(uint16_t body_index, uint16_t shape_index) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ボックス形状のスケール（ハーフサイズ）を得る
		@param[in]	index	ボディID
		@param[in]	shape_index	形状ID
		@return　ボックス形状のスケール
		*/
		//--------------------------------------------------------------------------------------
		bsm::Vec3 GetShapeBoxScale(uint16_t body_index, uint16_t shape_index) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	球体形状の半径を得る
		@param[in]	index	ボディID
		@param[in]	shape_index	形状ID
		@return　球体形状の半径
		*/
		//--------------------------------------------------------------------------------------
		float GetShapeSphereRadius(uint16_t body_index, uint16_t shape_index) const;
	};



}
//end basecross
