/*!
@file Components.cpp
@brief コンポーネント実体
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
		//現在の変数
		Vector3 m_Scale;
		Vector3 m_Pivot;
		Quaternion m_Quaternion;
		Vector3 m_Position;
		Impl():
			//スケールのみ初期化（他はデフォルト処理でよい）
			m_BeforeScale(1.0f,1.0f,1.0f),
			m_Scale(1.0f, 1.0f, 1.0f)
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

	Matrix4X4 Transform::GetBeforeWorldMatrix() const{
		Matrix4X4 mat;
		mat.AffineTransformation(pImpl->m_BeforeScale,
			pImpl->m_BeforePivot, 
			pImpl->m_BeforeQuaternion, 
			pImpl->m_BeforePosition
		);
		return mat;
	}


	//Getter&Setter

	Vector3 Transform::GetScale() const {
		return pImpl->m_Scale;
	}

	void Transform::SetScale(const Vector3& Scale) {
		pImpl->m_Scale = Scale;
	}
	void Transform::SetScale(float x, float y, float z) {
		SetScale(Vector3(x, y, z));
	}

	Vector3 Transform::GetPivot() const {
		return pImpl->m_Pivot;
	}
	void Transform::SetPivot(const Vector3& Pivot) {
		pImpl->m_Pivot = Pivot;
	}
	void Transform::SetPivot(float x, float y, float z) {
		SetPivot(Vector3(x, y, z));
	}

	Quaternion Transform::GetQuaternion() const {
		return pImpl->m_Quaternion;
	}
	void Transform::SetQuaternion(const Quaternion& quaternion) {
		pImpl->m_Quaternion = quaternion;
		pImpl->m_Quaternion.Normalize();
	}
	Vector3 Transform::GetRotation() const {
		return pImpl->m_Quaternion.GetRotation();
	}

	void Transform::SetRotation(const Vector3& Rot) {
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
		pImpl->m_Position = Position;
	}
	void Transform::SetPosition(float x, float y, float z) {
		SetPosition(Vector3(x, y, z));
	}

	void Transform::ResetPosition(const Vector3& Position) {
		pImpl->m_BeforePosition = Position;
		pImpl->m_Position = Position;
	}


	Matrix4X4 Transform::GetWorldMatrix() const{
		Matrix4X4 mat;
		mat.AffineTransformation(
			pImpl->m_Scale,
			pImpl->m_Pivot,
			pImpl->m_Quaternion,
			pImpl->m_Position
		);
		return mat;
	}

	Vector3 Transform::GetVelocity() const {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vector3 Velocity = pImpl->m_Position - pImpl->m_BeforePosition;
		Velocity /= ElapsedTime;
		return Velocity;
	}


	void Transform::SetToBefore() {
		pImpl->m_BeforeScale = pImpl->m_Scale;
		pImpl->m_BeforePivot = pImpl->m_Pivot;
		pImpl->m_BeforeQuaternion = pImpl->m_Quaternion;
		pImpl->m_BeforePosition = pImpl->m_Position;
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



}
//end basecross
