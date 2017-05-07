/*!
@file Collision.h
@brief 衝突判定コンポーネント
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	class CollisionSphere;
	class CollisionCapsule;
	class CollisionObb;
	class CollisionTriangles;
	class CollisionRect;

	//--------------------------------------------------------------------------------------
	///	 衝突した時の動作定義
	//--------------------------------------------------------------------------------------
	enum class IsHitAction {
		None,			//何もしない
		Slide,			//スライド
		Auto,			//オート計算
	};

	//--------------------------------------------------------------------------------------
	///	 スケールが一つだった場合の計算に使用するスケール
	//--------------------------------------------------------------------------------------
	enum class CalcScaling {
		XScale,
		YScale,
		ZScale,
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
		@brief	エスケープ処理の調整値を得る。
		@return	エスケープ処理の調整値
		*/
		//--------------------------------------------------------------------------------------
		float GetEscapeAdjustedValue() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エスケープ処理の調整値を設定する。
		@param[in]	f	エスケープ処理の調整値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetEscapeAdjustedValue(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 判定から除外するグループの取得。
		@return	 判定から除外するグループ（指定されてなければnullptr）
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObjectGroup> GetExcludeCollisionGroup() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 判定から除外するグループの設定。
		@return	 なし
		*/
		//--------------------------------------------------------------------------------------
		void SetExcludeCollisionGroup(const shared_ptr<GameObjectGroup>& Group);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	判定から除外するタグが存在するかどうかを得る
		@param[in]	tagstr	検証するタグ
		@return	存在すればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool FindExcludeCollisionTag(const wstring& tagstr) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	判定から除外するタグを追加する
		@param[in]	tagstr	追加するタグ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void  AddExcludeCollisionTag(const wstring& tagstr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	判定から除外するタグが存在したら削除する（存在しない場合は何もしない）
		@param[in]	tagstr	削除するタグ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void  RemoveExcludeCollisionTag(const wstring& tagstr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 判定から除外するオブジェクトかどうかを取得。
		@param[in]	Obj	調査するゲームオブジェクト
		@return	 判定から除外する設定ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsExcludeCollisionObject(const shared_ptr<GameObject>& Obj) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンコール。ダブルデスパッチ関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionCall(const shared_ptr<Collision>& Src) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {
			//衝突判定管理者に任せるので何もしない
		}
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
		@brief ヒットオブジェクトに入ってるかどうか
		@return	ヒットしていればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsHitObject(const shared_ptr<GameObject>& Obj) const;
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
		virtual void CollisionTest(const shared_ptr<CollisionSphere>& DestColl) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionCapsule>& DestColl) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionObb>& DestColl) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionTrianglesとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionTriangles>& DestColl) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionRect>& DestColl) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief １つ前のターンからの指定時間の位置に戻る
		@param[in]	TotalVelocoty	トータルの速度（RigidbodyとGravityを足したもの）
		@param[in]	SpanTime	時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void BackToBefore(const Vector3 TotalVelocoty, float SpanTime) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionSphere>& DestColl, Vector3& Ret) const {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionCapsule>& DestColl, Vector3& Ret) const {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionObb>& DestColl, Vector3& Ret) const {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionTriangles>& DestColl, Vector3& Ret) const {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionRect>& DestColl, Vector3& Ret) const {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突判定後処理（Collision共通）
		@param[in]	DestColl	相手のCollision
		@param[in]	SrcVelocity	自分の速度
		@param[in]	DestVelocity	相手の速度
		@param[in]	HitNormal	衝突法線
		@param[in]	AfterHitTime 衝突後のターン時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AfterCollision(const shared_ptr<Collision>& DestColl, const Vector3& SrcVelocity,  const Vector3& DestVelocity, const Vector3& HitNormal,
			float AfterHitTime);

		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突判定後処理呼び出しテンプレート１
		@tparam	SrcType	Srcのコリジョン型
		@tparam	DestType	Destのコリジョン型
		@param[in]	SrcVelocity	Srcの速度
		@param[in]	DestVelocity	Destの速度
		@param[in]	ElapsedTime	ターン時間
		@param[in]	HitTime	衝突時間
		@param[in]	SrcColl	Srcのコリジョン
		@param[in]	DestColl	Destのコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename SrcType, typename DestType>
		void AfterCollisionTmpl_1(const Vector3& SrcVelocity, const Vector3& DestVelocity, float ElapsedTime, float HitTime, 
			const shared_ptr<SrcType>& SrcColl, const shared_ptr<DestType>&  DestColl) {
			if (HitTime <= 0) {
				HitTime = 0;
			}
			if (HitTime >= ElapsedTime) {
				HitTime = ElapsedTime;
			}
			auto AfterHitTime = ElapsedTime - HitTime;
			bool UseSrcAfter = false;
			bool UseDestAfter = false;
			//衝突相手の登録
			AddHitObject(DestColl->GetGameObject());
			if (GetIsHitAction() != IsHitAction::None) {
				//事後処理が有効なら衝突前まで戻る
				BackToBefore(SrcVelocity, HitTime);
				UseSrcAfter = true;
			}
			if (!DestColl->IsFixed()) {
				//もし相手がFixでなければ相手の衝突相手の登録
				DestColl->AddHitObject(GetGameObject());
				if (DestColl->GetIsHitAction() != IsHitAction::None) {
					//事後処理が有効なら衝突前まで戻る
					DestColl->BackToBefore(DestVelocity, HitTime);
					UseDestAfter = true;
				}
			}
			else {
				//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
				if (GetIsHitAction() != IsHitAction::None) {
					CollisionEscape(DestColl);
				}
			}
			if (UseSrcAfter) {
				Vector3 HitNormal;
				GetHitNormal(DestColl, HitNormal);
				AfterCollision(DestColl, SrcVelocity, DestVelocity, HitNormal, AfterHitTime);
			}
			if (UseDestAfter) {
				Vector3 HitNormal;
				DestColl->GetHitNormal(SrcColl, HitNormal);
				DestColl->AfterCollision(SrcColl, DestVelocity, SrcVelocity, HitNormal, AfterHitTime);
			}

		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突判定後処理呼び出しテンプレート２
		@tparam	SrcType	Srcのコリジョン型
		@tparam	DestType	Destのコリジョン型
		@param[in]	SrcVelocity	Srcの速度
		@param[in]	DestVelocity	Destの速度
		@param[in]	ElapsedTime	ターン時間
		@param[in]	HitTime	衝突時間
		@param[in]	SrcColl	Srcのコリジョン
		@param[in]	DestColl	Destのコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename SrcType, typename DestType>
		void AfterCollisionTmpl_2(const Vector3& SrcVelocity, const Vector3& DestVelocity, float ElapsedTime, float HitTime,
			const shared_ptr<SrcType>& SrcColl, const shared_ptr<DestType>&  DestColl) {
			if (HitTime <= 0) {
				HitTime = 0;
			}
			if (HitTime >= ElapsedTime) {
				HitTime = ElapsedTime;
			}
			auto AfterHitTime = ElapsedTime - HitTime;
			bool UseSrcAfter = false;
			//衝突相手の登録
			AddHitObject(DestColl->GetGameObject());
			if (GetIsHitAction() != IsHitAction::None) {
				//事後処理が有効なら衝突前まで戻る
				BackToBefore(SrcVelocity, HitTime);
				UseSrcAfter = true;
			}
			if (GetIsHitAction() != IsHitAction::None) {
				CollisionEscape(DestColl);
			}
			if (UseSrcAfter) {
				Vector3 HitNormal;
				GetHitNormal(DestColl, HitNormal);
				AfterCollision(DestColl, SrcVelocity, DestVelocity, HitNormal, AfterHitTime);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionSphereからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionSphere>& DestColl) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionCapsuleからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionCapsule>& DestColl) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionObbからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionObb>& DestColl) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionTrianglesからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionTriangles>& DestColl) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief CollisionRectからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionRect>& DestColl) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 1つ前と現在の連結させたSPHEREを得る
		@return	連結させたSPHERE
		*/
		//--------------------------------------------------------------------------------------
		virtual SPHERE GetEnclosingSphere()const = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 自分と相手の連結したSPHEREの衝突判定（前処理用）
		@tparam	T	相手のコリジョンの型
		@param[in]	DestColl	相手のコリジョン
		@return	ヒットしてればtrue
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		bool CillisionWrappedSphere(const shared_ptr<T>& DestColl) {
			SPHERE Src = GetEnclosingSphere();
			SPHERE Dest = DestColl->GetEnclosingSphere();
			return HitTest::SPHERE_SPHERE(Src, Dest);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の包み込むAABBを返す。仮想関数
		@return	包み込むAABB(1つ前のターン時の内容は見ない)
		*/
		//--------------------------------------------------------------------------------------
		virtual AABB GetWrappingAABB()const = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心位置を返す。仮想関数
		@return	中心位置
		*/
		//--------------------------------------------------------------------------------------
		virtual Vector3 GetCenterPosition()const = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 線分と自分の衝突チェック（判定するのみ）
		@param[in]	Pos1	線分開始
		@param[in]	Pos2	線分終了
		@return	ヒットしてればtrue
		*/
		//--------------------------------------------------------------------------------------
		virtual bool HitTestWithSegment(const Vector3& Pos1, const Vector3& Pos2) { return false; }
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
		@brief	計算時、XYZどのスケールを使用するかを得る(x: 0,y: 1,z: 2)。デフォルトx
		@return	計算時、XYZどのスケールを使用するか
		*/
		//--------------------------------------------------------------------------------------
		CalcScaling GetCalcScaling() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	計算時、XYZどのスケールを使用するかを設定する(x: 0,y: 1,z: 2)。デフォルトx
		@param[in]	s	計算時、XYZどのスケールを使用するか
		@return	
		*/
		//--------------------------------------------------------------------------------------
		void SetCalcScaling(CalcScaling s);
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
		virtual void CollisionCall(const shared_ptr<Collision>& Src)override;
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
		@brief CollisionTrianglesとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionTriangles>& DestColl)override;
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
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionSphere>& DestColl, Vector3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionCapsule>& DestColl, Vector3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionObb>& DestColl, Vector3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionTriangles>& DestColl, Vector3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionRect>& DestColl, Vector3& Ret) const override;
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
		@brief CollisionTrianglesからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionTriangles>& DestColl)override;
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
		@brief 1つ前と現在の連結させたSPHEREを得る
		@return	連結させたSPHERE
		*/
		//--------------------------------------------------------------------------------------
		virtual SPHERE GetEnclosingSphere()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 線分と自分の衝突チェック（判定するのみ）
		@param[in]	Pos1	線分開始
		@param[in]	Pos2	線分終了
		@return	ヒットしてればtrue
		*/
		//--------------------------------------------------------------------------------------
		virtual bool HitTestWithSegment(const Vector3& Pos1, const Vector3& Pos2)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の包み込むAABBを返す。仮想関数
		@return	包み込むAABB(1つ前のターン時の内容は見ない)
		*/
		//--------------------------------------------------------------------------------------
		virtual AABB GetWrappingAABB()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心位置を返す。仮想関数
		@return	中心位置
		*/
		//--------------------------------------------------------------------------------------
		virtual Vector3 GetCenterPosition()const override;
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
		virtual void CollisionCall(const shared_ptr<Collision>& Src)override;

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
		@brief CollisionTrianglesとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionTriangles>& DestColl)override;
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
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionSphere>& DestColl, Vector3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionCapsule>& DestColl, Vector3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionObb>& DestColl, Vector3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionTriangles>& DestColl, Vector3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionRect>& DestColl, Vector3& Ret) const override;
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
		@brief CollisionTrianglesからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionTriangles>& DestColl)override;
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
		@brief 1つ前と現在の連結させたSPHEREを得る
		@return	連結させたSPHERE
		*/
		//--------------------------------------------------------------------------------------
		virtual SPHERE GetEnclosingSphere()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 線分と自分の衝突チェック（判定するのみ）
		@param[in]	Pos1	線分開始
		@param[in]	Pos2	線分終了
		@return	ヒットしてればtrue
		*/
		//--------------------------------------------------------------------------------------
		virtual bool HitTestWithSegment(const Vector3& Pos1, const Vector3& Pos2)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の包み込むAABBを返す。仮想関数
		@return	包み込むAABB(1つ前のターン時の内容は見ない)
		*/
		//--------------------------------------------------------------------------------------
		virtual AABB GetWrappingAABB()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心位置を返す。仮想関数
		@return	中心位置
		*/
		//--------------------------------------------------------------------------------------
		virtual Vector3 GetCenterPosition()const override;
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
		virtual void CollisionCall(const shared_ptr<Collision>& Src)override;

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
		@brief CollisionTrianglesとの衝突テスト
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionTest(const shared_ptr<CollisionTriangles>& DestColl)override;
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
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionSphere>& DestColl, Vector3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionCapsule>& DestColl, Vector3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionObb>& DestColl, Vector3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionTriangles>& DestColl, Vector3& Ret) const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突法線を得る
		@param[in]	DestColl	相手のCollision
		@param[out]	Ret	戻す法線の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void GetHitNormal(const shared_ptr<CollisionRect>& DestColl, Vector3& Ret) const override;
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
		@brief CollisionTrianglesからのエスケープ
		@param[in]	DestColl	相手のコリジョン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionEscape(const shared_ptr<CollisionTriangles>& DestColl)override;
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
		@brief 1つ前と現在の連結させたSPHEREを得る
		@return	連結させたSPHERE
		*/
		//--------------------------------------------------------------------------------------
		virtual SPHERE GetEnclosingSphere()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 線分と自分の衝突チェック（判定するのみ）
		@param[in]	Pos1	線分開始
		@param[in]	Pos2	線分終了
		@return	ヒットしてればtrue
		*/
		//--------------------------------------------------------------------------------------
		virtual bool HitTestWithSegment(const Vector3& Pos1, const Vector3& Pos2)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の包み込むAABBを返す。仮想関数
		@return	包み込むAABB(1つ前のターン時の内容は見ない)
		*/
		//--------------------------------------------------------------------------------------
		virtual AABB GetWrappingAABB()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心位置を返す。仮想関数
		@return	中心位置
		*/
		//--------------------------------------------------------------------------------------
		virtual Vector3 GetCenterPosition()const override;
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
	//	class CollisionTriangles : public Collision ;
	//	用途: 三角形の配列衝突判定コンポーネント
	//--------------------------------------------------------------------------------------
	class CollisionTriangles : public Collision {
	protected:
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit CollisionTriangles(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CollisionTriangles();
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
		@brief	作成時の1つの三角形の配列を得る
		@return	作成時の1つの三角形の配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<TRIANGLE>& GetMakedTriangles() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	作成時の三角形の配列を設定する
		@param[in]	trivec	作成時の三角形の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMakedTriangles(const vector<TRIANGLE>& trivec);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワイアフレームメッシュを設定する
		@param[in]	Key	メッシュ名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetWireFrameMesh(const wstring& Key);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の三角形の配列を得る
		@param[out]	trivec	設定される三角形の配列（内部でクリアされる）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetTriangles(vector<TRIANGLE>& trivec) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1つ前の三角形の配列を得る
		@param[out]	trivec	設定される三角形の配列（内部でクリアされる）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetBeforeTriangles(vector<TRIANGLE>& trivec) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の包み込むAABBを返す。仮想関数
		@return	包み込むAABB(1つ前のターン時の内容は見ない)
		*/
		//--------------------------------------------------------------------------------------
		virtual AABB GetWrappingAABB()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心位置を返す。仮想関数
		@return	中心位置
		*/
		//--------------------------------------------------------------------------------------
		virtual Vector3 GetCenterPosition()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	三角形の配列を包み込むAABBを返す
		@return	三角形の配列を包み込むAABB(1つ前のターン時の内容も反映される)
		*/
		//--------------------------------------------------------------------------------------
		AABB GetWrappedAABB()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コリジョンコール。ダブルデスパッチ関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void CollisionCall(const shared_ptr<Collision>& Src)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 1つ前と現在の連結させたSPHEREを得る
		@return	連結させたSPHERE
		*/
		//--------------------------------------------------------------------------------------
		virtual SPHERE GetEnclosingSphere()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 線分と自分の衝突チェック（判定するのみ）
		@param[in]	Pos1	線分開始
		@param[in]	Pos2	線分終了
		@return	ヒットしてればtrue
		*/
		//--------------------------------------------------------------------------------------
		virtual bool HitTestWithSegment(const Vector3& Pos1, const Vector3& Pos2)override;
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
		virtual void CollisionCall(const shared_ptr<Collision>& Src)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 線分と自分の衝突チェック（判定するのみ）
		@param[in]	Pos1	線分開始
		@param[in]	Pos2	線分終了
		@return	ヒットしてればtrue
		*/
		//--------------------------------------------------------------------------------------
		virtual bool HitTestWithSegment(const Vector3& Pos1, const Vector3& Pos2)override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 1つ前と現在の連結させたSPHEREを得る
		@return	連結させたSPHERE
		*/
		//--------------------------------------------------------------------------------------
		virtual SPHERE GetEnclosingSphere()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在の包み込むAABBを返す。仮想関数
		@return	包み込むAABB(1つ前のターン時の内容は見ない)
		*/
		//--------------------------------------------------------------------------------------
		virtual AABB GetWrappingAABB()const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心位置を返す。仮想関数
		@return	中心位置
		*/
		//--------------------------------------------------------------------------------------
		virtual Vector3 GetCenterPosition()const override;
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
