/*!
@file PhysicsManager.cpp
@brief 物理計算用マネージャなど実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	///	物理計算コンポーネントの親
	//--------------------------------------------------------------------------------------
	PsBodyComponent::PsBodyComponent(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		m_AutoTransform(true)
	{
		SetDrawActive(false);
	}
	PsBodyComponent::~PsBodyComponent() {}

	bsm::Vec3	PsBodyComponent::GetPosition() const {
		PsBodyStatus Status;
		GetGameObject()->GetStage()->GetBasePhysics().GetBodyStatus(GetIndex(), Status);
		return Status.m_Position;
	}

	bsm::Quat	PsBodyComponent::GetOrientation() const {
		PsBodyStatus Status;
		GetGameObject()->GetStage()->GetBasePhysics().GetBodyStatus(GetIndex(), Status);
		return Status.m_Orientation;
	}


	bsm::Vec3	PsBodyComponent::GetLinearVelocity() const {
		PsBodyStatus Status;
		GetGameObject()->GetStage()->GetBasePhysics().GetBodyStatus(GetIndex(), Status);
		return Status.m_LinearVelocity;
	}

	bsm::Vec3	PsBodyComponent::GetAngularVelocity() const {
		PsBodyStatus Status;
		GetGameObject()->GetStage()->GetBasePhysics().GetBodyStatus(GetIndex(), Status);
		return Status.m_AngularVelocity;
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

	void PsBodyComponent::SetPosition(const bsm::Vec3& pos) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyPosition(GetIndex(), pos);
	}

	void PsBodyComponent::SetLinearVelocity(const bsm::Vec3& v) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyLinearVelocity(GetIndex(), v);

	}
	void PsBodyComponent::SetAngularVelocity(const bsm::Vec3& v) {
		GetGameObject()->GetStage()->GetBasePhysics().SetBodyAngularVelocity(GetIndex(), v);

	}
	void PsBodyComponent::ApplyForce(const bsm::Vec3& v) {
		GetGameObject()->GetStage()->GetBasePhysics().ApplyBodyForce(GetIndex(), v);

	}
	void PsBodyComponent::ApplyTorque(const bsm::Vec3& v) {
		GetGameObject()->GetStage()->GetBasePhysics().ApplyBodyTorque(GetIndex(), v);
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


	void PsBodyComponent::DrawShapeWireFrame(const shared_ptr<MeshResource>& res, const bsm::Mat4x4& world) {
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
		CreateMesh(param);
	}

	void PsCapsuleBody::CreateMesh(const PsCapsuleParam& param) {
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
		m_CapsuleMesh = MeshResource::CreateMeshResource(new_pc_vertices, indices, false);
	}


	uint16_t PsCapsuleBody::GetIndex() const {
		return m_PsCapsule->GetIndex();
	}

	void PsCapsuleBody::Reset(const PsCapsuleParam& param, uint16_t index) {
		m_PsCapsule = GetGameObject()->GetStage()->GetBasePhysics().AddCapsule(param, index);
		CreateMesh(param);
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
		CreateMesh(param);
	}

	void PsCylinderBody::CreateMesh(const PsCylinderParam& param) {
		vector<VertexPositionNormalTexture> vertices;
		vector<VertexPositionColor> new_pc_vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCylinder(param.m_HalfLen * 2.0f,param.m_Radius * 2.0f,6, vertices, indices, true);
		for (auto& v : vertices) {
			VertexPositionColor vertex;
			vertex.position = v.position;
			vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			new_pc_vertices.push_back(vertex);
		}
		m_CylinderMesh = MeshResource::CreateMeshResource(new_pc_vertices, indices, false);
	}


	uint16_t PsCylinderBody::GetIndex() const {
		return m_PsCylinder->GetIndex();
	}

	void PsCylinderBody::Reset(const PsCylinderParam& param, uint16_t index) {
		m_PsCylinder = GetGameObject()->GetStage()->GetBasePhysics().AddCylinder(param, index);
		CreateMesh(param);
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
		CreateMesh(param);
	}

	void PsConvexBody::CreateMesh(const PsConvexParam& param) {
		vector<VertexPositionColor> new_pc_vertices;
		for (auto& v : param.m_ConvexMeshResource->GetVertices()) {
			VertexPositionColor vertex;
			vertex.position = v.position;
			vertex.color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			new_pc_vertices.push_back(vertex);
		}
		m_ConvexMesh = MeshResource::CreateMeshResource(new_pc_vertices, param.m_ConvexMeshResource->GetIndices(), false);
	}
	uint16_t PsConvexBody::GetIndex() const {
		return m_PsConvex->GetIndex();
	}

	void PsConvexBody::Reset(const PsConvexParam& param, uint16_t index) {
		m_PsConvex = GetGameObject()->GetStage()->GetBasePhysics().AddConvex(param, index);
		CreateMesh(param);
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



}
