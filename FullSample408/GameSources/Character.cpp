/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//構築と破棄
	TilingPlate::TilingPlate(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position,
		float UPic,
		float VPic
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position),
		m_UPic(UPic),
		m_VPic(VPic)
	{
		try {
			if (m_UPic <= 0.0f || m_VPic <= 0.0f) {
				throw BaseException(
					L"UVに0以下は渡せません",
					L"if (m_UPic <= 0.0f || m_VPic <= 0.0f)",
					L"TilingPlate::TilingPlate()"
				);
			}
		}
		catch (...) {
			throw;
		}
	}

	TilingPlate::~TilingPlate() {

	}
	//初期化
	void TilingPlate::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetQuaternion(m_Qt);
		PtrTrans->SetPosition(m_Position);
		AddComponent<CollisionRect>();

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSquare(1.0f, vertices, indices);
		float UCount = m_Scale.x / m_UPic;
		float VCount = m_Scale.y / m_VPic;
		for (size_t i = 0; i < vertices.size(); i++) {
			if (vertices[i].textureCoordinate.x >= 1.0f) {
				vertices[i].textureCoordinate.x = UCount;
			}
			if (vertices[i].textureCoordinate.y >= 1.0f) {
				vertices[i].textureCoordinate.y = VCount;
			}
		}
		//描画コンポーネントの追加
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//自分に影が映りこむようにする
		PtrDraw->SetOwnShadowActive(true);
		//描画コンポーネントテクスチャの設定
		PtrDraw->SetTextureResource(L"SKY_TX");
		//タイリング設定
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
	}


	TilingFixedBox::TilingFixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position,
		float UPic,
		float VPic
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position),
		m_UPic(UPic),
		m_VPic(VPic)
	{}
	TilingFixedBox::~TilingFixedBox() {}
	//初期化
	void TilingFixedBox::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(m_Rotation);
		PtrTrans->SetPosition(m_Position);
		auto Coll = AddComponent<CollisionObb>();
		Coll->SetFixed(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		float UCount = m_Scale.x / m_UPic;
		float VCount = m_Scale.y / m_VPic;
		for (size_t i = 0; i < vertices.size(); i++) {
			if (vertices[i].textureCoordinate.x >= 1.0f) {
				vertices[i].textureCoordinate.x = UCount;
			}
			if (vertices[i].textureCoordinate.y >= 1.0f) {
				vertices[i].textureCoordinate.y = VCount;
			}
		}
		//描画コンポーネントの追加
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//自分に影が映りこむようにする
		PtrDraw->SetOwnShadowActive(true);
		//描画コンポーネントテクスチャの設定
		PtrDraw->SetTextureResource(L"WALL_TX");
		//タイリング設定
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);

	}

	//--------------------------------------------------------------------------------------
	//	マスキングスプライト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MaskSprite::MaskSprite(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureKey):
		GameObject(StagePtr),
		m_TextureKey(TextureKey)
	{}

	MaskSprite::~MaskSprite() {}

	void MaskSprite::OnCreate() {
		SetDrawLayer(2);
		float HelfSize = 0.5f;
		//頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vec3(-1.0, 1.0, 0),Col4(1.0f,1.0f,1.0f,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(1.0f, 1.0f, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-1.0f, -1.0f, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(0.0f, 1.0f)) },
			{ VertexPositionColorTexture(Vec3(1.0f, -1.0f, 0), Col4(1.0f, 1.0f, 1.0f, 1.0f), Vec2(1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//メッシュの作成（変更できる）
		m_SpriteMesh = MeshResource::CreateMeshResource(vertices, indices, true);


		SetAlphaActive(true);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(1.0f,1.0f,1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(0, 0, 0);
	}

	void MaskSprite::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderStatePtr = Dev->GetRenderState();
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
		ID3D11SamplerState* pSampler = nullptr;
		pSampler = RenderStatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//シェーダの設定
		//頂点シェーダ
		pID3D11DeviceContext->VSSetShader(VSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSPCTSprite::GetPtr()->GetInputLayout());
		//ピクセルシェーダ
		pID3D11DeviceContext->PSSetShader(PSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		//個別処理
		SpriteConstants sb;
		//コンスタントバッファの作成
		//行列の取得
		auto PtrTrans = GetComponent<Transform>();
		//行列の定義
		bsm::Mat4x4 World;
		World = PtrTrans->Get2DWorldMatrix();
		//エミッシブ
		sb.Emissive = Col4(0,0,0,1);
		//デフィーズはすべて通す
		sb.Diffuse = Col4(1, 1, 1, 1);
		//行列の設定
		sb.World = World;
		//テクスチャ
		auto shTex = App::GetApp()->GetResource<TextureResource>(m_TextureKey);
		pID3D11DeviceContext->PSSetShaderResources(0, 1, shTex->GetShaderResourceView().GetAddressOf());
		//コンスタントバッファの更新
		pID3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		//頂点シェーダに渡す
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ストライドとオフセット
		UINT stride = m_SpriteMesh->GetNumStride();
		UINT offset = 0;
		//描画方法のセット
		pID3D11DeviceContext->IASetPrimitiveTopology(m_SpriteMesh->GetPrimitiveTopology());
		//頂点バッファのセット
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_SpriteMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(m_SpriteMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画
		pID3D11DeviceContext->DrawIndexed(m_SpriteMesh->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}


}
//end basecross
