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
		virtual void OnGet2DDrawProjMatrix(Matrix4X4& ProjMatrix) const;
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

		//初期化
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
		Color4 m_Color;				//色（透けさせる場合はaを0にする）
		Vector2 m_LocalScale;		//ローカルスケーリング（Zは1.0固定）		
		Quaternion m_LocalQt;		//ローカル回転。手動のときのみ使用
		Vector3 m_LocalPos;			//ローカル位置
		Vector2 m_UVLeftTop;		//UVの左上
		Vector2 m_UVRightBottom;	//UVの右下
		Vector3 m_Velocity;			//速度(必要に応じて使用)
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
			m_Color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
			m_LocalScale = Vector2(1.0f, 1.0f);
			m_LocalQt.Identity();
			m_LocalPos = Vector3(0, 0, 0);
			m_UVLeftTop = Vector2(0, 0);
			m_UVRightBottom = Vector2(1.0f, 1.0f);
			m_Velocity = Vector3(0, 0, 0);
		}
	};
	class ParticleManager;
	//--------------------------------------------------------------------------------------
	//	class Particle: public Object;
	//	用途: パーティクル
	//	*1つのエミッターを持ち、複数のParticleSpriteを保持する
	//--------------------------------------------------------------------------------------
	class Particle : public ObjectInterface {
	public:
		enum DrawOption {
			Billboard,	//ビルボード
			Faceing,	//フェイシング
			FaceingY,	//フェイシング(Y軸のみ)
			Normal		//通常(手動)
		};
		explicit Particle(size_t Count, DrawOption Option = DrawOption::Billboard);
		virtual ~Particle();
		//アクセサ
		DrawOption GetDrawOption()const;
		void SetDrawOption(DrawOption Option);

		bool IsActive() const;
		bool IsAllActive() const;
		void SetAllActive();
		void SetAllNoActive();

		void Reflesh(size_t Count, Particle::DrawOption Option = Particle::DrawOption::Billboard);

		vector<ParticleSprite>& GetParticleSpriteVec() const;

		const Vector3& GetEmitterPos() const;
		void SetEmitterPos(const Vector3& Pos);

		float GetTotalTime() const;
		void SetTotalTime(float f);
		void AddTotalTime(float f);

		float GetMaxTime() const;
		void SetMaxTime(float f);


		shared_ptr<TextureResource> GetTextureResource(bool ExceptionActive = true) const;
		void SetTextureResource(const wstring& ResKey);
		void SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr);

		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化を行う（仮想関数）<br />
		＊thisポインタが必要なオブジェクトはこの関数を多重定義して、取得できる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override{}
		virtual void Draw(const shared_ptr<ParticleManager>& Manager);

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MultiParticle : public GameObject;
	//	用途: マルチエフェクト 
	//--------------------------------------------------------------------------------------
	class MultiParticle : public GameObject {
	public:
		explicit MultiParticle(const shared_ptr<Stage>& StagePtr);
		virtual ~MultiParticle();
		//アクセサ
		vector< shared_ptr<Particle> >& GetParticleVec() const;

		bool GetAddType() const;
		bool IsAddType() const;
		void SetAddType(bool b);

		//操作
		virtual void OnPreCreate() override;
		shared_ptr<Particle> InsertParticle(size_t Count, Particle::DrawOption Option = Particle::DrawOption::Billboard);
		virtual void OnUpdate()override;
		virtual void OnDraw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class ParticleManager : public GameObject;
	//	用途: パーティクルマネージャ
	//--------------------------------------------------------------------------------------
	class ParticleManager : public GameObject {
	public:
		//構築と消滅
		explicit ParticleManager(const shared_ptr<Stage>& StagePtr,bool AddType);
		virtual ~ParticleManager();
		//初期化
		virtual void OnCreate() override;

		//アクセサ
		bool GetZBufferUse() const;
		bool IsZBufferUse() const;
		void SetZBufferUse(bool b);

		bool GetSamplerWrap() const;
		bool IsSamplerWrap() const;
		void SetSamplerWrap(bool b);

		//操作
		void AddParticle(const ParticleSprite& rParticleSprite, Particle::DrawOption Option,
			const Vector3& EmitterPos, const shared_ptr<TextureResource>& TextureRes);

		virtual void OnUpdate() override {}
		virtual void OnDraw()override;
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	衝突判定管理者
	//--------------------------------------------------------------------------------------
	class CollisionManager : public GameObject {
		void CollisionSub(size_t SrcIndex);
	public:
		//構築と消滅
		explicit CollisionManager(const shared_ptr<Stage>& StagePtr);
		virtual ~CollisionManager();
		//初期化
		virtual void OnCreate() override;
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
		StageCellMap(const shared_ptr<Stage>& StagePtr,const Vector3& MiniPos,
			float PieceSize, UINT PieceCountX, UINT PieceCountZ,int DefaultCost = 1);
		virtual ~StageCellMap();
		bool IsCellStringActive();
		void SetCellStringActive(bool b);
		bool FindCell(const Vector3& Pos,CellIndex& ret);
		void FindNearCell(const Vector3& Pos, CellIndex& ret);
		bool FindAABB(const CellIndex& Index,AABB& ret);
		void FindNearAABB(const Vector3& Pos, AABB& ret);
		void GetMapAABB(AABB& ret) const;
		vector<vector<CellPiece>>& GetCellVec() const;
		void RefleshCellMap(const Vector3& MiniPos,
			float PieceSize, UINT PieceCountX, UINT PieceCountZ, int DefaultCost = 1);
		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnDraw() override;
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};






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

		shared_ptr<ParticleManager> GetParticleManager(bool AddType) const;

		vector< shared_ptr<GameObject> >& GetGameObjectVec();


		vector< shared_ptr<Stage> >& GetChileStageVec();
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

		shared_ptr<Stage> GetParentStage() const;


		//GameObjectの新規追加
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


		template<typename T>
		void RemoveGameObject(const shared_ptr<GameObject>& Obj) {
			if (FindGameObject<T>(Obj)) {
				RemoveBackGameObject(Obj);
			}
		}


		//追加待ちになってるオブジェクトを追加する
		void SetWaitToObjectVec();


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
		shared_ptr<GameObject> GetSharedObject(const wstring& Key, bool ExceptionActive = true)const;
		void SetSharedGameObject(const wstring& Key, const shared_ptr<GameObject>& Ptr);

		shared_ptr<GameObjectGroup> CreateSharedObjectGroup(const wstring& Key);
		shared_ptr<GameObjectGroup> GetSharedObjectGroup(const wstring& Key, bool ExceptionActive = true)const;
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


		void SetSharedObjectGroup(const wstring& Key, const shared_ptr<GameObjectGroup>& NewPtr);



		void SetView(const shared_ptr<ViewBase>& v);
		const shared_ptr<ViewBase>& GetView(bool ExceptionActive = true)const;

		void SetLight(const shared_ptr<LightBase>& L);
		const shared_ptr<LightBase>& GetLight()const;


		template<typename T, typename... Ts>
		shared_ptr<T> CreateView(Ts&&... params) {
			//新たに作成する
			shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<Stage>(), params...);
			SetView(newPtr);
			return newPtr;
		}

		template<typename T, typename... Ts>
		shared_ptr<T> CreateLight(Ts&&... params) {
			//新たに作成する
			shared_ptr<T> newPtr = ObjectFactory::Create<T>(GetThis<Stage>(), params...);
			SetLight(newPtr);
			return newPtr;
		}

		//ステージ内の更新（シーンからよばれる）
		virtual void UpdateStage();
		//衝突判定の更新（ステージから呼ぶ）
		virtual void UpdateCollision();
		//衝突メッセージの発行
		virtual void UpdateMessageCollision();

		//シャドウマップを使うかどうか
		bool IsShadowmapDraw() const;
		void SetShadowmapDraw(bool b);


		//ステージ内のシャドウマップ描画（ステージからよばれる）
		virtual void DrawShadowmapStage();
		//ステージ内の描画（ステージからよばれる）
		virtual void DrawStage();
		//ステージ内のすべての描画（シーンからよばれる）
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
		Color4 GetClearColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	画面をクリアする色を設定する
		@param[in]	params	このステージを構築するのに使用するパラメータ。
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetClearColor(const Color4& col);
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
