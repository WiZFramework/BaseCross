/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	更新可能なライン
	//--------------------------------------------------------------------------------------
	class ActionLine : public GameObject {
		Vec3 m_StartPos;
		Vec3 m_EndPos;
		Col4 m_Col;
		bool m_IsUpdate;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	StartPos	スタートの位置
		@param[in]	EndPos	エンド位置
		@param[in]	col	線の色

		*/
		//--------------------------------------------------------------------------------------
		ActionLine(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos,
			const Vec3&  EndPos, const Col4& col);
		virtual ~ActionLine() {}
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//線のスタート位置の取得
		const Vec3& GetStartPos() const {
			return m_StartPos;
		}
		//線のスタート位置の設定
		void SetStartObj(const Vec3& Pos) {
			m_StartPos = Pos;
			m_IsUpdate = true;
		}
		//線のエンド位置の取得
		const Vec3& GetEndPos() const {
			return m_EndPos;
		}
		//線のエンド位置の設定
		void SetEndObj(const Vec3& Pos) {
			m_EndPos = Pos;
			m_IsUpdate = true;
		}
		//線のスタートとエンド位置の設定
		void ResetObject(const Vec3& StartPos, const Vec3& EndPos) {
			m_StartPos = StartPos;
			m_EndPos = EndPos;
			m_IsUpdate = true;
		}
	};

	//--------------------------------------------------------------------------------------
	//	タイリングするプレート
	//--------------------------------------------------------------------------------------
	class TilingPlate : public GameObject {
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingPlate(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingPlate();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	固定のボックス
	//--------------------------------------------------------------------------------------
	class TilingFixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
		float m_UPic;
		float m_VPic;
	public:
		//構築と破棄
		TilingFixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position,
			float UPic,
			float VPic
		);
		virtual ~TilingFixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};


}
//end basecross
