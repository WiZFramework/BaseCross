/*!
@file SoundObject.h
@brief サウンド実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	サウンドオブジェクト
	//--------------------------------------------------------------------------------------
	SoundObject::SoundObject(const wstring& ResKey) :
		ObjectInterface(),
		ShapeInterface(),
		m_ResKey(ResKey)
	{
		try {
			if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
				return;
			}
			auto SoundRes = App::GetApp()->GetResource<AudioResource>(ResKey);
			m_AudioResource = SoundRes;
			auto Engine = App::GetApp()->GetAudioManager()->GetSoundEffectEngine();

			ThrowIfFailed(
				Engine->CreateSourceVoice(&m_pSourceVoice, SoundRes->GetOutputWaveFormatEx()),
				L"サウンドエフェクト用サウンドボイスの作成に失敗しました",
				L"Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, SountRes->GetOutputWaveFormatEx())",
				L"SSoundObject::SoundObject()"
			);
		}
		catch (...) {
			throw;
		}
	}
	SoundObject::~SoundObject() {}

	shared_ptr<AudioResource> SoundObject::GetAudioResource() const {
		if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
			return nullptr;
		}
		auto shptr = m_AudioResource.lock();
		if (!shptr) {
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (!shptr)",
				L"SoundObject::GetAudioResource()"
			);

		}
		return shptr;
	}

	void SoundObject::Start(size_t LoopCount, float Volume) {
		if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
			return;
		}
		if (!m_pSourceVoice) {
			return;
		}
		auto shRes = m_AudioResource.lock();
		if (!shRes) {
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (!shRes)",
				L"SoundObject::Start()"
			);

		}
		XAUDIO2_BUFFER buffer = { 0 };
		buffer.AudioBytes = shRes->GetSoundData().size();
		buffer.LoopCount = LoopCount;
		buffer.pAudioData = &shRes->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		//多重定義関数を呼ぶ
		Start(buffer, Volume);

	}

	void SoundObject::Start(const XAUDIO2_BUFFER& Buffer, float Volume) {
		if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
			return;
		}
		if (!m_pSourceVoice) {
			return;
		}
		ThrowIfFailed(
			m_pSourceVoice->Stop(),
			L"サウンドエフェクト用サウンドボイスの停止に失敗しました",
			L"m_pSourceVoice->Stop()",
			L"SoundObject::Start()"
		);
		ThrowIfFailed(
			m_pSourceVoice->FlushSourceBuffers(),
			L"サウンドエフェクト用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pImpl->m_pSourceVoice->FlushSourceBuffers()",
			L"SoundObject::Start()"
		);
		if (m_AudioResource.expired()) {
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"SoundObject::Start()"
			);
		}
		ThrowIfFailed(
			m_pSourceVoice->SetVolume(Volume),
			L"サウンドエフェクト用サウンドのボリューム設定に失敗しました",
			L"m_pSourceVoice->SetVolume()",
			L"SoundObject::Start()"
		);

		ThrowIfFailed(
			m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"サウンドエフェクト用サウンドのソースバッファ設定に失敗しました",
			L"m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"SoundObject::Start()"
		);

		ThrowIfFailed(
			m_pSourceVoice->Start(),
			L"サウンドエフェクト用サウンドのスタートに失敗しました",
			L"m_pSourceVoice->Start()",
			L"SoundObject::Start()"
		);

	}

	void SoundObject::Stop() {
		if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
			return;
		}
		if (!m_pSourceVoice) {
			return;
		}
		ThrowIfFailed(
			m_pSourceVoice->Stop(),
			L"サウンドエフェクト用サウンドボイスの停止に失敗しました",
			L"m_pSourceVoice->Stop()",
			L"SoundObject::Stop()"
		);
		ThrowIfFailed(
			m_pSourceVoice->FlushSourceBuffers(),
			L"サウンドエフェクト用サウンドボイスのバッファのフラッシュに失敗しました",
			L"m_pSourceVoice->FlushSourceBuffers()",
			L"SoundObject::Stop()"
		);

	}



}
// end basecross
