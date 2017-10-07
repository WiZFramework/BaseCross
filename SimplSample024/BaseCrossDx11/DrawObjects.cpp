/*!
@file DrawObjects.cpp
@brief 描画オブジェクト実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	PNT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	PNTDrawObject::PNTDrawObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	PNTDrawObject::~PNTDrawObject() {}

	void PNTDrawObject::OnCreate() {
		//タグの追加
		AddTag(L"PNTDrawObject");
	}


	void PNTDrawObject::AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
		const shared_ptr<TextureResource>& TextureRes,
		const Mat4x4& WorldMat,
		bool Trace, bool Wrap) {
		DrawObject Obj;
		Obj.m_MeshRes = MeshRes;
		Obj.m_TextureRes = TextureRes;
		Obj.m_WorldMatrix = WorldMat;
		Obj.m_Trace = Trace;
		Obj.m_Wrap = Wrap;
		if (Trace) {
			m_TraceDrawObjectVec.push_back(Obj);
		}
		else {
			m_DrawObjectVec.push_back(Obj);
		}
	}

	void PNTDrawObject::OnDrawSub(vector<DrawObject>& ObjectVec, PNTStaticConstantBuffer& sb) {
		auto PtrGameStage = GetStage<GameStage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//サンプラーの準備
		ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
		ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//個別処理
		for (auto& v : ObjectVec) {
			//転置する
			v.m_WorldMatrix.transpose();
			//ワールド行列の決定
			sb.World = v.m_WorldMatrix;
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBPNTStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBPNTStatic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, v.m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(v.m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//テクスチャの設定
			ID3D11ShaderResourceView* pNull[1] = { 0 };
			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.m_TextureRes->GetShaderResourceView().GetAddressOf());
			//サンプラー
			if (v.m_Wrap) {
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerWrap);
			}
			else {
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerClamp);
			}
			//ブレンドステート
			if (v.m_Trace) {
				//透明処理
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				//透明処理の場合は、ラスタライザステートを変更して2回描画
				//ラスタライザステート（裏面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
				//ラスタライザステート（表面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//透明処理しない
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
				//ラスタライザステート（表面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
		}
	}

	void PNTDrawObject::OnDraw() {
		auto PtrGameStage = GetStage<GameStage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//各オブジェクト共通処理
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//行列の定義
		Mat4x4 View, Proj;
		//ライティング
		Vec4 LightDir;
		PtrGameStage->GetCamera().GetViewProjMatrix(View, Proj);
		PtrGameStage->GetLightDir(LightDir);
		//ビュー行列の決定
		//転置する
		View.transpose();
		//射影行列の決定
		//転置する
		Proj.transpose();
		//コンスタントバッファの準備
		PNTStaticConstantBuffer sb;
		sb.View = View;
		sb.Projection = Proj;
		sb.LightDir = LightDir;
		//ディフューズ
		sb.Diffuse = Col4(0.8f, 0.8f, 0.8f, 1.0f);
		//エミッシブ加算。
		sb.Emissive = Col4(0.4f, 0.4f, 0.4f, 0);
		//スペキュラー。
		sb.Specular = Col4(0.2f, 0.2f, 0.2f, 0);
		//カメラの位置
		Vec3 CameraEye = PtrGameStage->GetCamera().m_CamerEye;
		sb.EyePos = CameraEye;
		//サブ関数呼び出し(不透明)
		OnDrawSub(m_DrawObjectVec, sb);
		//--------------------------------------------------------
		//透明の3Dオブジェクトをカメラからの距離でソート
		//以下は、オブジェクトを引数に取りboolを返すラムダ式
		//--------------------------------------------------------
		auto func = [&](DrawObject& Left, DrawObject& Right)->bool {
			auto LeftPos = Left.m_WorldMatrix.transInMatrix();
			auto RightPos = Right.m_WorldMatrix.transInMatrix();
			auto LeftLen = bsm::length(LeftPos - CameraEye);
			auto RightLen = bsm::length(RightPos - CameraEye);
			return (LeftLen > RightLen);
		};
		//ラムダ式を使ってソート
		std::sort(m_TraceDrawObjectVec.begin(), m_TraceDrawObjectVec.end(), func);
		//サブ関数呼び出し(透明)
		OnDrawSub(m_TraceDrawObjectVec, sb);
		//後始末
		Dev->InitializeStates();
		//描画用の配列をクリア
		m_DrawObjectVec.clear();
		m_TraceDrawObjectVec.clear();
	}


	//--------------------------------------------------------------------------------------
	//	パーティクル
	//	*1つのエミッターを持ち、複数のParticleSpriteを保持する
	//--------------------------------------------------------------------------------------
	Particle::Particle(size_t Count, ParticleDrawOption Option) :
		ObjectInterface(),
		m_DrawOption(Option),
		m_ParticleSpriteVec(Count),
		m_EmitterPos(0, 0, 0),
		m_TotalTime(0),
		m_MaxTime(0)
	{}
	Particle::~Particle() {}

	void Particle::Draw(const shared_ptr<ParticleDrawObject>& Manager) {
		for (auto Psp : m_ParticleSpriteVec) {
			if (Psp.m_Active && !m_TextureResource.expired()) {
				Manager->AddParticle(Psp, m_DrawOption,
					m_EmitterPos, m_TextureResource.lock());
			}
		}
	}


	//--------------------------------------------------------------------------------------
	///	マルチパーティクル(パーティクルエフェクト)
	//--------------------------------------------------------------------------------------
	MultiParticle::MultiParticle(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_AddType(false)
	{}
	MultiParticle::~MultiParticle() {}

	shared_ptr<Particle> MultiParticle::InsertParticle(size_t Count, ParticleDrawOption Option) {
		for (size_t i = 0; i < m_ParticleVec.size(); i++) {
			//もし非アクティブのパーティクルがあれば初期化してリターン
			if (!m_ParticleVec[i]->IsActive()) {
				m_ParticleVec[i]->Reflesh(Count, Option);
				return m_ParticleVec[i];
			}
		}
		//新しいパーティクルを追加
		shared_ptr<Particle> ParticlePtr = ObjectFactory::Create<Particle>(Count, Option);
		m_ParticleVec.push_back(ParticlePtr);
		return ParticlePtr;
	}


	void MultiParticle::OnUpdate() {
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto ParticlePtr : GetParticleVec()) {
			ParticlePtr->AddTotalTime(ElapsedTime);
			for (auto& rParticleSprite : ParticlePtr->GetParticleSpriteVec()) {
				if (rParticleSprite.m_Active) {
					//移動速度に従って移動させる
					rParticleSprite.m_LocalPos += rParticleSprite.m_Velocity * ElapsedTime;
					if (ParticlePtr->m_TotalTime >= ParticlePtr->m_MaxTime) {
						//制限時間になったら
						rParticleSprite.m_Active = false;
					}
				}
			}
		}
	}


	void MultiParticle::OnDraw() {
		auto PtrGameStage = GetStage<GameStage>();
		auto DrawPtrAdd = PtrGameStage->FindTagGameObject<ParticleDrawObject>(L"ParticleDrawObjectAdd");
		auto DrawPtrAlpha = PtrGameStage->FindTagGameObject<ParticleDrawObject>(L"ParticleDrawObjectAlpha");

		if (m_ParticleVec.size() > 0) {
			for (auto Ptr : m_ParticleVec) {
				if (Ptr->IsActive()) {
					if (IsAddType()) {
						Ptr->Draw(DrawPtrAdd);
					}
					else {
						Ptr->Draw(DrawPtrAlpha);
					}
				}
			}
		}
	}



	//--------------------------------------------------------------------------------------
	///	パーティクル描画オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と消滅
	ParticleDrawObject::ParticleDrawObject(const shared_ptr<Stage>& StagePtr, bool AddType) :
		GameObject(StagePtr),
		m_MaxInstance(2000),
		m_ZBufferUse(true),
		m_SamplerWrap(false),
		m_AddType(AddType)
	{}
	ParticleDrawObject::~ParticleDrawObject() {}

	void ParticleDrawObject::CreateParticleBuffers() {
		try {
			float HelfSize = 0.5f;
			bsm::Vec4 col(1.0f, 1.0f, 1.0f, 1.0f);
			//頂点配列
			vector<VertexPositionColorTexture> vertices = {
				{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),  col,Vec2(0.0f, 0.0f)) },
				{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), col, Vec2(1.0f, 0.0f)) },
				{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0),  col,Vec2(0.0f, 1.0f)) },
				{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0),  col, Vec2(1.0f, 1.0f)) },
			};
			//インデックス配列
			vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
			//メッシュの作成
			m_MeshResource = MeshResource::CreateMeshResource(vertices, indices, false);
			//インスタンス行列バッファの作成
			//Max値で作成する
			vector<Mat4x4> matrices(m_MaxInstance, Mat4x4());
			MeshResource::CreateDynamicVertexBuffer(m_MatrixBuffer, matrices);
		}
		catch (...) {
			throw;
		}
	}


	//初期化
	void ParticleDrawObject::OnCreate() {
		CreateParticleBuffers();
		//タグの追加
		if (m_AddType) {
			AddTag(L"ParticleDrawObjectAdd");
		}
		else {
			AddTag(L"ParticleDrawObjectAlpha");
		}
	}

	void ParticleDrawObject::AddParticle(const ParticleSprite& rParticleSprite, ParticleDrawOption Option,
		const bsm::Vec3& EmitterPos, const shared_ptr<TextureResource>& TextureRes) {
		if (m_DrawParticleSpriteVec.size() >= m_MaxInstance) {
			//パーティクルスプライトが上限を超えた
			return;
		}
		auto PtrGameStage = GetStage<GameStage>();
		//カメラの位置
		Vec3 CameraEye = PtrGameStage->GetCamera().m_CamerEye;
		Vec3 CameraAt = PtrGameStage->GetCamera().m_CamerAt;
		Vec3 WorldPos = rParticleSprite.m_LocalPos + EmitterPos;
		float ToCaneraLength = bsm::length(CameraEye - WorldPos);
		Vec3 LocalScale;
		LocalScale.x = rParticleSprite.m_LocalScale.x;
		LocalScale.y = rParticleSprite.m_LocalScale.y;
		LocalScale.z = 1.0f;

		Vec3 Temp;
		Quat Qt;
		Mat4x4 RotMatrix;
		Vec3 DefUp(0, 1.0f, 0);
		switch (Option) {
		case ParticleDrawOption::Billboard:
		{
			Temp = CameraAt - CameraEye;
			Vec2 TempVec2(Temp.x, Temp.z);
			if (bsm::length(TempVec2) < 0.1f) {
				DefUp = bsm::Vec3(0, 0, 1.0f);
			}
			Temp.normalize();
			RotMatrix = XMMatrixLookAtLH(bsm::Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix = bsm::inverse(RotMatrix);
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
		}
		break;
		case ParticleDrawOption::Faceing:
		{
			Temp = WorldPos - CameraEye;
			Vec2 TempVec2(Temp.x, Temp.z);
			if (bsm::length(TempVec2) < 0.1f) {
				DefUp = bsm::Vec3(0, 0, 1.0f);
			}
			RotMatrix = XMMatrixLookAtLH(bsm::Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix = bsm::inverse(RotMatrix);
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
		}
		break;
		case ParticleDrawOption::FaceingY:
			Temp = WorldPos - CameraEye;
			Temp.normalize();
			Qt = XMQuaternionRotationRollPitchYaw(0, atan2(Temp.x, Temp.z), 0);
			Qt.normalize();
			break;
		case ParticleDrawOption::Normal:
			Qt = rParticleSprite.m_LocalQt;
			Qt.normalize();
			break;
		}

		bsm::Mat4x4 matrix;
		matrix.affineTransformation(
			LocalScale,
			Vec3(0, 0, 0),
			Qt,
			WorldPos
		);
		DrawParticleSprite Item;
		Item.m_ToCaneraLength = ToCaneraLength;
		Item.m_WorldMatrix = matrix;
		Item.m_TextureRes = TextureRes;
		Item.m_Diffuse = rParticleSprite.m_Color;
		m_DrawParticleSpriteVec.push_back(Item);
	}

	void ParticleDrawObject::OnDraw() {
		auto PtrGameStage = GetStage<GameStage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		if (m_DrawParticleSpriteVec.size() <= 0) {
			return;
		}

		//カメラ位置でソート
		auto func = [](DrawParticleSprite& Left, DrawParticleSprite& Right)->bool {
			return (Left.m_ToCaneraLength > Right.m_ToCaneraLength);
		};
		std::sort(m_DrawParticleSpriteVec.begin(), m_DrawParticleSpriteVec.end(), func);
		//先頭のテクスチャを得る
		auto  NowTexPtr = m_DrawParticleSpriteVec[0].m_TextureRes;
		size_t NowStartIndex = 0;
		size_t NowDrawCount = 0;

		shared_ptr<TextureResource> NowTexRes = m_DrawParticleSpriteVec[0].m_TextureRes;
		bsm::Col4 NowDiffuse = bsm::Col4(1, 1, 1, 1);
		for (size_t i = 0; i < m_DrawParticleSpriteVec.size(); i++) {
			if (m_DrawParticleSpriteVec[i].m_TextureRes != NowTexRes ||
				m_DrawParticleSpriteVec[i].m_Diffuse != NowDiffuse) {
				m_InstancVec.push_back(InstanceDrawStr(NowStartIndex, NowDrawCount, NowTexRes, NowDiffuse));
				NowStartIndex = i;
				NowDrawCount = 0;
				NowTexRes = m_DrawParticleSpriteVec[i].m_TextureRes;
				NowDiffuse = m_DrawParticleSpriteVec[i].m_Diffuse;
			}
			else {
				NowDrawCount++;
			}
		}
		m_InstancVec.push_back(InstanceDrawStr(NowStartIndex, NowDrawCount, NowTexRes, NowDiffuse));


		//インスタンスバッファにマップ
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//行列のマップ
		if (FAILED(pD3D11DeviceContext->Map(m_MatrixBuffer.Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"行列のMapに失敗しました。",
				L"if(FAILED(pD3D11DeviceContext->Map()))",
				L"ParticleDrawObject::OnDraw()"
			);
		}
		//行列の変更
		auto* matrices = (bsm::Mat4x4*)mappedBuffer.pData;
		bsm::Mat4x4 World;
		for (size_t i = 0; i < m_DrawParticleSpriteVec.size(); i++) {
			World = m_DrawParticleSpriteVec[i].m_WorldMatrix;
			//転置する
			World.transpose();
			matrices[i] = World;
		}
		//アンマップ
		pD3D11DeviceContext->Unmap(m_MatrixBuffer.Get(), 0);
		//ストライドとオフセット
		//形状の頂点バッファと行列バッファを設定
		UINT stride[2] = { sizeof(VertexPositionColorTexture), sizeof(bsm::Mat4x4) };
		UINT offset[2] = { 0, 0 };

		ID3D11Buffer* pBuf[2] = { m_MeshResource->GetVertexBuffer().Get(), m_MatrixBuffer.Get() };
		pD3D11DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(m_MeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCTStaticInstance::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCTStaticInstance::GetPtr()->GetInputLayout());

		//ブレンドステート
		if (m_AddType) {
			//加算処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAdditive(), nullptr, 0xffffffff);
		}
		else {
			//透明処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		}
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthRead(), 0);
		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//ラスタライザステート（表面描画）
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//ビュー行列の決定
		Mat4x4 View, Proj;
		PtrGameStage->GetCamera().GetViewProjMatrix(View, Proj);
		//転置する
		View.transpose();
		//転置する
		Proj.transpose();


		//コンスタントバッファの準備
		PCTStaticConstants sb;
		sb.World = bsm::Mat4x4();	//ワールド行列はダミー
		sb.View = View;
		sb.Projection = Proj;
		//エミッシブ加算は行わない。
		sb.Emissive = bsm::Col4(0, 0, 0, 0);

		for (auto& v : m_InstancVec) {
			sb.Diffuse = v.m_Diffuse;
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBPCTStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBPCTStatic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.Tex->GetShaderResourceView().GetAddressOf());
			//描画
			pD3D11DeviceContext->DrawIndexedInstanced(m_MeshResource->GetNumIndicis(), v.Count, 0, 0, v.Start);
		}
		//後始末
		Dev->InitializeStates();
		m_DrawParticleSpriteVec.clear();
		m_InstancVec.clear();
	}



}
//end basecross
