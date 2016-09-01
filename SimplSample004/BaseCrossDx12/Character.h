/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	四角形スプライトの構造体
	//--------------------------------------------------------------------------------------
	struct SquareSprite {
		Vector2 m_LocalScale;		///<ローカルスケーリング
		float m_LocalRot;				///<ローカル回転角度
		float m_LocalRotVelocity;		///<ローカル回転速度
		Vector2 m_LocalPos;				///<ローカル位置
		Vector2 m_LocalVelocity;		///<ローカル速度
		Vector2 m_LocalGravityVelocity;		///<ローカル加速速度
		///CbvSrvのデスクプリタハンドルのインクリメントサイズ
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///デスクプリタヒープ
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		///GPU側デスクプリタのハンドルの配列
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
		///コンスタントバッファアップロードヒープ
		ComPtr<ID3D12Resource> m_ConstantBufferUploadHeap;
		///コンスタントバッファのGPU側変数
		void* m_pConstantBuffer{ nullptr };
		///コンスタントバッファ
		struct SpriteConstantBuffer
		{
			Matrix4X4 World;
			Color4 Emissive;
			SpriteConstantBuffer() {
				memset(this, 0, sizeof(SpriteConstantBuffer));
			};
		};
		///コンスタントバッファデータ
		SpriteConstantBuffer m_SpriteConstantBuffer;
		SquareSprite() :
			m_LocalScale(64.0f, 64.0f),
			m_LocalRot(0),
			m_LocalRotVelocity(0),
			m_LocalPos(0, 0),
			m_LocalVelocity(0, 0),
			m_LocalGravityVelocity(0, 0)
		{
		}
		///デスクプリタヒープ作成
		void CreateDescriptorHeap();
		///コンスタントバッファ作成
		void CreateConstantBuffer();
		//コンスタントバッファ更新
		void UpdateConstantBuffer();
	};

	//--------------------------------------------------------------------------------------
	///	四角形スプライトのグループ
	//--------------------------------------------------------------------------------------
	class SquareSpriteGroup : public ObjectInterface, public ShapeInterface {
		//メッシュ
		shared_ptr<MeshResource> m_SquareSpriteMesh;
		vector<SquareSprite> m_SquareSpriteVec;
		float m_TotalTime;
		//各オブジェクトの位置等の変更
		void UpdateObjects(float ElapsedTime);
		//コンスタントバッファ更新
		void UpdateConstantBuffer();
		///ルートシグネチャ
		ComPtr<ID3D12RootSignature> m_RootSignature;
		//パイプラインステート
		ComPtr<ID3D12PipelineState> m_PipelineState;
		///コマンドリスト
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
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
		///描画処理
		void DrawObject();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		SquareSpriteGroup();
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareSpriteGroup();
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
