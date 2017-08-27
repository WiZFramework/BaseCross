/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//PNTStatic
	struct PNTStaticConstantBuffer
	{
		Mat4x4 World;
		Mat4x4 View;
		Mat4x4 Projection;
		Vec4 LightDir;
		Col4 Emissive;
		Col4 Diffuse;
		PNTStaticConstantBuffer() {
			memset(this, 0, sizeof(PNTStaticConstantBuffer));
		};
	};



	//--------------------------------------------------------------------------------------
	///	立方体
	//--------------------------------------------------------------------------------------
	struct CubeObject {
		Vec3 m_Scale;			///<スケーリング
		Quat m_Quaternion;	///<回転
		Vec3 m_QuaternionRot;	///<回転軸
		float m_QuaternionVelocity;	///<回転速度
		Vec3 m_Posision;				///<位置
		Vec3 m_Velocity;			///<速度
		CubeObject() {}
		void Refresh() {
			m_Scale = Vec3(0.1f, 0.1f, 0.1f);
			m_QuaternionRot = Vec3(Util::RandZeroToOne() - 0.5f, 1.0f, Util::RandZeroToOne() - 0.5f);
			m_Quaternion.rotation(m_QuaternionRot, Util::RandZeroToOne());
			m_QuaternionVelocity = (Util::RandZeroToOne() - 0.5f) * 10.0f;
			m_Posision = Vec3(Util::RandZeroToOne() - 0.5f, Util::RandZeroToOne(), Util::RandZeroToOne());
			m_Velocity = Vec3(Util::RandZeroToOne() - 0.5f, Util::RandZeroToOne() - 0.5f, Util::RandZeroToOne() - 0.5f);
			m_Velocity *= 2.0f;
		}
	};


	//--------------------------------------------------------------------------------------
	///	立方体グループ
	//--------------------------------------------------------------------------------------
	class CubeObjectGroup : public ObjectInterface, public ShapeInterface {
		//メッシュ
		shared_ptr<MeshResource> m_CubeMesh;
		//マテリアルメッシュ
		shared_ptr<MeshResource> m_InstanceMatrixMesh;

		wstring m_TextureFileName;		///<テクスチャファイル名
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		void CreateBuffers();	///<バッファの作成
		const size_t m_MaxInstance;				///<インスタンス最大値
		vector<CubeObject> m_CubeObjectVec;		///<立方体のインスタンス描画配列



		///ルートシグネチャ
		ComPtr<ID3D12RootSignature> m_RootSignature;
		///CbvSrvのデスクプリタハンドルのインクリメントサイズ
		UINT m_CbvSrvDescriptorHandleIncrementSize{ 0 };
		///デスクプリタヒープ
		ComPtr<ID3D12DescriptorHeap> m_CbvSrvUavDescriptorHeap;
		ComPtr<ID3D12DescriptorHeap> m_SamplerDescriptorHeap;
		///GPU側デスクプリタのハンドルの配列
		vector<CD3DX12_GPU_DESCRIPTOR_HANDLE> m_GPUDescriptorHandleVec;
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	TextureFileName	テクスチャファイル名
		*/
		//--------------------------------------------------------------------------------------
		CubeObjectGroup(const wstring& TextureFileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CubeObjectGroup();
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
