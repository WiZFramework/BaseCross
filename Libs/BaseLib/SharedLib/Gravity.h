/*!
@file Gravity.h
@brief 重力コンポーネント
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	重力コンポーネント
	//--------------------------------------------------------------------------------------
	class Gravity : public Component {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit Gravity(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Gravity();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	自由落下重力を得る
		@return	自由落下重力
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetDefaultGravity() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	自由落下重力を設定する
		@param[in]	gravity	自由落下重力値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDefaultGravity(const Vector3& gravity);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	自由落下重力を設定する
		@param[in]	x	重力値X
		@param[in]	y	重力値Y
		@param[in]	z	重力値Z
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDefaultGravity(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の重力を得る
		@return	現在の重力
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetGravity() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の重力を設定する
		@param[in]	gravity	重力値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetGravity(const Vector3& gravity);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の重力を0に設定する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetGravityZero();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の重力を自由落下に設定する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetGravityDefault();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の重力を設定する
		@param[in]	x	重力値X
		@param[in]	y	重力値Y
		@param[in]	z	重力値Z
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetGravity(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	重力による現在の速度を得る
		@return	重力による現在の速度
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetGravityVelocity() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	重力による現在の速度を設定する（強引に重力による速度を変更する場合に使用）。
		@param[in]	GravityVelocity	重力による現在の速度
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetGravityVelocity(const Vector3& GravityVelocity);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	重力による現在の速度を設定する（強引に重力による速度を変更する場合に使用）。
		@param[in]	x	重力による現在の速度X
		@param[in]	y	重力による現在の速度Y
		@param[in]	z	重力による現在の速度Z
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetGravityVelocity(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	重力による速度が0かどうかを得る
		@return	重力による速度が0かどうか
		*/
		//--------------------------------------------------------------------------------------
		bool IsGravityVelocityZero();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	重力による速度を0にする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetGravityVelocityZero();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ジャンプのスタート
		@param[in]	StartVec	初速度
		@param[in]	EscapeSpan	ジャンプ開始地点への上部調整値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void StartJump(const Vector3& StartVec, float EscapeSpan = 0.01f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ジャンプのスタート
		@param[in]	x	初速度X
		@param[in]	y	初速度Y
		@param[in]	z	初速度Z
		@param[in]	EscapeSpan	ジャンプ開始地点への上部調整値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void StartJump(float x, float y, float z, float EscapeSpan = 0.01f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定した時間だけ重力をかける
		@param[in]	CalcTime	時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateFromTime(float CalcTime);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理。デフォルトは何も行わない
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




}
//end basecross
