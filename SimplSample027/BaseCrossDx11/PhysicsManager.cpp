/*!
@file PhysicsManager.cpp
@brief 物理計算用マネージャなど実体
*/
#include "stdafx.h"
#include "Project.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	///	物理計算用マネージャ
	//--------------------------------------------------------------------------------------
	PhysicsManager::PhysicsManager(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		BasePhysics()
	{
	}
	PhysicsManager::~PhysicsManager() {}

	void PhysicsManager::CreateDrawObject() {
		auto PtrObj = make_shared<SimpleDrawObject>();
		//アフィン変換
		PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		PtrObj->m_Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		PtrObj->m_RasterizerState = RasterizerState::Wireframe;
		m_PtrObjVec.push_back(PtrObj);
	}


	shared_ptr<PhysicsBox> PhysicsManager::AddBox(const PsBoxParam& param, uint16_t index) {
		if (!m_BoxMeshRes) {
			//ワイアフレーム用メッシュの作成（変更できない）
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateCube(2.0f, vertices, indices);
			vector<VertexPositionColor> col_vertices;
			for (auto& v : vertices) {
				VertexPositionColor vertex;
				vertex.position = v.position;
				vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				col_vertices.push_back(vertex);
			}
			m_BoxMeshRes = MeshResource::CreateMeshResource(col_vertices, indices, false);
		}
		if (index >= GetNumBodies()) {
			CreateDrawObject();
		}
		return BasePhysics::AddBox(param, index);

	}

	shared_ptr<PhysicsSphere> PhysicsManager::AddSphere(const PsSphereParam& param, uint16_t index) {
		if (!m_SphereMeshRes) {
			//ワイアフレーム用メッシュの作成（変更できない）
			vector<VertexPositionNormalTexture> vertices;
			vector<uint16_t> indices;
			MeshUtill::CreateSphere(2.0f, 6, vertices, indices);
			vector<VertexPositionColor> col_vertices;
			for (auto& v : vertices) {
				VertexPositionColor vertex;
				vertex.position = v.position;
				vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
				col_vertices.push_back(vertex);
			}
			m_SphereMeshRes = MeshResource::CreateMeshResource(col_vertices, indices, false);
		}
		if (index >= GetNumBodies()) {
			CreateDrawObject();
		}
		return BasePhysics::AddSphere(param, index);
	}


	void PhysicsManager::OnCreate() {
	}


	void PhysicsManager::OnUpdate() {
		BasePhysics::Update();
	}


	void PhysicsManager::DrawShapeWireFrame(uint16_t index, const shared_ptr<MeshResource>& res, const bsm::Mat4x4& world) {
		m_PtrObjVec[index]->m_MeshRes = res;
		m_PtrObjVec[index]->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObjVec[index]->m_WorldMatrix = world;
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<SimplePCStaticRenderer>(L"SimplePCStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObjVec[index]);
	}


	void PhysicsManager::OnDraw() {
		for (auto i = 0; i < GetNumBodies(); i++) {
			//行列の定義
			bsm::Mat4x4 World, Local;
			//bsm::Vec3 Pos;
			//bsm::Quat Qt;
			PsBodyStatus Status;
			GetBodyStatus(i, Status);
			//ワールド行列の決定
			World.affineTransformation(
				bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
				bsm::Vec3(0, 0, 0),		//回転の中心（重心）
				Status.m_Orientation,				//回転角度
				Status.m_Position			//位置
			);
			for (auto j = 0; j <  GetNumShapes(i); j++) {
				bsm::Vec3 LocalPos;
				bsm::Quat LocalQt;
				GetShapeOffsetQuatPos(i, j, LocalQt, LocalPos);
				switch (GetShapeType(i,j)) {
				case sce::PhysicsEffects::ePfxShapeType::kPfxShapeBox:
				{
					auto& shape = getPfxShape(i, j);
					Local.affineTransformation(
						(bsm::Vec3)shape.getBox().m_half,			//スケーリング
						bsm::Vec3(0, 0, 0),		//回転の中心（重心）
						LocalQt,				//回転角度
						LocalPos				//位置
					);
					bsm::Mat4x4 DrawWorld = Local * World;
					DrawShapeWireFrame(i,m_BoxMeshRes, DrawWorld);
				}
				break;
				case sce::PhysicsEffects::ePfxShapeType::kPfxShapeSphere:
				{
					auto& shape = getPfxShape(i, j);
					Local.affineTransformation(
						bsm::Vec3(shape.getSphere().m_radius),			//スケーリング
						bsm::Vec3(0, 0, 0),		//回転の中心（重心）
						LocalQt,				//回転角度
						LocalPos				//位置
					);
					bsm::Mat4x4 DrawWorld = Local * World;
					DrawShapeWireFrame(i,m_SphereMeshRes, DrawWorld);

				}
				break;
				}
			}
		}

	}



}
