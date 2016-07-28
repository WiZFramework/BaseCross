/*!
@file DeviceResources.h
@brief テクスチャ、メッシュ、Dx11デバイスリソース
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/*!
	メモリ上に保持するDx12テクスチャリソース
	*/
	//--------------------------------------------------------------------------------------
	class TextureResource : public BaseResource {
		friend class ObjectFactory;
		TextureResource();
	public:
		virtual ~TextureResource();
		static  shared_ptr<TextureResource> CreateTextureResource(const wstring& FileName, const wstring& TexType = L"WIC");

		//仮想関数
		virtual void OnPreCreate()override {}
		virtual void OnCreate()override {}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	/*!
	@breaf Dx11メッシュクラス.<br />
	メッシュは、スタティック関数を使って生成する
	*/
	//--------------------------------------------------------------------------------------
	class MeshResource : public BaseResource {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf コンストラクタ<br />
		構築はスタティック関数を利用する
		*/
		//--------------------------------------------------------------------------------------
		MeshResource()
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MeshResource() {}

		//仮想関数
		virtual void OnPreCreate()override {}
		virtual void OnCreate()override {}
	};


	//--------------------------------------------------------------------------------------
	/*!
	DirectX11デバイスクラス
	*/
	//--------------------------------------------------------------------------------------
	class DeviceResources {
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@breaf コンストラクタ
		@param[in] hWnd	ウインドウのハンドル
		@param[in] isFullScreen フルスクリーンかどうか
		@param[in] Width		幅
		@param[in] Height		高さ
		*/
		//--------------------------------------------------------------------------------------
		DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		//--------------------------------------------------------------------------------------
		/*!
		@breaf デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DeviceResources();
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンテンツの作成後の処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterInitContents() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief フロントバッファに転送する
		@param[in]	SyncInterval	インターバル
		@param[in]	Flags		フラグ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Present(unsigned int SyncInterval, unsigned int  Flags);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




}
//end basecross
