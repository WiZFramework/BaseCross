/*!
@file GameObject.cpp
@brief ゲームオブジェクト、ステージ実体
*/

#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct GameObject::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct GameObject::Impl {
		bool m_UpdateActive;	//updateするかどうか
		bool m_DrawActive;		//Drawするかどうか
		bool m_AlphaActive;		//透明かどうか
		int m_DrawLayer;	//描画レイヤー
		bool m_SpriteDraw;	//スプライトとして描画するかどうか

		weak_ptr<Stage> m_Stage;	//所属ステージ
		map<type_index, shared_ptr<Component> > m_CompMap;
		list<type_index> m_CompOrder;	//コンポーネント実行順番

		shared_ptr<Rigidbody> m_Rigidbody;	//Rigidbodyは別にする
		shared_ptr<Gravity> m_Gravity;	//Gravityは別にする
		shared_ptr<Collision> m_Collision;	//Collisionも別にする
		shared_ptr<Transform> m_Transform;	//Transformも別にする


		Impl(const shared_ptr<Stage>& StagePtr) :
			m_UpdateActive(true),
			m_DrawActive(true),
			m_AlphaActive(false),
			m_DrawLayer(0),
			m_SpriteDraw(false),
			m_Stage(StagePtr)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	ゲーム配置オブジェクト親クラス
	//--------------------------------------------------------------------------------------
	//privateメンバ
	shared_ptr<Component> GameObject::SearchComponent(type_index TypeIndex)const {
		auto it = pImpl->m_CompMap.find(TypeIndex);
		if (it != pImpl->m_CompMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	shared_ptr<Transform> GameObject::GetTransform()const {
		return pImpl->m_Transform;

	}
	shared_ptr<Rigidbody> GameObject::GetRigidbody()const {
		return pImpl->m_Rigidbody;
	}
	shared_ptr<Gravity> GameObject::GetGravity()const {
		return pImpl->m_Gravity;
	}
	shared_ptr<Collision> GameObject::GetCollision()const {
		return pImpl->m_Collision;
	}
	shared_ptr<CollisionSphere> GameObject::GetCollisionSphere()const {
		return dynamic_pointer_cast<CollisionSphere>(pImpl->m_Collision);
	}

	shared_ptr<CollisionCapsule> GameObject::GetCollisionCapsule()const {
		return dynamic_pointer_cast<CollisionCapsule>(pImpl->m_Collision);
	}
	shared_ptr<CollisionObb> GameObject::GetCollisionObb()const {
		return dynamic_pointer_cast<CollisionObb>(pImpl->m_Collision);
	}

	void GameObject::SetRigidbody(const shared_ptr<Rigidbody>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Rigidbody = Ptr;
	}
	void GameObject::SetGravity(const shared_ptr<Gravity>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Gravity = Ptr;
	}
	void GameObject::SetCollision(const shared_ptr<Collision>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}
	void GameObject::SetCollisionSphere(const shared_ptr<CollisionSphere>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}

	void GameObject::SetCollisionCapsule(const shared_ptr<CollisionCapsule>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}

	void GameObject::SetCollisionObb(const shared_ptr<CollisionObb>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Collision = Ptr;
	}

	void GameObject::SetTransform(const shared_ptr<Transform>& Ptr) {
		Ptr->AttachGameObject(GetThis<GameObject>());
		pImpl->m_Transform = Ptr;
	}

	void GameObject::AddMakedComponent(type_index TypeIndex, const shared_ptr<Component>& Ptr) {
		if (!SearchComponent(TypeIndex)) {
			//そのコンポーネントがまだなければ新規登録
			pImpl->m_CompOrder.push_back(TypeIndex);
		}
		//mapに追加もしくは更新
		pImpl->m_CompMap[TypeIndex] = Ptr;
		Ptr->AttachGameObject(GetThis<GameObject>());
	}

	map<type_index, shared_ptr<Component> >& GameObject::GetCompoMap()const {
		return pImpl->m_CompMap;
	}

	void GameObject::RemoveTgtComponent(type_index TypeIndex) {
		//順番リストを検証して削除
		auto it = pImpl->m_CompOrder.begin();
		while (it != pImpl->m_CompOrder.end()) {
			if (*it == TypeIndex) {
				auto it2 = pImpl->m_CompMap.find(*it);
				if (it2 != pImpl->m_CompMap.end()) {
					//指定の型のコンポーネントが見つかった
					//mapデータを削除
					pImpl->m_CompMap.erase(it2);
				}
				pImpl->m_CompOrder.erase(it);
				return;
			}
			it++;
		}
	}



	GameObject::GameObject(const shared_ptr<Stage>& StagePtr) :
		ObjectInterface(),
		ShapeInterface(),
		pImpl(new Impl(StagePtr))
	{
	}
	GameObject::~GameObject() {}
	//アクセサ
	bool GameObject::IsUpdateActive() const { return pImpl->m_UpdateActive; }
	bool GameObject::GetUpdateActive() const { return pImpl->m_UpdateActive; }
	void GameObject::SetUpdateActive(bool b) { pImpl->m_UpdateActive = b; }

	bool GameObject::IsDrawActive() const { return pImpl->m_DrawActive; }
	bool GameObject::GetDrawActive() const { return pImpl->m_DrawActive; }
	void GameObject::SetDrawActive(bool b) { pImpl->m_DrawActive = b; }

	bool GameObject::IsAlphaActive() const { return pImpl->m_AlphaActive; }
	bool GameObject::GetAlphaActive() const { return pImpl->m_AlphaActive; }
	void GameObject::SetAlphaActive(bool b) { pImpl->m_AlphaActive = b; }


	//スプライトとしてDrawするかどうか
	bool  GameObject::IsSpriteDraw() const {return pImpl->m_SpriteDraw;}
	bool  GameObject::GetSpriteDraw() const {return pImpl->m_SpriteDraw;}
	void  GameObject::SetSpriteDraw(bool b) {pImpl->m_SpriteDraw = b;}
	//描画レイヤーの取得と設定
	int GameObject::GetDrawLayer() const {return pImpl->m_DrawLayer;}
	void  GameObject::SetDrawLayer(int l) {pImpl->m_DrawLayer = l;}

	shared_ptr<Stage> GameObject::GetStage(bool ExceptionActive) const {
		auto shptr = pImpl->m_Stage.lock();
		if (shptr) {
			return shptr;
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"所属ステージがnullです。自分自身がステージではありませんか？",
					L"if (!shptr)",
					L"GameObject::GetStage()"
				);
			}
			else {
				return nullptr;
			}
		}
		// 所属ステージがnullだった
		// 自分自身がステージの可能性がある
		return nullptr;
	}
	void GameObject::SetStage(const shared_ptr<Stage>& stage) { pImpl->m_Stage = stage; }

	void GameObject::ComponentUpdate() {
		auto TMptr = GetComponent<Transform>();
		auto RigidPtr = GetComponent<Rigidbody>(false);
		auto GravityPtr = GetComponent<Gravity>(false);
		if (RigidPtr) {
			//Rigidbodyがあればフォースを初期化
			RigidPtr->SetForce(0, 0, 0);
		}
		//マップを検証してUpdate
		list<type_index>::iterator it = pImpl->m_CompOrder.begin();
		while (it != pImpl->m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = pImpl->m_CompMap.find(*it);
			if (it2 != pImpl->m_CompMap.end()) {
				//指定の型のコンポーネントが見つかった
				if (it2->second->IsUpdateActive()) {
					it2->second->OnUpdate();
				}
			}
			it++;
		}
		if (GravityPtr && GravityPtr->IsUpdateActive()) {
			//GravityPtrがあればUpdate()
			GravityPtr->OnUpdate();
		}
		if (RigidPtr && RigidPtr->IsUpdateActive()) {
			//RigidbodyがあればUpdate()
			RigidPtr->OnUpdate();
		}
		//TransformのUpdate
		if (TMptr->IsUpdateActive()) {
			TMptr->OnUpdate();
		}
	}

	void GameObject::CollisionReset() {
		auto CollisionPtr = GetComponent<Collision>(false);
		if (CollisionPtr) {
			CollisionPtr->ClearHitObject();
		}
	}

	void GameObject::CollisionChk() {
		auto CollisionPtr = GetComponent<Collision>(false);
		if (CollisionPtr && CollisionPtr->IsUpdateActive()) {
			//CollisionがあればUpdate()
			CollisionPtr->OnUpdate();
		}
	}

	void GameObject::CollisionGravityChk() {
		auto CollisionPtr = GetComponent<Collision>(false);
		if (CollisionPtr && !CollisionPtr->IsOnObject()) {
			auto GravityPtr = GetComponent<Gravity>(false);
			if (GravityPtr) {
				GravityPtr->SetGravityDefault();
			}
		}
	}

	void GameObject::ToMessageCollision() {
		auto CollisionPtr = GetComponent<Collision>(false);
		if (CollisionPtr && CollisionPtr->GetHitObjectVec().size() > 0) {
			OnCollision(CollisionPtr->GetHitObjectVec());
		}
	}

	void GameObject::DrawShadowmap() {
		auto shadowptr = GetDynamicComponent<Shadowmap>(false);
		if (shadowptr) {
			shadowptr->OnDraw();
		}
	}

	void GameObject::ComponentDraw() {
		//Transformがなければ例外
		auto Tptr = GetComponent<Transform>();
		auto RigidPtr = GetComponent<Rigidbody>(false);
		auto GravityPtr = GetComponent<Gravity>(false);
		//マップを検証してDraw
		list<type_index>::iterator it = pImpl->m_CompOrder.begin();
		while (it != pImpl->m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = pImpl->m_CompMap.find(*it);
			//指定の型のコンポーネントが見つかった
			if (it2 != pImpl->m_CompMap.end() && !dynamic_pointer_cast<Shadowmap>(it2->second)) {
				//シャドウマップ以外なら実行
				//そのコンポーネントの子コンポーネントの描画
				if (it2->second->IsDrawActive()) {
					it2->second->OnDraw();
				}
			}
			it++;
		}
		//派生クラス対策
		if (GravityPtr && GravityPtr->IsDrawActive()) {
			//GravityPtrがあればUpdate()
			GravityPtr->OnDraw();
		}
		if (RigidPtr && RigidPtr->IsDrawActive()) {
			//RigidbodyがあればUpdate()
			RigidPtr->OnDraw();
		}
		//TransformのDraw
		if (Tptr->IsDrawActive()) {
			Tptr->OnDraw();
		}
	}

	void GameObject::OnPreCreate() {
		//Transform必須
		AddComponent<Transform>();
	}

	void GameObject::OnDraw() {
		//コンポーネント描画
		//派生クラスで多重定義する場合は
		//コンポーネント描画する場合は
		//GameObject::Draw()を呼び出す
		ComponentDraw();
	}


	//--------------------------------------------------------------------------------------
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct GameObjectGroup::Impl {
		vector< weak_ptr<GameObject> > m_Group;
		Impl() {}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class GameObjectGroup;
	//--------------------------------------------------------------------------------------
	GameObjectGroup::GameObjectGroup() :
		ObjectInterface(),
		pImpl(new Impl())
	{}
	GameObjectGroup::~GameObjectGroup() {}
	//アクセサ
	const vector< weak_ptr<GameObject> >& GameObjectGroup::GetGroupVector() const {
		return pImpl->m_Group;
	}
	shared_ptr<GameObject> GameObjectGroup::at(size_t index) {
		if (index >= pImpl->m_Group.size()) {
			wstring msg = Util::UintToWStr(index);
			msg += L" >= ";
			msg += Util::UintToWStr(pImpl->m_Group.size());
			throw BaseException(
				L"インデックスが範囲外です",
				msg,
				L"GameObjectGroup::at()"
			);
		}
		if (pImpl->m_Group.at(index).expired()) {
			wstring msg = Util::UintToWStr(index);
			throw BaseException(
				L"そのインデックスのオブジェクトは無効です。",
				msg,
				L"GameObjectGroup::at()"
			);
		}
		return pImpl->m_Group.at(index).lock();
	}
	size_t GameObjectGroup::size() const {
		return pImpl->m_Group.size();
	}
	//操作
	void GameObjectGroup::IntoGroup(const shared_ptr<GameObject>& Obj) {
		pImpl->m_Group.push_back(Obj);
	}

	void GameObjectGroup::AllClear() {
		pImpl->m_Group.clear();
	}



	//--------------------------------------------------------------------------------------
	//	struct Stage::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct Stage::Impl {
		bool m_UpdateActive;	//updateするかどうか

		//オブジェクトの配列
		vector< shared_ptr<GameObject> > m_GameObjectVec;
		//途中にオブジェクトが追加された場合、ターンの開始まで待つ配列
		vector< shared_ptr<GameObject> > m_WaitAddObjectVec;
		//現在Drawされているビューのインデックス
		size_t m_DrawViewIndex;
		//ビューのポインタ
		shared_ptr<ViewBase> m_ViewBase;
		//ライトのポインタ
		shared_ptr<LightBase> m_LightBase;

		//シェアオブジェクトポインタのマップ
		map<const wstring, weak_ptr<GameObject> > m_SharedMap;
		//シェアグループのポインタのマップ
		map<const wstring, shared_ptr<GameObjectGroup> >  m_SharedGroupMap;


		vector< shared_ptr<Stage> > m_ChildStageVec;	//子供ステージの配列
		weak_ptr<Stage> m_ParentStage;		//親ステージ



		Impl() :
			m_UpdateActive(true),
			m_DrawViewIndex(0)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	ステージクラス
	//--------------------------------------------------------------------------------------
	Stage::Stage() :
		ObjectInterface(),
		ShapeInterface(),
		pImpl(new Impl())
	{}
	Stage::~Stage() {}

	//プライベートサブ関数
	void Stage::PushBackGameObject(const shared_ptr<GameObject>& Ptr) {
		//このステージはクリエイト後である
		if (IsCreated()) {
			pImpl->m_WaitAddObjectVec.push_back(Ptr);
		}
		else {
			//クリエイト前
			pImpl->m_GameObjectVec.push_back(Ptr);
		}
	}

	shared_ptr<GameObject> Stage::GetSharedGameObjectEx(const wstring& Key, bool ExceptionActive)const {
		map<const wstring, weak_ptr<GameObject> >::const_iterator it;
		//重複キーの検査
		it = pImpl->m_SharedMap.find(Key);
		if (it != pImpl->m_SharedMap.end()) {
			if (it->second.expired()) {
				//すでに無効
				if (ExceptionActive) {
					//例外発生
					wstring keyerr = Key;
					throw BaseException(
						L"オブジェクトが無効です",
						keyerr,
						L"Stage::GetSharedGameObject()"
					);
				}
			}
			return it->second.lock();
		}
		else {
			//指定の名前が見つからなかった
			if (ExceptionActive) {
				//例外発生
				wstring keyerr = Key;
				throw BaseException(
					L"オブジェクトが見つかりません",
					keyerr,
					L"Stage::GetSharedGameObject()"
				);
			}
		}
		return nullptr;
	}


	vector< shared_ptr<GameObject> >& Stage::GetGameObjectVec() { return pImpl->m_GameObjectVec; }

	shared_ptr<GameObject> Stage::GetSharedObject(const wstring& Key, bool ExceptionActive)const {
		shared_ptr<GameObject> Ptr = GetSharedGameObjectEx(Key, ExceptionActive);
		return Ptr;
	}


	void Stage::SetSharedGameObject(const wstring& Key, const shared_ptr<GameObject>& Ptr) {
		map<const wstring, weak_ptr<GameObject> >::iterator it;
		//重複キーの検査
		it = pImpl->m_SharedMap.find(Key);
		if (it != pImpl->m_SharedMap.end()) {
			//既に存在した
			//例外発生
			wstring keyerr = Key;
			throw BaseException(
				L"同名のシェアオブジェクトがあります",
				keyerr,
				L"Stage::SetSharedGameObjectEx()"
			);
		}
		else {
			pImpl->m_SharedMap[Key] = Ptr;
		}
	}

	shared_ptr<GameObjectGroup> Stage::CreateSharedObjectGroup(const wstring& Key) {
		try {
			map<const wstring, shared_ptr<GameObjectGroup> >::iterator it;
			//重複キーの検査
			it = pImpl->m_SharedGroupMap.find(Key);
			if (it != pImpl->m_SharedGroupMap.end()) {
				//既に存在した
				//例外発生
				wstring keyerr = Key;
				throw BaseException(
					L"同名のシェアオブジェクト配列があります",
					keyerr,
					L"Stage::CreateSharedObjectGroup()"
				);
			}
			else {
				auto Ptr = ObjectFactory::Create<GameObjectGroup>();
				pImpl->m_SharedGroupMap[Key] = Ptr;
				return Ptr;
			}
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<GameObjectGroup> Stage::GetSharedObjectGroup(const wstring& Key, bool ExceptionActive)const {
		//重複キーの検査
		auto it = pImpl->m_SharedGroupMap.find(Key);
		if (it != pImpl->m_SharedGroupMap.end()) {
			//ペアのsecondを返す
			return it->second;
		}
		else {
			//指定の名前が見つからなかった
			if (ExceptionActive) {
				//例外発生
				wstring keyerr = Key;
				throw BaseException(
					L"指定のキーが見つかりません",
					keyerr,
					L"Stage::GetSharedObjectGroup() const"
				);
			}
		}
		return nullptr;
	}

	void Stage::SetSharedObjectGroup(const wstring& Key, const shared_ptr<GameObjectGroup>& NewPtr) {
		//重複キーの検査
		auto it = pImpl->m_SharedGroupMap.find(Key);
		if (it != pImpl->m_SharedGroupMap.end()) {
			//例外発生
			wstring keyerr = Key;
			throw BaseException(
				L"同名のシェアオブジェクト配列があります",
				keyerr,
				L"Stage::SetSharedObjectGroup()"
			);
		}
		else {
			//指定の名前が見つからなかった
			//登録できる
			pImpl->m_SharedGroupMap[Key] = NewPtr;
		}
	}


	vector< shared_ptr<Stage> >& Stage::GetChileStageVec() {
		return pImpl->m_ChildStageVec;
	}
	void Stage::AddChileStageBase(const shared_ptr<Stage>& ChildStage) {
		pImpl->m_ChildStageVec.push_back(ChildStage);
		ChildStage->SetParentStage(GetThis<Stage>());
	}

	shared_ptr<Stage> Stage::GetParentStage() const {
		if (!pImpl->m_ParentStage.expired()) {
			return  pImpl->m_ParentStage.lock();
		}
		return nullptr;
	}
	void Stage::SetParentStage(const shared_ptr<Stage>& ParentStage) {
		pImpl->m_ParentStage = ParentStage;
	}


	void Stage::SetView(const shared_ptr<ViewBase>& v) {
		pImpl->m_ViewBase = v;
	}

	const shared_ptr<ViewBase>& Stage::GetView()const {
		if (!pImpl->m_ViewBase) {
			throw BaseException(
				L"ステージにビューが設定されていません。",
				L"if (!pImpl->m_ViewBase)",
				L"Stage::GetView()"
			);
		}
		return pImpl->m_ViewBase;
	}

	void Stage::SetLight(const shared_ptr<LightBase>& L) {
		pImpl->m_LightBase = L;

	}
	const shared_ptr<LightBase>& Stage::GetLight()const {
		if (!pImpl->m_LightBase) {
			throw BaseException(
				L"ステージにライトが設定されていません。",
				L"if (!pImpl->m_LightBase)",
				L"Stage::GetLight()"
			);
		}
		return pImpl->m_LightBase;
	}


	//アクセサ
	bool Stage::IsUpdateActive() const { return pImpl->m_UpdateActive; }
	bool Stage::GetUpdateActive() const { return pImpl->m_UpdateActive; }
	void Stage::SetUpdateActive(bool b) { pImpl->m_UpdateActive = b; }




	//ステージ内の更新（シーンからよばれる）
	void Stage::UpdateStage() {
		//Transformコンポーネントの値をバックアップにコピー
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				auto ptr2 = ptr->GetComponent<Transform>();
				ptr2->SetToBefore();
			}
		}
		//配置オブジェクトの更新1
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->OnUpdate();
			}
		}
		//自身の更新1
		if (IsUpdateActive()) {
			OnUpdate();
		}
		//配置オブジェクトのコンポーネント更新1
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->ComponentUpdate();
			}
		}
		//衝突判定の更新（ステージから呼ぶ）
		UpdateCollision();
		//衝突による重力の変化の更新
		UpdateCollisionGravity();
		//自身のビューをアップデート
		if (IsUpdateActive() && pImpl->m_ViewBase) {
			pImpl->m_ViewBase->OnUpdate();
		}
		//衝突判定のメッセージ発行（ステージから呼ぶ）
		UpdateMessageCollision();
		//配置オブジェクトのコンポーネント更新3
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->OnLastUpdate();
			}
		}
		//自身の更新3
		if (IsUpdateActive()) {
			OnLastUpdate();
		}

		//コリジョンのリセット
		for (auto ptr : GetGameObjectVec()) {
			ptr->CollisionReset();
		}


		//子供ステージの更新
		for (auto PtrChileStage : GetChileStageVec()) {
			PtrChileStage->UpdateStage();
		}
	}

	//衝突判定の更新（ステージから呼ぶ）
	void Stage::UpdateCollision() {
		//衝突判定チェック
		//配置オブジェクトの衝突チェック
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->CollisionChk();
			}
		}
	}

	void Stage::UpdateCollisionGravity() {
		//配置オブジェクトの衝突重力更新チェック
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->CollisionGravityChk();
			}
		}

	}

	void Stage::UpdateMessageCollision() {
		//配置オブジェクトの衝突メッセージ発行
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->ToMessageCollision();
			}
		}

	}



	void Stage::DrawShadowmapStage() {
		for (auto ptr : pImpl->m_GameObjectVec) {
			ptr->DrawShadowmap();
		}
	}


	//ステージ内の描画（シーンからよばれる）
	void Stage::DrawStage() {
		for (auto ptr : pImpl->m_GameObjectVec) {
			ptr->OnDraw();
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct SceneBase::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct SceneBase::Impl {
		shared_ptr<Stage> m_ActiveStage;	//アクティブなステージ
		Impl():
			m_ActiveStage()
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	シーン親クラス
	//--------------------------------------------------------------------------------------
	SceneBase::SceneBase() :
		SceneInterface(),
		pImpl(new Impl())
	{
		try {
			//デフォルトのリソースの作成
			App::GetApp()->RegisterResource(L"DEFAULT_SQUARE", MeshResource::CreateSquare(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_CUBE", MeshResource::CreateCube(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_SPHERE", MeshResource::CreateSphere(1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_CAPSULE", MeshResource::CreateCapsule(1.0f, 1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_CYLINDER", MeshResource::CreateCylinder(1.0f, 1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_CONE", MeshResource::CreateCone(1.0f, 1.0f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_TORUS", MeshResource::CreateTorus(1.0f, 0.3f, 18));
			App::GetApp()->RegisterResource(L"DEFAULT_TETRAHEDRON", MeshResource::CreateTetrahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_OCTAHEDRON", MeshResource::CreateOctahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_DODECAHEDRON", MeshResource::CreateDodecahedron(1.0f));
			App::GetApp()->RegisterResource(L"DEFAULT_ICOSAHEDRON", MeshResource::CreateIcosahedron(1.0f));
		}
		catch (...) {
			throw;
		}
	}
	SceneBase::~SceneBase() {}

	shared_ptr<Stage> SceneBase::GetActiveStage() const {
		if (!pImpl->m_ActiveStage) {
			//アクティブなステージが無効なら
			throw BaseException(
				L"アクティブなステージがありません",
				L"if(!m_ActiveStage.get())",
				L"SceneBase::GetActiveStage()"
			);
		}
		return pImpl->m_ActiveStage;
	}
	void SceneBase::SetActiveStage(const shared_ptr<Stage>& stage) {
		pImpl->m_ActiveStage = stage;
	}

	void SceneBase::OnUpdate() {
		if (pImpl->m_ActiveStage) {
			//ステージのアップデート
			pImpl->m_ActiveStage->UpdateStage();
		}
	}
	void SceneBase::OnDraw() {
		if (pImpl->m_ActiveStage) {
			//描画デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			Dev->ClearDefultViews(Color4(0, 0, 0, 1.0));
			Dev->ClearShadowmapViews();
			Dev->StartShadowmapDraw();
			pImpl->m_ActiveStage->DrawShadowmapStage();
			Dev->EndShadowmapDraw();
			//デフォルト描画の開始
			Dev->StartDefultDraw();
			pImpl->m_ActiveStage->DrawStage();
			//デフォルト描画の終了
			Dev->EndDefultDraw();


		}
	}









}
//end basecross
