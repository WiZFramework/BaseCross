/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///	四角形スプライトのグループ実体
	//--------------------------------------------------------------------------------------
	SquareSpriteGroup::SquareSpriteGroup() :
		ObjectInterface(),
		ShapeInterface(),
		m_TotalTime(0)
	{}
	SquareSpriteGroup::~SquareSpriteGroup() {}
	void SquareSpriteGroup::OnCreate() {
		float HelfSize = 0.5f;
		//頂点配列
		m_BackupVertices = {
			{ VertexPositionColor(Vector3(-HelfSize, HelfSize, 0), Color4(1.0f, 0.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, HelfSize, 0), Color4(0.0f, 1.0f, 0.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(-HelfSize, -HelfSize, 0), Color4(0.0f, 0.0f, 1.0f, 1.0f)) },
			{ VertexPositionColor(Vector3(HelfSize, -HelfSize, 0), Color4(1.0f, 0.0f, 1.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//メッシュの作成
		//頂点変更できる
		m_SquareSpriteMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);
		//グループの配列の作成
		m_SquareSpriteVec.assign(100, SquareSprite());
		for (auto& v : m_SquareSpriteVec) {
			v.m_LocalRot = Util::RandZeroToOne(true);
			v.m_LocalRotVelocity = Util::RandZeroToOne(true) * 20.0f - 10.0f;
			v.m_LocalPos = Vector2(0, 0);
			v.m_LocalVelocity = Vector2(Util::RandZeroToOne(true) * 200.0f - 100.0f, 100 + Util::RandZeroToOne(true) * 100.0f);
			v.m_LocalGravityVelocity = Vector2(0, 0);
			v.m_DrawContext = ObjectFactory::Create<VSPSDrawContext>(VSPSDrawContext::CreateParam::CreateCbv);
			v.m_DrawContext->CreateConstantBuffer(sizeof(v.m_SpriteConstantBuffer));
			v.m_DrawContext->CreateDefault2DPipelineCmdList<VertexPositionColor, VSPCSprite, PSPCSprite>();
		}
	}

	//各オブジェクトの位置等の変更
	void SquareSpriteGroup::UpdateObjects(float ElapsedTime) {
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		h /= 2.0f;
		for (auto& v : m_SquareSpriteVec) {
			if (v.m_LocalPos.y < -h) {
				v.m_LocalRot = Util::RandZeroToOne(true);
				v.m_LocalRotVelocity = Util::RandZeroToOne(true) * 20.0f - 10.0f;
				v.m_LocalPos = Vector2(0, 0);
				v.m_LocalVelocity = Vector2(Util::RandZeroToOne(true) * 200.0f - 100.0f, 100 + Util::RandZeroToOne(true) * 100.0f);
				v.m_LocalGravityVelocity = Vector2(0, 0);
			}
			else {
				v.m_LocalRot += v.m_LocalRotVelocity * ElapsedTime;
				v.m_LocalPos += v.m_LocalVelocity * ElapsedTime;
				v.m_LocalGravityVelocity += Vector2(0, -98.0f) * ElapsedTime;
				v.m_LocalPos += v.m_LocalGravityVelocity * ElapsedTime;
			}
		}
	}


	void SquareSpriteGroup::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateObjects(ElapsedTime);
	}

	void SquareSpriteGroup::OnDraw() {
		//グループをまとめて描画
		for (auto& v : m_SquareSpriteVec) {
			//行列の定義
			Matrix4X4 World, Proj;
			//ワールド行列の決定
			World.AffineTransformation2D(
				v.m_LocalScale,			//スケーリング
				Vector2(0, 0),		//回転の中心（重心）
				v.m_LocalRot,				//回転角度
				v.m_LocalPos				//位置
			);
			//射影行列の決定
			float w = static_cast<float>(App::GetApp()->GetGameWidth());
			float h = static_cast<float>(App::GetApp()->GetGameHeight());
			Proj.OrthographicLH(w, h, -1.0, 1.0f);
			//行列の合成
			World *= Proj;
			//コンスタントバッファの準備
			v.m_SpriteConstantBuffer.Emissive = Color4(0.0f, 0.0f, 0, 1.0f);
			v.m_SpriteConstantBuffer.World = World;
			//更新
			v.m_DrawContext->UpdateConstantBuffer(&v.m_SpriteConstantBuffer, sizeof(v.m_SpriteConstantBuffer));
			//描画
			v.m_DrawContext->DrawIndexed<VertexPositionColor>(m_SquareSpriteMesh);
		}
	}


}
//end basecross
