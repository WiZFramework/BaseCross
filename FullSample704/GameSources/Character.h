/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	位置指定のライン
	//--------------------------------------------------------------------------------------
	class PositionLine : public GameObject {
		Vec3 m_StartPos;
		Vec3 m_EndPos;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	StartObj	スタートのオブジェクト
		@param[in]	EndObj	エンドのオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		PositionLine(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos,
			const Vec3& EndPos);
		virtual ~PositionLine() {}
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//線のスタート位置の取得
		const Vec3& GetStartPos() const {
			return m_StartPos;
		}
		//線のスタート位置の設定
		void SetStartPos(const Vec3& Pos) {
			m_StartPos = Pos;
		}
		//線のエンド位置の取得
		const Vec3& GetEndPos() const {
			return m_EndPos;
		}
		//線のエンド位置の設定
		void SetEndPos(const Vec3& Pos) {
			m_EndPos = Pos;
		}
		//線のスタートとエンドオブジェクト位置の設定
		void ResetObject(const Vec3& StartPos, const Vec3& EndPos) {
			m_StartPos = StartPos;
			m_EndPos = EndPos;
		}
	};




	//--------------------------------------------------------------------------------------
	///	物理計算する固定のボックス
	//--------------------------------------------------------------------------------------
	class FixedPsBox : public GameObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
	public:
		//構築と破棄
		FixedPsBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~FixedPsBox();
		//初期化
		virtual void OnCreate() override;
	};





	//--------------------------------------------------------------------------------------
	///	ラグドール
	//--------------------------------------------------------------------------------------
	class Ragdoll : public GameObject {
		Vec3 m_StartPos;
		Quat m_StartQt;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	StartPos	初期位置
		@param[in]	StartQt		初期姿勢
		*/
		//--------------------------------------------------------------------------------------
		Ragdoll(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, const Quat& StartQt);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Ragdoll();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	};

	//--------------------------------------------------------------------------------------
	///	ラグドールマネージャ（描画を管理する）
	//--------------------------------------------------------------------------------------
	class RagdollManager : public GameObject {
		weak_ptr<Ragdoll> m_Ragdoll;
		vector<weak_ptr<GameObject>> m_RagdollObjVec;
		size_t m_HoldIndex;
		bool m_Holded;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	ragdoll		ラグドール
		*/
		//--------------------------------------------------------------------------------------
		RagdollManager(const shared_ptr<Stage>& StagePtr, const shared_ptr<Ragdoll>& ragdoll);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RagdollManager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ホールド状態の設定
		@param[in]	b	ホールド状態ならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetHold(bool b) {
			m_Holded = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ホールドされてるオブジェクトのインデックスの設定
		@param[in]	i	ホールドされてるオブジェクトのインデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetHoldIndex(size_t i) {
			m_HoldIndex = i;

		}

	};




}
//end basecross
