/*!
@file GameObject.h
@brief ゲームオブジェクト、ステージ
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {



	//--------------------------------------------------------------------------------------
	///	ゲーム配置オブジェクト親クラス
	//--------------------------------------------------------------------------------------
	class GameObject :public ObjectInterface, public ShapeInterface {
		shared_ptr<Component> SearchComponent(type_index TypeIndex)const;
		shared_ptr<Transform> GetTransform()const;
		shared_ptr<Rigidbody> GetRigidbody()const;
		shared_ptr<Collision> GetCollision()const;
		shared_ptr<CollisionSphere> GetCollisionSphere()const;
		shared_ptr<CollisionCapsule> GetCollisionCapsule()const;
		shared_ptr<CollisionObb> GetCollisionObb()const;
		shared_ptr<CollisionTriangles> GetCollisionTriangles()const;
		shared_ptr<CollisionRect> GetCollisionRect()const;

		void SetRigidbody(const shared_ptr<Rigidbody>& Ptr);
		void SetTransform(const shared_ptr<Transform>& Ptr);
		void SetCollision(const shared_ptr<Collision>& Ptr);
		void AddMakedComponent(type_index TypeIndex, const shared_ptr<Component>& Ptr);
		template<typename T>
		shared_ptr<T> SearchDynamicComponent()const {
			auto mapPtr = GetCompoMap();
			auto it = mapPtr.begin();
			while (it != mapPtr.end()) {
				auto Ptr = dynamic_pointer_cast<T>(it->second);
				if (Ptr) {
					return Ptr;
				}
				it++;
			}
			return nullptr;
		}
		map<type_index, shared_ptr<Component> >& GetCompoMap() const;
		void RemoveTgtComponent(type_index TypeIndex);

		map<type_index, shared_ptr<Behavior> >& GetBehaviorMap() const;
		shared_ptr<Behavior> SearchBehavior(type_index TypeIndex)const;
		void AddMakedBehavior(type_index TypeIndex, const shared_ptr<Behavior>& Ptr);



	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit GameObject(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理が有効かどうかを得る
		@return	更新処理が有効ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsUpdateActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理が有効かどうかを得る
		@return	更新処理が有効ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetUpdateActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理が有効かどうかを設定する
		@param[in]	b	更新処理が有効ならならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetUpdateActive(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理が有効かどうかを得る
		@return	描画処理が有効ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsDrawActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理が有効かどうかを得る
		@return	描画処理が有効ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetDrawActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理が有効かどうかを設定する
		@param[in]	b	描画処理が有効ならならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDrawActive(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	透明処理（半透明含む）が有効かどうかを得る
		@return	透明処理（半透明含む）が有効ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsAlphaActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	透明処理（半透明含む）が有効かどうかを得る
		@return	透明処理（半透明含む）が有効ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetAlphaActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	透明処理（半透明含む）が有効かどうかを設定する
		@param[in]	b	透明処理（半透明含む）が有効ならならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAlphaActive(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スプライトとしてDrawするかどうかを得る
		@return	スプライトとしてDrawするならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsSpriteDraw() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スプライトとしてDrawするかどうかを得る
		@return	スプライトとしてDrawするならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetSpriteDraw() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スプライトとしてDrawするかどうかを設定する
		@param[in]	b	スプライトとしてDrawするならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetSpriteDraw(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画レイヤーを得る（デフォルト0）
		@return	描画レイヤー
		*/
		//--------------------------------------------------------------------------------------
		int GetDrawLayer() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画レイヤーを設定する
		@param[in]	l	描画レイヤー（マイナス設定可）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void  SetDrawLayer(int l);

		//--------------------------------------------------------------------------------------
		/*!
		@brief	タグのセットを得る
		@return	タグのセット
		*/
		//--------------------------------------------------------------------------------------
		set<wstring>& GetTagSet() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定するタグが存在するかどうかを得る
		@param[in]	tagstr	検証するタグ
		@return	存在すればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool FindTag(const wstring& tagstr) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定するタグを追加する
		@param[in]	tagstr	追加するタグ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void  AddTag(const wstring& tagstr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定するタグが存在したら削除する（存在しない場合は何もしない）
		@param[in]	tagstr	削除するタグ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void  RemoveTag(const wstring& tagstr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	数字タグのセットを得る
		@return	数字タグのセット
		*/
		//--------------------------------------------------------------------------------------
		set<int>& GetNumTagSet() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定する数字タグが存在するかどうかを得る
		@param[in]	numtag	検証する数字タグ
		@return	存在すればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool FindNumTag(int numtag) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定する数字タグを追加する
		@param[in]	numtag	追加する数字タグ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void  AddNumTag(int numtag);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定する数字タグが存在したら削除する（存在しない場合は何もしない）
		@param[in]	numtag	削除する数字タグ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void  RemoveNumTag(int numtag);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	所属するステージを得る
		@param[in]	ExceptionActive	対象がnullだった場合に例外処理するかどうか
		@return	所属するステージ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetStage(bool ExceptionActive = true) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	所属するステージを得る(型チェックあり)
		@param[in]	ExceptionActive	対象がnullだった場合に例外処理するかどうか
		@return	所属するステージ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetTypeStage(bool ExceptionActive = true) const {
			auto StagePtr = GetStage(ExceptionActive);
			if (!StagePtr) {
				return nullptr;
			}
			auto TargetPtr = dynamic_pointer_cast<T>(StagePtr);
			if (!TargetPtr) {
				if (ExceptionActive) {
					throw BaseException(
						L"ステージがありましたが、型キャストできません",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetTypeStage<T>()"
					);
				}
				else {
					return nullptr;
				}
			}
			return TargetPtr;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	所属するステージを設定する
		@param[in]	stage	所属するステージ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetStage(const shared_ptr<Stage>& stage);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンポーネントの取得
		@tparam	T	取得する型
		@param[in]	ExceptionActive	対象がnullだった場合に例外処理するかどうか
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		shared_ptr<T> GetComponent(bool ExceptionActive = true)const {
			auto Ptr = SearchComponent(type_index(typeid(T)));
			if (Ptr) {
				//指定の型のコンポーネントが見つかった
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr) {
					return RetPtr;
				}
				else {
					throw BaseException(
						L"コンポーネントがありましたが、型キャストできません",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetComponent<T>()"
					);
				}
			}
			else {
				if (ExceptionActive) {
					throw BaseException(
						L"コンポーネントが見つかりません",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetComponent<T>()"
					);
				}
			}
			return nullptr;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Rigidbodyコンポーネントの取得
		@param[in]	ExceptionActive	対象がnullだった場合に例外処理するかどうか
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<Rigidbody> GetComponent<Rigidbody>(bool ExceptionActive)const {
			auto Ptr = GetRigidbody();
			if (!Ptr) {
				if (ExceptionActive) {
					throw BaseException(
						L"コンポーネントが見つかりません",
						L"Rigidbody",
						L"GameObject::GetComponent<Rigidbody>()"
					);
				}
				else {
					return nullptr;
				}
			}
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Collisionコンポーネントの取得。親クラスなのでGetのみ用意
		@param[in]	ExceptionActive	対象がnullだった場合に例外処理するかどうか
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<Collision> GetComponent<Collision>(bool ExceptionActive)const {
			auto Ptr = GetCollision();
			if (!Ptr) {
				if (ExceptionActive) {
					throw BaseException(
						L"コンポーネントが見つかりません",
						L"Collision",
						L"GameObject::GetComponent<Collision>()"
					);
				}
				else {
					return nullptr;
				}
			}
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CollisionSphereコンポーネントの取得
		@param[in]	ExceptionActive	対象がnullだった場合に例外処理するかどうか
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<CollisionSphere> GetComponent<CollisionSphere>(bool ExceptionActive)const {
			auto Ptr = GetCollisionSphere();
			if (!Ptr) {
				if (ExceptionActive) {
					throw BaseException(
						L"コンポーネントが見つかりません",
						L"CollisionSphere",
						L"GameObject::GetComponent<CollisionSphere>()"
					);
				}
				else {
					return nullptr;
				}
			}
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CollisionCapsuleコンポーネントの取得
		@param[in]	ExceptionActive	対象がnullだった場合に例外処理するかどうか
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<CollisionCapsule> GetComponent<CollisionCapsule>(bool ExceptionActive)const {
			auto Ptr = GetCollisionCapsule();
			if (!Ptr) {
				if (ExceptionActive) {
					throw BaseException(
						L"コンポーネントが見つかりません",
						L"CollisionCapsule",
						L"GameObject::GetComponent<CollisionCapsule>()"
					);
				}
				else {
					return nullptr;
				}
			}
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CollisionObbコンポーネントの取得
		@param[in]	ExceptionActive	対象がnullだった場合に例外処理するかどうか
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<CollisionObb> GetComponent<CollisionObb>(bool ExceptionActive)const {
			auto Ptr = GetCollisionObb();
			if (!Ptr) {
				if (ExceptionActive) {
					throw BaseException(
						L"コンポーネントが見つかりません",
						L"CollisionObb",
						L"GameObject::GetComponent<CollisionObb>()"
					);
				}
				else {
					return nullptr;
				}
			}
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CollisionTrianglesコンポーネントの取得
		@param[in]	ExceptionActive	対象がnullだった場合に例外処理するかどうか
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<CollisionTriangles> GetComponent<CollisionTriangles>(bool ExceptionActive)const {
			auto Ptr = GetCollisionTriangles();
			if (!Ptr) {
				if (ExceptionActive) {
					throw BaseException(
						L"コンポーネントが見つかりません",
						L"CollisionTriangles",
						L"GameObject::GetComponent<CollisionTriangles>()"
					);
				}
				else {
					return nullptr;
				}
			}
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CollisionRectコンポーネントの取得
		@param[in]	ExceptionActive	対象がnullだった場合に例外処理するかどうか
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<CollisionRect> GetComponent<CollisionRect>(bool ExceptionActive)const {
			auto Ptr = GetCollisionRect();
			if (!Ptr) {
				if (ExceptionActive) {
					throw BaseException(
						L"コンポーネントが見つかりません",
						L"CollisionRect",
						L"GameObject::GetComponent<CollisionRect>()"
					);
				}
				else {
					return nullptr;
				}
			}
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Transformコンポーネントの取得
		@param[in]	ExceptionActive	対象がnullだった場合に例外処理するかどうか
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<Transform> GetComponent<Transform>(bool ExceptionActive)const {
			auto Ptr = GetTransform();
			if (!Ptr) {
				if (ExceptionActive) {
					throw BaseException(
						L"コンポーネントが見つかりません",
						L"Transform",
						L"GameObject::GetComponent<Transform>()"
					);
				}
				else {
					return nullptr;
				}
			}
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	DynamicCastを利用したコンポーネントの取得。コンポーネントの派生クラス使用時に使う
		@tparam	T	取得する型（Tに型変換できるもの）
		@param[in]	ExceptionActive	対象がnullだった場合に例外処理するかどうか
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		shared_ptr<T> GetDynamicComponent(bool ExceptionActive = true)const {
			auto Ptr = SearchDynamicComponent<T>();
			if (Ptr) {
				return Ptr;
			}
			else {
				if (ExceptionActive) {
					throw BaseException(
						L"型キャストできるコンポーネントが見つかりません",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetDynamicComponent<T>()"
					);
				}
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンポーネントの追加。新規にコンポーネントクラスを作る場合、コンストラクタの第1引数はGameObjectとして作成する。
		@tparam	T	追加する型
		@tparam	Ts	可変長変数の型
		@param[in]	params	このコンポーネントを構築するのに使用するパラメータ。（第2パラメータ以降）
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> AddComponent(Ts&&... params) {
			auto Ptr = SearchComponent(type_index(typeid(T)));
			//指定の型のコンポーネントが見つかった
			if (Ptr) {
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr) {
					return RetPtr;
				}
				else {
					throw BaseException(
						L"すでにコンポーネントがありましたが、型キャストできません",
						Util::GetWSTypeName<T>(),
						L"GameObject::AddComponent<T>()"
					);
				}
			}
			else {
				//見つからない。新たに作成する
				shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>(), params...);
				AddMakedComponent(type_index(typeid(T)), newPtr);
				return newPtr;
			}
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Rigidbodyコンポーネントの追加
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<Rigidbody> AddComponent<Rigidbody>() {
			auto Ptr = GetRigidbody();
			if (Ptr) {
				return Ptr;
			}
			else {
				//無ければ新たに制作する
				auto RigidbodyPtr = ObjectFactory::Create<Rigidbody>(GetThis<GameObject>());
				SetRigidbody(RigidbodyPtr);
				return RigidbodyPtr;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Collisionコンポーネントの追加。参照はできるが、直接作成はできない
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<Collision> AddComponent<Collision>() {
			auto Ptr = GetCollision();
			if (Ptr) {
				return Ptr;
			}
			else {
				//無くても作成できない
				throw BaseException(
					L"このコンポーネントは作成できません。",
					L"Collision",
					L"GameObject::AddComponent<Collision>()"
				);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CollisionSphereコンポーネントの追加
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<CollisionSphere> AddComponent<CollisionSphere>() {
			auto Ptr = GetCollisionSphere();
			if (Ptr) {
				return Ptr;
			}
			else {
				//無ければ新たに制作する
				auto CollisionSpherePtr = ObjectFactory::Create<CollisionSphere>(GetThis<GameObject>());
				SetCollision(CollisionSpherePtr);
				return CollisionSpherePtr;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CollisionCapsuleコンポーネントの追加
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<CollisionCapsule> AddComponent<CollisionCapsule>() {
			auto Ptr = GetCollisionCapsule();
			if (Ptr) {
				return Ptr;
			}
			else {
				//無ければ新たに制作する
				auto CollisionCapsulePtr = ObjectFactory::Create<CollisionCapsule>(GetThis<GameObject>());
				SetCollision(CollisionCapsulePtr);
				return CollisionCapsulePtr;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CollisionObbコンポーネントの追加
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<CollisionObb> AddComponent<CollisionObb>() {
			auto Ptr = GetCollisionObb();
			if (Ptr) {
				return Ptr;
			}
			else {
				//無ければ新たに制作する
				auto CollisionObbPtr = ObjectFactory::Create<CollisionObb>(GetThis<GameObject>());
				SetCollision(CollisionObbPtr);
				return CollisionObbPtr;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CollisionTrianglesコンポーネントの追加
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<CollisionTriangles> AddComponent<CollisionTriangles>() {
			auto Ptr = GetCollisionTriangles();
			if (Ptr) {
				return Ptr;
			}
			else {
				//無ければ新たに制作する
				auto CollisionTrianglesPtr = ObjectFactory::Create<CollisionTriangles>(GetThis<GameObject>());
				SetCollision(CollisionTrianglesPtr);
				return CollisionTrianglesPtr;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CollisionRectコンポーネントの追加
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<CollisionRect> AddComponent<CollisionRect>() {
			auto Ptr = GetCollisionRect();
			if (Ptr) {
				return Ptr;
			}
			else {
				//無ければ新たに制作する
				auto CollisionRectPtr = ObjectFactory::Create<CollisionRect>(GetThis<GameObject>());
				SetCollision(CollisionRectPtr);
				return CollisionRectPtr;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Transformコンポーネントの追加
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <>
		shared_ptr<Transform> AddComponent<Transform>() {
			auto Ptr = GetTransform();
			if (Ptr) {
				return Ptr;
			}
			else {
				//無ければ新たに制作する
				auto TransformPtr = ObjectFactory::Create<Transform>(GetThis<GameObject>());
				SetTransform(TransformPtr);
				return TransformPtr;
			}
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンポーネントの削除
		@tparam	T	削除する型
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		void RemoveComponent() {
			RemoveTgtComponent(type_index(typeid(T)));
		}

		//RemoveComponentは特殊化は用意しない
		//固定コンポーネントを無効にする場合は
		//UpdaetActiveをfalseにすべきである

		//--------------------------------------------------------------------------------------
		/*!
		@brief	行動の取得。存在しなければ作成する
		@tparam	T	取得する型
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		shared_ptr<T> GetBehavior() {
			auto Ptr = SearchBehavior(type_index(typeid(T)));
			if (Ptr) {
				//指定の型の行動が見つかった
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr) {
					return RetPtr;
				}
				else {
					throw BaseException(
						L"行動がありましたが、型キャストできません",
						Util::GetWSTypeName<T>(),
						L"GameObject::GetBehavior<T>()"
					);
				}
			}
			else {
				//無ければ新たに制作する
				shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<GameObject>());
				AddMakedBehavior(type_index(typeid(T)), newPtr);
				return newPtr;
			}
			return nullptr;
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief	行動の検索。
		@tparam	T	取得する型
		@return	存在すればtrue
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		bool FindBehavior() {
			auto Ptr = SearchBehavior(type_index(typeid(T)));
			if (Ptr) {
				//指定の型の行動が見つかった
				auto RetPtr = dynamic_pointer_cast<T>(Ptr);
				if (RetPtr) {
					return true;
				}
				else {
					return false;
				}
			}
			return false;
		}



		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンポーネントの更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ComponentUpdate();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突オブジェクトのリセット（BeforHitに移される）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CollisionReset();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップの描画処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void DrawShadowmap();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンポーネントの描画処理（主にDrawコンポーネント向け）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ComponentDraw();

		//--------------------------------------------------------------------------------------
		/*!
		@brief	前初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化（デフォルトは何もしない）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新（デフォルトは何もしない）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突メッセージの発行（ステージより呼ばれる）。
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ToMessageCollision();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突発生時時のイベント（デフォルトは何もしない）
		@param[in]	OtherVec	新しく衝突した相手の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突し続ける相手があった場合のイベント（デフォルトは何もしない）
		@param[in]	OtherVec	衝突し続けた相手の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCollisionExcute(vector<shared_ptr<GameObject>>& OtherVec) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突を抜けた相手があった場合のイベント（デフォルトは何もしない）
		@param[in]	OtherVec	衝突を抜けた相手の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCollisionExit(vector<shared_ptr<GameObject>>& OtherVec) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画前準備（デフォルトは何もしない）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	このオブジェクトを描画するためのカメラを得る（デフォルトはステージのビューから取得）
		@return	カメラのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		virtual const shared_ptr<Camera>& OnGetDrawCamera()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	このオブジェクトを描画するためライトを得る（デフォルトはステージのライトベースから取得）
		@return	ライトの実体
		*/
		//--------------------------------------------------------------------------------------
		virtual  const Light& OnGetDrawLight() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	このオブジェクトを2D描画するための射影行列を得る（デフォルトはステージのビューから取得）
		@param[out]	ProjMatrix	射影行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnGet2DDrawProjMatrix(bsm::Mat4x4& ProjMatrix) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画
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
	///	ゲームオブジェクトのweak_ptrをグループ化したもの
	//--------------------------------------------------------------------------------------
	class GameObjectGroup : public ObjectInterface {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		GameObjectGroup();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameObjectGroup();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	グループ内のゲームオブジェクトのweak_ptrの配列を得る
		@return	グループ内のゲームオブジェクトのweak_ptrの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector< weak_ptr<GameObject> >& GetGroupVector() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	グループ内のゲームオブジェクトを得る
		@param[in]	index	グループ内オブジェクトのインデックス
		@return	グループ内のゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> at(size_t index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	グループ内のゲームオブジェクトの数を得る
		@return	グループ内のゲームオブジェクトの数
		*/
		//--------------------------------------------------------------------------------------
		size_t size() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	グループにゲームオブジェクトを追加する
		@param[in]	Obj	追加するゲームオブジェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void IntoGroup(const shared_ptr<GameObject>& Obj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	グループをクリアする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AllClear();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	パーティクル内の、1つのスプライトをあらわす構造体
	//--------------------------------------------------------------------------------------
	struct ParticleSprite {
		bool m_Active;				//アクティブかどうか
		bsm::Col4 m_Color;				//色（透けさせる場合はaを0にする）
		bsm::Vec2 m_LocalScale;		//ローカルスケーリング（Zは1.0固定）		
		bsm::Quat m_LocalQt;		//ローカル回転。手動のときのみ使用
		bsm::Vec3 m_LocalPos;			//ローカル位置
		bsm::Vec2 m_UVLeftTop;		//UVの左上
		bsm::Vec2 m_UVRightBottom;	//UVの右下
		bsm::Vec3 m_Velocity;			//速度(必要に応じて使用)
		ParticleSprite() :
			m_Active(true),
			m_Color(1.0f, 1.0f, 1.0f, 1.0f),
			m_LocalScale(1.0f, 1.0f),
			m_LocalQt(),
			m_LocalPos(0, 0, 0),
			m_UVLeftTop(0, 0),
			m_UVRightBottom(1.0f, 1.0f),
			m_Velocity(0, 0, 0)
		{}
		void Reflesh() {
			m_Active = true;
			m_Color = bsm::Col4(1.0f, 1.0f, 1.0f, 1.0f);
			m_LocalScale = bsm::Vec2(1.0f, 1.0f);
			m_LocalQt.identity();
			m_LocalPos = bsm::Vec3(0, 0, 0);
			m_UVLeftTop = bsm::Vec2(0, 0);
			m_UVRightBottom = bsm::Vec2(1.0f, 1.0f);
			m_Velocity = bsm::Vec3(0, 0, 0);
		}
	};
	class ParticleManager;
	//--------------------------------------------------------------------------------------
	///	パーティクル
	///	*1つのエミッターを持ち、複数のParticleSpriteを保持する
	//--------------------------------------------------------------------------------------
	class Particle : public ObjectInterface {
	public:
		enum DrawOption {
			Billboard,	//ビルボード
			Faceing,	//フェイシング
			FaceingY,	//フェイシング(Y軸のみ)
			Normal		//通常(手動)
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Count	パーティクルの数
		@param[in]	Option	描画オプション
		*/
		//--------------------------------------------------------------------------------------
		explicit Particle(size_t Count, DrawOption Option = DrawOption::Billboard);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Particle();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画オプションを得る
		@return	描画オプション
		*/
		//--------------------------------------------------------------------------------------
		DrawOption GetDrawOption()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画オプションを設定する
		@param[in]	Option	描画オプション
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDrawOption(DrawOption Option);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	アクティブかどうかを得る
		@return	1つでもアクティブがあればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	すべてがアクティブかどうかを得る
		@return	すべてがアクティブがあればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsAllActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	すべてをアクティブに設定する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAllActive();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	すべてを非アクティブに設定する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAllNoActive();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	パーティクルを作成しなおす
		@param[in]	Count	パーティクルの数
		@param[in]	Option	描画オプション
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Reflesh(size_t Count, Particle::DrawOption Option = Particle::DrawOption::Billboard);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	パーティクルスプライトの配列を得る
		@return	パーティクルスプライトの配列
		*/
		//--------------------------------------------------------------------------------------
		vector<ParticleSprite>& GetParticleSpriteVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エミッター位置を得る
		@return	エミッター位置
		*/
		//--------------------------------------------------------------------------------------
		const bsm::Vec3& GetEmitterPos() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	エミッター位置を設定する
		@param[in]	Pos	エミッター位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetEmitterPos(const bsm::Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	トータルの時間を得る
		@return	トータルの時間
		*/
		//--------------------------------------------------------------------------------------
		float GetTotalTime() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	トータルの時間を設定する
		@param[in]	f	トータルの時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTotalTime(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	トータルの時間を追加する
		@param[in]	f	トータルの時間に加算する時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddTotalTime(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マックスの時間を得る
		@return	マックスの時間
		*/
		//--------------------------------------------------------------------------------------
		float GetMaxTime() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マックスの時間を設定する
		@param[in]	f	マックスの時間
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMaxTime(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テクスチャリソースを得る
		@param[in]	ExceptionActive	無効だった時例外を出すかどうか
		@return	テクスチャリソース
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<TextureResource> GetTextureResource(bool ExceptionActive = true) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テクスチャリソースを設定する
		@param[in]	ResKey	テクスチャリソースキー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const wstring& ResKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テクスチャリソースを設定する
		@param[in]	TextureResourcePtr	テクスチャリソースのポインタ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr);
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Draw(const shared_ptr<ParticleManager>& Manager);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	マルチエフェクト （マルチパーティクル）
	//--------------------------------------------------------------------------------------
	class MultiParticle : public GameObject {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit MultiParticle(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MultiParticle();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	パーティクルの配列を得る
		@return	パーティクルの配列
		*/
		//--------------------------------------------------------------------------------------
		vector< shared_ptr<Particle> >& GetParticleVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	加算描画するかどうかを得る
		@return	加算描画するならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetAddType() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	加算描画するかどうかを得る
		@return	加算描画するならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsAddType() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	加算描画するかどうかを設定する
		@param[in]	b	加算描画するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAddType(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化前処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief パーティクルの追加
		@param[in]	Count	パーティクルの数
		@param[in]	Option	描画オプション
		@return	追加されたパーティクル
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Particle> InsertParticle(size_t Count, Particle::DrawOption Option = Particle::DrawOption::Billboard);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理
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
	///	パーティクルマネージャ
	//--------------------------------------------------------------------------------------
	class ParticleManager : public GameObject {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	AddType	加算描画するかどうか
		*/
		//--------------------------------------------------------------------------------------
		explicit ParticleManager(const shared_ptr<Stage>& StagePtr,bool AddType);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ParticleManager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Zバッファを使用するかどうかを得る
		@return	Zバッファを使用するならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetZBufferUse() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Zバッファを使用するかどうかを得る
		@return	Zバッファを使用するならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsZBufferUse() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Zバッファを使用するかどうかを設定する
		@param[in]	b	Zバッファを使用するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetZBufferUse(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ラッピングサンプラーかどうかを得る
		@returnラッピングサンプラーならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetSamplerWrap() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ラッピングサンプラーかどうかを得る
		@return	ラッピングサンプラーならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsSamplerWrap() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ラッピングサンプラーを設定する
		@param[in]	b	ラッピングサンプラーにするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetSamplerWrap(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画のためのパーティクルスプライトを追加する
		@param[in]	rParticleSprite	パーティクルスプライト
		@param[in]	Option	描画オプション
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddParticle(const ParticleSprite& rParticleSprite, Particle::DrawOption Option,
			const bsm::Vec3& EmitterPos, const shared_ptr<TextureResource>& TextureRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	衝突判定マネージャ
	//--------------------------------------------------------------------------------------
	class CollisionManager : public GameObject {
		void CollisionSub(size_t SrcIndex);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit CollisionManager(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CollisionManager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override;
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	struct CellIndex;
	//--------------------------------------------------------------------------------------
	//	ステージのセルマップで使用するセル構造体
	//--------------------------------------------------------------------------------------
	struct CellPiece{
		CellIndex m_Index;
		CellIndex m_ParentIndex;
		int m_Cost;
		AABB m_PieceRange;
		CellPiece(){}
	};

	//--------------------------------------------------------------------------------------
	//	ステージのセルマップ（派生クラスを作るかインスタンスを作成する）
	//--------------------------------------------------------------------------------------
	class StageCellMap : public GameObject {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	MiniPos	最小地点（起点）
		@param[in]	PieceSize	ピースのサイズ
		@param[in]	PieceCountX	X方向ピース数
		@param[in]	PieceCountZ	Z方向ピース数
		@param[in]	DefaultCost	デフォルトのコスト
		*/
		//--------------------------------------------------------------------------------------
		StageCellMap(const shared_ptr<Stage>& StagePtr,const bsm::Vec3& MiniPos,
			float PieceSize, UINT PieceCountX, UINT PieceCountZ,int DefaultCost = 1);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~StageCellMap();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	セル情報を出力するか
		@return	セル情報を出力するならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsCellStringActive();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	セル情報を出力するかを設定する
		@param[in]	b	セル情報を出力するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetCellStringActive(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	位置情報からセルを検索する
		@param[in]	Pos	位置情報
		@param[out]	ret	返されるセルインデックス
		@return	セルが検索出来たらtrue
		*/
		//--------------------------------------------------------------------------------------
		bool FindCell(const bsm::Vec3& Pos,CellIndex& ret);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	位置情報から一番近いセルを検索する
		@param[in]	Pos	位置情報
		@param[out]	ret	返されるセルインデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void FindNearCell(const bsm::Vec3& Pos, CellIndex& ret);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	セルインデックスからAABBを検索する
		@param[in]	Index	セルインデックス
		@param[out]	ret	返されるAABB
		@return	AABBが検索出来たらtrue
		*/
		//--------------------------------------------------------------------------------------
		bool FindAABB(const CellIndex& Index,AABB& ret);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	セルインデックスから一番近いAABBを検索する
		@param[in]	Index	セルインデックス
		@param[out]	ret	返されるAABB
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void FindNearAABB(const bsm::Vec3& Pos, AABB& ret);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マップのAABBを検索する
		@param[out]	ret	返されるAABB
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetMapAABB(AABB& ret) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	セルの2次元配列を得る
		@return	セルの2次元配列
		*/
		//--------------------------------------------------------------------------------------
		vector<vector<CellPiece>>& GetCellVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	セルマップを作成しなおす
		@param[in]	MiniPos	最小地点（起点）
		@param[in]	PieceSize	ピースのサイズ
		@param[in]	PieceCountX	X方向ピース数
		@param[in]	PieceCountZ	Z方向ピース数
		@param[in]	DefaultCost	デフォルトのコスト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void RefleshCellMap(const bsm::Vec3& MiniPos,
			float PieceSize, UINT PieceCountX, UINT PieceCountZ, int DefaultCost = 1);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw() override;
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	class PhysicsManager;
	//--------------------------------------------------------------------------------------
	//	ステージクラス
	//--------------------------------------------------------------------------------------
	class Stage :public ObjectInterface, public ShapeInterface {
		//プライベートサブ関数
		void PushBackGameObject(const shared_ptr<GameObject>& Ptr);
		void RemoveBackGameObject(const shared_ptr<GameObject>& Ptr);
		shared_ptr<GameObject> GetSharedGameObjectEx(const wstring& Key, bool ExceptionActive) const;
		void SetParentStage(const shared_ptr<Stage>& ParentStage);
		void AddChileStageBase(const shared_ptr<Stage>& ChildStage);
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Stage();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Stage();
	public:
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理が有効かどうかを得る
		@return	更新処理が有効ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsUpdateActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理が有効かどうかを得る
		@return	更新処理が有効ならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetUpdateActive() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理が有効かどうかを設定する
		@param[in]	b	更新処理が有効ならならtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetUpdateActive(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	パーティクルマネージャを得る<br />
		パーティクルマネージャは、加算処理する、しないの2種類実装される
		@param[in]	AddType	加算処理するタイプかどうか
		@return	パーティクルマネージャ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ParticleManager> GetParticleManager(bool AddType) const;

		shared_ptr<PhysicsManager> GetPhysicsManager() const;

		bool IsPhysicsActive() const;
		void SetPhysicsActive(bool b);

		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトの配列を得る
		@return	ゲームオブジェクトの配列
		*/
		//--------------------------------------------------------------------------------------
		vector< shared_ptr<GameObject> >& GetGameObjectVec();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	子ステージの配列を得る
		@return	子ステージの配列
		*/
		//--------------------------------------------------------------------------------------
		vector< shared_ptr<Stage> >& GetChileStageVec();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	子ステージを作成する
		@tparam	T	作成する型
		@return	作成された子ステージ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<Stage> AddChileStage() {
			auto Ptr = ObjectFactory::Create<T>();
			auto StagePtr = dynamic_pointer_cast<Stage>(Ptr);
			if (!StagePtr) {
				throw BaseException(
					L"以下はStageに型キャストできません。",
					Util::GetWSTypeName<T>(),
					L"Stage::AddChileStage<T>()"
				);
			}
			AddChileStageBase(Ptr);
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	親ステージを得る
		@return	親ステージ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetParentStage() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトを追加する
		@tparam	T	作成する型
		@tparam	Ts	可変長パラメータの型
		@param[in]	params	可変長パラメータ
		@return	作成されたゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> AddGameObject(Ts&&... params) {
			try {
				auto Ptr = ObjectFactory::Create<T>(GetThis<Stage>(), params...);
				PushBackGameObject(Ptr);
				return Ptr;
			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	パラメータをOnCreateWithParamに渡すゲームオブジェクトを追加する
		@tparam	T	作成する型
		@tparam	Ts	可変長パラメータの型
		@param[in]	params	可変長パラメータ
		@return	作成されたゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> AddGameObjectWithParam(Ts&&... params) {
			try {
				auto Ptr = ObjectFactory::CreateGameObjectWithParam<T>(GetThis<Stage>(), params...);
				PushBackGameObject(Ptr);
				return Ptr;
			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インスタンスからそのゲームオブジェクトが指定した型かどうかを得る
		@tparam	T	調べる型
		@param[in]	Obj	インスタンス
		@return	指定した型で存在すればtrue
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		bool FindGameObject(const shared_ptr<GameObject>& Obj) {
			auto shptr = dynamic_pointer_cast<T>(Obj);
			if (shptr) {
				for (auto ptr : GetGameObjectVec()) {
					if (Obj == ptr) {
						return true;
					}
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトを削除する
		@tparam	T	削除する型
		@param[in]	Obj	インスタンス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void RemoveGameObject(const shared_ptr<GameObject>& Obj) {
			if (FindGameObject<T>(Obj)) {
				RemoveBackGameObject(Obj);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	追加待ちになってるオブジェクトを追加する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetWaitToObjectVec();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	共有されてるゲームオブジェクトを取得する
		@tparam	T	取得する型
		@param[in]	Key	キー
		@param[in]	ExceptionActive	見つからないときに例外を発行するかどうか
		@return	共有されてるゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetSharedGameObject(const wstring& Key, bool ExceptionActive = true)const {
			shared_ptr<T> Ptr = dynamic_pointer_cast<T>(GetSharedGameObjectEx(Key, ExceptionActive));
			if (Ptr) {
				return Ptr;
			}
			else {
				//キャストできない
				if (ExceptionActive) {
					//例外発生
					wstring keyerr = Key;
					wstring str = L"オブジェクトを";
					str += Util::GetWSTypeName<T>();
					str += L"型にキャストできません";
					throw BaseException(
						str,
						keyerr,
						L"Stage::GetSharedGameObject()"
					);
				}
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	共有されてるゲームオブジェクトを取得する（GameObject型）
		@param[in]	Key	キー
		@param[in]	ExceptionActive	見つからないときに例外を発行するかどうか
		@return	共有されてるゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetSharedObject(const wstring& Key, bool ExceptionActive = true)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトを共有する
		@param[in]	Key	キー
		@param[in]	Ptr	ゲームオブジェクトのポインタ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetSharedGameObject(const wstring& Key, const shared_ptr<GameObject>& Ptr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	共有するゲームオブジェクトグループを作成する
		@param[in]	Key	キー
		@return	共有するゲームオブジェクトグループ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObjectGroup> CreateSharedObjectGroup(const wstring& Key);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	共有するゲームオブジェクトグループを得る
		@param[in]	Key	キー
		@param[in]	ExceptionActive	見つからないときに例外を発行するかどうか
		@return	共有するゲームオブジェクトグループ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObjectGroup> GetSharedObjectGroup(const wstring& Key, bool ExceptionActive = true)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	共有するゲームオブジェクトグループを得る（グループ派生クラスを作った場合用）
		@tparam	T	取得する型
		@param[in]	Key	キー
		@param[in]	ExceptionActive	見つからないときに例外を発行するかどうか
		@return	共有するゲームオブジェクトグループ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetSharedObjectGroup(const wstring& Key, bool ExceptionActive = true)const {
			auto RetPtr = dynamic_pointer_cast<T>(GetSharedObjectGroup(Key, ExceptionActive));
			if (RetPtr) {
				return RetPtr;
			}
			else {
				if (ExceptionActive) {
					//例外発生
					wstring keyerr = Key;
					throw BaseException(
						L"指定のきーのグループはT型に変換できません",
						keyerr,
						L"Stage::GetSharedObjectGroup<T>()"
					);
				}
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	共有するゲームオブジェクトグループをセットする（グループ派生クラスを作った場合用）
		@param[in]	Key	キー
		@param[in]	NewPtr	共有するゲームオブジェクトグループ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetSharedObjectGroup(const wstring& Key, const shared_ptr<GameObjectGroup>& NewPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ビューをセットする
		@param[in]	v	ビュー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetView(const shared_ptr<ViewBase>& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ビューを得る
		@param[in]	ExceptionActive	無かった時例外を出すかどうか
		@return	ビュー
		*/
		//--------------------------------------------------------------------------------------
		const shared_ptr<ViewBase>& GetView(bool ExceptionActive = true)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライトをセットする
		@param[in]	L	ライト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLight(const shared_ptr<LightBase>& L);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライトを得る
		@return	ライト
		*/
		//--------------------------------------------------------------------------------------
		const shared_ptr<LightBase>& GetLight()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ビューを作成する
		@tparam	T	作成する型
		@tparam	Ts	可変長パラメータの型
		@param[in]	params	可変長パラメータ
		@return	ビュー
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> CreateView(Ts&&... params) {
			//新たに作成する
			shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<Stage>(), params...);
			SetView(newPtr);
			return newPtr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ライトを作成する
		@tparam	T	作成する型
		@tparam	Ts	可変長パラメータの型
		@param[in]	params	可変長パラメータ
		@return	ライト
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> CreateLight(Ts&&... params) {
			//新たに作成する
			shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<Stage>(), params...);
			SetLight(newPtr);
			return newPtr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステージ内の更新（シーンからよばれる）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateStage();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突判定の更新（ステージから呼ぶ）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateCollision();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	衝突メッセージの発行
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateMessageCollision();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップを使うかどうかを得る
		@return	シャドウマップを使うかどうか
		*/
		//--------------------------------------------------------------------------------------
		bool IsShadowmapDraw() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップを使うかどうかを設定する
		@param[in]	b	シャドウマップを使うかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetShadowmapDraw(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステージ内のシャドウマップ描画（ステージからよばれる）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void DrawShadowmapStage();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステージ内の描画（ステージからよばれる）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void DrawStage();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステージ内のすべての描画（シーンからよばれる）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void RenderStage();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	前初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreCreate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新（デフォルトは何もしない）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画（デフォルトは何もしない）
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
	///	シーン親クラス
	//--------------------------------------------------------------------------------------
	class SceneBase :public SceneInterface {
		void SetActiveStage(const shared_ptr<Stage>& stage);
		void ConvertVertex(const vector<VertexPositionNormalTexture>& vertices,
			vector<VertexPositionColor>& new_pc_vertices,
			vector<VertexPositionTexture>& new_pt_vertices,
			vector<VertexPositionNormalTangentTexture>& new_pntnt_vertices
			);
	protected:
		SceneBase();
		virtual ~SceneBase();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のステージを得る
		@return	現在のステージ（失敗事例外）
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetActiveStage() const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	画面をクリアする色を得る
		@return	画面をクリアする色
		*/
		//--------------------------------------------------------------------------------------
		bsm::Col4 GetClearColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	画面をクリアする色を設定する
		@param[in]	params	このステージを構築するのに使用するパラメータ。
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetClearColor(const bsm::Col4& col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定の型の現在のステージを得る
		@tparam	T	取得する型
		@return	指定の型のステージ（失敗事例外）
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetActiveTypeStage() const {
			auto TypeStagePtr = dynamic_pointer_cast<T>(GetActiveStage());
			if (!TypeStagePtr) {
				throw BaseException(
					L"以下に型キャストできません。",
					Util::GetWSTypeName<T>(),
					L"SceneBase::GetActiveTypeStage<T>()"
				);
			}
			return TypeStagePtr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	アクティブなステージを設定して初期化する
		@tparam	T	取得する型（Stageに型変換できるもの）
		@tparam	Ts	可変長変数の型
		@param[in]	params	このステージを構築するのに使用するパラメータ。
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> ResetActiveStage(Ts&&... params) {
			auto Ptr = ObjectFactory::Create<T>(params...);
			auto StagePtr = dynamic_pointer_cast<Stage>(Ptr);
			if (!StagePtr) {
				throw BaseException(
					L"以下はStageに型キャストできません。",
					Util::GetWSTypeName<T>(),
					L"SceneBase::ResetActiveStage<T>()"
				);
			}
			SetActiveStage(StagePtr);
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	アクティブなステージを設定して初期化する
		@tparam	T	取得する型（Stageに型変換できるもの）
		@tparam	Ts	可変長変数の型
		@param[in]	params	このステージを構築するのに使用するパラメータ。
		@return	コンポーネント
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> ResetActiveStageWithParam(Ts&&... params) {
			auto Ptr = ObjectFactory::CreateWithParam<T>(params...);
			auto StagePtr = dynamic_pointer_cast<Stage>(Ptr);
			if (!StagePtr) {
				throw BaseException(
					L"以下はStageに型キャストできません。",
					Util::GetWSTypeName<T>(),
					L"SceneBase::ResetActiveStageWithParam<T>()"
				);
			}
			SetActiveStage(StagePtr);
			return Ptr;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	シーンを変化させる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シーンを描画する
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
	//	ゲームオブジェクトクリエーターCSV(親)
	//--------------------------------------------------------------------------------------
	class GameObjectCreatorBaseCSV {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		explicit GameObjectCreatorBaseCSV() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameObjectCreatorBaseCSV() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトを作成する（純粋仮想関数）
		@param[in]	StagePtr	ステージ
		@param[in]	Line	1行分のCSV文字列
		@return	ゲームオブジェクトのポインタ
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<GameObject> Create(const shared_ptr<Stage>& StagePtr, const wstring& Line) = 0;
	};
	//--------------------------------------------------------------------------------------
	//	ゲームオブジェクトクリエーターCSV
	//--------------------------------------------------------------------------------------
	template<typename T>
	class GameObjectCreatorCSV : public GameObjectCreatorBaseCSV {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		explicit GameObjectCreatorCSV() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameObjectCreatorCSV() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトを作成する（仮想関数）
		@param[in]	StagePtr	ステージ
		@param[in]	Line	1行分のCSV文字列
		@return	ゲームオブジェクトのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> Create(const shared_ptr<Stage>& StagePtr, const wstring& Line)override {
			return StagePtr->AddGameObject<T>(Line);
		}
	};

	//--------------------------------------------------------------------------------------
	//	ゲームオブジェクトビルダーCSV
	//--------------------------------------------------------------------------------------
	class GameObjecttCSVBuilder {
		map<wstring, shared_ptr<GameObjectCreatorBaseCSV>>& GetCreatorMap() const;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		GameObjecttCSVBuilder();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameObjecttCSVBuilder();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クリエーターを登録する
		@tparam	T	登録する型（ゲームオブジェクトの派生）
		@param[in]	ClsName	型の識別名（通常は型名を文字列にしたもの）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void Register(const wstring& ClsName) {
			GetCreatorMap()[ClsName] = shared_ptr<GameObjectCreatorCSV<T>>(new GameObjectCreatorCSV<T>());
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	型の識別名を使ってゲームオブジェクトを構築する
		@param[in]	ClsName	型の識別名（通常は型名を文字列にしたもの）
		@param[in]	StagePtr	所属するステージ
		@param[in]	Line	1行分のCSV文字列
		@return	作成したゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> CreateFromCSV(const wstring& ClsName, const shared_ptr<Stage>& StagePtr, const wstring& Line);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CSVからゲームオブジェクトを構築する
		@param[in]	StagePtr	所属するステージ
		@param[in]	CSVFileName	CSVファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Build(const shared_ptr<Stage>& StagePtr, const wstring& CSVFileName);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	ゲームオブジェクトクリエーターXML(親)
	//--------------------------------------------------------------------------------------
	class GameObjectCreatorBaseXML {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		explicit GameObjectCreatorBaseXML() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameObjectCreatorBaseXML() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトを作成する（純粋仮想関数）
		@param[in]	StagePtr	ステージ
		@param[in]	pNode	XMLノード
		@return	ゲームオブジェクトのポインタ
		*/
		//--------------------------------------------------------------------------------------
		virtual shared_ptr<GameObject> Create(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode) = 0;
	};
	//--------------------------------------------------------------------------------------
	//	ゲームオブジェクトクリエーターXML
	//--------------------------------------------------------------------------------------
	template<typename T>
	class GameObjectCreatorXML : public GameObjectCreatorBaseXML {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		explicit GameObjectCreatorXML() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameObjectCreatorXML() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトを作成する（仮想関数）
		@param[in]	StagePtr	ステージ
		@param[in]	pNode	XMLノード
		@return	ゲームオブジェクトのポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> Create(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode)override {
			return StagePtr->AddGameObject<T>(pNode);
		}
	};

	//--------------------------------------------------------------------------------------
	//	ゲームオブジェクトビルダーXML
	//--------------------------------------------------------------------------------------
	class GameObjecttXMLBuilder {
		map<wstring, shared_ptr<GameObjectCreatorBaseXML>>& GetCreatorMap() const;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		GameObjecttXMLBuilder();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameObjecttXMLBuilder();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	クリエーターを登録する
		@tparam	T	登録する型（ゲームオブジェクトの派生）
		@param[in]	ClsName	型の識別名（通常は型名を文字列にしたもの）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void Register(const wstring& ClsName) {
			GetCreatorMap()[ClsName] = shared_ptr<GameObjectCreatorXML<T>>(new GameObjectCreatorXML<T>());
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	型の識別名を使ってゲームオブジェクトを構築する
		@param[in]	ClsName	型の識別名（通常は型名を文字列にしたもの）
		@param[in]	StagePtr	所属するステージ
		@param[in]	pNode	XMLノード
		@return	作成したゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> CreateFromXML(const wstring& ClsName, const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XMLからゲームオブジェクトを構築する
		@param[in]	StagePtr	所属するステージ
		@param[in]	XMLFileName	XMLファイル名
		@param[in]	GameObjectsPath	ゲームオブジェクト(複数)のノードへのパス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Build(const shared_ptr<Stage>& StagePtr, const wstring& XMLFileName, const wstring& GameObjectsPath);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};







}
//end basecross
