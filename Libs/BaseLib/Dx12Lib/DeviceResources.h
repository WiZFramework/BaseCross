/*!
@file DeviceResources.h
@brief �e�N�X�`���A���b�V���ADx12�f�o�C�X���\�[�X
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	/// ��������ɕێ�����Dx12�e�N�X�`�����\�[�X
	//--------------------------------------------------------------------------------------
	class TextureResource : public BaseResource {
		friend class ObjectFactory;
		void OnCreateWithParam(uint32_t Width, uint32_t Height, const vector<uint8_t>& data);
		void OnCreateWithParam(const wstring& FileName, const wstring& TexType);
		TextureResource();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TextureResource();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ��������Ƀe�N�X�`�����\�[�X�̍쐬
		@param[in]	Width ��
		@param[in]	Height ����
		@return	�e�N�X�`�����\�[�X��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		static  shared_ptr<TextureResource> CreateTextureResource(uint32_t Width, uint32_t Height, const vector<uint8_t>& data);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �摜�t�@�C������e�N�X�`�����\�[�X�̍쐬
		@param[in]	FileName �t�@�C����
		@param[in]	TexType �摜�^�C�v
		@return	�e�N�X�`�����\�[�X��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		static  shared_ptr<TextureResource> CreateTextureResource(const wstring& FileName, const wstring& TexType = L"WIC");
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`���̕��𓾂�
		@return	�e�N�X�`���̕�
		*/
		//--------------------------------------------------------------------------------------
		uint32_t GetTextureWidth();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`���̍����𓾂�
		@return	�e�N�X�`���̍���
		*/
		//--------------------------------------------------------------------------------------
		uint32_t GetTextureHeight();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �s�N�Z�����𓾂�
		@return	�s�N�Z����
		*/
		//--------------------------------------------------------------------------------------
		uint32_t GetPixelSize();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`���̃��\�[�X�f�X�N�v���^�𓾂�
		@return	�e�N�X�`���̃��\�[�X�f�X�N�v���^
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_RESOURCE_DESC& GetTextureResDesc() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`���̃��\�[�X�𓾂�
		@return	�e�N�X�`���̃��\�[�X
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12Resource> GetTexture() const;
		//�e�N�X�`���f�[�^
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`���f�[�^�𓾂�
		@return	�e�N�X�`���f�[�^�̔z��
		*/
		//--------------------------------------------------------------------------------------
		vector<uint8_t>& GetData() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �e�N�X�`���f�[�^��ݒ肷��
		@param[in]	data �f�[�^�̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void SetData(const vector<uint8_t>& data);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���\�[�X���X�V����
		@param[in]	commandList �R�}���h���X�g
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void UpdateResources(const ComPtr<ID3D12GraphicsCommandList>& commandList);
		//���z�֐��i��֐��j
		virtual void OnPreCreate()override {}
		virtual void OnCreate()override {}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	/// Dx12���b�V���N���X
	//--------------------------------------------------------------------------------------
	class MeshResource : public BaseResource {
		friend class ObjectFactory;
		ComPtr<ID3D12Resource> m_VertexBuffer;	//���_�o�b�t�@
		ComPtr<ID3D12Resource> m_IndexBuffer;	//�C���f�b�N�X�o�b�t�@
		ComPtr<ID3D12Resource> m_VertexBufferUploadHeap;
		ComPtr<ID3D12Resource> m_IndexBufferUploadHeap;
		D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
		D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
		UINT m_NumVertices;				//���_�̐�
		UINT m_NumIndicis;				//�C���f�b�N�X��
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
		@breaf �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MeshResource() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �C���f�b�N�X���܂ރ��b�V�����\�[�X�̍쐬
		@tparam	T ���_�̌^
		@param[in]	vertices ���_�̔z��
		@param[in]	indices �C���f�b�N�X�̔z��
		@param[in]	AccessWrite ���_�ύX�\���ǂ���
		@return	���b�V�����\�[�X��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static  shared_ptr<MeshResource> CreateMeshResource(const vector<T>& vertices, const vector<uint16_t>& indices, bool AccessWrite) {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			auto Ptr = ObjectFactory::Create<MeshResource>();
			Ptr->m_AccessWrite = AccessWrite;
			//�f�[�^�̃o�b�N�A�b�v
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
			//���_�o�b�t�@�̍쐬
			{
				ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_VertexBuffer)),
					L"���_�o�b�t�@�쐬�Ɏ��s���܂����B",
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
					L"���_�A�b�v���[�h�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);
				// Initialize the vertex buffer view.
				Ptr->m_VertexBufferView.BufferLocation = Ptr->m_VertexBuffer->GetGPUVirtualAddress();
				Ptr->m_VertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(T));
				Ptr->m_VertexBufferView.SizeInBytes = static_cast<UINT>(sizeof(T) * vertices.size());
			}
			//���_���̐ݒ�
			Ptr->m_NumVertices = static_cast<UINT>(vertices.size());
			//�C���f�b�N�X�o�b�t�@�̍쐬
			UINT indexBufferSize = static_cast<UINT>(sizeof(uint16_t) * indices.size());
			{
				ThrowIfFailed(Dev->GetDevice()->CreateCommittedResource(
					&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
					D3D12_HEAP_FLAG_NONE,
					&CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
					D3D12_RESOURCE_STATE_COPY_DEST,
					nullptr,
					IID_PPV_ARGS(&Ptr->m_IndexBuffer)),
					L"�C���f�b�N�X�o�b�t�@�쐬�Ɏ��s���܂����B",
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
					L"�C���f�b�N�X�A�b�v���[�h�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);

				// Describe the index buffer view.
				Ptr->m_IndexBufferView.BufferLocation = Ptr->m_IndexBuffer->GetGPUVirtualAddress();
				Ptr->m_IndexBufferView.Format = DXGI_FORMAT_R16_UINT;
				Ptr->m_IndexBufferView.SizeInBytes = indexBufferSize;

			}
			//�C���f�b�N�X���̐ݒ�
			Ptr->m_NumIndicis = static_cast<UINT>(indices.size());
			return Ptr;
		}


		//--------------------------------------------------------------------------------------
		/*!
		@brief �C���f�b�N�X���܂܂Ȃ����b�V�����\�[�X�̍쐬
		@tparam	T ���_�̌^
		@param[in]	vertices ���_�̔z��
		@param[in]	AccessWrite ���_�ύX�\���ǂ���
		@return	���b�V�����\�[�X��shared_ptr
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static  shared_ptr<MeshResource> CreateMeshResource(const vector<T>& vertices, bool AccessWrite) {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			auto Ptr = ObjectFactory::Create<MeshResource>();
			Ptr->m_AccessWrite = AccessWrite;

			//�f�[�^�̃o�b�N�A�b�v
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
					L"���_�o�b�t�@�쐬�Ɏ��s���܂����B",
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
					L"���_�A�b�v���[�h�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if(FAILED(pDx12Device->CreateCommittedResource())",
					L"MeshResource::CreateMeshResource()"
				);
				// Initialize the vertex buffer view.
				Ptr->m_VertexBufferView.BufferLocation = Ptr->m_VertexBuffer->GetGPUVirtualAddress();
				Ptr->m_VertexBufferView.StrideInBytes = static_cast<UINT>(sizeof(T));
				Ptr->m_VertexBufferView.SizeInBytes = static_cast<UINT>(sizeof(T) * vertices.size());
			}
			//���_���̐ݒ�
			Ptr->m_NumVertices = static_cast<UINT>(vertices.size());
			return Ptr;
		}

		//�v���~�e�B�u���\�[�X�\�z
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 4�p�`���ʂ̍쐬
		@param[in]	size		1�ӂ̃T�C�Y
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
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
		@breaf �L���[�u�i�����́j�̍쐬
		@param[in]	size		1�ӂ̃T�C�Y
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
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
		@breaf ���̂̍쐬
		@param[in]	Diameter		���a
		@param[in]	Tessellation	������
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
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
		@breaf �J�v�Z���̍쐬
		@param[in]	Diameter		���a
		@param[in]	Height,		����
		@param[in]	Tessellation	������
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCapsule(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false) {
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			Vector3 PointA(0, -Height / 2.0f, 0);
			Vector3 PointB(0, Height / 2.0f, 0);
			//Capsule�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateCapsule(Diameter, PointA, PointB, Tessellation, vertices, indices);
			return CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �V�����_�[�̍쐬
		@param[in]	Height,		����
		@param[in]	Diameter		���a
		@param[in]	Tessellation	������
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCylinder(float Height, float Diameter, size_t Tessellation, bool AccessWrite = false) {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Cylinder�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateCylinder(Height, Diameter, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �R�[���̍쐬
		@param[in]	Diameter		���a
		@param[in]	Height,		����
		@param[in]	Tessellation	������
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateCone(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false) {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Cone�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateCone(Diameter, Height, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �g�[���X�̍쐬
		@param[in]	Diameter		���a
		@param[in]	Thickness		�h�[�i�b�c�̑���
		@param[in]	Tessellation	������
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateTorus(float Diameter, float Thickness, size_t Tessellation, bool AccessWrite = false) {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Torus�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateTorus(Diameter, Thickness, Tessellation, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ��4�ʑ̂̍쐬
		@param[in]	Size		1�ӂ̃T�C�Y
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateTetrahedron(float Size, bool AccessWrite = false) {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Tetrahedron�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateTetrahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ��8�ʑ̂̍쐬
		@param[in]	Size		1�ӂ̃T�C�Y
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateOctahedron(float Size, bool AccessWrite = false) {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Octahedron�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateOctahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ��12�ʑ̂̍쐬
		@param[in]	Size		1�ӂ̃T�C�Y
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateDodecahedron(float Size, bool AccessWrite = false) {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Dodecahedron�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateDodecahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ��20�ʑ̂̍쐬
		@param[in]	Size		1�ӂ̃T�C�Y
		@param[in]	AccessWrite	���_��ύX�ł��邩�ǂ���
		@return	���\�[�X�̃X�}�[�g�|�C���^
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<MeshResource> CreateIcosahedron(float Size, bool AccessWrite = false) {
			//���_�z��
			vector<VertexPositionNormalTexture> vertices;
			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Icosahedron�̍쐬(�w���p�[�֐��𗘗p)
			MeshUtill::CreateIcosahedron(Size, vertices, indices);
			return MeshResource::CreateMeshResource<VertexPositionNormalTexture>(vertices, indices, AccessWrite);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ���_�o�b�t�@�r���[�̎擾
		@return	���_�o�b�t�@�r���[�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_VERTEX_BUFFER_VIEW& GetVertexBufferView() const {
			return m_VertexBufferView;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ���_���̎擾
		@return	���_��
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumVertices() const {
			return m_NumVertices;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �C���f�b�N�X�o�b�t�@�r���[�̎擾
		@return	�C���f�b�N�X�o�b�t�@�r���[�̎Q��
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_INDEX_BUFFER_VIEW& GetIndexBufferView() const {
			return m_IndexBufferView;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �C���f�b�N�X���̎擾
		@return	�C���f�b�N�X��
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumIndicis() const {
			return m_NumIndicis;
		}

		//����
		//--------------------------------------------------------------------------------------
		/*!
		@brief  ���_�̍X�V
		@tparam	T	���_�̌^
		@param[in] vertices	���_�̔z��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void UpdateVirtex(const vector<T>& vertices) {
			if (!m_AccessWrite) {
				ThrowBaseException(
					L"���̃��b�V���̒��_�ύX�͋�����Ă��܂���",
					L"if (!m_AccessWrite)",
					L"MeshResource::UpdateVirtex()"
				);
			}
			auto shptr = dynamic_pointer_cast< BackupData<T> >(m_BackupData);
			if (!shptr) {
				return;
			}
			if (shptr->m_Vertices.size() != vertices.size()) {
				return;
			}
			for (size_t i = 0; i < vertices.size();i++) {
				shptr->m_Vertices[i] = vertices[i];
			}
			m_DataRefresh = true;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief  ���\�[�X�̍X�V
		@tparam	T	���_�̌^
		@param[in] commandList	�R�}���h���X�g
		@return	�Ȃ�
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
			//���_�o�b�t�@�̍X�V
			D3D12_SUBRESOURCE_DATA vertexData = {};
			vertexData.pData = (void*)&shptr->m_Vertices[0];
			vertexData.RowPitch = shptr->m_Vertices.size() * sizeof(T);
			vertexData.SlicePitch = vertexData.RowPitch;
			UpdateSubresources<1>(commandList.Get(), m_VertexBuffer.Get(), m_VertexBufferUploadHeap.Get(), 0, 0, 1, &vertexData);
			commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_VertexBuffer.Get(),
				D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

			//�C���f�b�N�X�o�b�t�@�̍X�V
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
	//	�p�r: �u�����h�X�e�[�g
	//--------------------------------------------------------------------------------------
	enum class BlendState {
		Opaque,
		AlphaBlend,
		Additive,
		NonPremultiplied,
	};

	//--------------------------------------------------------------------------------------
	//	enum class DepthStencilState;
	//	�p�r: �f�v�X�X�e���V���X�e�[�g
	//--------------------------------------------------------------------------------------
	enum class DepthStencilState {
		None,
		Default,
		Read,
	};

	//--------------------------------------------------------------------------------------
	//	enum class RasterizerState;
	//	�p�r: ���X�^���C�U�X�e�[�g
	//--------------------------------------------------------------------------------------
	enum class RasterizerState {
		CullNone,
		CullFront,
		CullBack,
		Wireframe,
	};

	//--------------------------------------------------------------------------------------
	//	enum class SamplerState;
	//	�p�r: �T���v���[�X�e�[�g
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




	//--------------------------------------------------------------------------------------
	//	class DeviceResources;
	/*!
	DirectX12�f�o�C�X�N���X
	*/
	//--------------------------------------------------------------------------------------
	class DeviceResources {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �R���X�g���N�^
		@param[in] hWnd	�E�C���h�E�̃n���h��
		@param[in] isFullScreen �t���X�N���[�����ǂ���
		@param[in] Width		��
		@param[in] Height		����
		*/
		//--------------------------------------------------------------------------------------
		DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		//--------------------------------------------------------------------------------------
		/*!
		@breaf �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~DeviceResources();
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �t���[�����𓾂�
		@return	�t���[����
		*/
		//--------------------------------------------------------------------------------------
		UINT GetFrameCount() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �t���[���C���f�b�N�X�𓾂�
		@return	�t���[���C���f�b�N�X
		*/
		//--------------------------------------------------------------------------------------
		UINT GetFrameIndex() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �X���b�v�`�F�C���𓾂�
		@return	�X���b�v�`�F�C��
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<IDXGISwapChain3> GetSwapChain() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �f�o�C�X�𓾂�
		@return	�f�o�C�X
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12Device> GetDevice() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief  �����_�����O�^�[�Q�b�g�i�̃|�C���^�j�𓾂�
		@return	�����_�����O�^�[�Q�b�g�i�̃|�C���^�j
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12Resource>* GetRenderTargets() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �w�肵���C���f�b�N�X�̃����_�����O�^�[�Q�b�g�𓾂�
		@return	�w�肵���C���f�b�N�X�̃����_�����O�^�[�Q�b�g
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12Resource> GetRenderTarget(UINT Index) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R�}���h�A���P�[�^�𓾂�
		@return	�R�}���h�A���P�[�^
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R�}���h�L���[�𓾂�
		@return	�R�}���h�L���[
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12CommandQueue> GetCommandQueue() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �����_�����O�^�[�Q�b�g�r���[�̃q�[�v�̈�𓾂�
		@return	�����_�����O�^�[�Q�b�g�r���[�̃q�[�v�̈�
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12DescriptorHeap> GetRtvHeap() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�v�X�X�e���V���r���[�̃q�[�v�̈�𓾂�
		@return	�f�v�X�X�e���V���r���[�̃q�[�v�̈�
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D12DescriptorHeap> GetDsvHeap() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �����_�����O�^�[�Q�b�g�r���[�̃f�X�N�v���^�T�C�Y�𓾂�
		@return	�����_�����O�^�[�Q�b�g�r���[�̃f�X�N�v���^�T�C�Y
		*/
		//--------------------------------------------------------------------------------------
		UINT GetRtvDescriptorSize() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �r���[�|�[�g�𓾂�
		@return	�r���[�|�[�g
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_VIEWPORT& GetViewport() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V�U�[�̈�𓾂�
		@return	�V�U�[�̈�
		*/
		//--------------------------------------------------------------------------------------
		const D3D12_RECT& GetScissorRect() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R�}���h���X�g��ǉ�����
		@param[in] Tgt	�R�}���h���X�g�̃|�C���^�i���|�C���^�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void InsertDrawCommandLists(ID3D12CommandList* Tgt);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R�}���h���X�g�����s���t�����g�o�b�t�@�ւ̓]�����s��
		@param[in] SyncInterval	�C���^�[�o��
		@param[in] Flags	�t���O
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void Present(unsigned int SyncInterval, unsigned int  Flags);


		//--------------------------------------------------------------------------------------
		/*!
		@brief ���܂��Ă���R�}���h���X�g�����s���ăR�}���h���X�g�̃o�b�t�@���N���A����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ExecuteCommandLists();


		//--------------------------------------------------------------------------------------
		/*!
		@brief 1�O�̃t���[���̏�����҂�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void WaitForPreviousFrame();
		//���z�֐�
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V���h�E�}�b�v�̃N���A�i����`�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void ClearShadowmapViews() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V���h�E�}�b�v�`��̊J�n�i����`�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void StartShadowmapDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �V���h�E�}�b�v�`��̏I���i����`�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void EndShadowmapDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ʏ�`��̃N���A�i����`�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void ClearDefultViews(const Color4& col = Color4(0, 0, 0, 1.0f));
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ʏ�`��̊J�n�i����`�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void StartDefultDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �ʏ�`��̏I���i����`�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void EndDefultDraw() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �����_�����O�^�[�Q�b�g�ނ̏����i����`�j
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void InitRenderTargets() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���e���c�̍쐬��̏���
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void AfterInitContents();
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	/// �f�X�N�v���^�q�[�v���[�e�B���e�B
	//--------------------------------------------------------------------------------------
	namespace DescriptorHeap {
		static inline ComPtr<ID3D12DescriptorHeap> CreateDirect(const D3D12_DESCRIPTOR_HEAP_DESC& desc) {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12DescriptorHeap> Ret;
			ThrowIfFailed(Dev->GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&Ret)),
				L"CbvSrvUav�p�̃f�X�N�v���^�q�[�v�̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateDescriptorHeap()",
				L"DescriptorHeap::CreateDirect()"
			);
			return Ret;
		}
		static inline ComPtr<ID3D12DescriptorHeap> CreateCbvSrvUavHeap(UINT NumDescriptorHeap) {
			//CbvSrv�f�X�N�v���^�q�[�v
			D3D12_DESCRIPTOR_HEAP_DESC CbvSrvHeapDesc = {};
			CbvSrvHeapDesc.NumDescriptors = NumDescriptorHeap;
			CbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			CbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			return CreateDirect(CbvSrvHeapDesc);
		}
		static inline ComPtr<ID3D12DescriptorHeap> CreateSamplerHeap(UINT NumDescriptorHeap) {
			//�T���v���[�f�X�N�v���^�q�[�v
			D3D12_DESCRIPTOR_HEAP_DESC SamplerHeapDesc = {};
			SamplerHeapDesc.NumDescriptors = NumDescriptorHeap;
			SamplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			SamplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
			return CreateDirect(SamplerHeapDesc);
		}

	}

	//--------------------------------------------------------------------------------------
	/// ���[�g�V�O�l�`�����[�e�B���e�B
	//--------------------------------------------------------------------------------------
	namespace RootSignature {
		static inline ComPtr<ID3D12RootSignature> CreateDirect(const D3D12_ROOT_SIGNATURE_DESC& desc) {
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3DBlob> signature;
			ComPtr<ID3DBlob> error;
			ComPtr<ID3D12RootSignature> Ret;
			ThrowIfFailed(D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error),
				L"���[�g�V�O�l�`���̃V���A���C�Y�Ɏ��s���܂���",
				L"D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error)",
				L"RootSignature::Create()"
			);
			ThrowIfFailed(
				Dev->GetDevice()->CreateRootSignature(0, signature->GetBufferPointer(),
					signature->GetBufferSize(), IID_PPV_ARGS(&Ret)),
				L"���[�g�V�O�l�`���̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateRootSignature()",
				L"RootSignature::Create()"
			);
			return Ret;
		}
		//��ԃV���v���ȃ��[�g�V�O�l�`��
		static inline ComPtr<ID3D12RootSignature> CreateSimple() {
			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			return CreateDirect(rootSignatureDesc);
		}
		//�R���X�^���g�o�b�t�@�̂�
		static inline ComPtr<ID3D12RootSignature> CreateCbv() {
			CD3DX12_DESCRIPTOR_RANGE ranges[1];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
			CD3DX12_ROOT_PARAMETER rootParameters[1];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_ALL);

			D3D12_ROOT_SIGNATURE_FLAGS rootSignatureFlags =
				D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, rootSignatureFlags);
			return CreateDirect(rootSignatureDesc);
		}
		//�V�F�[�_���\�[�X�ƃT���v���[
		static inline ComPtr<ID3D12RootSignature> CreateSrvSmp() {

			CD3DX12_DESCRIPTOR_RANGE ranges[2];
			ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
			ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);

			CD3DX12_ROOT_PARAMETER rootParameters[2];
			rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
			rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);

			CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
			rootSignatureDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

			return CreateDirect(rootSignatureDesc);
		}

		//�V�F�[�_���\�[�X�ƃT���v���[�ƃR���X�^���g�o�b�t�@
		static inline ComPtr<ID3D12RootSignature> CreateSrvSmpCbv() {

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

			return CreateDirect(rootSignatureDesc);
		}

	}

	//--------------------------------------------------------------------------------------
	/// �T���v���[���[�e�B���e�B�i�ύX�\�j
	//--------------------------------------------------------------------------------------
	namespace DynamicSampler {
		static inline void CreateSampler(const SamplerState State, D3D12_CPU_DESCRIPTOR_HANDLE& Handle) {
			D3D12_SAMPLER_DESC samplerDesc = {};
			//�f�t�H���g�����Ă���
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
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			Dev->GetDevice()->CreateSampler(&samplerDesc, Handle);
		}
	}


	//--------------------------------------------------------------------------------------
	/// �p�C�v���C���X�e�[�g���[�e�B���e�B
	//--------------------------------------------------------------------------------------
	namespace PipelineState {
		static inline ComPtr<ID3D12PipelineState> CreateDirect(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc) {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12PipelineState> Ret;
			ThrowIfFailed(Dev->GetDevice()->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&Ret)),
				L"�p�C�v���C���X�e�[�g�̍쐬�Ɏ��s���܂���",
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
			//���ʃJ�����O
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
	}


	//--------------------------------------------------------------------------------------
	/// �R�}���h���X�g���[�e�B���e�B
	//--------------------------------------------------------------------------------------
	namespace CommandList {
		static inline  ComPtr<ID3D12GraphicsCommandList> CreateDefault(const ComPtr<ID3D12PipelineState>& pipelineState) {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ComPtr<ID3D12GraphicsCommandList> Ret;
			ThrowIfFailed(Dev->GetDevice()->CreateCommandList(
				0,
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				Dev->GetCommandAllocator().Get(),
				pipelineState.Get(),
				IID_PPV_ARGS(&Ret)),
				L"�R�}���h���X�g�̍쐬�Ɏ��s���܂���",
				L"Dev->GetDevice()->CreateCommandList()",
				L"CommandList::CreateDefault()"
			);
			return Ret;
		}
		static inline  void Reset(const ComPtr<ID3D12PipelineState>& pipelineState, const ComPtr<ID3D12GraphicsCommandList>& commandList) {
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ThrowIfFailed(commandList->Reset(Dev->GetCommandAllocator().Get(), pipelineState.Get()),
				L"�R�}���h���X�g�̃��Z�b�g�Ɏ��s���܂���",
				L"commandList->Reset(Dev->GetCommandAllocator().Get(),pipelineState.Get())",
				L"CommandList::Reset()"
			);

		}
		static inline  void Close(const ComPtr<ID3D12GraphicsCommandList>& commandList) {
			ThrowIfFailed(commandList->Close(),
				L"�R�}���h���X�g�̃N���[�Y�Ɏ��s���܂���",
				L"commandList->Close()",
				L"CommandList::Close()"
			);

		}


	}

	class TextureResource;

	//--------------------------------------------------------------------------------------
	/// �V�F�[�_���\�[�X(�e�N�X�`��)���[�e�B���e�B�N���X
	//--------------------------------------------------------------------------------------
	class ShaderResource {
	public:
		ShaderResource();
		virtual ~ShaderResource();
		void SetTextureResource(const shared_ptr<TextureResource>& textureResorce, const D3D12_CPU_DESCRIPTOR_HANDLE& hadle);
		void UpdateResources(const ComPtr<ID3D12GraphicsCommandList>& commandList);
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	class MeshResource;

	//--------------------------------------------------------------------------------------
	/// VS��PS�����`��p�R���e�N�X�g�i�e�I�u�W�F�N�g�̕`��N���X�j
	//--------------------------------------------------------------------------------------
	class VSPSDrawContext : public ObjectInterface {
	public:
		enum class CreateParam {
			CreateSimple,
			CreateSrvSmp,
			CreateCbv,
			CreateSrvSmpCbv
		};
	private:
		CreateParam m_CreateParam;
		D3D12_GRAPHICS_PIPELINE_STATE_DESC m_PineLineDesc;
		ComPtr<ID3D12PipelineState> m_PipelineState;
		//�R�}���h���X�g
		ComPtr<ID3D12GraphicsCommandList> m_CommandList;
		//���[�g�V�O�l�`���̎擾
		ComPtr<ID3D12RootSignature> GetRootSignature() const;
		//�`��T�u�֐�
		void DrawIndexedBase(const shared_ptr<MeshResource>& Mesh);
		void DrawVertexBase(const shared_ptr<MeshResource>& Mesh);
		//�p�C�v���C���̃��Z�b�g
		void ResetPipeLine();
		//�V�F�[�_���\�[�X�̍X�V
		void UpdateShaderResource();
		//�V���v���ȃI�u�W�F�N�g�̍\�z
		void CreateSimple();
		//�V�F�[�_���\�[�X�ƃT���v���[�����I�u�W�F�N�g�̍\�z
		void CreateSrvSmp();
		//�R���X�^���g�o�b�t�@�̂ݎ��I�u�W�F�N�g�̍\�z
		void CreateCbv();
		//�V�F�[�_���\�[�X�ƃT���v���[�ƃR���X�^���g�o�b�t�@�����I�u�W�F�N�g�̍\�z
		void CreateSrvSmpCbv();
	public:
		VSPSDrawContext(CreateParam param);
		virtual ~VSPSDrawContext();
		//���z�֐�
		virtual void OnPreCreate() override {}
		virtual void OnCreate()override;
		void SetTextureResource(const shared_ptr<TextureResource>& textureResorce);
		void SetSamplerState(const SamplerState State);
		void SetBlendState(const D3D12_BLEND_DESC& State);
		void SetBlendState(const BlendState Mode);
		void SetRasterizerState(const RasterizerState Mode);
		void SetRasterizerState(const D3D12_RASTERIZER_DESC& State);
		void SetDepthStencilState(const DepthStencilState Mode);
		void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& State);

		void CreateConstantBuffer(UINT BufferSize);
		void UpdateConstantBuffer(void* SrcBuffer, UINT BufferSize);

		ComPtr<ID3D12GraphicsCommandList> GetCommandList() const {
			return m_CommandList;
		}

		//�p�C�v���C���X�e�[�g
		template<typename Vertex, typename VS, typename PS>
		void CreateDefault3DPipelineCmdList() {
			m_PipelineState = PipelineState::CreateDefault3D<Vertex, VS, PS>(GetRootSignature(), m_PineLineDesc);
			m_CommandList = CommandList::CreateDefault(m_PipelineState);
			CommandList::Close(m_CommandList);
		}
		template<typename Vertex, typename VS, typename PS>
		void CreateDefault2DPipelineCmdList() {
			m_PipelineState = PipelineState::CreateDefault2D<Vertex, VS, PS>(GetRootSignature(), m_PineLineDesc);
			m_CommandList = CommandList::CreateDefault(m_PipelineState);
			CommandList::Close(m_CommandList);
		}

		template<typename Vertex>
		void DrawIndexed(const shared_ptr<MeshResource>& Mesh) {
			ResetPipeLine();
			Mesh->UpdateResources<Vertex>(m_CommandList);
			UpdateShaderResource();
			DrawIndexedBase(Mesh);
		}
		template<typename Vertex>
		void DrawVertex(const shared_ptr<MeshResource>& Mesh) {
			ResetPipeLine();
			Mesh->UpdateResources<Vertex>(m_CommandList);
			UpdateShaderResource();
			DrawVertexBase(Mesh);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���܂��Ă���R�}���h���X�g�����s���ăR�}���h���X�g�̃o�b�t�@���N���A����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void ExecuteCommandLists();


	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;

	};

	//--------------------------------------------------------------------------------------
	///	�V�F�[�_�֘A���\�[�X�̃C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	class Dx12ShaderResource {
	protected:
		Dx12ShaderResource();
		virtual ~Dx12ShaderResource();
		//�V�F�[�_�A�N�Z�T
		ID3DBlob* GetShaderBlob(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr);
		ComPtr<ID3DBlob>& GetShaderBlobComPtr(const wstring& Filename, ComPtr<ID3DBlob>& ShaderComPtr);
	private:
		//�R�s�[�֎~
		Dx12ShaderResource(const Dx12ShaderResource&) = delete;
		Dx12ShaderResource& operator=(const Dx12ShaderResource&) = delete;
		//���[�u�֎~
		Dx12ShaderResource(const Dx12ShaderResource&&) = delete;
		Dx12ShaderResource& operator=(const Dx12ShaderResource&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	///	�V���O���g���V�F�[�_�̐e�N���X(�V�F�[�_�̎�ނɂ�����炸�����N���X�g�p)
	//--------------------------------------------------------------------------------------
	template<typename ShaderType>
	class Dx12Shader : public Dx12ShaderResource {
		wstring m_Filename;
	protected:
		//�f���[�^�[
		struct Deleter
		{
			void operator()(ShaderType *p) { delete p; }
		};
		ComPtr<ID3DBlob> m_ShaderPtr;
		//�\�z�Ɣj��
		Dx12Shader<ShaderType>(const wstring& Filename) : Dx12ShaderResource(), m_Filename(Filename) {}
		virtual ~Dx12Shader() {}
		static unique_ptr<ShaderType, Deleter> m_Ptr;
	public:
		//�V�F�[�_���J�A�N�Z�b�T
		ID3DBlob* GetShader() {
			return GetShaderBlob(m_Filename, m_ShaderPtr);
		}
		ComPtr<ID3DBlob>& GetShaderComPtr() {
			return GetShaderBlobComPtr(m_Filename, m_ShaderPtr);
		}
		//�C���X�^���X�擾
		static unique_ptr<ShaderType, Deleter>& GetPtr() {
			if (!m_Ptr) {
				m_Ptr.reset(new ShaderType());
			}
			return m_Ptr;
		}
	};


	//������ʃV�F�[�_�[
	//�V�F�[�_�錾�p�}�N��
#define DECLARE_DX12SHADER(ShaderName) class ShaderName : public Dx12Shader<ShaderName>{ \
	public: \
		ShaderName(); \
	};
	//�V�F�[�_���̗p�}�N��
#define IMPLEMENT_DX12SHADER(ShaderName,CsoFilename) unique_ptr<ShaderName, ShaderName::Deleter> ShaderName::m_Ptr; \
	ShaderName::ShaderName() : \
	Dx12Shader(CsoFilename){}





}
//end basecross