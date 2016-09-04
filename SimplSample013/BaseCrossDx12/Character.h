/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//コンスタントバッファ構造体
	struct PNTStaticConstantBuffer
	{
		Matrix4X4 World;
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Color4 Emissive;
		Color4 Diffuse;
		PNTStaticConstantBuffer() {
			memset(this, 0, sizeof(PNTStaticConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	///	PNTスタティック描画（PNT頂点のプリミティブ形状用）
	//--------------------------------------------------------------------------------------
	class PNTStaticDraw : public ObjectInterface {
	protected:
		bool m_Trace;					///<透明処理するかどうか
		wstring m_TextureFileName;		///<テクスチャファイル名
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
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
	public:
		//コンストラクタ
		PNTStaticDraw(const wstring& TextureFileName, bool Trace);
		//デストラクタ
		virtual ~PNTStaticDraw() {}

		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//コンスタントバッファ更新
		void UpdateConstantBuffer(PNTStaticConstantBuffer & Data);
		///描画処理
		void DrawObject(const shared_ptr<MeshResource>& Mesh);
	};



	class Scene;

	//--------------------------------------------------------------------------------------
	///	平面
	//--------------------------------------------------------------------------------------
	class SquareObject : public ObjectInterface, public ShapeInterface {
		weak_ptr<Scene> m_Scene;			///<シーン
		shared_ptr<PNTStaticDraw> m_PNTStaticDraw;
		//メッシュ
		shared_ptr<MeshResource> m_SquareMesh;
		wstring m_TextureFileName;		///<テクスチャファイル名
		Vector3 m_Scale;				///<スケーリング
		Quaternion m_Qt;			///<回転
		Vector3 m_Pos;				///<位置
		void CreateBuffers();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	PtrScene	シーンのポインタ
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		SquareObject(const shared_ptr<Scene> PtrScene,
			const wstring& TextureFileName, const Vector3& Scale, const Quaternion& Qt, const Vector3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareObject();
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



	//--------------------------------------------------------------------------------------
	///	球
	//--------------------------------------------------------------------------------------
	class SphereObject : public ObjectInterface, public ShapeInterface {
		weak_ptr<Scene> m_Scene;			///<シーン
		shared_ptr<PNTStaticDraw> m_PNTStaticDraw;
		//メッシュ
		shared_ptr<MeshResource> m_SphereMesh;
		UINT m_Division;				///<分割数
		wstring m_TextureFileName;		///<テクスチャファイル名
		//shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		Vector3 m_Scale;				///<スケーリング
		Quaternion m_Qt;			///<回転
		Vector3 m_Pos;				///<位置
		bool m_Trace;					///<透明処理するかどうか
		void CreateBuffers();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	PtrScene	シーンのポインタ
		@param[in]	Division	分割数
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Trace	透明処理するかどうか
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		SphereObject(const shared_ptr<Scene> PtrScene,
			UINT Division, const wstring& TextureFileName, bool Trace, const Vector3& Pos);
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
