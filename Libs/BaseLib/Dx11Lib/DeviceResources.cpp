/*!
@file DeviceResources.cpp
@brief テクスチャ、メッシュ、Dx11デバイスリソース実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct TextureResource::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct TextureResource::Impl {
		ComPtr<ID3D11ShaderResourceView> m_ShaderResView;	//リソースビュー
		wstring m_FileName;		//テクスチャファイルへのパス

		Impl(const wstring& FileName, const wstring& TexType = L"WIC");
		~Impl() {}
		//ミューテックス
		std::mutex Mutex;

	};

	TextureResource::Impl::Impl(const wstring& FileName, const wstring& TexType) :
		m_FileName(FileName) {
		try {
			if (FileName == L"") {
				throw BaseException(
					L"ファイルが指定されていません",
					L"if(FileName == L\"\")",
					L"Texture::Impl::Impl()"
				);
			}
			DWORD RetCode;
			RetCode = GetFileAttributes(m_FileName.c_str());
			if (RetCode == -1) {
				wstring patherr = m_FileName;
				throw BaseException(
					L"ファイルが存在しません",
					patherr.c_str(),
					L"Texture::Impl::Impl()"
				);
			}
			HRESULT hr;
			//テクスチャ作成
			DirectX::TexMetadata metadata;
			DirectX::ScratchImage image;

			if (TexType == L"WIC") {
				//ファイル拡張子の調査
				wchar_t Drivebuff[_MAX_DRIVE];
				wchar_t Dirbuff[_MAX_DIR];
				wchar_t FileNamebuff[_MAX_FNAME];
				wchar_t Extbuff[_MAX_EXT];

				::ZeroMemory(Drivebuff, sizeof(Drivebuff));
				::ZeroMemory(Dirbuff, sizeof(Dirbuff));
				::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
				::ZeroMemory(Extbuff, sizeof(Extbuff));

				_wsplitpath_s(m_FileName.c_str(),
					Drivebuff, _MAX_DRIVE,
					Dirbuff, _MAX_DIR,
					FileNamebuff, _MAX_FNAME,
					Extbuff, _MAX_EXT);

				wstring ExtStr = Extbuff;

				if (ExtStr == L".dds" || ExtStr == L".DDS") {
					ThrowIfFailed(
						DirectX::LoadFromDDSFile(m_FileName.c_str(), 0, &metadata, image),
						L"テクスチャの読み込みに失敗しました",
						m_FileName,
						L"Texture::Impl::Impl()"
					);
				}
				else if (ExtStr == L".tga" || ExtStr == L".TGA") {
					ThrowIfFailed(
						DirectX::LoadFromTGAFile(m_FileName.c_str(), &metadata, image),
						L"テクスチャの読み込みに失敗しました",
						m_FileName,
						L"Texture::Impl::Impl()"
					);
				}
				else {
					ThrowIfFailed(
						DirectX::LoadFromWICFile(m_FileName.c_str(), 0, &metadata, image),
						L"テクスチャの読み込みに失敗しました",
						m_FileName,
						L"Texture::Impl::Impl()"
					);
				}
			}
			else if (TexType == L"DDS") {
				ThrowIfFailed(
					DirectX::LoadFromDDSFile(m_FileName.c_str(), 0, &metadata, image),
					L"テクスチャの読み込みに失敗しました",
					m_FileName,
					L"Texture::Impl::Impl()"
				);
			}
			else if (TexType == L"TGA") {
				ThrowIfFailed(
					DirectX::LoadFromTGAFile(m_FileName.c_str(), &metadata, image),
					L"テクスチャの読み込みに失敗しました",
					m_FileName,
					L"Texture::Impl::Impl()"
				);
			}
			else {
				throw BaseException(
					L"そのタイプは対応していません",
					TexType.c_str(),
					L"Texture::Impl::Impl()"
				);
			}

			//デバイスとコンテキストインターフェイスの取得
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			ID3D11DeviceContext* pID3D11DeviceContex = Dev->GetD3DDeviceContext();

			Util::DemandCreate(m_ShaderResView, Mutex, [&](ID3D11ShaderResourceView** pResult) -> HRESULT
			{
				// 画像からシェーダリソースViewの作成
				hr = ThrowIfFailed(CreateShaderResourceView(pDx11Device, image.GetImages(), image.GetImageCount(), metadata, pResult),
					L"シェーダーリソースビューを作成できません",
					L"if( FAILED( CreateShaderResourceView() ) )",
					L"Texture::Impl::Impl()"
				);
				return hr;
			});


		}
		catch (...) {
			throw;
		}
	}



	//--------------------------------------------------------------------------------------
	//	class TextureResource : public BaseResource;
	//	用途: テクスチャのラッピングクラス
	//--------------------------------------------------------------------------------------

	shared_ptr<TextureResource> TextureResource::CreateTextureResource(const wstring& FileName, const wstring& TexType) {
		try {
			return ObjectFactory::Create<TextureResource>(FileName, TexType);
		}
		catch (...) {
			throw;
		}
	}

	TextureResource::TextureResource(const wstring& FileName, const wstring& TexType) :
		BaseResource(),
		pImpl(new Impl(FileName, TexType))
	{
	}
	TextureResource::~TextureResource() {
	}

	//アクセッサ
	ComPtr<ID3D11ShaderResourceView>& TextureResource::GetShaderResourceView() const {
		return pImpl->m_ShaderResView;
	}
	const wstring& TextureResource::GetTextureFileName() const {
		return pImpl->m_FileName;
	}



	//--------------------------------------------------------------------------------------
	//	class MeshResource : public MeshResource;
	/*!
	@brief Dx11プリミティブメッシュクラス.<br />
	プリミティブメッシュは、スタティック関数を使って生成する
	*/
	//--------------------------------------------------------------------------------------
	//構築
	MeshResource::MeshResource() :
		BaseResource(),
		m_IsSkining(false),
		m_BoneCount(0),
		m_SampleCount(0),
		m_MeshTypeIndex(typeid(VertexPosition)),	//便宜上VertexPositionに初期化
		m_NumStride(sizeof(VertexPosition)),
		m_PrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{}
	//破棄
	MeshResource::~MeshResource() {}

	//リソース構築

	shared_ptr<MeshResource> MeshResource::CreateSquare(float Size, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Squareの作成(ヘルパー関数を利用)
			MeshUtill::CreateSquare(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}
	shared_ptr<MeshResource> MeshResource::CreateCube(float Size, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Cubeの作成(ヘルパー関数を利用)
			MeshUtill::CreateCube(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}
	shared_ptr<MeshResource> MeshResource::CreateSphere(float Diameter, size_t Tessellation, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Sphereの作成(ヘルパー関数を利用)
			MeshUtill::CreateSphere(Diameter, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateCapsule(float Diameter, float Height, size_t Tessellation, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			bsm::Vec3 PointA(0, -Height / 2.0f, 0);
			bsm::Vec3 PointB(0, Height / 2.0f, 0);
			//Capsuleの作成(ヘルパー関数を利用)
			MeshUtill::CreateCapsule(Diameter, PointA, PointB, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}


	}



	shared_ptr<MeshResource> MeshResource::CreateCylinder(float Height, float Diameter, size_t Tessellation, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Cylinderの作成(ヘルパー関数を利用)
			MeshUtill::CreateCylinder(Height, Diameter, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateCone(float Diameter, float Height, size_t Tessellation, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Coneの作成(ヘルパー関数を利用)
			MeshUtill::CreateCone(Diameter, Height, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateTorus(float Diameter, float Thickness, size_t Tessellation, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Torusの作成(ヘルパー関数を利用)
			MeshUtill::CreateTorus(Diameter, Thickness, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}

	}

	shared_ptr<MeshResource> MeshResource::CreateTetrahedron(float Size, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Tetrahedronの作成(ヘルパー関数を利用)
			MeshUtill::CreateTetrahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	//正8面体
	shared_ptr<MeshResource> MeshResource::CreateOctahedron(float Size, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Octahedronの作成(ヘルパー関数を利用)
			MeshUtill::CreateOctahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateDodecahedron(float Size, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Dodecahedronの作成(ヘルパー関数を利用)
			MeshUtill::CreateDodecahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateIcosahedron(float Size, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Icosahedronの作成(ヘルパー関数を利用)
			MeshUtill::CreateIcosahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		catch (...) {
			throw;
		}
	}

	void MeshResource::ReadBaseData(const wstring& BinDataDir, const wstring& BinDataFile,
		vector<VertexPositionNormalTexture>& vertices, vector<VertexPositionNormalTangentTexture>& vertices_withtan,
		vector<uint16_t>& indices, vector<MaterialEx>& materials) {
		vertices.clear();
		vertices_withtan.clear();
		indices.clear();
		materials.clear();
		wstring DataFile = BinDataDir + BinDataFile;
		BinaryReader Reader(DataFile);
		//ヘッダの読み込み
		auto pHeader = Reader.ReadArray<char>(16);
		string str = pHeader;
		if (str != "BDV1.0") {
			throw BaseException(
				L"データ形式が違います",
				DataFile,
				L"MeshResource::ReadBaseData()"
			);
		}
		//頂点の読み込み
		auto blockHeader = Reader.Read<BlockHeader>();
		if (!(blockHeader.m_Type == BlockType::Vertex || blockHeader.m_Type == BlockType::VertexWithTangent)) {
			throw BaseException(
				L"頂点のヘッダが違います",
				DataFile,
				L"MeshResource::ReadBaseData()"
			);
		}
		if (blockHeader.m_Type == BlockType::Vertex) {
			auto VerTexSize = blockHeader.m_Size / sizeof(VertexPositionNormalTexturePOD);
			auto pVertex = Reader.ReadArray<VertexPositionNormalTexturePOD>((size_t)VerTexSize);
			for (UINT i = 0; i < VerTexSize; i++) {
				VertexPositionNormalTexture v;
				v.position.x = pVertex[i].position[0];
				v.position.y = pVertex[i].position[1];
				v.position.z = pVertex[i].position[2];
				v.normal.x = pVertex[i].normal[0];
				v.normal.y = pVertex[i].normal[1];
				v.normal.z = pVertex[i].normal[2];
				v.textureCoordinate.x = pVertex[i].textureCoordinate[0];
				v.textureCoordinate.y = pVertex[i].textureCoordinate[1];
				vertices.push_back(v);
			}
		}
		else if (blockHeader.m_Type == BlockType::VertexWithTangent) {
			auto VerTexSize = blockHeader.m_Size / sizeof(VertexPositionNormalTangentTexturePOD);
			auto pVertex = Reader.ReadArray<VertexPositionNormalTangentTexturePOD>((size_t)VerTexSize);
			for (UINT i = 0; i < VerTexSize; i++) {
				VertexPositionNormalTangentTexture v;
				v.position.x = pVertex[i].position[0];
				v.position.y = pVertex[i].position[1];
				v.position.z = pVertex[i].position[2];
				v.normal.x = pVertex[i].normal[0];
				v.normal.y = pVertex[i].normal[1];
				v.normal.z = pVertex[i].normal[2];
				v.tangent.x = pVertex[i].tangent[0];
				v.tangent.y = pVertex[i].tangent[1];
				v.tangent.z = pVertex[i].tangent[2];
				v.tangent.w = pVertex[i].tangent[3];
				v.textureCoordinate.x = pVertex[i].textureCoordinate[0];
				v.textureCoordinate.y = pVertex[i].textureCoordinate[1];
				vertices_withtan.push_back(v);
			}
		}
		else {
			throw BaseException(
				L"頂点の型が違います",
				DataFile,
				L"MeshResource::ReadBaseData()"
			);
		}

		//インデックスの読み込み
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::Index) {
			throw BaseException(
				L"インデックスのヘッダが違います",
				DataFile,
				L"MeshResource::ReadBaseData()"
			);
		}

		auto IndexSize = blockHeader.m_Size / sizeof(uint16_t);
		auto pIndex = Reader.ReadArray<uint16_t>((size_t)IndexSize);
		for (UINT i = 0; i < IndexSize; i++) {
			indices.push_back(pIndex[i]);
		}

		//マテリアルの読み込み
		//マテリアル数の読み込み
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::MaterialCount) {
			throw BaseException(
				L"マテリアル数のヘッダが違います",
				DataFile,
				L"MeshResource::ReadBaseData()"
			);
		}
		UINT MaterialCount = blockHeader.m_Size;
		for (UINT i = 0; i < MaterialCount; i++) {
			//テクスチャファイル名が可変長なので注意。
			blockHeader = Reader.Read<BlockHeader>();
			if (blockHeader.m_Type != BlockType::Material) {
				throw BaseException(
					L"マテリアルのヘッダが違います",
					DataFile,
					L"MeshResource::ReadBaseData()"
				);
			}
			UINT TextureFileNameSize = blockHeader.m_Size - sizeof(MaterialExPOD);
			auto rMaterial = Reader.Read<MaterialExPOD>();
			MaterialEx ToM;
			//!開始インデックス
			ToM.m_StartIndex = rMaterial.m_StartIndex;
			//!描画インデックスカウント
			ToM.m_IndexCount = rMaterial.m_IndexCount;
			//! デフィーズ（物体の色）
			ToM.m_Diffuse.x = rMaterial.m_Diffuse[0];
			ToM.m_Diffuse.y = rMaterial.m_Diffuse[1];
			ToM.m_Diffuse.z = rMaterial.m_Diffuse[2];
			ToM.m_Diffuse.w = rMaterial.m_Diffuse[3];
			//! スペキュラー（反射光）
			ToM.m_Specular.x = rMaterial.m_Specular[0];
			ToM.m_Specular.y = rMaterial.m_Specular[1];
			ToM.m_Specular.z = rMaterial.m_Specular[2];
			ToM.m_Specular.w = rMaterial.m_Specular[3];
			//! アンビエント（環境色）
			ToM.m_Ambient.x = rMaterial.m_Ambient[0];
			ToM.m_Ambient.y = rMaterial.m_Ambient[1];
			ToM.m_Ambient.z = rMaterial.m_Ambient[2];
			ToM.m_Ambient.w = rMaterial.m_Ambient[3];
			//! エミッシブ（放射光）
			ToM.m_Emissive.x = rMaterial.m_Emissive[0];
			ToM.m_Emissive.y = rMaterial.m_Emissive[1];
			ToM.m_Emissive.z = rMaterial.m_Emissive[2];
			ToM.m_Emissive.w = rMaterial.m_Emissive[3];
			auto pTexture = Reader.ReadArray<wchar_t>(TextureFileNameSize / sizeof(wchar_t));
			wstring TextureFileStr = pTexture;
			if (TextureFileStr != L"") {
				TextureFileStr = BinDataDir + TextureFileStr;
				ToM.m_TextureResource = ObjectFactory::Create<TextureResource>(TextureFileStr);

			}
			else {
				ToM.m_TextureResource = nullptr;
			}
			materials.push_back(ToM);
		}

		//Endの読み込み
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::End) {
			throw BaseException(
				L"Endヘッダが違います",
				DataFile,
				L"MeshResource::ReadBaseData()"
			);
		}
	}



	void MeshResource::ReadBaseBoneData(const wstring& BinDataDir, const wstring& BinDataFile,
		vector<VertexPositionNormalTextureSkinning>& vertices, vector<VertexPositionNormalTangentTextureSkinning>& vertices_withtan,
		vector<uint16_t>& indices, vector<MaterialEx>& materials,
		vector<bsm::Mat4x4>& bonematrix, UINT& BoneCount, UINT& SampleCount) {
		vertices.clear();
		vertices_withtan.clear();
		indices.clear();
		materials.clear();
		bonematrix.clear();

		wstring DataFile = BinDataDir + BinDataFile;
		BinaryReader Reader(DataFile);
		//ヘッダの読み込み
		auto pHeader = Reader.ReadArray<char>(16);
		string str = pHeader;
		if (str != "BDV1.0") {
			throw BaseException(
				L"データ形式が違います",
				DataFile,
				L"MeshResource::ReadBaseBoneData()"
			);
		}
		//頂点の読み込み
		auto blockHeader = Reader.Read<BlockHeader>();
		if (!(blockHeader.m_Type == BlockType::SkinedVertex || blockHeader.m_Type == BlockType::SkinedVertexWithTangent)) {
			throw BaseException(
				L"頂点(スキン処理)のヘッダが違います",
				DataFile,
				L"MeshResource::ReadBaseBoneData()"
			);
		}
		if (blockHeader.m_Type == BlockType::SkinedVertex) {
			auto VerTexSize = blockHeader.m_Size / sizeof(VertexPositionNormalTextureSkinningPOD);
			auto pVertex = Reader.ReadArray<VertexPositionNormalTextureSkinningPOD>((size_t)VerTexSize);
			for (UINT i = 0; i < VerTexSize; i++) {
				VertexPositionNormalTextureSkinning v;
				v.position.x = pVertex[i].position[0];
				v.position.y = pVertex[i].position[1];
				v.position.z = pVertex[i].position[2];
				v.normal.x = pVertex[i].normal[0];
				v.normal.y = pVertex[i].normal[1];
				v.normal.z = pVertex[i].normal[2];
				v.textureCoordinate.x = pVertex[i].textureCoordinate[0];
				v.textureCoordinate.y = pVertex[i].textureCoordinate[1];
				for (int j = 0; j < 4; j++) {
					v.indices[j] = pVertex[i].indices[j];
					v.weights[j] = pVertex[i].weights[j];
				}
				vertices.push_back(v);
			}
		}
		else if (blockHeader.m_Type == BlockType::SkinedVertexWithTangent) {
			auto VerTexSize = blockHeader.m_Size / sizeof(VertexPositionNormalTangentTextureSkinningPOD);
			auto pVertex = Reader.ReadArray<VertexPositionNormalTangentTextureSkinningPOD>((size_t)VerTexSize);
			for (UINT i = 0; i < VerTexSize; i++) {
				VertexPositionNormalTangentTextureSkinning v;
				v.position.x = pVertex[i].position[0];
				v.position.y = pVertex[i].position[1];
				v.position.z = pVertex[i].position[2];
				v.normal.x = pVertex[i].normal[0];
				v.normal.y = pVertex[i].normal[1];
				v.normal.z = pVertex[i].normal[2];
				v.tangent.x = pVertex[i].tangent[0];
				v.tangent.y = pVertex[i].tangent[1];
				v.tangent.z = pVertex[i].tangent[2];
				v.tangent.w = pVertex[i].tangent[3];
				v.textureCoordinate.x = pVertex[i].textureCoordinate[0];
				v.textureCoordinate.y = pVertex[i].textureCoordinate[1];
				for (int j = 0; j < 4; j++) {
					v.indices[j] = pVertex[i].indices[j];
					v.weights[j] = pVertex[i].weights[j];
				}
				vertices_withtan.push_back(v);
			}

		}



		//インデックスの読み込み
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::Index) {
			throw BaseException(
				L"インデックスのヘッダが違います",
				DataFile,
				L"MeshResource::ReadBaseBoneData()"
			);
		}

		auto IndexSize = blockHeader.m_Size / sizeof(uint16_t);
		auto pIndex = Reader.ReadArray<uint16_t>((size_t)IndexSize);
		for (UINT i = 0; i < IndexSize; i++) {
			indices.push_back(pIndex[i]);
		}

		//マテリアルの読み込み
		//マテリアル数の読み込み
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::MaterialCount) {
			throw BaseException(
				L"マテリアル数のヘッダが違います",
				DataFile,
				L"MeshResource::ReadBaseData()"
			);
		}
		UINT MaterialCount = blockHeader.m_Size;
		for (UINT i = 0; i < MaterialCount; i++) {
			//テクスチャファイル名が可変長なので注意。
			blockHeader = Reader.Read<BlockHeader>();
			if (blockHeader.m_Type != BlockType::Material) {
				throw BaseException(
					L"マテリアルのヘッダが違います",
					DataFile,
					L"MeshResource::ReadBaseBoneData()"
				);
			}
			UINT TextureFileNameSize = blockHeader.m_Size - sizeof(MaterialExPOD);
			auto rMaterial = Reader.Read<MaterialExPOD>();
			MaterialEx ToM;
			//!開始インデックス
			ToM.m_StartIndex = rMaterial.m_StartIndex;
			//!描画インデックスカウント
			ToM.m_IndexCount = rMaterial.m_IndexCount;
			//! デフィーズ（物体の色）
			ToM.m_Diffuse.x = rMaterial.m_Diffuse[0];
			ToM.m_Diffuse.y = rMaterial.m_Diffuse[1];
			ToM.m_Diffuse.z = rMaterial.m_Diffuse[2];
			ToM.m_Diffuse.w = rMaterial.m_Diffuse[3];
			//! スペキュラー（反射光）
			ToM.m_Specular.x = rMaterial.m_Specular[0];
			ToM.m_Specular.y = rMaterial.m_Specular[1];
			ToM.m_Specular.z = rMaterial.m_Specular[2];
			ToM.m_Specular.w = rMaterial.m_Specular[3];
			//! アンビエント（環境色）
			ToM.m_Ambient.x = rMaterial.m_Ambient[0];
			ToM.m_Ambient.y = rMaterial.m_Ambient[1];
			ToM.m_Ambient.z = rMaterial.m_Ambient[2];
			ToM.m_Ambient.w = rMaterial.m_Ambient[3];
			//! エミッシブ（放射光）
			ToM.m_Emissive.x = rMaterial.m_Emissive[0];
			ToM.m_Emissive.y = rMaterial.m_Emissive[1];
			ToM.m_Emissive.z = rMaterial.m_Emissive[2];
			ToM.m_Emissive.w = rMaterial.m_Emissive[3];
			auto pTexture = Reader.ReadArray<wchar_t>(TextureFileNameSize / sizeof(wchar_t));
			wstring TextureFileStr = pTexture;
			if (TextureFileStr != L"") {
				TextureFileStr = BinDataDir + TextureFileStr;
				ToM.m_TextureResource = ObjectFactory::Create<TextureResource>(TextureFileStr);
			}
			else {
				ToM.m_TextureResource = nullptr;
			}
			materials.push_back(ToM);
		}

		//ボーン数
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::BoneCount) {
			throw BaseException(
				L"ボーン数のヘッダが違います",
				DataFile,
				L"MeshResource::ReadBaseBoneData()"
			);
		}
		BoneCount = blockHeader.m_Size;
		//ボーンアニメーション行列
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::AnimeMatrix) {
			throw BaseException(
				L"アニメーション行列のヘッダが違います",
				DataFile,
				L"MeshResource::ReadBaseBoneData()"
			);
		}
		auto MatrixSize = blockHeader.m_Size / sizeof(MatrixPOD);
		auto pAnimeMatrix = Reader.ReadArray<MatrixPOD>((size_t)MatrixSize);
		for (UINT i = 0; i < MatrixSize; i++) {
			//ボーン単位ではなく行列単位で読み込む
			bsm::Mat4x4 mat;
			for (int u = 0; u < 4; u++) {
				for (int v = 0; v < 4; v++) {
					mat(u, v) = pAnimeMatrix->m_Mat[u][v];
				}
			}
			bonematrix.push_back(mat);
			pAnimeMatrix++;
		}
		SampleCount = MatrixSize / BoneCount;

		//End
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::End) {
			throw BaseException(
				L"終了ヘッダが違います",
				DataFile,
				L"MeshResource::ReadBaseBoneData()"
			);
		}


	}



	shared_ptr<MeshResource> MeshResource::CreateStaticModelMesh(const wstring& BinDataDir, const wstring& BinDataFile, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//タンジェント付きにコンバートする配列(ダミー)
			vector<VertexPositionNormalTangentTexture> new_pntnt_vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//マテリアルを設定する配列
			vector<MaterialEx> Materials;
			ReadBaseData(BinDataDir, BinDataFile, vertices, new_pntnt_vertices, indices, Materials);
			auto Ptr = MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
			Ptr->m_MaterialExVec.clear();
			for (auto& v : Materials) {
				Ptr->m_MaterialExVec.push_back(v);
			}
			return Ptr;
		}
		catch (...) {
			throw;
		}
	}

	shared_ptr<MeshResource> MeshResource::CreateStaticModelMeshWithTangent(const wstring& BinDataDir, const wstring& BinDataFile, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTexture> vertices;
			//タンジェント付きにコンバートする配列
			vector<VertexPositionNormalTangentTexture> new_pntnt_vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//マテリアルを設定する配列
			vector<MaterialEx> Materials;
			ReadBaseData(BinDataDir, BinDataFile, vertices, new_pntnt_vertices,indices, Materials);
			if (vertices.size() > 0) {
				//binデータにはタンジェントは入ってなかった
				new_pntnt_vertices.clear();
				for (size_t i = 0; i < vertices.size(); i++) {
					VertexPositionNormalTangentTexture new_pntnt_v;

					new_pntnt_v.position = vertices[i].position;
					new_pntnt_v.normal = vertices[i].normal;
					new_pntnt_v.textureCoordinate = vertices[i].textureCoordinate;
					bsm::Vec3 n = bsm::cross((bsm::Vec3)new_pntnt_v.normal, bsm::Vec3(0, 1, 0));
					new_pntnt_v.tangent = XMFLOAT4(0, 0, 0, 0);
					new_pntnt_vertices.push_back(new_pntnt_v);
				}
				MeshUtill::SetNormalTangent(new_pntnt_vertices);
			}
			auto Ptr = MeshResource::CreateMeshResource<VertexPositionNormalTangentTexture>(new_pntnt_vertices, indices, AccessWrite);
			Ptr->m_MaterialExVec.clear();
			for (auto& v : Materials) {
				Ptr->m_MaterialExVec.push_back(v);
			}
			return Ptr;
		}
		catch (...) {
			throw;
		}
	}



	shared_ptr<MeshResource> MeshResource::CreateBoneModelMesh(const wstring& BinDataDir,
		const wstring& BinDataFile, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTextureSkinning> vertices;
			//タンジェント付きにコンバートする配列(ダミー)
			vector<VertexPositionNormalTangentTextureSkinning> new_pntnt_vertices;
			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//マテリアルを設定する配列
			vector<MaterialEx> Materials;
			//サンプリング行列
			vector<bsm::Mat4x4> SampleMatrices;
			//ボーン数
			UINT BoneCount;
			//サンプル数
			UINT SampleCount;
			ReadBaseBoneData(BinDataDir, BinDataFile, vertices, new_pntnt_vertices,
				indices, Materials,
				SampleMatrices, BoneCount, SampleCount);
			auto Ptr = MeshResource::CreateMeshResource<VertexPositionNormalTextureSkinning>(vertices, indices, AccessWrite);
			Ptr->m_MaterialExVec.clear();
			for (auto& v : Materials) {
				Ptr->m_MaterialExVec.push_back(v);
			}
			Ptr->m_SampleMatrixVec.clear();
			for (auto& v : SampleMatrices) {
				Ptr->m_SampleMatrixVec.push_back(v);
			}
			Ptr->m_BoneCount = BoneCount;
			Ptr->m_SampleCount = SampleCount;
			Ptr->m_IsSkining = true;
			return Ptr;
		}
		catch (...) {
			throw;
		}

	}

	shared_ptr<MeshResource> MeshResource::CreateBoneModelMeshWithTangent(const wstring& BinDataDir,
		const wstring& BinDataFile, bool AccessWrite) {
		try {
			//頂点配列
			vector<VertexPositionNormalTextureSkinning> vertices;
			//タンジェント付きにコンバートする配列
			vector<VertexPositionNormalTangentTextureSkinning> new_pntnt_vertices;

			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//マテリアルを設定する配列
			vector<MaterialEx> Materials;
			//サンプリング行列
			vector<bsm::Mat4x4> SampleMatrices;
			//ボーン数
			UINT BoneCount;
			//サンプル数
			UINT SampleCount;
			ReadBaseBoneData(BinDataDir, BinDataFile, vertices, new_pntnt_vertices,
				indices, Materials,
				SampleMatrices, BoneCount, SampleCount);
			if (vertices.size() > 0) {
				//計算で法線を入れる
				new_pntnt_vertices.clear();
				for (size_t i = 0; i < vertices.size(); i++) {
					VertexPositionNormalTangentTextureSkinning new_pntnt_v;
					new_pntnt_v.position = vertices[i].position;
					new_pntnt_v.normal = vertices[i].normal;
					new_pntnt_v.textureCoordinate = vertices[i].textureCoordinate;
					new_pntnt_v.tangent = XMFLOAT4(0, 0, 0, 0);
					for (size_t j = 0; j < 4; j++) {
						new_pntnt_v.indices[j] = vertices[i].indices[j];
						new_pntnt_v.weights[j] = vertices[i].weights[j];
					}
					new_pntnt_vertices.push_back(new_pntnt_v);
				}
				MeshUtill::SetNormalTangent(new_pntnt_vertices);
			}

			auto Ptr = MeshResource::CreateMeshResource<VertexPositionNormalTangentTextureSkinning>(new_pntnt_vertices, indices, AccessWrite);
			Ptr->m_MaterialExVec.clear();
			for (auto& v : Materials) {
				Ptr->m_MaterialExVec.push_back(v);
			}
			Ptr->m_SampleMatrixVec.clear();
			for (auto& v : SampleMatrices) {
				Ptr->m_SampleMatrixVec.push_back(v);
			}
			Ptr->m_BoneCount = BoneCount;
			Ptr->m_SampleCount = SampleCount;
			Ptr->m_IsSkining = true;
			return Ptr;
		}
		catch (...) {
			throw;
		}

	}



	//--------------------------------------------------------------------------------------
	//	struct MultiAudioObject::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct MultiAudioObject::Impl {
		map<wstring, SoundItem> m_SoundMap;
		Impl()
		{}
		~Impl() {
		}
		SoundItem* GetItem(const wstring& ResKey) {
			auto it = m_SoundMap.find(ResKey);
			if (it != m_SoundMap.end()) {
				//同じ名前が見つかった
				return &it->second;
			}
			return nullptr;
		}
		void RemoveItem(const wstring& ResKey) {
			auto it = m_SoundMap.find(ResKey);
			if (it != m_SoundMap.end()) {
				//同じ名前が見つかった
				//Mapから削除
				m_SoundMap.erase(it);
			}
		}
		void Start(const SoundItem* pItem, const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Stop(const SoundItem* pItem);
	};

	void MultiAudioObject::Impl::Start(const SoundItem* pItem, const XAUDIO2_BUFFER& Buffer, float Volume) {
		if (!pItem->m_SourceVoice) {
			throw BaseException(
				L"サウンドボイスが不定です",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiAudioObject::Impl::Start()"
			);
		}
		ThrowIfFailed(
			pItem->m_SourceVoice->Stop(),
			L"サウンドエフェクト用サウンドボイスの停止に失敗しました",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiAudioObject::Impl::Start()"
		);
		ThrowIfFailed(
			pItem->m_SourceVoice->FlushSourceBuffers(),
			L"サウンドエフェクト用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiAudioObject::Impl::Start()"
		);
		if (pItem->m_AudioResource.expired()) {
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				L"if (pImpl->m_AudioResource.expired())",
				L"MultiAudioObject::Impl::Start()"
			);
		}
		ThrowIfFailed(
			pItem->m_SourceVoice->SetVolume(Volume),
			L"サウンドエフェクト用サウンドのボリューム設定に失敗しました",
			L"pItem->m_pSourceVoice->SetVolume()",
			L"MultiAudioObject::Impl::Start()"
		);

		ThrowIfFailed(
			pItem->m_SourceVoice->SubmitSourceBuffer(&Buffer),
			L"サウンドエフェクト用サウンドのソースバッファ設定に失敗しました",
			L"pItem->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"MultiAudioObject::Impl::Start()"
		);

		ThrowIfFailed(
			pItem->m_SourceVoice->Start(),
			L"サウンドエフェクト用サウンドのスタートに失敗しました",
			L"pItem->m_pSourceVoice->Start()",
			L"MultiAudioObject::Impl::Start()"
		);
	}

	void MultiAudioObject::Impl::Stop(const SoundItem* pItem) {
		if (!pItem->m_SourceVoice) {
			throw BaseException(
				L"サウンドボイスが不定です",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiAudioObject::Impl::Stop()"
			);
		}
		ThrowIfFailed(
			pItem->m_SourceVoice->Stop(),
			L"サウンドエフェクト用サウンドボイスの停止に失敗しました",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiAudioObject::Impl::Stop()"
		);
		ThrowIfFailed(
			pItem->m_SourceVoice->FlushSourceBuffers(),
			L"サウンドエフェクト用サウンドボイスのバッファのフラッシュに失敗しました",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiAudioObject::Impl::Stop()"
		);
	}


	//--------------------------------------------------------------------------------------
	//	class MultiAudioObject: public Object;
	//	用途: マルチオーディオ
	// ＊コンポーネントではないのでシーンに持たせることができる
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MultiAudioObject::MultiAudioObject() :
		pImpl(new Impl())
	{}
	MultiAudioObject::~MultiAudioObject() {}

	//アクセサ
	shared_ptr<AudioResource> MultiAudioObject::AddAudioResource(const wstring& ResKey) {
		try {
			if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
				return nullptr;
			}

			if (ResKey == L"") {
				throw BaseException(
					L"オーディオリソース名が空白です",
					L"if (ResKey == L\"\")",
					L"MultiAudioObject::AddAudioResource()"
				);
			}
			auto pItem = pImpl->GetItem(ResKey);
			if (pItem) {
				//同じ名前が見つかった
				if (!pItem->m_AudioResource.expired()) {
					return pItem->m_AudioResource.lock();
				}
				else {
					throw BaseException(
						L"同じ名前のオーディオリソースがありましたが、リソースが無効です",
						ResKey,
						L"MultiAudioObject::AddAudioResource()"
					);
				}
			}
			auto SoundRes = App::GetApp()->GetResource<AudioResource>(ResKey);
			SoundItem Item;
			Item.m_AudioResource = SoundRes;
			auto Engine = App::GetApp()->GetAudioManager()->GetSoundEffectEngine();

			ThrowIfFailed(
				Engine->CreateSourceVoice(&Item.m_SourceVoice, SoundRes->GetOutputWaveFormatEx()),
				L"サウンドエフェクト用サウンドボイスの作成に失敗しました",
				L"Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, SountRes->GetOutputWaveFormatEx())",
				L"MultiAudioObject::AddAudioResource()"
			);
			pImpl->m_SoundMap[ResKey] = Item;
			return SoundRes;
		}
		catch (...) {
			throw;
		}
	}
	void MultiAudioObject::RemoveAudioResource(const wstring& ResKey) {
		if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
			return;
		}
		if (ResKey == L"") {
			throw BaseException(
				L"オーディオリソース名が空白です",
				L"if (ResKey == L\"\")",
				L"MultiAudioObject::RemoveAudioResource()"
			);
		}
		pImpl->RemoveItem(ResKey);
	}


	shared_ptr<AudioResource> MultiAudioObject::GetAudioResource(const wstring& ResKey, bool ExceptionActive) const {
		if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
			return nullptr;
		}
		if (ResKey == L"") {
			if (ExceptionActive) {
				throw BaseException(
					L"オーディオリソース名が空白です",
					L"if (ResKey == L\"\")",
					L"MultiAudioObject::GetAudioResource()"
				);
			}
			else {
				return nullptr;
			}
		}
		auto pItem = pImpl->GetItem(ResKey);
		if (pItem) {
			//同じ名前が見つかった
			if (!pItem->m_AudioResource.expired()) {
				return pItem->m_AudioResource.lock();
			}
			else {
				if (ExceptionActive) {
					throw BaseException(
						L"同じ名前のオーディオリソースがありましたが、リソースが無効です",
						ResKey,
						L"MultiAudioObject::AddAudioResource()"
					);
				}
				else {
					return nullptr;
				}
			}
		}
		else {
			if (ExceptionActive) {
				throw BaseException(
					L"指定の名前のオーディオリソースがありません",
					ResKey,
					L"MultiAudioObject::GetAudioResource()"
				);
			}
			else {
				return nullptr;
			}
		}
	}

	void MultiAudioObject::Start(const wstring& ResKey, const XAUDIO2_BUFFER& Buffer, float Volume) {
		if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
			return;
		}
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem) {
			throw BaseException(
				L"指定の名前のオーディオリソースがありません",
				ResKey,
				L"MultiAudioObject::Start()"
			);
		}
		pImpl->Start(pItem, Buffer, Volume);
	}

	void MultiAudioObject::Start(const wstring& ResKey, size_t LoopCount, float Volume) {
		if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
			return;
		}
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem) {
			throw BaseException(
				L"指定の名前のオーディオリソースがありません",
				ResKey,
				L"MultiAudioObject::Start()"
			);
		}
		if (pItem->m_AudioResource.expired()) {
			throw BaseException(
				L"オーディオリソースが有効ではありません",
				ResKey,
				L"MultiAudioObject::Start()"
			);
		}
		XAUDIO2_BUFFER buffer = { 0 };
		auto ResPtr = pItem->m_AudioResource.lock();
		buffer.AudioBytes = ResPtr->GetSoundData().size();
		buffer.LoopCount = LoopCount;
		buffer.pAudioData = &ResPtr->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		//pImplの関数を呼ぶ
		pImpl->Start(pItem, buffer, Volume);
	}
	void MultiAudioObject::Stop(const wstring& ResKey) {
		if (!App::GetApp()->GetAudioManager()->IsAudioAvailable()) {
			return;
		}
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem) {
			throw BaseException(
				L"指定の名前のオーディオリソースがありません",
				ResKey,
				L"MultiAudioObject::Stop()"
			);
		}
		pImpl->Stop(pItem);
	}






	//--------------------------------------------------------------------------------------
	//	struct DeviceResources::Impl;
	//	用途: Direct11デバイスイディオム
	//--------------------------------------------------------------------------------------
	struct DeviceResources::Impl {
		D3D_DRIVER_TYPE	m_D3DDriverType;			//ドライバのタイプ
		D3D_FEATURE_LEVEL m_D3DFeatureLevel;
		// Direct3D オブジェクト。
		ComPtr<ID3D11Device2> m_D3D11Device;		//デバイス
		ComPtr<ID3D11DeviceContext2> m_D3D11Context;		//コンテキスト
		ComPtr<IDXGISwapChain1> m_D3D11SwapChain;	//スワップチェーン


													// Direct2D 描画コンポーネント。
		ComPtr<ID2D1Factory2>		m_d2dFactory;
		ComPtr<ID2D1Device1>		m_d2dDevice;
		ComPtr<ID2D1DeviceContext1>	m_d2dContext;

		// DirectWrite 描画コンポーネント。
		ComPtr<IDWriteFactory2>		m_dwriteFactory;
		ComPtr<IWICImagingFactory2>	m_wicFactory;

		float m_dpi;

		shared_ptr<DefaultRenderTarget> m_DefaultRenderTarget;	///<デフォルトのレンダリングターゲット
		shared_ptr<ShadowMapRenderTarget> m_ShadowMapRenderTarget;	///<シャドウマップのレンダリングターゲット
		shared_ptr<RenderState> m_RenderState;					///<レンダリングステート



		void CreateSamplerState(D3D11_FILTER filter,
			D3D11_TEXTURE_ADDRESS_MODE addressMode,
			ID3D11SamplerState** pResult) {
			try {

				auto Dev = App::GetApp()->GetDeviceResources();
				auto pDx11Device = Dev->GetD3DDevice();

				D3D11_SAMPLER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));

				desc.Filter = filter;

				desc.AddressU = addressMode;
				desc.AddressV = addressMode;
				desc.AddressW = addressMode;

				desc.MaxAnisotropy = (pDx11Device->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? 16 : 2;

				desc.MaxLOD = FLT_MAX;
				desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

				HRESULT hr = pDx11Device->CreateSamplerState(&desc, pResult);

				if (FAILED(hr)) {
					// 初期化失敗
					throw BaseException(
						L"サンプラーステート作成に失敗しました。",
						L"if(FAILED(pDx11Device->CreateSamplerState()))",
						L"DeviceResources::Impl::CreateSamplerState()"
					);
				}
			}
			catch (...) {
				throw;
			}

		}
		void CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend,
			ID3D11BlendState** pResult) {
			try {
				auto Dev = App::GetApp()->GetDeviceResources();
				auto pDx11Device = Dev->GetD3DDevice();

				D3D11_BLEND_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) ||
					(destBlend != D3D11_BLEND_ZERO);

				desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
				desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
				desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

				HRESULT hr = pDx11Device->CreateBlendState(&desc, pResult);
				if (FAILED(hr)) {
					// 初期化失敗
					throw BaseException(
						L"ブレンドステート作成に失敗しました。",
						L"if(FAILED(pDx11Device->CreateBlendState()))",
						L"DeviceResources::Impl::CreateBlendState()"
					);
				}
			}
			catch (...) {
				throw;
			}
		}
		void CreateDepthStencilState(bool enable, bool writeEnable,
			ID3D11DepthStencilState** pResult) {
			try {
				auto Dev = App::GetApp()->GetDeviceResources();
				auto pDx11Device = Dev->GetD3DDevice();

				D3D11_DEPTH_STENCIL_DESC desc;
				ZeroMemory(&desc, sizeof(desc));

				desc.DepthEnable = enable;
				desc.DepthWriteMask = writeEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
				desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

				desc.StencilEnable = false;
				desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
				desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

				desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
				desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
				desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
				desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

				desc.BackFace = desc.FrontFace;

				HRESULT hr = pDx11Device->CreateDepthStencilState(&desc, pResult);
				if (FAILED(hr)) {
					// 初期化失敗
					throw BaseException(
						L"デプスステンシルステート作成に失敗しました。",
						L"if(FAILED(pDx11Device->CreateDepthStencilState()))",
						L"DeviceResources::Impl::CreateDepthStencilState()"
					);
				}
			}
			catch (...) {
				throw;
			}
		}
		void CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
			ID3D11RasterizerState** pResult, bool Scissor = false) {
			try {
				auto Dev = App::GetApp()->GetDeviceResources();
				auto pDx11Device = Dev->GetD3DDevice();

				D3D11_RASTERIZER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));

				desc.CullMode = cullMode;
				desc.FillMode = fillMode;
				desc.DepthClipEnable = true;
				desc.MultisampleEnable = true;
				desc.ScissorEnable = Scissor;

				HRESULT hr = pDx11Device->CreateRasterizerState(&desc, pResult);
				if (FAILED(hr)) {
					// 初期化失敗
					throw BaseException(
						L"ラスタライザステート作成に失敗しました。",
						L"if(FAILED(pDx11Device->CreateRasterizerState()))",
						L"DeviceResources::Impl::CreateRasterizerState()"
					);
				}
			}
			catch (...) {
				throw;
			}
		}

		ComPtr<ID3D11BlendState> m_OpaquePtr;
		ComPtr<ID3D11SamplerState> m_LinearClampPtr;
		ComPtr<ID3D11DepthStencilState> m_DepthDefaultPtr;
		ComPtr<ID3D11RasterizerState> m_CullBackPtr;
		//ミューテックス
		std::mutex Mutex;

		//構築と破棄
		Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		~Impl() {}
		//リソースの構築
		void CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);

		ID3D11SamplerState* GetLinearClampSampler() {
			return Util::DemandCreate(m_LinearClampPtr, Mutex, [&](ID3D11SamplerState** pResult)
			{
				CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, pResult);
			});
		}
		ID3D11BlendState* GetOpaqueBlend() {
			return Util::DemandCreate(m_OpaquePtr, Mutex, [&](ID3D11BlendState** pResult)
			{
				CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_ZERO, pResult);
			});

		}

		ID3D11RasterizerState* GetCullBackRasterizer() {
			return Util::DemandCreate(m_CullBackPtr, Mutex, [&](ID3D11RasterizerState** pResult)
			{
				CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, pResult);
			});
		}

		ID3D11DepthStencilState* GetDefaultDepthStencil() {
			return Util::DemandCreate(m_DepthDefaultPtr, Mutex, [&](ID3D11DepthStencilState** pResult)
			{
				CreateDepthStencilState(true, true, pResult);
			});
		}



	};
	//構築
	DeviceResources::Impl::Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) :
		m_dpi(96.0f)
	{
		try {
			//リソースの構築
			CreateDeviceResources(hWnd, isFullScreen, Width, Height);
		}
		catch (...) {
			throw;
		}
	}
	//リソースの構築
	void DeviceResources::Impl::CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) {

		// Direct2D リソースを初期化します。
		D2D1_FACTORY_OPTIONS options;
		ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
		// プロジェクトがデバッグ ビルドに含まれている場合は、Direct2D デバッグを SDK レイヤーを介して有効にします。
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif
		// Direct2D ファクトリを初期化します。
		ThrowIfFailed(
			D2D1CreateFactory(
				D2D1_FACTORY_TYPE_SINGLE_THREADED,
				__uuidof(ID2D1Factory2),
				&options,
				&m_d2dFactory
			),
			L"Factory作成に失敗しました。",
			L"D2D1CreateFactory()",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);


		// DirectWrite ファクトリを初期化します。
		ThrowIfFailed(
			DWriteCreateFactory(
				DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory2),
				&m_dwriteFactory
			),
			L"DirectWrite ファクトリ作成に失敗しました。",
			L"DWriteCreateFactory()",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);

		ThrowIfFailed(
			CoCreateInstance(
				CLSID_WICImagingFactory2,
				nullptr,
				CLSCTX_INPROC_SERVER,
				IID_PPV_ARGS(&m_wicFactory)
			),
			L"(WIC) ファクトリ作成に失敗しました。",
			L"CoCreateInstance()",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);


		HRESULT hr;	//APIの戻り値用
					//デバイス作成用フラグ
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		//デバッグ時
#ifdef _DEBUG
		//		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		//ドライバのタイプを配列化
		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		//選択できるドライバーの数
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		//機能セットの配列化
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,	//Dx11機能オンリー
		};
		//選択できる機能の数
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		//スワップチェーンのデスクプリタの設定
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;					//バッファ数(2ダブルバッファ)
		sd.BufferDesc.Width = Width;		//バッファの幅
		sd.BufferDesc.Height = Height;	//バッファの高さ
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//8 ビット アルファをサポートする 4 成分、16 ビット符号なし正規化整数フォーマット
		sd.BufferDesc.RefreshRate.Numerator = 60;	//リフレッシュレート最大値
		sd.BufferDesc.RefreshRate.Denominator = 1;	//リフレッシュレート最小値
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//出力用として定義
		sd.OutputWindow = hWnd;		//ウインドウのハンドル
		sd.SampleDesc.Count = 1;		//マルチサンプリング 数は1
		sd.SampleDesc.Quality = 0;		//マルチサンプリングクオリティは0（最大）

		sd.Windowed = TRUE;			//ウインドウモードで作成してあとからフルsクリーンにする

		ComPtr<ID3D11Device> temp_device;
		ComPtr<ID3D11DeviceContext>	temp_context;
		ComPtr<IDXGISwapChain>	temp_swapChain;


		//デバイスとスワップチェーンの作成
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
			//ドライバタイプを配列から取得
			m_D3DDriverType = driverTypes[driverTypeIndex];
			//デバイスとスワップチェーンの作成
			hr = D3D11CreateDeviceAndSwapChain(
				nullptr,
				m_D3DDriverType,
				nullptr,
				createDeviceFlags,
				featureLevels,
				numFeatureLevels,
				D3D11_SDK_VERSION,
				&sd,
				&temp_swapChain,
				&temp_device,
				&m_D3DFeatureLevel,
				&temp_context
			);
			//成功したらそのドライバを使う
			if (SUCCEEDED(hr))
				break;
		}
		ThrowIfFailed(
			hr,
			L"DX11デバイスとスワップチェーンの作成に失敗しました。",
			L"D3D11CreateDeviceAndSwapChain()",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);

		//リソースをバージョンアップする
		ThrowIfFailed(
			temp_device.As(&m_D3D11Device),
			L"DX11デバイスのバージョンアップに失敗しました。",
			L"temp_device.As(&m_D3D11Device)",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);
		ThrowIfFailed(
			temp_context.As(&m_D3D11Context),
			L"DX11コンテクストのバージョンアップに失敗しました。",
			L"temp_context.As(&m_D3D11Context)",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);
		ThrowIfFailed(
			temp_swapChain.As(&m_D3D11SwapChain),
			L"DX11スワップチェーンのバージョンアップに失敗しました。",
			L"temp_swapChain.As(&m_D3D11SwapChain)",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);

		// Direct2D デバイス オブジェクトと、対応するコンテキストを作成します。

		ComPtr<IDXGIDevice3> dxgiDevice;

		ThrowIfFailed(
			m_D3D11Device.As(&dxgiDevice),
			L"dxgiDeviceのバージョンアップに失敗しました。",
			L"m_d3dDevice.As(&dxgiDevice)",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);

		ThrowIfFailed(
			m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice),
			L"dxgiDeviceのバージョンアップに失敗しました。",
			L"m_d2dFactory->CreateDevice()",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);

		ThrowIfFailed(
			m_d2dDevice->CreateDeviceContext(
				D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
				&m_d2dContext
			),
			L"2dデバイスコンテキスト作成に失敗しました。",
			L"m_d2dDevice->CreateDeviceContext()",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);
		if (isFullScreen) {
			ThrowIfFailed(
				m_D3D11SwapChain->SetFullscreenState(true, NULL),
				L"フルスクリーン移行に失敗しました。",
				L"m_D3D11SwapChain->SetFullscreenState(true, NULL)",
				L"DeviceResources::Impl::CreateDeviceResources()"
			);

		}

	}

	//--------------------------------------------------------------------------------------
	//	class DeviceResources;
	//	用途: Direct11デバイスなどクラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	DeviceResources::DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height) :
		pImpl(new Impl(hWnd, isFullScreen, Width, Height))
	{
		//レンダリングステートの作成
		pImpl->m_RenderState = make_shared<RenderState>();
	}
	DeviceResources::~DeviceResources() {}
	//アクセサ
	ID3D11Device2*	DeviceResources::GetD3DDevice() const { return pImpl->m_D3D11Device.Get(); }
	ID3D11DeviceContext2* DeviceResources::GetD3DDeviceContext() const { return pImpl->m_D3D11Context.Get(); }
	IDXGISwapChain1* DeviceResources::GetSwapChain() const { return pImpl->m_D3D11SwapChain.Get(); }
	D3D_FEATURE_LEVEL DeviceResources::GetFeatureLevel() const { return pImpl->m_D3DFeatureLevel; }

	// D2D アクセサー。
	ID2D1Factory2*			DeviceResources::GetD2DFactory() const { return pImpl->m_d2dFactory.Get(); }
	ID2D1Device1*			DeviceResources::GetD2DDevice() const { return pImpl->m_d2dDevice.Get(); }
	ID2D1DeviceContext1*	DeviceResources::GetD2DDeviceContext() const { return pImpl->m_d2dContext.Get(); }
	IDWriteFactory2*		DeviceResources::GetDWriteFactory() const { return pImpl->m_dwriteFactory.Get(); }
	IWICImagingFactory2*	DeviceResources::GetWicImagingFactory() const { return pImpl->m_wicFactory.Get(); }

	void DeviceResources::InitializeStates() {
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		ID3D11Buffer* pNullConstantBuffer[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = { nullptr };

		auto pID3D11DeviceContext = GetD3DDeviceContext();
		//後始末
		//コンスタントバッファクリア
		pID3D11DeviceContext->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->GSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->CSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		//シェーダーリソースもクリア
		pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
		//サンプラーもクリア
		pID3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
		//シェーダのクリア
		pID3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->CSSetShader(nullptr, nullptr, 0);
		//インプットレイアウトのクリア
		pID3D11DeviceContext->IASetInputLayout(nullptr);
		//デフォルトの設定
		//リニアサンプラーを設定しておく
		ID3D11SamplerState* samplerState = pImpl->GetLinearClampSampler();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		//ブレンド塗りつぶし
		pID3D11DeviceContext->OMSetBlendState(pImpl->GetOpaqueBlend(), nullptr, 0xffffffff);
		//レンダーステート前面のみ描画
		pID3D11DeviceContext->RSSetState(pImpl->GetCullBackRasterizer());
		//デプスステンシル使用
		pID3D11DeviceContext->OMSetDepthStencilState(pImpl->GetDefaultDepthStencil(), 0);

	}

	shared_ptr<DefaultRenderTarget> DeviceResources::GetDefaultRenderTarget() {
		if (!pImpl->m_DefaultRenderTarget) {
			//デフォルトのレンダリングターゲットを作成
			pImpl->m_DefaultRenderTarget = make_shared<DefaultRenderTarget>();
		}
		return pImpl->m_DefaultRenderTarget;
	}
	shared_ptr<ShadowMapRenderTarget> DeviceResources::GetShadowMapRenderTarget(float ShadowMapDimension ) {
		if (!pImpl->m_ShadowMapRenderTarget) {
			//シャドウマップのレンダリングターゲットを作成
			pImpl->m_ShadowMapRenderTarget = make_shared<ShadowMapRenderTarget>(ShadowMapDimension);
		}
		return pImpl->m_ShadowMapRenderTarget;
	}
	shared_ptr<RenderState> DeviceResources::GetRenderState()const {
		return pImpl->m_RenderState;
	}



	void DeviceResources::ClearShadowmapViews() {
		auto ShadowTarget = GetShadowMapRenderTarget();
		ShadowTarget->ClearViews();
	}
	void DeviceResources::StartShadowmapDraw() {
		auto ShadowTarget = GetShadowMapRenderTarget();
		ShadowTarget->StartRenderTarget();
	}
	void DeviceResources::EndShadowmapDraw() {
		auto ShadowTarget = GetShadowMapRenderTarget();
		ShadowTarget->EndRenderTarget();
	}
	void DeviceResources::ClearDefaultViews(const bsm::Col4& col) {
		auto DefaultTarget = GetDefaultRenderTarget();
		DefaultTarget->ClearViews(col);
	}
	void DeviceResources::StartDefaultDraw() {
		auto DefaultTarget = GetDefaultRenderTarget();
		DefaultTarget->StartRenderTarget();
	}
	void DeviceResources::EndDefaultDraw() {
		auto DefaultTarget = GetDefaultRenderTarget();
		DefaultTarget->EndRenderTarget();
	}

	void DeviceResources::Present(unsigned int SyncInterval, unsigned int  Flags) {
		// バックバッファからフロントバッファに転送
		GetSwapChain()->Present(SyncInterval, Flags);
	}


	//--------------------------------------------------------------------------------------
	//	struct RenderState;
	//	用途: レンダリングステートクラス
	//--------------------------------------------------------------------------------------
	struct RenderState::Impl {
		//ブレンドステート
		ComPtr<ID3D11BlendState> m_OpaquePtr;
		ComPtr<ID3D11BlendState> m_AlphaBlendPtr;
		ComPtr<ID3D11BlendState> m_AlphaBlendExPtr;

		ComPtr<ID3D11BlendState> m_AdditivePtr;
		ComPtr<ID3D11BlendState> m_NonPremultipliedPtr;
		//デプスステンシルステート
		ComPtr<ID3D11DepthStencilState> m_DepthNonePtr;
		ComPtr<ID3D11DepthStencilState> m_DepthDefaultPtr;
		ComPtr<ID3D11DepthStencilState> m_DepthReadPtr;
		//ラスタライザステート
		ComPtr<ID3D11RasterizerState> m_CullNonePtr;
		ComPtr<ID3D11RasterizerState> m_CullNoneScissorPtr;
		ComPtr<ID3D11RasterizerState> m_CullFrontPtr;
		ComPtr<ID3D11RasterizerState> m_CullFrontScissorPtr;
		ComPtr<ID3D11RasterizerState> m_CullBackPtr;
		ComPtr<ID3D11RasterizerState> m_CullBackScissorPtr;
		ComPtr<ID3D11RasterizerState> m_WireframePtr;
		ComPtr<ID3D11RasterizerState> m_WireframeScissorPtr;
		//サンプラーステート
		ComPtr<ID3D11SamplerState> m_PointWrapPtr;
		ComPtr<ID3D11SamplerState> m_PointClampPtr;
		ComPtr<ID3D11SamplerState> m_LinearWrapPtr;
		ComPtr<ID3D11SamplerState> m_LinearClampPtr;
		ComPtr<ID3D11SamplerState> m_AnisotropicWrapPtr;
		ComPtr<ID3D11SamplerState> m_AnisotropicClampPtr;
		ComPtr<ID3D11SamplerState> m_ComparisonLinearPtr;
		//ミューテックス
		std::mutex Mutex;

		//--------------------------------------------------------------------------------------
		//	Impl(};
		//	用途: コンストラクタ
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		Impl() {}
		~Impl() {}
		//--------------------------------------------------------------------------------------
		//	void CreateBlendState(
		//	D3D11_BLEND srcBlend,		//ソースブレンド
		//	D3D11_BLEND destBlend,		//デストブレンド
		//	ID3D11BlendState** pResult	//受け取るインターフェイス
		//	);
		//	用途: ブレンドステートを作成するヘルパー関数
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend,
			ID3D11BlendState** pResult);
		//--------------------------------------------------------------------------------------
		//	void CreateDepthStencilState(
		//	bool enable,
		//	bool writeEnable,
		//	ID3D11DepthStencilState** pResult
		//	);
		//	用途: デプスステンシルステートを作成するヘルパー関数
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void CreateDepthStencilState(bool enable, bool writeEnable,
			ID3D11DepthStencilState** pResult);
		//--------------------------------------------------------------------------------------
		//	void CreateRasterizerState(
		//	D3D11_CULL_MODE cullMode,			//カリング
		//	D3D11_FILL_MODE fillMode,			//塗り
		//	ID3D11RasterizerState** pResult,		//受け取るインターフェイス
		//	bool Scissor = false				//画面分割するかどうか
		//	);
		//	用途: ラスタライザステートを作成するヘルパー関数
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
			ID3D11RasterizerState** pResult, bool Scissor = false);
		//--------------------------------------------------------------------------------------
		//	void CreateSamplerState(
		//	D3D11_FILTER filter,					//フィルター
		//	D3D11_TEXTURE_ADDRESS_MODE addressMode	//アドレスモード
		//	ID3D11SamplerState** pResult			//受け取るインターフェイス
		//	);
		//	用途: サンプラーステートを作成するヘルパー関数
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void CreateSamplerState(D3D11_FILTER filter,
			D3D11_TEXTURE_ADDRESS_MODE addressMode,
			ID3D11SamplerState** pResult);


	};

	//--------------------------------------------------------------------------------------
	//	void RenderState::Impl::CreateBlendState(
	//	D3D11_BLEND srcBlend,		//ソースブレンド
	//	D3D11_BLEND destBlend,		//デストブレンド
	//	ID3D11BlendState** pResult	//受け取るインターフェイス
	//	);
	//	用途: ブレンドステートを作成するヘルパー関数
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	void RenderState::Impl::CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend,
		ID3D11BlendState** pResult) {
		try {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();

			D3D11_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) ||
				(destBlend != D3D11_BLEND_ZERO);

			desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
			desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
			desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			HRESULT hr = pDx11Device->CreateBlendState(&desc, pResult);
			if (FAILED(hr)) {
				// 初期化失敗
				throw BaseException(
					L"ブレンドステート作成に失敗しました。",
					L"if(FAILED(pDx11Device->CreateBlendState()))",
					L"RenderState::Impl::CreateBlendState()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	void RenderState::Impl::CreateDepthStencilState(
	//	bool enable,
	//	bool writeEnable,
	//	ID3D11DepthStencilState** pResult
	//	);
	//	用途: デプスステンシルステートを作成するヘルパー関数
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	void RenderState::Impl::CreateDepthStencilState(bool enable, bool writeEnable,
		ID3D11DepthStencilState** pResult) {
		try {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();

			D3D11_DEPTH_STENCIL_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.DepthEnable = enable;
			desc.DepthWriteMask = writeEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

			desc.StencilEnable = false;
			desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

			desc.BackFace = desc.FrontFace;

			HRESULT hr = pDx11Device->CreateDepthStencilState(&desc, pResult);
			if (FAILED(hr)) {
				// 初期化失敗
				throw BaseException(
					L"デプスステンシルステート作成に失敗しました。",
					L"if(FAILED(pDx11Device->CreateDepthStencilState()))",
					L"RenderState::Impl::CreateDepthStencilState()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	void RenderState::Impl::CreateRasterizerState(
	//	D3D11_CULL_MODE cullMode,			//カリング
	//	D3D11_FILL_MODE fillMode,			//塗り
	//	ID3D11RasterizerState** pResult,		//受け取るインターフェイス
	//	bool Scissor = false				//画面分割するかどうか
	//	);
	//	用途: ラスタライザステートを作成するヘルパー関数
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	void RenderState::Impl::CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
		ID3D11RasterizerState** pResult, bool Scissor) {
		try {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();

			D3D11_RASTERIZER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.CullMode = cullMode;
			desc.FillMode = fillMode;
			desc.DepthClipEnable = true;
			desc.MultisampleEnable = true;
			desc.ScissorEnable = Scissor;

			HRESULT hr = pDx11Device->CreateRasterizerState(&desc, pResult);
			if (FAILED(hr)) {
				// 初期化失敗
				throw BaseException(
					L"ラスタライザステート作成に失敗しました。",
					L"if(FAILED(pDx11Device->CreateRasterizerState()))",
					L"RenderState::Impl::CreateRasterizerState()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	void RenderState::Impl::CreateSamplerState(
	//	D3D11_FILTER filter,					//フィルター
	//	D3D11_TEXTURE_ADDRESS_MODE addressMode	//アドレスモード
	//	ID3D11SamplerState** pResult			//受け取るインターフェイス
	//	);
	//	用途: サンプラーステートを作成するヘルパー関数
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	void RenderState::Impl::CreateSamplerState(D3D11_FILTER filter,
		D3D11_TEXTURE_ADDRESS_MODE addressMode,
		ID3D11SamplerState** pResult) {
		try {

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();

			D3D11_SAMPLER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.Filter = filter;

			desc.AddressU = addressMode;
			desc.AddressV = addressMode;
			desc.AddressW = addressMode;

			desc.MaxAnisotropy = (pDx11Device->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? 16 : 2;

			desc.MaxLOD = FLT_MAX;
			desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

			HRESULT hr = pDx11Device->CreateSamplerState(&desc, pResult);

			if (FAILED(hr)) {
				// 初期化失敗
				throw BaseException(
					L"サンプラーステート作成に失敗しました。",
					L"if(FAILED(pDx11Device->CreateSamplerState()))",
					L"RenderState::Impl::CreateSamplerState()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	class RenderState;
	//	用途: レンダリングステートクラス
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//	RenderState::RenderState();
	//	用途: コンストラクタ
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	RenderState::RenderState() :
		pImpl(new Impl())
	{
	}

	RenderState::~RenderState() {
	}

	//--------------------------------------------------------------------------------------
	//	用途: ブレンドステートアクセッサ
	//	戻り値: ブレンドステートのポインタ
	//--------------------------------------------------------------------------------------
	ID3D11BlendState* RenderState::GetOpaque()const {
		return Util::DemandCreate(pImpl->m_OpaquePtr, pImpl->Mutex, [&](ID3D11BlendState** pResult)
		{
			pImpl->CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_ZERO, pResult);
		});

	}

	ID3D11BlendState* RenderState::GetAlphaBlend()const {
		return Util::DemandCreate(pImpl->m_AlphaBlendPtr, pImpl->Mutex, [&](ID3D11BlendState** pResult)
		{
			pImpl->CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, pResult);
		});
	}

	ID3D11BlendState* RenderState::GetAlphaBlendEx()const {
		if (!pImpl->m_AlphaBlendExPtr) {
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();
			D3D11_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			HRESULT hr = pDx11Device->CreateBlendState(&desc, &pImpl->m_AlphaBlendExPtr);
			if (FAILED(hr)) {
				// 初期化失敗
				throw BaseException(
					L"ブレンドステート作成に失敗しました。",
					L"if(FAILED(pDx11Device->CreateBlendState()))",
					L"BasicState::GetAlphaBlendEx()"
				);
			}
		}
		return pImpl->m_AlphaBlendExPtr.Get();
	}


	ID3D11BlendState* RenderState::GetAdditive()const {
		return Util::DemandCreate(pImpl->m_AdditivePtr, pImpl->Mutex, [&](ID3D11BlendState** pResult)
		{
			pImpl->CreateBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, pResult);
		});
	}

	ID3D11BlendState*  RenderState::GetNonPremultiplied()const {
		return Util::DemandCreate(pImpl->m_NonPremultipliedPtr, pImpl->Mutex, [&](ID3D11BlendState** pResult)
		{
			pImpl->CreateBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, pResult);
		});
	}

	//--------------------------------------------------------------------------------------
	//	用途: デプスステンシルステートアクセッサ
	//	戻り値: デプスステンシルステートのポインタ
	//--------------------------------------------------------------------------------------
	ID3D11DepthStencilState* RenderState::GetDepthNone()const {
		return Util::DemandCreate(pImpl->m_DepthNonePtr, pImpl->Mutex, [&](ID3D11DepthStencilState** pResult)
		{
			pImpl->CreateDepthStencilState(false, false, pResult);
		});
	}

	ID3D11DepthStencilState* RenderState::GetDepthDefault()const {
		return Util::DemandCreate(pImpl->m_DepthDefaultPtr, pImpl->Mutex, [&](ID3D11DepthStencilState** pResult)
		{
			pImpl->CreateDepthStencilState(true, true, pResult);
		});
	}

	ID3D11DepthStencilState* RenderState::GetDepthRead()const {
		return Util::DemandCreate(pImpl->m_DepthReadPtr, pImpl->Mutex, [&](ID3D11DepthStencilState** pResult)
		{
			pImpl->CreateDepthStencilState(true, false, pResult);
		});
	}

	//--------------------------------------------------------------------------------------
	//	用途: ラスタライザステートアクセッサ
	//	戻り値: ラスタライザステートのポインタ
	//--------------------------------------------------------------------------------------
	ID3D11RasterizerState* RenderState::GetCullNone()const {
		return Util::DemandCreate(pImpl->m_CullNonePtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, pResult);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullNoneScissor()const {
		return Util::DemandCreate(pImpl->m_CullNoneScissorPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, pResult, true);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullFront()const {
		return Util::DemandCreate(pImpl->m_CullFrontPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID, pResult);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullFrontScissor()const {
		return Util::DemandCreate(pImpl->m_CullFrontScissorPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID, pResult, true);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullBack()const {
		return Util::DemandCreate(pImpl->m_CullBackPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, pResult);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullBackScissor()const {
		return Util::DemandCreate(pImpl->m_CullBackScissorPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, pResult, true);
		});
	}

	ID3D11RasterizerState* RenderState::GetWireframe()const {
		return Util::DemandCreate(pImpl->m_WireframePtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME, pResult);
		});
	}

	ID3D11RasterizerState* RenderState::GetWireframeScissor()const {
		return Util::DemandCreate(pImpl->m_WireframeScissorPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME, pResult, true);
		});
	}

	//--------------------------------------------------------------------------------------
	//	用途: サンプラーステートアクセッサ
	//	戻り値: サンプラーステートのポインタ
	//--------------------------------------------------------------------------------------
	ID3D11SamplerState* RenderState::GetPointWrap()const {
		return Util::DemandCreate(pImpl->m_PointWrapPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetPointClamp()const {
		return Util::DemandCreate(pImpl->m_PointClampPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetLinearWrap()const {
		return Util::DemandCreate(pImpl->m_LinearWrapPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetLinearClamp()const {
		return Util::DemandCreate(pImpl->m_LinearClampPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetAnisotropicWrap()const {
		return Util::DemandCreate(pImpl->m_AnisotropicWrapPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetAnisotropicClamp()const {
		return Util::DemandCreate(pImpl->m_AnisotropicClampPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetComparisonLinear()const {
		if (!pImpl->m_ComparisonLinearPtr) {
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			D3D11_SAMPLER_DESC comparisonSamplerDesc;
			ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
			comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
			comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
			comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
			comparisonSamplerDesc.BorderColor[0] = 1.0f;
			comparisonSamplerDesc.BorderColor[1] = 1.0f;
			comparisonSamplerDesc.BorderColor[2] = 1.0f;
			comparisonSamplerDesc.BorderColor[3] = 1.0f;
			comparisonSamplerDesc.MinLOD = 0.f;
			comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			comparisonSamplerDesc.MipLODBias = 0.f;
			comparisonSamplerDesc.MaxAnisotropy = 0;
			comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
			comparisonSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
			ThrowIfFailed(
				pDx11Device->CreateSamplerState(&comparisonSamplerDesc, &pImpl->m_ComparisonLinearPtr),
				L"比較用サンプラーの作成に失敗しました",
				L"pDx11Device->CreateSamplerState(&comparisonSamplerDesc, &pImpl->m_ComparisonSampler_linear)",
				L"RenderState::GetComparisonLinear()"
			);
		}
		return pImpl->m_ComparisonLinearPtr.Get();
	}

	//--------------------------------------------------------------------------------------
	//	struct RenderTarget::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct RenderTarget::Impl {
		D3D11_VIEWPORT m_ViewPort;	//ビューポート
		Impl() {
			ZeroMemory(&m_ViewPort, sizeof(D3D11_VIEWPORT));
		}
	};

	//--------------------------------------------------------------------------------------
	//	class RenderTarget;
	//	用途: レンダリングターゲット
	//--------------------------------------------------------------------------------------
	//構築と破棄
	RenderTarget::RenderTarget() :
		pImpl(new Impl())
	{}
	RenderTarget::~RenderTarget() {}


	const D3D11_VIEWPORT& RenderTarget::GetViewport() const {
		return pImpl->m_ViewPort;
	}

	void RenderTarget::SetViewport(const D3D11_VIEWPORT& Viewport) {
		pImpl->m_ViewPort = Viewport;
	}


	//--------------------------------------------------------------------------------------
	//	struct ShadowMapRenderTarget::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct ShadowMapRenderTarget::Impl {
		const float m_ShadowMapDimension;	//シャドウマップの大きさ
		ComPtr<ID3D11ShaderResourceView>	m_ShaderResourceView;	//シェーダリソースビュー
		ComPtr<ID3D11RasterizerState> m_ShadowRenderState;	//シャドウマップのラスタライザステート
		ComPtr<ID3D11Texture2D>		m_DepthStencil;		//深度ステンシルバッファ
		ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;	//深度ステンシルビュー
		Impl(float ShadowMapDimension) :
			m_ShadowMapDimension(ShadowMapDimension)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	//	class ShadowMapRenderTarget: public RenderTarget;
	//	用途: シャドウマップのレンダリングターゲット
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ShadowMapRenderTarget::ShadowMapRenderTarget(float ShadowMapDimension) :
		pImpl(new Impl(ShadowMapDimension))
	{
		try {
			//デバイスとコンテキストインターフェイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11Device = Dev->GetD3DDevice();
			auto pSwapChain = Dev->GetSwapChain();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

			//シャドウマップテクスチャの作成
			D3D11_TEXTURE2D_DESC shadowMapDesc;
			ZeroMemory(&shadowMapDesc, sizeof(D3D11_TEXTURE2D_DESC));
			shadowMapDesc.Width = static_cast<UINT>(pImpl->m_ShadowMapDimension);
			shadowMapDesc.Height = static_cast<UINT>(pImpl->m_ShadowMapDimension);
			shadowMapDesc.MipLevels = 1;
			shadowMapDesc.ArraySize = 1;
			shadowMapDesc.SampleDesc.Count = 1;
			shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
			shadowMapDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			shadowMapDesc.Format = DXGI_FORMAT_R32_TYPELESS;

			ThrowIfFailed(
				pD3D11Device->CreateTexture2D(&shadowMapDesc, nullptr, &pImpl->m_DepthStencil),
				L"シャドウマップテクスチャの作成に失敗しました。",
				L"pD3D11Device->CreateTexture2D(&shadowMapDesc, nullptr, &m_DepthStencil)",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
			);

			//深度ステンシルビュー作成
			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
			ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
			depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;

			ThrowIfFailed(
				pD3D11Device->CreateDepthStencilView(pImpl->m_DepthStencil.Get(), &depthStencilViewDesc, &pImpl->m_DepthStencilView),
				L"深度ステンシルビューの作成に失敗しました。",
				L"pD3D11Device->CreateDepthStencilView(&m_DepthStencil, &depthStencilViewDesc, &m_DepthStencilView)",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
			);

			//シェーダーリソースビュー作成
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			ThrowIfFailed(
				pD3D11Device->CreateShaderResourceView(pImpl->m_DepthStencil.Get(), &shaderResourceViewDesc, &pImpl->m_ShaderResourceView),
				L"シェーダーリソースビューの作成に失敗しました。",
				L"pD3D11Device->CreateShaderResourceView(m_DepthStencil.Get(), &shaderResourceViewDesc, &m_ShaderResourceView)",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
			);


			D3D11_RASTERIZER_DESC shadowRenderStateDesc;
			ZeroMemory(&shadowRenderStateDesc, sizeof(D3D11_RASTERIZER_DESC));
			shadowRenderStateDesc.CullMode = D3D11_CULL_FRONT;
			shadowRenderStateDesc.FillMode = D3D11_FILL_SOLID;
			shadowRenderStateDesc.DepthClipEnable = true;

			ThrowIfFailed(pD3D11Device->CreateRasterizerState(&shadowRenderStateDesc, &pImpl->m_ShadowRenderState),
				L"シャドウマップのラスタライザステートの作成に失敗しました。",
				L"pD3D11Device->CreateRasterizerState(&shadowRenderStateDesc,&pImpl->m_ShadowRenderState)",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
			);


			//ビューポートの作成
			//シャドウレンダリングビューポート
			D3D11_VIEWPORT ViewPort;
			ZeroMemory(&ViewPort, sizeof(D3D11_VIEWPORT));
			ViewPort.Height = pImpl->m_ShadowMapDimension;
			ViewPort.Width = pImpl->m_ShadowMapDimension;
			ViewPort.MinDepth = 0.f;
			ViewPort.MaxDepth = 1.f;
			SetViewport(ViewPort);
		}
		catch (...) {
			throw;
		}
	}

	ShadowMapRenderTarget::~ShadowMapRenderTarget() {}

	//アクセサ
	float ShadowMapRenderTarget::GetShadowMapDimension() const { return pImpl->m_ShadowMapDimension; }
	ID3D11ShaderResourceView* ShadowMapRenderTarget::GetShaderResourceView() const { return pImpl->m_ShaderResourceView.Get(); }
	ID3D11Texture2D* ShadowMapRenderTarget::GetDepthStencil() const { return pImpl->m_DepthStencil.Get(); }
	ID3D11DepthStencilView*	ShadowMapRenderTarget::GetDepthStencilView() const { return pImpl->m_DepthStencilView.Get(); }


	//レンダリングターゲットをクリアする
	void ShadowMapRenderTarget::ClearViews(const bsm::Col4& col) {
		//シャドウマップはcolは未使用
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//シャドウマップクリア
		pD3D11DeviceContext->ClearDepthStencilView(pImpl->m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);
	}
	//レンダリングターゲットを開始する
	void ShadowMapRenderTarget::StartRenderTarget() {
		//デバイスとコンテキストインターフェイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//レンダリングターゲットは深度ステンシルビューのみ指定
		ID3D11RenderTargetView* pnullView = nullptr;

		pD3D11DeviceContext->OMSetRenderTargets(1, &pnullView, pImpl->m_DepthStencilView.Get());
		//ビューポートの設定
		pD3D11DeviceContext->RSSetViewports(1, &GetViewport());
		//ラスタライザステートの設定(設定解除)
		pD3D11DeviceContext->RSSetState(nullptr);
		//シェーダーは指定しない
		//頂点シェーダの設定(ここでは指定しない)
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		//ピクセルシェーダの設定（使用しない）
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		//ジオメトリシェーダの設定（使用しない）
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);


	}
	//レンダリングターゲットを終了する
	void ShadowMapRenderTarget::EndRenderTarget() {
		//デバイスとコンテキストインターフェイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//レンダリングターゲットは深度ステンシルビューのみ指定
		ID3D11RenderTargetView* pnullView = nullptr;
		pD3D11DeviceContext->OMSetRenderTargets(1, &pnullView, nullptr);
		//ビューポートの設定
		pD3D11DeviceContext->RSSetViewports(1, &GetViewport());
		//ラスタライザステートの設定(設定解除)
		pD3D11DeviceContext->RSSetState(nullptr);
		//シェーダーは指定しない
		//頂点シェーダの設定(ここでは指定しない)
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		//ピクセルシェーダの設定（使用しない）
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		//ジオメトリシェーダの設定（使用しない）
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
	}

	//--------------------------------------------------------------------------------------
	//	struct DefaultRenderTarget::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct DefaultRenderTarget::Impl {
		//ビュー関連
		ComPtr<ID3D11RenderTargetView> m_D3D11RenderTargetView;	//レンダリングターゲットレビュー
		ComPtr<ID3D11Texture2D>		m_DepthStencil;		//深度ステンシルバッファ
		ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;	//深度ステンシルビュー

		ComPtr<ID2D1Bitmap1>		m_d2dTargetBitmap;

		Impl()
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	//	class DefaultRenderTarget : public RenderTarget;
	//	用途: デフォルトのレンダーターゲット
	//	＊デフォルトのレンダラー
	//--------------------------------------------------------------------------------------
	//構築
	DefaultRenderTarget::DefaultRenderTarget() :
		pImpl(new Impl())
	{
		try {

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11Device = Dev->GetD3DDevice();
			auto pSwapChain = Dev->GetSwapChain();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto pD2D11DeviceContext = Dev->GetD2DDeviceContext();


			//レンダリングターゲットビューの作成
			ComPtr<ID3D11Texture2D> pBackBuffer;
			//まずバックバッファのポインタを得る
			ThrowIfFailed(
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer),
				L"スワップチェーンからバックバッファの取得に失敗しました。",
				L"pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
			);
			//バックバッファからレンダリングターゲットのビューを作成する
			ThrowIfFailed(
				pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pImpl->m_D3D11RenderTargetView),
				L"DX11バックバッファからのレンダリングターゲットビューを作成に失敗しました。",
				L"pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_D3D11RenderTargetView)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
			);

			//深度テクスチャの作成
			D3D11_TEXTURE2D_DESC descDepth;
			ZeroMemory(&descDepth, sizeof(descDepth));
			descDepth.Width = App::GetApp()->GetGameWidth();
			descDepth.Height = App::GetApp()->GetGameHeight();
			descDepth.MipLevels = 1;
			descDepth.ArraySize = 1;
			descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDepth.SampleDesc.Count = 1;
			descDepth.SampleDesc.Quality = 0;
			descDepth.Usage = D3D11_USAGE_DEFAULT;
			descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			descDepth.CPUAccessFlags = 0;
			descDepth.MiscFlags = 0;

			ThrowIfFailed(
				pD3D11Device->CreateTexture2D(&descDepth, nullptr, &pImpl->m_DepthStencil),
				L"DX11深度テクスチャの作成失敗の作成に失敗しました。",
				L"pD3D11Device->CreateTexture2D(&descDepth, nullptr, &m_DepthStencil)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
			);

			//深度ステンシルビューの作成
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;

			ThrowIfFailed(
				pD3D11Device->CreateDepthStencilView(pImpl->m_DepthStencil.Get(), &descDSV, &pImpl->m_DepthStencilView),
				L"DX11深度ステンシルビューの作成に失敗しました。",
				L"pD3D11Device->CreateDepthStencilView(m_DepthStencil.Get(), &descDSV, &m_DepthStencilView)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
			);

			ComPtr<IDXGISurface2> dxgiBackBuffer;
			ThrowIfFailed(
				pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)),
				L"2dデバイスコンテキスト作成に失敗しました。",
				L"m_d2dDevice->CreateDeviceContext()",
				L"DeviceResources::Impl::CreateDeviceResources()"
			);


			ThrowIfFailed(
				pD2D11DeviceContext->CreateBitmapFromDxgiSurface(
					dxgiBackBuffer.Get(),
					nullptr,	//デフォルト設定
					&pImpl->m_d2dTargetBitmap
				),
				L"2dビットマップ作成に失敗しました。",
				L"pD2D11DeviceContext->CreateBitmapFromDxgiSurface()",
				L"DefaultRenderTarget::DefaultRenderTarget()"
			);

			pD2D11DeviceContext->SetTarget(pImpl->m_d2dTargetBitmap.Get());
			//グレースケール テキストのアンチエイリアシング
			pD2D11DeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

			//デフォルトビューポートのセット
			D3D11_VIEWPORT ViewPort;
			ZeroMemory(&ViewPort, sizeof(ViewPort));
			ViewPort.Width = (float)App::GetApp()->GetGameWidth();
			ViewPort.Height = (float)App::GetApp()->GetGameHeight();
			ViewPort.MinDepth = 0.0f;
			ViewPort.MaxDepth = 1.0f;
			ViewPort.TopLeftX = 0;
			ViewPort.TopLeftY = 0;
			SetViewport(ViewPort);


		}
		catch (...) {
			throw;
		}
	}
	DefaultRenderTarget::~DefaultRenderTarget() {}


	//アクセサ
	ID3D11RenderTargetView* DefaultRenderTarget::GetRenderTargetView() const { return pImpl->m_D3D11RenderTargetView.Get(); }
	ID3D11Texture2D* DefaultRenderTarget::GetDepthStencil() const { return pImpl->m_DepthStencil.Get(); }
	ID3D11DepthStencilView*	DefaultRenderTarget::GetDepthStencilView() const { return pImpl->m_DepthStencilView.Get(); }
	ID2D1Bitmap1*			DefaultRenderTarget::GetD2DTargetBitmap() const { return pImpl->m_d2dTargetBitmap.Get(); }


	//操作
	//スクリーン全体を指定の色でクリアする
	void DefaultRenderTarget::ClearViews(const bsm::Col4& col) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//バッファのクリア
		float Color[4] = { col.x, col.y, col.z, col.w };
		D3D11_VIEWPORT ViewPort;
		//ビューポートのセットアップ
		ZeroMemory(&ViewPort, sizeof(ViewPort));
		ViewPort.Width = (float)App::GetApp()->GetGameWidth();
		ViewPort.Height = (float)App::GetApp()->GetGameHeight();
		ViewPort.MinDepth = 0.0f;
		ViewPort.MaxDepth = 1.0f;
		ViewPort.TopLeftX = 0;
		ViewPort.TopLeftY = 0;
		pD3D11DeviceContext->RSSetViewports(1, &ViewPort);
		//レンダリングターゲットのクリア
		pD3D11DeviceContext->ClearRenderTargetView(pImpl->m_D3D11RenderTargetView.Get(), Color);
		//通常の深度バッファとステンシルバッファのクリア
		pD3D11DeviceContext->ClearDepthStencilView(pImpl->m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	//レンダリングターゲットを開始する
	void DefaultRenderTarget::StartRenderTarget() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		ID3D11RenderTargetView* pV = pImpl->m_D3D11RenderTargetView.Get();
		//レンダリングターゲットとステンシルを設定
		pD3D11DeviceContext->OMSetRenderTargets(1, &pV, pImpl->m_DepthStencilView.Get());
		//ビューポートの設定
		auto ViewPort = GetViewport();
		pD3D11DeviceContext->RSSetViewports(1, &ViewPort);

		D3D11_RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = (LONG)ViewPort.Width;
		rect.bottom = (LONG)ViewPort.Height;
		pD3D11DeviceContext->RSSetScissorRects(1, &rect);

		//シェーダーリソースビューのクリア
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
		pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
		//シェーダーは指定しない
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//ブレンドは指定しない
		pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	}
	//レンダリングターゲットを終了する
	void DefaultRenderTarget::EndRenderTarget() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//シェーダーリソースビューのクリア
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
		pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
		//シェーダーは指定しない
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//ブレンドは指定しない
		pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}

	//--------------------------------------------------------------------------------------
	//	struct ShaderResource::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct ShaderResource::Impl {
		Impl() {}
		~Impl() {}
	};
	ShaderResource::ShaderResource() :pImpl(new Impl()) {}
	ShaderResource::~ShaderResource() {}
	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::ReadBinaryFile(
	//		const wstring& fileName,		//ファイル名
	//		unique_ptr<uint8_t[]>& Data,	//バイナリデータの戻り
	//		size_t& CsoSz					//サイズの戻り
	//	);
	//	用途: バイナリファイルを読み込む
	//--------------------------------------------------------------------------------------
	void ShaderResource::ReadBinaryFile(const wstring& fileName, unique_ptr<uint8_t[]>& Data, size_t& CsoSz) {
		try {
			if (fileName == L"") {
				throw BaseException(
					L"ファイルが指定されていません",
					L"if(fileName == L\"\")",
					L"ShaderResource::ReadBinaryFile()"
				);
			}
			DWORD RetCode;
			RetCode = GetFileAttributes(fileName.c_str());
			if (RetCode == -1) {
				throw BaseException(
					L"ファイルが存在しません",
					fileName,
					L"ShaderResource::ReadBinaryFile()"
				);
			}

			ThrowIfFailed(
				BinaryReader::ReadEntireFile(fileName, Data, &CsoSz),
				L"ファイルの読み込みに失敗しました。",
				fileName,
				L"ShaderResource::ReadBinaryFile()"
			);

		}
		catch (...) {
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateVertexShader(
	//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
	//		size_t CsoSz,					//サイズ
	//		ID3D11VertexShader** pResult			//受け取るシェーダ
	//	);
	//	用途: バイナリデータから頂点シェーダを作成する
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateVertexShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11VertexShader** pResult) {
		try {
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//頂点シェーダーの作成
			ThrowIfFailed(
				pDx11Device->CreateVertexShader(Data.get(), CsoSz, nullptr, pResult),
				L"頂点シェーダの作成に失敗しました。",
				L"if( FAILED( pDx11Device->CreateVertexShader() ) )",
				L"ShaderResource::CreateVertexShader()"
			);

		}
		catch (...) {
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateInputLayout(
	//		unique_ptr<uint8_t[]>& Data,	//頂点シェーダのバイナリデータ
	//		size_t CsoSz,					//サイズ
	//		const D3D11_INPUT_ELEMENT_DESC* pElement,	//頂点定義
	//		UINT NumElement,							//頂点定義の数
	//		ID3D11InputLayout** pResult			//受け取るレイアウト
	//	);
	//	用途: バイナリデータからインプットレイアウトを作成する
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateInputLayout(unique_ptr<uint8_t[]>& Data, size_t CsoSz,
		const D3D11_INPUT_ELEMENT_DESC* pElement, UINT NumElement, ID3D11InputLayout** pResult) {
		try {
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//インプットレイアウトの作成
			HRESULT hr = pDx11Device->CreateInputLayout(
				pElement,	//頂点定義
				NumElement,		//頂点定義の数
				Data.get(),
				CsoSz,
				pResult
			);
			//失敗したら
			if (FAILED(hr))
			{
				throw basecross::BaseException(
					L"頂点シェーダのインプットレイアウトの作成に失敗しました。",
					L"if( FAILED( pDx11Device->CreateInputLayout() ) )",
					L"ShaderResource::CreateInputLayout()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreatePixelShader(
	//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
	//		size_t CsoSz,					//サイズ
	//		ID3D11PixelShader** pResult			//受け取るシェーダ
	//	);
	//	用途: バイナリデータからピクセルシェーダを作成する
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreatePixelShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11PixelShader** pResult) {
		try {
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//ピクセルシェーダ作成
			HRESULT hr = pDx11Device->CreatePixelShader(
				Data.get(),
				CsoSz,
				nullptr,
				pResult);
			if (FAILED(hr)) {
				throw basecross::BaseException(
					L"ピクセルシェーダの作成に失敗しました。",
					L"f( FAILED( pDx11Device->CreatePixelShader() ) )",
					L"ShaderResource::CreatePixelShader()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateGeometryShader(
	//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
	//		size_t CsoSz,					//サイズ
	//		ID3D11GeometryShader** pResult			//受け取るシェーダ
	//	);
	//	用途: バイナリデータからジオメトリシェーダを作成する
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11GeometryShader** pResult) {
		try {
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//ジオメトリシェーダ作成
			HRESULT hr = pDx11Device->CreateGeometryShader(
				Data.get(),
				CsoSz,
				nullptr,
				pResult);
			if (FAILED(hr)) {
				throw basecross::BaseException(
					L"ジオメトリシェーダの作成に失敗しました。",
					L"f( FAILED( pDx11Device->CreateGeometryShader() ) )",
					L"ShaderResource::CreateGeometryShader()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	void ShaderResource::CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz,
		const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries, UINT Stride, UINT NumStride, ID3D11GeometryShader** pResult) {
		try {
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();
			//ジオメトリシェーダ作成
			UINT StridBuff[1] = { Stride };
			HRESULT hr = pDx11Device->CreateGeometryShaderWithStreamOutput(
				Data.get(),
				CsoSz,
				SOEntries.data(),
				SOEntries.size(),
				StridBuff,
				NumStride,
				0,
				nullptr,
				pResult);
			if (FAILED(hr)) {
				throw basecross::BaseException(
					L"ジオメトリシェーダの作成に失敗しました。",
					L"f( FAILED( pDx11Device->CreateGeometryShader() ) )",
					L"ShaderResource::CreateGeometryShader()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateComputeShader(
	//		unique_ptr<uint8_t[]>& Data,	//バイナリデータ
	//		size_t CsoSz,					//サイズ
	//		ID3D11ComputeShader** pResult			//受け取るシェーダ
	//	);
	//	用途: バイナリデータからコンピュートシェーダを作成する
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateComputeShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11ComputeShader** pResult) {
		try {
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//ピクセルシェーダ作成
			HRESULT hr = pDx11Device->CreateComputeShader(
				Data.get(),
				CsoSz,
				nullptr,
				pResult);
			if (FAILED(hr)) {
				throw basecross::BaseException(
					L"コンピュートシェーダの作成に失敗しました。",
					L"f( FAILED( pDx11Device->CreateComputeShader() ) )",
					L"ShaderResource::CreateComputeShader()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateConstantBuffer(
	//		UINT BuffSize,					//サイズ
	//		ID3D11Buffer** pResult			//受け取るシェーダ
	//	);
	//	用途: コンスタントバッファを作成する
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateConstantBuffer(UINT BuffSize, ID3D11Buffer** pResult) {
		try {
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//コンスタントバッファの作成
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = BuffSize;
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			HRESULT hr = pDx11Device->CreateBuffer(&bd, NULL, pResult);
			if (FAILED(hr)) {
				// 初期化失敗
				throw BaseException(
					L"コンスタントバッファ作成に失敗しました。",
					L"if( FAILED( hr ) )",
					L"ShaderResource::CreateConstantBuffer()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
