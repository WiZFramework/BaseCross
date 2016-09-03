/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	//	class NormalTextureBox  : public ObjectInterface,public ShapeInterface;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	class NormalTextureBox : public ObjectInterface,public ShapeInterface {
		//メッシュ
		shared_ptr<MeshResource> m_CubeMesh;
		//テクスチャ
		shared_ptr<TextureResource> m_WallTex;
		Vector3 m_Scale;
		Vector3 m_Pos;
		Quaternion m_Qt;

		///ルートシグネチャ
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrvのデスクプリタハンドルのインクリメントサイズ
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///デスクプリタヒープ
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU側デスクプリタのハンドルの配列
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;

		//コンスタントバッファ構造体
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
		//コンスタントバッファデータ
		StaticConstantBuffer m_ConstantBufferData;
		///コンスタントバッファアップロードヒープ
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///コンスタントバッファのGPU側変数
		void* m_pConstantBuffer{ nullptr };
		///パイプラインステート
		ComPtr<ID3D12PipelineState> m_PipelineState;
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
		//コンスタントバッファ更新
		void UpdateConstantBuffer();
		///描画処理
		void DrawObject();


	public:
		//構築と破棄
		NormalTextureBox(const Vector3& Pos);
		virtual ~NormalTextureBox();
		//初期化
		virtual void OnCreate()override;
		void OnUpdate()override;
		void OnDraw()override;
	};



}
//end basecross
