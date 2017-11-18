/*!
@file DeviceResources.h
@brief テクスチャ、メッシュ、Dx11デバイスリソース
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	テクスチャリソース
	//--------------------------------------------------------------------------------------
	class TextureResource : public BaseResource {
		friend class ObjectFactory;
		explicit TextureResource(const wstring& FileName, const wstring& TexType = L"WIC");
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
	///	一つのメッシュデータ(MultiMeshで使用)
	//--------------------------------------------------------------------------------------
	struct MeshPrimData {
		//頂点バッファ
		ComPtr<ID3D11Buffer> m_VertexBuffer;
		//インデックスバッファ
		ComPtr<ID3D11Buffer> m_IndexBuffer;
		//頂点の数
		UINT m_NumVertices;
		//インデックスの数
		UINT m_NumIndicis;
		//このメッシュの形インデックス
		type_index m_MeshTypeIndex;
		//ストライド数
		UINT m_NumStride;
		//描画トポロジー
		D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;
		//バックアップデータ
		shared_ptr<BackupDataBase> m_BackUpData;
		//メッシュととタンスフォームの差分行列（メッシュ単位で設定する場合）
		bsm::Mat4x4 m_MeshToTransformMatrix;
		//メッシュの差分行列を設定するかどうか
		bool m_UseMeshToTransformMatrix;
		//テクスチャリソース(メッシュ単位で設定する場合)
		weak_ptr<TextureResource> m_TextureResource;
		//マテリアルの配列（モデルで使用）
		vector<MaterialEx> m_MaterialExVec;
		//以下、ボーン用
		//ボーンかどうか
		bool m_IsSkining;
		//ボーンの数
		UINT m_BoneCount;
		//サンプリング数
		UINT m_SampleCount;
		//サンプリングされたボーン行列
		vector<bsm::Mat4x4> m_SampleMatrixVec;
		//マルチメッシュの場合のメッシュインデックス
		UINT m_MultiMeshIndex;
		MeshPrimData():
			m_IsSkining(false),
			m_BoneCount(0),
			m_SampleCount(0),
			m_MeshTypeIndex(typeid(VertexPosition)),	//便宜上VertexPositionに初期化
			m_NumStride(sizeof(VertexPosition)),
			m_PrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST),
			m_MeshToTransformMatrix(),
			m_UseMeshToTransformMatrix(false),
			m_MultiMeshIndex(0)
		{}
	};


	//--------------------------------------------------------------------------------------
	///	メッシュリソース
	//--------------------------------------------------------------------------------------
	class MeshResource : public BaseResource {
		friend class ObjectFactory;
		MeshPrimData m_MeshPrimData;
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
			m_MeshPrimData.m_VertexBuffer = VertexBuffer;
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
			m_MeshPrimData.m_NumVertices = NumVertices;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックスバッファの設定
		@param[in]	IndexBuffer	インデックスバッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetIndexBuffer(ComPtr<ID3D11Buffer>& IndexBuffer) {
			m_MeshPrimData.m_IndexBuffer = IndexBuffer;
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
			m_MeshPrimData.m_NumIndicis = NumIndicis;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点の型とストライドを設定する
		@tparam	T	頂点の型
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void SetVertexType() {
			m_MeshPrimData.m_MeshTypeIndex = typeid(T);
			m_MeshPrimData.m_NumStride = sizeof(T);
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
			return m_MeshPrimData.m_VertexBuffer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックスバッファの取得
		@return	インデックスバッファ
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer> GetIndexBuffer() const {
			return m_MeshPrimData.m_IndexBuffer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点数の取得
		@return	頂点数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumVertices() const {
			return m_MeshPrimData.m_NumVertices;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックス数の取得
		@return	インデックス数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumIndicis() const {
			return m_MeshPrimData.m_NumIndicis;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ストライド数の取得
		@return	ストライド数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumStride() const {
			return m_MeshPrimData.m_NumStride;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画トポロジーの取得
		@return	描画トポロジー
		*/
		//--------------------------------------------------------------------------------------
		D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const {
			return m_MeshPrimData.m_PrimitiveTopology;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画トポロジーの設定
		@param[in]	Topology	描画トポロジー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology){
			m_MeshPrimData.m_PrimitiveTopology = Topology;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	type_indexの取得
		@return	type_index
		*/
		//--------------------------------------------------------------------------------------
		type_index GetTypeIndex() const {
			return m_MeshPrimData.m_MeshTypeIndex;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュとワールド行列の間を補完する行列の取得(メッシュに設定する場合)<br />
		メッシュのサイズや向き、中心の位置がゲームオブジェクトのワールド行列と差異がある場合、<br />
		その間を補完する行列を設定できる。
		@return	メッシュとワールド行列の間を補完する行列
		*/
		//--------------------------------------------------------------------------------------
		const bsm::Mat4x4& GetMeshToTransformMatrix() const {
			return m_MeshPrimData.m_MeshToTransformMatrix;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュとワールド行列の間を補完する行列の取得(メッシュに設定する場合)<br />
		メッシュのサイズや向き、中心の位置がゲームオブジェクトのワールド行列と差異がある場合、<br />
		その間を補完する行列を設定できる。
		@param[in]	Mat	設定する行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshToTransformMatrix(const bsm::Mat4x4& Mat) {
			m_MeshPrimData.m_MeshToTransformMatrix = Mat;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュに設定したメッシュとワールド行列の間を補完する行列を使うかどうか
		@return　メッシュに設定したメッシュとワールド行列の間を補完する行列を使うかどうか
		*/
		//--------------------------------------------------------------------------------------
		bool IsUseMeshToTransformMatrix() const {
			return m_MeshPrimData.m_UseMeshToTransformMatrix;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュに設定したメッシュとワールド行列の間を補完する行列を使うかどうかを設定
		@param[in]	b	使用するならtrue
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		void SetUseMeshToTransformMatrix(bool b){
			m_MeshPrimData.m_UseMeshToTransformMatrix = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テクスチャリソースの設定(メッシュ毎の場合)
		@param[in]	TextureRes	テクスチャリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const shared_ptr<TextureResource>& TextureRes) {
			m_MeshPrimData.m_TextureResource = TextureRes;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テクスチャリソースの設定(メッシュ毎の場合)
		@param[in]	TextureKey	登録されているテクスチャキー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const wstring& TextureKey) {
			this->SetTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テクスチャリソースの取得(メッシュ毎の場合)
		@return	テクスチャリソース
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<TextureResource> GetTextureResource() const {
			//テクスチャがなければnullを返す
			auto shptr = m_MeshPrimData.m_TextureResource.lock();
			if (shptr) {
				return shptr;
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マテリアル配列の取得
		@return	マテリアルの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<MaterialEx>& GetMaterialExVec()const {
			return m_MeshPrimData.m_MaterialExVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マテリアル配列の取得(書き込み用)
		@return	マテリアルの配列
		*/
		//--------------------------------------------------------------------------------------
		vector<MaterialEx>& GetMaterialExVec(){
			return m_MeshPrimData.m_MaterialExVec;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	スキニングするかどうか.<br />
		仮想関数なので、派生クラスで独自に多重定義する
		@return	スキニングする場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		virtual bool IsSkining() const { return m_MeshPrimData.m_IsSkining; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ボーン数を得る
		@return	ボーン数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetBoneCount() const {
			return m_MeshPrimData.m_BoneCount;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	サンプリング数を得る
		@return	サンプリング数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetSampleCount() const {
			return m_MeshPrimData.m_SampleCount;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	サンプリングされた行列の配列を得る
		@return	サンプリングされた行列の配列の参照
		*/
		//--------------------------------------------------------------------------------------
		const vector<bsm::Mat4x4>& GetSampleMatrixVec() const {
			return m_MeshPrimData.m_SampleMatrixVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュデータを得る
		@return	メッシュデータ
		*/
		//--------------------------------------------------------------------------------------
		const MeshPrimData& GetMashData()const {
			return m_MeshPrimData;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュデータを設定する
		@param[out]	data	取得するデータの参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMashData(const MeshPrimData& data) {
			m_MeshPrimData = data;
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
		@tparam	T	データの型
		@param[in]	pDx11Device	デバイス
		@param[in]	data	データ
		@param[in]	bindFlags	バインドフラグ
		@param[out]	pBuffer	バッファ
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
		@param[in]	Reader	リーダー
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[out]	vertices	頂点の参照
		@param[out]	indices	インデックスの参照
		@param[out]	materials	マテリアルの参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ReadBaseData(BinaryReader& Reader, const wstring& BinDataDir, const wstring& BinDataFile,
			vector<VertexPositionNormalTexture>& vertices, vector<VertexPositionNormalTangentTexture>& vertices_withtan,
			vector<uint16_t>& indices, vector<MaterialEx>& materials);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルメッシュデータの読み込み（ボーンメッシュ）
		@param[in]	Reader	リーダー
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
		static void ReadBaseBoneData(BinaryReader& Reader,const wstring& BinDataDir, const wstring& BinDataFile,
			vector<VertexPositionNormalTextureSkinning>& vertices, vector<VertexPositionNormalTangentTextureSkinning>& vertices_withtan,
			vector<uint16_t>& indices, vector<MaterialEx>& materials,
			vector<bsm::Mat4x4>& bonematrix, UINT& BoneCount, UINT& SampleCount);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルメッシュの作成（スタティックメッシュ・リーダ指定版）
		@param[in]	Reader	リーダー
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateStaticModelMeshBase(BinaryReader& Reader, const wstring& BinDataDir,
			const wstring& BinDataFile, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	タンジェント付きオリジナルメッシュの作成（スタティックメッシュ・リーダ指定版）
		@param[in]	Reader	リーダー
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateStaticModelMeshWithTangentBase(BinaryReader& Reader, const wstring& BinDataDir,
			const wstring& BinDataFile, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルメッシュの作成（ボーンメッシュ・リーダ指定版）
		@param[in]	Reader	リーダー
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateBoneModelMeshBase(BinaryReader& Reader, const wstring& BinDataDir,
			const wstring& BinDataFile, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	タンジェント付きオリジナルメッシュの作成（ボーンメッシュ・リーダ指定版）
		@param[in]	Reader	リーダー
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateBoneModelMeshWithTangentBase(BinaryReader& Reader, const wstring& BinDataDir,
			const wstring& BinDataFile, bool AccessWrite = false);



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
		@brief	タンジェント付きオリジナルメッシュの作成（スタティックメッシュ）
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateStaticModelMeshWithTangent(const wstring& BinDataDir,
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
		@brief	タンジェント付きオリジナルメッシュの作成（ボーンメッシュ）
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateBoneModelMeshWithTangent(const wstring& BinDataDir,
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
				Util::DemandCreate(Ptr->m_MeshPrimData.m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					auto PtrBackup = shared_ptr< BackupData<T> >(new  BackupData<T>());
					for (auto& v : vertices) {
						PtrBackup->m_Vertices.push_back(v);
					}
					Ptr->m_MeshPrimData.m_BackUpData = PtrBackup;
					//頂点バッファの作成
					MeshResource::CreateDynamicPrimitiveVertexBuffer(pDx11Device, vertices, pResult);
				});
			}
			else {
				Util::DemandCreate(Ptr->m_MeshPrimData.m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					//頂点バッファの作成
					MeshResource::CreatePrimitiveBuffer(pDx11Device, vertices, D3D11_BIND_VERTEX_BUFFER, pResult);
				});
			}
			//頂点数の設定
			Ptr->m_MeshPrimData.m_NumVertices = static_cast<UINT>(vertices.size());
			Ptr->m_MeshPrimData.m_MeshTypeIndex = typeid(T);
			Ptr->m_MeshPrimData.m_NumStride = sizeof(T);
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
				Util::DemandCreate(Ptr->m_MeshPrimData.m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					auto PtrBackup = shared_ptr< BackupData<T> >(new  BackupData<T>());
					for (auto& v : vertices) {
						PtrBackup->m_Vertices.push_back(v);
					}
					Ptr->m_MeshPrimData.m_BackUpData = PtrBackup;
					//頂点バッファの作成
					MeshResource::CreateDynamicPrimitiveVertexBuffer(pDx11Device, vertices, pResult);
				});
			}
			else {
				Util::DemandCreate(Ptr->m_MeshPrimData.m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					//頂点バッファの作成
					MeshResource::CreatePrimitiveBuffer(pDx11Device, vertices, D3D11_BIND_VERTEX_BUFFER, pResult);
				});
			}
			//頂点数の設定
			Ptr->m_MeshPrimData.m_NumVertices = static_cast<UINT>(vertices.size());
			//インデックスの作成
			Util::DemandCreate(Ptr->m_MeshPrimData.m_IndexBuffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				if (Ptr->m_MeshPrimData.m_BackUpData) {
					auto PtrBackup = dynamic_pointer_cast<BackupData<T>>(Ptr->m_MeshPrimData.m_BackUpData);
					for (auto& v : indices) {
						PtrBackup->m_Indices.push_back(v);
					}
				}
				//インデックスバッファの作成
				MeshResource::CreatePrimitiveBuffer(pDx11Device, indices, D3D11_BIND_INDEX_BUFFER, pResult);
			});

			//インデックス数の設定
			Ptr->m_MeshPrimData.m_NumIndicis = static_cast<UINT>(indices.size());
			Ptr->m_MeshPrimData.m_MeshTypeIndex = typeid(T);
			Ptr->m_MeshPrimData.m_NumStride = sizeof(T);
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
			auto Ptr = dynamic_pointer_cast< BackupData<T> >(m_MeshPrimData.m_BackUpData);
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
			auto Ptr = dynamic_pointer_cast< BackupData<T> >(m_MeshPrimData.m_BackUpData);
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



	//--------------------------------------------------------------------------------------
	///	マルチメッシュリソース
	//--------------------------------------------------------------------------------------
	class MultiMeshResource : public BaseResource {
		friend class ObjectFactory;
		//メッシュデータの配列
		vector<MeshPrimData> m_MeshPrimDataVec;
		//配列の上限チェック(エラーの場合は例外)
		void CheckMeshVecCount(size_t Index)const {
			if (Index >= m_MeshPrimDataVec.size()) {
				throw BaseException(
					L"メッシュ配列の上限を超えました",
					L"if (Index >= m_MeshPrimDataVec.size())",
					L"MultiMeshResource::CheckVecCount()"
				);
			}
		}
	protected:
		//派生クラスからのみアクセスできるアクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点バッファの設定
		@param[in]	VertexBuffer	頂点バッファ
		@param[in]	Index	データ配列のインデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetVertexBuffer(const ComPtr<ID3D11Buffer>& VertexBuffer, size_t Index) {
			CheckMeshVecCount(Index);
			m_MeshPrimDataVec[Index].m_VertexBuffer = VertexBuffer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点数の設定<br />
		頂点バッファの数と合わせなければならないので、慎重に使用する
		@param[in]	NumVertices	頂点数
		@param[in]	Index	データ配列のインデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetNumVertices(UINT NumVertices, size_t Index) {
			CheckMeshVecCount(Index);
			m_MeshPrimDataVec[Index].m_NumVertices = NumVertices;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックスバッファの設定
		@param[in]	IndexBuffer	インデックスバッファ
		@param[in]	Index	データ配列のインデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetIndexBuffer(const ComPtr<ID3D11Buffer>& IndexBuffer, size_t Index) {
			CheckMeshVecCount(Index);
			m_MeshPrimDataVec[Index].m_IndexBuffer = IndexBuffer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックス数の設定<br />
		インデックスバッファの数と合わせなければならないので、慎重に使用する
		@param[in]	IndexBuffer	インデックスバッファ
		@param[in]	Index	データ配列のインデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetNumIndicis(UINT NumIndicis, size_t Index) {
			CheckMeshVecCount(Index);
			m_MeshPrimDataVec[Index].m_NumIndicis = NumIndicis;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点の型とストライドを設定する
		@tparam	T	頂点の型
		@param[in]	Index	データ配列のインデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void SetVertexType(size_t Index) {
			CheckMeshVecCount(Index);
			m_MeshPrimDataVec[Index].m_MeshTypeIndex = typeid(T);
			m_MeshPrimDataVec[Index].m_NumStride = sizeof(T);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ<br />
		構築はスタティック関数を利用する
		*/
		//--------------------------------------------------------------------------------------
		MultiMeshResource();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MultiMeshResource();
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュデータの数の取得
		@return	メッシュデータの数
		*/
		//--------------------------------------------------------------------------------------
		size_t GetMeshVecCount() const {
			return m_MeshPrimDataVec.size();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュデータの配列の取得
		@return	メッシュデータの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<MeshPrimData>& GetMeshVec() const {
			return m_MeshPrimDataVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュデータの配列の取得（書き換え用）
		@return	メッシュデータの配列
		*/
		//--------------------------------------------------------------------------------------
		vector<MeshPrimData>& GetMeshVec(){
			return m_MeshPrimDataVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュデータの追加
		@return	追加されたインデックス
		*/
		//--------------------------------------------------------------------------------------
		size_t AddMesh(const MeshPrimData& Data) {
			auto retcount = m_MeshPrimDataVec.size();
			m_MeshPrimDataVec.push_back(Data);
			for (size_t i = 0; i < m_MeshPrimDataVec.size(); i++) {
				m_MeshPrimDataVec[i].m_MultiMeshIndex = i;
			}
			return retcount;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュデータの追加。メッシュデータ内のComPtrは共有されるので注意
		@param[in]	Res	メッシュリソース
		@return	追加されたインデックス
		*/
		//--------------------------------------------------------------------------------------
		size_t AddMesh(const shared_ptr<MeshResource>& Res) {
			auto retcount = m_MeshPrimDataVec.size();
			m_MeshPrimDataVec.push_back(Res->GetMashData());
			for (size_t i = 0; i < m_MeshPrimDataVec.size(); i++) {
				m_MeshPrimDataVec[i].m_MultiMeshIndex = i;
			}
			return retcount;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュデータの追加。メッシュデータ内のComPtrは共有されるので注意
		@param[in]	MeshKey	メッシュキー
		@return	追加されたインデックス
		*/
		//--------------------------------------------------------------------------------------
		size_t AddMesh(const wstring& MeshKey) {
			return AddMesh(App::GetApp()->GetResource<MeshResource>(MeshKey));
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの追加
		@tparam	T	頂点の型
		@param[in]	vertices	頂点の配列
		@param[in]	AccessWrite	上書き可能かどうか
		@return	追加されたインデックス
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		size_t AddMesh(const vector<T>& vertices, bool AccessWrite) {
			auto Ptr = MeshResource::CreateMeshResource<T>(vertices, AccessWrite);
			return AddMesh(Ptr->GetMeshData());
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュリソースの追加
		@tparam	T	頂点の型
		@param[in]	vertices	頂点の配列
		@param[in]	indices	インデックスの配列
		@param[in]	AccessWrite	上書き可能かどうか
		@return	追加されたインデックス
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		size_t AddMesh(const vector<T>& vertices, const vector<uint16_t>& indices, bool AccessWrite) {
			auto Ptr = MeshResource::CreateMeshResource<T>(vertices, indices, AccessWrite);
			return AddMesh(Ptr->GetMeshData());
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュデータを削除する（すぐに削除するので、インデックスを管理する場合は注意）
		@param[out]	Index	削除するインデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void RemoveMashData(size_t Index){
			CheckMeshVecCount(Index);
			m_MeshPrimDataVec.erase(m_MeshPrimDataVec.begin() + Index);
			for (size_t i = 0; i < m_MeshPrimDataVec.size(); i++) {
				m_MeshPrimDataVec[i].m_MultiMeshIndex = i;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュデータを得る
		@param[out]	data	取得するデータの参照
		@param[in]	Index	データ配列のインデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetMashData(MeshPrimData& data, size_t Index)const {
			CheckMeshVecCount(Index);
			data = m_MeshPrimDataVec[Index];
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点バッファの取得
		@param[in]	Index	データ配列のインデックス
		@return	頂点バッファ
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer> GetVertexBuffer(size_t Index) const {
			CheckMeshVecCount(Index);
			return m_MeshPrimDataVec[Index].m_VertexBuffer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックスバッファの取得
		@param[in]	Index	データ配列のインデックス
		@return	インデックスバッファ
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer> GetIndexBuffer(size_t Index) const {
			CheckMeshVecCount(Index);
			return m_MeshPrimDataVec[Index].m_IndexBuffer;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点数の取得
		@param[in]	Index	データ配列のインデックス
		@return	頂点数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumVertices(size_t Index) const {
			CheckMeshVecCount(Index);
			return  m_MeshPrimDataVec[Index].m_NumVertices;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックス数の取得
		@param[in]	Index	データ配列のインデックス
		@return	インデックス数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumIndicis(size_t Index) const {
			CheckMeshVecCount(Index);
			return m_MeshPrimDataVec[Index].m_NumIndicis;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ストライド数の取得
		@param[in]	Index	データ配列のインデックス
		@return	ストライド数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumStride(size_t Index) const {
			CheckMeshVecCount(Index);
			return m_MeshPrimDataVec[Index].m_NumStride;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画トポロジーの取得
		@param[in]	Index	データ配列のインデックス
		@return	描画トポロジー
		*/
		//--------------------------------------------------------------------------------------
		D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(size_t Index) const {
			CheckMeshVecCount(Index);
			return m_MeshPrimDataVec[Index].m_PrimitiveTopology;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画トポロジーの設定
		@param[in]	Topology	描画トポロジー
		@param[in]	Index	データ配列のインデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology, size_t Index) {
			CheckMeshVecCount(Index);
			m_MeshPrimDataVec[Index].m_PrimitiveTopology = Topology;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	type_indexの取得
		@param[in]	Index	データ配列のインデックス
		@return	type_index
		*/
		//--------------------------------------------------------------------------------------
		type_index GetTypeIndex(size_t Index) const {
			CheckMeshVecCount(Index);
			return m_MeshPrimDataVec[Index].m_MeshTypeIndex;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュとワールド行列の間を補完する行列の取得(メッシュに設定する場合)<br />
		メッシュのサイズや向き、中心の位置がゲームオブジェクトのワールド行列と差異がある場合、<br />
		その間を補完する行列を設定できる。
		@return	メッシュとワールド行列の間を補完する行列
		*/
		//--------------------------------------------------------------------------------------
		const bsm::Mat4x4& GetMeshToTransformMatrix(size_t Index) const {
			CheckMeshVecCount(Index);
			return m_MeshPrimDataVec[Index].m_MeshToTransformMatrix;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュとワールド行列の間を補完する行列の取得(メッシュに設定する場合)<br />
		メッシュのサイズや向き、中心の位置がゲームオブジェクトのワールド行列と差異がある場合、<br />
		その間を補完する行列を設定できる。
		@param[in]	Mat	設定する行列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetMeshToTransformMatrix(const bsm::Mat4x4& Mat, size_t Index) {
			CheckMeshVecCount(Index);
			m_MeshPrimDataVec[Index].m_MeshToTransformMatrix = Mat;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュに設定したメッシュとワールド行列の間を補完する行列を使うかどうか
		@return　メッシュに設定したメッシュとワールド行列の間を補完する行列を使うかどうか
		*/
		//--------------------------------------------------------------------------------------
		bool IsUseMeshToTransformMatrix(size_t Index) const {
			CheckMeshVecCount(Index);
			return m_MeshPrimDataVec[Index].m_UseMeshToTransformMatrix;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッシュに設定したメッシュとワールド行列の間を補完する行列を使うかどうかを設定
		@param[in]	b	使用するならtrue
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		void SetUseMeshToTransformMatrix(bool b, size_t Index) {
			CheckMeshVecCount(Index);
			m_MeshPrimDataVec[Index].m_UseMeshToTransformMatrix = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テクスチャリソースの設定(メッシュ毎の場合)
		@param[in]	TextureRes	テクスチャリソース
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const shared_ptr<TextureResource>& TextureRes, size_t Index) {
			CheckMeshVecCount(Index);
			m_MeshPrimDataVec[Index].m_TextureResource = TextureRes;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テクスチャリソースの設定(メッシュ毎の場合)
		@param[in]	TextureKey	登録されているテクスチャキー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const wstring& TextureKey, size_t Index) {
			CheckMeshVecCount(Index);
			this->SetTextureResource(App::GetApp()->GetResource<TextureResource>(TextureKey), Index);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テクスチャリソースの取得(メッシュ毎の場合)
		@return	テクスチャリソース
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<TextureResource> GetTextureResource(size_t Index) const {
			CheckMeshVecCount(Index);
			//テクスチャがなければnullを返す
			auto shptr = m_MeshPrimDataVec[Index].m_TextureResource.lock();
			if (shptr) {
				return shptr;
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マテリアル配列の取得
		@param[in]	Index	データ配列のインデックス
		@return	マテリアルの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<MaterialEx>& GetMaterialExVec(size_t Index)const {
			CheckMeshVecCount(Index);
			return m_MeshPrimDataVec[Index].m_MaterialExVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マテリアル配列の取得(書き込み用)
		@param[in]	Index	データ配列のインデックス
		@return	マテリアルの配列
		*/
		//--------------------------------------------------------------------------------------
		vector<MaterialEx>& GetMaterialExVec(size_t Index) {
			CheckMeshVecCount(Index);
			return m_MeshPrimDataVec[Index].m_MaterialExVec;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	スキニングするかどうか.<br />
		仮想関数なので、派生クラスで独自に多重定義する
		@param[in]	Index	データ配列のインデックス
		@return	スキニングする場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		virtual bool IsSkining(size_t Index) const {
			CheckMeshVecCount(Index);
			return m_MeshPrimDataVec[Index].m_IsSkining;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ボーン数を得る
		@param[in]	Index	データ配列のインデックス
		@return	ボーン数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetBoneCount(size_t Index) const {
			CheckMeshVecCount(Index);
			return m_MeshPrimDataVec[Index].m_BoneCount;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	サンプリング数を得る
		@param[in]	Index	データ配列のインデックス
		@return	サンプリング数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetSampleCount(size_t Index) const {
			CheckMeshVecCount(Index);
			return m_MeshPrimDataVec[Index].m_SampleCount;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	サンプリングされた行列の配列を得る
		@param[in]	Index	データ配列のインデックス
		@return	サンプリングされた行列の配列の参照
		*/
		//--------------------------------------------------------------------------------------
		const vector<bsm::Mat4x4>& GetSampleMatrixVec(size_t Index) const {
			CheckMeshVecCount(Index);
			return m_MeshPrimDataVec[Index].m_SampleMatrixVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	バックアップデータの取得
		@tparam	T	頂点の型
		@param[in]	Index	データ配列のインデックス
		@return	バックアップデータの配列。
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		vector<T>& GetBackupVerteces(size_t Index) const {
			CheckMeshVecCount(Index);
			auto Ptr = dynamic_pointer_cast< BackupData<T> >(m_MeshPrimDataVec[Index].m_BackUpData);
			if (!Ptr) {
				throw BaseException(
					L"バックアップをT型にキャストできません",
					Util::GetWSTypeName<T>(),
					L"MultiMeshResource::GetBackupVerteces<T>()"
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
		@param[in]	Index	データ配列のインデックス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void UpdateVirtexBuffer(const vector<T>& NewBuffer, size_t Index) {
			CheckMeshVecCount(Index);
			auto Ptr = dynamic_pointer_cast< BackupData<T> >(m_MeshPrimDataVec[Index].m_BackUpData);
			if (!Ptr) {
				throw BaseException(
					L"バックアップをT型にキャストできません",
					Util::GetWSTypeName<T>(),
					L"MultiMeshResource::UpdateVirtexBuffer<T>()"
				);
			}

			if (NewBuffer.size() != Ptr->m_Vertices.size()) {
				// Map失敗
				throw BaseException(
					L"変更する頂点の数がバックアップと違います",
					L"if (NewBuffer.size() != Ptr->m_Vertices.size())",
					L"MultiMeshResource::UpdateVirtexBuffer<T>()"
				);
			}
			//座標を変更する
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			//頂点バッファをリソースから取り出す
			auto pVertexBuffer = GetVertexBuffer(Index).Get();

			//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
			D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
			D3D11_MAPPED_SUBRESOURCE mappedBuffer;
			//頂点のマップ
			if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))) {
				// Map失敗
				throw BaseException(
					L"頂点のMapに失敗しました。",
					L"if(FAILED(pID3D11DeviceContext->Map()))",
					L"MultiMeshResource::UpdateVirtexBuffer<T>()"
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチメッシュリソースの作成
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static  shared_ptr<MultiMeshResource> CreateMultiMeshResource() {
			auto Ptr = ObjectFactory::Create<MultiMeshResource>();
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルマルチメッシュの作成（スタティックメッシュ）
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MultiMeshResource> CreateStaticModelMultiMesh(const wstring& BinDataDir,
			const wstring& BinDataFile, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	タンジェント付きオリジナルマルチメッシュの作成（スタティックメッシュ）
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MultiMeshResource> CreateStaticModelMultiMeshWithTangent(const wstring& BinDataDir,
			const wstring& BinDataFile, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オリジナルマルチメッシュの作成（ボーンメッシュ）
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MultiMeshResource> CreateBoneModelMultiMesh(const wstring& BinDataDir,
			const wstring& BinDataFile, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	タンジェント付きオリジナルマルチメッシュの作成（ボーンメッシュ）
		@param[in]	BinDataDir	基準ディレクトリ
		@param[in]	BinDataFile	データファイル名
		@param[in]	AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MultiMeshResource> CreateBoneModelMultiMeshWithTangent(const wstring& BinDataDir,
			const wstring& BinDataFile, bool AccessWrite = false);

	};

	class AudioResource;
	//--------------------------------------------------------------------------------------
	///	マルチサウンドで使用するアイテム
	//--------------------------------------------------------------------------------------
	struct SoundItem {
		weak_ptr<AudioResource> m_AudioResource;
		IXAudio2SourceVoice* m_SourceVoice;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		SoundItem() :
			m_SourceVoice(nullptr)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		~SoundItem() {
		}
	};

	//--------------------------------------------------------------------------------------
	///	マルチオーディオ
	/// ＊コンポーネントではないのでシーンに持たせることができる
	//--------------------------------------------------------------------------------------
	class MultiAudioObject : public ObjectInterface {
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		MultiAudioObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MultiAudioObject();
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オーディオリソースを取得する
		@return	オーディオリソース
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<AudioResource> GetAudioResource(const wstring& ResKey, bool ExceptionActive = true) const;
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オーディオリソースを追加する
		@param[in]	ResKey リソース名
		@return	追加されたオーディオリソース
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<AudioResource> AddAudioResource(const wstring& ResKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オーディオリソースを削除する
		@param[in]	ResKey リソース名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void RemoveAudioResource(const wstring& ResKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オーディオ再生を開始する
		@param[in]	ResKey リソース名
		@param[in]	Buffer バッファ
		@param[in]	Volume ボリューム
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Start(const wstring& ResKey, const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オーディオ再生を開始する
		@param[in]	ResKey リソース名
		@param[in]	LoopCount ループ回数
		@param[in]	Volume ボリューム
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Start(const wstring& ResKey, size_t LoopCount = 0, float Volume = 1.0f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オーディオをストップする
		@param[in]	ResKey リソース名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Stop(const wstring& ResKey);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	初期化処理（空関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override {}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
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
		DoubleDraw,	//背面描画の後、前面描画
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
		@brief コンテンツの作成後の処理（Dx12と互換性を保つためにある）
		@param[in]	ShadowActive	影が有効かどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterInitContents(bool ShadowActive) {}
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
		virtual void ClearDefaultViews(const bsm::Col4& col = bsm::Col4(0, 0, 0, 1.0f));
		//--------------------------------------------------------------------------------------
		/*!
		@brief	通常描画の開始
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void StartDefaultDraw();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	通常描画の終了
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void EndDefaultDraw();
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
		//以下、レンダリングステートの各enum値によるデバイスへの設定
		//--------------------------------------------------------------------------------------
		/*!
		@brief	BlendStateの設定
		@param[in]	pContext	ID3D11DeviceContext2コンテキストのポインタ
		@param[in]	state	BlendState値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBlendState(ID3D11DeviceContext2* pContext, BlendState state) {
			switch (state) {
			case BlendState::Opaque:
				pContext->OMSetBlendState(GetOpaque(), nullptr, 0xffffffff);
				break;
			case BlendState::AlphaBlend:
				pContext->OMSetBlendState(GetAlphaBlendEx(), nullptr, 0xffffffff);
				break;
			case BlendState::Additive:
				pContext->OMSetBlendState(GetAdditive(), nullptr, 0xffffffff);
				break;
			case BlendState::NonPremultiplied:
				pContext->OMSetBlendState(GetNonPremultiplied(), nullptr, 0xffffffff);
				break;
			default:
				//デフォルトは塗りつぶし
				pContext->OMSetBlendState(GetOpaque(), nullptr, 0xffffffff);
				break;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	DepthStencilStateの設定
		@param[in]	pContext	ID3D11DeviceContext2コンテキストのポインタ
		@param[in]	state	DepthStencilState値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDepthStencilState(ID3D11DeviceContext2* pContext, DepthStencilState state) {
			switch (state) {
			case DepthStencilState::None:
				pContext->OMSetDepthStencilState(GetDepthNone(), 0);
				break;
			case DepthStencilState::Default:
				pContext->OMSetDepthStencilState(GetDepthDefault(), 0);
				break;
			case DepthStencilState::Read:
				pContext->OMSetDepthStencilState(GetDepthRead(), 0);
				break;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	RasterizerStateの設定
		@param[in]	pContext	ID3D11DeviceContext2コンテキストのポインタ
		@param[in]	state	RasterizerState値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetRasterizerState(ID3D11DeviceContext2* pContext, RasterizerState state) {
			switch (state) {
			case RasterizerState::CullBack:
				//ラスタライザステート（表描画）
				pContext->RSSetState(GetCullBack());
				break;
			case RasterizerState::CullFront:
				//ラスタライザステート（裏描画）
				pContext->RSSetState(GetCullFront());
				break;
			case RasterizerState::CullNone:
				//ラスタライザステート（両面描画）
				pContext->RSSetState(GetCullNone());
				break;
			case RasterizerState::Wireframe:
				//ラスタライザステート（ワイアフレーム）
				pContext->RSSetState(GetWireframe());
				break;
			case RasterizerState::DoubleDraw:
				//ラスタライザステート（裏描画を設定）
				pContext->RSSetState(GetCullFront());
				break;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1つのSamplerStateの設定
		@param[in]	pContext	ID3D11DeviceContext2コンテキストのポインタ
		@param[in]	state	SamplerState値
		@param[in]	slot	スロット番号
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetSamplerState(ID3D11DeviceContext2* pContext, SamplerState state,UINT slot) {
			ID3D11SamplerState* pSampler = nullptr;
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			switch (state) {
			case SamplerState::SamplerNone:
				//サンプラークリア
				pContext->PSSetSamplers(slot, 1, pNullSR);
				break;
			case SamplerState::PointWrap:
				pSampler = GetPointWrap();
				pContext->PSSetSamplers(slot, 1, &pSampler);
				break;
			case SamplerState::PointClamp:
				pSampler = GetPointClamp();
				pContext->PSSetSamplers(slot, 1, &pSampler);
				break;
			case SamplerState::LinearWrap:
				pSampler = GetLinearWrap();
				pContext->PSSetSamplers(slot, 1, &pSampler);
				break;
			case SamplerState::LinearClamp:
				pSampler = GetLinearClamp();
				pContext->PSSetSamplers(slot, 1, &pSampler);
				break;
			case SamplerState::AnisotropicWrap:
				pSampler = GetAnisotropicWrap();
				pContext->PSSetSamplers(slot, 1, &pSampler);
				break;
			case SamplerState::AnisotropicClamp:
				pSampler = GetAnisotropicClamp();
				pContext->PSSetSamplers(slot, 1, &pSampler);
				break;
			case SamplerState::ComparisonLinear:
				pSampler = GetComparisonLinear();
				pContext->PSSetSamplers(slot, 1, &pSampler);
				break;
			default:
				//デフォルトはClamp
				pSampler = GetLinearClamp();
				pContext->PSSetSamplers(slot, 1, &pSampler);
				break;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	SamplerStateをすべてクリアする
		@param[in]	pContext	ID3D11DeviceContext2コンテキストのポインタ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetSamplerAllClear(ID3D11DeviceContext2* pContext) {
			ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
			pContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
		}


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
		virtual void ClearViews(const bsm::Col4& col = bsm::Col4(0, 0, 0, 1.0f)) = 0;
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
		virtual void ClearViews(const bsm::Col4& col = bsm::Col4(0, 0, 0, 1.0f)) override;
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
		virtual void ClearViews(const bsm::Col4& col = bsm::Col4(0, 0, 0, 1.0f)) override;
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
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		ShaderResource();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShaderResource();
	public:
		static const int m_LightMax{ 3 };
		//--------------------------------------------------------------------------------------
		/*!
		@brief	バイナリファイルを読み込む
		@param[in]	fileName	ファイル名
		@param[out]	Data	バイナリデータの戻り
		@param[out]	CsoSz	サイズの戻り
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ReadBinaryFile(const wstring& fileName, unique_ptr<uint8_t[]>& Data, size_t& CsoSz);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	バイナリデータから頂点シェーダを作成する
		@param[in]	Data	バイナリデータ
		@param[in]	CsoSz	サイズ
		@param[out]	pResult	受け取るシェーダ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateVertexShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11VertexShader** pResult);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	バイナリデータからインプットレイアウトを作成する
		@param[in]	Data	バイナリデータ
		@param[in]	CsoSz	サイズ
		@param[in]	pElement	頂点定義
		@param[in]	NumElement	頂点定義の数
		@param[out]	pResult	受け取るレイアウト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateInputLayout(unique_ptr<uint8_t[]>& Data, size_t CsoSz,
			const D3D11_INPUT_ELEMENT_DESC* pElement, UINT NumElement, ID3D11InputLayout** pResult);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	バイナリデータからピクセルシェーダを作成する
		@param[in]	Data	バイナリデータ
		@param[in]	CsoSz	サイズ
		@param[out]	pResult	受け取るシェーダ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreatePixelShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11PixelShader** pResult);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	バイナリデータからジオメトリシェーダを作成する
		@param[in]	Data	バイナリデータ
		@param[in]	CsoSz	サイズ
		@param[out]	pResult	受け取るシェーダ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11GeometryShader** pResult);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	バイナリデータからジオメトリシェーダを作成する
		@param[in]	Data	バイナリデータ
		@param[in]	CsoSz	サイズ
		@param[in]	SOEntries	定義されたエントリの配列
		@param[in]	Stride	1個のストライドサイズ
		@param[in]	NumStride	ストライド数
		@param[out]	pResult	受け取るシェーダ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz,
			const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries, UINT Stride, UINT NumStride, ID3D11GeometryShader** pResult);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	バイナリデータからコンピュートシェーダを作成する
		@param[in]	Data	バイナリデータ
		@param[in]	CsoSz	サイズ
		@param[out]	pResult	受け取るシェーダ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateComputeShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11ComputeShader** pResult);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスタントバッファを作成する
		@param[in]	BuffSize	サイズ
		@param[out]	pResult	受け取るコンスタントバッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void CreateConstantBuffer(UINT BuffSize, ID3D11Buffer** pResult);
	protected:
		//ミューテックス
		std::mutex MutexBase;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点シェーダアクセッサ
		@param[in]	Filename	ファイル名
		@param[out]	VSPtr	検証するシェーダ（COM）
		@return	シェーダインターフェイス
		*/
		//--------------------------------------------------------------------------------------
		ID3D11VertexShader* GetVertexShaderBase(const wstring& Filename, ComPtr<ID3D11VertexShader>& VSPtr) {
			return Util::DemandCreate(VSPtr, MutexBase, [&](ID3D11VertexShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//シェーダの読み込み
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateVertexShader(data, cso_sz, pResult);
			});
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インプットレイアウトアクセッサ
		@param[in]	Filename	ファイル名
		@param[out]	ILPtr	検証するシェーダ（COM）
		@return	シェーダインターフェイス
		*/
		//--------------------------------------------------------------------------------------
		ID3D11InputLayout* GetInputLayoutBase(const wstring& Filename, ComPtr<ID3D11InputLayout>& ILPtr,
			const D3D11_INPUT_ELEMENT_DESC* pElem, UINT NumElements) {
			return Util::DemandCreate(ILPtr, MutexBase, [&](ID3D11InputLayout** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//シェーダの読み込み
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateInputLayout(
					data,
					cso_sz,
					pElem,
					NumElements,
					pResult);
			});
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスタントバッファアクセッサ
		@param[in]	BuffSize	サイズ
		@param[out]	Buffer	検証するバッファ（COM）
		@return	シェーダインターフェイス
		*/
		//--------------------------------------------------------------------------------------
		ID3D11Buffer* GetConstantBufferBase(UINT BuffSize, ComPtr<ID3D11Buffer>& Buffer) {
			return Util::DemandCreate(Buffer, MutexBase, [&](ID3D11Buffer** pResult)
			{
				ShaderResource::CreateConstantBuffer(BuffSize, pResult);
			});
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ピクセルシェーダアクセッサ
		@param[in]	Filename	ファイル名
		@param[out]	PSPtr	検証するシェーダ（COM）
		@return	シェーダインターフェイス
		*/
		//--------------------------------------------------------------------------------------
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ジオメトリシェーダアクセッサ
		@param[in]	Filename	ファイル名
		@param[out]	GSPtr	検証するシェーダ（COM）
		@return	シェーダインターフェイス
		*/
		//--------------------------------------------------------------------------------------
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ジオメトリシェーダアクセッサ
		@param[in]	Filename	ファイル名
		@param[in]	SOEntries	エントリの配列
		@param[in]	Stride	ストライド
		@param[in]	NumStride	ストライド数
		@param[out]	GSPtr	検証するシェーダ（COM）
		@return	シェーダインターフェイス
		*/
		//--------------------------------------------------------------------------------------
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンピュートシェーダアクセッサ
		@param[in]	Filename	ファイル名
		@param[out]	CSPtr	検証するシェーダ（COM）
		@return	シェーダインターフェイス
		*/
		//--------------------------------------------------------------------------------------
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
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		ShaderResource(const ShaderResource&) = delete;
		ShaderResource& operator=(const ShaderResource&) = delete;
		//ムーブ禁止
		ShaderResource(const ShaderResource&&) = delete;
		ShaderResource& operator=(const ShaderResource&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	///	コンスタントバッファ(シングルトン)
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
		ConstantBuffer<ShaderType, ConstantType>(ConstantBuffer const&) = delete;
		ConstantBuffer<ShaderType, ConstantType>& operator=(ConstantBuffer<ShaderType, ConstantType> const&) = delete;
		//ムーブ禁止
		ConstantBuffer(const ConstantBuffer&&) = delete;
		ConstantBuffer& operator=(const ConstantBuffer&&) = delete;
	public:
		//公開アクセッサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスタントバッファを得る
		@return	コンスタントバッファ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11Buffer* GetBuffer() {
			return GetConstantBufferBase(sizeof(ConstantType), m_Buffer);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インスタンスを得る
		@return	クラスのインスタンス
		*/
		//--------------------------------------------------------------------------------------
		static unique_ptr<ShaderType, Deleter>& GetPtr() {
			if (!m_Ptr) {
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};


	//--------------------------------------------------------------------------------------
	///	コンスタントバッファ
	//--------------------------------------------------------------------------------------
	template<typename ConstantType>
	class CBuffer : public ShaderResource {
		ComPtr<ID3D11Buffer> m_Buffer;
		//コピー禁止
		CBuffer(CBuffer const&) = delete;
		CBuffer& operator=(CBuffer<ConstantType> const&) = delete;
		//ムーブ禁止
		CBuffer(const CBuffer&&) = delete;
		CBuffer& operator=(const CBuffer&&) = delete;
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		CBuffer() : ShaderResource() {
			GetConstantBufferBase(sizeof(ConstantType), m_Buffer);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CBuffer() {}
		//公開アクセッサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンスタントバッファを得る
		@return	コンスタントバッファ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11Buffer* GetBuffer() {
			return GetConstantBufferBase(sizeof(ConstantType), m_Buffer);
		}
	};



	//--------------------------------------------------------------------------------------
	///	頂点シェーダ(シングルトン)
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点シェーダを得る
		@return	頂点シェーダ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11VertexShader* GetShader() {
			return GetVertexShaderBase(m_Filename, m_VSPtr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インプットレイアウトを得る
		@return	インプットレイアウト
		*/
		//--------------------------------------------------------------------------------------
		ID3D11InputLayout* GetInputLayout() {
			return GetInputLayoutBase(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インスタンスを得る
		@return	クラスのインスタンス
		*/
		//--------------------------------------------------------------------------------------
		static unique_ptr<ShaderType, Deleter>& GetPtr() {
			if (!m_Ptr) {
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};

	//--------------------------------------------------------------------------------------
	///	頂点シェーダ
	//--------------------------------------------------------------------------------------
	template<typename VirtexType>
	class VShader : public ShaderResource {
		wstring m_Filename;
		ComPtr<ID3D11VertexShader> m_VSPtr;
		ComPtr<ID3D11InputLayout> m_ILPtr;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		VShader() : ShaderResource(), m_Filename() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Filename	ファイル名
		*/
		//--------------------------------------------------------------------------------------
		VShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename) {
			GetVertexShaderBase(m_Filename, m_VSPtr);
			GetInputLayoutBase(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~VShader() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	構築
		@param[in]	Filename	ファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Create(const wstring& Filename) {
			m_Filename = Filename;
			m_VSPtr = nullptr;
			m_ILPtr = nullptr;
			GetVertexShaderBase(m_Filename, m_VSPtr);
			GetInputLayoutBase(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	頂点シェーダを得る
		@return	頂点シェーダ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11VertexShader* GetShader() {
			return GetVertexShaderBase(m_Filename, m_VSPtr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インプットレイアウトを得る
		@return	インプットレイアウト
		*/
		//--------------------------------------------------------------------------------------
		ID3D11InputLayout* GetInputLayout() {
			return GetInputLayoutBase(m_Filename, m_ILPtr, VirtexType::GetVertexElement(), VirtexType::GetNumElements());
		}
	};

	//--------------------------------------------------------------------------------------
	///	ピクセルシェーダ(シングルトン)
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ピクセルシェーダを得る
		@return	ピクセルシェーダ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11PixelShader* GetShader() {
			return GetPixelShaderBase(m_Filename, m_PSPtr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インスタンスを得る
		@return	クラスのインスタンス
		*/
		//--------------------------------------------------------------------------------------
		static unique_ptr<ShaderType, Deleter>& GetPtr() {
			if (!m_Ptr) {
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};

	//--------------------------------------------------------------------------------------
	///	ピクセルシェーダ
	//--------------------------------------------------------------------------------------
	class PShader : public ShaderResource {
		wstring m_Filename;
		ComPtr<ID3D11PixelShader> m_PSPtr;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		PShader() : ShaderResource(), m_Filename() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Filename	ファイル名
		*/
		//--------------------------------------------------------------------------------------
		PShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename) {
			GetPixelShaderBase(m_Filename, m_PSPtr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PShader() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	構築
		@param[in]	Filename	ファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Create(const wstring& Filename) {
			m_Filename = Filename;
			m_PSPtr = nullptr;
			GetPixelShaderBase(m_Filename, m_PSPtr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ピクセルシェーダを得る
		@return	ピクセルシェーダ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11PixelShader* GetShader() {
			return GetPixelShaderBase(m_Filename, m_PSPtr);
		}
	};
	//--------------------------------------------------------------------------------------
	///	ジオメトリシェーダ(シングルトン)
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
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スロットエントリの登録
		@param[in]	SOEntries	エントリの配列
		@param[in]	Stride	ストライド
		@param[in]	NumStride	ストライド数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ジオメトリシェーダを得る
		@return	ジオメトリシェーダ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11GeometryShader* GetShader() {
			if (m_SOEntries.size() > 0) {
				return GetGeometryShaderBase(m_Filename, m_SOEntries, m_Stride, m_NumStride, m_GSPtr);
			}
			else {
				return GetGeometryShaderBase(m_Filename, m_GSPtr);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インスタンスを得る
		@return	クラスのインスタンス
		*/
		//--------------------------------------------------------------------------------------
		static unique_ptr<ShaderType, Deleter>& GetPtr() {
			if (!m_Ptr) {
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};


	//--------------------------------------------------------------------------------------
	///	ジオメトリシェーダ
	//--------------------------------------------------------------------------------------
	class GShader : public ShaderResource {
		ComPtr<ID3D11GeometryShader> m_GSPtr;
		wstring m_Filename;
		vector<D3D11_SO_DECLARATION_ENTRY> m_SOEntries;
		UINT m_Stride;
		UINT m_NumStride;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		GShader() : ShaderResource(), m_Filename() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Filename	ファイル名
		*/
		//--------------------------------------------------------------------------------------
		GShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GShader() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スロットエントリの登録
		@param[in]	SOEntries	エントリの配列
		@param[in]	Stride	ストライド
		@param[in]	NumStride	ストライド数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	構築
		@param[in]	Filename	ファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ジオメトリシェーダを得る
		@return	ジオメトリシェーダ
		*/
		//--------------------------------------------------------------------------------------
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
	///	コンピュートシェーダ(シングルトン)
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンピュートシェーダを得る
		@return	コンピュートシェーダ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11ComputeShader* GetShader() {
			return GetComputeShaderBase(m_Filename, m_CSPtr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インスタンスを得る
		@return	クラスのインスタンス
		*/
		//--------------------------------------------------------------------------------------
		static unique_ptr<ShaderType, Deleter>& GetPtr() {
			if (!m_Ptr) {
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};

	//--------------------------------------------------------------------------------------
	///	コンピュートシェーダ
	//--------------------------------------------------------------------------------------
	class CShader : public ShaderResource {
		wstring m_Filename;
		ComPtr<ID3D11ComputeShader> m_CSPtr;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		CShader() : ShaderResource(), m_Filename() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Filename	ファイル名
		*/
		//--------------------------------------------------------------------------------------
		CShader(const wstring& Filename) : ShaderResource(), m_Filename(Filename) {
			GetComputeShaderBase(m_Filename, m_CSPtr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CShader() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	構築
		@param[in]	Filename	ファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Create(const wstring& Filename) {
			m_Filename = Filename;
			m_CSPtr = nullptr;
			GetComputeShaderBase(m_Filename, m_CSPtr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンピュートシェーダを得る
		@return	コンピュートシェーダ
		*/
		//--------------------------------------------------------------------------------------
		ID3D11ComputeShader* GetShader() {
			return GetComputeShaderBase(m_Filename, m_CSPtr);
		}
	};

	//これより個別シェーダー
	//コンスタントバッファ宣言用マクロ
#define DECLARE_DX11_CONSTANT_BUFFER(CBName,CBStruct) class CBName : public ConstantBuffer<CBName,CBStruct> { public: };

	//シェーダ宣言用マクロ
#define DECLARE_DX11_VERTEX_SHADER(ShaderName,VertexType) class ShaderName : public VertexShader<ShaderName,VertexType>{ public: ShaderName(); };

#define DECLARE_DX11_PIXEL_SHADER(ShaderName) class ShaderName : public PixelShader<ShaderName>{ public: ShaderName(); };

#define DECLARE_DX11_GEOMETRY_SHADER(ShaderName) class ShaderName : public GeometryShader<ShaderName>{ public: ShaderName(); };

	//コンスタントバッファ実体用マク
#define IMPLEMENT_DX11_CONSTANT_BUFFER(CBName) unique_ptr<CBName, CBName::Deleter> CBName::m_Ptr;

	//シェーダ実体用マクロ
#define IMPLEMENT_DX11_VERTEX_SHADER(ShaderName,CsoFilename) unique_ptr<ShaderName, ShaderName::Deleter> ShaderName::m_Ptr; ShaderName::ShaderName() : VertexShader(CsoFilename){}

#define IMPLEMENT_DX11_PIXEL_SHADER(ShaderName,CsoFilename) unique_ptr<ShaderName, ShaderName::Deleter> ShaderName::m_Ptr; ShaderName::ShaderName() : PixelShader(CsoFilename){}

#define IMPLEMENT_DX11_GEOMETRY_SHADER(ShaderName,CsoFilename)	unique_ptr<ShaderName, ShaderName::Deleter> ShaderName::m_Ptr; ShaderName::ShaderName() : GeometryShader(CsoFilename){}




}
//end basecross
