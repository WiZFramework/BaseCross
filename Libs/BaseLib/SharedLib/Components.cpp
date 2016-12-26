/*!
@file Components.cpp
@brief コンポーネント実体
@copyright Copyright (c) 2016 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct Component::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Component::Impl {
		weak_ptr<GameObject> m_GameObject;
		bool m_UpdateActive{ true };	//updateするかどうか
		bool m_DrawActive{ true };		//Drawするかどうか
		explicit Impl(const shared_ptr<GameObject>& GameObjectPtr) :
			m_GameObject(GameObjectPtr)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class Component :public Object;
	//	用途: コンポーネント基底クラス
	//--------------------------------------------------------------------------------------
	Component::Component(const shared_ptr<GameObject>& GameObjectPtr) :
		ObjectInterface(),
		ShapeInterface(),
		pImpl(new Impl(GameObjectPtr))
	{}
	Component::~Component() {}
	//アクセサ
	shared_ptr<GameObject> Component::GetGameObject() const {
		auto shptr = pImpl->m_GameObject.lock();
		if (!shptr) {
			throw BaseException(
				L"GameObjectは有効ではありません",
				L"if (!shptr)",
				L"Component::GetGameObject()"
			);
		}
		else {
			return shptr;
		}
	}

	void Component::AttachGameObject(const shared_ptr<GameObject>& GameObjectPtr) {
		pImpl->m_GameObject = GameObjectPtr;
	}
	shared_ptr<Stage> Component::GetStage() const {
		return GetGameObject()->GetStage();
	}


	bool Component::IsUpdateActive() const {
		return pImpl->m_UpdateActive;
	}
	bool Component::GetUpdateActive() const {
		return pImpl->m_UpdateActive;
	}
	void Component::SetUpdateActive(bool b) {
		pImpl->m_UpdateActive = b;
	}

	bool Component::IsDrawActive() const {
		return pImpl->m_DrawActive;
	}
	bool Component::GetDrawActive() const {
		return pImpl->m_DrawActive;
	}
	void Component::SetDrawActive(bool b) {
		pImpl->m_DrawActive = b;
	}


	//--------------------------------------------------------------------------------------
	//	struct Transform::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct Transform::Impl {
		bool m_Init{ false };	//初期化済みかどうか（1回目のUpdateで、Beforeに値を入れる）
		//1つ前の変数
		Vector3 m_BeforeScale;
		Vector3 m_BeforePivot;
		Quaternion m_BeforeQuaternion;
		Vector3 m_BeforePosition;
		//再計算抑制用変数
		bool m_BeforeChangeed;
		Matrix4X4 m_BeforeWorldMatrix;
		//現在の変数
		Vector3 m_Scale;
		Vector3 m_Pivot;
		Quaternion m_Quaternion;
		Vector3 m_Position;
		//再計算抑制用変数
		bool m_Changeed;
		Matrix4X4 m_WorldMatrix;

		Impl():
			//スケールのみ初期化（他はデフォルト処理でよい）
			m_BeforeScale(1.0f,1.0f,1.0f),
			m_Scale(1.0f, 1.0f, 1.0f),
			m_BeforeChangeed(true),
			m_Changeed(true)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class Transform : public Component ;
	//	用途: 変化
	//--------------------------------------------------------------------------------------
	Transform::Transform(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr), pImpl(new Impl())
	{}
	Transform::~Transform() {}

	//アクセサ
	//BeforeGetter
	Vector3 Transform::GetBeforeScale() const {
		return pImpl->m_BeforeScale;
	}

	Vector3 Transform::GetBeforePivot() const{
		return pImpl->m_BeforePivot;
	}

	Quaternion Transform::GetBeforeQuaternion() const {
		return pImpl->m_BeforeQuaternion;
	}

	Vector3 Transform::GetBeforeRotation() const {
		return pImpl->m_BeforeQuaternion.GetRotation();
	}

	Vector3 Transform::GetBeforePosition() const {
		return pImpl->m_BeforePosition;
	}

	bool Transform::IsSameBeforeWorldMatrix(const Matrix4X4& mat) const {
		return mat.EqualInt(GetBeforeWorldMatrix());
	}


	const Matrix4X4& Transform::GetBeforeWorldMatrix() const{
		if (pImpl->m_BeforeChangeed) {
			pImpl->m_BeforeWorldMatrix.AffineTransformation(
				pImpl->m_BeforeScale,
				pImpl->m_BeforePivot,
				pImpl->m_BeforeQuaternion,
				pImpl->m_BeforePosition
			);
			pImpl->m_BeforeChangeed = false;
		}
		return pImpl->m_BeforeWorldMatrix;
	}


	//Getter&Setter

	Vector3 Transform::GetScale() const {
		return pImpl->m_Scale;
	}

	void Transform::SetScale(const Vector3& Scale) {
		pImpl->m_Changeed = true;
		pImpl->m_Scale = Scale;
	}
	void Transform::SetScale(float x, float y, float z) {
		SetScale(Vector3(x, y, z));
	}

	Vector3 Transform::GetPivot() const {
		return pImpl->m_Pivot;
	}
	void Transform::SetPivot(const Vector3& Pivot) {
		pImpl->m_Changeed = true;
		pImpl->m_Pivot = Pivot;
	}
	void Transform::SetPivot(float x, float y, float z) {
		SetPivot(Vector3(x, y, z));
	}

	Quaternion Transform::GetQuaternion() const {
		return pImpl->m_Quaternion;
	}
	void Transform::SetQuaternion(const Quaternion& quaternion) {
		pImpl->m_Changeed = true;
		pImpl->m_Quaternion = quaternion;
		pImpl->m_Quaternion.Normalize();
	}
	Vector3 Transform::GetRotation() const {
		return pImpl->m_Quaternion.GetRotation();
	}

	void Transform::SetRotation(const Vector3& Rot) {
		pImpl->m_Changeed = true;
		Quaternion Qt;
		Qt.RotationRollPitchYawFromVector(Rot);
		SetQuaternion(Qt);
	}
	void Transform::SetRotation(float x, float y, float z) {
		SetRotation(Vector3(x, y, z));
	}

	Vector3 Transform::GetPosition() const {
		return pImpl->m_Position;
	}

	void Transform::SetPosition(const Vector3& Position) {
		pImpl->m_Changeed = true;
		pImpl->m_Position = Position;
	}
	void Transform::SetPosition(float x, float y, float z) {
		SetPosition(Vector3(x, y, z));
	}

	void Transform::ResetPosition(const Vector3& Position) {
		pImpl->m_BeforeChangeed = true;
		pImpl->m_BeforePosition = Position;
		pImpl->m_Changeed = true;
		pImpl->m_Position = Position;
	}


	bool Transform::IsSameWorldMatrix(const Matrix4X4& mat) const {
		return mat.EqualInt(GetWorldMatrix());
	}


	const Matrix4X4& Transform::GetWorldMatrix() const{
		if (pImpl->m_Changeed) {
			pImpl->m_WorldMatrix.AffineTransformation(
				pImpl->m_Scale,
				pImpl->m_Pivot,
				pImpl->m_Quaternion,
				pImpl->m_Position
			);
			pImpl->m_Changeed = false;
		}
		return pImpl->m_WorldMatrix;
	}

	Vector3 Transform::GetVelocity() const {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vector3 Velocity = pImpl->m_Position - pImpl->m_BeforePosition;
		Velocity /= ElapsedTime;
		return Velocity;
	}


	void Transform::SetToBefore() {
		if (pImpl->m_BeforeScale != pImpl->m_Scale) {
			pImpl->m_BeforeChangeed = true;
			pImpl->m_BeforeScale = pImpl->m_Scale;
		}
		if (pImpl->m_BeforePivot != pImpl->m_Pivot) {
			pImpl->m_BeforeChangeed = true;
			pImpl->m_BeforePivot = pImpl->m_Pivot;
		}
		if (pImpl->m_BeforeQuaternion != pImpl->m_Quaternion) {
			pImpl->m_BeforeChangeed = true;
			pImpl->m_BeforeQuaternion = pImpl->m_Quaternion;
		}
		if (pImpl->m_BeforePosition != pImpl->m_Position) {
			pImpl->m_BeforeChangeed = true;
			pImpl->m_BeforePosition = pImpl->m_Position;
		}
	}

	//操作
	void Transform::OnUpdate() {
		if (!pImpl->m_Init) {
			SetToBefore();
			pImpl->m_Init = true;
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct PlayMusic::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct PlayMusic::Impl {
		weak_ptr<AudioResource> m_AudioResource;
		IXAudio2SourceVoice* m_pSourceVoice;
		//リソースキー
		wstring m_ResKey;
		Impl(const wstring& ResKey) :
			m_ResKey(ResKey),
			m_pSourceVoice(nullptr)
		{}
		~Impl() {
		}
	};

	//--------------------------------------------------------------------------------------
	//	class PlayMusic: public Component;
	//	用途: ミュージックコンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	PlayMusic::PlayMusic(const shared_ptr<GameObject>& GameObjectPtr, const wstring& ResKey) :
		Component(GameObjectPtr),
		pImpl(new Impl(ResKey))
	{
		try {
			auto MusicRes = App::GetApp()->GetResource<AudioResource>(ResKey);
			pImpl->m_AudioResource = MusicRes;
			auto Engine = App::GetApp()->GetAudioManager()->GetMusicEngine();

			ThrowIfFailed(
				Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, MusicRes->GetOutputWaveFormatEx()),
				L"ミュージック用サウンドボイスの作成に失敗しました",
				L"Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, MusicRes->GetOutputWaveFormatEx())",
				L"PlayMusic::PlayMusic()"
			);
		}
		catch (...) {
			throw;
		}
	}
	PlayMusic::~PlayMusic() {
	}

	shared_ptr<AudioResource> PlayMusic::GetAudioResource() const {
		if (pImpl->m_AudioResource.expired()) {
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"PlayMusic::GetAudioResource()"
			);
		}
		return pImpl->m_AudioResource.lock();
	}


	void PlayMusic::Start(size_t LoopCount, float Volume) {
		if (!pImpl->m_pSourceVoice) {
			return;
		}
		if (pImpl->m_AudioResource.expired()) {
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"PlayMusic::Start()"
			);
		}
		XAUDIO2_BUFFER buffer = { 0 };
		auto ResPtr = pImpl->m_AudioResource.lock();
		buffer.AudioBytes = ResPtr->GetSoundData().size();
		buffer.LoopCount = LoopCount;
		buffer.pAudioData = &ResPtr->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		//多重定義関数を呼ぶ
		Start(buffer, Volume);
	}

	void PlayMusic::Start(const XAUDIO2_BUFFER& Buffer, float Volume) {
		if (!pImpl->m_pSourceVoice) {
			return;
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->Stop(),
			L"ミュージック用サウンドボイスの停止に失敗しました",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Start()"
		);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"ミュージック用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Start()"
		);
		if (pImpl->m_AudioResource.expired()) {
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"PlayMusic::Start()"
			);
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->SetVolume(Volume),
			L"ミュージックのボリューム設定に失敗しました",
			L"pImpl->m_pSourceVoice->SetVolume()",
			L"PlayMusic::Start()"
		);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"ミュージックのソースバッファ設定に失敗しました",
			L"pImpl->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"PlayMusic::Start()"
		);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->Start(),
			L"ミュージックサウンドのスタートに失敗しました",
			L"pImpl->m_pSourceVoice->Start()",
			L"PlayMusic::Start()"
		);

	}


	void PlayMusic::Stop() {
		if (!pImpl->m_pSourceVoice) {
			return;
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->Stop(),
			L"ミュージック用サウンドボイスの停止に失敗しました",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Stop()"
		);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"ミュージック用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Stop()"
		);
	}





	//--------------------------------------------------------------------------------------
	//	struct SoundEffect::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct SoundEffect::Impl {
		weak_ptr<AudioResource> m_AudioResource;
		IXAudio2SourceVoice* m_pSourceVoice;
		//リソースキー
		wstring m_ResKey;
		Impl(const wstring& ResKey) :
			m_ResKey(ResKey),
			m_pSourceVoice(nullptr)
		{}
		~Impl() {
		}
	};



	//--------------------------------------------------------------------------------------
	//	class SoundEffect: public Component;
	//	用途: サウンドエフェクトコンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SoundEffect::SoundEffect(const shared_ptr<GameObject>& GameObjectPtr, const wstring& ResKey) :
		Component(GameObjectPtr),
		pImpl(new Impl(ResKey))
	{
		try {
			auto SoundRes = App::GetApp()->GetResource<AudioResource>(ResKey);
			pImpl->m_AudioResource = SoundRes;
			auto Engine = App::GetApp()->GetAudioManager()->GetSoundEffectEngine();

			ThrowIfFailed(
				Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, SoundRes->GetOutputWaveFormatEx()),
				L"サウンドエフェクト用サウンドボイスの作成に失敗しました",
				L"Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, SountRes->GetOutputWaveFormatEx())",
				L"SoundEffect::SoundEffect()"
			);
		}
		catch (...) {
			throw;
		}
	}
	SoundEffect::~SoundEffect() {
	}

	shared_ptr<AudioResource> SoundEffect::GetAudioResource() const {
		if (pImpl->m_AudioResource.expired()) {
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"SoundEffect::GetAudioResource()"
			);
		}
		return pImpl->m_AudioResource.lock();
	}


	void SoundEffect::Start(size_t LoopCount, float Volume) {
		if (!pImpl->m_pSourceVoice) {
			return;
		}
		if (pImpl->m_AudioResource.expired()) {
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"SoundEffect::Start()"
			);
		}
		XAUDIO2_BUFFER buffer = { 0 };
		auto ResPtr = pImpl->m_AudioResource.lock();
		buffer.AudioBytes = ResPtr->GetSoundData().size();
		buffer.LoopCount = LoopCount;
		buffer.pAudioData = &ResPtr->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		//多重定義関数を呼ぶ
		Start(buffer, Volume);
	}

	void SoundEffect::Start(const XAUDIO2_BUFFER& Buffer, float Volume) {
		if (!pImpl->m_pSourceVoice) {
			return;
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->Stop(),
			L"サウンドエフェクト用サウンドボイスの停止に失敗しました",
			L"pImpl->m_pSourceVoice->Stop()",
			L"SoundEffect::Start()"
		);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"サウンドエフェクト用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pImpl->m_pSourceVoice->FlushSourceBuffers()",
			L"SoundEffect::Start()"
		);
		if (pImpl->m_AudioResource.expired()) {
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"SoundEffect::Start()"
			);
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->SetVolume(Volume),
			L"サウンドエフェクト用サウンドのボリューム設定に失敗しました",
			L"pImpl->m_pSourceVoice->SetVolume()",
			L"SoundEffect::Start()"
		);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"サウンドエフェクト用サウンドのソースバッファ設定に失敗しました",
			L"pImpl->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"SoundEffect::Start()"
		);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->Start(),
			L"サウンドエフェクト用サウンドのスタートに失敗しました",
			L"pImpl->m_pSourceVoice->Start()",
			L"SoundEffect::Start()"
		);

	}


	void SoundEffect::Stop() {
		if (!pImpl->m_pSourceVoice) {
			return;
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->Stop(),
			L"サウンドエフェクト用サウンドボイスの停止に失敗しました",
			L"pImpl->m_pSourceVoice->Stop()",
			L"SoundEffect::Stop()"
		);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"サウンドエフェクト用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pImpl->m_pSourceVoice->FlushSourceBuffers()",
			L"SoundEffect::Stop()"
		);
	}




	//--------------------------------------------------------------------------------------
	//	struct MultiSoundEffect::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct MultiSoundEffect::Impl {
		map<wstring, SoundItem> m_SoundMap;
		Impl()
		{}
		~Impl() {
		}
		SoundItem* GetItem(const wstring& ResKey) {
			auto it = m_SoundMap.find(ResKey);
			if (it != m_SoundMap.end()) {
				//同じ名前が見つかった
				return &it->second;
			}
			return nullptr;
		}
		void RemoveItem(const wstring& ResKey) {
			auto it = m_SoundMap.find(ResKey);
			if (it != m_SoundMap.end()) {
				//同じ名前が見つかった
				//Mapから削除
				m_SoundMap.erase(it);
			}
		}
		void Start(const SoundItem* pItem, const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Stop(const SoundItem* pItem);

	};

	void MultiSoundEffect::Impl::Start(const SoundItem* pItem, const XAUDIO2_BUFFER& Buffer, float Volume) {
		if (!pItem->m_pSourceVoice) {
			throw BaseException(
				L"サウンドボイスが不定です",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiSoundEffect::Impl::Start()"
			);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->Stop(),
			L"サウンドエフェクト用サウンドボイスの停止に失敗しました",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiSoundEffect::Impl::Start()"
		);
		ThrowIfFailed(
			pItem->m_pSourceVoice->FlushSourceBuffers(),
			L"サウンドエフェクト用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiSoundEffect::Impl::Start()"
		);
		if (pItem->m_AudioResource.expired()) {
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"MultiSoundEffect::Impl::Start()"
			);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->SetVolume(Volume),
			L"サウンドエフェクト用サウンドのボリューム設定に失敗しました",
			L"pItem->m_pSourceVoice->SetVolume()",
			L"MultiSoundEffect::Impl::Start()"
		);

		ThrowIfFailed(
			pItem->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"サウンドエフェクト用サウンドのソースバッファ設定に失敗しました",
			L"pItem->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"MultiSoundEffect::Impl::Start()"
		);

		ThrowIfFailed(
			pItem->m_pSourceVoice->Start(),
			L"サウンドエフェクト用サウンドのスタートに失敗しました",
			L"pItem->m_pSourceVoice->Start()",
			L"MultiSoundEffect::Impl::Start()"
		);
	}

	void MultiSoundEffect::Impl::Stop(const SoundItem* pItem) {
		if (!pItem->m_pSourceVoice) {
			throw BaseException(
				L"サウンドボイスが不定です",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiSoundEffect::Impl::Stop()"
			);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->Stop(),
			L"サウンドエフェクト用サウンドボイスの停止に失敗しました",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiSoundEffect::Impl::Stop()"
		);
		ThrowIfFailed(
			pItem->m_pSourceVoice->FlushSourceBuffers(),
			L"サウンドエフェクト用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiSoundEffect::Impl::Stop()"
		);
	}



	//--------------------------------------------------------------------------------------
	//	class MultiSoundEffect: public Component;
	//	用途: マルチサウンドエフェクトコンポーネント
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiSoundEffect::MultiSoundEffect(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	MultiSoundEffect::~MultiSoundEffect() {}

	//アクセサ
	shared_ptr<AudioResource> MultiSoundEffect::AddAudioResource(const wstring& ResKey) {
		try {
			if (ResKey == L"") {
				throw BaseException(
					L"オーディオリソース名が空白です",
					L"if (ResKey == L\"\")",
					L"MultiSoundEffect::AddAudioResource()"
				);
			}
			auto pItem = pImpl->GetItem(ResKey);
			if (pItem) {
				//同じ名前が見つかった
				if (!pItem->m_AudioResource.expired()) {
					return pItem->m_AudioResource.lock();
				}
				else {
					throw BaseException(
						L"同じ名前のオーディオリソースがありましたが、リソースが無効です",
						ResKey,
						L"MultiSoundEffect::AddAudioResource()"
					);
				}
			}
			auto SoundRes = App::GetApp()->GetResource<AudioResource>(ResKey);
			SoundItem Item;
			Item.m_AudioResource = SoundRes;
			auto Engine = App::GetApp()->GetAudioManager()->GetSoundEffectEngine();
			ThrowIfFailed(
				Engine->CreateSourceVoice(&Item.m_pSourceVoice, SoundRes->GetOutputWaveFormatEx()),
				L"サウンドエフェクト用サウンドボイスの作成に失敗しました",
				L"Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, SountRes->GetOutputWaveFormatEx())",
				L"MultiSoundEffect::AddAudioResource()"
			);
			pImpl->m_SoundMap[ResKey] = Item;
			return SoundRes;
		}
		catch (...) {
			throw;
		}
	}
	void MultiSoundEffect::RemoveAudioResource(const wstring& ResKey) {
		if (ResKey == L"") {
			throw BaseException(
				L"オーディオリソース名が空白です",
				L"if (ResKey == L\"\")",
				L"MultiSoundEffect::RemoveAudioResource()"
			);
		}
		pImpl->RemoveItem(ResKey);
	}


	shared_ptr<AudioResource> MultiSoundEffect::GetAudioResource(const wstring& ResKey, bool ExceptionActive) const {
		if (ResKey == L"") {
			if (ExceptionActive) {
				throw BaseException(
					L"オーディオリソース名が空白です",
					L"if (ResKey == L\"\")",
					L"MultiSoundEffect::GetAudioResource()"
				);
			}
			else {
				return nullptr;
			}
		}
		auto pItem = pImpl->GetItem(ResKey);
		if (pItem) {
			//同じ名前が見つかった
			if (!pItem->m_AudioResource.expired()) {
				return pItem->m_AudioResource.lock();
			}
			else {
				if (ExceptionActive) {
					throw BaseException(
						L"同じ名前のオーディオリソースがありましたが、リソースが無効です",
						ResKey,
						L"MultiSoundEffect::AddAudioResource()"
					);
				}
				else {
					return nullptr;
				}
			}
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"指定の名前のオーディオリソースがありません",
					ResKey,
					L"MultiSoundEffect::GetAudioResource()"
				);
			}
			else {
				return nullptr;
			}
		}
	}

	void MultiSoundEffect::Start(const wstring& ResKey, const XAUDIO2_BUFFER& Buffer, float Volume) {
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem) {
			throw BaseException(
				L"指定の名前のオーディオリソースがありません",
				ResKey,
				L"MultiSoundEffect::Start()"
			);
		}
		pImpl->Start(pItem, Buffer, Volume);
	}

	void MultiSoundEffect::Start(const wstring& ResKey, size_t LoopCount, float Volume) {
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem) {
			throw BaseException(
				L"指定の名前のオーディオリソースがありません",
				ResKey,
				L"MultiSoundEffect::Start()"
			);
		}
		if (pItem->m_AudioResource.expired()) {
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				ResKey,
				L"MultiSoundEffect::Start()"
			);
		}
		XAUDIO2_BUFFER buffer = { 0 };
		auto ResPtr = pItem->m_AudioResource.lock();
		buffer.AudioBytes = ResPtr->GetSoundData().size();
		buffer.LoopCount = LoopCount;
		buffer.pAudioData = &ResPtr->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		//pImplの関数を呼ぶ
		pImpl->Start(pItem, buffer, Volume);
	}
	void MultiSoundEffect::Stop(const wstring& ResKey) {
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem) {
			throw BaseException(
				L"指定の名前のオーディオリソースがありません",
				ResKey,
				L"MultiSoundEffect::Stop()"
			);
		}
		pImpl->Stop(pItem);
	}


	struct CellNode {
		int x;
		int z;
		int px;
		int pz;
		int cost;
		int heuristic;
		int score;
	};


	//--------------------------------------------------------------------------------------
	//	struct PathSearch::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct PathSearch::Impl {
		weak_ptr<StageCellMap> m_StageCellMap;
		//目標のセルインデックス
		CellIndex m_TargetIndex;
		//こちら側のセルインデックス
		CellIndex m_BaseIndex;
		vector<CellNode> m_OpenVec;
		vector<CellNode> m_CloseVec;
		vector<vector<int>> m_WayArr = {
			{ -1, -1 },
			{ 0, -1 },
			{ 1, -1 },
			{ 1,  0 },
			{ 1,  1 },
			{ 0,  1 },
			{ -1,  1 },
			{ -1,  0 },
		};

		static bool NodeAsc(const CellNode& left, const CellNode& right) {
			return left.score < right.score;
		}
		bool IsInOpen(const CellIndex& Chk);
		bool AddOpenNode(const CellIndex& Base, const CellIndex& Target);
		int GetHeuristic(const CellIndex& Base, const CellIndex& Target);
		bool IsInVector(const CellIndex& Base, const vector<CellNode>& vec, CellNode& retNode);
		void InOpenScore(const CellIndex& Base, vector<CellNode>& retvec);
		bool SearchCellBase(const CellIndex& Start, const CellIndex& Target);
		shared_ptr<StageCellMap> GetStageCellMap() const;


		Impl()
		{}
		~Impl() {}
	};

	int PathSearch::Impl::GetHeuristic(const CellIndex& Base, const CellIndex& Target) {
		int spanX = abs((int)Base.x - (int)Target.x);
		int spanZ = abs((int)Base.z - (int)Target.z);
		//どちらか長いほうを返す
		return (spanX >= spanZ) ? spanX : spanZ;
	}

	bool PathSearch::Impl::IsInVector(const CellIndex& Base, const vector<CellNode>& vec, CellNode& retNode) {
		for (auto& v : vec) {
			if (Base.x == v.x && Base.z == v.z) {
				retNode = v;
				return true;
			}
		}
		return false;
	}

	bool PathSearch::Impl::IsInOpen(const CellIndex& Chk) {
		auto it = m_OpenVec.begin();
		while (it != m_OpenVec.end()) {
			if (it->x == Chk.x && it->z == Chk.z) {
				return true;
			}
			it++;
		}
		return false;
	}

	void PathSearch::Impl::InOpenScore(const CellIndex& Base, vector<CellNode>& retvec) {
		for (size_t i = 0; i < m_WayArr.size(); i++) {
			int posx = Base.x + m_WayArr[i][0];
			int posz = Base.z + m_WayArr[i][1];
			CellNode Node;
			if (IsInVector(CellIndex(posx, posz), m_OpenVec, Node)) {
				retvec.push_back(Node);
			}
		}
		if (!retvec.empty()) {
			std::sort(retvec.begin(), retvec.end(), NodeAsc);
		}
	}

	bool PathSearch::Impl::AddOpenNode(const CellIndex& Base, const CellIndex& Target) {
		//Openリストから指定のNodeを取り出す
		auto it = m_OpenVec.begin();
		CellNode TempNode;
		bool find = false;
		while (it != m_OpenVec.end()) {
			if (it->x == Base.x && it->z == Base.z) {
				//取り出したらeraseしCloseリストに追加
				TempNode = *it;
				m_CloseVec.push_back(TempNode);
				m_OpenVec.erase(it);
				find = true;
				break;
			}
			it++;
		}
		if (!find) {
			return false;
		}
		auto ShCellMap = m_StageCellMap.lock();
		if (!ShCellMap) {
			//セルマップがない
			throw BaseException(
				L"セルマップが指定されてません",
				L"if (!ShCellMap)",
				L"PathFindSteering::AddOpenNode()"
			);
		}

		auto& CellVec = ShCellMap->GetCellVec();
		for (size_t i = 0; i < m_WayArr.size(); i++) {
			int posx = TempNode.x + m_WayArr[i][0];
			int posz = TempNode.z + m_WayArr[i][1];
			if (posx >= 0 && posx < (int)CellVec.size() &&
				posz >= 0 && posz < (int)CellVec[posx].size()) {
				//posx,poszがセルマップの範囲内
				if (CellVec[posx][posz].m_Cost == -1) {
					//コストが-1ではない
					continue;
				}
				CellNode ret;
				if (IsInVector(CellIndex(posx, posz), m_OpenVec, ret)) {
					//OpenVecにあった
					continue;
				}
				if (IsInVector(CellIndex(posx, posz), m_CloseVec, ret)) {
					//CloseVecにあった
					continue;
				}
				//現在オープンリストにない
				//OpenVecに追加
				CellIndex BaseIndex(posx, posz);
				CellNode Node;
				Node.x = posx;
				Node.z = posz;
				Node.px = TempNode.x;
				Node.pz = TempNode.z;
				Node.cost = TempNode.cost + CellVec[posx][posz].m_Cost;
				Node.heuristic = GetHeuristic(BaseIndex, Target);
				Node.score = Node.cost + Node.heuristic;
				m_OpenVec.push_back(Node);
			}
		}
		return true;
	}

	bool PathSearch::Impl::SearchCellBase(const CellIndex& Start, const CellIndex& Target) {
		if (Start == Target) {
			//たどり着いた
			return true;
		}
		if (!AddOpenNode(Start, Target)) {
			return false;
		}
		//隣接するOpenなNodeを取得
		vector<CellNode> adjacentvec;
		InOpenScore(Start, adjacentvec);
		if (!adjacentvec.empty()) {
			//隣接する有効なNodeを順番に検証
			for (auto& v : adjacentvec) {
				CellIndex Index;
				Index.x = v.x;
				Index.z = v.z;
				if (SearchCellBase(Index, Target)) {
					return true;
				}
			}
		}
		return false;
	}


	shared_ptr<StageCellMap> PathSearch::Impl::GetStageCellMap() const {
		auto ShCellMap = m_StageCellMap.lock();
		if (!ShCellMap) {
			//セルマップがない
			throw BaseException(
				L"セルマップが指定されてません",
				L"if (!ShCellMap)",
				L"PathSearch::Impl::GetStageCellMap()"
			);
		}
		return ShCellMap;
	}








	//--------------------------------------------------------------------------------------
	///	 経路検索操舵コンポーネント
	//--------------------------------------------------------------------------------------
	PathSearch::PathSearch(const shared_ptr<GameObject>& GameObjectPtr,
		const shared_ptr<StageCellMap>& StageCellMapPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{
		pImpl->m_StageCellMap = StageCellMapPtr;
	}

	PathSearch::~PathSearch() {}


	bool PathSearch::SearchCell(const Vector3& TargetPosition, vector<CellIndex>& RetCellIndexVec) {
		//オープン・クローズリストのクリア
		pImpl->m_OpenVec.clear();
		pImpl->m_CloseVec.clear();
		RetCellIndexVec.clear();
		auto ShCellMap = pImpl->GetStageCellMap();
		//ターゲットのインデックスを取得
		if (!ShCellMap->FindCell(TargetPosition, pImpl->m_TargetIndex)) {
			//無かったらリターン
			return false;
		}
		auto Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
		//自分自身のインデックスを取得
		if (!ShCellMap->FindCell(Pos, pImpl->m_BaseIndex)) {
			//無かったらリターン
			return false;
		}
		//最初のNodeを設定
		auto& CellVec = ShCellMap->GetCellVec();
		CellNode Node;
		Node.x = pImpl->m_BaseIndex.x;
		Node.z = pImpl->m_BaseIndex.z;
		Node.px = -1;
		Node.pz = -1;
		Node.cost = CellVec[pImpl->m_BaseIndex.x][pImpl->m_BaseIndex.z].m_Cost;
		Node.heuristic = pImpl->GetHeuristic(pImpl->m_BaseIndex, pImpl->m_TargetIndex);
		Node.score = Node.cost + Node.heuristic;
		if (Node.cost == -1) {
			//最初の場所が行けない場所だった
			return false;
		}
		bool DirectHit = false;
		for (UINT x = 0; x < CellVec.size(); x++) {
			for (UINT z = 0; z < CellVec[x].size(); z++) {
				if (CellVec[x][z].m_Cost == -1) {
					//障害物があった
					if (HitTest::SEGMENT_AABB(Pos, TargetPosition, CellVec[x][z].m_PieceRange)) {
						//障害物とレイがヒットしている
						DirectHit = true;
						break;
					}
				}
			}
			if (DirectHit) {
				break;
			}
		}
		if (!DirectHit) {
			//どの障害物ともヒットしてない
			//直接ターゲットに行ける
			//まず自分自身
			RetCellIndexVec.push_back(pImpl->m_BaseIndex);
			if (pImpl->m_BaseIndex != pImpl->m_TargetIndex) {
				//続いてターゲット
				RetCellIndexVec.push_back(pImpl->m_TargetIndex);
			}
			//成功
			return true;
		}
		pImpl->m_OpenVec.push_back(Node);
		if (pImpl->SearchCellBase(pImpl->m_BaseIndex, pImpl->m_TargetIndex)) {
			//経路が見つかった
			CellNode TempNode;
			CellIndex TempCellIndex;
			//最初のNodeはOpenリストにある
			pImpl->IsInVector(pImpl->m_TargetIndex, pImpl->m_OpenVec, TempNode);
			TempCellIndex.x = TempNode.x;
			TempCellIndex.z = TempNode.z;
			RetCellIndexVec.push_back(TempCellIndex);
			while (pImpl->m_BaseIndex != TempCellIndex) {
				//2番目以降はCloseリストにある
				TempCellIndex.x = TempNode.px;
				TempCellIndex.z = TempNode.pz;
				if (!pImpl->IsInVector(TempCellIndex, pImpl->m_CloseVec, TempNode)) {
					break;
				}
				RetCellIndexVec.push_back(TempCellIndex);
				TempCellIndex.x = TempNode.x;
				TempCellIndex.z = TempNode.z;
			}
			//逆順にして終了
			std::reverse(RetCellIndexVec.begin(), RetCellIndexVec.end());
			return true;
		}
		//経路が見つからなかった
		return false;

	}

}
//end basecross
