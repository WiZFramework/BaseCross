/*!
@file App.h
@brief アプリケーションクラス。入力機器等
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	class DeviceResources;
	class TextureResource;
	class AudioResource;


	//--------------------------------------------------------------------------------------
	///	オーディオマネージャクラス
	//--------------------------------------------------------------------------------------
	class AudioManager {
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		AudioManager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~AudioManager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief オーディオの初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CreateDeviceIndependentResources();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ミュージックエンジンの取得
		@return	IXAudio2のポインタ
		*/
		//--------------------------------------------------------------------------------------
		IXAudio2* GetMusicEngine()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief サウンドエフェクトエンジンの取得
		@return	IXAudio2のポインタ
		*/
		//--------------------------------------------------------------------------------------
		IXAudio2* GetSoundEffectEngine()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief オーディオの中断
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SuspendAudio();
		//--------------------------------------------------------------------------------------
		/*!
		@brief オーディオの再開
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ResumeAudio();
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		AudioManager(const AudioManager&) = delete;
		AudioManager& operator=(const AudioManager&) = delete;
		//ムーブ禁止
		AudioManager(const AudioManager&&) = delete;
		AudioManager& operator=(const AudioManager&&) = delete;
	};


	//--------------------------------------------------------------------------------------
	///	オーディオリソース（wav）クラス
	//--------------------------------------------------------------------------------------
	class AudioResource : public BaseResource {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ<br />
		ウェブファイル名を渡す
		@param[in]	FileName	ファイル名
		*/
		//--------------------------------------------------------------------------------------
		AudioResource(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~AudioResource();
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	サウンドデータの取得
		@return	このウェブリソースのサウンドデータ
		*/
		//--------------------------------------------------------------------------------------
		const vector<byte>& GetSoundData()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ウェブフォーマットの取得
		@return	このウェブリソースのWAVEFORMATEXウェブフォーマットのポインタ
		*/
		//--------------------------------------------------------------------------------------
		WAVEFORMATEX*  GetOutputWaveFormatEx() const;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	struct BackupDataBase {
		BackupDataBase() {}
		virtual ~BackupDataBase() {}

	};
	template<typename T>
	struct BackupData : public BackupDataBase {
		vector<T> m_Vertices;
		vector<uint16_t> m_Indices;
	};

	//--------------------------------------------------------------------------------------
	///	VertexPositionNormalTexture読み込み用構造体
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalTexturePOD {
		float position[3];	///< 位置情報
		float normal[3];	///< 法線
		float textureCoordinate[2];	///< テクスチャUV
	};
	//--------------------------------------------------------------------------------------
	///	VertexPositionNormalTextureSkinning読み込み用構造体
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalTextureSkinningPOD {
		float position[3];	///< 位置情報
		float normal[3];	///< 法線
		float textureCoordinate[2];	///< テクスチャUV
		uint32_t indices[4];	///< ボーンのインデックス
		float weights[4];	///< 各ボーンのウエイト
	};



	//--------------------------------------------------------------------------------------
	///	マテリアル構造体
	//--------------------------------------------------------------------------------------
	struct MaterialEx {
		///開始インデックス
		UINT m_StartIndex;
		///描画インデックスカウント
		UINT m_IndexCount;
		/// デフィーズ（物体の色）
		Color4 m_Diffuse;
		/// スペキュラー（反射光）
		Color4 m_Specular;
		/// アンビエント（環境色）
		Color4 m_Ambient;
		/// エミッシブ（放射光）
		Color4 m_Emissive;
		/// シェーダリソースビュー（テクスチャリソース）
		shared_ptr<TextureResource> m_TextureResource;
	};

	//--------------------------------------------------------------------------------------
	///	マテリアル読み込み用構造体
	//--------------------------------------------------------------------------------------
	struct MaterialExPOD {
		///開始インデックス
		UINT m_StartIndex;
		///描画インデックスカウント
		UINT m_IndexCount;
		/// デフィーズ（物体の色）
		float m_Diffuse[4];
		/// スペキュラー（反射光）
		float m_Specular[4];
		/// アンビエント（環境色）
		float m_Ambient[4];
		/// エミッシブ（放射光）
		float m_Emissive[4];
	};

	//--------------------------------------------------------------------------------------
	///	行列読み込み用構造体
	//--------------------------------------------------------------------------------------
	struct	MatrixPOD
	{
		float	m_Mat[4][4];	///>行列
	};



	//--------------------------------------------------------------------------------------
	///	ブロックタイプ定義（モデルメッシュ読み込み用）
	//--------------------------------------------------------------------------------------
	enum class BlockType {
		Vertex,	///< 頂点
		Index,	///< インデックス
		Material,	///< マテリアル
		MaterialCount,	///< マテリアル数
		SkinedVertex,	///< スキンぐぎゅれつ
		BoneCount,	///< ボーン数
		AnimeMatrix,	///< アニメーション行列
		End = 100	///< 終了
	};

	//--------------------------------------------------------------------------------------
	///	ブロックヘッダ構造体
	//--------------------------------------------------------------------------------------
	struct BlockHeader {
		BlockType m_Type;	///< ブロックタイプ
		UINT m_Size;	///< バイト数
	};


	//--------------------------------------------------------------------------------------
	/// XBoxコントローラーのステータス。XINPUT_STATE派生
	//--------------------------------------------------------------------------------------
	struct CONTROLER_STATE : public XINPUT_STATE
	{

		WORD wButtons;		///< ボタン状態
		BYTE bLeftTrigger;	///< 左トリガ
		BYTE bRightTrigger;	///< 右トリガ

		float fThumbRX;	///< 右スティックXのflort変換
		float fThumbRY;	///< 右スティックYのflort変換
		float fThumbLX;	///< 左スティックXのflort変換
		float fThumbLY;	///< 左スティックYのflort変換
		WORD wNowUpdateButtons;	///< 状態が変わったときにフラグが立つボタン
		WORD wPressedButtons;	///< 押された瞬間を所持するボタン
		WORD wReleasedButtons;	///< 離された瞬間を所持するボタン
		WORD wLastButtons;	///< 一つ前のボタン
		bool bConnected;	///< 接続しているかどうか
	};
	//--------------------------------------------------------------------------------------
	///キーボードのステータス。マウスのステータスも取得する
	//--------------------------------------------------------------------------------------
	struct KEYBOARD_STATE {
		static const DWORD MAX_KEYVCODE = 256; ///< キー数上限
		bool m_bPushKeyTbl[MAX_KEYVCODE];	///< 現在のキーボードの状態
		bool m_bLastKeyTbl[MAX_KEYVCODE];	///< 一つ前のキーボードの状態
		bool m_bPressedKeyTbl[MAX_KEYVCODE];	///< 押された瞬間のキーボード
		bool m_bUpKeyTbl[MAX_KEYVCODE];		///< 離された瞬間のキーボード
		bool m_KeyMessageActive;	///<何かのキーイベントが発生
		Point2D<int> m_MouseClientPoint; ///< クライアント上のマウスポイント
										 //--------------------------------------------------------------------------------------
										 /*!
										 @brief	コンストラクタ
										 */
										 //--------------------------------------------------------------------------------------
		KEYBOARD_STATE() :
			m_KeyMessageActive{ false },
			m_MouseClientPoint{ 0, 0 }
		{
			//キーボードテーブルの初期化
			::ZeroMemory(&m_bLastKeyTbl, sizeof(m_bLastKeyTbl));
			::ZeroMemory(&m_bPressedKeyTbl, sizeof(m_bPressedKeyTbl));
			::ZeroMemory(&m_bPushKeyTbl, sizeof(m_bPushKeyTbl));
			::ZeroMemory(&m_bUpKeyTbl, sizeof(m_bUpKeyTbl));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マウス使用でマウスがクライアント領域にあるかどうかのチェック
		@param[in]	hWnd	ウインドウのハンドル。
		@param[in]	UseKeyVec	使用するキーコード配列
		@return マウス使用でマウスがクライアント領域にあればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsMouseEnabled(HWND hWnd, vector<DWORD>& UseKeyVec) {
			//マウスポイントは0,0に初期化
			m_MouseClientPoint = { 0,0 };
			//マウス利用にかかわらず、マウスポインタ座標は設定
			POINT p;
			::GetCursorPos(&p);
			if (::ScreenToClient(hWnd, &p)) {
				//クライアント座標に変換できたときのみ、マウス座標を設定
				m_MouseClientPoint = { p.x, p.y };
			}
			else {
				return false;
			}

			//UseKeyVecにマウスがあった場合
			vector<DWORD> MouseTemp = { VK_LBUTTON, VK_RBUTTON, VK_MBUTTON };
			bool MouseChk = false;
			for (auto chk : MouseTemp) {
				auto it = find(UseKeyVec.begin(), UseKeyVec.end(), chk);
				if (it != UseKeyVec.end()) {
					MouseChk = true;
					break;
				}
			}
			//マウス使用で座標がクライアント領域内ならtrue
			if (MouseChk) {
				RECT rc;
				::GetClientRect(hWnd, &rc);
				if (::PtInRect(&rc, p)) {
					return true;
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	キーステートを得る
		@param[in]	hWnd	ウインドウのハンドル。Escキー入力時にメッセージを送るため
		@param[in]	UseKeyVec	使用するキーコード配列
		@return キー入力があればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetKeyState(HWND hWnd, vector<DWORD>& UseKeyVec) {
			m_KeyMessageActive = false;
			//一つ前にコピー
			CopyMemory(m_bLastKeyTbl, m_bPushKeyTbl, sizeof(m_bLastKeyTbl));
			//キーボードの状態を初期化
			::ZeroMemory(&m_bPushKeyTbl, sizeof(m_bPushKeyTbl));
			::ZeroMemory(&m_bPressedKeyTbl, sizeof(m_bPressedKeyTbl));
			::ZeroMemory(&m_bUpKeyTbl, sizeof(m_bUpKeyTbl));
			//マウスのチェック
			bool MouseEnabled = IsMouseEnabled(hWnd, UseKeyVec);
			//自分自身にフォーカスがない場合はfalse
			if (::GetFocus() != hWnd) {
				return false;
			}
			SHORT ret = GetAsyncKeyState((int)VK_ESCAPE);
			if (ret & 0x8000) {
				//Escキーは無条件にfalse
				return false;
			}
			size_t sz = UseKeyVec.size();
			for (size_t i = 0; i < sz; i++) {
				ret = GetAsyncKeyState((int)UseKeyVec[i]);
				if (UseKeyVec[i] == VK_LBUTTON || UseKeyVec[i] == VK_RBUTTON || UseKeyVec[i] == VK_MBUTTON) {
					if (!MouseEnabled) {
						//マウスが無効ならUsedに入っていても無視
						continue;
					}
				}
				if (ret & 0x8000) {
					m_bPushKeyTbl[UseKeyVec[i]] = true;
					m_KeyMessageActive = true;	//メッセージをゲームが受け取る
					if (!m_bLastKeyTbl[UseKeyVec[i]]) {
						//前回押されてなくて今回押された
						m_bPressedKeyTbl[UseKeyVec[i]] = true;
					}
				}
				else {
					//キーは押されてない
					if (m_bLastKeyTbl[UseKeyVec[i]]) {
						//前回押されていて今回押されてない
						m_bUpKeyTbl[UseKeyVec[i]] = true;
						m_KeyMessageActive = true;	//メッセージをゲームが受け取る
					}
				}
			}
			return m_KeyMessageActive;
		}
	};

	//--------------------------------------------------------------------------------------
	///	入力機器
	//--------------------------------------------------------------------------------------
	class InputDevice {
		static const DWORD MAX_CONTROLLERS = 4; // コントローラ数
		vector<CONTROLER_STATE> m_State; // コントローラステートの配列
		KEYBOARD_STATE m_KeyState; // キーボードステート
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		InputDevice() :
			m_State(MAX_CONTROLLERS),
			m_KeyState()
		{
			for (DWORD i = 0; i < MAX_CONTROLLERS; i++) {
				::ZeroMemory(&m_State[i], sizeof(CONTROLER_STATE));
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		~InputDevice() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief マウスとキーボードの状態を得る
		@param[in]	hWnd	ウインドウのハンドル。Escキー入力時にメッセージを送るため
		@param[in]	UseKeyVec	使用するキー
		@return	マウスかキーボードメッセージがあればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool ResetInputState(HWND hWnd, vector<DWORD>& UseKeyVec) {
			bool Ret1 = m_KeyState.GetKeyState(hWnd, UseKeyVec);
			return Ret1;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief コントローラーの状態をm_Stateに取得する
		*/
		//--------------------------------------------------------------------------------------
		void ResetControlerState() {
			DWORD dwResult;
			for (DWORD i = 0; i < MAX_CONTROLLERS; i++)
			{
				//一つ前の状態を保存
				m_State[i].wLastButtons = m_State[i].Gamepad.wButtons;
				// XInputから現在のステータスを得る
				XINPUT_STATE State;
				dwResult = XInputGetState(i, &State);
				if (dwResult == ERROR_SUCCESS) {
					m_State[i].dwPacketNumber = State.dwPacketNumber;
					m_State[i].Gamepad = State.Gamepad;
					m_State[i].bConnected = true;
					//ボタンの設定
					//コピー
					m_State[i].wButtons = m_State[i].Gamepad.wButtons;
					//更新ボタン
					m_State[i].wNowUpdateButtons = m_State[i].Gamepad.wButtons;
					m_State[i].bLeftTrigger = m_State[i].Gamepad.bLeftTrigger;
					m_State[i].bRightTrigger = m_State[i].Gamepad.bRightTrigger;
					//前回より変化しているボタンのみフラグを立てる
					//XOR演算により実装
					m_State[i].wNowUpdateButtons ^= m_State[i].wLastButtons;
					//押された瞬間をセット
					m_State[i].wPressedButtons = m_State[i].wNowUpdateButtons & m_State[i].wButtons;
					//離された瞬間をセット
					m_State[i].wReleasedButtons = m_State[i].wNowUpdateButtons & (m_State[i].wButtons ^ 0xffff);
					//レバーのデッドゾーンの設定
					if ((m_State[i].Gamepad.sThumbLX < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
						m_State[i].Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
						(m_State[i].Gamepad.sThumbLY < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE &&
							m_State[i].Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
					{
						m_State[i].Gamepad.sThumbLX = 0;
						m_State[i].Gamepad.sThumbLY = 0;
					}
					if (m_State[i].Gamepad.sThumbLX >= 0) {
						m_State[i].fThumbLX = (float)m_State[i].Gamepad.sThumbLX / (float)SHRT_MAX;
					}
					else {
						m_State[i].fThumbLX = (float)m_State[i].Gamepad.sThumbLX / (float)(SHRT_MAX + 1);
					}
					if (m_State[i].Gamepad.sThumbLY >= 0) {
						m_State[i].fThumbLY = (float)m_State[i].Gamepad.sThumbLY / (float)SHRT_MAX;
					}
					else {
						m_State[i].fThumbLY = (float)m_State[i].Gamepad.sThumbLY / (float)(SHRT_MAX + 1);
					}

					if ((m_State[i].Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
						m_State[i].Gamepad.sThumbRX > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE) &&
						(m_State[i].Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
							m_State[i].Gamepad.sThumbRY > -XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE))
					{
						m_State[i].Gamepad.sThumbRX = 0;
						m_State[i].Gamepad.sThumbRY = 0;
					}
					if (m_State[i].Gamepad.sThumbRX >= 0) {
						m_State[i].fThumbRX = (float)m_State[i].Gamepad.sThumbRX / (float)SHRT_MAX;
					}
					else {
						m_State[i].fThumbRX = (float)m_State[i].Gamepad.sThumbRX / (float)(SHRT_MAX + 1);
					}
					if (m_State[i].Gamepad.sThumbRY >= 0) {
						m_State[i].fThumbRY = (float)m_State[i].Gamepad.sThumbRY / (float)SHRT_MAX;
					}
					else {
						m_State[i].fThumbRY = (float)m_State[i].Gamepad.sThumbRY / (float)(SHRT_MAX + 1);
					}
				}
				else {
					m_State[i].bConnected = false;
				}
			}
		}
		//	アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief キーボードステートの取得
		@return	キーボードステート
		*/
		//--------------------------------------------------------------------------------------
		const KEYBOARD_STATE& GetKeyState() const { return m_KeyState; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief コントローラステートの取得
		@return	コントローラステート
		*/
		//--------------------------------------------------------------------------------------
		const vector<CONTROLER_STATE>& GetControlerVec()const { return m_State; }
	};


	//--------------------------------------------------------------------------------------
	///	アプリケーションクラス
	//--------------------------------------------------------------------------------------
	class App {
		// デリーター
		struct AppDeleter
		{
			void operator()(App *p) { delete p; }
		};
		static unique_ptr<App, AppDeleter> m_App;		// Singletonで利用する自分自身のポインタ
		HINSTANCE m_hInstance;							// モジュールのインスタンス
		HWND m_hWnd;									// メインウインドウのハンドル
		bool m_FullScreen;								// フルスクリーンかどうか
		UINT m_GameWidth;								// ゲーム盤幅(ピクセル)
		UINT m_GameHeight;								// ゲーム盤高さ(ピクセル)
		shared_ptr<DeviceResources> m_DeviceResources;	// デバイス
		shared_ptr<SceneInterface> m_SceneInterface;				// シーン

		map<wstring, shared_ptr<BaseResource> > m_ResMap;		// キーとリソースを結び付けるマップ
		StepTimer m_Timer;										// タイマー
		InputDevice m_InputDevice;					// 入力機器
		unique_ptr<AudioManager> m_AudioManager;	// オーディオマネージャ
		App(HINSTANCE hInstance, HWND hWnd, bool FullScreen, UINT Width, UINT Height);
		virtual ~App() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief シングルトン構築とアクセサ（構築時はすべてのパラメータを設定する）
		@param[in]	hInstance = 0	インスタンス
		@param[in]	hWnd = 0		ウインドウのハンドル
		@param[in]	FullScreen = false	フルスクリーンかどうか
		@param[in]	Width = 0	幅
		@param[in]	Height = 0	高さ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static unique_ptr<App, AppDeleter>& GetApp(HINSTANCE hInstance = 0, HWND hWnd = 0,
			bool FullScreen = false, UINT Width = 0, UINT Height = 0);
		//--------------------------------------------------------------------------------------
		/*!
		@brief Appが作成されてるかチェック
		@return	作成されていればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool AppCheck();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 強制破棄
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void DeleteApp();
		//--------------------------------------------------------------------------------------
		/*!
		@brief オーディオマネージャの取得
		@return	オーディオマネージャ
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr<AudioManager>& GetAudioManager();
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief アプリケーションのインスタンス取得
		@return	アプリケーションのインスタンス
		*/
		//--------------------------------------------------------------------------------------
		HINSTANCE GetHInstance() const { return m_hInstance; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief ウインドウのハンドルの取得
		@return	ウインドウのハンドル
		*/
		//--------------------------------------------------------------------------------------
		HWND GetHWnd() const { return m_hWnd; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief フルスクリーンかどうか
		@return	フルスクリーンならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsFullScreen() const { return m_FullScreen; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief フルスクリーンかどうか
		@return	フルスクリーンならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetFullScreen() const { return m_FullScreen; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief ゲーム盤の幅の取得
		@return	ゲーム盤の幅
		*/
		//--------------------------------------------------------------------------------------
		UINT GetGameWidth() const { return m_GameWidth; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief ゲーム盤の幅の取得
		@return	ゲーム盤の幅
		*/
		//--------------------------------------------------------------------------------------
		UINT GetGameHeight() const { return m_GameHeight; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief デバイスリソースの取得
		@return	デバイスリソース
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<DeviceResources> GetDeviceResources() const { return m_DeviceResources; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief シーンインターフェイスの取得
		@return	シーンインターフェイス
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<SceneInterface> GetSceneInterface() const { return m_SceneInterface; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief シーンインターフェイスの設定
		@param[in]	ptr	シーンインターフェイス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetSceneInterface(const shared_ptr<SceneInterface>& ptr) { m_SceneInterface = ptr; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief タイマーの取得(設定アクセサはない)
		@return	タイマー
		*/
		//--------------------------------------------------------------------------------------
		const StepTimer& GetStepTimer() const { return m_Timer; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief 前回のターンからの経過時間を得る
		@return	前回のターンからの経過時間
		*/
		//--------------------------------------------------------------------------------------
		float GetElapsedTime() {
			double d = m_Timer.GetElapsedSeconds();
			//0除算回避のため1000分の1未満は返らないようにする
			if (d < 0.001) {
				d = 0.001;
			}
			return (float)d;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 入力機器の取得
		@return	入力機器
		*/
		//--------------------------------------------------------------------------------------
		const InputDevice& GetInputDevice() const { return m_InputDevice; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief マウスとキーボードの状態を得る
		@param[in]	hWnd	ウインドウのハンドル。Escキー入力時にメッセージを送るため
		@param[in]	UseKeyVec	使用するキー
		@return	マウスかキーボードメッセージがあればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool ResetInputState(HWND hWnd, vector<DWORD>& UseKeyVec) {
			if (UseKeyVec.size() == 0) {
				return false;
			}
			return m_InputDevice.ResetInputState(hWnd, UseKeyVec);
		}
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief  シーンの作成
		@tparam	T	シーンクラス
		@tparam Ts...	可変長パラメータ型
		@return	シーン
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> CreateScene(Ts&&... params) {
			try {
				//シーンの作成
				auto Ptr = ObjectFactory::Create<T>(params...);
				m_SceneInterface = Ptr;
				return Ptr;
			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief シーンの取得
		@tparam	T	シーンクラス
		@return	シーン
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetScene() const {
			try {
				auto Ptr = dynamic_pointer_cast<T>(m_SceneInterface);
				if (!Ptr) {
					wstring str = L"m_SceneInterfaceを";
					str += Util::GetWSTypeName<T>();
					str += L"型のシーンに型変換できません";
					throw BaseException(
						str,
						L"if(!dynamic_pointer_cast<T>(m_SceneInterface))",
						L"App::GetScene<T>()"
					);

				}
				return Ptr;
			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief シーンの更新と描画
		@param[in]	SyncInterval	インターバル値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateDraw(unsigned int SyncInterval);
		//!フレームカウントを得る
		//--------------------------------------------------------------------------------------
		/*!
		@brief フレームカウントを得る
		@return	フレームカウント
		*/
		//--------------------------------------------------------------------------------------
		unsigned int GetFrameCount() {
			return (unsigned int)m_Timer.GetFrameCount();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief リソースの登録（重複があれば何もしないが、矛盾があれば例外）
		@param[in]	Key リソースキー
		@param[in]	ResObj リソース
		@return	なし（矛盾があれば例外）
		*/
		//--------------------------------------------------------------------------------------
		void RegisterResource(const wstring& Key, const shared_ptr<BaseResource>& ResObj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャの登録(同じキーのテクスチャがなければファイル名で作成し、登録)。<br />
		同じ名前のテクスチャがあればそのポインタを返す
		@param[in]	Key	リソースキー
		@param[in]	TextureFileName テクスチャファイル名
		@param[in]	TexType = L"WIC" テクスチャタイプ（デフォルトWIC）
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<TextureResource> RegisterTexture(const wstring& Key,
			const wstring& TextureFileName, const wstring& TexType = L"WIC");
		//--------------------------------------------------------------------------------------
		/*!
		@brief Wavリソースの登録(同じキーのWavリソースがなければファイル名で作成し、登録)。<br />
		同じ名前のWavリソースがあればそのポインタを返す
		@param[in]	Key リソースキー
		@param[in]	WavFileName テクスチャファイル名
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<AudioResource> RegisterWav(const wstring& Key, const wstring& WavFileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief リソースの確認
		@tparam	T 確認するリソース型
		@param[in]	Key リソースキー
		@return	そのキーのリソースがあればtrue、無ければfalse（見つかっても指定の型でなければ例外）
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		bool CheckResource(const wstring& Key) const {
			if (Key == L"") {
				throw BaseException(
					L"キーが空白です",
					L"if(Key == L\"\")",
					L"App::CheckResource()"
				);
			}
			map<wstring, shared_ptr<BaseResource> >::const_iterator it;
			it = m_ResMap.find(Key);
			if (it != m_ResMap.end()) {
				//指定の名前が見つかった
				shared_ptr<T> pT = dynamic_pointer_cast<T>(it->second);
				if (pT) {
					return true;
				}
				else {
					wstring keyerr = Key;
					wstring str = L"指定のキーは";
					str += Util::GetWSTypeName<T>();
					str += L"*型に変換できません";
					throw BaseException(
						str,
						keyerr,
						L"App::CheckResource()"
					);
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief キーの確認
		@param[in]	Key リソースキー
		@return	そのキーが存在すればtrue。リソースのポインタを返すわけではない。キーの重複のみチェック
		*/
		//--------------------------------------------------------------------------------------
		bool CheckResourceKey(const wstring& Key) const {
			if (Key == L"") {
				throw BaseException(
					L"キーが空白です",
					L"if(Key == L\"\")",
					L"App::CheckResourceKey()"
				);
			}
			map<wstring, shared_ptr<BaseResource> >::const_iterator it;
			it = m_ResMap.find(Key);
			if (it != m_ResMap.end()) {
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief リソースの取得
		@tparam	T 取得するリソース型
		@param[in]	Key リソースキー
		@return	リソースのポインタ（登録がなければ例外）
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		shared_ptr<T> GetResource(const wstring& Key) const {
			if (Key == L"") {
				throw BaseException(
					L"キーが空白です",
					L"if(Key == L\"\")",
					L"App::GetResource()"
				);
			}
			map<wstring, shared_ptr<BaseResource> >::const_iterator it;
			it = m_ResMap.find(Key);
			if (it != m_ResMap.end()) {
				//指定の名前が見つかった
				shared_ptr<T> pT = dynamic_pointer_cast<T>(it->second);
				if (pT) {
					return pT;
				}
				else {
					wstring keyerr = Key;
					wstring str = L"指定のキーは";
					str += Util::GetWSTypeName<T>();
					str += L"*型に変換できません";
					throw BaseException(
						str,
						keyerr,
						L"App::GetResource()"
					);
				}
			}
			else {
				//見つからない
				wstring keyerr = Key;
				throw BaseException(
					L"指定のキーは存在しません",
					keyerr,
					L"App::GetResource()"
				);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief リソースキーの取得
		@tparam	T 取得するリソース型
		@param[in]	Ptr リソースのポインタ
		@return	リソースキー（登録がなければ例外）
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		const wstring& GetResourceKey(const shared_ptr<T>& Ptr) const {
			map<wstring, shared_ptr<BaseResource> >::const_iterator it;
			for (it = m_ResMap.begin(); it != m_ResMap.end(); it++) {
				if (it->second == static_pointer_cast<BaseResource>(Ptr)) {
					return it->first;
				}
			}
			//見つからない
			wstring str = Util::GetWSTypeName<T>();
			throw BaseException(
				L"指定のリソースは存在しません。",
				str,
				L"App::GetResourceKey()"
			);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief リソースの解放
		@tparam	T 解放するリソース型
		@param[in]	Key リソースキー
		@return	なし（登録がなければ例外）
		*/
		//--------------------------------------------------------------------------------------
		template <typename T>
		void UnRegisterResource(const wstring& Key) {
			try {
				//以下に失敗したら例外が投げられる
				shared_ptr<T> p = GetResource<T>(Key);
				m_ResMap.erase(Key);
				p.reset();

			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief データディレクトリの取得
		@param[out]	Dir 取得する文字列
		@return	なし（Dirに相対パスが入る）
		*/
		//--------------------------------------------------------------------------------------
		void GetDataDirectory(wstring& Dir) {
			Dir = m_wstrRelativeDataPath;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief シェーダディレクトリの取得
		@param[out]	Dir 取得する文字列
		@return	なし（Dirに相対パスが入る）
		*/
		//--------------------------------------------------------------------------------------
		void GetShadersDirectory(wstring& Dir) {
			Dir = m_wstrRelativeShadersPath;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ウインドウメッセージ
		@param[in]	message	メッセージ
		@param[in]	wParam	第1パラメータ
		@param[in]	lParam	第2パラメータ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void OnMessage(UINT message, WPARAM wParam, LPARAM lParam);
		//--------------------------------------------------------------------------------------
		//	公開変数
		//--------------------------------------------------------------------------------------
		wstring		m_wstrModulePath;		///< モジュール名フルパス
		wstring		m_wstrDir;				///< モジュールがあるディレクトリ
		wstring		m_wstrDataPath;			///< 絶対パスのメディアディレクトリ
		wstring		m_wstrShadersPath;		///< 絶対パスのシェーダディレクトリ
		wstring		m_wstrRelativeDataPath;	///< 相対パスのメディアディレクトリ
		wstring		m_wstrRelativeShadersPath;	///< 相対パスのシェーダディレクトリ
		map<wstring, wstring> m_ConfigMap;		///< 汎用マップ（各アプリケションで使用できる）
	private:
		//コピー禁止
		App(const App&) = delete;
		App& operator=(const App&) = delete;
		//ムーブ禁止
		App(const App&&) = delete;
		App& operator=(const App&&) = delete;

	};





}
//end basecross

