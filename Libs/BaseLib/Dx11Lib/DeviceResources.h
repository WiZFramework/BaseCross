/*!
@file DeviceResources.h
@brief テクスチャ、メッシュ、Dx11デバイスリソース
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	テクスチャリソース
	//--------------------------------------------------------------------------------------
	class TextureResource : public BaseResource {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テクスチャ作成用のstatic関数
		@param[in]	FileName	ファイル名
		@param[in]	TexType	テクスチャタイプ
		*/
		//--------------------------------------------------------------------------------------
		static  shared_ptr<TextureResource> CreateTextureResource(const wstring& FileName, const wstring& TexType = L"WIC");
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	FileName	ファイル名
		*/
		//--------------------------------------------------------------------------------------
		explicit TextureResource(const wstring& FileName, const wstring& TexType = L"WIC");
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TextureResource();
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シェーダーリソースビューの取得
		@return	このテクスチャのシェーダーリソースビュー
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テクスチャのファイル名の取得
		@return	このテクスチャのファイル名
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetTextureFileName() const;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	メッシュリソース
	//--------------------------------------------------------------------------------------
	class MeshResource : public BaseResource {
		friend class ObjectFactory;
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//頂点バッファ
		ComPtr<ID3D11Buffer> m_IndexBuffer;	//インデックスバッファ
		UINT m_NumVertices;				//頂点の数
		UINT m_NumIndicis;				//インデックスの数
		shared_ptr<BackupDataBase> m_BackUpData;
		vector<MaterialEx> m_MaterialExVec;	//マテリアルの配列（モデルで使用）
											//以下、ボーン用
		bool m_IsSkining;
		UINT m_BoneCount;	//ボーンの数
		UINT m_SampleCount;	//サンプリング数
		vector<Matrix4X4> m_SampleMatrixVec;	//サンプリングされたボーン行列
	protected:
		//派生クラスからのみアクセスできるアクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点バッファの設定
		@param[in]	VertexBuffer	頂点バッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetVertexBuffer(ComPtr<ID3D11Buffer>& VertexBuffer) {
			m_VertexBuffer = VertexBuffer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点数の設定<br />
		頂点バッファの数と合わせなければならないので、慎重に使用する
		@param[in]	NumVertices	頂点数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetNumVertices(UINT NumVertices) {
			m_NumVertices = NumVertices;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックスバッファの設定
		@param[in]	IndexBuffer	インデックスバッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetIndexBuffer(ComPtr<ID3D11Buffer>& IndexBuffer) {
			m_IndexBuffer = IndexBuffer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックス数の設定<br />
		インデックスバッファの数と合わせなければならないので、慎重に使用する
		@param[in]	IndexBuffer	インデックスバッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetNumIndicis(UINT NumIndicis) {
			m_NumIndicis = NumIndicis;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ<br />
		構築はスタティック関数を利用する
		*/
		//--------------------------------------------------------------------------------------
		MeshResource();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MeshResource();
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点バッファの取得
		@return	頂点バッファ
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer> GetVertexBuffer() const {
			return m_VertexBuffer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックスバッファの取得
		@return	インデックスバッファ
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer> GetIndexBuffer() const {
			return m_IndexBuffer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点数の取得
		@return	頂点数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumVertices() const {
			return m_NumVertices;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックス数の取得
		@return	インデックス数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumIndicis() const {
			return m_NumIndicis;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マテリアル配列の取得
		@return	マテリアルの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<MaterialEx>& GetMaterialExVec()const {
			return m_MaterialExVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スキニングするかどうか.<br />
		仮想関数なので、派生クラスで独自に多重定義する
		@return	スキニングする場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		virtual bool IsSkining() const { return m_IsSkining; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ボーン数を得る
		@return	ボーン数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetBoneCount() const {
			return m_BoneCount;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	サンプリング数を得る
		@return	サンプリング数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetSampleCount() const {
			return m_SampleCount;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	サンプリングされた行列の配列を得る
		@return	サンプリングされた行列の配列の参照
		*/
		//--------------------------------------------------------------------------------------
		const vector<Matrix4X4>& GetSampleMatrixVec() const {
			return m_SampleMatrixVec;
		}
		//リソース構築
		//--------------------------------------------------------------------------------------
		/*!
		@brief	バッファを作成する
		@param[in]	pDx11Device	デバイス
		@param[in]	pBufferDesc	バッファデスクプリタ
		@param[in]	pDataDesc	データデスクプリタ
		@param[out]	pBuffer	頂点バッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreatePrimitiveBuffer(ID3D11Device* pDx11Device, const D3D11_BUFFER_DESC* pBufferDesc, const D3D11_SUBRESOURCE_DATA* pDataDesc, ID3D11Buffer** pBuffer) {
			if (FAILED(pDx11Device->CreateBuffer(pBufferDesc, pDataDesc, pBuffer))) {
				// 初期化失敗
				throw BaseException(
					L"バッファ作成に失敗しました。",
					L"if(FAILED(pDx11Device->CreateBuffer()))",
					L"VertexUtil::CreatePrimitiveBuffer()"
				);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	バッファを作成する
		@tparam	T	頂点の型
		@param[in]	pDx11Device	デバイス
		@param[in]	data	データ
		@param[in]	bindFlags	バインドフラグ
		@param[out]	pBuffer	頂点バッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static void CreatePrimitiveBuffer(ID3D11Device* pDx11Device, T const& data, D3D11_BIND_FLAG bindFlags, ID3D11Buffer** pBuffer) {
			D3D11_BUFFER_DESC bufferDesc = { 0 };
			bufferDesc.ByteWidth = (UINT)data.size() * sizeof(T::value_type);
			bufferDesc.BindFlags = bindFlags;
			bufferDesc.Usage = D3D11_USAGE_DEFAULT;

			D3D11_SUBRESOURCE_DATA dataDesc = { 0 };
			dataDesc.pSysMem = &data.front();

			if (FAILED(pDx11Device->CreateBuffer(&bufferDesc, &dataDesc, pBuffer))) {
				// 初期化失敗
				throw BaseException(
					L"バッファ作成に失敗しました。",
					L"if(FAILED(pDx11Device->CreateBuffer()))",
					L"VertexUtil::CreatePrimitiveBuffer()"
				);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点バッファを作成する
		@tparam	T	頂点の型
		@param[out]	VertexBuffer	返されるバッファ
		@param[in]	data	データ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static void CreateVertexBuffer(ComPtr<ID3D11Buffer>& VertexBuffer, T const& data) {
			mutex Mutex;
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();
			Util::DemandCreate(VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				//頂点バッファの作成
				MeshResource::CreatePrimitiveBuffer(pDx11Device, data, D3D11_BIND_VERTEX_BUFFER, pResult);
			});
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックスバッファを作成する
		@tparam	T	インデックスの型
		@param[out]	IndexBuffer	返されるバッファ
		@param[in]	data	データ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static void CreateIndexBuffer(ComPtr<ID3D11Buffer>& IndexBuffer, T const& data) {
			mutex Mutex;
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();
			Util::DemandCreate(IndexBuffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				//インデックスバッファの作成
				MeshResource::CreatePrimitiveBuffer(pDx11Device, data, D3D11_BIND_INDEX_BUFFER, pResult);
			});
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点データを変更できる頂点バッファを作成する
		@tparam	T	頂点の型
		@param[in]	pDx11Device	デバイス
		@param[in]	data	データ
		@param[out]	pBuffer	返されるバッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static void CreateDynamicPrimitiveVertexBuffer(ID3D11Device* pDx11Device, T const& data, ID3D11Buffer** pBuffer) {
			D3D11_BUFFER_DESC bufferDesc = { 0 };
			bufferDesc.ByteWidth = (UINT)data.size() * sizeof(T::value_type);
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA dataDesc = { 0 };
			dataDesc.pSysMem = &data.front();

			if (FAILED(pDx11Device->CreateBuffer(&bufferDesc, &dataDesc, pBuffer))) {
				// 初期化失敗
				throw BaseException(
					L"バッファ作成に失敗しました。",
					L"if(FAILED(pDx11Device->CreateBuffer()))",
					L"VertexUtil::CreateDynamicVertexBuffer()"
				);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点データを変更できる頂点バッファを作成する
		@tparam	T	頂点の型
		@param[out]	VertexBuffer	返されるバッファ
		@param[in]	data	データ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static void CreateDynamicVertexBuffer(ComPtr<ID3D11Buffer>& VertexBuffer, T const& data) {
			mutex Mutex;
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();
			Util::DemandCreate(VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				//頂点バッファの作成
				MeshResource::CreateDynamicPrimitiveVertexBuffer(pDx11Device, data, pResult);
			});
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	4角形平面の作成
		@param[in]	size		1辺のサイズ
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateSquare(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	キューブ（立方体）の作成
		@param[in]	size	1辺のサイズ
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCube(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	球体の作成
		@param[in]	Diameter	直径
		@param[in]	Tessellation	分割数
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateSphere(float Diameter, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カプセルの作成
		@param[in]	Diameter	直径
		@param[in]	Height	高さ
		@param[in]	Tessellation	分割数
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCapsule(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シリンダーの作成
		@param[in]	Height	高さ
		@param[in]	Diameter	直径
		@param[in]	Tessellation	分割数
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCylinder(float Height, float Diameter, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コーンの作成
		@param[in]	Diameter	直径
		@param[in]	Height	高さ
		@param[in]	Tessellation	分割数
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCone(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	トーラスの作成
		@param[in]	Diameter	直径
		@param[in]	Thickness	ドーナッツの太さ
		@param[in]	Tessellation	分割数
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateTorus(float Diameter, float Thickness, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	正4面体の作成
		@param[in]	Size	1辺のサイズ
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateTetrahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	正8面体の作成
		@param[in]	Size	1辺のサイズ
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateOctahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	正12面体の作成
		@param[in]	Size	1辺のサイズ
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateDodecahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	正20面体の作成
		@param[in]	Size	1辺のサイズ
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateIcosahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルメッシュデータの読み込み（スタティックメッシュ）
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[out]	vertices	頂点の参照
		@param[out]	indices	インデックスの参照
		@param[out]	materials	マテリアルの参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ReadBaseData(const wstring& BinDataDir, const wstring& BinDataFile,
			vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices, vector<MaterialEx>& materials);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルメッシュデータの読み込み（ボーンメッシュ）
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[out]	vertices	頂点の参照
		@param[out]	indices	インデックスの参照
		@param[out]	materials	マテリアルの参照
		@param[out]	bonematrix	ボーン行列の参照
		@param[out]	BoneCount	ボーン数の参照
		@param[out]	SampleCount	サンプル数の参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ReadBaseBoneData(const wstring& BinDataDir, const wstring& BinDataFile,
			vector<VertexPositionNormalTextureSkinning>& vertices, vector<uint16_t>& indices, vector<MaterialEx>& materials,
			vector<Matrix4X4>& bonematrix, UINT& BoneCount, UINT& SampleCount);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルメッシュの作成（スタティックメッシュ）
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateStaticModelMesh(const wstring& BinDataDir,
			const wstring& BinDataFile, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルメッシュの作成（ボーンメッシュ）
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateBoneModelMesh(const wstring& BinDataDir,
			const wstring& BinDataFile, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの作成
		@tparam	T	頂点型
		@param[in]	vertices	頂点の配列
		@param[in]	AccessWrite	上書き可能かどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static  shared_ptr<MeshResource> CreateMeshResource(const vector<T>& vertices, bool AccessWrite) {
			std::mutex Mutex;
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();
			auto Ptr = ObjectFactory::Create<MeshResource>();
			//バッファの作成
			if (AccessWrite) {
				Util::DemandCreate(Ptr->m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					auto PtrBackup = shared_ptr< BackupData<T> >(new  BackupData<T>());
					for (auto& v : vertices) {
						PtrBackup->m_Vertices.push_back(v);
					}
					Ptr->m_BackUpData = PtrBackup;
					//頂点バッファの作成
					MeshResource::CreateDynamicPrimitiveVertexBuffer(pDx11Device, vertices, pResult);
				});
			}
			else {
				Util::DemandCreate(Ptr->m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					//頂点バッファの作成
					MeshResource::CreatePrimitiveBuffer(pDx11Device, vertices, D3D11_BIND_VERTEX_BUFFER, pResult);
				});
			}
			//頂点数の設定
			Ptr->m_NumVertices = static_cast<UINT>(vertices.size());
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの作成
		@tparam	T	頂点の型
		@param[in]	vertices	頂点の配列
		@param[in]	indices	インデックスの配列
		@param[in]	AccessWrite	上書き可能かどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static  shared_ptr<MeshResource> CreateMeshResource(const vector<T>& vertices, const vector<uint16_t>& indices, bool AccessWrite) {
			std::mutex Mutex;
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto Ptr = ObjectFactory::Create<MeshResource>();
			//バッファの作成
			if (AccessWrite) {
				Util::DemandCreate(Ptr->m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					auto PtrBackup = shared_ptr< BackupData<T> >(new  BackupData<T>());
					for (auto& v : vertices) {
						PtrBackup->m_Vertices.push_back(v);
					}
					Ptr->m_BackUpData = PtrBackup;
					//頂点バッファの作成
					MeshResource::CreateDynamicPrimitiveVertexBuffer(pDx11Device, vertices, pResult);
				});
			}
			else {
				Util::DemandCreate(Ptr->m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					//頂点バッファの作成
					MeshResource::CreatePrimitiveBuffer(pDx11Device, vertices, D3D11_BIND_VERTEX_BUFFER, pResult);
				});
			}
			//頂点数の設定
			Ptr->m_NumVertices = static_cast<UINT>(vertices.size());
			//インデックスの作成
			Util::DemandCreate(Ptr->m_IndexBuffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				//インデックスバッファの作成
				MeshResource::CreatePrimitiveBuffer(pDx11Device, indices, D3D11_BIND_INDEX_BUFFER, pResult);
			});
			//インデックス数の設定
			Ptr->m_NumIndicis = static_cast<UINT>(indices.size());
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	バックアップデータの取得<br />
		AccessWriteがtrueで作成されたリソースは、頂点の配列によって頂点を変更できる。<br />
		AccessWriteがtrueでない場合は、バックアップは空である。
		@tparam	T	頂点の型
		@param	なし
		@return	バックアップデータの配列。
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		vector<T>& GetBackupVerteces() const {
			auto Ptr = dynamic_pointer_cast< BackupData<T> >(m_BackUpData);
			if (!Ptr) {
				throw BaseException(
					L"バックアップをT型にキャストできません",
					Util::GetWSTypeName<T>(),
					L"MeshResource::GetBackupVerteces<T>()"
				);
			}
			return Ptr->m_Vertices;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点の変更.<br />
		AccessWriteがtrueで作成されたリソースは、頂点の配列によって頂点を変更する。
		@tparam	T	頂点の型
		@param[in]	NewBuffer	頂点の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void UpdateVirtexBuffer(const vector<T>& NewBuffer) {
			auto Ptr = dynamic_pointer_cast< BackupData<T> >(m_BackUpData);
			if (!Ptr) {
				throw BaseException(
					L"バックアップをT型にキャストできません",
					Util::GetWSTypeName<T>(),
					L"MeshResource::UpdateVirtexBuffer<T>()"
				);
			}

			if (NewBuffer.size() != Ptr->m_Vertices.size()) {
				// Map失敗
				throw BaseException(
					L"変更する頂点の数がバックアップと違います",
					L"if (NewBuffer.size() != Ptr->m_Vertices.size())",
					L"MeshResource::UpdateVirtexBuffer<T>()"
				);
			}
			//座標を変更する
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			//頂点バッファをリソースから取り出す
			auto pVertexBuffer = GetVertexBuffer().Get();

			//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
			D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
			D3D11_MAPPED_SUBRESOURCE mappedBuffer;
			//頂点のマップ
			if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))) {
				// Map失敗
				throw BaseException(
					L"頂点のMapに失敗しました。",
					L"if(FAILED(pID3D11DeviceContext->Map()))",
					L"MeshResource::UpdateVirtexBuffer<T>()"
				);
			}
			//頂点の変更
			T* vertices = (T*)mappedBuffer.pData;
			for (size_t i = 0; i < NewBuffer.size(); i++) {
				vertices[i] = NewBuffer[i];
			}
			//アンマップ
			pID3D11DeviceContext->Unmap(pVertexBuffer, 0);

		}
	};

	//汎用的な設定用定義

	//--------------------------------------------------------------------------------------
	///	ブレンドステート
	//--------------------------------------------------------------------------------------
	enum class BlendState {
		Opaque,
		AlphaBlend,
		Additive,
		NonPremultiplied,
	};

	//--------------------------------------------------------------------------------------
	///	デプスステンシルステート
	//--------------------------------------------------------------------------------------
	enum class DepthStencilState {
		None,
		Default,
		Read,
	};

	//--------------------------------------------------------------------------------------
	///	ラスタライザステート
	//--------------------------------------------------------------------------------------
	enum class RasterizerState {
		CullNone,
		CullFront,
		CullBack,
		Wireframe,
	};

	//--------------------------------------------------------------------------------------
	///	サンプラーステート
	//--------------------------------------------------------------------------------------
	enum class SamplerState {
		SamplerNone,
		PointWrap,
		PointClamp,
		LinearWrap,
		LinearClamp,
		AnisotropicWrap,
		AnisotropicClamp,
		ComparisonLinear,
	};


	//--------------------------------------------------------------------------------------
	///	パイプライン定義
	//--------------------------------------------------------------------------------------
	struct PipeLineDesc {
		//--------------------------------------------------------------------------------------
		//	ブレンドステート
		//--------------------------------------------------------------------------------------
		BlendState m_BlendState;
		//--------------------------------------------------------------------------------------
		//	デプスステンシルステート
		//--------------------------------------------------------------------------------------
		DepthStencilState m_DepthStencilState;
		//--------------------------------------------------------------------------------------
		//	ラスタライザステート
		//--------------------------------------------------------------------------------------
		RasterizerState m_RasterizerState;
	};

	class DefaultRenderTarget;
	class ShadowMapRenderTarget;
	class RenderState;
	//--------------------------------------------------------------------------------------
	///	DirectX11デバイスクラス
	//--------------------------------------------------------------------------------------
	class DeviceResources {
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	hWnd	ウインドウのハンドル
		@param[in]	isFullScreen	フルスクリーンかどうか
		@param[in]	Width	幅
		@param[in]	Height	高さ
		*/
		//--------------------------------------------------------------------------------------
		DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DeviceResources();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	後初期化（Dx12と互換性を保つためにある）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AfterInitContents() {}
		//D3Dアクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	D3D11Device2インターフェイスの取得
		@return	D3D11Device2インターフェイス
		*/
		//--------------------------------------------------------------------------------------
		ID3D11Device2* GetD3DDevice() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	D3D11DeviceContext2インターフェイスの取得
		@return	D3D11DeviceContext2インターフェイス
		*/
		//--------------------------------------------------------------------------------------
		ID3D11DeviceContext2* GetD3DDeviceContext() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	DXGISwapChain1インターフェイスの取得
		@return	DXGISwapChain1インターフェイス
		*/
		//--------------------------------------------------------------------------------------
		IDXGISwapChain1* GetSwapChain() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	D3D_FEATURE_LEVELの取得
		@return	D3D_FEATURE_LEVEL
		*/
		//--------------------------------------------------------------------------------------
		D3D_FEATURE_LEVEL GetFeatureLevel() const;
		// D2D アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	D2D1Factory2インターフェイスの取得
		@return	D2D1Factory2インターフェイス
		*/
		//--------------------------------------------------------------------------------------
		ID2D1Factory2* GetD2DFactory() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	D2D1Device1インターフェイスの取得
		@return	D2D1Device1インターフェイス
		*/
		//--------------------------------------------------------------------------------------
		ID2D1Device1* GetD2DDevice() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	D2D1DeviceContext1インターフェイスの取得
		@return	D2D1DeviceContext1インターフェイス
		*/
		//--------------------------------------------------------------------------------------
		ID2D1DeviceContext1* GetD2DDeviceContext() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	DWriteFactory2インターフェイスの取得
		@return	DWriteFactory2インターフェイス
		*/
		//--------------------------------------------------------------------------------------
		IDWriteFactory2* GetDWriteFactory() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	WICImagingFactory2インターフェイスの取得
		@return	WICImagingFactory2インターフェイス
		*/
		//--------------------------------------------------------------------------------------
		IWICImagingFactory2* GetWicImagingFactory() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップのクリア
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void ClearShadowmapViews();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップ描画の開始
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void StartShadowmapDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップ描画の終了
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EndShadowmapDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	通常描画のクリア
		@param[in]	col	クリア色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void ClearDefultViews(const Color4& col = Color4(0, 0, 0, 1.0f));
		//--------------------------------------------------------------------------------------
		/*!
		@brief	通常描画の開始
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void StartDefultDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	通常描画の終了
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EndDefultDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画の後始末<br />
		すべてのパイプライン設定をデフォルトに戻す
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void InitializeStates();
		//--------------------------------------------------------------------------------------
		/*!
		@brief デフォルトのレンダリングターゲットの取得
		@return	デフォルトのレンダリングターゲット
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<DefaultRenderTarget> GetDefaultRenderTarget();
		//--------------------------------------------------------------------------------------
		/*!
		@brief シャドウマップのレンダリングターゲットの取得
		@return	シャドウマップのレンダリングターゲット
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<ShadowMapRenderTarget> GetShadowMapRenderTarget(float ShadowMapDimension = 2048.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief レンダリングステートの取得
		@return	レンダリングステート
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<RenderState> GetRenderState()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief フロントバッファに転送
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Present(unsigned int SyncInterval, unsigned int  Flags);

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		DeviceResources(const DeviceResources&) = delete;
		DeviceResources& operator=(const DeviceResources&) = delete;
		//ムーブ禁止
		DeviceResources(const DeviceResources&&) = delete;
		DeviceResources& operator=(const DeviceResources&&) = delete;
	};



	//--------------------------------------------------------------------------------------
	///	レンダリングステートクラス
	//--------------------------------------------------------------------------------------
	class RenderState {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		RenderState();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RenderState();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	塗りつぶしのブレンドステートの取得
		@return	ブレンドステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11BlendState* GetOpaque()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単純な透明処理の取得
		@return	ブレンドステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11BlendState* GetAlphaBlend()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	半透明処理の取得
		@return	ブレンドステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11BlendState* GetAlphaBlendEx()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	加算処理の取得
		@return	ブレンドステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11BlendState* GetAdditive()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	アルファを使用した元のデータと対象データをブレンド処理の取得
		@return	ブレンドステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11BlendState* GetNonPremultiplied()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	深度処理なしのデプスステンシルステートの取得
		@return	デプスステンシルステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11DepthStencilState* GetDepthNone()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	通常深度のデプスステンシルステートの取得
		@return	デプスステンシルステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11DepthStencilState* GetDepthDefault()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	読み取り専用のデプスステンシルステートの取得
		@return	デプスステンシルステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11DepthStencilState* GetDepthRead()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カリングなしのラスタライザステートの取得
		@return	ラスタライザステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11RasterizerState* GetCullNone()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カリングなしのラスタライザステート（シーサー処理）の取得
		@return	ラスタライザステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11RasterizerState* GetCullNoneScissor()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フロントカリング（背面描画のみ）のラスタライザステートの取得
		@return	ラスタライザステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11RasterizerState* GetCullFront()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フロントカリング（背面描画のみ）のラスタライザステート（シーサー処理）の取得
		@return	ラスタライザステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11RasterizerState* GetCullFrontScissor()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	バックカリング（前面描画のみ）のラスタライザステートの取得
		@return	ラスタライザステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11RasterizerState* GetCullBack()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	バックカリング（前面描画のみ）のラスタライザステート（シーサー処理）の取得
		@return	ラスタライザステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11RasterizerState* GetCullBackScissor()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワイアフレームのラスタライザステートの取得
		@return	ラスタライザステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11RasterizerState* GetWireframe()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワイアフレームのラスタライザステート（シーサー処理）の取得
		@return	ラスタライザステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11RasterizerState* GetWireframeScissor()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	点フィルタリングとテクスチャー座標ラップのサンプラーステートの取得
		@return	サンプラーステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11SamplerState* GetPointWrap()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	点フィルタリングとテクスチャー座標クランプのサンプラーステートの取得
		@return	サンプラーステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11SamplerState* GetPointClamp()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	線形フィルタリングとテクスチャー座標ラップのサンプラーステートの取得
		@return	サンプラーステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11SamplerState* GetLinearWrap()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	線形フィルタリングとテクスチャー座標クランプのサンプラーステートの取得
		@return	サンプラーステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11SamplerState* GetLinearClamp()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	異方性フィルタリングとテクスチャー座標ラップのサンプラーステートの取得
		@return	サンプラーステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11SamplerState* GetAnisotropicWrap()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	異方性フィルタリングとテクスチャー座標クランプのサンプラーステートの取得
		@return	サンプラーステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11SamplerState* GetAnisotropicClamp()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	線形フィルタリングを使った比較サンプラーステートの取得
		@return	サンプラーステートインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11SamplerState* GetComparisonLinear()const;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		RenderState(const RenderState&) = delete;
		RenderState& operator=(const RenderState&) = delete;
		//ムーブ禁止
		RenderState(const RenderState&&) = delete;
		RenderState& operator=(const RenderState&&) = delete;
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
		@brief	ビューポートを得る
		@return	ビューポート
		*/
		//--------------------------------------------------------------------------------------
		const D3D11_VIEWPORT& GetViewport() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ビューポートの設定
		@param[in]	Viewport	ビューポート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetViewport(const D3D11_VIEWPORT& Viewport);
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

	class View;
	class Stage;

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
		@brief	シャドウマップのシェーダリソースビューを得る
		@return	シャドウマップのシェーダリソースビューインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11ShaderResourceView* GetShaderResourceView() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップのデプスステンシルテクスチャを得る
		@return	シャドウマップのデプスステンシルテクスチャインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11Texture2D*		GetDepthStencil() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップのデプスステンシルシェーダリソースビューを得る
		@return	シャドウマップのデプスステンシルシェーダリソースビューインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11DepthStencilView*	GetDepthStencilView() const;
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
		virtual void EndRenderTarget()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	デフォルトのレンダリングターゲット
	//--------------------------------------------------------------------------------------
	class DefaultRenderTarget : public RenderTarget {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		explicit DefaultRenderTarget();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DefaultRenderTarget();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	レンダリングターゲットビューを得る
		@return	レンダリングターゲットビューインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11RenderTargetView* GetRenderTargetView() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デプスステンシルテクスチャを得る
		@return	デプスステンシルテクスチャインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11Texture2D*		GetDepthStencil() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デプスステンシルビューを得る
		@return	デプスステンシルビューインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11DepthStencilView*	GetDepthStencilView() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	2次元ターゲットのビットマップを得る
		@return	2次元ターゲットのビットマップインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		ID2D1Bitmap1*			GetD2DTargetBitmap() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	レンダリングターゲットをクリアする仮想関数（スクリーン全体をクリアする）
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
		virtual void EndRenderTarget()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




	//--------------------------------------------------------------------------------------
	///	シェーダ関連リソースのインターフェイス
	//--------------------------------------------------------------------------------------
	class ShaderResource {
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	protected:
		ShaderResource();
		virtual ~ShaderResource();
	public:
		static const int m_LightMax{ 3 };
		//--------------------------------------------------------------------------------------
		//	static void ReadBinaryFile(
		//		const wstring& fileName,		//ファイル名
		//		unique_ptr<uint8_t[]>& Data,	//バイナリデータの戻り
		//		size_t& CsoSz					//サイズの戻り
		//	);
		//	用途: バイナリファイルを読み込む
		//--------------------------------------------------------------------------------------
		static void ReadBinaryFile(const wstring& fileName, unique_ptr<uint8_t[]>& Data, size_t& CsoSz);
		//--------------------------------------------------------------------------------------
		//	static void CreateVertexShader(
		//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
		//		size_t CsoSz,					//サイズ
		//		ID3D11VertexShader** pResult			//受け取るシェーダ
		//	);
		//	用途: バイナリデータから頂点シェーダを作成する
		//--------------------------------------------------------------------------------------
		static void CreateVertexShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11VertexShader** pResult);
		//--------------------------------------------------------------------------------------
		//	static void CreateInputLayout(
		//		unique_ptr<uint8_t[]>& Data,	//頂点シェーダのバイナリデータ
		//		size_t CsoSz,					//サイズ
		//		const D3D11_INPUT_ELEMENT_DESC* pElement,	//頂点定義
		//		UINT NumElement,							//頂点定義の数
		//		ID3D11InputLayout** pResult			//受け取るレイアウト
		//	);
		//	用途: バイナリデータからインプットレイアウトを作成する
		//--------------------------------------------------------------------------------------
		static void CreateInputLayout(unique_ptr<uint8_t[]>& Data, size_t CsoSz,
			const D3D11_INPUT_ELEMENT_DESC* pElement, UINT NumElement, ID3D11InputLayout** pResult);
		//--------------------------------------------------------------------------------------
		//	static void CreatePixelShader(
		//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
		//		size_t CsoSz,					//サイズ
		//		ID3D11PixelShader** pResult			//受け取るシェーダ
		//	);
		//	用途: バイナリデータからピクセルシェーダを作成する
		//--------------------------------------------------------------------------------------
		static void CreatePixelShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11PixelShader** pResult);
		//--------------------------------------------------------------------------------------
		//	static void CreateGeometryShader(
		//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
		//		size_t CsoSz,					//サイズ
		//		ID3D11GeometryShader** pResult			//受け取るシェーダ
		//	);
		//	用途: バイナリデータからジオメトリシェーダを作成する
		//--------------------------------------------------------------------------------------
		static void CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11GeometryShader** pResult);

		static void CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz,
			const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries, UINT Stride, UINT NumStride, ID3D11GeometryShader** pResult);

		//--------------------------------------------------------------------------------------
		//	static void CreateComputeShader(
		//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
		//		size_t CsoSz,					//サイズ
		//		ID3D11ComputeShader** pResult			//受け取るシェーダ
		//	);
		//	用途: バイナリデータからコンピュートシェーダを作成する
		//--------------------------------------------------------------------------------------
		static void CreateComputeShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11ComputeShader** pResult);
		//--------------------------------------------------------------------------------------
		//	static void CreateConstantBuffer(
		//		UINT BuffSize,					//サイズ
		//		ID3D11Buffer** pResult			//受け取るシェーダ
		//	);
		//	用途: コンスタントバッファを作成する
		//--------------------------------------------------------------------------------------
		static void CreateConstantBuffer(UINT BuffSize, ID3D11Buffer** pResult);
	protected:
		//ミューテックス
		std::mutex MutexBase;
		//ピクセルシェーダアクセッサ
		ID3D11PixelShader* GetPixelShaderBase(const wstring& Filename, ComPtr<ID3D11PixelShader>& PSPtr) {
			return Util::DemandCreate(PSPtr, MutexBase, [&](ID3D11PixelShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//シェーダの読み込み
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreatePixelShader(data, cso_sz, pResult);
			});
		}

		//ジオメトリシェーダアクセッサ
		ID3D11GeometryShader* GetGeometryShaderBase(const wstring& Filename, ComPtr<ID3D11GeometryShader>& GSPtr) {
			return Util::DemandCreate(GSPtr, MutexBase, [&](ID3D11GeometryShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//シェーダの読み込み
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateGeometryShader(data, cso_sz, pResult);
			});
		}


		ID3D11GeometryShader* GetGeometryShaderBase(const wstring& Filename,
			const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries,
			UINT Stride, UINT NumStride,
			ComPtr<ID3D11GeometryShader>& GSPtr) {
			return Util::DemandCreate(GSPtr, MutexBase, [&](ID3D11GeometryShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//シェーダの読み込み
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateGeometryShader(data, cso_sz, SOEntries, Stride, NumStride, pResult);
			});
		}

		//コンピュートシェーダアクセッサ
		ID3D11ComputeShader* GetComputeShaderBase(const wstring& Filename, ComPtr<ID3D11ComputeShader>& CSPtr) {
			return Util::DemandCreate(CSPtr, MutexBase, [&](ID3D11ComputeShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//シェーダの読み込み
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateComputeShader(data, cso_sz, pResult);
			});
		}

		//頂点シェーダアクセッサ
		ID3D11VertexShader* GetVertexShaderBase(const wstring& Filename, ComPtr<ID3D11VertexShader>& VSPtr);
		//インプットレイアウトアクセッサ
		ID3D11InputLayout* GetInputLayoutBase2(const wstring& Filename, ComPtr<ID3D11InputLayout>& ILPtr,
			const D3D11_INPUT_ELEMENT_DESC* pElem, UINT NumElements);
		//コンスタントバッファアクセッサ
		ID3D11Buffer* GetConstantBufferBase(UINT BuffSize, ComPtr<ID3D11Buffer>& Buffer);
	private:
		//コピー禁止
		ShaderResource(const ShaderResource&) = delete;
		ShaderResource& operator=(const ShaderResource&) = delete;
		//ムーブ禁止
		ShaderResource(const ShaderResource&&) = delete;
		ShaderResource& operator=(const ShaderResource&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	//	template<typename ShaderType, typename ConstantType>
	//	class ConstantBuffer : public ShaderResource;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	template<typename ShaderType, typename ConstantType>
	class ConstantBuffer : public ShaderResource {
	protected:
		//デリーター
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11Buffer> m_Buffer;
		//ミューテックス
		std::mutex Mutex;
		//構築と破棄
		ConstantBuffer<ShaderType, ConstantType>() : ShaderResource() {}
		virtual ~ConstantBuffer() {}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
		//コピー禁止
		ConstantBuffer<ShaderType, ConstantType>(ConstantBuffer const&);
		ConstantBuffer<ShaderType, ConstantType>& operator=(ConstantBuffer<ShaderType, ConstantType> const&);
	public:
		//公開アクセッサ
		ID3D11Buffer* GetBuffer() {
			return GetConstantBufferBase(sizeof(ConstantType), m_Buffer);
		}
		//インスタンス取得
		static unique_ptr<ShaderType, Deleter>& GetPtr() {
			if (!m_Ptr) {
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};


	//--------------------------------------------------------------------------------------
	//	template<typename ConstantType>
	//	class CBuffer : public ShaderResource;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	template<typename ConstantType>
	class CBuffer : public ShaderResource {
		ComPtr<ID3D11Buffer> m_Buffer;
		//コピー禁止
		CBuffer(CBuffer const&);
		CBuffer& operator=(CBuffer<ConstantType> const&);
	public:
		//構築と破棄
		CBuffer() : ShaderResource() {
			GetConstantBufferBase(sizeof(ConstantType), m_Buffer);
		}
		virtual ~CBuffer() {}
		//公開アクセッサ
		ID3D11Buffer* GetBuffer() {
			return GetConstantBufferBase(sizeof(ConstantType), m_Buffer);
		}
	};



	//--------------------------------------------------------------------------------------
	//	class VertexShader : public ShaderResource;
	//	用途: 頂点シェーダ
	//--------------------------------------------------------------------------------------
	template<typename ShaderType, typename VirtexType>
	class VertexShader : public ShaderResource {
		wstring m_Filename;
	protected:
		//デリーター
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11VertexShader> m_VSPtr;
		ComPtr<ID3D11InputLayout> m_ILPtr;
		//ミューテックス
		std::mutex Mutex;
		//構築と破棄
		VertexShader<ShaderType, VirtexType>(const wstring& Filename) : ShaderResource(), m_Filename(Filename) {}
		virtual ~VertexShader() {}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
	public:
		//頂点シェーダ公開アクセッサ
		ID3D11VertexShader* GetShader() {
			return GetVertexShaderBase(m_Filename, m_VSPtr);
		}
		//インプットレイアウト公開アクセッサ
		ID3D11InputLayout* GetInputLayout() {
			return GetInputLayoutBase2(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
		//インスタンス取得
		static unique_ptr<ShaderType, Deleter>& GetPtr() {
			if (!m_Ptr) {
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};

	//--------------------------------------------------------------------------------------
	//template<typename VirtexType>
	//	class VShader : public ShaderResource;
	//	用途: シングルトン処理をしない頂点シェーダ（クライアントで直接使用できる）
	//--------------------------------------------------------------------------------------
	template<typename VirtexType>
	class VShader : public ShaderResource {
		wstring m_Filename;
		ComPtr<ID3D11VertexShader> m_VSPtr;
		ComPtr<ID3D11InputLayout> m_ILPtr;
	public:
		VShader() : ShaderResource(), m_Filename() {}
		VShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename) {
			GetVertexShaderBase(m_Filename, m_VSPtr);
			GetInputLayoutBase2(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
		virtual ~VShader() {}
		void Create(const wstring& Filename) {
			m_Filename = Filename;
			m_VSPtr = nullptr;
			m_ILPtr = nullptr;
			GetVertexShaderBase(m_Filename, m_VSPtr);
			GetInputLayoutBase2(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
		//頂点シェーダ公開アクセッサ
		ID3D11VertexShader* GetShader() {
			return GetVertexShaderBase(m_Filename, m_VSPtr);
		}
		//インプットレイアウト公開アクセッサ
		ID3D11InputLayout* GetInputLayout() {
			return GetInputLayoutBase2(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
	};

	//--------------------------------------------------------------------------------------
	//	template<typename ShaderType>
	//	class PixelShader : public ShaderResource;
	//	用途: ピクセルシェーダ
	//--------------------------------------------------------------------------------------
	template<typename ShaderType>
	class PixelShader : public ShaderResource {
		wstring m_Filename;
	protected:
		//デリーター
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11PixelShader> m_PSPtr;
		//ミューテックス
		std::mutex Mutex;
		//構築と破棄
		PixelShader<ShaderType>(const wstring& Filename) : ShaderResource(), m_Filename(Filename) {}
		virtual ~PixelShader() {}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
	public:
		//ピクセルシェーダ公開アクセッサ
		ID3D11PixelShader* GetShader() {
			return GetPixelShaderBase(m_Filename, m_PSPtr);
		}
		//インスタンス取得
		static unique_ptr<ShaderType, Deleter>& GetPtr() {
			if (!m_Ptr) {
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class PShader : public ShaderResource;
	//	用途: ピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PShader : public ShaderResource {
		wstring m_Filename;
		ComPtr<ID3D11PixelShader> m_PSPtr;
	public:
		//構築と破棄
		PShader() : ShaderResource(), m_Filename() {}
		PShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename) {
			GetPixelShaderBase(m_Filename, m_PSPtr);
		}
		virtual ~PShader() {}
		void Create(const wstring& Filename) {
			m_Filename = Filename;
			m_PSPtr = nullptr;
			GetPixelShaderBase(m_Filename, m_PSPtr);
		}
		//ピクセルシェーダ公開アクセッサ
		ID3D11PixelShader* GetShader() {
			return GetPixelShaderBase(m_Filename, m_PSPtr);
		}
	};



	//--------------------------------------------------------------------------------------
	//	template<typename ShaderType>
	//	class GeometryShader : public ShaderResource;
	//	用途: ジオメトリシェーダ
	//--------------------------------------------------------------------------------------
	template<typename ShaderType>
	class GeometryShader : public ShaderResource {
		wstring m_Filename;
		vector<D3D11_SO_DECLARATION_ENTRY> m_SOEntries;
		UINT m_Stride;
		UINT m_NumStride;
	protected:
		//デリーター
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11GeometryShader> m_GSPtr;
		//ミューテックス
		std::mutex Mutex;
		//構築と破棄
		GeometryShader<ShaderType>(const wstring& Filename) : ShaderResource(), m_Filename(Filename) {}
		virtual ~GeometryShader() {}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
		//スロットエントリの登録
		void SetSOEntries(const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries,
			UINT Stride, UINT NumStride) {
			m_SOEntries.clear();
			for (auto& rEntry : SOEntries) {
				D3D11_SO_DECLARATION_ENTRY Dest = rEntry;
				m_SOEntries.push_back(Dest);
			}
			m_Stride = Stride;
			m_NumStride = NumStride;
		}
	public:
		//ジオメトリシェーダ公開アクセッサ
		ID3D11GeometryShader* GetShader() {
			if (m_SOEntries.size() > 0) {
				return GetGeometryShaderBase(m_Filename, m_SOEntries, m_Stride, m_NumStride, m_GSPtr);
			}
			else {
				return GetGeometryShaderBase(m_Filename, m_GSPtr);
			}
		}
		//インスタンス取得
		static unique_ptr<ShaderType, Deleter>& GetPtr() {
			if (!m_Ptr) {
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};


	//--------------------------------------------------------------------------------------
	//	class GShader : public ShaderResource;
	//	用途: ジオメトリシェーダ
	//--------------------------------------------------------------------------------------
	class GShader : public ShaderResource {
		ComPtr<ID3D11GeometryShader> m_GSPtr;
		wstring m_Filename;
		vector<D3D11_SO_DECLARATION_ENTRY> m_SOEntries;
		UINT m_Stride;
		UINT m_NumStride;
	public:
		//構築と破棄
		GShader() : ShaderResource(), m_Filename() {}
		GShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename) {}
		virtual ~GShader() {}
		//スロットエントリの登録
		void SetSOEntries(const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries,
			UINT Stride, UINT NumStride) {
			m_SOEntries.clear();
			for (auto& rEntry : SOEntries) {
				D3D11_SO_DECLARATION_ENTRY Dest = rEntry;
				m_SOEntries.push_back(Dest);
			}
			m_Stride = Stride;
			m_NumStride = NumStride;
		}
		void Create(const wstring& Filename) {
			m_Filename = Filename;
			m_GSPtr = nullptr;
			if (m_SOEntries.size() > 0) {
				GetGeometryShaderBase(m_Filename, m_SOEntries, m_Stride, m_NumStride, m_GSPtr);
			}
			else {
				GetGeometryShaderBase(m_Filename, m_GSPtr);
			}
		}
		//ジオメトリシェーダ公開アクセッサ
		ID3D11GeometryShader* GetShader() {
			if (m_SOEntries.size() > 0) {
				return GetGeometryShaderBase(m_Filename, m_SOEntries, m_Stride, m_NumStride, m_GSPtr);
			}
			else {
				return GetGeometryShaderBase(m_Filename, m_GSPtr);
			}
		}
	};


	//--------------------------------------------------------------------------------------
	//	template<typename ShaderType>
	//	class ComputeShader : public ShaderResource;
	//	用途: コンピュートシェーダ
	//--------------------------------------------------------------------------------------
	template<typename ShaderType>
	class ComputeShader : public ShaderResource {
		wstring m_Filename;
	protected:
		//デリーター
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3D11ComputeShader> m_CSPtr;
		//ミューテックス
		std::mutex Mutex;
		//構築と破棄
		ComputeShader<ShaderType>(const wstring& Filename) : ShaderResource(), m_Filename(Filename) {}
		virtual ~ComputeShader() {}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
	public:
		//コンピュートシェーダ公開アクセッサ
		ID3D11ComputeShader* GetShader() {
			return GetComputeShaderBase(m_Filename, m_CSPtr);
		}
		//インスタンス取得
		static unique_ptr<ShaderType, Deleter>& GetPtr() {
			if (!m_Ptr) {
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class CShader : public ShaderResource;
	//	用途: コンピュートシェーダ
	//--------------------------------------------------------------------------------------
	class CShader : public ShaderResource {
		wstring m_Filename;
		ComPtr<ID3D11ComputeShader> m_CSPtr;
	public:
		//構築と破棄
		CShader() : ShaderResource(), m_Filename() {}
		CShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename) {
			GetComputeShaderBase(m_Filename, m_CSPtr);
		}
		virtual ~CShader() {}
		void Create(const wstring& Filename) {
			m_Filename = Filename;
			m_CSPtr = nullptr;
			GetComputeShaderBase(m_Filename, m_CSPtr);
		}
		//コンピュートシェーダ公開アクセッサ
		ID3D11ComputeShader* GetShader() {
			return GetComputeShaderBase(m_Filename, m_CSPtr);
		}
	};

	//これより個別シェーダー
	//コンスタントバッファ宣言用マクロ
#define DECLARE_DX11_CONSTANT_BUFFER(CBName,CBStruct) class CBName : public ConstantBuffer<CBName,CBStruct> { \
public: \
			};

	//シェーダ宣言用マクロ
#define DECLARE_DX11_VERTEX_SHADER(ShaderName,VertexType) class ShaderName : public VertexShader<ShaderName,VertexType>{ \
	public: \
		ShaderName(); \
			};

#define DECLARE_DX11_PIXEL_SHADER(ShaderName) class ShaderName : public PixelShader<ShaderName>{ \
	public: \
		ShaderName(); \
			};

#define DECLARE_DX11_GEOMETRY_SHADER(ShaderName) class ShaderName : public GeometryShader<ShaderName>{ \
	public: \
		ShaderName(); \
		};


	//コンスタントバッファ実体用マクロ
#define IMPLEMENT_DX11_CONSTANT_BUFFER(CBName) unique_ptr<CBName, CBName::Deleter> CBName::m_Ptr;

	//シェーダ実体用マクロ
#define IMPLEMENT_DX11_VERTEX_SHADER(ShaderName,CsoFilename) unique_ptr<ShaderName, ShaderName::Deleter> ShaderName::m_Ptr; \
	ShaderName::ShaderName() : \
	VertexShader(CsoFilename){}

#define IMPLEMENT_DX11_PIXEL_SHADER(ShaderName,CsoFilename) unique_ptr<ShaderName, ShaderName::Deleter> ShaderName::m_Ptr; \
	ShaderName::ShaderName() : \
	PixelShader(CsoFilename){}

#define IMPLEMENT_DX11_GEOMETRY_SHADER(ShaderName,CsoFilename)	unique_ptr<ShaderName, ShaderName::Deleter> ShaderName::m_Ptr; \
	ShaderName::ShaderName() : \
	GeometryShader(CsoFilename){}




}
//end basecross
