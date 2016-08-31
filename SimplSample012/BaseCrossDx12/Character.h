/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	Dx12の3D描画用の構造体
	//--------------------------------------------------------------------------------------
	struct Dx12DrawContext3D {
		//ルートシグネチャ
		ComPtr<ID3D12RootSignature> m_RootSignature;
		//デスクプリタハンドルのインクリメントサイズ
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		//デスクプリタヒープ
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		//GPU側デスクプリタのハンドルの配列
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		//CPU側サンプラーデスクプリタのハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE m_SamplerDescriptorHandle;

		//パイプラインステーsと作成用定義
		D3D12_GRAPHICS_PIPELINE_STATE_DESC m_PineLineDesc;
		//パイプラインステート
		ComPtr<ID3D12PipelineState> m_FrontPipelineState;
		ComPtr<ID3D12PipelineState> m_BackPipelineState;

		//コマンドリスト
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		UINT m_SrvDescriptorHeapCount{ 0 };
		UINT m_CbxDescriptorHeapCount{ 0 };
		UINT m_SamplerDescriptorHeapCount{ 0 };

		//コンスタントバッファ
		struct StaticConstantBuffer
		{
			Matrix4X4 World;
			Matrix4X4 View;
			Matrix4X4 Projection;
			Vector4 LightDir;
			Color4 Emissive;
			Color4 Diffuse;
			StaticConstantBuffer() {
				memset(this, 0, sizeof(StaticConstantBuffer));
			};
		};
		//コンスタントバッファのオブジェクト側変数
		StaticConstantBuffer m_StaticConstantBuffer;
		//コンスタントバッファアップロードヒープ
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		//コンスタントバッファのGPU側変数
		void* m_pConstantBuffer{ nullptr };

		//メッシュ
		shared_ptr<MeshResource> m_SphereMesh;
		//テクスチャリソース
		shared_ptr<TextureResource> m_TextureResource;
		//透明処理するかどうか
		bool m_Trace;
		//初期化
		void Init();
		void DrawContext();
	};


	//--------------------------------------------------------------------------------------
	///	球
	//--------------------------------------------------------------------------------------
	class SphereObject : public ObjectInterface, public ShapeInterface {
		//描画用構造体
		Dx12DrawContext3D m_Dx12DrawContext3D;
		UINT m_Division;				///<分割数
		wstring m_TextureFileName;		///<テクスチャファイル名
		Vector3 m_Scale;				///<スケーリング
		Quaternion m_Qt;			///<回転
		Vector3 m_Pos;				///<位置
		bool m_Trace;					///<透明処理するかどうか
		void CreateBuffers();
		void UpdateConstantBuffer();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	Division	分割数
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Trace	透明処理するかどうか
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		SphereObject(UINT Division, const wstring& TextureFileName, bool Trace, const Vector3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SphereObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

}
//end basecross
