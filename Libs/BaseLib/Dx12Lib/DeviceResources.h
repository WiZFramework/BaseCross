/*!
@file DeviceResources.h
@brief テクスチャ、メッシュ、Dx12デバイスリソース
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// メモリ上に保持するDx12テクスチャリソース
	//--------------------------------------------------------------------------------------
	class TextureResource : public BaseResource {
		friend class ObjectFactory;
		void OnCreateWithParam(uint32_t Width, uint32_t Height, const vector<uint8_t>& data);
		void OnCreateWithParam(const wstring& FileName, const wstring& TexType);
		TextureResource();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TextureResource();
		//--------------------------------------------------------------------------------------
		/*!
		@brief メモリ上にテクスチャリソースの作成
		@param[in]	Width 幅
		@param[in]	Height 高さ
		@return	テクスチャリソースのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		static  shared_ptr<TextureResource> CreateTextureResource(uint32_t Width, uint32_t Height, const vector<uint8_t>& data);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 画像ファイルからテクスチャリソースの作成
		@param[in]	FileName ファイル名
		@param[in]	TexType 画像タイプ
		@return	テクスチャリソースのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		static  shared_ptr<TextureResource> CreateTextureResource(const wstring& FileName, const wstring& TexType = L"WIC");
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャの幅を得る
		@return	テクスチャの幅
		*/
		//--------------------------------------------------------------------------------------
		uint32_t GetTextureWidth();
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャの高さを得る
		@return	テクスチャの高さ
		*/
		//--------------------------------------------------------------------------------------
		uint32_t GetTextureHeight();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ピクセル数を得る
		@return	ピクセル数
		*/
		//--------------------------------------------------------------------------------------
		uint32_t GetPixelSize();
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャのリソースデスクプリタを得る
		@return	テクスチャのリソースデスクプリタ
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_RESOURCE_DESC& GetTextureResDesc() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャのリソースを得る
		@return	テクスチャのリソース
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12Resource> GetTexture() const;
		//テクスチャデータ
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャデータを得る
		@return	テクスチャデータの配列
		*/
		//--------------------------------------------------------------------------------------
		vector<uint8_t>& GetData() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャデータを設定する
		@param[in]	data データの配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetData(const vector<uint8_t>& data);
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャデータを設定する(SetData関数と同じ)
		@param[in]	data データの配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateTexture(const vector<uint8_t>& data);
		//--------------------------------------------------------------------------------------
		/*!
		@brief リソースを更新する
		@param[in]	commandList コマンドリスト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateResources(const ComPtr<ID3D12GraphicsCommandList>& commandList);
		//仮想関数（空関数）
		virtual void OnPreCreate()override {}
		virtual void OnCreate()override {}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	/// Dx12メッシュクラス
	//--------------------------------------------------------------------------------------
	class MeshResource : public BaseResource {
		friend class ObjectFactory;
		ComPtr<ID3D12Resource> m_VertexBuffer;	//頂点バッファ
		ComPtr<ID3D12Resource> m_IndexBuffer;	//インデックスバッファ
		ComPtr<ID3D12Resource> m_VertexBufferUploadHeap;
		ComPtr<ID3D12Resource> m_IndexBufferUploadHeap;
		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
		UINT m_NumVertices;				//頂点の数
		UINT m_NumIndicis;				//インデックス数
		bool m_AccessWrite;
		bool m_DataRefresh;
		shared_ptr<BackupDataBase> m_BackupData;
		MeshResource() :
			m_VertexBufferView{ 0 },
			m_IndexBufferView{ 0 },
			m_NumVertices(0),
			m_NumIndicis(0),
			m_AccessWrite(false),
			m_DataRefresh(true)
		{}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MeshResource() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief インデックスを含むメッシュリソースの作成
		@tparam	T 頂点の型
		@param[in]	vertices 頂点の配列
		@param[in]	indices インデックスの配列
		@param[in]	AccessWrite 頂点変更可能かどうか
		@return	メッシュリソースのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static  shared_ptr<MeshResource> CreateMeshResource(const vector<T>& vertices, const vector<uint16_t>& indices, bool AccessWrite) {
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			auto Ptr = ObjectFactory::Create<MeshResource>();
			Ptr->m_AccessWrite = AccessWrite;
			//データのバックアップ

			auto DataPtr = make_shared< BackupData<T> >();
			DataPtr->m_Vertices.clear();
			for (auto v : vertices) {
				DataPtr->m_Vertices.push_back(v);
			}
			DataPtr->m_Indices.clear();
			for (auto i : indices) {
				DataPtr->m_Indices.push_back(i);
			}
			Ptr->m_BackupData = DataPtr;
			UINT vertexBufferSize = sizeof(T) * vertices.size();
			//頂点バッファの作成
			{
				ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_VertexBuffer)),
					L"頂点バッファ作成に失敗しました。",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);

				ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_VertexBufferUploadHeap)),
					L"頂点アップロードバッファ作成に失敗しました。",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);
				// Initialize the vertex buffer view.
				Ptr->m_VertexBufferView.BufferLocation = Ptr->m_VertexBuffer->GetGPUVirtualAddress();
				Ptr->m_VertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(T));
				Ptr->m_VertexBufferView.SizeInBytes = static_cast<UINT>(sizeof(T) * vertices.size());
			}
			//頂点数の設定
			Ptr->m_NumVertices = static_cast<UINT>(vertices.size());
			//インデックスバッファの作成
			UINT indexBufferSize = static_cast<UINT>(sizeof(uint16_t) * indices.size());
			{
				ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_IndexBuffer)),
					L"インデックスバッファ作成に失敗しました。",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);

				ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_IndexBufferUploadHeap)),
					L"インデックスアップロードバッファ作成に失敗しました。",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);

				// Describe the index buffer view.
				Ptr->m_IndexBufferView.BufferLocation = Ptr->m_IndexBuffer->GetGPUVirtualAddress();
				Ptr->m_IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
				Ptr->m_IndexBufferView.SizeInBytes = indexBufferSize;

			}
			//インデックス数の設定
			Ptr->m_NumIndicis = static_cast<UINT>(indices.size());
			return Ptr;
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief インデックスを含まないメッシュリソースの作成
		@tparam	T 頂点の型
		@param[in]	vertices 頂点の配列
		@param[in]	AccessWrite 頂点変更可能かどうか
		@return	メッシュリソースのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static  shared_ptr<MeshResource> CreateMeshResource(const vector<T>& vertices, bool AccessWrite) {
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			auto Ptr = ObjectFactory::Create<MeshResource>();
			Ptr->m_AccessWrite = AccessWrite;

			//データのバックアップ

			auto DataPtr = make_shared< BackupData<T> >();
			DataPtr->m_Vertices.clear();
			for (auto v : vertices) {
				DataPtr->m_Vertices.push_back(v);
			}
			Ptr->m_BackupData = DataPtr;
			UINT vertexBufferSize = sizeof(T) * vertices.size();
			// Create the vertex buffer.
			{
				ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_VertexBuffer)),
					L"頂点バッファ作成に失敗しました。",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);

				ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_VertexBufferUploadHeap)),
					L"頂点アップロードバッファ作成に失敗しました。",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);
				// Initialize the vertex buffer view.
				Ptr->m_VertexBufferView.BufferLocation = Ptr->m_VertexBuffer->GetGPUVirtualAddress();
				Ptr->m_VertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(T));
				Ptr->m_VertexBufferView.SizeInBytes = static_cast<UINT>(sizeof(T) * vertices.size());
			}
			//頂点数の設定
			Ptr->m_NumVertices = static_cast<UINT>(vertices.size());
			return Ptr;
		}

		//プリミティブリソース構築
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 4角形平面の作成
		@param[in]	size		1辺のサイズ
		@param[in]	AccessWrite	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateSquare(float Size, bool AccessWrite = false) {
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateSquare(Size, vertices, indices);
			return CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@breaf キューブ（立方体）の作成
		@param[in]	size		1辺のサイズ
		@param[in]	AccessWrite	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCube(float Size, bool AccessWrite = false) {
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateCube(Size, vertices, indices);
			return CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 球体の作成
		@param[in]	Diameter		直径
		@param[in]	Tessellation	分割数
		@param[in]	AccessWrite	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateSphere(float Diameter, size_t Tessellation, bool AccessWrite = false) {
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateSphere(Diameter, Tessellation, vertices, indices);
			return CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf カプセルの作成
		@param[in]	Diameter		直径
		@param[in]	Height,		高さ
		@param[in]	Tessellation	分割数
		@param[in]	AccessWrite	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCapsule(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false) {
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			Vector3 PointA(0, -Height / 2.0f, 0);
			Vector3 PointB(0, Height / 2.0f, 0);
			//Capsuleの作成(ヘルパー関数を利用)
			MeshUtill::CreateCapsule(Diameter, PointA, PointB, Tessellation, vertices, indices);
			return CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf シリンダーの作成
		@param[in]	Height,		高さ
		@param[in]	Diameter		直径
		@param[in]	Tessellation	分割数
		@param[in]	AccessWrite	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCylinder(float Height, float Diameter, size_t Tessellation, bool AccessWrite = false) {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Cylinderの作成(ヘルパー関数を利用)
			MeshUtill::CreateCylinder(Height, Diameter, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf コーンの作成
		@param[in]	Diameter		直径
		@param[in]	Height,		高さ
		@param[in]	Tessellation	分割数
		@param[in]	AccessWrite	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCone(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false) {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Coneの作成(ヘルパー関数を利用)
			MeshUtill::CreateCone(Diameter, Height, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf トーラスの作成
		@param[in]	Diameter		直径
		@param[in]	Thickness		ドーナッツの太さ
		@param[in]	Tessellation	分割数
		@param[in]	AccessWrite	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateTorus(float Diameter, float Thickness, size_t Tessellation, bool AccessWrite = false) {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Torusの作成(ヘルパー関数を利用)
			MeshUtill::CreateTorus(Diameter, Thickness, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 正4面体の作成
		@param[in]	Size		1辺のサイズ
		@param[in]	AccessWrite	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateTetrahedron(float Size, bool AccessWrite = false) {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Tetrahedronの作成(ヘルパー関数を利用)
			MeshUtill::CreateTetrahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 正8面体の作成
		@param[in]	Size		1辺のサイズ
		@param[in]	AccessWrite	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateOctahedron(float Size, bool AccessWrite = false) {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Octahedronの作成(ヘルパー関数を利用)
			MeshUtill::CreateOctahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 正12面体の作成
		@param[in]	Size		1辺のサイズ
		@param[in]	AccessWrite	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateDodecahedron(float Size, bool AccessWrite = false) {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Dodecahedronの作成(ヘルパー関数を利用)
			MeshUtill::CreateDodecahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 正20面体の作成
		@param[in]	Size		1辺のサイズ
		@param[in]	AccessWrite	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateIcosahedron(float Size, bool AccessWrite = false) {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Icosahedronの作成(ヘルパー関数を利用)
			MeshUtill::CreateIcosahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 頂点バッファの取得
		@return	頂点の参照
		*/
		//--------------------------------------------------------------------------------------
		const ComPtr<ID3D12Resource>& GetVertexBuffer()const {
			return m_VertexBuffer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 頂点バッファビューの取得
		@return	頂点バッファビューの参照
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const {
			return m_VertexBufferView;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 頂点数の取得
		@return	頂点数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumVertices() const {
			return m_NumVertices;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf インデックスバッファの取得
		@return	インデックスバッファの参照
		*/
		//--------------------------------------------------------------------------------------
		const ComPtr<ID3D12Resource>& GetIndexBuffer()const {
			return m_IndexBuffer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf インデックスバッファビューの取得
		@return	インデックスバッファビューの参照
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const {
			return m_IndexBufferView;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf インデックス数の取得
		@return	インデックス数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumIndicis() const {
			return m_NumIndicis;
		}

		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief  頂点の更新
		@tparam	T	頂点の型
		@param[in] vertices	頂点の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void UpdateVirtex(const vector<T>& vertices) {
			if (!m_AccessWrite) {
				ThrowBaseException(
					L"このメッシュの頂点変更は許可されていません",
					L"if (!m_AccessWrite)",
					L"MeshResource::UpdateVirtex()"
				);
			}
			auto shptr = dynamic_pointer_cast< BackupData<T> >(m_BackupData);
			if (!shptr) {
				return;
			}
			if (shptr->m_Vertices.size() < vertices.size()) {
				return;
			}
			for (size_t i = 0; i < vertices.size();i++) {
				shptr->m_Vertices[i] = vertices[i];
			}
			m_DataRefresh = true;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief  リソースの更新
		@tparam	T	頂点の型
		@param[in] commandList	コマンドリスト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void UpdateResources(const ComPtr<ID3D12GraphicsCommandList>& commandList) {
			if (!m_DataRefresh) {
				return;
			}
			auto shptr = dynamic_pointer_cast< BackupData<T> >(m_BackupData);
			if (!shptr) {
				return;
			}
			//頂点バッファの更新
			D3D12_SUBRESOURCE_DATA vertexData = {};
			vertexData.pData = (void*)&shptr->m_Vertices[0];
			vertexData.RowPitch = shptr->m_Vertices.size() * sizeof(T);
			vertexData.SlicePitch = vertexData.RowPitch;
			UpdateSubresources<1>(commandList.Get(), m_VertexBuffer.Get(), m_VertexBufferUploadHeap.Get(), 0, 0, 1, &vertexData);
			commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_VertexBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));
			//インデックスバッファの更新
			if (shptr->m_Indices.size() > 0) {
				D3D12_SUBRESOURCE_DATA indexData = {};
				indexData.pData = (void*)&shptr->m_Indices[0];
				indexData.RowPitch = shptr->m_Indices.size() * sizeof(uint16_t);
				indexData.SlicePitch = indexData.RowPitch;
				UpdateSubresources<1>(commandList.Get(), m_IndexBuffer.Get(), m_IndexBufferUploadHeap.Get(), 0, 0, 1, &indexData);
				commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_IndexBuffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER));
			}
			m_DataRefresh = false;
		}
		virtual void OnPreCreate()override {}
		virtual void OnCreate()override {}
	};


	//--------------------------------------------------------------------------------------
	//	enum class BlendState;
	//	用途: ブレンドステート
	//--------------------------------------------------------------------------------------
	enum class BlendState {
		Opaque,
		AlphaBlend,
		Additive,
		NonPremultiplied,
	};

	//--------------------------------------------------------------------------------------
	//	enum class DepthStencilState;
	//	用途: デプスステンシルステート
	//--------------------------------------------------------------------------------------
	enum class DepthStencilState {
		None,
		Default,
		Read,
	};

	//--------------------------------------------------------------------------------------
	//	enum class RasterizerState;
	//	用途: ラスタライザステート
	//--------------------------------------------------------------------------------------
	enum class RasterizerState {
		CullNone,
		CullFront,
		CullBack,
		Wireframe,
	};

	//--------------------------------------------------------------------------------------
	//	enum class SamplerState;
	//	用途: サンプラーステート
	//--------------------------------------------------------------------------------------
	enum class SamplerState {
		PointWrap,
		PointClamp,
		LinearWrap,
		LinearClamp,
		AnisotropicWrap,
		AnisotropicClamp,
		ComparisonLinear,
	};


	class ShadowMapRenderTarget;

	//--------------------------------------------------------------------------------------
	//	class DeviceResources;
	/*!
	DirectX12デバイスクラス
	*/
	//--------------------------------------------------------------------------------------
	class DeviceResources {
	public:
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
		@brief  フレーム数を得る
		@return	フレーム数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetFrameCount() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  フレームインデックスを得る
		@return	フレームインデックス
		*/
		//--------------------------------------------------------------------------------------
		UINT GetFrameIndex() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  スワップチェインを得る
		@return	スワップチェイン
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<IDXGISwapChain3> GetSwapChain() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  デバイスを得る
		@return	デバイス
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12Device> GetDevice() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief シャドウマップのレンダリングターゲットの作成
		@param[in] ShadowMapDimension	シャドウマップの幅及び高さ
		@return	シャドウマップのレンダリングターゲット
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ShadowMapRenderTarget> CreateShadowMapRenderTarget(float ShadowMapDimension = 2048.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief シャドウマップのレンダリングターゲットの取得
		@return	シャドウマップのレンダリングターゲット
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ShadowMapRenderTarget> GetShadowMapRenderTarget(float ShadowMapDimension = 2048.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief  レンダリングターゲット（のポインタ）を得る
		@return	レンダリングターゲット（のポインタ）
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12Resource>* GetRenderTargets() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 指定したインデックスのレンダリングターゲットを得る
		@return	指定したインデックスのレンダリングターゲット
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12Resource> GetRenderTarget(UINT Index) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コマンドアロケータを得る
		@return	コマンドアロケータ
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コマンドキューを得る
		@return	コマンドキュー
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12CommandQueue> GetCommandQueue() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief レンダリングターゲットビューのヒープ領域を得る
		@return	レンダリングターゲットビューのヒープ領域
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12DescriptorHeap> GetRtvHeap() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief デプスステンシルビューのヒープ領域を得る
		@return	デプスステンシルビューのヒープ領域
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12DescriptorHeap> GetDsvHeap() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief レンダリングターゲットビューのデスクプリタサイズを得る
		@return	レンダリングターゲットビューのデスクプリタサイズ
		*/
		//--------------------------------------------------------------------------------------
		UINT GetRtvDescriptorSize() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ビューポートを得る
		@return	ビューポート
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_VIEWPORT& GetViewport() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief シザー領域を得る
		@return	シザー領域
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_RECT& GetScissorRect() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コマンドリストを追加する
		@param[in] Tgt	コマンドリストのポインタ（生ポインタ）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void InsertDrawCommandLists(ID3D12CommandList* Tgt);
		//--------------------------------------------------------------------------------------
		/*!
		@brief コマンドリストを実行しフロントバッファへの転送を行う
		@param[in] SyncInterval	インターバル
		@param[in] Flags	フラグ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Present(unsigned int SyncInterval, unsigned int  Flags);


		//--------------------------------------------------------------------------------------
		/*!
		@brief たまっているコマンドリストを実行してコマンドリストのバッファをクリアする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ExecuteCommandLists();


		//--------------------------------------------------------------------------------------
		/*!
		@brief 1つ前のフレームの処理を待つ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void WaitForPreviousFrame();
		//仮想関数
		//--------------------------------------------------------------------------------------
		/*!
		@brief シャドウマップのクリア（未定義）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void ClearShadowmapViews();
		//--------------------------------------------------------------------------------------
		/*!
		@brief シャドウマップ描画の開始（未定義）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void StartShadowmapDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief シャドウマップ描画の終了（未定義）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EndShadowmapDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 通常描画のクリア（未定義）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void ClearDefultViews(const Color4& col = Color4(0, 0, 0, 1.0f));
		//--------------------------------------------------------------------------------------
		/*!
		@brief 通常描画の開始（未定義）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void StartDefultDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 通常描画の終了（未定義）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EndDefultDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief レンダリングターゲット類の準備（未定義）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void InitRenderTargets() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンテンツの作成後の処理
		@param[in]	ShadowActive	影が有効かどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterInitContents(bool ShadowActive);
		//--------------------------------------------------------------------------------------
		/*!
		@brief キーに割り当てられたルートシグネチャを得る
		@param[in] Key	キー
		@return	見つかればルートシグネチャ。なければnurrptr
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12RootSignature> GetRootSignature(const wstring& Key);

		//--------------------------------------------------------------------------------------
		/*!
		@brief キーに割り当てられたルートシグネチャを設定する
		@param[in] Key	キー
		@param[in] rootsig	設定するルートシグネチャ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetRootSignature(const wstring& Key,const ComPtr<ID3D12RootSignature>& rootsig);

		//--------------------------------------------------------------------------------------
		/*!
		@brief レンダーターゲットビューのハンドルを得る
		@return	レンダーターゲットビューのハンドル
		*/
		//--------------------------------------------------------------------------------------
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtvHandle() const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief デプスステンシルビューのハンドルを得る
		@return	デプスステンシルビューのハンドル
		*/
		//--------------------------------------------------------------------------------------
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetDsvHandle() const;


	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	レンダーターゲット基底クラス
	//--------------------------------------------------------------------------------------
	class RenderTarget {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		RenderTarget();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RenderTarget();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	レンダリングターゲットをクリアする純粋仮想関数
		@param[in]	col	クリア色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void ClearViews(const Color4& col = Color4(0, 0, 0, 1.0f)) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	レンダリングターゲットを開始する純粋仮想関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void StartRenderTarget() = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	レンダリングターゲットを終了する純粋仮想関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EndRenderTarget() = 0;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		RenderTarget(const RenderTarget&) = delete;
		RenderTarget& operator=(const RenderTarget&) = delete;
		//ムーブ禁止
		RenderTarget(const RenderTarget&&) = delete;
		RenderTarget& operator=(const RenderTarget&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	///	シャドウマップのレンダリングターゲット
	//--------------------------------------------------------------------------------------
	class ShadowMapRenderTarget : public RenderTarget {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	ShadowMapDimension	シャドウマップの大きさ
		*/
		//--------------------------------------------------------------------------------------
		ShadowMapRenderTarget(float ShadowMapDimension);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShadowMapRenderTarget();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップの大きさを得る
		@return	シャドウマップの大きさ
		*/
		//--------------------------------------------------------------------------------------
		float GetShadowMapDimension() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	レンダリングターゲットをクリアする仮想関数
		@param[in]	col	クリア色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void ClearViews(const Color4& col = Color4(0, 0, 0, 1.0f)) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	レンダリングターゲットを開始する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void StartRenderTarget()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	レンダリングターゲットを終了する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EndRenderTarget()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デプスステンシルビューのハンドルを得る
		@return	デプスステンシルビューのハンドル
		*/
		//--------------------------------------------------------------------------------------
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetDsvHandle() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief デプスステンシルを得る
		@return	デプスステンシル
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12Resource> GetDepthStencil() const;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};






	//--------------------------------------------------------------------------------------
	/// デスクプリタヒープユーティリティ
	//--------------------------------------------------------------------------------------
	namespace DescriptorHeap {
		static inline ComPtr<ID3D12DescriptorHeap> CreateDirect(const D3D12_DESCRIPTOR_HEAP_DESC& desc) {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12DescriptorHeap> Ret;
			ThrowIfFailed(Dev->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&Ret)),
				L"デスクプリタヒープの作成に失敗しました",
				L"Dev->GetDevice()->CreateDescriptorHeap()",
				L"DescriptorHeap::CreateDirect()"
			);
			return Ret;
		}
		static inline ComPtr<ID3D12DescriptorHeap> CreateCbvSrvUavHeap(UINT NumDescriptorHeap) {
			//CbvSrvデスクプリタヒープ
			D3D12_DESCRIPTOR_HEAP_DESC CbvSrvHeapDesc = {};
			CbvSrvHeapDesc.NumDescriptors = NumDescriptorHeap;
			CbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			CbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			return CreateDirect(CbvSrvHeapDesc);
		}
		static inline ComPtr<ID3D12DescriptorHeap> CreateSamplerHeap(UINT NumDescriptorHeap) {
			//サンプラーデスクプリタヒープ
			D3D12_DESCRIPTOR_HEAP_DESC SamplerHeapDesc = {};
			SamplerHeapDesc.NumDescriptors = NumDescriptorHeap;
			SamplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			SamplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			return CreateDirect(SamplerHeapDesc);
		}

	}

	//--------------------------------------------------------------------------------------
	/// ルートシグネチャユーティリティ
	//--------------------------------------------------------------------------------------
	namespace RootSignature {
		static inline ComPtr<ID3D12RootSignature> CreateDirect(const D3D12_ROOT_SIGNATURE_DESC& desc) {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3DBlob> signature;
			ComPtr<ID3DBlob> error;
			ComPtr<ID3D12RootSignature> Ret;
			ThrowIfFailed(D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error),
				L"ルートシグネチャのシリアライズに失敗しました",
				L"D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error)",
				L"RootSignature::Create()"
			);
			ThrowIfFailed(
				Dev->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(),
					signature->GetBufferSize(), IID_PPV_ARGS(&Ret)),
				L"ルートシグネチャの作成に失敗しました",
				L"Dev->GetDevice()->CreateRootSignature()",
				L"RootSignature::Create()"
			);
			return Ret;
		}
		//一番シンプルなルートシグネチャ
		static inline ComPtr<ID3D12RootSignature> CreateSimple() {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12RootSignature> Ret = Dev->GetRootSignature(L"Simple");
			if (Ret != nullptr) {
				return Ret;
			}

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			Ret = CreateDirect(rootSignatureDesc);
			Dev->SetRootSignature(L"Simple", Ret);
			return Ret;
		}
		//コンスタントバッファのみ
		static inline ComPtr<ID3D12RootSignature> CreateCbv() {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12RootSignature> Ret = Dev->GetRootSignature(L"Cbv");
			if (Ret != nullptr) {
				return Ret;
			}

			CD3DX12_DESCRIPTOR_RANGE ranges[1];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
			CD3DX12_ROOT_PARAMETER rootParameters[1];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_ALL);

			D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);

			Ret = CreateDirect(rootSignatureDesc);
			Dev->SetRootSignature(L"Cbv", Ret);
			return Ret;
		}
		//シェーダリソースとサンプラー
		static inline ComPtr<ID3D12RootSignature> CreateSrvSmp() {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12RootSignature> Ret = Dev->GetRootSignature(L"SrvSmp");
			if (Ret != nullptr) {
				return Ret;
			}

			CD3DX12_DESCRIPTOR_RANGE ranges[2];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
			ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

			CD3DX12_ROOT_PARAMETER rootParameters[2];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			Ret = CreateDirect(rootSignatureDesc);
			Dev->SetRootSignature(L"SrvSmp", Ret);
			return Ret;
		}

		//シェーダリソースとサンプラーとコンスタントバッファ
		static inline ComPtr<ID3D12RootSignature> CreateSrvSmpCbv() {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12RootSignature> Ret = Dev->GetRootSignature(L"SrvSmpCbv");
			if (Ret != nullptr) {
				return Ret;
			}


			CD3DX12_DESCRIPTOR_RANGE ranges[3];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
			ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
			ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

			CD3DX12_ROOT_PARAMETER rootParameters[3];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_ALL);

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			Ret = CreateDirect(rootSignatureDesc);
			Dev->SetRootSignature(L"SrvSmpCbv", Ret);
			return Ret;
		}
		//シェーダリソース2つとサンプラー2つとコンスタントバッファ1つ
		static inline ComPtr<ID3D12RootSignature> CreateSrv2Smp2Cbv() {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12RootSignature> Ret = Dev->GetRootSignature(L"Srv2Smp2Cbv");
			if (Ret != nullptr) {
				return Ret;
			}


			CD3DX12_DESCRIPTOR_RANGE ranges[5];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
			ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);
			ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);
			ranges[3].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 1);
			ranges[4].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);

			CD3DX12_ROOT_PARAMETER rootParameters[5];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[3].InitAsDescriptorTable(1, &ranges[3], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[4].InitAsDescriptorTable(1, &ranges[4], D3D12_SHADER_VISIBILITY_ALL);

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			Ret = CreateDirect(rootSignatureDesc);
			Dev->SetRootSignature(L"Srv2Smp2Cbv", Ret);
			return Ret;
		}


	}

	//--------------------------------------------------------------------------------------
	/// サンプラーユーティリティ（変更可能）
	//--------------------------------------------------------------------------------------
	namespace DynamicSampler {
		static inline void CreateSampler(const SamplerState State, D3D12_CPU_DESCRIPTOR_HANDLE& Handle) {
			D3D12_SAMPLER_DESC samplerDesc = {};
			//デフォルトを入れておく
			samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
			samplerDesc.MinLOD = 0;
			samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
			samplerDesc.MipLODBias = 0.0f;
			samplerDesc.MaxAnisotropy = 1;
			samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;

			switch (State) {
			case SamplerState::PointWrap:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				break;
			case SamplerState::PointClamp:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				break;
			case SamplerState::LinearWrap:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				break;
			case SamplerState::LinearClamp:
				samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				break;
			case SamplerState::AnisotropicWrap:
				samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
				break;
			case SamplerState::AnisotropicClamp:
				samplerDesc.Filter = D3D12_FILTER_ANISOTROPIC;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
				break;
			case SamplerState::ComparisonLinear:
				samplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
				samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
				samplerDesc.BorderColor[0] = 1.0f;
				samplerDesc.BorderColor[1] = 1.0f;
				samplerDesc.BorderColor[2] = 1.0f;
				samplerDesc.BorderColor[3] = 1.0f;
				samplerDesc.MinLOD = 0;
				samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
				samplerDesc.MipLODBias = 0.0f;
				samplerDesc.MaxAnisotropy = 0;
				samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

				break;
			}

			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			Dev->GetDevice()->CreateSampler(&samplerDesc, Handle);
		}
	}


	//--------------------------------------------------------------------------------------
	/// パイプラインステートユーティリティ
	//--------------------------------------------------------------------------------------
	namespace PipelineState {
		static inline ComPtr<ID3D12PipelineState> CreateDirect(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc) {
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12PipelineState> Ret;
			ThrowIfFailed(Dev->GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&Ret)),
				L"パイプラインステートの作成に失敗しました",
				L"Dev->GetDevice()->CreateGraphicsPipelineState()",
				L"PipelineState::CreateDirect()"
			);
			return Ret;
		}
		template<typename Vertex, typename VS, typename PS>
		static inline ComPtr<ID3D12PipelineState> CreateDefault2D(const ComPtr<ID3D12RootSignature>& rootSignature, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc) {

			ZeroMemory(&RetDesc, sizeof(RetDesc));
			RetDesc.InputLayout = { Vertex::GetVertexElement(), Vertex::GetNumElements() };
			RetDesc.pRootSignature = rootSignature.Get();

			RetDesc.VS = { reinterpret_cast<UINT8*>(
				VS::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
				VS::GetPtr()->GetShaderComPtr()->GetBufferSize() };
			RetDesc.PS = { reinterpret_cast<UINT8*>(
				PS::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
				PS::GetPtr()->GetShaderComPtr()->GetBufferSize() };
			RetDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			RetDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			RetDesc.DepthStencilState.DepthEnable = FALSE;
			RetDesc.DepthStencilState.StencilEnable = FALSE;
			RetDesc.SampleMask = UINT_MAX;
			RetDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			RetDesc.NumRenderTargets = 1;
			RetDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			RetDesc.SampleDesc.Count = 1;
			return CreateDirect(RetDesc);
		}
		template<typename Vertex, typename VS, typename PS>
		static inline ComPtr<ID3D12PipelineState> CreateDefault3D(const ComPtr<ID3D12RootSignature>& rootSignature, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc) {

			CD3DX12_RASTERIZER_DESC rasterizerStateDesc(D3D12_DEFAULT);
			//裏面カリング
			rasterizerStateDesc.CullMode = D3D12_CULL_MODE_NONE;

			ZeroMemory(&RetDesc, sizeof(RetDesc));
			RetDesc.InputLayout = { Vertex::GetVertexElement(), Vertex::GetNumElements() };
			RetDesc.pRootSignature = rootSignature.Get();
			RetDesc.VS =
			{
				reinterpret_cast<UINT8*>(VS::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
				VS::GetPtr()->GetShaderComPtr()->GetBufferSize()
			};
			RetDesc.PS =
			{
				reinterpret_cast<UINT8*>(PS::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
				PS::GetPtr()->GetShaderComPtr()->GetBufferSize()
			};
			RetDesc.RasterizerState = rasterizerStateDesc;
			RetDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			RetDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			RetDesc.SampleMask = UINT_MAX;
			RetDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			RetDesc.NumRenderTargets = 1;
			RetDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			RetDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
			RetDesc.SampleDesc.Count = 1;
			return CreateDirect(RetDesc);
		}

		template<typename Vertex, typename VS>
		static inline ComPtr<ID3D12PipelineState> CreateShadowmap3D(const ComPtr<ID3D12RootSignature>& rootSignature, D3D12_GRAPHICS_PIPELINE_STATE_DESC& RetDesc) {

			CD3DX12_RASTERIZER_DESC rasterizerStateDesc(D3D12_DEFAULT);
			//表面カリング
			rasterizerStateDesc.CullMode = D3D12_CULL_MODE_FRONT;
			rasterizerStateDesc.FillMode = D3D12_FILL_MODE_SOLID;
			rasterizerStateDesc.DepthClipEnable = TRUE;

			ZeroMemory(&RetDesc, sizeof(RetDesc));
			RetDesc.InputLayout = { Vertex::GetVertexElement(), Vertex::GetNumElements() };
			RetDesc.pRootSignature = rootSignature.Get();
			RetDesc.VS =
			{
				reinterpret_cast<UINT8*>(VS::GetPtr()->GetShaderComPtr()->GetBufferPointer()),
				VS::GetPtr()->GetShaderComPtr()->GetBufferSize()
			};

			RetDesc.PS = CD3DX12_SHADER_BYTECODE(0, 0);
			RetDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
			RetDesc.NumRenderTargets = 0;

			RetDesc.RasterizerState = rasterizerStateDesc;
			RetDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			RetDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			RetDesc.SampleMask = UINT_MAX;
			RetDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			RetDesc.NumRenderTargets = 0;
			RetDesc.RTVFormats[0] = DXGI_FORMAT_UNKNOWN;
			RetDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
			RetDesc.SampleDesc.Count = 1;
			return CreateDirect(RetDesc);
		}

	}


	//--------------------------------------------------------------------------------------
	/// コマンドリストユーティリティ
	//--------------------------------------------------------------------------------------
	namespace CommandList {
		static inline  ComPtr<ID3D12GraphicsCommandList> CreateSimple() {
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12GraphicsCommandList> Ret;
			ThrowIfFailed(Dev->GetDevice()->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				Dev->GetCommandAllocator().Get(),
				nullptr,
				IID_PPV_ARGS(&Ret)),
				L"コマンドリストの作成に失敗しました",
				L"Dev->GetDevice()->CreateCommandList()",
				L"CommandList::CreateSimple()"
			);
			return Ret;
		}

		static inline  ComPtr<ID3D12GraphicsCommandList> CreateDefault(const ComPtr<ID3D12PipelineState>& pipelineState) {
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12GraphicsCommandList> Ret;
			ThrowIfFailed(Dev->GetDevice()->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				Dev->GetCommandAllocator().Get(),
				pipelineState.Get(),
				IID_PPV_ARGS(&Ret)),
				L"コマンドリストの作成に失敗しました",
				L"Dev->GetDevice()->CreateCommandList()",
				L"CommandList::CreateDefault()"
			);
			return Ret;
		}
		static inline  void Reset(const ComPtr<ID3D12GraphicsCommandList>& commandList) {
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ThrowIfFailed(commandList->Reset(Dev->GetCommandAllocator().Get(), nullptr),
				L"コマンドリストのリセットに失敗しました",
				L"commandList->Reset(Dev->GetCommandAllocator().Get(),nullptr)",
				L"CommandList::Reset()"
			);

		}
		static inline  void Reset(const ComPtr<ID3D12PipelineState>& pipelineState, const ComPtr<ID3D12GraphicsCommandList>& commandList) {
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ThrowIfFailed(commandList->Reset(Dev->GetCommandAllocator().Get(), pipelineState.Get()),
				L"コマンドリストのリセットに失敗しました",
				L"commandList->Reset(Dev->GetCommandAllocator().Get(),pipelineState.Get())",
				L"CommandList::Reset()"
			);

		}
		static inline  void Close(const ComPtr<ID3D12GraphicsCommandList>& commandList) {
			ThrowIfFailed(commandList->Close(),
				L"コマンドリストのクローズに失敗しました",
				L"commandList->Close()",
				L"CommandList::Close()"
			);

		}


	}

	//--------------------------------------------------------------------------------------
	///	シェーダ関連リソースのインターフェイス
	//--------------------------------------------------------------------------------------
	class Dx12ShaderResource {
	protected:
		Dx12ShaderResource();
		virtual ~Dx12ShaderResource();
		//シェーダアクセサ
		ID3DBlob* GetShaderBlob(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr);
		ComPtr<ID3DBlob>& GetShaderBlobComPtr(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr);
	private:
		//コピー禁止
		Dx12ShaderResource(const Dx12ShaderResource&) = delete;
		Dx12ShaderResource& operator=(const Dx12ShaderResource&) = delete;
		//ムーブ禁止
		Dx12ShaderResource(const Dx12ShaderResource&&) = delete;
		Dx12ShaderResource& operator=(const Dx12ShaderResource&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	///	シングルトンシェーダの親クラス(シェーダの種類にかかわらず同じクラス使用)
	//--------------------------------------------------------------------------------------
	template<typename ShaderType>
	class Dx12Shader : public Dx12ShaderResource {
		wstring m_Filename;
	protected:
		//デリーター
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3DBlob> m_ShaderPtr;
		//構築と破棄
		Dx12Shader<ShaderType>(const wstring& Filename) : Dx12ShaderResource(), m_Filename(Filename) {}
		virtual ~Dx12Shader() {}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
	public:
		//シェーダ公開アクセッサ
		ID3DBlob* GetShader() {
			return GetShaderBlob(m_Filename, m_ShaderPtr);
		}
		ComPtr<ID3DBlob>& GetShaderComPtr() {
			return GetShaderBlobComPtr(m_Filename, m_ShaderPtr);
		}
		//インスタンス取得
		static unique_ptr<ShaderType, Deleter>& GetPtr() {
			if (!m_Ptr) {
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};


	//これより個別シェーダー
	//シェーダ宣言用マクロ
#define DECLARE_DX12SHADER(ShaderName) class ShaderName : public Dx12Shader<ShaderName>{ \
	public: \
		ShaderName(); \
	};
	//シェーダ実体用マクロ
#define IMPLEMENT_DX12SHADER(ShaderName,CsoFilename) unique_ptr<ShaderName, ShaderName::Deleter> ShaderName::m_Ptr; \
	ShaderName::ShaderName() : \
	Dx12Shader(CsoFilename){}





}
//end basecross
