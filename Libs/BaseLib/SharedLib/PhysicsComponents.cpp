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
	PsSingleSphereBody::PsSingleSphereBody(const shared_ptr<GameObject>& GameObjectPtr, const PsSphereParam& param):
		PsBodyComponent(GameObjectPtr)
	{
		m_PhysicsSphere = GameObjectPtr->GetStage()->GetBasePhysics().AddSingleSphere(param);
	}

	uint16_t PsSingleSphereBody::GetIndex() const {
		return m_PhysicsSphere->GetIndex();
	}

	void PsSingleSphereBody::Reset(const PsSphereParam& param, uint16_t index) {
		m_PhysicsSphere = GetGameObject()->GetStage()->GetBasePhysics().AddSingleSphere(param, index);
	}

	bool PsSingleSphereBody::CollisionTestBase(const SPHERE& src) {
		SPHERE dest;
		auto& param = m_PhysicsSphere->GetParam();
		dest.m_Radius = param.m_Radius;
		dest.m_Center = GetPosition();
		return HitTest::SPHERE_SPHERE(src, dest);
	}

	void PsSingleSphereBody::OnDraw() {
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
			bsm::Vec3(m_PhysicsSphere->GetParam().m_Radius),			//スケーリング
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
	PsSingleBoxBody::PsSingleBoxBody(const shared_ptr<GameObject>& GameObjectPtr, const PsBoxParam& param) :
		PsBodyComponent(GameObjectPtr)
	{
		m_PhysicsBox = GameObjectPtr->GetStage()->GetBasePhysics().AddSingleBox(param);


	}

	uint16_t PsSingleBoxBody::GetIndex() const {
		return m_PhysicsBox->GetIndex();
	}

	void PsSingleBoxBody::Reset(const PsBoxParam& param, uint16_t index) {
		m_PhysicsBox = GetGameObject()->GetStage()->GetBasePhysics().AddSingleBox(param, index);
	}


	bool PsSingleBoxBody::CollisionTestBase(const SPHERE& src) {
		OBB dest(m_PhysicsBox->GetParam().m_HalfSize * 2,
			GetOrientation(), GetPosition());
		bsm::Vec3 ret;
		return HitTest::SPHERE_OBB(src, dest, ret);
	}

	void PsSingleBoxBody::OnDraw() {
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
			m_PhysicsBox->GetParam().m_HalfSize,			//スケーリング
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
	PsSingleCapsuleBody::PsSingleCapsuleBody(const shared_ptr<GameObject>& GameObjectPtr, const PsCapsuleParam& param):
		PsBodyComponent(GameObjectPtr)
	{
		m_PhysicsCapsule = GetGameObject()->GetStage()->GetBasePhysics().AddSingleCapsule(param);
		CreateMesh(param);
	}

	void PsSingleCapsuleBody::CreateMesh(const PsCapsuleParam& param) {
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


	uint16_t PsSingleCapsuleBody::GetIndex() const {
		return m_PhysicsCapsule->GetIndex();
	}

	void PsSingleCapsuleBody::Reset(const PsCapsuleParam& param, uint16_t index) {
		m_PhysicsCapsule = GetGameObject()->GetStage()->GetBasePhysics().AddSingleCapsule(param, index);
		CreateMesh(param);
	}

	bool PsSingleCapsuleBody::CollisionTestBase(const SPHERE& src) {
		//ワールド行列の決定
		Mat4x4 World;
		World.affineTransformation(
			bsm::Vec3(1.0, 1.0, 1.0),			//スケーリング
			bsm::Vec3(0, 0, 0),		//回転の中心（重心）
			GetOrientation(),				//回転角度
			GetPosition()			//位置
		);
		CAPSULE dest(m_PhysicsCapsule->GetParam().m_Radius,
			Vec3(0, -m_PhysicsCapsule->GetParam().m_HalfLen, 0),
			Vec3(0, m_PhysicsCapsule->GetParam().m_HalfLen, 0),
			World);
		bsm::Vec3 ret;
		return HitTest::SPHERE_CAPSULE(src, dest, ret);
	}



	void PsSingleCapsuleBody::OnDraw() {
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
	PsSingleCylinderBody::PsSingleCylinderBody(const shared_ptr<GameObject>& GameObjectPtr, const PsCylinderParam& param):
		PsBodyComponent(GameObjectPtr)
	{
		m_PhysicsCylinder = GetGameObject()->GetStage()->GetBasePhysics().AddSingleCylinder(param);
		CreateMesh(param);
	}

	void PsSingleCylinderBody::CreateMesh(const PsCylinderParam& param) {
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


	uint16_t PsSingleCylinderBody::GetIndex() const {
		return m_PhysicsCylinder->GetIndex();
	}

	void PsSingleCylinderBody::Reset(const PsCylinderParam& param, uint16_t index) {
		m_PhysicsCylinder = GetGameObject()->GetStage()->GetBasePhysics().AddSingleCylinder(param, index);
		CreateMesh(param);
	}

	void PsSingleCylinderBody::OnDraw() {
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




}
