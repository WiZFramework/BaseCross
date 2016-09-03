/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	スプライト描画(親クラス)
	//--------------------------------------------------------------------------------------
	class SpriteDraw : public ObjectInterface {
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
		virtual void CreateConstantBuffer() = 0;
		///パイプラインステート作成
		virtual void CreatePipelineState() = 0;
		///コマンドリスト作成
		void CreateCommandList();
		//プロテクトコンストラクタ
		SpriteDraw(const wstring& TextureFileName, bool Trace);
		//プロテクトデストラクタ
		virtual ~SpriteDraw() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		///描画処理
		virtual void DrawObject(const shared_ptr<MeshResource>& Mesh) = 0;
	};

	//--------------------------------------------------------------------------------------
	// Diffuse無しコンスタントバッファ
	//--------------------------------------------------------------------------------------
	struct SpriteConstantBuffer
	{
		Matrix4X4 World;
		Color4 Emissive;
		SpriteConstantBuffer() {
			memset(this, 0, sizeof(SpriteConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	///	PCTスプライト描画
	//--------------------------------------------------------------------------------------
	class PCTSpriteDraw : public SpriteDraw {
	protected:
		///コンスタントバッファ作成
		virtual void CreateConstantBuffer();
		///パイプラインステート作成
		virtual void CreatePipelineState();
	public:
		PCTSpriteDraw(const wstring& TextureFileName, bool Trace);
		virtual ~PCTSpriteDraw() {}
		///コンスタントバッファ更新
		void UpdateConstantBuffer(SpriteConstantBuffer& CBuff);
		///描画処理
		virtual void DrawObject(const shared_ptr<MeshResource>& Mesh)override;
	};


	//--------------------------------------------------------------------------------------
	// Diffuse入りコンスタントバッファ
	//--------------------------------------------------------------------------------------
	struct DiffuseSpriteConstantBuffer
	{
		Matrix4X4 World;
		Color4 Emissive;
		Color4 Diffuse;
		DiffuseSpriteConstantBuffer() {
			memset(this, 0, sizeof(DiffuseSpriteConstantBuffer));
		};
	};

	//--------------------------------------------------------------------------------------
	///	PTスプライト描画
	//--------------------------------------------------------------------------------------
	class PTSpriteDraw : public SpriteDraw {
	protected:
		///コンスタントバッファ作成
		virtual void CreateConstantBuffer();
		///パイプラインステート作成
		virtual void CreatePipelineState();
	public:
		PTSpriteDraw(const wstring& TextureFileName, bool Trace);
		virtual ~PTSpriteDraw() {}
		///コンスタントバッファ更新
		void UpdateConstantBuffer(DiffuseSpriteConstantBuffer& CBuff);
		///描画処理
		virtual void DrawObject(const shared_ptr<MeshResource>& Mesh)override;
	};


	//--------------------------------------------------------------------------------------
	///	壁スプライト
	//--------------------------------------------------------------------------------------
	class WallSprite : public ObjectInterface, public ShapeInterface {
		//PTスプライト描画
		shared_ptr<PCTSpriteDraw> m_PCTSpriteDraw;
		//Diffuse無しコンスタントバッファ
		SpriteConstantBuffer m_SpriteConstantBuffer;
		//メッシュ
		shared_ptr<MeshResource> m_SquareMesh;
		wstring m_TextureFileName;		///<テクスチャファイル名
		bool m_Trace;					///<透明処理するかどうか
		Vector2 m_Scale;				///<スケーリング
		Vector2 m_Pos;				///<位置
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		WallSprite(const wstring& TextureFileName, bool Trace, const Vector2& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~WallSprite();
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
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};



	//--------------------------------------------------------------------------------------
	///	四角形スプライト
	//--------------------------------------------------------------------------------------
	class SquareSprite : public ObjectInterface, public ShapeInterface {
		//PTスプライト描画
		shared_ptr<PTSpriteDraw> m_PTSpriteDraw;
		//Diffuse入りコンスタントバッファ
		DiffuseSpriteConstantBuffer m_DiffuseSpriteConstantBuffer;
		//メッシュ
		shared_ptr<MeshResource> m_SquareMesh;
		//バックアップしておく頂点データ
		vector<VertexPositionTexture> m_BackupVertices;
		wstring m_TextureFileName;		///<テクスチャファイル名
		bool m_Trace;					///<透明処理するかどうか
		Vector2 m_Scale;				///<スケーリング
		Vector2 m_Pos;				///<位置
		float m_TotalTime;
		//頂点の変更
		void UpdateVertex(float ElapsedTime);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		SquareSprite(const wstring& TextureFileName, bool Trace, const Vector2& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareSprite();
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
