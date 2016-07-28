/*!
@file App.h
@brief アプリケーションクラス。入力機器等
*/
#include "stdafx.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct AudioManager::Impl;
	//	用途: Implクラス
	//--------------------------------------------------------------------------------------
	struct AudioManager::Impl {
		bool m_audioAvailable;
		ComPtr<IXAudio2>    m_musicEngine;
		ComPtr<IXAudio2>    m_soundEffectEngine;
		IXAudio2MasteringVoice* m_musicMasteringVoice;
		IXAudio2MasteringVoice* m_soundEffectMasteringVoice;
		Impl() :
			m_audioAvailable{ false }
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	//	class AudioManager;
	//--------------------------------------------------------------------------------------
	AudioManager::AudioManager() :
		pImpl(new Impl)
	{
	}
	AudioManager::~AudioManager() {}

	void AudioManager::CreateDeviceIndependentResources()
	{
		UINT32 flags = 0;

		ThrowIfFailed(
			XAudio2Create(&pImpl->m_musicEngine, flags),
			L"音楽用オーディオエンジンの初期化に失敗しました",
			L"XAudio2Create(&m_musicEngine, flags)",
			L"AudioManager::CreateDeviceIndependentResources()"
		);

#if defined(_DEBUG)
		XAUDIO2_DEBUG_CONFIGURATION debugConfiguration = { 0 };
		debugConfiguration.BreakMask = XAUDIO2_LOG_ERRORS;
		debugConfiguration.TraceMask = XAUDIO2_LOG_ERRORS;
		pImpl->m_musicEngine->SetDebugConfiguration(&debugConfiguration);
#endif
		HRESULT hr = pImpl->m_musicEngine->CreateMasteringVoice(&pImpl->m_musicMasteringVoice);
		if (FAILED(hr))
		{
			throw BaseException(
				L"音楽のマスタリングボイスの初期化に失敗しました",
				L"m_musicEngine->CreateMasteringVoice(&m_musicMasteringVoice)",
				L"AudioManager::CreateDeviceIndependentResources()"
			);
			// Unable to create an audio device
			pImpl->m_audioAvailable = false;
			return;
		}

		ThrowIfFailed(
			XAudio2Create(&pImpl->m_soundEffectEngine, flags),
			L"サウンド用オーディオエンジンの初期化に失敗しました",
			L"XAudio2Create(&m_soundEffectEngine, flags)",
			L"AudioManager::CreateDeviceIndependentResources()"
		);
#if defined(_DEBUG)
		pImpl->m_soundEffectEngine->SetDebugConfiguration(&debugConfiguration);
#endif
		ThrowIfFailed(
			pImpl->m_soundEffectEngine->CreateMasteringVoice(&pImpl->m_soundEffectMasteringVoice),
			L"サウンド用マスタリングボイスの初期化に失敗しました",
			L"m_soundEffectEngine->CreateMasteringVoice(&m_soundEffectMasteringVoice)",
			L"AudioManager::CreateDeviceIndependentResources()"
		);
		pImpl->m_audioAvailable = true;
	}

	IXAudio2* AudioManager::GetMusicEngine()const
	{
		return pImpl->m_musicEngine.Get();
	}

	IXAudio2* AudioManager::GetSoundEffectEngine()const
	{
		return pImpl->m_soundEffectEngine.Get();
	}

	void AudioManager::SuspendAudio()
	{
		if (pImpl->m_audioAvailable)
		{
			pImpl->m_musicEngine->StopEngine();
			pImpl->m_soundEffectEngine->StopEngine();
		}
	}

	void AudioManager::ResumeAudio()
	{
		if (pImpl->m_audioAvailable)
		{
			ThrowIfFailed(
				pImpl->m_musicEngine->StartEngine(),
				L"音楽用エンジンのスタートに失敗しました",
				L"m_musicEngine->StartEngine()",
				L"AudioManager::CreateDeviceIndependentResources()"
			);
			ThrowIfFailed(
				pImpl->m_soundEffectEngine->StartEngine(),
				L"サウンド用エンジンのスタートに失敗しました",
				L"m_soundEffectEngine->StartEngine()",
				L"AudioManager::CreateDeviceIndependentResources()"
			);
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct AudioResource::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct AudioResource::Impl {
		WAVEFORMATEX m_WaveFormat;	//ウェブフォーマット
		vector<byte> m_SoundData;	//データ
		wstring m_FileName;		//ファイルへのパス
		Impl(const wstring& FileName) :
			m_FileName(FileName) {}
		~Impl() {}
		//ミューテックス
		std::mutex Mutex;

	};



	//--------------------------------------------------------------------------------------
	//	class AudioResource : public BaseResource;
	//	用途: オーディオリソース（wavなど）のラッピングクラス
	//--------------------------------------------------------------------------------------
	AudioResource::AudioResource(const wstring& FileName) :
		BaseResource(),
		pImpl(new Impl(FileName))
	{
		try {

			ThrowIfFailed(
				MFStartup(MF_VERSION),
				L"MediaFoundationの初期化に失敗しました",
				L"MFStartup(MF_VERSION)",
				L"AudioResource::AudioResource()"
			);

			ComPtr<IMFSourceReader> reader;

			ThrowIfFailed(
				MFCreateSourceReaderFromURL(FileName.c_str(), nullptr, &reader),
				L"サウンドリーダーの作成に失敗しました",
				L"MFCreateSourceReaderFromURL(FileName.c_str(),nullptr,&reader)",
				L"AudioResource::AudioResource()"
			);

			// Set the decoded output format as PCM.
			// XAudio2 on Windows can process PCM and ADPCM-encoded buffers.
			// When using MediaFoundation, this sample always decodes into PCM.
			Microsoft::WRL::ComPtr<IMFMediaType> mediaType;

			ThrowIfFailed(
				MFCreateMediaType(&mediaType),
				L"メディアタイプの作成に失敗しました",
				L"MFCreateMediaType(&mediaType)",
				L"AudioResource::AudioResource()"
			);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio),
				L"メディアGUIDの設定に失敗しました",
				L"mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio)",
				L"AudioResource::AudioResource()"
			);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM),
				L"メディアサブGUIDの設定に失敗しました",
				L"mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM)",
				L"AudioResource::AudioResource()"
			);

			ThrowIfFailed(
				reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get()),
				L"リーダーへのメディアタイプの設定に失敗しました",
				L"reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get())",
				L"AudioResource::AudioResource()"
			);


			// Get the complete WAVEFORMAT from the Media Type.
			Microsoft::WRL::ComPtr<IMFMediaType> outputMediaType;

			ThrowIfFailed(
				reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType),
				L"出力用のメディアタイプの設定に失敗しました",
				L"reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType)",
				L"AudioResource::AudioResource()"
			);

			UINT32 size = 0;
			WAVEFORMATEX* waveFormat;

			ThrowIfFailed(
				MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size),
				L"ウェブフォーマットの設定に失敗しました",
				L"MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size)",
				L"AudioResource::AudioResource()"
			);


			CopyMemory(&pImpl->m_WaveFormat, waveFormat, sizeof(pImpl->m_WaveFormat));
			CoTaskMemFree(waveFormat);

			PROPVARIANT propVariant;

			ThrowIfFailed(
				reader->GetPresentationAttribute(static_cast<uint32>(MF_SOURCE_READER_MEDIASOURCE), MF_PD_DURATION, &propVariant),
				L"アトリビュートの設定に失敗しました",
				L"reader->GetPresentationAttribute(static_cast<uint32>(MF_SOURCE_READER_MEDIASOURCE), MF_PD_DURATION, &propVariant)",
				L"AudioResource::AudioResource()"
			);

			// 'duration' is in 100ns units; convert to seconds, and round up
			// to the nearest whole byte.
			LONGLONG duration = propVariant.uhVal.QuadPart;
			unsigned int maxStreamLengthInBytes =
				static_cast<unsigned int>(
				((duration * static_cast<ULONGLONG>(pImpl->m_WaveFormat.nAvgBytesPerSec)) + 10000000) /
					10000000
					);

			pImpl->m_SoundData.resize(maxStreamLengthInBytes);

			ComPtr<IMFSample> sample;
			ComPtr<IMFMediaBuffer> mediaBuffer;
			DWORD flags = 0;

			int positionInData = 0;
			bool done = false;
			while (!done)
			{

				ThrowIfFailed(
					reader->ReadSample(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &flags, nullptr, &sample),
					L"サンプラーの読み込みに失敗しました",
					L"reader->ReadSample(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &flags, nullptr, &sample)",
					L"AudioResource::AudioResource()"
				);

				if (sample != nullptr)
				{
					ThrowIfFailed(
						sample->ConvertToContiguousBuffer(&mediaBuffer),
						L"サンプラーのコンバートに失敗しました",
						L"sample->ConvertToContiguousBuffer(&mediaBuffer)",
						L"AudioResource::AudioResource()"
					);

					BYTE *audioData = nullptr;
					DWORD sampleBufferLength = 0;

					ThrowIfFailed(
						mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength),
						L"バッファのLockに失敗しました",
						L"mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength)",
						L"AudioResource::AudioResource()"
					);

					for (DWORD i = 0; i < sampleBufferLength; i++)
					{
						pImpl->m_SoundData[positionInData++] = audioData[i];
					}
				}
				if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
				{
					done = true;
				}
			}
		}
		catch (...) {
			throw;
		}
	}
	AudioResource::~AudioResource() {}

	const vector<byte>& AudioResource::GetSoundData()const {
		return pImpl->m_SoundData;
	}

	WAVEFORMATEX*  AudioResource::GetOutputWaveFormatEx()const {
		return &pImpl->m_WaveFormat;
	}





	//--------------------------------------------------------------------------------------
	//	class App;
	//	用途: アプリケーションクラス
	//--------------------------------------------------------------------------------------
	//static変数実体
	unique_ptr<App, App::AppDeleter> App::m_App;
	//構築
	App::App(HINSTANCE hInstance, HWND hWnd, bool FullScreen, UINT Width, UINT Height) :
		m_hInstance{ hInstance },
		m_hWnd{ hWnd },
		m_FullScreen{ FullScreen },
		m_GameWidth{ Width },
		m_GameHeight{ Height },
		m_Timer()
	{
		try {
			//基準ディレクトリの設定
			//相対パスにすると、ファイルダイアログでカレントパスが狂うので
			//絶対パス指定
			wchar_t Modulebuff[MAX_PATH];
			wchar_t Drivebuff[_MAX_DRIVE];
			wchar_t Dirbuff[_MAX_DIR];
			wchar_t FileNamebuff[_MAX_FNAME];
			wchar_t Extbuff[_MAX_EXT];

			::ZeroMemory(Modulebuff, sizeof(Modulebuff));
			::ZeroMemory(Drivebuff, sizeof(Drivebuff));
			::ZeroMemory(Dirbuff, sizeof(Dirbuff));
			::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
			::ZeroMemory(Extbuff, sizeof(Extbuff));

			//モジュール名（プログラムファイル名）を得る
			if (!::GetModuleFileName(nullptr, Modulebuff, sizeof(Modulebuff))) {
				throw BaseException(
					L"モジュールが取得できません。",
					L"if(!::GetModuleFileName())",
					L"App::App()"
				);
			}
			m_wstrModulePath = Modulebuff;
			//モジュール名から、各ブロックに分ける
			_wsplitpath_s(Modulebuff,
				Drivebuff, _MAX_DRIVE,
				Dirbuff, _MAX_DIR,
				FileNamebuff, _MAX_FNAME,
				Extbuff, _MAX_EXT);

			//ドライブ名の取得
			m_wstrDir = Drivebuff;
			//ディレクトリ名の取得
			m_wstrDir += Dirbuff;
			//mediaディレクトリを探す
			m_wstrDataPath = m_wstrDir;
			m_wstrDataPath += L"media";
			//まず、実行ファイルと同じディレクトリを探す
			DWORD RetCode;
			RetCode = GetFileAttributes(m_wstrDataPath.c_str());
			if (RetCode == 0xFFFFFFFF) {
				//失敗した
				m_wstrDataPath = m_wstrDir;
				m_wstrDataPath += L"..\\media";
				RetCode = GetFileAttributes(m_wstrDataPath.c_str());
				if (RetCode == 0xFFFFFFFF) {
					//再び失敗した
					throw BaseException(
						L"mediaディレクトリを確認できません。",
						L"if(RetCode == 0xFFFFFFFF)",
						L"App::App()"
					);
				}
				else {
					m_wstrDataPath += L"\\";
					//相対パスの設定
					m_wstrRelativeDataPath = L"..\\media\\";
				}
			}
			else {
				m_wstrDataPath += L"\\";
				//相対パスの設定
				m_wstrRelativeDataPath = L"media\\";
			}
			m_wstrShadersPath = m_wstrDataPath + L"Shaders\\";
			m_wstrRelativeShadersPath = m_wstrRelativeDataPath + L"Shaders\\";


			////デバイスリソースの構築
			m_DeviceResources = shared_ptr<DeviceResources>(new DeviceResources(hWnd, FullScreen, Width, Height));
			m_DeviceResources->AfterInitContents();
			//オーディオマネージャの取得
			GetAudioManager();
			//乱数の初期化
			srand((unsigned)time(nullptr));

		}
		catch (...) {
			throw;
		}
	}

	//シングルトン構築とアクセサ
	unique_ptr<App, App::AppDeleter>& App::GetApp(HINSTANCE hInstance, HWND hWnd,
		bool FullScreen, UINT Width, UINT Height) {
		try {
			if (m_App.get() == 0) {
				//自分自身の構築
				m_App.reset(new App(hInstance, hWnd, FullScreen, Width, Height));
			}
			return m_App;
		}
		catch (...) {
			throw;
		}

	}

	bool App::AppCheck() {
		if (m_App.get() == 0) {
			return false;
		}
		return true;
	}

	//強制破棄
	void App::DeleteApp() {
		if (m_App.get()) {
			m_App.reset();
		}
	}
	// オーディオマネージャの取得
	unique_ptr<AudioManager>& App::GetAudioManager() {
		try {
			if (m_AudioManager.get() == 0) {
				m_AudioManager.reset(new AudioManager);
				m_AudioManager->CreateDeviceIndependentResources();

			}
			return m_AudioManager;
		}
		catch (...) {
			throw;
		}
	}

	void App::RegisterResource(const wstring& Key, const shared_ptr<BaseResource>& ResObj) {
		try {
			if (Key == L"") {
				throw BaseException(
					L"キーが空白です",
					L"if(Key == L\"\")",
					L"App::RegisterResource()"
				);
			}
			if (!ResObj) {
				throw BaseException(
					L"リソースが不定です",
					L"if(!pResObj)",
					L"App::RegisterResource()"
				);
			}
			map<wstring, shared_ptr<BaseResource> >::iterator it;
			//重複ポインタの検査
			for (it = m_ResMap.begin(); it != m_ResMap.end(); it++) {
				if (it->second == ResObj) {
					//ポインタが重複していても、キーが同じなら
					//すでに登録されているのでリターン
					if (it->first == Key) {
						return;
					}
					wstring keyerr = Key;
					throw BaseException(
						L"すでにそのリソースは登録されています",
						keyerr,
						L"App::RegisterResource()"
					);
				}
			}
			//重複キーの検査
			it = m_ResMap.find(Key);
			if (it != m_ResMap.end()) {
				//指定の名前が見つかった
				//例外発生
				wstring keyerr = Key;
				throw BaseException(
					L"すでにそのキーは登録されています",
					keyerr,
					L"App::RegisterResource()"
				);
			}
			else {
				//見つからない
				//リソースペアの追加
				m_ResMap[Key] = ResObj;
			}
		}
		catch (...) {
			throw;
		}
	}

	//テクスチャの登録(同じキーのテクスチャがなければファイル名で作成し、登録)
	//同じ名前のテクスチャがあればそのポインタを返す
	shared_ptr<TextureResource> App::RegisterTexture(const wstring& Key, const wstring& TextureFileName, const wstring& TexType) {
		if (CheckResource<TextureResource>(Key)) {
			return GetResource<TextureResource>(Key);
		}
		//
		auto PtrTexture = TextureResource::CreateTextureResource(TextureFileName, TexType);
		RegisterResource(Key, PtrTexture);
		return PtrTexture;
	}


	//Wavの登録(同じキーのWavがなければファイル名で作成し、登録)
	//同じ名前のWavがあればそのポインタを返す
	shared_ptr<AudioResource> App::RegisterWav(const wstring& Key, const wstring& WavFileName) {
		if (CheckResource<AudioResource>(Key)) {
			return GetResource<AudioResource>(Key);
		}
		//
		auto PtrWav = ObjectFactory::Create<AudioResource>(WavFileName);
		RegisterResource(Key, PtrWav);
		return PtrWav;
	}

	void App::UpdateDraw(unsigned int SyncInterval) {
		if (!m_SceneInterface) {
			//シーンがが無効なら
			throw BaseException(
				L"シーンがありません",
				L"if(!m_SceneInterface)",
				L"App::UpdateDraw()"
			);
		}

		// シーン オブジェクトを更新します。
		m_InputDevice.ResetControlerState();
		m_Timer.Tick([&]()
		{
			m_SceneInterface->OnUpdate();
		});
		// 初回更新前にレンダリングは行わない。
		if (GetFrameCount() == 0)
		{
			return;
		}
		m_SceneInterface->OnDraw();
		// バックバッファからフロントバッファに転送
		m_DeviceResources->Present(SyncInterval,0);
	}


	void App::OnMessage(UINT message, WPARAM wParam, LPARAM lParam) {
	}





}
//end basecross
