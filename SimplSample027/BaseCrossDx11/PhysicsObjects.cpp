/*!
@file PhysicsObjects.cpp
@brief 物理計算オブジェクト実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	物理計算する固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedPsBox::FixedPsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	FixedPsBox::~FixedPsBox() {}
	//初期化
	void FixedPsBox::OnCreate() {

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f,vertices, indices);
		//メッシュの作成（変更できない）
		m_BoxMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Position
		);
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(L"SKY_TX");
		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = m_BoxMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = true;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_FogEnabled = true;
		//フォグは青色っぽく
		m_PtrObj->m_FogColor = Col4(0.4f, 0.4f, 0.8f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_BoxMesh;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();


		//物理計算ボックス
		PsBoxParam param;
		param.m_HalfSize = m_Scale * 0.5f;
		//固定なので質量はいらない
		param.m_Mass = 0.0f;
		//慣性テンソルもデフォルトで良い
		param.m_MotionType = PsMotionType::MotionTypeFixed;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		m_PhysicsBox = GetStage()->GetPhysicsManager()->AddBox(param);

	}

	void FixedPsBox::OnDrawShadowmap() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Position
		);
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}


	void FixedPsBox::OnDraw() {
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}


	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブなボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ActivePsBox::ActivePsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsBox::~ActivePsBox() {}
	//初期化
	void ActivePsBox::OnCreate() {

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		//メッシュの作成（変更できない）
		m_BoxMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Position
		);
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(L"SKY_TX");
		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = m_BoxMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = true;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_FogEnabled = true;
		//フォグは青色っぽく
		m_PtrObj->m_FogColor = Col4(0.4f, 0.4f, 0.8f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_BoxMesh;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();

		//物理計算ボックス
		PsBoxParam param;
		//basecrossのスケーリングは各辺基準なので、ハーフサイズ基準にする
		param.m_HalfSize = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//慣性テンソルの計算
		param.m_Inertia = BasePhysics::CalcInertiaBox(param.m_HalfSize, param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		m_PhysicsBox = GetStage()->GetPhysicsManager()->AddBox(param);

	}

	void ActivePsBox::OnUpdate() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsBox->GetIndex(), Status);

	}

	void ActivePsBox::OnDrawShadowmap() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsBox->GetIndex(), Status);
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			Status.m_Orientation,
			Status.m_Position
		);
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}


	void ActivePsBox::OnDraw() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsBox->GetIndex(), Status);
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			Status.m_Orientation,
			Status.m_Position
		);
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_WorldMatrix = World;
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブな球体
	//--------------------------------------------------------------------------------------
	//構築と破棄
	ActivePsSphere::ActivePsSphere(const shared_ptr<Stage>& StagePtr,
		float Scale,
		const Quat& Qt,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	ActivePsSphere::~ActivePsSphere() {}
	//初期化
	void ActivePsSphere::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		//メッシュの作成（変更できない）
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			Vec3(m_Scale),
			Vec3(0, 0, 0),
			m_Qt,
			m_Position
		);
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(L"SKY_TX");
		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = m_SphereMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = true;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_FogEnabled = true;
		//フォグは青色っぽく
		m_PtrObj->m_FogColor = Col4(0.4f, 0.4f, 0.8f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_SphereMesh;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();

		//物理計算球体
		PsSphereParam param;
		//basecrossのスケーリングは直径基準なので、半径基準にする
		param.m_Radius = m_Scale * 0.5f;
		param.m_Mass = 1.0f;
		//慣性テンソルの計算
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		m_PhysicsSphere = GetStage()->GetPhysicsManager()->AddSphere(param);

	}

	void ActivePsSphere::OnUpdate() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);

	}

	void ActivePsSphere::OnDrawShadowmap() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			Vec3(m_Scale),
			Vec3(0, 0, 0),
			Status.m_Orientation,
			Status.m_Position
		);
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}


	void ActivePsSphere::OnDraw() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			Vec3(m_Scale),
			Vec3(0, 0, 0),
			Status.m_Orientation,
			Status.m_Position
		);
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_WorldMatrix = World;
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}


	//--------------------------------------------------------------------------------------
	///	物理計算する発射する球体
	//--------------------------------------------------------------------------------------
	FirePsSphere::FirePsSphere(const shared_ptr<Stage>& StagePtr,
		const Vec3& Emitter, const Vec3& Velocity) :
		GameObject(StagePtr),
		m_Emitter(Emitter),
		m_Velocity(Velocity)
	{}
	FirePsSphere::~FirePsSphere() {}
	//初期化
	void FirePsSphere::OnCreate() {

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		//メッシュの作成（変更できない）
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);


		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			Vec3(0.25f),
			Vec3(0, 0, 0),
			Quat(),
			m_Emitter
		);
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(L"SKY_TX");
		//描画データの構築
		m_PtrObj = make_shared<BcDrawObject>();
		m_PtrObj->m_MeshRes = m_SphereMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = true;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_FogEnabled = true;
		//フォグは青色っぽく
		m_PtrObj->m_FogColor = Col4(0.4f, 0.4f, 0.8f, 1.0f);
		m_PtrObj->m_FogStart = -10.0f;
		m_PtrObj->m_FogEnd = -30.0f;

		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_SphereMesh;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();

		PsSphereParam param;
		//basecrossのスケーリングは直径基準なので、半径基準にする
		param.m_Radius = 0.25f * 0.5f;
		param.m_Mass = 1.0f;
		//慣性テンソルの計算
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		param.m_UseSleep = false;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
		param.m_Pos = m_Emitter;
		param.m_LinearVelocity = m_Velocity;
		m_PhysicsSphere = GetStage()->GetPhysicsManager()->AddSphere(param);
	}
	//更新
	void FirePsSphere::OnUpdate() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);

	}

	void FirePsSphere::Reset(const Vec3& Emitter, const Vec3& Velocity) {
		//現在のインデックスを得る
		auto Index = m_PhysicsSphere->GetIndex();
		PsSphereParam param;
		//basecrossのスケーリングは直径基準なので、半径基準にする
		param.m_Radius = 0.25f * 0.5f;
		param.m_Mass = 1.0f;
		//慣性テンソルの計算
		param.m_Inertia = BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
		param.m_UseSleep = false;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
		param.m_Pos = Emitter;
		param.m_LinearVelocity = Velocity;
		//同じインデックスで再構築
		m_PhysicsSphere = GetStage()->GetPhysicsManager()->AddSphere(param, Index);
	}


	void FirePsSphere::OnDrawShadowmap() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			Vec3(0.25f),
			Vec3(0, 0, 0),
			Status.m_Orientation,
			Status.m_Position
		);
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;
		m_PtrShadowmapObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_ShadowmapRenderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<ShadowmapRenderer>(L"ShadowmapRenderer");
			m_ShadowmapRenderer = shptr;
		}
		shptr->AddDrawObject(m_PtrShadowmapObj);
	}


	void FirePsSphere::OnDraw() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			Vec3(0.25f),
			Vec3(0, 0, 0),
			Status.m_Orientation,
			Status.m_Position
		);
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_WorldMatrix = World;
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			shptr = GetStage<Stage>()->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}

}
//end basecross
