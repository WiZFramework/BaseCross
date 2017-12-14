/*!
@file DrawObjects.cpp
@brief 描画オブジェクト実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	シャドウマップの描画クラス
	//--------------------------------------------------------------------------------------
	float ShadowmapRenderer::m_LightHeight(50.0f);
	float ShadowmapRenderer::m_LightNear(1.0f);
	float ShadowmapRenderer::m_LightFar(200.0f);
	float ShadowmapRenderer::m_ViewWidth(32.0f);
	float ShadowmapRenderer::m_ViewHeight(32.0f);

	ShadowmapRenderer::ShadowmapRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		GameObject(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	ShadowmapRenderer::~ShadowmapRenderer() {}

	void ShadowmapRenderer::OnCreate() {
	}

	void ShadowmapRenderer::AddDrawObject(const shared_ptr<ShadowmapObject>& Obj) {
		m_ShadowmapObjectVec.push_back(Obj);
	}

	void ShadowmapRenderer::OnDrawShadowmap() {

		auto PtrStage = GetStage<Stage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//各オブジェクト共通処理
		//ピクセルシェーダはセットしない！
		pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		//ジオメトリシェーダの設定（使用しない）
		pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);

		//個別処理
		for (auto& v : m_ShadowmapObjectVec) {
			//位置の取得
			auto Pos = v->m_WorldMatrix.transInMatrix();
			Vec4 LightDir4;
			PtrStage->GetLightDir(LightDir4);
			Vec3 LightDir(LightDir4.x, LightDir4.y, LightDir4.z);
			Vec3 PosSpan = LightDir;
			PosSpan *= 0.1f;
			Pos += PosSpan;
			//行列の定義
			Mat4x4 World, LightView, LightProj;
			//ワールド行列の決定
			World.affineTransformation(
				v->m_WorldMatrix.scaleInMatrix(),			//スケーリング
				Vec3(0, 0, 0),		//回転の中心（重心）
				v->m_WorldMatrix.quatInMatrix(),				//回転角度
				Pos				//位置
			);
			LightDir = LightDir  * -1.0;
			Vec3 LightAt = v->m_Camera.m_CamerAt;
			Vec3 LightEye = LightAt + (LightDir * m_LightHeight);
			//ライトのビューと射影を計算
			LightView = XMMatrixLookAtLH(LightEye, LightAt, Vec3(0, 1.0f, 0));
			LightProj = XMMatrixOrthographicLH(m_ViewWidth, m_ViewHeight, m_LightNear, m_LightFar);
			ShadowConstants Cb;
			Cb.mWorld = bsm::transpose(World);
			Cb.mView = bsm::transpose(LightView);
			Cb.mProj = bsm::transpose(LightProj);

			bool IsSkin = false;
			bool IsSkinStride = false;
			if (v->m_MeshRes->IsSkining()) {
				if (v->m_pLocalBoneVec) {
					//ボーンの設定
					size_t BoneSz = v->m_pLocalBoneVec->size();
					UINT cb_count = 0;
					for (size_t b = 0; b < BoneSz; b++) {
						bsm::Mat4x4 mat = v->m_pLocalBoneVec->at(b);
						mat.transpose();
						Cb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
						Cb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
						Cb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
						cb_count += 3;
					}
					IsSkin = true;
				}
				IsSkinStride = true;
			}

			if (IsSkin) {
				if (v->m_IsNormalmap) {
					//頂点シェーダーのセット
					pID3D11DeviceContext->VSSetShader(VSShadowmapBoneWithTan::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトのセット
					pID3D11DeviceContext->IASetInputLayout(VSShadowmapBoneWithTan::GetPtr()->GetInputLayout());
				}
				else {
					//頂点シェーダーのセット
					pID3D11DeviceContext->VSSetShader(VSShadowmapBone::GetPtr()->GetShader(), nullptr, 0);
					//インプットレイアウトのセット
					pID3D11DeviceContext->IASetInputLayout(VSShadowmapBone::GetPtr()->GetInputLayout());
				}
			}
			else {
				//頂点シェーダーのセット
				pID3D11DeviceContext->VSSetShader(VSShadowmap::GetPtr()->GetShader(), nullptr, 0);
				//インプットレイアウトのセット
				pID3D11DeviceContext->IASetInputLayout(VSShadowmap::GetPtr()->GetInputLayout());
			}


			//コンスタントバッファの更新
			pID3D11DeviceContext->UpdateSubresource(CBShadow::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
			//ストライドとオフセット
			UINT stride = v->m_MeshRes->GetNumStride();
			UINT offset = 0;
			//描画方法のセット
			pID3D11DeviceContext->IASetPrimitiveTopology(v->m_MeshRes->GetPrimitiveTopology());
			//頂点バッファをセット
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, v->m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pID3D11DeviceContext->IASetIndexBuffer(v->m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBShadow::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//コンスタントバッファをピクセルシェーダにセット
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
			//描画
			pID3D11DeviceContext->DrawIndexed(v->m_MeshRes->GetNumIndicis(), 0, 0);
		}
		//後始末
		Dev->InitializeStates();
		m_ShadowmapObjectVec.clear();
	}

	//--------------------------------------------------------------------------------------
	///	Simple描画オブジェクトの親
	//--------------------------------------------------------------------------------------
	void SimpleRenderer::AddDrawObject(const shared_ptr<SimpleDrawObject>& DrawObj) {
		if (DrawObj->m_OwnShadowmapActive) {
			if (DrawObj->m_BlendState == BlendState::AlphaBlend || DrawObj->m_BlendState == BlendState::Additive) {
				m_ShadowTraceDrawObjectVec.push_back(DrawObj);
			}
			else {
				m_ShadowDrawObjectVec.push_back(DrawObj);
			}

		}
		else {
			if (DrawObj->m_BlendState == BlendState::AlphaBlend || DrawObj->m_BlendState == BlendState::Additive) {
				m_TraceDrawObjectVec.push_back(DrawObj);
			}
			else {
				m_DrawObjectVec.push_back(DrawObj);
			}
		}
	}

	void SimpleRenderer::ClearDrawObject() {
		//描画用の配列をクリア
		m_DrawObjectVec.clear();
		m_TraceDrawObjectVec.clear();
		m_ShadowDrawObjectVec.clear();
		m_ShadowTraceDrawObjectVec.clear();
	}

	void SimpleRenderer::SetConstants(const shared_ptr<SimpleDrawObject>& Obj, SimpleConstants& Cb) {
		auto PtrStage = GetStage<Stage>();
		//マルチライトを得る
		auto& MLight = PtrStage->GetMultiLights();
		//行列の定義
		bsm::Mat4x4 World, ViewMat, ProjMat;
		//ワールド行列の決定
		World = Obj->m_WorldMatrix;
		//転置する
		World.transpose();
		//カメラを得る
		Obj->m_Camera.GetViewProjMatrix(ViewMat, ProjMat);
		Vec4 LightDir;
		PtrStage->GetLightDir(LightDir);
		//転置する
		ViewMat.transpose();
		//転置する
		ProjMat.transpose();
		Cb.World = World;
		Cb.View = ViewMat;
		Cb.Projection = ProjMat;
		//ディフューズ
		Cb.Diffuse = Obj->m_Diffuse;
		//エミッシブ加算。
		Cb.Emissive = Obj->m_Emissive;
		//スペキュラー。
		Cb.Specular = Obj->m_Specular;
		//ライティング
		Cb.LightDir = LightDir;
		Cb.LightDir.w = 1.0f;
		Cb.EyePos = Obj->m_Camera.m_CamerEye;
		Cb.EyePos.w = 1.0f;
		Vec3 CalcLightDir(LightDir.x, LightDir.y, LightDir.z);
		CalcLightDir = -1.0 * CalcLightDir;
		Vec3 LightAt = Obj->m_Camera.m_CamerAt;
		Vec3 LightEye = CalcLightDir;
		LightEye *= ShadowmapRenderer::m_LightHeight;
		LightEye = LightAt + LightEye;
		Cb.LightPos = LightEye;
		Cb.LightPos.w = 1.0f;
		bsm::Mat4x4 LightView, LightProj;
		//ライトのビューと射影を計算
		LightView = XMMatrixLookAtLH(LightEye, LightAt, Vec3(0, 1.0f, 0));
		LightProj = XMMatrixOrthographicLH(ShadowmapRenderer::m_ViewWidth, ShadowmapRenderer::m_ViewHeight,
			ShadowmapRenderer::m_LightNear, ShadowmapRenderer::m_LightFar);
		Cb.LightView = bsm::transpose(LightView);
		Cb.LightProjection = bsm::transpose(LightProj);
		//ボーンの設定
		size_t BoneSz = Obj->m_LocalBonesMatrix.size();
		if (BoneSz > 0) {
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				bsm::Mat4x4 mat = Obj->m_LocalBonesMatrix[b];
				mat.transpose();
				Cb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
				Cb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
				Cb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
				cb_count += 3;
			}
		}
	}


	//--------------------------------------------------------------------------------------
	///	SimplePC頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	SimplePCStaticRenderer::SimplePCStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	SimplePCStaticRenderer::~SimplePCStaticRenderer() {}

	void SimplePCStaticRenderer::OnCreate() {
	}

	void SimplePCStaticRenderer::OnDraw() {
		Draw<VSPCStatic, PSPCStatic>(false);
	}


	//--------------------------------------------------------------------------------------
	///	SimplePCT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	SimplePCTStaticRenderer::SimplePCTStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	SimplePCTStaticRenderer::~SimplePCTStaticRenderer() {}

	void SimplePCTStaticRenderer::OnCreate() {
	}

	void SimplePCTStaticRenderer::OnDraw() {
		Draw<VSPCTStatic, PSPCTStatic>(false);
	}



	//--------------------------------------------------------------------------------------
	///	SimplePNT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	SimplePNTStaticRenderer::SimplePNTStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	SimplePNTStaticRenderer::~SimplePNTStaticRenderer() {}

	void SimplePNTStaticRenderer::OnCreate() {
	}

	void SimplePNTStaticRenderer::OnDraw() {
		Draw<VSPNTStatic, VSPNTStaticShadow, PSPNTStatic, PSPNTStaticShadow>(false);
	}

	//--------------------------------------------------------------------------------------
	///	SimplePNT頂点オブジェクトの描画クラス(シャドウマップを持つ場合)
	//--------------------------------------------------------------------------------------
	SimplePNTStaticRenderer2::SimplePNTStaticRenderer2(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	SimplePNTStaticRenderer2::~SimplePNTStaticRenderer2() {}

	void SimplePNTStaticRenderer2::OnCreate() {
	}

	void SimplePNTStaticRenderer2::OnDraw() {
		Draw<VSPNTStatic, VSPNTStaticShadow, PSPNTStatic, PSPNTStaticShadow2>(false);
	}


	//--------------------------------------------------------------------------------------
	///	SimplePNT頂点モデルオブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	SimplePNTStaticModelRenderer::SimplePNTStaticModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	SimplePNTStaticModelRenderer::~SimplePNTStaticModelRenderer() {}

	void SimplePNTStaticModelRenderer::OnCreate() {
	}

	void SimplePNTStaticModelRenderer::OnDraw() {
		Draw<VSPNTStatic, VSPNTStaticShadow, PSPNTStatic, PSPNTStaticShadow>(true);
	}

	//--------------------------------------------------------------------------------------
	///	SimplePNT頂点モデルオブジェクトの描画クラス(シャドウマップを持つ場合)
	//--------------------------------------------------------------------------------------
	SimplePNTStaticModelRenderer2::SimplePNTStaticModelRenderer2(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	SimplePNTStaticModelRenderer2::~SimplePNTStaticModelRenderer2() {}

	void SimplePNTStaticModelRenderer2::OnCreate() {
	}

	void SimplePNTStaticModelRenderer2::OnDraw() {
		Draw<VSPNTStatic, VSPNTStaticShadow, PSPNTStatic, PSPNTStaticShadow2>(true);
	}

	//--------------------------------------------------------------------------------------
	///	SimplePNT頂点ボーンモデルオブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	SimplePNTBoneModelRenderer::SimplePNTBoneModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	SimplePNTBoneModelRenderer::~SimplePNTBoneModelRenderer() {}

	void SimplePNTBoneModelRenderer::OnCreate() {
	}

	void SimplePNTBoneModelRenderer::OnDraw() {
		Draw<VSPNTBone, VSPNTBoneShadow, PSPNTStatic, PSPNTStaticShadow>(true);
	}

	//--------------------------------------------------------------------------------------
	///	SimplePNT頂点ボーンモデルオブジェクトの描画クラス(シャドウマップを持つ場合)
	//--------------------------------------------------------------------------------------
	SimplePNTBoneModelRenderer2::SimplePNTBoneModelRenderer2(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		SimpleRenderer(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	SimplePNTBoneModelRenderer2::~SimplePNTBoneModelRenderer2() {}

	void SimplePNTBoneModelRenderer2::OnCreate() {
	}

	void SimplePNTBoneModelRenderer2::OnDraw() {
		Draw<VSPNTBone, VSPNTBoneShadow, PSPNTStatic, PSPNTStaticShadow2>(true);
	}




	//--------------------------------------------------------------------------------------
	///	Bc描画オブジェクトの親
	//--------------------------------------------------------------------------------------
	void BcRenderer::AddDrawObject(const shared_ptr<BcDrawObject>& DrawObj) {
		if (DrawObj->m_OwnShadowmapActive) {
			if (DrawObj->m_BlendState == BlendState::AlphaBlend || DrawObj->m_BlendState == BlendState::Additive) {
				m_ShadowTraceDrawObjectVec.push_back(DrawObj);
			}
			else {
				m_ShadowDrawObjectVec.push_back(DrawObj);
			}

		}
		else {
			if (DrawObj->m_BlendState == BlendState::AlphaBlend || DrawObj->m_BlendState == BlendState::Additive) {
				m_TraceDrawObjectVec.push_back(DrawObj);
			}
			else {
				m_DrawObjectVec.push_back(DrawObj);
			}
		}
	}

	void BcRenderer::ClearDrawObject() {
		//描画用の配列をクリア
		m_DrawObjectVec.clear();
		m_TraceDrawObjectVec.clear();
		m_ShadowDrawObjectVec.clear();
		m_ShadowTraceDrawObjectVec.clear();
	}

	void BcRenderer::SetConstants(const shared_ptr<BcDrawObject>& Obj, BasicConstants& BcCb) {
		auto PtrStage = GetStage<Stage>();
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//マルチライトを得る
		auto& MLight = PtrStage->GetMultiLights();
		//ワールド行列
		Mat4x4 world = Obj->m_WorldMatrix;
		//ビュート射影行列の定義
		Mat4x4 view, proj;
		Obj->m_Camera.GetViewProjMatrix(view, proj);
		//行列の設定
		auto worldView = world * view;
		BcCb.worldViewProj = XMMatrixTranspose(XMMatrixMultiply(worldView, proj));

		//フォグの設定
		if (Obj->m_FogEnabled)
		{
			auto start = Obj->m_FogStart;
			auto end = Obj->m_FogEnd;
			if (start == end)
			{
				// Degenerate case: force everything to 100% fogged if start and end are the same.
				static const XMVECTORF32 fullyFogged = { 0, 0, 0, 1 };
				BcCb.fogVector = fullyFogged;
			}
			else
			{
				XMMATRIX worldViewTrans = worldView;
				// _13, _23, _33, _43
				XMVECTOR worldViewZ = XMVectorMergeXY(XMVectorMergeZW(worldViewTrans.r[0], worldViewTrans.r[2]),
					XMVectorMergeZW(worldViewTrans.r[1], worldViewTrans.r[3]));
				XMVECTOR wOffset = XMVectorSwizzle<1, 2, 3, 0>(XMLoadFloat(&start));
				BcCb.fogVector = (worldViewZ + wOffset) / (start - end);
			}
			BcCb.fogColor = Obj->m_FogColor;
		}
		else
		{
			BcCb.fogVector = g_XMZero;
			BcCb.fogColor = g_XMZero;

		}
		//ライトの設定
		if (Obj->m_LightingEnabled)
		{
			for (int i = 0; i < 3; i++) {
				BcCb.lightDirection[i] = MLight.m_Directional[i];
				BcCb.lightDiffuseColor[i] = MLight.m_DiffuseColor[i];
				BcCb.lightSpecularColor[i] = MLight.m_SpecularColor[i];
			}
			BcCb.world = XMMatrixTranspose(world);
			XMMATRIX worldInverse = XMMatrixInverse(nullptr, world);
			BcCb.worldInverseTranspose[0] = worldInverse.r[0];
			BcCb.worldInverseTranspose[1] = worldInverse.r[1];
			BcCb.worldInverseTranspose[2] = worldInverse.r[2];
			XMMATRIX viewInverse = XMMatrixInverse(nullptr, view);
			BcCb.eyePosition = viewInverse.r[3];
		}
		XMVECTOR diffuse = Obj->m_Diffuse;
		XMVECTOR alphaVector = XMVectorReplicate(Obj->m_Alpha);
		XMVECTOR emissiveColor = Obj->m_Emissive;
		XMVECTOR ambientLightColor = MLight.m_AmbientLightColor;
		if (Obj->m_LightingEnabled)
		{
			BcCb.activeFlg.x = 3;
			// Merge emissive and ambient light contributions.
			BcCb.emissiveColor = (emissiveColor + ambientLightColor * diffuse) * alphaVector;
			BcCb.specularColorAndPower = Obj->m_SpecularColorAndPower;
		}
		else
		{
			BcCb.activeFlg.x = 0;
			// Merge diffuse and emissive light contributions.
			diffuse += emissiveColor;
			BcCb.specularColorAndPower = Obj->m_SpecularColorAndPower;
		}
		// xyz = diffuse * alpha, w = alpha.
		BcCb.diffuseColor = XMVectorSelect(alphaVector, diffuse * alphaVector, g_XMSelect1110);

		//影用
		if (Obj->m_OwnShadowmapActive) {
			SingleLight Light;
			MLight.GetMainLight(Light);
			bsm::Vec3 CalcLightDir(Light.m_Directional * -1.0);
			bsm::Vec3 LightAt(Obj->m_Camera.m_CamerAt);
			bsm::Vec3 LightEye(CalcLightDir);
			LightEye *= ShadowmapRenderer::m_LightHeight;
			LightEye = LightAt + LightEye;
			bsm::Vec4 LightEye4(LightEye, 1.0f);
			LightEye4.w = 1.0f;
			BcCb.lightPos = LightEye4;
			bsm::Vec4 eyePos4(Obj->m_Camera.m_CamerEye, 1.0f);
			eyePos4.w = 1.0f;
			BcCb.eyePos = eyePos4;
			bsm::Mat4x4 LightView, LightProj;
			//ライトのビューと射影を計算
			LightView = XMMatrixLookAtLH(LightEye, LightAt, bsm::Vec3(0, 1.0f, 0));
			LightProj = XMMatrixOrthographicLH(ShadowmapRenderer::m_ViewWidth, ShadowmapRenderer::m_ViewHeight,
				ShadowmapRenderer::m_LightNear, ShadowmapRenderer::m_LightFar);
			BcCb.lightView = bsm::transpose(LightView);
			BcCb.lightProjection = bsm::transpose(LightProj);
		}

		//ボーンの設定
		size_t BoneSz = Obj->m_LocalBonesMatrix.size();
		if (BoneSz > 0) {
			UINT cb_count = 0;
			for (size_t b = 0; b < BoneSz; b++) {
				bsm::Mat4x4 mat = Obj->m_LocalBonesMatrix[b];
				mat.transpose();
				BcCb.bones[cb_count] = ((XMMATRIX)mat).r[0];
				BcCb.bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
				BcCb.bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
				cb_count += 3;
			}
		}
	}

	void BcRenderer::ConvertToNormalVertex(
		const vector<VertexPositionNormalTexture>& vertices,
		vector<VertexPositionNormalTangentTexture>& new_vertices
	) {
		new_vertices.clear();
		for (size_t i = 0; i < vertices.size(); i++) {
			VertexPositionNormalTangentTexture new_pntnt_v;

			new_pntnt_v.position = vertices[i].position;
			new_pntnt_v.normal = vertices[i].normal;
			new_pntnt_v.textureCoordinate = vertices[i].textureCoordinate;
			Vec3 n = cross((Vec3)new_pntnt_v.normal, Vec3(0, 1, 0));
			new_pntnt_v.tangent = Vec4(n.x, n.y, n.z, 0.0f);
			new_pntnt_v.tangent.w = 0.0f;
			new_vertices.push_back(new_pntnt_v);
		}
		MeshUtill::SetNormalTangent(new_vertices);
	}


	//--------------------------------------------------------------------------------------
	///	BcPNT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	BcPNTStaticRenderer::BcPNTStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		BcRenderer(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	BcPNTStaticRenderer::~BcPNTStaticRenderer() {}

	void BcPNTStaticRenderer::OnCreate() {
	}

	void BcPNTStaticRenderer::OnDraw() {
		Draw<BcVSPNTStaticPL, BcVSPNTStaticPLShadow, BcPSPNTPL, BcPSPNTPLShadow>(false);
	}

	//--------------------------------------------------------------------------------------
	///	BcPNTnT頂点オブジェクト(法線マップ使用)の描画クラス
	//--------------------------------------------------------------------------------------
	BcPNTnTStaticRenderer::BcPNTnTStaticRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		BcRenderer(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	BcPNTnTStaticRenderer::~BcPNTnTStaticRenderer() {}

	void BcPNTnTStaticRenderer::OnCreate() {
	}

	void BcPNTnTStaticRenderer::OnDraw() {
		Draw<BcVSPNTnTStaticPL, BcVSPNTnTStaticPLShadow, BcPSPNTnTPL, BcPSPNTnTPLShadow>(false);
	}

	//--------------------------------------------------------------------------------------
	///	BcPNT頂点モデルオブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	BcPNTStaticModelRenderer::BcPNTStaticModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		BcRenderer(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	BcPNTStaticModelRenderer::~BcPNTStaticModelRenderer() {}

	void BcPNTStaticModelRenderer::OnCreate() {
	}

	void BcPNTStaticModelRenderer::OnDraw() {
		Draw<BcVSPNTStaticPL, BcVSPNTStaticPLShadow, BcPSPNTPL, BcPSPNTPLShadow>(true);
	}

	//--------------------------------------------------------------------------------------
	///	BcPNTnT頂点モデルオブジェクトの描画クラス(法線マップ付きモデル)
	//--------------------------------------------------------------------------------------
	BcPNTnTStaticModelRenderer::BcPNTnTStaticModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		BcRenderer(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	BcPNTnTStaticModelRenderer::~BcPNTnTStaticModelRenderer() {}

	void BcPNTnTStaticModelRenderer::OnCreate() {
	}

	void BcPNTnTStaticModelRenderer::OnDraw() {
		Draw<BcVSPNTnTStaticPL, BcVSPNTnTStaticPLShadow, BcPSPNTnTPL, BcPSPNTnTPLShadow>(true);
	}

	//--------------------------------------------------------------------------------------
	///	BcPNT頂点ボーンモデルオブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	BcPNTBoneModelRenderer::BcPNTBoneModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		BcRenderer(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	BcPNTBoneModelRenderer::~BcPNTBoneModelRenderer() {}

	void BcPNTBoneModelRenderer::OnCreate() {
	}

	void BcPNTBoneModelRenderer::OnDraw() {
		Draw<BcVSPNTBonePL, BcVSPNTBonePLShadow, BcPSPNTPL, BcPSPNTPLShadow>(true);
	}

	//--------------------------------------------------------------------------------------
	///	BcPNTnT頂点ボーンモデルオブジェクトの描画クラス(法線マップ付きモデル)
	//--------------------------------------------------------------------------------------
	BcPNTnTBoneModelRenderer::BcPNTnTBoneModelRenderer(const shared_ptr<Stage>& StagePtr, const wstring& Tagname) :
		BcRenderer(StagePtr)
	{
		//タグの追加
		AddTag(Tagname);
	}
	BcPNTnTBoneModelRenderer::~BcPNTnTBoneModelRenderer() {}

	void BcPNTnTBoneModelRenderer::OnCreate() {
	}

	void BcPNTnTBoneModelRenderer::OnDraw() {
		Draw<BcVSPNTnTBonePL, BcVSPNTnTBonePLShadow, BcPSPNTnTPL, BcPSPNTnTPLShadow>(true);
	}




}
//end basecross
