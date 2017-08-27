/*!
@file VertexHelper.h
@brief 頂点定義、等
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
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
	/// 位置を持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置を持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct VertexPosition
	{
		VertexPosition()
		{ }

		VertexPosition(float x,float y,float z)
			: position(bsm::Vec3(x, y, z))
		{ }


		VertexPosition(bsm::Vec3 const& pos)
			: position(pos)
		{ }

		VertexPosition(FXMVECTOR pos)
		{ 
			XMStoreFloat3(&position, pos);
		}

		bsm::Vec3 position;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionLayout );
		}
	};

	//--------------------------------------------------------------------------------------
	/// 位置と色を持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionColorLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置と色を持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct VertexPositionColor
	{
		VertexPositionColor()
		{ }

		VertexPositionColor(bsm::Vec3 const& pos, bsm::Vec4 const& col)
			: position(pos),
			color(col)
		{ }

		VertexPositionColor(FXMVECTOR pos, FXMVECTOR col)
		{
			XMStoreFloat3(&position, pos);
			XMStoreFloat4(&color, col);
		}

		bsm::Vec3 position;
		bsm::Vec4 color;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionColorLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionColorLayout );
		}
	};

	//--------------------------------------------------------------------------------------
	/// 位置とテクスチャを持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionTextureLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置とテクスチャを持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct VertexPositionTexture
	{
		VertexPositionTexture()
		{ }

		VertexPositionTexture(bsm::Vec3 const& pos, bsm::Vec2 const& tex)
			: position(pos),
			textureCoordinate(tex)
		{ }

		VertexPositionTexture(FXMVECTOR pos, FXMVECTOR tex)
		{
			XMStoreFloat3(&position, pos);
			XMStoreFloat2(&textureCoordinate, tex);
		}

		bsm::Vec3 position;
		bsm::Vec2 textureCoordinate;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionTextureLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionTextureLayout );
		}
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線を持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線を持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormal
	{
		VertexPositionNormal()
		{ }

		VertexPositionNormal(bsm::Vec3 const& pos, bsm::Vec3 const& norm)
			: position(pos),
			normal(norm)
		{ }

		VertexPositionNormal(FXMVECTOR pos, FXMVECTOR norm)
		{
			XMStoreFloat3(&position, pos);
			XMStoreFloat3(&normal, norm);
		}

		bsm::Vec3 position;
		bsm::Vec3 normal;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionNormalLayout );
		}
	};

	//--------------------------------------------------------------------------------------
	/// 位置と色とテクスチャを持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionColorTextureLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置と色とテクスチャを持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct VertexPositionColorTexture
	{
		VertexPositionColorTexture()
		{ }

		VertexPositionColorTexture(bsm::Vec3 const& pos, bsm::Vec4 const& col, bsm::Vec2 const& tex)
			: position(pos),
			color(col),
			textureCoordinate(tex)
		{ }

		VertexPositionColorTexture(FXMVECTOR pos, FXMVECTOR col, FXMVECTOR tex)
		{
			XMStoreFloat3(&position, pos);
			XMStoreFloat4(&color, col);
			XMStoreFloat2(&textureCoordinate, tex);
		}

		bsm::Vec3 position;
		bsm::Vec4 color;
		bsm::Vec2 textureCoordinate;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionColorTextureLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionColorTextureLayout );
		}
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線と色を持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalColorLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線と色を持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalColor
	{
		VertexPositionNormalColor()
		{ }

		VertexPositionNormalColor(bsm::Vec3 const& pos, bsm::Vec3 const& norm, bsm::Vec4 const& col)
			: position(pos),
			normal(norm),
			color(col)
		{ }

		VertexPositionNormalColor(FXMVECTOR pos, FXMVECTOR norm, FXMVECTOR col)
		{
			XMStoreFloat3(&position, pos);
			XMStoreFloat3(&normal, norm);
			XMStoreFloat4(&color, col);
		}

		bsm::Vec3 position;
		bsm::Vec3 normal;
		bsm::Vec4 color;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalColorLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionNormalColorLayout );
		}
	};
	//--------------------------------------------------------------------------------------
	/// 位置と法線とテクスチャを持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線とテクスチャを持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalTexture
	{
		VertexPositionNormalTexture()
		{ }

		VertexPositionNormalTexture(bsm::Vec3 const& pos, bsm::Vec3 const& norm, bsm::Vec2 const& tex)
			: position(pos),
			normal(norm),
			textureCoordinate(tex)
		{ }

		VertexPositionNormalTexture(FXMVECTOR pos, FXMVECTOR norm, FXMVECTOR tex)
		{ 
			XMStoreFloat3(&position, pos);
			XMStoreFloat3(&normal, norm);
			XMStoreFloat2(&textureCoordinate, tex);
		}

		bsm::Vec3 position;
		bsm::Vec3 normal;
		bsm::Vec2 textureCoordinate;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalTextureLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionNormalTextureLayout );
		}
	};


	//--------------------------------------------------------------------------------------
	/// 位置と法線とテクスチャとスキニング情報を持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureSkinningLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES",0, DXGI_FORMAT_R32G32B32A32_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線とテクスチャとスキニング情報を持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct  VertexPositionNormalTextureSkinning{
		VertexPositionNormalTextureSkinning(){}
	
		VertexPositionNormalTextureSkinning(bsm::Vec3 const& pos, bsm::Vec3 const& norm, bsm::Vec2 const& tex,
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
		{ 
			XMStoreFloat3(&position, pos);
			XMStoreFloat3(&normal, norm);
			XMStoreFloat2(&textureCoordinate, tex);
			for (int i = 0; i < 4; i++){
				indices[i] = index[i];
				weights[i] = weight[i];
			}
		}


		bsm::Vec3 position;
		bsm::Vec3 normal;
		bsm::Vec2 textureCoordinate;
		uint32_t indices[4];
		float weights[4];

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalTextureSkinningLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionNormalTextureSkinningLayout );
		}
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線と色とテクスチャを持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalColorTextureLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線と色とテクスチャを持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalColorTexture
	{
		VertexPositionNormalColorTexture()
		{ }

		VertexPositionNormalColorTexture(bsm::Vec3 const& pos, bsm::Vec3 const& norm, bsm::Vec4 const& col, bsm::Vec2 const& tex)
			: position(pos),
			normal(norm),
			color(col),
			textureCoordinate(tex)
		{ }

		VertexPositionNormalColorTexture(FXMVECTOR pos, FXMVECTOR norm, FXMVECTOR col, CXMVECTOR tex)
		{ 
			XMStoreFloat3(&position, pos);
			XMStoreFloat3(&normal, norm);
			XMStoreFloat4(&color, col);
			XMStoreFloat2(&textureCoordinate, tex);
		}

		bsm::Vec3 position;
		bsm::Vec3 normal;
		bsm::Vec4 color;
		bsm::Vec2 textureCoordinate;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalColorTextureLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionNormalColorTextureLayout );
		}
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線とタンジェントとテクスチャを持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTangentTextureLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線とタンジェントとテクスチャを持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalTangentTexture
	{
		VertexPositionNormalTangentTexture()
		{ }


		VertexPositionNormalTangentTexture(bsm::Vec3 const& pos, bsm::Vec3 const& norm, bsm::Vec4 const& tan, bsm::Vec2 const& tex)
			: position(pos),
			normal(norm),
			tangent(tan),
			textureCoordinate(tex)
		{
		}

		VertexPositionNormalTangentTexture(FXMVECTOR pos, FXMVECTOR norm, FXMVECTOR tan, CXMVECTOR tex)
		{
			XMStoreFloat3(&position, pos);
			XMStoreFloat3(&normal, norm);
			XMStoreFloat4(&tangent, tan);
			XMStoreFloat2(&textureCoordinate, tex);
		}


		bsm::Vec3 position;
		bsm::Vec3 normal;
		bsm::Vec4 tangent;
		bsm::Vec2 textureCoordinate;


		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement() {
			return VertexPositionNormalTangentTextureLayout;
		}
		static UINT GetNumElements() {
			return  ARRAYSIZE(VertexPositionNormalTangentTextureLayout);
		}
	};


	//--------------------------------------------------------------------------------------
	/// 位置と法線とタンジェントとテクスチャとスキニング情報を持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTangentTextureSkinningLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES",0, DXGI_FORMAT_R32G32B32A32_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線とタンジェントとテクスチャとスキニング情報を持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct  VertexPositionNormalTangentTextureSkinning {
		VertexPositionNormalTangentTextureSkinning() {}

		VertexPositionNormalTangentTextureSkinning(bsm::Vec3 const& pos, bsm::Vec3 const& norm, bsm::Vec4 const& tan, bsm::Vec2 const& tex,
			uint32_t* index, float* weight)
			: position(pos),
			normal(norm),
			tangent(tan),
			textureCoordinate(tex)
		{
			for (int i = 0; i < 4; i++) {
				indices[i] = index[i];
				weights[i] = weight[i];
			}
		}

		VertexPositionNormalTangentTextureSkinning(FXMVECTOR pos, FXMVECTOR norm, FXMVECTOR tan, CXMVECTOR tex,
			uint32_t* index, float* weight)
		{
			XMStoreFloat3(&position, pos);
			XMStoreFloat3(&normal, norm);
			XMStoreFloat4(&tangent, tan);
			XMStoreFloat2(&textureCoordinate, tex);
			for (int i = 0; i < 4; i++) {
				indices[i] = index[i];
				weights[i] = weight[i];
			}
		}

		bsm::Vec3 position;
		bsm::Vec3 normal;
		bsm::Vec4 tangent;
		bsm::Vec2 textureCoordinate;
		uint32_t indices[4];
		float weights[4];

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement() {
			return VertexPositionNormalTangentTextureSkinningLayout;
		}
		static UINT GetNumElements() {
			return  ARRAYSIZE(VertexPositionNormalTangentTextureSkinningLayout);
		}
	};


	//--------------------------------------------------------------------------------------
	/// 位置と法線とタンジェントと色とテクスチャを持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTangentColorTextureLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線とタンジェントと色とテクスチャを持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalTangentColorTexture
	{
		VertexPositionNormalTangentColorTexture()
		{ }


		VertexPositionNormalTangentColorTexture(bsm::Vec3 const& pos, bsm::Vec3 const& norm, bsm::Vec4 const& tan, uint32_t rgba, bsm::Vec2 const& tex)
			: position(pos),
			normal(norm),
			tangent(tan),
			color(rgba),
			textureCoordinate(tex)
		{
		}

		VertexPositionNormalTangentColorTexture(FXMVECTOR pos, FXMVECTOR norm, FXMVECTOR tan, uint32_t rgba, CXMVECTOR tex)
			: color(rgba)
		{
			XMStoreFloat3(&this->position, pos);
			XMStoreFloat3(&this->normal, norm);
			XMStoreFloat4(&this->tangent, tan);
			XMStoreFloat2(&this->textureCoordinate, tex);
		}

		VertexPositionNormalTangentColorTexture(bsm::Vec3 const& position, bsm::Vec3 const& normal, bsm::Vec4 const& tangent, bsm::Vec4 const& color, bsm::Vec2 const& textureCoordinate)
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

		void SetColor( bsm::Vec4 const& color ) { SetColor( XMLoadFloat4( &color ) ); }
		void XM_CALLCONV SetColor( FXMVECTOR color );

		bsm::Vec3 position;
		bsm::Vec3 normal;
		bsm::Vec4 tangent;
		uint32_t color;
		bsm::Vec2 textureCoordinate;


		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalTangentColorTextureLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionNormalTangentColorTextureLayout );
		}
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線とタンジェントと色とテクスチャとスキニング情報を持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTangentColorTextureSkinningLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES",0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線とタンジェントと色とテクスチャとスキニング情報を持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct VertexPositionNormalTangentColorTextureSkinning : public VertexPositionNormalTangentColorTexture
	{
		VertexPositionNormalTangentColorTextureSkinning()
		{ }

		uint32_t indices;
		uint32_t weights;

		VertexPositionNormalTangentColorTextureSkinning(bsm::Vec3 const& position, bsm::Vec3 const& normal, bsm::Vec4 const& tangent, uint32_t rgba,
									bsm::Vec2 const& textureCoordinate, XMUINT4 const& indices, bsm::Vec4 const& weights)
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

		VertexPositionNormalTangentColorTextureSkinning(bsm::Vec3 const& position, bsm::Vec3 const& normal, bsm::Vec4 const& tangent, bsm::Vec4 const& color,
									bsm::Vec2 const& textureCoordinate, XMUINT4 const& indices, bsm::Vec4 const& weights)
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

		void SetBlendWeights( bsm::Vec4 const& weights ) { SetBlendWeights( XMLoadFloat4( &weights ) ); }
		void XM_CALLCONV SetBlendWeights( FXMVECTOR weights );

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalTangentColorTextureSkinningLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE( VertexPositionNormalTangentColorTextureSkinningLayout );
		}
	};


	//--------------------------------------------------------------------------------------
	/// 位置と色と行列を持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionColorMatrixLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};


	//--------------------------------------------------------------------------------------
	/// 位置と色と行列をを持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct VertexPositionColorMatrix
	{
		VertexPositionColorMatrix()
		{ }

		VertexPositionColorMatrix(bsm::Vec3 const& pos, bsm::Vec4 const& col, bsm::Mat4x4 const& mat)
			: position(pos),
			color(col),
			matrix(mat)
		{
		}

		VertexPositionColorMatrix(FXMVECTOR pos, FXMVECTOR col, FXMMATRIX const& mat)
		{
			XMStoreFloat3(&position, pos);
			XMStoreFloat4(&color, col);
			XMStoreFloat4x4(&matrix, mat);
		}

		bsm::Vec3 position;
		bsm::Vec4 color;
		bsm::Mat4x4 matrix;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement() {
			return VertexPositionColorMatrixLayout;
		}
		static UINT GetNumElements() {
			return  ARRAYSIZE(VertexPositionColorMatrixLayout);
		}
	};



	//--------------------------------------------------------------------------------------
	/// 位置とテクスチャと行列を持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionTextureMatrixLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置とテクスチャと行列を持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct  VertexPositionTextureMatrix
	{
		VertexPositionTextureMatrix()
		{ }

		VertexPositionTextureMatrix(bsm::Vec3 const& pos, bsm::Vec2 const& tex,
			bsm::Mat4x4 const& mat)
			: position(pos),
			textureCoordinate(tex),
			matrix(mat)
		{ }

		VertexPositionTextureMatrix(FXMVECTOR pos, FXMVECTOR tex,
			FXMMATRIX const& mat)
		{
			XMStoreFloat3(&position, pos);
			XMStoreFloat2(&textureCoordinate, tex);
			XMStoreFloat4x4(&matrix, mat);
		}

		bsm::Vec3 position;
		bsm::Vec2 textureCoordinate;
		bsm::Mat4x4 matrix;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement() {
			return VertexPositionTextureMatrixLayout;
		}
		static UINT GetNumElements() {
			return  ARRAYSIZE(VertexPositionTextureMatrixLayout);
		}
	};

	//--------------------------------------------------------------------------------------
	/// 位置と色とテクスチャと行列を持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionColorTextureMatrixLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};


	//--------------------------------------------------------------------------------------
	/// 位置と色とテクスチャと行列をを持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct VertexPositionColorTextureMatrix
	{
		VertexPositionColorTextureMatrix()
		{ }

		VertexPositionColorTextureMatrix(bsm::Vec3 const& pos, bsm::Vec4 const& col, bsm::Vec2 const& tex,
			bsm::Mat4x4 const& mat)
			: position(pos),
			color(col),
			textureCoordinate(tex),
			matrix(mat)
		{ }

		VertexPositionColorTextureMatrix(FXMVECTOR pos, FXMVECTOR col, FXMVECTOR tex,
			FXMMATRIX const& mat)
		{
			XMStoreFloat3(&position, pos);
			XMStoreFloat4(&color, col);
			XMStoreFloat2(&textureCoordinate, tex);
			XMStoreFloat4x4(&matrix, mat);
		}

		bsm::Vec3 position;
		bsm::Vec4 color;
		bsm::Vec2 textureCoordinate;
		bsm::Mat4x4 matrix;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement() {
			return VertexPositionColorTextureMatrixLayout;
		}
		static UINT GetNumElements() {
			return  ARRAYSIZE(VertexPositionColorTextureMatrixLayout);
		}
	};


	//--------------------------------------------------------------------------------------
	/// 位置と法線とテクスチャと行列を持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureMatrixLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	//--------------------------------------------------------------------------------------
	/// 位置と法線とテクスチャと行列を持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct  VertexPositionNormalTextureMatrix
	{
		VertexPositionNormalTextureMatrix()
		{ }

		VertexPositionNormalTextureMatrix(bsm::Vec3 const& pos, bsm::Vec3 const& norm, bsm::Vec2 const& tex,
			bsm::Mat4x4 const& mat)
			: position(pos),
			normal(norm),
			textureCoordinate(tex),
			matrix(mat)
		{ }

		VertexPositionNormalTextureMatrix(FXMVECTOR pos, FXMVECTOR norm, FXMVECTOR tex,
			FXMMATRIX const& mat)
		{
			XMStoreFloat3(&position, pos);
			XMStoreFloat3(&normal, norm);
			XMStoreFloat2(&textureCoordinate, tex);
			XMStoreFloat4x4(&matrix, mat);
		}

		bsm::Vec3 position;
		bsm::Vec3 normal;
		bsm::Vec2 textureCoordinate;
		bsm::Mat4x4 matrix;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalTextureMatrixLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE(VertexPositionNormalTextureMatrixLayout);
		}
	};




}
//end of namespace basecross.
