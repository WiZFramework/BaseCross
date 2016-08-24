/*!
@file VertexHelper.h
@brief ���_��`�A��
*/

#pragma once

#include "stdafx.h"

namespace basecross{


    #if (DIRECTXMATH_VERSION < 305) && !defined(XM_CALLCONV)
    #define XM_CALLCONV __fastcall
    typedef const XMVECTOR& HXMVECTOR;
    typedef const XMMATRIX& FXMMATRIX;
    #endif

	//--------------------------------------------------------------------------------------
	/// �ʒu�������̓��C�A�E�g�̒�`
	//--------------------------------------------------------------------------------------
	const D3D12_INPUT_ELEMENT_DESC VertexPositionLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�������_�̒�`
	//--------------------------------------------------------------------------------------
	struct VertexPosition
	{
		VertexPosition()
		{ }

		VertexPosition(float x,float y,float z)
			: position(Vector3(x, y, z))
		{ }


		VertexPosition(XMFLOAT3 const& pos)
			: position(pos)
		{ }

		VertexPosition(FXMVECTOR pos)
			: position(pos)
		{ }

		Vector3 position;

		static const D3D12_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionLayout );
		}
	};


	//--------------------------------------------------------------------------------------
	/// �ʒu�ƐF�������̓��C�A�E�g�̒�`
	//--------------------------------------------------------------------------------------
	const D3D12_INPUT_ELEMENT_DESC VertexPositionColorLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�ƐF�������_�̒�`
	//--------------------------------------------------------------------------------------
	struct VertexPositionColor
	{
		VertexPositionColor()
		{ }

		VertexPositionColor(XMFLOAT3 const& pos, XMFLOAT4 const& col)
			: position(pos),
			color(col)
		{ }

		VertexPositionColor(FXMVECTOR pos, FXMVECTOR col)
			: position(pos),
			color(col)
		{
		}

		Vector3 position;
		Color4 color;

		static const D3D12_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionColorLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionColorLayout );
		}
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�ƃe�N�X�`���������̓��C�A�E�g�̒�`
	//--------------------------------------------------------------------------------------
	const D3D12_INPUT_ELEMENT_DESC VertexPositionTextureLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�ƃe�N�X�`���������_�̒�`
	//--------------------------------------------------------------------------------------
	struct VertexPositionTexture
	{
		VertexPositionTexture()
		{ }

		VertexPositionTexture(XMFLOAT3 const& pos, XMFLOAT2 const& tex)
			: position(pos),
			textureCoordinate(tex)
		{ }

		VertexPositionTexture(FXMVECTOR pos, FXMVECTOR tex)
			: position(pos),
			textureCoordinate(tex)
		{
		}

		Vector3 position;
		Vector2 textureCoordinate;

		static const D3D12_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionTextureLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionTextureLayout );
		}
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���������̓��C�A�E�g�̒�`
	//--------------------------------------------------------------------------------------
	const D3D12_INPUT_ELEMENT_DESC VertexPositionNormalLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���������_�̒�`
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormal
	{
		VertexPositionNormal()
		{ }

		VertexPositionNormal(XMFLOAT3 const& pos, XMFLOAT3 const& norm)
			: position(pos),
			normal(norm)
		{ }

		VertexPositionNormal(FXMVECTOR pos, FXMVECTOR norm)
			: position(pos),
			normal(norm)
		{
		}

		Vector3 position;
		Vector3 normal;

		static const D3D12_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionNormalLayout );
		}
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�ƐF�ƃe�N�X�`���������̓��C�A�E�g�̒�`
	//--------------------------------------------------------------------------------------
	const D3D12_INPUT_ELEMENT_DESC VertexPositionColorTextureLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�ƐF�ƃe�N�X�`���������_�̒�`
	//--------------------------------------------------------------------------------------
	struct VertexPositionColorTexture
	{
		VertexPositionColorTexture()
		{ }

		VertexPositionColorTexture(XMFLOAT3 const& pos, XMFLOAT4 const& col, XMFLOAT2 const& tex)
			: position(pos),
			color(col),
			textureCoordinate(tex)
		{ }

		VertexPositionColorTexture(FXMVECTOR pos, FXMVECTOR col, FXMVECTOR tex)
			: position(pos),
			color(col),
			textureCoordinate(tex)
		{
		}

		Vector3 position;
		Color4 color;
		Vector2 textureCoordinate;

		static const D3D12_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionColorTextureLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionColorTextureLayout );
		}
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���ƐF�������̓��C�A�E�g�̒�`
	//--------------------------------------------------------------------------------------
	const D3D12_INPUT_ELEMENT_DESC VertexPositionNormalColorLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���ƐF�������_�̒�`
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalColor
	{
		VertexPositionNormalColor()
		{ }

		VertexPositionNormalColor(XMFLOAT3 const& pos, XMFLOAT3 const& norm, XMFLOAT4 const& col)
			: position(pos),
			normal(norm),
			color(col)
		{ }

		VertexPositionNormalColor(FXMVECTOR pos, FXMVECTOR norm, FXMVECTOR col)
			: position(pos),
			normal(norm),
			color(col)
		{
		}

		Vector3 position;
		Vector3 normal;
		Color4 color;

		static const D3D12_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalColorLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionNormalColorLayout );
		}
	};



	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���ƃe�N�X�`���������̓��C�A�E�g�̒�`
	//--------------------------------------------------------------------------------------
	const D3D12_INPUT_ELEMENT_DESC VertexPositionNormalTextureLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���ƃe�N�X�`���������_�̒�`
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalTexture
	{
		VertexPositionNormalTexture()
		{ }

		VertexPositionNormalTexture(XMFLOAT3 const& pos, XMFLOAT3 const& norm, XMFLOAT2 const& tex)
			: position(pos),
			normal(norm),
			textureCoordinate(tex)
		{ }

		VertexPositionNormalTexture(FXMVECTOR pos, FXMVECTOR norm, FXMVECTOR tex)
			: position(pos),
			normal(norm),
			textureCoordinate(tex)
		{ }

		Vector3 position;
		Vector3 normal;
		Vector2 textureCoordinate;

		static const D3D12_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalTextureLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionNormalTextureLayout );
		}
	};


	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���ƃe�N�X�`���ƃX�L�j���O���������̓��C�A�E�g�̒�`
	//--------------------------------------------------------------------------------------
	const D3D12_INPUT_ELEMENT_DESC VertexPositionNormalTextureSkinningLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,     0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES",0, DXGI_FORMAT_R32G32B32A32_UINT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};


	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���ƃe�N�X�`���ƃX�L�j���O���������_�̒�`
	//--------------------------------------------------------------------------------------
	struct  VertexPositionNormalTextureSkinning
	{
		VertexPositionNormalTextureSkinning(){}
	
		VertexPositionNormalTextureSkinning(XMFLOAT3 const& pos, XMFLOAT3 const& norm, XMFLOAT2 const& tex,
			uint32_t* index,float* weight)
			: position(pos),
			normal(norm),
			textureCoordinate(tex)
		{ 
			for(int i = 0;i < 4;i++){
				indices[i] = index[i];
				weights[i] = weight[i];
			}
		}

		VertexPositionNormalTextureSkinning(FXMVECTOR pos, FXMVECTOR norm, FXMVECTOR tex,
			uint32_t* index, float* weight)
			: position(pos),
			normal(norm),
			textureCoordinate(tex)
		{ 
			for (int i = 0; i < 4; i++){
				indices[i] = index[i];
				weights[i] = weight[i];
			}
		}


		Vector3 position;
		Vector3 normal;
		Vector2 textureCoordinate;
		uint32_t indices[4];
		float weights[4];

		static const D3D12_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalTextureSkinningLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionNormalTextureSkinningLayout );
		}
	};



	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���ƐF�ƃe�N�X�`���������̓��C�A�E�g�̒�`
	//--------------------------------------------------------------------------------------
	const D3D12_INPUT_ELEMENT_DESC VertexPositionNormalColorTextureLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,  0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,  0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���ƐF�ƃe�N�X�`���������_�̒�`
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalColorTexture
	{
		VertexPositionNormalColorTexture()
		{ }

		VertexPositionNormalColorTexture(XMFLOAT3 const& pos, XMFLOAT3 const& norm, XMFLOAT4 const& col, XMFLOAT2 const& tex)
			: position(pos),
			normal(norm),
			color(col),
			textureCoordinate(tex)
		{ }

		VertexPositionNormalColorTexture(FXMVECTOR pos, FXMVECTOR norm, FXMVECTOR col, CXMVECTOR tex)
			: position(pos),
			normal(norm),
			color(col),
			textureCoordinate(tex)
		{ }

		Vector3 position;
		Vector3 normal;
		Color4 color;
		Vector2 textureCoordinate;

		static const D3D12_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalColorTextureLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionNormalColorTextureLayout );
		}
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���ƃ^���W�F���g�ƐF�ƃe�N�X�`���������̓��C�A�E�g�̒�`
	//--------------------------------------------------------------------------------------
	const D3D12_INPUT_ELEMENT_DESC VertexPositionNormalTangentColorTextureLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���ƃ^���W�F���g�ƐF�ƃe�N�X�`���������_�̒�`
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalTangentColorTexture
	{
		VertexPositionNormalTangentColorTexture()
		{ }


		VertexPositionNormalTangentColorTexture(XMFLOAT3 const& pos, XMFLOAT3 const& norm, XMFLOAT4 const& tan, uint32_t rgba, XMFLOAT2 const& tex)
			: position(pos),
			normal(norm),
			tangent(tan),
			color(rgba),
			textureCoordinate(tex)
		{
		}

		VertexPositionNormalTangentColorTexture(FXMVECTOR pos, FXMVECTOR norm, FXMVECTOR tan, uint32_t rgba, CXMVECTOR tex)
			: position(pos),
			normal(norm),
			tangent(tan),
			color(rgba),
			textureCoordinate(tex)
		{
		}

		VertexPositionNormalTangentColorTexture(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& tangent, XMFLOAT4 const& color, XMFLOAT2 const& textureCoordinate)
			: position(position),
			normal(normal),
			tangent(tangent),
			textureCoordinate(textureCoordinate)
		{
			SetColor( color );
		}

		VertexPositionNormalTangentColorTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR tangent, CXMVECTOR color, CXMVECTOR textureCoordinate)
		{
			XMStoreFloat3(&this->position, position);
			XMStoreFloat3(&this->normal, normal);
			XMStoreFloat4(&this->tangent, tangent);
			XMStoreFloat2(&this->textureCoordinate, textureCoordinate);

			SetColor( color );
		}

		void SetColor( XMFLOAT4 const& color ) { SetColor( XMLoadFloat4( &color ) ); }
		void XM_CALLCONV SetColor( FXMVECTOR color );

		Vector3 position;
		Vector3 normal;
		Vector4 tangent;
		uint32_t color;
		Vector2 textureCoordinate;


		static const D3D12_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalTangentColorTextureLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionNormalTangentColorTextureLayout );
		}
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���ƃ^���W�F���g�ƐF�ƃe�N�X�`���ƃX�L�j���O���������̓��C�A�E�g�̒�`
	//--------------------------------------------------------------------------------------
	const D3D12_INPUT_ELEMENT_DESC VertexPositionNormalTangentColorTextureSkinningLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES",0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// �ʒu�Ɩ@���ƃ^���W�F���g�ƐF�ƃe�N�X�`���ƃX�L�j���O���������_�̒�`
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalTangentColorTextureSkinning : public VertexPositionNormalTangentColorTexture
	{
		VertexPositionNormalTangentColorTextureSkinning()
		{ }

		uint32_t indices;
		uint32_t weights;

		VertexPositionNormalTangentColorTextureSkinning(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& tangent, uint32_t rgba,
									XMFLOAT2 const& textureCoordinate, XMUINT4 const& indices, XMFLOAT4 const& weights)
			: VertexPositionNormalTangentColorTexture(position,normal,tangent,rgba,textureCoordinate)
		{
			SetBlendIndices( indices );
			SetBlendWeights( weights );
		}

		VertexPositionNormalTangentColorTextureSkinning(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR tangent, uint32_t rgba, CXMVECTOR textureCoordinate,
									XMUINT4 const& indices, CXMVECTOR weights)
			: VertexPositionNormalTangentColorTexture(position,normal,tangent,rgba,textureCoordinate)
		{
			SetBlendIndices( indices );
			SetBlendWeights( weights );
		}

		VertexPositionNormalTangentColorTextureSkinning(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& tangent, XMFLOAT4 const& color,
									XMFLOAT2 const& textureCoordinate, XMUINT4 const& indices, XMFLOAT4 const& weights)
			: VertexPositionNormalTangentColorTexture(position,normal,tangent,color,textureCoordinate)
		{
			SetBlendIndices( indices );
			SetBlendWeights( weights );
		}

		VertexPositionNormalTangentColorTextureSkinning(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR tangent, CXMVECTOR color, CXMVECTOR textureCoordinate,
									XMUINT4 const& indices, CXMVECTOR weights)
			: VertexPositionNormalTangentColorTexture(position,normal,tangent,color,textureCoordinate)
		{
			SetBlendIndices( indices );
			SetBlendWeights( weights );
		}

		void SetBlendIndices( XMUINT4 const& indices );

		void SetBlendWeights( XMFLOAT4 const& weights ) { SetBlendWeights( XMLoadFloat4( &weights ) ); }
		void XM_CALLCONV SetBlendWeights( FXMVECTOR weights );

		static const D3D12_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalTangentColorTextureSkinningLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionNormalTangentColorTextureSkinningLayout );
		}
	};

}
//end of namespace basecross.