/*!
@file PhysicsManager.cpp
@brief 物理計算用マネージャなど実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	///	物理計算コンポーネントの親の親
	//--------------------------------------------------------------------------------------
	PsBodyComponentBase::PsBodyComponentBase(const shared_ptr<GameObject>& GameObjectPtr):
		Component(GameObjectPtr),
		m_AutoTransform(true)
	{
		SetDrawActive(false);
	}
	PsBodyComponentBase::~PsBodyComponentBase() {}

	void PsBodyComponentBase::DrawShapeWireFrame(const shared_ptr<MeshResource>& res, const bsm::Mat4x4& world) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		bsm::Mat4x4 World, ViewMat, ProjMat;
		World = world;
		//転置する
		World.transpose();
		//カメラを得る
		auto CameraPtr = GetGameObject()->OnGetDrawCamera();
		//ビューと射影行列を得る
		ViewMat = CameraPtr->GetViewMatrix();
		//転置する
		ViewMat.transpose();
		//転置する
		ProjMat = CameraPtr->GetProjMatrix();
		ProjMat.transpose();
		//コンスタントバッファの準備
		SimpleConstants sb;
		sb.World = World;
		sb.View = ViewMat;
		sb.Projection = ProjMat;
		//エミッシブ
		sb.Emissive = Col4(0, 0, 0, 0);
		//デフィーズはすべて通す
		sb.Diffuse = Col4(1, 1, 1, 1);
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, res->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(res->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCStatic::GetPtr()->GetInputLayout());
		//ブレンドステート
		//透明処理しない
		pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//ラスタライザステート(ワイアフレーム)
		pD3D11DeviceContext->RSSetState(RenderState->GetWireframe());
		pD3D11DeviceContext->DrawIndexed(res->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();

	}

	shared_ptr<MeshResource> PsBodyComponentBase::CreateCapsuleMesh(const PsCapsuleParam& param) {
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionColor> new_pc_vertices;
		vector<uint16_t> indices;
		bsm::Vec3 PointA(0, 0, 0);
		bsm::Vec3 PointB(0, 0, 0);
		PointA -= bsm::Vec3(0, param.m_HalfLen, 0);
		PointB += bsm::Vec3(0, param.m_HalfLen, 0);
		MeshUtill::CreateCapsule(param.m_Radius * 2.0f,
			PointA, PointB, 6, vertices, indices, true);
		for (auto& v : vertices) {
			VertexPositionColor vertex;
			vertex.position = v.position;
			vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			new_pc_vertices.push_back(vertex);
		}
		return MeshResource::CreateMeshResource(new_pc_vertices, indices, false);
	}

	shared_ptr<MeshResource> PsBodyComponentBase::CreateCylinderMesh(const PsCylinderParam& param) {
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionColor> new_pc_vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCylinder(param.m_HalfLen * 2.0f, param.m_Radius * 2.0f, 6, vertices, indices, true);
		for (auto& v : vertices) {
			VertexPositionColor vertex;
			vertex.position = v.position;
			vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			new_pc_vertices.push_back(vertex);
		}
		return MeshResource::CreateMeshResource(new_pc_vertices, indices, false);
	}

	shared_ptr<MeshResource> PsBodyComponentBase::CreateConvexMesh(const PsConvexParam& param) {
		vector<VertexPositionColor> new_pc_vertices;
		for (auto& v : param.m_ConvexMeshResource->GetVertices()) {
			VertexPositionColor vertex;
			vertex.position = v.position;
			vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			new_pc_vertices.push_back(vertex);
		}
		return MeshResource::CreateMeshResource(new_pc_vertices, param.m_ConvexMeshResource->GetIndices(), false);
	}





	//--------------------------------------------------------------------------------------
	///	物理計算コンポーネントの親
	//--------------------------------------------------------------------------------------
	PsBodyComponent::PsBodyComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		PsBodyComponentBase(GameObjectPtr)
	{}
	PsBodyComponent::~PsBodyComponent() {}

	bsm::Vec3	PsBodyComponent::GetPosition() const {
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyPosition(GetIndex());
	}

	bsm::Quat	PsBodyComponent::GetOrientation() const {
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyOrientation(GetIndex());
	}

	bsm::Vec3	PsBodyComponent::GetLinearVelocity() const {
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyLinearVelocity(GetIndex());
	}

	bsm::Vec3	PsBodyComponent::GetAngularVelocity() const {
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyAngularVelocity(GetIndex());
	}

	void PsBodyComponent::GetStatus(PsBodyStatus& st) const {
		GetGameObject()->GetStage()->GetBasePhysics().GetBodyStatus(GetIndex(), st);
	}

	void PsBodyComponent::SetStatus(const PsBodyUpdateStatus& st) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyStatus(GetIndex(), st);
	}

	void PsBodyComponent::WakeUp() {
		GetGameObject()->GetStage()->GetBasePhysics().WakeUpBody(GetIndex());
	}

	bool PsBodyComponent::IsAutoGravity()const {
		return GetGameObject()->GetStage()->GetBasePhysics().IsAutoGravity(GetIndex());
	}

	void PsBodyComponent::SetAutoGravity(bool b) {
		GetGameObject()->GetStage()->GetBasePhysics().SetAutoGravity(GetIndex(),b);
	}


	void PsBodyComponent::SetPosition(const bsm::Vec3& pos) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyPosition(GetIndex(), pos);
	}

	void PsBodyComponent::SetOrientation(const bsm::Quat& qt) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyOrientation(GetIndex(), qt);
	}

	void PsBodyComponent::SetLinearVelocity(const bsm::Vec3& v) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyLinearVelocity(GetIndex(), v);

	}
	void PsBodyComponent::SetAngularVelocity(const bsm::Vec3& v) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyAngularVelocity(GetIndex(), v);
	}

	void PsBodyComponent::MovePosition(const bsm::Vec3 &pos, float timeStep) {
		GetGameObject()->GetStage()->GetBasePhysics().MoveBodyPosition(GetIndex(), pos, timeStep);
	}
	void PsBodyComponent::MoveOrientation(const bsm::Quat& qt, float timeStep) {
		GetGameObject()->GetStage()->GetBasePhysics().MoveBodyOrientation(GetIndex(), qt, timeStep);
	}


	void PsBodyComponent::ApplyForce(const bsm::Vec3& v) {
		GetGameObject()->GetStage()->GetBasePhysics().ApplyBodyForce(GetIndex(), v);

	}
	void PsBodyComponent::ApplyTorque(const bsm::Vec3& v) {
		GetGameObject()->GetStage()->GetBasePhysics().ApplyBodyTorque(GetIndex(), v);
	}

	void PsBodyComponent::SetContactFilterSelf(uint32_t val) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyContactFilterSelf(GetIndex(), val);
	}
	void PsBodyComponent::SetContactFilterTarget(uint32_t val) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyContactFilterTarget(GetIndex(), val);
	}


	bool PsBodyComponent::GetContactsVec(vector<uint16_t>& contacts)const {
		return GetGameObject()->GetStage()->GetBasePhysics().GetContactsVec(GetIndex(), contacts);
	}
	bool PsBodyComponent::GetContactsSet(set<uint16_t>& contacts)const {
		return GetGameObject()->GetStage()->GetBasePhysics().GetContactsSet(GetIndex(), contacts);

	}


	bool PsBodyComponent::GetContactGameObjects(vector<shared_ptr<GameObject>>& objects)const {
		set<uint16_t> indeces;
		if (GetContactsSet(indeces)) {
			auto& ObjVec = GetGameObject()->GetStage()->GetGameObjectVec();
			for (auto& v : ObjVec) {
				auto Comp = v->GetDynamicComponent<PsBodyComponent>(false);
				if (Comp) {
					auto index = Comp->GetIndex();
					if (indeces.find(index) != indeces.end()) {
						objects.push_back(v);
					}
				}
			}
		}
		if (objects.size() > 0) {
			return true;
		}
		else {
			return false;
		}
	}



	void PsBodyComponent::OnUpdate() {
		if (IsAutoTransform()) {
			PsBodyStatus Status;
			GetGameObject()->GetStage()->GetBasePhysics().GetBodyStatus(GetIndex(), Status);
			auto Ptr = GetGameObject()->GetComponent<Transform>();
			Ptr->SetPosition(Status.m_Position);
			Ptr->SetQuaternion(Status.m_Orientation);
		}
	}




	//--------------------------------------------------------------------------------------
	///	単体の球体コンポーネント
	//--------------------------------------------------------------------------------------
	PsSphereBody::PsSphereBody(const shared_ptr<GameObject>& GameObjectPtr, const PsSphereParam& param):
		PsBodyComponent(GameObjectPtr)
	{
		m_PsSphere = GameObjectPtr->GetStage()->GetBasePhysics().AddSphere(param);
	}

	uint16_t PsSphereBody::GetIndex() const {
		return m_PsSphere->GetIndex();
	}

	const PsSphereParam& PsSphereBody::GetParam() const {
		return m_PsSphere->GetParam();
	}


	void PsSphereBody::Reset(const PsSphereParam& param, uint16_t index) {
		m_PsSphere = GetGameObject()->GetStage()->GetBasePhysics().AddSphere(param, index);
	}

	bool PsSphereBody::CollisionTestBase(const SPHERE& src) {
		SPHERE dest;
		auto& param = m_PsSphere->GetParam();
		dest.m_Radius = param.m_Radius;
		dest.m_Center = GetPosition();
		return HitTest::SPHERE_SPHERE(src, dest);
	}

	void PsSphereBody::OnDraw() {
		auto index = GetIndex();
		//行列の定義
		bsm::Mat4x4 World, Local;
		PsBodyStatus Status;
		auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
		auto MeshRes = App::GetApp()->GetResource<MeshResource>(L"PSWIRE_PC_SPHERE");
		BasePs.GetBodyStatus(index, Status);
		//ワールド行列の決定
		World.affineTransformation(
			bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			Status.m_Orientation,				//回転角度
			Status.m_Position			//位置
		);
		bsm::Vec3 LocalPos;
		bsm::Quat LocalQt;
		BasePs.GetShapeOffsetQuatPos(index, 0, LocalQt, LocalPos);
		Local.affineTransformation(
			bsm::Vec3(m_PsSphere->GetParam().m_Radius),			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			LocalQt,				//回転角度
			LocalPos				//位置
		);
		bsm::Mat4x4 DrawWorld = Local * World;
		DrawShapeWireFrame(MeshRes, DrawWorld);
	}

	//--------------------------------------------------------------------------------------
	///	単体のボックスコンポーネント
	//--------------------------------------------------------------------------------------
	PsBoxBody::PsBoxBody(const shared_ptr<GameObject>& GameObjectPtr, const PsBoxParam& param) :
		PsBodyComponent(GameObjectPtr)
	{
		m_PsBox = GameObjectPtr->GetStage()->GetBasePhysics().AddBox(param);
	}

	uint16_t PsBoxBody::GetIndex() const {
		return m_PsBox->GetIndex();
	}

	const PsBoxParam& PsBoxBody::GetParam() const {
		return m_PsBox->GetParam();
	}

	void PsBoxBody::Reset(const PsBoxParam& param, uint16_t index) {
		m_PsBox = GetGameObject()->GetStage()->GetBasePhysics().AddBox(param, index);
	}


	bool PsBoxBody::CollisionTestBase(const SPHERE& src) {
		OBB dest(m_PsBox->GetParam().m_HalfSize * 2,
			GetOrientation(), GetPosition());
		bsm::Vec3 ret;
		return HitTest::SPHERE_OBB(src, dest, ret);
	}

	void PsBoxBody::OnDraw() {
		auto index = GetIndex();
		//行列の定義
		bsm::Mat4x4 World, Local;
		PsBodyStatus Status;
		auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
		auto MeshRes = App::GetApp()->GetResource<MeshResource>(L"PSWIRE_PC_CUBE");
		BasePs.GetBodyStatus(index, Status);
		//ワールド行列の決定
		World.affineTransformation(
			bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			Status.m_Orientation,				//回転角度
			Status.m_Position			//位置
		);
		bsm::Vec3 LocalPos;
		bsm::Quat LocalQt;
		BasePs.GetShapeOffsetQuatPos(index, 0, LocalQt, LocalPos);
		Local.affineTransformation(
			m_PsBox->GetParam().m_HalfSize,			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			LocalQt,				//回転角度
			LocalPos				//位置
		);
		bsm::Mat4x4 DrawWorld = Local * World;
		DrawShapeWireFrame(MeshRes, DrawWorld);
	}

	//--------------------------------------------------------------------------------------
	///	単体のカプセルコンポーネント
	//--------------------------------------------------------------------------------------
	PsCapsuleBody::PsCapsuleBody(const shared_ptr<GameObject>& GameObjectPtr, const PsCapsuleParam& param):
		PsBodyComponent(GameObjectPtr)
	{
		m_PsCapsule = GetGameObject()->GetStage()->GetBasePhysics().AddCapsule(param);
		m_CapsuleMesh = PsBodyComponentBase::CreateCapsuleMesh(param);
	}


	uint16_t PsCapsuleBody::GetIndex() const {
		return m_PsCapsule->GetIndex();
	}

	const PsCapsuleParam& PsCapsuleBody::GetParam() const {
		return m_PsCapsule->GetParam();
	}


	void PsCapsuleBody::Reset(const PsCapsuleParam& param, uint16_t index) {
		m_PsCapsule = GetGameObject()->GetStage()->GetBasePhysics().AddCapsule(param, index);
		m_CapsuleMesh = PsBodyComponentBase::CreateCapsuleMesh(param);
	}

	bool PsCapsuleBody::CollisionTestBase(const SPHERE& src) {
		//ワールド行列の決定
		Mat4x4 World;
		World.affineTransformation(
			bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			GetOrientation(),				//回転角度
			GetPosition()			//位置
		);
		CAPSULE dest(m_PsCapsule->GetParam().m_Radius,
			Vec3(0, -m_PsCapsule->GetParam().m_HalfLen, 0),
			Vec3(0, m_PsCapsule->GetParam().m_HalfLen, 0),
			World);
		bsm::Vec3 ret;
		return HitTest::SPHERE_CAPSULE(src, dest, ret);
	}



	void PsCapsuleBody::OnDraw() {
		auto index = GetIndex();
		//行列の定義
		bsm::Mat4x4 World, Local;
		PsBodyStatus Status;
		auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
		auto MeshRes = m_CapsuleMesh;
		BasePs.GetBodyStatus(index, Status);
		//ワールド行列の決定
		World.affineTransformation(
			bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			Status.m_Orientation,				//回転角度
			Status.m_Position			//位置
		);
		bsm::Vec3 LocalPos;
		bsm::Quat LocalQt;
		BasePs.GetShapeOffsetQuatPos(index, 0, LocalQt, LocalPos);

		Local.affineTransformation(
			bsm::Vec3(1.0f),			//スケーリングは1.0f
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			LocalQt,				//回転角度
			LocalPos				//位置
		);
		bsm::Mat4x4 DrawWorld = Local * World;
		DrawShapeWireFrame(MeshRes, DrawWorld);

	}

	//--------------------------------------------------------------------------------------
	///	単体のシリンダーコンポーネント
	//--------------------------------------------------------------------------------------
	PsCylinderBody::PsCylinderBody(const shared_ptr<GameObject>& GameObjectPtr, const PsCylinderParam& param):
		PsBodyComponent(GameObjectPtr)
	{
		m_PsCylinder = GetGameObject()->GetStage()->GetBasePhysics().AddCylinder(param);
		m_CylinderMesh = PsBodyComponentBase::CreateCylinderMesh(param);
	}


	uint16_t PsCylinderBody::GetIndex() const {
		return m_PsCylinder->GetIndex();
	}

	const PsCylinderParam& PsCylinderBody::GetParam() const {
		return m_PsCylinder->GetParam();
	}


	void PsCylinderBody::Reset(const PsCylinderParam& param, uint16_t index) {
		m_PsCylinder = GetGameObject()->GetStage()->GetBasePhysics().AddCylinder(param, index);
		m_CylinderMesh = PsBodyComponentBase::CreateCylinderMesh(param);
	}

	void PsCylinderBody::OnDraw() {
		auto index = GetIndex();
		//行列の定義
		bsm::Mat4x4 World, Local;
		PsBodyStatus Status;
		auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
		auto MeshRes = m_CylinderMesh;
		BasePs.GetBodyStatus(index, Status);
		//ワールド行列の決定
		World.affineTransformation(
			bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			Status.m_Orientation,				//回転角度
			Status.m_Position			//位置
		);
		bsm::Vec3 LocalPos;
		bsm::Quat LocalQt;
		BasePs.GetShapeOffsetQuatPos(index, 0, LocalQt, LocalPos);
		Local.affineTransformation(
			bsm::Vec3(1.0f),			//スケーリングは1.0f
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			LocalQt,				//回転角度
			LocalPos				//位置
		);
		bsm::Mat4x4 DrawWorld = Local * World;
		DrawShapeWireFrame(MeshRes, DrawWorld);
	}

	//--------------------------------------------------------------------------------------
	///	単体の凸面コンポーネント
	//--------------------------------------------------------------------------------------
	PsConvexBody::PsConvexBody(const shared_ptr<GameObject>& GameObjectPtr, const PsConvexParam& param):
		PsBodyComponent(GameObjectPtr)
	{
		m_PsConvex = GetGameObject()->GetStage()->GetBasePhysics().AddConvex(param);
		m_ConvexMesh = PsBodyComponentBase::CreateConvexMesh(param);
	}

	uint16_t PsConvexBody::GetIndex() const {
		return m_PsConvex->GetIndex();
	}

	const PsConvexParam& PsConvexBody::GetParam() const {
		return m_PsConvex->GetParam();
	}


	void PsConvexBody::Reset(const PsConvexParam& param, uint16_t index) {
		m_PsConvex = GetGameObject()->GetStage()->GetBasePhysics().AddConvex(param, index);
		m_ConvexMesh = PsBodyComponentBase::CreateConvexMesh(param);
	}

	void PsConvexBody::OnDraw() {
		auto index = GetIndex();
		//行列の定義
		bsm::Mat4x4 World, Local;
		PsBodyStatus Status;
		auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
		auto MeshRes = m_ConvexMesh;
		BasePs.GetBodyStatus(index, Status);
		//ワールド行列の決定
		World.affineTransformation(
			bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			Status.m_Orientation,				//回転角度
			Status.m_Position			//位置
		);
		bsm::Vec3 LocalPos;
		bsm::Quat LocalQt;
		BasePs.GetShapeOffsetQuatPos(index, 0, LocalQt, LocalPos);

		Local.affineTransformation(
			bsm::Vec3(1.0f),			//スケーリングは1.0f
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			LocalQt,				//回転角度
			LocalPos				//位置
		);
		bsm::Mat4x4 DrawWorld = Local * World;
		DrawShapeWireFrame(MeshRes, DrawWorld);

	}

	//--------------------------------------------------------------------------------------
	///	プリミティブ合成コンポーネント
	//--------------------------------------------------------------------------------------
	PsCombinedBody::PsCombinedBody(const shared_ptr<GameObject>& GameObjectPtr, const PsCombinedParam& param):
		PsBodyComponent(GameObjectPtr)
	{
		m_PsCombined = GetGameObject()->GetStage()->GetBasePhysics().AddCombined(param);
		CreateMesh(param);
	}

	void PsCombinedBody::CreateMesh(const PsCombinedParam& param) {
		m_CombinedMeshVec.clear();
		for (auto& Prim : param.m_Primitives){
			vector<VertexPositionNormalTexture> vertices;
			vector<VertexPositionColor> new_pc_vertices;
			vector<uint16_t> indices;
			switch (Prim.m_CombinedType) {
				case PsCombinedType::TypeSphere:
				{
					m_CombinedMeshVec.push_back(App::GetApp()->GetResource<MeshResource>(L"PSWIRE_PC_SPHERE"));
				}
				break;
				case PsCombinedType::TypeBox:
				{
					m_CombinedMeshVec.push_back(App::GetApp()->GetResource<MeshResource>(L"PSWIRE_PC_CUBE"));
				}
				break;
				case PsCombinedType::TypeCapsule:
				{
					bsm::Vec3 PointA(0, 0, 0);
					bsm::Vec3 PointB(0, 0, 0);
					PointA -= bsm::Vec3(0, Prim.m_HalfLen, 0);
					PointB += bsm::Vec3(0, Prim.m_HalfLen, 0);
					MeshUtill::CreateCapsule(Prim.m_Radius * 2.0f,
						PointA, PointB, 6, vertices, indices, true);
					for (auto& v : vertices) {
						VertexPositionColor vertex;
						vertex.position = v.position;
						vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
						new_pc_vertices.push_back(vertex);
					}
					m_CombinedMeshVec.push_back(MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
				}
				break;
				case PsCombinedType::TypeCylinder:
				{
					MeshUtill::CreateCylinder(Prim.m_HalfLen * 2.0f, Prim.m_Radius * 2.0f, 6, vertices, indices, true);
					for (auto& v : vertices) {
						VertexPositionColor vertex;
						vertex.position = v.position;
						vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
						new_pc_vertices.push_back(vertex);
					}
					m_CombinedMeshVec.push_back(MeshResource::CreateMeshResource(new_pc_vertices, indices, false));
				}
				break;
				case PsCombinedType::TypeConvex:
				{
					for (auto& v : Prim.m_ConvexMeshResource->GetVertices()) {
						VertexPositionColor vertex;
						vertex.position = v.position;
						vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
						new_pc_vertices.push_back(vertex);
					}
					m_CombinedMeshVec.push_back(MeshResource::CreateMeshResource(new_pc_vertices, Prim.m_ConvexMeshResource->GetIndices(), false));
				}
				break;
			}
		}

	}

	uint16_t PsCombinedBody::GetIndex() const {
		return m_PsCombined->GetIndex();
	}

	const PsCombinedParam& PsCombinedBody::GetParam() const {
		return m_PsCombined->GetParam();
	}


	void PsCombinedBody::Reset(const PsCombinedParam& param, uint16_t index) {
		m_PsCombined = GetGameObject()->GetStage()->GetBasePhysics().AddCombined(param, index);
		CreateMesh(param);
	}

	void PsCombinedBody::OnDraw() {
		auto index = GetIndex();
		//行列の定義
		bsm::Mat4x4 World, Local;
		PsBodyStatus Status;
		auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
		BasePs.GetBodyStatus(index, Status);
		//ワールド行列の決定
		World.affineTransformation(
			bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			Status.m_Orientation,				//回転角度
			Status.m_Position			//位置
		);

		auto& param = m_PsCombined->GetParam();
		for (size_t i = 0; i < param.m_Primitives.size(); i++) {
			auto& prim = param.m_Primitives[i];
			bsm::Vec3 LocalPos;
			bsm::Quat LocalQt;
			BasePs.GetShapeOffsetQuatPos(index, (uint16_t)i, LocalQt, LocalPos);
			switch (prim.m_CombinedType) {
				case PsCombinedType::TypeSphere:
				{
					Local.affineTransformation(
						Vec3(prim.m_Radius),	//スケーリング
						bsm::Vec3(0, 0, 0),		//回転の中心（重心）
						LocalQt,				//回転角度
						LocalPos				//位置
					);
				}
				break;
				case PsCombinedType::TypeBox:
				{
					Local.affineTransformation(
						prim.m_HalfSize,			//スケーリング
						bsm::Vec3(0, 0, 0),		//回転の中心（重心）
						LocalQt,				//回転角度
						LocalPos				//位置
					);
				}
				break;
				default:
				{
					//球とボックス以外はスケーリング1.0
					Local.affineTransformation(
						bsm::Vec3(1.0f),			//スケーリングは1.0f
						bsm::Vec3(0, 0, 0),		//回転の中心（重心）
						LocalQt,				//回転角度
						LocalPos				//位置
					);
				}
				break;
			}
			bsm::Mat4x4 DrawWorld = Local * World;
			DrawShapeWireFrame(m_CombinedMeshVec[i], DrawWorld);
		}
	}

	//--------------------------------------------------------------------------------------
	///	マルチ物理コンポーネント
	//--------------------------------------------------------------------------------------
	PsMultiBody::PsMultiBody(const shared_ptr<GameObject>& GameObjectPtr):
		PsBodyComponentBase(GameObjectPtr)
	{}

	void PsMultiBody::CheckObjIndex(size_t index) const{
		if (index >= m_PsObjectVec.size()) {
			throw BaseException(
				L"PsMultiBodyのオブジェクトインデックスが範囲外です",
				L"if (index >= m_PsObjectVec.size())",
				L"PsMultiBody::CheckObjIndex()"
			);
		}
	}

	void  PsMultiBody::CheckJointIndex(size_t index) const {
		if (index >= m_PsJointVec.size()) {
			throw BaseException(
				L"PsMultiJointのオブジェクトインデックスが範囲外です",
				L"if (index >= m_PsJointVec.size())",
				L"PsMultiBody::CheckJointIndex()"
			);
		}

	}



	size_t PsMultiBody::AddSphere(const PsSphereParam& param) {
		size_t obj_index = m_PsObjectVec.size();
		auto PsSpherePtr = GetGameObject()->GetStage()->GetBasePhysics().AddSphere(param);
		m_PsObjectVec.push_back(PsSpherePtr);
		return obj_index;
	}

	size_t PsMultiBody::AddBox(const PsBoxParam& param) {
		size_t obj_index = m_PsObjectVec.size();
		auto PsBoxPtr = GetGameObject()->GetStage()->GetBasePhysics().AddBox(param);
		m_PsObjectVec.push_back(PsBoxPtr);
		return obj_index;
	}

	size_t PsMultiBody::AddCapsule(const PsCapsuleParam& param) {
		size_t obj_index = m_PsObjectVec.size();
		auto PsCapsuleObj = GetGameObject()->GetStage()->GetBasePhysics().AddCapsule(param);
		m_PsObjectVec.push_back(PsCapsuleObj);
		auto index = PsCapsuleObj->GetIndex();
		auto CapsuleMesh = PsBodyComponentBase::CreateCapsuleMesh(param);
		//Mapに登録
		m_MeshMap[index] =  CapsuleMesh;
		return obj_index;
	}

	size_t PsMultiBody::AddCylinder(const PsCylinderParam& param) {
		size_t obj_index = m_PsObjectVec.size();
		auto PsCylinderObj = GetGameObject()->GetStage()->GetBasePhysics().AddCylinder(param);
		m_PsObjectVec.push_back(PsCylinderObj);
		auto index = PsCylinderObj->GetIndex();
		auto CylinderMesh = PsBodyComponentBase::CreateCylinderMesh(param);
		//Mapに登録
		m_MeshMap[index] = CylinderMesh;
		return obj_index;
	}

	size_t PsMultiBody::AddConvex(const PsConvexParam& param) {
		size_t obj_index = m_PsObjectVec.size();
		auto PsConvexObj = GetGameObject()->GetStage()->GetBasePhysics().AddConvex(param);
		m_PsObjectVec.push_back(PsConvexObj);
		auto index = PsConvexObj->GetIndex();
		auto ConvexMesh = PsBodyComponentBase::CreateConvexMesh(param);
		//Mapに登録
		m_MeshMap[index] = ConvexMesh;
		return obj_index;
	}

	size_t PsMultiBody::AddBallJoint(const PsBallJointParam& param) {
		size_t j_index = m_PsJointVec.size();
		m_PsJointVec.push_back(GetGameObject()->GetStage()->GetBasePhysics().AddBallJoint(param));
		return j_index;
	}

	size_t PsMultiBody::AddSwingTwistJoint(const PsSwingTwistJointParam& param) {
		size_t j_index = m_PsJointVec.size();
		m_PsJointVec.push_back(GetGameObject()->GetStage()->GetBasePhysics().AddSwingTwistJoint(param));
		return j_index;
	}

	size_t PsMultiBody::AddHingeJoint(const PsHingeJointParam& param) {
		size_t j_index = m_PsJointVec.size();
		m_PsJointVec.push_back(GetGameObject()->GetStage()->GetBasePhysics().AddHingeJoint(param));
		return j_index;
	}

	size_t PsMultiBody::AddSliderJoint(const PsSliderJointParam& param) {
		size_t j_index = m_PsJointVec.size();
		m_PsJointVec.push_back(GetGameObject()->GetStage()->GetBasePhysics().AddSliderJoint(param));
		return j_index;
	}

	size_t PsMultiBody::AddFixJoint(const PsFixJointParam& param) {
		size_t j_index = m_PsJointVec.size();
		m_PsJointVec.push_back(GetGameObject()->GetStage()->GetBasePhysics().AddFixJoint(param));
		return j_index;
	}

	size_t PsMultiBody::AddUniversalJoint(const PsUniversalJointParam& param) {
		size_t j_index = m_PsJointVec.size();
		m_PsJointVec.push_back(GetGameObject()->GetStage()->GetBasePhysics().AddUniversalJoint(param));
		return j_index;
	}

	uint16_t PsMultiBody::GetJointIndex(size_t index) const {
		CheckJointIndex(index);
		return m_PsJointVec[index]->GetIndex();
	}

	shared_ptr<PsJoint> PsMultiBody::GetPsJoint(size_t index) const {
		CheckJointIndex(index);
		return m_PsJointVec[index];
	}



	uint16_t PsMultiBody::GetIndex(size_t index) const {
		CheckObjIndex(index);
		return m_PsObjectVec[index]->GetIndex();
	}

	shared_ptr<PsObject> PsMultiBody::GetPsObject(size_t index) const {
		CheckObjIndex(index);
		return  m_PsObjectVec[index];
	}


	void PsMultiBody::GetStatus(PsBodyStatus& st, size_t index) const {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().GetBodyStatus(GetIndex(index), st);
	}
	bsm::Vec3	PsMultiBody::GetPosition(size_t index) const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyPosition(GetIndex(index));
	}
	bsm::Quat	PsMultiBody::GetOrientation(size_t index) const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyOrientation(GetIndex(index));
	}
	bsm::Vec3	PsMultiBody::GetLinearVelocity(size_t index)const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyLinearVelocity(GetIndex(index));

	}

	bsm::Vec3	PsMultiBody::GetAngularVelocity(size_t index)const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().GetBodyAngularVelocity(GetIndex(index));

	}
	void PsMultiBody::SetStatus(const PsBodyUpdateStatus& st, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyStatus(GetIndex(index), st);

	}
	void PsMultiBody::WakeUp(size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().WakeUpBody(GetIndex(index));

	}
	void PsMultiBody::WakeUpAll() {
		for (auto& v : m_PsObjectVec) {
			GetGameObject()->GetStage()->GetBasePhysics().WakeUpBody(v->GetIndex());
		}
	}

	bool PsMultiBody::IsAutoGravity(size_t index)const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().IsAutoGravity(GetIndex(index));
	}
	void PsMultiBody::SetAutoGravity(bool b, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetAutoGravity(GetIndex(index), b);
	}


	void PsMultiBody::SetPosition(const bsm::Vec3& pos, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyPosition(GetIndex(index), pos);
	}
	void PsMultiBody::SetOrientation(const bsm::Quat& qt, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyOrientation(GetIndex(index), qt);

	}
	void PsMultiBody::SetLinearVelocity(const bsm::Vec3& v, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyLinearVelocity(GetIndex(index), v);

	}
	void PsMultiBody::SetAngularVelocity(const bsm::Vec3& v, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyAngularVelocity(GetIndex(index), v);

	}
	void PsMultiBody::MovePosition(const bsm::Vec3 &pos, float timeStep, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().MoveBodyPosition(GetIndex(index), pos, timeStep);

	}
	void PsMultiBody::MoveOrientation(const bsm::Quat& qt, float timeStep, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().MoveBodyOrientation(GetIndex(index), qt, timeStep);

	}
	void PsMultiBody::ApplyForce(const bsm::Vec3& v, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().ApplyBodyForce(GetIndex(index), v);

	}
	void PsMultiBody::ApplyTorque(const bsm::Vec3& v, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().ApplyBodyTorque(GetIndex(index), v);

	}
	void PsMultiBody::SetContactFilterSelf(uint32_t val, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyContactFilterSelf(GetIndex(index), val);

	}
	void PsMultiBody::SetContactFilterTarget(uint32_t val, size_t index) {
		CheckObjIndex(index);
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyContactFilterTarget(GetIndex(index), val);

	}
	bool PsMultiBody::GetContactsVec(vector<uint16_t>& contacts, size_t index)const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().GetContactsVec(GetIndex(index), contacts);

	}
	bool PsMultiBody::GetContactsSet(set<uint16_t>& contacts, size_t index)const {
		CheckObjIndex(index);
		return GetGameObject()->GetStage()->GetBasePhysics().GetContactsSet(GetIndex(index), contacts);
	}

	void PsMultiBody::OnDraw() {
		for (auto& v : m_PsObjectVec) {
			auto index = v->GetIndex();
			switch (v->GetShapeType()) {
				case PsShapeType::Sphere:
				{
					auto PsSpherePtr = dynamic_pointer_cast<PsSphere>(v);
					if (PsSpherePtr) {
						//行列の定義
						bsm::Mat4x4 World, Local;
						PsBodyStatus Status;
						auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
						auto MeshRes = App::GetApp()->GetResource<MeshResource>(L"PSWIRE_PC_SPHERE");
						BasePs.GetBodyStatus(index, Status);
						//ワールド行列の決定
						World.affineTransformation(
							bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							Status.m_Orientation,				//回転角度
							Status.m_Position			//位置
						);
						bsm::Vec3 LocalPos;
						bsm::Quat LocalQt;
						BasePs.GetShapeOffsetQuatPos(index, 0, LocalQt, LocalPos);
						Local.affineTransformation(
							bsm::Vec3(PsSpherePtr->GetParam().m_Radius),			//スケーリング
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							LocalQt,				//回転角度
							LocalPos				//位置
						);
						bsm::Mat4x4 DrawWorld = Local * World;
						DrawShapeWireFrame(MeshRes, DrawWorld);
					}
				}
				break;
				case PsShapeType::Box:
				{
					auto PsBoxPtr = dynamic_pointer_cast<PsBox>(v);
					if (PsBoxPtr) {
						//行列の定義
						bsm::Mat4x4 World, Local;
						PsBodyStatus Status;
						auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
						auto MeshRes = App::GetApp()->GetResource<MeshResource>(L"PSWIRE_PC_CUBE");
						BasePs.GetBodyStatus(index, Status);
						//ワールド行列の決定
						World.affineTransformation(
							bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							Status.m_Orientation,				//回転角度
							Status.m_Position			//位置
						);
						bsm::Vec3 LocalPos;
						bsm::Quat LocalQt;
						BasePs.GetShapeOffsetQuatPos(index, 0, LocalQt, LocalPos);
						Local.affineTransformation(
							PsBoxPtr->GetParam().m_HalfSize,			//スケーリング
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							LocalQt,				//回転角度
							LocalPos				//位置
						);
						bsm::Mat4x4 DrawWorld = Local * World;
						DrawShapeWireFrame(MeshRes, DrawWorld);
					}
				}
				break;
				case PsShapeType::Capsule:
				{
					auto PsCapsulePtr = dynamic_pointer_cast<PsCapsule>(v);
					if (PsCapsulePtr && m_MeshMap.find(index) != m_MeshMap.end()) {
						//行列の定義
						bsm::Mat4x4 World, Local;
						PsBodyStatus Status;
						auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
						auto MeshRes = m_MeshMap[index];
						BasePs.GetBodyStatus(index, Status);
						//ワールド行列の決定
						World.affineTransformation(
							bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							Status.m_Orientation,				//回転角度
							Status.m_Position			//位置
						);
						bsm::Vec3 LocalPos;
						bsm::Quat LocalQt;
						BasePs.GetShapeOffsetQuatPos(index, 0, LocalQt, LocalPos);

						Local.affineTransformation(
							bsm::Vec3(1.0f),			//スケーリングは1.0f
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							LocalQt,				//回転角度
							LocalPos				//位置
						);
						bsm::Mat4x4 DrawWorld = Local * World;
						DrawShapeWireFrame(MeshRes, DrawWorld);
					}
				}
				break;
				case PsShapeType::Cylinder:
				{
					auto PsCylinderPtr = dynamic_pointer_cast<PsCylinder>(v);
					if (PsCylinderPtr && m_MeshMap.find(index) != m_MeshMap.end()) {
						//行列の定義
						bsm::Mat4x4 World, Local;
						PsBodyStatus Status;
						auto& BasePs = GetGameObject()->GetStage()->GetBasePhysics();
						auto MeshRes = m_MeshMap[index];
						BasePs.GetBodyStatus(index, Status);
						//ワールド行列の決定
						World.affineTransformation(
							bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							Status.m_Orientation,				//回転角度
							Status.m_Position			//位置
						);
						bsm::Vec3 LocalPos;
						bsm::Quat LocalQt;
						BasePs.GetShapeOffsetQuatPos(index, 0, LocalQt, LocalPos);
						Local.affineTransformation(
							bsm::Vec3(1.0f),			//スケーリングは1.0f
							bsm::Vec3(0, 0, 0),		//回転の中心（重心）
							LocalQt,				//回転角度
							LocalPos				//位置
						);
						bsm::Mat4x4 DrawWorld = Local * World;
						DrawShapeWireFrame(MeshRes, DrawWorld);
					}
				}
				break;
				case PsShapeType::Convex:
				break;
			}
		}
	}





}
