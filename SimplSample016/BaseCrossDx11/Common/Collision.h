/*!
@file Collision.h
@brief 衝突判定コンポーネント
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	class CollisionSphere;
	class CollisionCapsule;
	class CollisionObb;
	class CollisionRect;

	//--------------------------------------------------------------------------------------
	///	 衝突した時の動作定義
	//--------------------------------------------------------------------------------------
	enum class IsHitAction {
		AutoOnObjectRepel,	///<反発
		Slide,	//滑る
		AutoOnParent,	//親子関係を実装反発
		AutoOnParentSlide,	//親子関係を実装スライド
	};

	//--------------------------------------------------------------------------------------
	///	 衝突判定コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	class Collision : public Component {
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突配列を検証してテンポラリ配列にセットする
		@param[in]	Tgt	検証するオブジェクト
		@param[out]	SetVec	セットするテンポラリ配列
		@param[in]	ScanVec	スキャンする配列
		@param[in]	IsFind	見つかったらセットするかどうか（falseの場合は「見つからなかったら」セットする）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void FindIfCollisionVector(
			shared_ptr<GameObject> Tgt,
			vector<shared_ptr<GameObject>>& SetVec,
			const vector<shared_ptr<GameObject>>& ScanVec,
			bool IsFind
		);
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit Collision(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Collision();

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	固定衝突オブジェクトかどうかを得る。
		@return	固定衝突オブジェクトならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetFixed() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	固定衝突オブジェクトかどうかを得る。
		@return	固定衝突オブジェクトならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsFixed() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	固定衝突オブジェクトかどうかを設定する
		@param[in]	b	固定衝突オブジェクトならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetFixed(bool b);

		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンコール。ダブルデスパッチ関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollitionCall(const shared_ptr<Collision>& Src) = 0;

		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief ヒットオブジェクトの追加
		@param[in]	DestColl	相手のゲームオブジェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddHitObject(const shared_ptr<GameObject>& DestObject);

		//--------------------------------------------------------------------------------------
		/*!
		@brief 1つ前のヒットオブジェクトにコピー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetToBeforeHitObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ヒットオブジェクトのクリア
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ClearHitObject();

		//--------------------------------------------------------------------------------------
		/*!
		@brief ヒットオブジェクト配列の取得
		@return	ヒットオブジェクト配列
		*/
		//--------------------------------------------------------------------------------------
		vector<shared_ptr<GameObject>>& GetHitObjectVec();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 1つ前のヒットオブジェクト配列の取得
		@return	1つ前のヒットオブジェクト配列
		*/
		//--------------------------------------------------------------------------------------
		vector<shared_ptr<GameObject>>& GetBeforeHitObjectVec();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 新しいヒットオブジェクト配列の取得
		@return	新しいヒットオブジェクト配列
		*/
		//--------------------------------------------------------------------------------------
		vector<shared_ptr<GameObject>>& GetNewHitObjectVec();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突し続けるヒットオブジェクト配列の取得
		@return	衝突し続けるヒットオブジェクト配列
		*/
		//--------------------------------------------------------------------------------------
		vector<shared_ptr<GameObject>>& GetExcuteHitObjectVec();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突を終了したヒットオブジェクト配列の取得
		@return	衝突を終了したヒットオブジェクト配列
		*/
		//--------------------------------------------------------------------------------------
		vector<shared_ptr<GameObject>>& GetExitHitObjectVec();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突した時のアクションを得る。
		@return	衝突した時のアクション
		*/
		//--------------------------------------------------------------------------------------
		IsHitAction GetIsHitAction() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突した時のアクションを設定する
		@param[in]	HitAction	アクション
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetIsHitAction(IsHitAction HitAction);
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionSphere>& DestColl) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionObb>& DestColl) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionRect>& DestColl) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief １つ前のターンからの指定時間の位置に戻る
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	SpanTime	時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void BackToBefore(const Vector3 TotalVelocoty, float SpanTime) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief /GravityとRigidbodyの後処理（Collision共通）
		@param[in]	DestColl	相手のCollision
		@param[in]	ContactBase	スライドの基準となる法線
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AfterCollisionSub(const shared_ptr<Collision>& DestColl, const Vector3& ContactBase);
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	衝突時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionSphere>& DestColl, float SpanTime) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	衝突時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionCapsule>& DestColl, float SpanTime) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	衝突時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionObb>& DestColl, float SpanTime) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	衝突時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionRect>& DestColl, float SpanTime) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionSphere>& DestColl) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionCapsule>& DestColl) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionObb>& DestColl) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionRect>& DestColl) = 0;
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

	//--------------------------------------------------------------------------------------
	//	class CollisionSphere : public Collision ;
	//	用途: 球衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	class CollisionSphere : public Collision {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit CollisionSphere(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CollisionSphere();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の直径を得る
		@return	作成時の直径
		*/
		//--------------------------------------------------------------------------------------
		float GetMakedDiameter() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の直径を設定する
		@param[in]	f	作成時の直径
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMakedDiameter(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の半径を得る。内部では直径のみ持っているので、直径の0.5を返す
		@return	作成時の半径
		*/
		//--------------------------------------------------------------------------------------
		float GetMakedRadius() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の直径を半径をもとに設定する。内部では直径のみ持っているので、2倍したものを直径に設定する
		@param[in]	f	作成時の半径
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMakedRadius(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のSPHER境界ボリュームを得る
		@return	現在のSPHER境界ボリューム
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetSphere() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1ターン前のSPHER境界ボリュームを得る
		@return	1ターン前のSPHER境界ボリューム
		*/
		//--------------------------------------------------------------------------------------
		SPHERE GetBeforeSphere() const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンコール。ダブルデスパッチ関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollitionCall(const shared_ptr<Collision>& Src)override;


		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionSphere>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionCapsule>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionObb>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionRect>& DestColl)override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief １つ前のターンからの指定時間の位置に戻る
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	SpanTime	時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void BackToBefore(const Vector3 TotalVelocoty, float SpanTime)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	処理する時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionSphere>& DestColl, float SpanTime) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	処理する時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionCapsule>& DestColl, float SpanTime) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	処理する時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionObb>& DestColl, float SpanTime)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	衝突時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionRect>& DestColl, float SpanTime)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionSphere>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionCapsule>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionObb>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionRect>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理。DrawActiveがtrue時に呼ばれる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class CollisionCapsule : public Collision ;
	//	用途: カプセル衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	class CollisionCapsule : public Collision {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit CollisionCapsule(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CollisionCapsule();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の直径を得る
		@return	作成時の直径
		*/
		//--------------------------------------------------------------------------------------
		float GetMakedDiameter() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の直径を設定する
		@param[in]	f	作成時の直径
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMakedDiameter(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の半径を得る。内部では直径のみ持っているので、直径の0.5を返す
		@return	作成時の半径
		*/
		//--------------------------------------------------------------------------------------
		float GetMakedRadius() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の直径を半径をもとに設定する。内部では直径のみ持っているので、2倍したものを直径に設定する
		@param[in]	f	作成時の半径
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMakedRadius(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の高さを得る
		@return	作成時の高さ
		*/
		//--------------------------------------------------------------------------------------
		float GetMakedHeight() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の高さを得る
		@param[in]	f	作成時の高さ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMakedHeight(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のCAPSULE境界ボリュームを得る
		@return	現在のCAPSULE境界ボリューム
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE GetCapsule() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1ターン前のCAPSULE境界ボリュームを得る
		@return	1ターン前のCAPSULE境界ボリューム
		*/
		//--------------------------------------------------------------------------------------
		CAPSULE GetBeforeCapsule() const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンコール。ダブルデスパッチ関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollitionCall(const shared_ptr<Collision>& Src)override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionSphere>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionCapsule>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionObb>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionRect>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief １つ前のターンからの指定時間の位置に戻る
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	SpanTime	時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void BackToBefore(const Vector3 TotalVelocoty, float SpanTime)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	処理する時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionSphere>& DestColl, float SpanTime) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	処理する時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionCapsule>& DestColl, float SpanTime) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	処理する時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionObb>& DestColl, float SpanTime)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	衝突時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionRect>& DestColl, float SpanTime)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionSphere>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionCapsule>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionObb>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionRect>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理。DrawActiveがtrue時に呼ばれる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};





	//--------------------------------------------------------------------------------------
	//	class CollisionObb : public Collision ;
	//	用途: Obb衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	class CollisionObb : public Collision {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit CollisionObb(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CollisionObb();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の1辺の長さを得る
		@return	作成時の1辺の長さ
		*/
		//--------------------------------------------------------------------------------------
		float GetMakedSize() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の1辺の長さを設定する
		@param[in]	f	作成時の1辺の長さ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMakedSize(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のOBB境界ボリュームを得る
		@return	現在のOBB境界ボリューム
		*/
		//--------------------------------------------------------------------------------------
		OBB GetObb() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1ターン前のOBB境界ボリュームを得る
		@return	1ターン前のOBB境界ボリューム
		*/
		//--------------------------------------------------------------------------------------
		OBB GetBeforeObb() const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンコール。ダブルデスパッチ関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollitionCall(const shared_ptr<Collision>& Src)override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionSphere>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionCapsule>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionObb>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionRect>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief １つ前のターンからの指定時間の位置に戻る
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	SpanTime	時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void BackToBefore(const Vector3 TotalVelocoty, float SpanTime)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	処理する時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionSphere>& DestColl, float SpanTime)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	処理する時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionCapsule>& DestColl, float SpanTime) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	処理する時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionObb>& DestColl, float SpanTime)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	衝突時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionRect>& DestColl, float SpanTime)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionSphere>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionCapsule>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionObb>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionRect>& DestColl)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理。DrawActiveがtrue時に呼ばれる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class CollisionRect : public Collision ;
	//	用途: Rect(矩形)衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	class CollisionRect : public Collision {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit CollisionRect(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CollisionRect();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	固定衝突オブジェクトかどうかを設定する
		@param[in]	b	固定衝突オブジェクトならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void SetFixed(bool b)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の1辺の長さを得る
		@return	作成時の1辺の長さ
		*/
		//--------------------------------------------------------------------------------------
		float GetMakedSize() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の1辺の長さを設定する
		@param[in]	f	作成時の1辺の長さ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMakedSize(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のCOLRECT境界ボリュームを得る
		@return	現在のCOLRECT境界ボリューム
		*/
		//--------------------------------------------------------------------------------------
		COLRECT GetColRect() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1ターン前のCOLRECT境界ボリュームを得る
		@return	1ターン前のCOLRECT境界ボリューム
		*/
		//--------------------------------------------------------------------------------------
		COLRECT GetBeforeColRect() const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンコール。ダブルデスパッチ関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollitionCall(const shared_ptr<Collision>& Src)override;


		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionSphere>& DestColl) override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionCapsule>& DestColl)override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionObb>& DestColl)override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionRect>& DestColl)override{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief １つ前のターンからの指定時間の位置に戻る
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	SpanTime	時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void BackToBefore(const Vector3 TotalVelocoty, float SpanTime)override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	処理する時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionSphere>& DestColl, float SpanTime)override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	処理する時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionCapsule>& DestColl, float SpanTime) override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	処理する時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionObb>& DestColl, float SpanTime)override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectとの衝突後処理
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	DestColl	相手のコリジョン
		@param[in]	SpanTime	衝突時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterCollision(const Vector3& TotalVelocoty, const shared_ptr<CollisionRect>& DestColl, float SpanTime)override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionSphere>& DestColl)override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionCapsule>& DestColl)override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionObb>& DestColl)override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionRect>& DestColl)override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理。DrawActiveがtrue時に呼ばれる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




}
//end basecross
