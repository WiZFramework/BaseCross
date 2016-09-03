/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	立方体
	//--------------------------------------------------------------------------------------
	class CubeObject : public ObjectInterface, public ShapeInterface {
		//メッシュ
		shared_ptr<MeshResource> m_CubeMesh;
		Vector3 m_Scale;				///<スケーリング
		Quaternion m_Qt;			///<回転
		Vector3 m_Pos;				///<位置

		///ルートシグネチャ
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrvのデスクプリタハンドルのインクリメントサイズ
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///デスクプリタヒープ
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		///GPU側デスクプリタのハンドルの配列
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		// コンスタントバッファ
		struct StaticConstantBuffer
		{
			Matrix4X4 World;
			Matrix4X4 View;
			Matrix4X4 Projection;
			Color4 Emissive;
			StaticConstantBuffer() {
				memset(this, 0, sizeof(StaticConstantBuffer));
			};
		};
		StaticConstantBuffer m_StaticConstantBuffer;
		///コンスタントバッファアップロードヒープ
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///コンスタントバッファのGPU側変数
		void* m_pConstantBuffer{ nullptr };
		//パイプラインステート
		ComPtr<ID3D12PipelineState> m_PipelineState;
		///コマンドリスト
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;

		//頂点データの作成
		void CreateBuffers();
		///ルートシグネチャ作成
		void CreateRootSignature();
		///デスクプリタヒープ作成
		void CreateDescriptorHeap();
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		CubeObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CubeObject();
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
