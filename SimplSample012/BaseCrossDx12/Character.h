/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	球
	//--------------------------------------------------------------------------------------
	class SphereObject : public ObjectInterface, public ShapeInterface {
		//メッシュ
		shared_ptr<MeshResource> m_SphereMesh;
		//テクスチャリソース
		shared_ptr<TextureResource> m_TextureResource;

		UINT m_Division;				///<分割数
		wstring m_TextureFileName;		///<テクスチャファイル名
		Vec3 m_Scale;				///<スケーリング
		Quat m_Qt;			///<回転
		Vec3 m_Pos;				///<位置
		bool m_Trace;					///<透明処理するかどうか
		void CreateBuffers();
		void UpdateConstantBuffer();

		///ルートシグネチャ
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrvのデスクプリタハンドルのインクリメントサイズ
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///デスクプリタヒープ
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU側デスクプリタのハンドルの配列
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;

		///コンスタントバッファ
		struct StaticConstantBuffer
		{
			Mat4x4 World;
			Mat4x4 View;
			Mat4x4 Projection;
			Vec4 LightDir;
			Col4 Emissive;
			Col4 Diffuse;
			StaticConstantBuffer() {
				memset(this, 0, sizeof(StaticConstantBuffer));
			};
		};
		///コンスタントバッファのオブジェクト側変数
		StaticConstantBuffer m_StaticConstantBuffer;
		///コンスタントバッファアップロードヒープ
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///コンスタントバッファのGPU側変数
		void* m_pConstantBuffer{ nullptr };
		///パイプラインステート
		ComPtr<ID3D12PipelineState> m_CullBackPipelineState;
		ComPtr<ID3D12PipelineState> m_CullFrontPipelineState;
		///コマンドリスト
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;


		///各初期化関数
		///ルートシグネチャ作成
		void CreateRootSignature();
		///デスクプリタヒープ作成
		void CreateDescriptorHeap();
		///サンプラー作成
		void CreateSampler();
		///シェーダーリソースビュー作成
		void CreateShaderResourceView();
		///コンスタントバッファ作成
		void CreateConstantBuffer();
		///パイプラインステート作成
		void CreatePipelineState();
		///コマンドリスト作成
		void CreateCommandList();

		///描画処理
		void DrawObject();


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
		SphereObject(UINT Division, const wstring& TextureFileName, bool Trace, const Vec3& Pos);
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
