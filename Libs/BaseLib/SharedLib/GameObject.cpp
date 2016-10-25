/*!
@file GameObject.cpp
@brief ゲームオブジェクト、ステージ実体
@copyright Copyright (c) 2016 WiZ Tamura Hiroki,Yamanoi Yasushi.
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
		auto CollisionPtr = GetComponent<Collision>(false);
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
			//GravityPtrがあればDraw()
			GravityPtr->OnDraw();
		}
		if (RigidPtr && RigidPtr->IsDrawActive()) {
			//RigidbodyがあればDraw()
			RigidPtr->OnDraw();
		}
		if (CollisionPtr && CollisionPtr->IsDrawActive()) {
			//CollisionがあればDraw()
			CollisionPtr->OnDraw();
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

	const shared_ptr<Camera>& GameObject::OnGetDrawCamera() const {
		//デフォルトはビューのカメラから取り出す
		auto StageView = GetStage()->GetView();
		return StageView->GetTargetCamera();
	}

	const Light& GameObject::OnGetDrawLight() const {
		//ステージからライトを取り出す
		auto StageLight = GetStage()->GetLight();
		return StageLight->GetTargetLight();
	}


	void GameObject::OnGet2DDrawProjMatrix(Matrix4X4& ProjMatrix) const {
		auto viewport = GetStage()->GetView()->GetTargetViewport();
		float w = static_cast<float>(viewport.Width);
		float h = static_cast<float>(viewport.Height);
		ProjMatrix.OrthographicLH(w, h, viewport.MinDepth, viewport.MaxDepth);
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
	//	struct Particle::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct Particle::Impl {
		DrawOption m_DrawOption;		//表示オプション
		vector<ParticleSprite> m_ParticleSpriteVec;	//保存しておくスプライトの配列
		Vector3 m_EmitterPos;			//エミッター位置
		float m_TotalTime;				//タイマー制御する場合に使用する変数
		float m_MaxTime;				//このパーティクル集合体の表示時間
		weak_ptr<TextureResource> m_TextureResource;	//テクスチャ
		Impl(size_t Count, DrawOption Option) :
			m_DrawOption(Option),
			m_ParticleSpriteVec(Count),
			m_EmitterPos(0, 0, 0),
			m_TotalTime(0),
			m_MaxTime(0)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class Particle;
	//	用途: パーティクル
	//	*1つのエミッターを持ち、複数のParticleSpriteを保持する
	//--------------------------------------------------------------------------------------
	Particle::Particle(size_t Count, DrawOption Option) :
		ObjectInterface(),
		pImpl(new Impl(Count, Option))
	{}
	Particle::~Particle() {}

	Particle::DrawOption Particle::GetDrawOption()const {
		return pImpl->m_DrawOption;
	}
	void Particle::SetDrawOption(DrawOption Option) {
		pImpl->m_DrawOption = Option;
	}

	const Vector3& Particle::GetEmitterPos() const {
		return pImpl->m_EmitterPos;
	}
	void Particle::SetEmitterPos(const Vector3& Pos) {
		pImpl->m_EmitterPos = Pos;
	}

	float Particle::GetTotalTime() const {
		return pImpl->m_TotalTime;
	}
	void Particle::SetTotalTime(float f) {
		pImpl->m_TotalTime = f;
	}
	void Particle::AddTotalTime(float f) {
		pImpl->m_TotalTime += f;
	}

	float Particle::GetMaxTime() const {
		return pImpl->m_MaxTime;
	}
	void Particle::SetMaxTime(float f) {
		pImpl->m_MaxTime = f;
	}



	bool Particle::IsActive() const {
		for (auto Psp : pImpl->m_ParticleSpriteVec) {
			if (Psp.m_Active) {
				//1つでもアクティブがあればtrue
				return true;
			}
		}
		return false;
	}
	bool Particle::IsAllActive() const {
		for (auto Psp : pImpl->m_ParticleSpriteVec) {
			if (!Psp.m_Active) {
				//1つでも非アクティブがあればfalse
				return false;
			}
		}
		return true;
	}
	void Particle::SetAllActive() {
		for (auto Psp : pImpl->m_ParticleSpriteVec) {
			Psp.m_Active = true;
		}
	}
	void Particle::SetAllNoActive() {
		for (auto Psp : pImpl->m_ParticleSpriteVec) {
			Psp.m_Active = false;
		}
	}

	void Particle::Reflesh(size_t Count, Particle::DrawOption Option) {
		pImpl->m_DrawOption = Option;
		pImpl->m_EmitterPos = Vector3(0, 0, 0);
		pImpl->m_TotalTime = 0;
		pImpl->m_MaxTime = 0;
		pImpl->m_ParticleSpriteVec.clear();
		pImpl->m_ParticleSpriteVec.resize(Count);
		for (auto Psp : pImpl->m_ParticleSpriteVec) {
			Psp.Reflesh();
		}
	}

	vector<ParticleSprite>& Particle::GetParticleSpriteVec() const {
		return pImpl->m_ParticleSpriteVec;
	}

	shared_ptr<TextureResource> Particle::GetTextureResource(bool ExceptionActive) const {
		if (!pImpl->m_TextureResource.expired()) {
			return pImpl->m_TextureResource.lock();
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"テクスチャリソースが見つかりません",
					L"if (pImpl->m_Texture.expired())",
					L"Particle::GetTextureResource()"
				);
			}
		}
		return nullptr;
	}
	void Particle::SetTextureResource(const wstring& ResKey) {
		try {
			if (ResKey == L"") {
				throw BaseException(
					L"テクスチャキーが空白です",
					L"if (ResKey == L\"\"",
					L"Particle::SetTextureResource()"
				);
			}
			pImpl->m_TextureResource = App::GetApp()->GetResource<TextureResource>(ResKey);
		}
		catch (...) {
			throw;
		}
	}
	void Particle::SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr) {
		pImpl->m_TextureResource = TextureResourcePtr;
	}

	void Particle::Draw(const shared_ptr<ParticleManager>& Manager) {
		for (auto Psp : pImpl->m_ParticleSpriteVec) {
			if (Psp.m_Active && !pImpl->m_TextureResource.expired()) {
				Manager->AddParticle(Psp, GetDrawOption(),
					GetEmitterPos(), pImpl->m_TextureResource.lock());
			}
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct MultiParticle::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct MultiParticle::Impl {
		vector< shared_ptr<Particle> > m_ParticleVec;
		Impl()
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class MultiParticle : public GameObject;
	//	用途: マルチエフェクト 
	//--------------------------------------------------------------------------------------
	MultiParticle::MultiParticle(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		pImpl(new Impl())
	{}
	MultiParticle::~MultiParticle() {}

	vector< shared_ptr<Particle> >& MultiParticle::GetParticleVec() const {
		return pImpl->m_ParticleVec;
	}


	shared_ptr<Particle> MultiParticle::InsertParticle(size_t Count, Particle::DrawOption Option) {
		for (size_t i = 0; i < pImpl->m_ParticleVec.size(); i++) {
			//もし非アクティブのパーティクルがあれば初期化してリターン
			if (!pImpl->m_ParticleVec[i]->IsActive()) {
				pImpl->m_ParticleVec[i]->Reflesh(Count, Option);
				return pImpl->m_ParticleVec[i];
			}
		}
		//新しいパーティクルを追加
		shared_ptr<Particle> ParticlePtr = ObjectFactory::Create<Particle>(Count, Option);
		pImpl->m_ParticleVec.push_back(ParticlePtr);
		return ParticlePtr;
	}

	void MultiParticle::OnPreCreate() {
		GameObject::OnPreCreate();
		//透明処理のみ指定しておく
		SetAlphaActive(true);
	}


	void MultiParticle::OnUpdate() {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto ParticlePtr : GetParticleVec()) {
			ParticlePtr->AddTotalTime(ElapsedTime);
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					//移動速度に従って移動させる
					rParticleSprite.m_LocalPos += rParticleSprite.m_Velocity * ElapsedTime;
					if (ParticlePtr->GetTotalTime() >= ParticlePtr->GetMaxTime()) {
						//制限時間になったら
						rParticleSprite.m_Active = false;
					}
				}
			}
		}
	}


	void MultiParticle::OnDraw() {
		if (pImpl->m_ParticleVec.size() > 0) {
			for (auto Ptr : pImpl->m_ParticleVec) {
				if (Ptr->IsActive()) {
					Ptr->Draw(GetStage()->GetParticleManager());
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct ParticleManager::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct ParticleManager::Impl {
		bool m_ZBufferUse;				//Zバッファを使用するかどうか
		bool m_SamplerWrap;				//サンプラーのラッピングするかどうか
		Impl() :
			m_ZBufferUse{ true },
			m_SamplerWrap{ false }
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class ParticleManager : public GameObject;
	//	用途: パーティクルマネージャ
	//--------------------------------------------------------------------------------------
	//構築と消滅
	ParticleManager::ParticleManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		pImpl(new Impl())
	{}
	ParticleManager::~ParticleManager() {}

	//初期化
	void ParticleManager::OnCreate() {
		try {
			//上限2000でマネージャ作成
			AddComponent<PCTParticleDraw>(2000);
			//透明処理のみ指定しておく
			SetAlphaActive(true);
		}
		catch (...) {
			throw;
		}

	}

	bool ParticleManager::GetZBufferUse() const {
		return pImpl->m_ZBufferUse;
	}
	bool ParticleManager::IsZBufferUse() const {
		return pImpl->m_ZBufferUse;
	}
	void ParticleManager::SetZBufferUse(bool b) {
		pImpl->m_ZBufferUse = b;
	}

	bool ParticleManager::GetSamplerWrap() const {
		return pImpl->m_SamplerWrap;
	}
	bool ParticleManager::IsSamplerWrap() const {
		return pImpl->m_SamplerWrap;
	}
	void ParticleManager::SetSamplerWrap(bool b) {
		pImpl->m_SamplerWrap = b;
	}

	void ParticleManager::AddParticle(const ParticleSprite& rParticleSprite, Particle::DrawOption Option,
		const Vector3& EmitterPos, const shared_ptr<TextureResource>& TextureRes) {
		auto DrawCom = GetComponent<PCTParticleDraw>();

		auto StageView = GetStage()->GetView();

		auto PtrCamera = StageView->GetTargetCamera();
		//カメラの位置
		Vector3 CameraEye = PtrCamera->GetEye();
		Vector3 CameraAt = PtrCamera->GetAt();


		Vector3 WorldPos = rParticleSprite.m_LocalPos + EmitterPos;
		float ToCaneraLength = Vector3EX::Length(CameraEye - WorldPos);

		Vector3 LocalScale;
		LocalScale.x = rParticleSprite.m_LocalScale.x;
		LocalScale.y = rParticleSprite.m_LocalScale.y;
		LocalScale.z = 1.0f;

		Vector3 Temp;
		Quaternion Qt;
		Matrix4X4 RotMatrix;
		Vector4 dammi(0, 0, 0, 0);
		Vector3 DefUp(0, 1.0f, 0);
		switch (Option) {
		case Particle::DrawOption::Billboard:
		{
			Temp = CameraAt - CameraEye;
			Vector2 TempVec2(Temp.x, Temp.z);
			if (Vector2EX::Length(TempVec2) < 0.1f) {
				DefUp = Vector3(0, 0, 1.0f);
			}
			Temp.Normalize();
			RotMatrix.LookAtLH(Vector3(0, 0, 0), Temp, DefUp);
			RotMatrix.Inverse(&dammi);
			Qt = RotMatrix.QtInMatrix();
			Qt.Normalize();
		}
		break;
		case Particle::DrawOption::Faceing:
		{
			Temp = WorldPos - CameraEye;
			Vector2 TempVec2(Temp.x, Temp.z);
			if (Vector2EX::Length(TempVec2) < 0.1f) {
				DefUp = Vector3(0, 0, 1.0f);
			}
			RotMatrix.LookAtLH(Vector3(0, 0, 0), Temp, DefUp);
			RotMatrix.Inverse(&dammi);
			Qt = RotMatrix.QtInMatrix();
			Qt.Normalize();
		}
		break;
		case Particle::DrawOption::FaceingY:
			Temp = WorldPos - CameraEye;
			Temp.Normalize();
			Qt.RotationRollPitchYaw(0, atan2(Temp.x, Temp.z), 0);
			Qt.Normalize();
			break;
		case Particle::DrawOption::Normal:
			Qt = rParticleSprite.m_LocalQt;
			Qt.Normalize();
			break;
		}

		Matrix4X4 matrix;
		matrix.DefTransformation(
			LocalScale,
			Qt,
			WorldPos
		);
		DrawCom->AddParticle(ToCaneraLength, matrix, TextureRes);
	}

	void ParticleManager::OnDraw() {
		auto DrawCom = GetComponent<PCTParticleDraw>();
		DrawCom->OnDraw();
	}




	//--------------------------------------------------------------------------------------
	//	struct Stage::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct Stage::Impl {
		//updateするかどうか
		bool m_UpdateActive;
		//パーティクルマネージャ
		shared_ptr<ParticleManager> m_ParticleManager;
		//オブジェクトの配列
		vector< shared_ptr<GameObject> > m_GameObjectVec;
		//途中にオブジェクトが追加された場合、ターンの開始まで待つ配列
		vector< shared_ptr<GameObject> > m_WaitAddObjectVec;

		//Spriteかそうでないかを分離する配列
		vector< shared_ptr<GameObject> > m_SpriteVec;
		vector< shared_ptr<GameObject> > m_Object3DVec;
		//3Dの透明と非透明を分離する配列
		vector< shared_ptr<GameObject> > m_Object3DNormalVec;
		vector< shared_ptr<GameObject> > m_Object3DAlphaVec;


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

	shared_ptr<ParticleManager> Stage::GetParticleManager() const { return pImpl->m_ParticleManager; }

	vector< shared_ptr<GameObject> >& Stage::GetGameObjectVec() { return pImpl->m_GameObjectVec; }

	//追加待ちになってるオブジェクトを追加する
	void Stage::SetWaitToObjectVec(){
		if (pImpl->m_WaitAddObjectVec.size() > 0){
			for (auto Ptr : pImpl->m_WaitAddObjectVec){
				pImpl->m_GameObjectVec.push_back(Ptr);
			}
		}
		pImpl->m_WaitAddObjectVec.clear();
	}



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

	void Stage::OnPreCreate() {
		//パーティクルマネージャの作成
		pImpl->m_ParticleManager = ObjectFactory::Create<ParticleManager>(GetThis<Stage>());
	}


	//ステージ内の更新（シーンからよばれる）
	void Stage::UpdateStage() {
		//追加まちオブジェクトの追加
		SetWaitToObjectVec();
		//Transformコンポーネントの値をバックアップにコピー
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				auto ptr2 = ptr->GetComponent<Transform>();
				ptr2->SetToBefore();
			}
		}
		//配置オブジェクトのコンポーネント更新1
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsUpdateActive()) {
				ptr->ComponentUpdate();
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
		//衝突判定の更新（ステージから呼ぶ）
		UpdateCollision();
		//衝突による重力の変化の更新
		UpdateCollisionGravity();
		//衝突判定のメッセージ発行（ステージから呼ぶ）
		UpdateMessageCollision();
		//自身のビューをアップデート
		if (IsUpdateActive() && pImpl->m_ViewBase) {
			pImpl->m_ViewBase->OnUpdate();
		}
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
		//レイヤーの取得と設定
		set<int> DrawLayers;
		//Spriteかそうでないかを分離
		for (auto ptr : GetGameObjectVec()) {
			if (ptr->IsDrawActive()) {
				//描画レイヤーに登録
				DrawLayers.insert(ptr->GetDrawLayer());
				//Spriteかその派生クラスなら分離
				if (ptr->GetDynamicComponent<SpriteBaseDraw>(false) || ptr->IsSpriteDraw()) {
					pImpl->m_SpriteVec.push_back(ptr);
				}
				else {
					pImpl->m_Object3DVec.push_back(ptr);
				}
			}
		}
		//3Dの透明と非透明を分離
		for (auto ptr : pImpl->m_Object3DVec) {
			if (ptr->IsDrawActive()) {
				if (ptr->IsAlphaActive()) {
					pImpl->m_Object3DAlphaVec.push_back(ptr);
				}
				else {
					pImpl->m_Object3DNormalVec.push_back(ptr);
				}
			}
		}

		auto PtrCamera = pImpl->m_ViewBase->GetTargetCamera();
		//カメラの位置
		Vector3 CameraEye = PtrCamera->GetEye();
		//透明の3Dオブジェクトをカメラからの距離でソート
		//以下は、オブジェクトを引数に取りboolを返すラムダ式
		//--------------------------------------------------------
		auto func = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool {
			auto PtrLeftTrans = Left->GetComponent<Transform>();
			auto PtrRightTrans = Right->GetComponent<Transform>();

			auto LeftPos = PtrLeftTrans->GetWorldMatrix().PosInMatrixSt();
			auto RightPos = PtrRightTrans->GetWorldMatrix().PosInMatrixSt();

			auto LeftLen = Vector3EX::Length(LeftPos - CameraEye);
			auto RightLen = Vector3EX::Length(RightPos - CameraEye);

			return (LeftLen > RightLen);
		};

		std::sort(pImpl->m_Object3DAlphaVec.begin(), pImpl->m_Object3DAlphaVec.end(), func);

		//3Dノーマルオブジェクトの描画準備
		for (auto ptr : pImpl->m_Object3DNormalVec) {
			ptr->OnPreDraw();
		}
		//3D透明オブジェクトの描画準備
		for (auto ptr : pImpl->m_Object3DAlphaVec) {
			ptr->OnPreDraw();
		}
		//パーティクルの描画準備
		if (GetParticleManager()) {
			GetParticleManager()->OnPreDraw();
		}
		//スプライトオブジェクトの描画準備
		for (auto ptr : pImpl->m_SpriteVec) {
			ptr->OnPreDraw();
		}

		//--------------------------------------------------------
		//スプライトをZ座標距離でソート
		//以下は、オブジェクトを引数に取りboolを返すラムダ式
		//--------------------------------------------------------
		auto funcSprite = [&](shared_ptr<GameObject>& Left, shared_ptr<GameObject>& Right)->bool {
			auto PtrLeftTrans = Left->GetComponent<Transform>();
			auto PtrRightTrans = Right->GetComponent<Transform>();

			auto LeftPos = PtrLeftTrans->GetWorldMatrix().PosInMatrixSt();
			auto RightPos = PtrRightTrans->GetWorldMatrix().PosInMatrixSt();

			float LeftZ = LeftPos.z;
			float RightZ = RightPos.z;
			return (LeftZ > RightZ);
		};
		std::sort(pImpl->m_SpriteVec.begin(), pImpl->m_SpriteVec.end(), funcSprite);

		for (auto it = DrawLayers.begin(); it != DrawLayers.end(); it++) {
			int Tgt = *it;
			//3Dノーマルオブジェクトの描画
			for (auto ptr : pImpl->m_Object3DNormalVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}
			//3D透明オブジェクトの描画
			for (auto ptr : pImpl->m_Object3DAlphaVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}
			//パーティクルの描画
			//パーティクルマネージャは描画レイヤーごとに初期化されるので
			//毎レイヤー描画する
			auto PartPtr = GetParticleManager();
			if (PartPtr) {
				PartPtr->OnDraw();
			}
			//スプライトオブジェクトの描画
			for (auto ptr : pImpl->m_SpriteVec) {
				if (ptr->GetDrawLayer() == Tgt) {
					ptr->OnDraw();
				}
			}
		}
		//ステージのDraw();
		OnDraw();
		//ワーク用配列のクリア
		//ワーク配列は毎ターンごとに初期化されるが、
		//最大値は減らないので2回目のターン以降は高速に動作する
		pImpl->m_Object3DVec.clear();
		pImpl->m_SpriteVec.clear();
		pImpl->m_Object3DNormalVec.clear();
		pImpl->m_Object3DAlphaVec.clear();
		//子供ステージの描画
		for (auto PtrChileStage : pImpl->m_ChildStageVec) {
			PtrChileStage->DrawStage();
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

			vector<VertexPositionNormalTexture> vertices;
			vector<VertexPositionColor> new_vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateCube(1.0f, vertices, indices);
			for (size_t i = 0; i < vertices.size(); i++) {
				VertexPositionColor new_v;
				new_v.position = vertices[i].position;
				new_v.color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
				new_vertices.push_back(new_v);
			}
			App::GetApp()->RegisterResource(L"DEFAULT_PC_CUBE", MeshResource::CreateMeshResource(new_vertices, indices,false));
			vertices.clear();
			new_vertices.clear();
			indices.clear();
			MeshUtill::CreateSphere(1.0f,18, vertices, indices);
			for (size_t i = 0; i < vertices.size(); i++) {
				VertexPositionColor new_v;
				new_v.position = vertices[i].position;
				new_v.color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
				new_vertices.push_back(new_v);
			}
			App::GetApp()->RegisterResource(L"DEFAULT_PC_SPHERE", MeshResource::CreateMeshResource(new_vertices, indices, false));


			

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
