/*!
@file Components.cpp
@brief �R���|�[�l���g����
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct Component::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct Component::Impl {
		weak_ptr<GameObject> m_GameObject;
		bool m_UpdateActive{ true };	//update���邩�ǂ���
		bool m_DrawActive{ true };		//Draw���邩�ǂ���
		explicit Impl(const shared_ptr<GameObject>& GameObjectPtr) :
			m_GameObject(GameObjectPtr)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class Component :public Object;
	//	�p�r: �R���|�[�l���g���N���X
	//--------------------------------------------------------------------------------------
	Component::Component(const shared_ptr<GameObject>& GameObjectPtr) :
		ObjectInterface(),
		ShapeInterface(),
		pImpl(new Impl(GameObjectPtr))
	{}
	Component::~Component() {}
	//�A�N�Z�T
	shared_ptr<GameObject> Component::GetGameObject() const {
		auto shptr = pImpl->m_GameObject.lock();
		if (!shptr) {
			throw BaseException(
				L"GameObject�͗L���ł͂���܂���",
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
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct Transform::Impl {
		bool m_Init{ false };	//�������ς݂��ǂ����i1��ڂ�Update�ŁABefore�ɒl������j
		//1�O�̕ϐ�
		Vector3 m_BeforeScale;
		Vector3 m_BeforePivot;
		Quaternion m_BeforeQuaternion;
		Vector3 m_BeforePosition;
		//�Čv�Z�}���p�ϐ�
		bool m_BeforeChangeed;
		Matrix4X4 m_BeforeWorldMatrix;
		//���݂̕ϐ�
		Vector3 m_Scale;
		Vector3 m_Pivot;
		Quaternion m_Quaternion;
		Vector3 m_Position;
		//�Čv�Z�}���p�ϐ�
		bool m_Changeed;
		Matrix4X4 m_WorldMatrix;

		Impl():
			//�X�P�[���̂ݏ������i���̓f�t�H���g�����ł悢�j
			m_BeforeScale(1.0f,1.0f,1.0f),
			m_Scale(1.0f, 1.0f, 1.0f),
			m_BeforeChangeed(true),
			m_Changeed(true)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class Transform : public Component ;
	//	�p�r: �ω�
	//--------------------------------------------------------------------------------------
	Transform::Transform(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr), pImpl(new Impl())
	{}
	Transform::~Transform() {}

	//�A�N�Z�T
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
		//�O��̃^�[������̎���
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

	//����
	void Transform::OnUpdate() {
		if (!pImpl->m_Init) {
			SetToBefore();
			pImpl->m_Init = true;
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct PlayMusic::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct PlayMusic::Impl {
		weak_ptr<AudioResource> m_AudioResource;
		IXAudio2SourceVoice* m_pSourceVoice;
		//���\�[�X�L�[
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
	//	�p�r: �~���[�W�b�N�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
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
				L"�~���[�W�b�N�p�T�E���h�{�C�X�̍쐬�Ɏ��s���܂���",
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
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
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
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
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
		//���d��`�֐����Ă�
		Start(buffer, Volume);
	}

	void PlayMusic::Start(const XAUDIO2_BUFFER& Buffer, float Volume) {
		if (!pImpl->m_pSourceVoice) {
			return;
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->Stop(),
			L"�~���[�W�b�N�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Start()"
		);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"�~���[�W�b�N�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Start()"
		);
		if (pImpl->m_AudioResource.expired()) {
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				L"if (pImpl->m_AudioResource.expired())",
				L"PlayMusic::Start()"
			);
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->SetVolume(Volume),
			L"�~���[�W�b�N�̃{�����[���ݒ�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->SetVolume()",
			L"PlayMusic::Start()"
		);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"�~���[�W�b�N�̃\�[�X�o�b�t�@�ݒ�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"PlayMusic::Start()"
		);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->Start(),
			L"�~���[�W�b�N�T�E���h�̃X�^�[�g�Ɏ��s���܂���",
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
			L"�~���[�W�b�N�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Stop()"
		);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"�~���[�W�b�N�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->Stop()",
			L"PlayMusic::Stop()"
		);
	}





	//--------------------------------------------------------------------------------------
	//	struct SoundEffect::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct SoundEffect::Impl {
		weak_ptr<AudioResource> m_AudioResource;
		IXAudio2SourceVoice* m_pSourceVoice;
		//���\�[�X�L�[
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
	//	�p�r: �T�E���h�G�t�F�N�g�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
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
				L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̍쐬�Ɏ��s���܂���",
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
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
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
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
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
		//���d��`�֐����Ă�
		Start(buffer, Volume);
	}

	void SoundEffect::Start(const XAUDIO2_BUFFER& Buffer, float Volume) {
		if (!pImpl->m_pSourceVoice) {
			return;
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->Stop(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->Stop()",
			L"SoundEffect::Start()"
		);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->FlushSourceBuffers()",
			L"SoundEffect::Start()"
		);
		if (pImpl->m_AudioResource.expired()) {
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				L"if (pImpl->m_AudioResource.expired())",
				L"SoundEffect::Start()"
			);
		}
		ThrowIfFailed(
			pImpl->m_pSourceVoice->SetVolume(Volume),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃{�����[���ݒ�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->SetVolume()",
			L"SoundEffect::Start()"
		);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃\�[�X�o�b�t�@�ݒ�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"SoundEffect::Start()"
		);

		ThrowIfFailed(
			pImpl->m_pSourceVoice->Start(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃X�^�[�g�Ɏ��s���܂���",
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
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->Stop()",
			L"SoundEffect::Stop()"
		);
		ThrowIfFailed(
			pImpl->m_pSourceVoice->FlushSourceBuffers(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pImpl->m_pSourceVoice->FlushSourceBuffers()",
			L"SoundEffect::Stop()"
		);
	}




	//--------------------------------------------------------------------------------------
	//	struct MultiSoundEffect::Impl;
	//	�p�r: �R���|�[�l���gImpl�N���X
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
				//�������O����������
				return &it->second;
			}
			return nullptr;
		}
		void RemoveItem(const wstring& ResKey) {
			auto it = m_SoundMap.find(ResKey);
			if (it != m_SoundMap.end()) {
				//�������O����������
				//Map����폜
				m_SoundMap.erase(it);
			}
		}
		void Start(const SoundItem* pItem, const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Stop(const SoundItem* pItem);

	};

	void MultiSoundEffect::Impl::Start(const SoundItem* pItem, const XAUDIO2_BUFFER& Buffer, float Volume) {
		if (!pItem->m_pSourceVoice) {
			throw BaseException(
				L"�T�E���h�{�C�X���s��ł�",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiSoundEffect::Impl::Start()"
			);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->Stop(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiSoundEffect::Impl::Start()"
		);
		ThrowIfFailed(
			pItem->m_pSourceVoice->FlushSourceBuffers(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiSoundEffect::Impl::Start()"
		);
		if (pItem->m_AudioResource.expired()) {
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				L"if (pImpl->m_AudioResource.expired())",
				L"MultiSoundEffect::Impl::Start()"
			);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->SetVolume(Volume),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃{�����[���ݒ�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->SetVolume()",
			L"MultiSoundEffect::Impl::Start()"
		);

		ThrowIfFailed(
			pItem->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃\�[�X�o�b�t�@�ݒ�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"MultiSoundEffect::Impl::Start()"
		);

		ThrowIfFailed(
			pItem->m_pSourceVoice->Start(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃X�^�[�g�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->Start()",
			L"MultiSoundEffect::Impl::Start()"
		);
	}

	void MultiSoundEffect::Impl::Stop(const SoundItem* pItem) {
		if (!pItem->m_pSourceVoice) {
			throw BaseException(
				L"�T�E���h�{�C�X���s��ł�",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiSoundEffect::Impl::Stop()"
			);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->Stop(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiSoundEffect::Impl::Stop()"
		);
		ThrowIfFailed(
			pItem->m_pSourceVoice->FlushSourceBuffers(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiSoundEffect::Impl::Stop()"
		);
	}



	//--------------------------------------------------------------------------------------
	//	class MultiSoundEffect: public Component;
	//	�p�r: �}���`�T�E���h�G�t�F�N�g�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiSoundEffect::MultiSoundEffect(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	MultiSoundEffect::~MultiSoundEffect() {}

	//�A�N�Z�T
	shared_ptr<AudioResource> MultiSoundEffect::AddAudioResource(const wstring& ResKey) {
		try {
			if (ResKey == L"") {
				throw BaseException(
					L"�I�[�f�B�I���\�[�X�����󔒂ł�",
					L"if (ResKey == L\"\")",
					L"MultiSoundEffect::AddAudioResource()"
				);
			}
			auto pItem = pImpl->GetItem(ResKey);
			if (pItem) {
				//�������O����������
				if (!pItem->m_AudioResource.expired()) {
					return pItem->m_AudioResource.lock();
				}
				else {
					throw BaseException(
						L"�������O�̃I�[�f�B�I���\�[�X������܂������A���\�[�X�������ł�",
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
				L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̍쐬�Ɏ��s���܂���",
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
				L"�I�[�f�B�I���\�[�X�����󔒂ł�",
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
					L"�I�[�f�B�I���\�[�X�����󔒂ł�",
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
			//�������O����������
			if (!pItem->m_AudioResource.expired()) {
				return pItem->m_AudioResource.lock();
			}
			else {
				if (ExceptionActive) {
					throw BaseException(
						L"�������O�̃I�[�f�B�I���\�[�X������܂������A���\�[�X�������ł�",
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
					L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
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
				L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
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
				L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
				ResKey,
				L"MultiSoundEffect::Start()"
			);
		}
		if (pItem->m_AudioResource.expired()) {
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
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
		//pImpl�̊֐����Ă�
		pImpl->Start(pItem, buffer, Volume);
	}
	void MultiSoundEffect::Stop(const wstring& ResKey) {
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem) {
			throw BaseException(
				L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
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
	//	�p�r: �R���|�[�l���gImpl�N���X
	//--------------------------------------------------------------------------------------
	struct PathSearch::Impl {
		weak_ptr<StageCellMap> m_StageCellMap;
		//�ڕW�̃Z���C���f�b�N�X
		CellIndex m_TargetIndex;
		//�����瑤�̃Z���C���f�b�N�X
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
		//�ǂ��炩�����ق���Ԃ�
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
		//Open���X�g����w���Node�����o��
		auto it = m_OpenVec.begin();
		CellNode TempNode;
		bool find = false;
		while (it != m_OpenVec.end()) {
			if (it->x == Base.x && it->z == Base.z) {
				//���o������erase��Close���X�g�ɒǉ�
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
			//�Z���}�b�v���Ȃ�
			throw BaseException(
				L"�Z���}�b�v���w�肳��Ă܂���",
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
				//posx,posz���Z���}�b�v�͈͓̔�
				if (CellVec[posx][posz].m_Cost == -1) {
					//�R�X�g��-1�ł͂Ȃ�
					continue;
				}
				CellNode ret;
				if (IsInVector(CellIndex(posx, posz), m_OpenVec, ret)) {
					//OpenVec�ɂ�����
					continue;
				}
				if (IsInVector(CellIndex(posx, posz), m_CloseVec, ret)) {
					//CloseVec�ɂ�����
					continue;
				}
				//���݃I�[�v�����X�g�ɂȂ�
				//OpenVec�ɒǉ�
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
			//���ǂ蒅����
			return true;
		}
		if (!AddOpenNode(Start, Target)) {
			return false;
		}
		//�אڂ���Open��Node���擾
		vector<CellNode> adjacentvec;
		InOpenScore(Start, adjacentvec);
		if (!adjacentvec.empty()) {
			//�אڂ���L����Node�����ԂɌ���
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
			//�Z���}�b�v���Ȃ�
			throw BaseException(
				L"�Z���}�b�v���w�肳��Ă܂���",
				L"if (!ShCellMap)",
				L"PathSearch::Impl::GetStageCellMap()"
			);
		}
		return ShCellMap;
	}








	//--------------------------------------------------------------------------------------
	///	 �o�H�������ǃR���|�[�l���g
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
		//�I�[�v���E�N���[�Y���X�g�̃N���A
		pImpl->m_OpenVec.clear();
		pImpl->m_CloseVec.clear();
		RetCellIndexVec.clear();
		auto ShCellMap = pImpl->GetStageCellMap();
		//�^�[�Q�b�g�̃C���f�b�N�X���擾
		if (!ShCellMap->FindCell(TargetPosition, pImpl->m_TargetIndex)) {
			//���������烊�^�[��
			return false;
		}
		auto Pos = GetGameObject()->GetComponent<Transform>()->GetPosition();
		//�������g�̃C���f�b�N�X���擾
		if (!ShCellMap->FindCell(Pos, pImpl->m_BaseIndex)) {
			//���������烊�^�[��
			return false;
		}
		//�ŏ���Node��ݒ�
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
			//�ŏ��̏ꏊ���s���Ȃ��ꏊ������
			return false;
		}
		bool DirectHit = false;
		for (UINT x = 0; x < CellVec.size(); x++) {
			for (UINT z = 0; z < CellVec[x].size(); z++) {
				if (CellVec[x][z].m_Cost == -1) {
					//��Q����������
					if (HitTest::SEGMENT_AABB(Pos, TargetPosition, CellVec[x][z].m_PieceRange)) {
						//��Q���ƃ��C���q�b�g���Ă���
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
			//�ǂ̏�Q���Ƃ��q�b�g���ĂȂ�
			//���ڃ^�[�Q�b�g�ɍs����
			//�܂��������g
			RetCellIndexVec.push_back(pImpl->m_BaseIndex);
			if (pImpl->m_BaseIndex != pImpl->m_TargetIndex) {
				//�����ă^�[�Q�b�g
				RetCellIndexVec.push_back(pImpl->m_TargetIndex);
			}
			//����
			return true;
		}
		pImpl->m_OpenVec.push_back(Node);
		if (pImpl->SearchCellBase(pImpl->m_BaseIndex, pImpl->m_TargetIndex)) {
			//�o�H����������
			CellNode TempNode;
			CellIndex TempCellIndex;
			//�ŏ���Node��Open���X�g�ɂ���
			pImpl->IsInVector(pImpl->m_TargetIndex, pImpl->m_OpenVec, TempNode);
			TempCellIndex.x = TempNode.x;
			TempCellIndex.z = TempNode.z;
			RetCellIndexVec.push_back(TempCellIndex);
			while (pImpl->m_BaseIndex != TempCellIndex) {
				//2�Ԗڈȍ~��Close���X�g�ɂ���
				TempCellIndex.x = TempNode.px;
				TempCellIndex.z = TempNode.pz;
				if (!pImpl->IsInVector(TempCellIndex, pImpl->m_CloseVec, TempNode)) {
					break;
				}
				RetCellIndexVec.push_back(TempCellIndex);
				TempCellIndex.x = TempNode.x;
				TempCellIndex.z = TempNode.z;
			}
			//�t���ɂ��ďI��
			std::reverse(RetCellIndexVec.begin(), RetCellIndexVec.end());
			return true;
		}
		//�o�H��������Ȃ�����
		return false;

	}

}
//end basecross