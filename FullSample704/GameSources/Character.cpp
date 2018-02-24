/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	位置指定のライン
	//--------------------------------------------------------------------------------------
	PositionLine::PositionLine(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos,const Vec3& EndPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_EndPos(EndPos)
	{}




	//初期化
	void PositionLine::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vec3(1.0f, 1.0f, 1.0f));
		Quat Qt;
		Qt.identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(Vec3(0.0f, 0.0f, 0.0f));

		//描画コンポーネント
		auto PtrDraw = AddComponent<PCStaticDraw>();

		vector<VertexPositionColor> vertices = {
			{ VertexPositionColor(m_StartPos,  Vec4(1.0f, 1.0f,0.0f,1.0f)) },
			{ VertexPositionColor(m_EndPos,  Vec4(1.0f, 1.0f,0.0f,1.0f)) }

		};
		vector<uint16_t> indices = {
			0, 1
		};

		auto MeshRes = MeshResource::CreateMeshResource(vertices, indices, true);
		MeshRes->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		PtrDraw->SetOriginalMeshResource(MeshRes);
		PtrDraw->SetOriginalMeshUse(true);

	}

	void PositionLine::OnUpdate() {

		auto PtrDraw = GetComponent<PCStaticDraw>();
		const vector<VertexPositionColor>& BackupVec = PtrDraw->GetOriginalMeshResource()->GetBackupVerteces<VertexPositionColor>();
		vector<VertexPositionColor> new_vec;
		VertexPositionColor new_v;
		new_v = BackupVec[0];
		new_v.position = m_StartPos;
		new_vec.push_back(new_v);

		new_v = BackupVec[1];
		new_v.position = m_EndPos;
		new_vec.push_back(new_v);

		PtrDraw->UpdateVertices(new_vec);

	}


	//--------------------------------------------------------------------------------------
	///	物理計算する固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedPsBox::FixedPsBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Position
	):
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Position(Position)
	{}

	FixedPsBox::~FixedPsBox(){}
	//初期化
	void FixedPsBox::OnCreate() {

		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetQuaternion(m_Qt);
		PtrTransform->SetPosition(m_Position);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

		//物理計算ボックス
		PsBoxParam param;
		//DEFAULT_CUBEのスケーリングは各辺基準なので、ハーフサイズにする
		param.m_HalfSize = m_Scale * 0.5f;
		//固定なので質量はいらない
		param.m_Mass = 0.0f;
		//慣性テンソルもデフォルトで良い
		param.m_MotionType = PsMotionType::MotionTypeFixed;
		param.m_Quat = m_Qt;
		param.m_Pos = m_Position;
		auto PsPtr = AddComponent<PsBoxBody>(param);
		PsPtr->SetDrawActive(true);

	}



	//--------------------------------------------------------------------------------------
	///	ラグドール
	//--------------------------------------------------------------------------------------
	Ragdoll::Ragdoll(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos, 
		const Quat& StartQt) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_StartQt(StartQt)
	{
	}
	Ragdoll::~Ragdoll() {}

	void Ragdoll::OnCreate() {
		//初期位置などの設定
		auto Ptr = GetComponent<Transform>();
		float Scale(1.0f);
		Ptr->SetScale(Vec3(Scale));
		Ptr->SetQuaternion(m_StartQt);
		Ptr->SetPosition(m_StartPos);

		//マルチ物理オブジェクト
		auto PsPtr = AddComponent<PsMultiBody>();
		PsPtr->SetDrawActive(true);
		PsPtr->SetAutoTransform(false);

		uint32_t contactFilterSelfA = 0x01;
		uint32_t contactFilterSelfB = 0x02;
		uint32_t contactFilterSelfC = 0x04;
		uint32_t contactFilterSelfD = 0x08;
		uint32_t contactFilterTargetA = 0x0d;
		uint32_t contactFilterTargetB = 0x0a;
		uint32_t contactFilterTargetC = 0x05;
		uint32_t contactFilterTargetD = 0x0b;

		size_t HeadNum, TorsoNum, BodyNum,
			UpperLegLNum, LowerLegLNum, UpperArmLNum, LowerArmLNum,
			UpperLegRNum, LowerLegRNum, UpperArmRNum, LowerArmRNum;


		float inertiaScale = 3.0f;
		{
			//頭部の作成
			PsSphereParam param;
			param.m_Radius = 0.3f;
			param.m_Mass = 3.0f;
			//慣性テンソルの計算
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(0.0f, 3.38433f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfB;
			param.m_ContactFilterTarget = contactFilterTargetB;
			//頭部の追加
			HeadNum = PsPtr->AddSphere(param);


		}
		{
			//腰の作成
			PsSphereParam param;
			param.m_Radius = 0.35f;
			param.m_Mass = 10.0f;
			//慣性テンソルの計算
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaSphere(param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(0.0f, 1.96820f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfB;
			param.m_ContactFilterTarget = contactFilterTargetB;
			//腰の追加
			TorsoNum = PsPtr->AddSphere(param);

		}
		{
			//胴体の作成
			PsCapsuleParam param;
			param.m_HalfLen = 0.1f;
			param.m_Radius = 0.38f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 8.0f;
			//慣性テンソルの計算
			//調整する
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(0.0f, 2.66926f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfA;
			param.m_ContactFilterTarget = contactFilterTargetA;
			//胴体の追加
			BodyNum = PsPtr->AddCapsule(param);
		}
		{
			//左太ももの作成
			PsCapsuleParam param;
			param.m_HalfLen = 0.35f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 8.0f;
			//慣性テンソルの計算
			//調整する
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(0.21f, 1.34871f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfC;
			param.m_ContactFilterTarget = contactFilterTargetC;
			//左太ももの追加
			UpperLegLNum = PsPtr->AddCapsule(param);

		}
		{
			//左下腿部の作成
			PsCapsuleParam param;
			param.m_HalfLen = 0.3f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 4.0f;
			//慣性テンソルの計算
			//調整する
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(0.21f, 0.59253f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfD;
			param.m_ContactFilterTarget = contactFilterTargetD;
			//左下腿部の追加
			LowerLegLNum = PsPtr->AddCapsule(param);
		}

		{
			//左上部腕の作成
			PsCapsuleParam param;
			param.m_HalfLen = 0.25f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 5.0f;
			//慣性テンソルの計算
			//調整する
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = Quat(0.0f, 0.0f, 0.70711f, 0.70711f) * m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(0.72813f, 2.87483f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfB;
			param.m_ContactFilterTarget = contactFilterTargetB;
			//左上部腕の追加
			UpperArmLNum = PsPtr->AddCapsule(param);
		}
		{
			//左下部腕の作成
			PsCapsuleParam param;
			param.m_HalfLen = 0.225f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 3.0f;
			//慣性テンソルの計算
			//調整する
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = Quat(0.0f, 0.0f, 0.70711f, 0.70711f) * m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(1.42931f, 2.87483f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfC;
			param.m_ContactFilterTarget = contactFilterTargetC;
			//左下部腕の追加
			LowerArmLNum = PsPtr->AddCapsule(param);
		}
		{
			//右太ももの作成
			PsCapsuleParam param;
			param.m_HalfLen = 0.35f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 8.0f;
			//慣性テンソルの計算
			//調整する
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(-0.21f, 1.34871f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfC;
			param.m_ContactFilterTarget = contactFilterTargetC;
			//右太ももの追加
			UpperLegRNum = PsPtr->AddCapsule(param);

		}

		{
			//右下腿部の作成
			PsCapsuleParam param;
			param.m_HalfLen = 0.3f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 4.0f;
			//慣性テンソルの計算
			//調整する
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(-0.21f, 0.59253f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfD;
			param.m_ContactFilterTarget = contactFilterTargetD;
			//右下腿部の追加
			LowerLegRNum = PsPtr->AddCapsule(param);
		}

		{
			//右上部腕の作成
			PsCapsuleParam param;
			param.m_HalfLen = 0.25f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 5.0f;
			//慣性テンソルの計算
			//調整する
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = Quat(0.0f, 0.0f, -0.70711f, 0.70711f) * m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(-0.72813f, 2.87483f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfB;
			param.m_ContactFilterTarget = contactFilterTargetB;
			//右上部腕の追加
			UpperArmRNum = PsPtr->AddCapsule(param);
		}
		{
			//右下部腕の作成
			PsCapsuleParam param;
			param.m_HalfLen = 0.225f;
			param.m_Radius = 0.15f;
			param.m_OffsetOrientation = Quat(0.0f, 0.0f, 0.70711f, 0.70711f);
			param.m_Mass = 3.0f;
			//慣性テンソルの計算
			//調整する
			param.m_Inertia = inertiaScale * BasePhysics::CalcInertiaCylinderY(param.m_HalfLen + param.m_Radius, param.m_Radius, param.m_Mass);
			param.m_MotionType = PsMotionType::MotionTypeActive;
			param.m_Quat = Quat(0.0f, 0.0f, -0.70711f, 0.70711f) * m_StartQt;
			param.m_Pos = m_StartPos + rotate(m_StartQt, Vec3(-1.42931f, 2.87483f, 0.0f));
			param.m_ContactFilterSelf = contactFilterSelfC;
			param.m_ContactFilterTarget = contactFilterTargetC;
			//右下部腕の追加
			LowerArmRNum = PsPtr->AddCapsule(param);
		}
		{
			//腰と胴体のジョイント
			PsSwingTwistJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(0.0f, 2.26425f, 0.0f));
			jparam.m_TwistAxis = rotate(m_StartQt, Vec3(0.0f, 1.0f, 0.0f));
			jparam.m_IndexA = PsPtr->GetPsObject(TorsoNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(BodyNum)->GetIndex();
			PsPtr->AddSwingTwistJoint(jparam);
		}
		{
			//胴体と頭部のジョイント
			PsSwingTwistJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(0.0f, 3.13575f, 0.0f));
			jparam.m_TwistAxis = rotate(m_StartQt, Vec3(0.0f, 1.0f, 0.0f));
			jparam.m_IndexA = PsPtr->GetPsObject(BodyNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(HeadNum)->GetIndex();
			PsPtr->AddSwingTwistJoint(jparam);
		}
		{
			//胴体と左上部腕のジョイント
			PsSwingTwistJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(0.40038f, 2.87192f, 0.0f));
			jparam.m_TwistAxis = rotate(m_StartQt, Vec3(1.0f, 0.0f, 0.0f));
			jparam.m_IndexA = PsPtr->GetPsObject(BodyNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(UpperArmLNum)->GetIndex();
			PsPtr->AddSwingTwistJoint(jparam);
		}
		{
			//左上部腕と左下部腕のジョイント
			PsHingeJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(1.08651f, 2.87483f, 0.00000f));
			jparam.m_Axis = rotate(m_StartQt, Vec3(0.0f, -1.0f, 0.0f));
			jparam.m_LowerAngle = 0.0f;
			jparam.m_UpperAngle = 1.57f;
			jparam.m_IndexA = PsPtr->GetPsObject(UpperArmLNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(LowerArmLNum)->GetIndex();
			PsPtr->AddHingeJoint(jparam);
		}
		{
			//胴体と右上部腕のジョイント
			PsSwingTwistJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(-0.40360f, 2.87499f, 0.00000f));
			jparam.m_TwistAxis = rotate(m_StartQt, Vec3(-1.0f, 0.0f, 0.0f));
			jparam.m_IndexA = PsPtr->GetPsObject(BodyNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(UpperArmRNum)->GetIndex();
			PsPtr->AddSwingTwistJoint(jparam);
		}
		{
			//右上部腕と右下部腕のジョイント
			PsHingeJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(-1.09407f, 2.87483f, 0.00000f));
			jparam.m_Axis = rotate(m_StartQt, Vec3(0.0f, -1.0f, 0.0f));
			jparam.m_LowerAngle = 0.0f;
			jparam.m_UpperAngle = 1.57f;
			jparam.m_IndexA = PsPtr->GetPsObject(UpperArmRNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(LowerArmRNum)->GetIndex();
			PsPtr->AddHingeJoint(jparam);
		}
		{
			//腰と左太もものジョイント
			PsUniversalJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(0.20993f, 1.69661f, 0.00000f));
			jparam.m_Axis = rotate(m_StartQt, Vec3(0.0f, -1.0f, 0.0f));
			jparam.m_Swing1LowerAngle = 0.0f;
			jparam.m_Swing1UpperAngle = 0.52f;
			jparam.m_Swing2LowerAngle = 0.0f;
			jparam.m_Swing2UpperAngle = 1.57f;
			jparam.m_IndexA = PsPtr->GetPsObject(TorsoNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(UpperLegLNum)->GetIndex();
			PsPtr->AddUniversalJoint(jparam);
		}
		{
			//腰と右太もものジョイント
			PsUniversalJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(-0.21311f, 1.69661f, 0.00000f));
			jparam.m_Axis = rotate(m_StartQt, Vec3(0.0f, -1.0f, 0.0f));
			jparam.m_Swing1LowerAngle = 0.0f;
			jparam.m_Swing1UpperAngle = 0.52f;
			jparam.m_Swing2LowerAngle = 0.0f;
			jparam.m_Swing2UpperAngle = 1.57f;
			jparam.m_IndexA = PsPtr->GetPsObject(TorsoNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(UpperLegRNum)->GetIndex();
			PsPtr->AddUniversalJoint(jparam);

		}
		{
			//左太ももと左下腿部のジョイント
			PsHingeJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(0.21000f, 0.97062f, 0.00000f));
			jparam.m_Axis = rotate(m_StartQt, Vec3(1.0f, 0.0f, 0.0f));
			jparam.m_LowerAngle = 0.0f;
			jparam.m_UpperAngle = 1.57f;
			jparam.m_IndexA = PsPtr->GetPsObject(UpperLegLNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(LowerLegLNum)->GetIndex();
			PsPtr->AddHingeJoint(jparam);

		}
		{
			//右太ももと右下腿部のジョイント
			PsHingeJointParam jparam;
			jparam.m_AnchorPoint = m_StartPos + rotate(m_StartQt, Vec3(-0.21000f, 0.97062f, 0.00000f));
			jparam.m_Axis = rotate(m_StartQt, Vec3(1.0f, 0.0f, 0.0f));
			jparam.m_LowerAngle = 0.0f;
			jparam.m_UpperAngle = 1.57f;
			jparam.m_IndexA = PsPtr->GetPsObject(UpperLegRNum)->GetIndex();
			jparam.m_IndexB = PsPtr->GetPsObject(LowerLegRNum)->GetIndex();
			PsPtr->AddHingeJoint(jparam);

		}
		//ジョイントのパラメータ調整
		for (auto& j : PsPtr->GetPsJointVec()) {
			j->getPfxJoint().m_constraints[0].m_warmStarting = 1;
			j->getPfxJoint().m_constraints[1].m_warmStarting = 1;
			j->getPfxJoint().m_constraints[2].m_warmStarting = 1;

			j->getPfxJoint().m_constraints[3].m_damping = 0.05f;
			j->getPfxJoint().m_constraints[4].m_damping = 0.05f;
			j->getPfxJoint().m_constraints[5].m_damping = 0.05f;
		}



	}

	void Ragdoll::OnUpdate() {
		auto Ptr = GetComponent<Transform>();
		auto PsPtr = GetComponent<PsMultiBody>(false);
		if (PsPtr) {
			auto BasePos =PsPtr->GetPosition(0);
			Ptr->SetPosition(BasePos);
		}

	}

	//--------------------------------------------------------------------------------------
	///	ラグドールマネージャ（描画を管理する）
	//--------------------------------------------------------------------------------------
	RagdollManager::RagdollManager(const shared_ptr<Stage>& StagePtr, const shared_ptr<Ragdoll>& ragdoll):
		GameObject(StagePtr),
		m_Ragdoll(ragdoll),
		m_HoldIndex(0),
		m_Holded(false)
	{}
	RagdollManager::~RagdollManager() {}

	void RagdollManager::OnCreate() {
		auto RagSh = m_Ragdoll.lock();
		if (RagSh) {
			auto PtrPsBody = RagSh->GetComponent<PsMultiBody>();
			{
				//頭
				auto Ptr = GetStage()->AddGameObject<GameObject>();
				auto SpherePtr = dynamic_pointer_cast<PsSphere>(PtrPsBody->GetPsObject(0));
				if (SpherePtr) {
					Vec3 Scale(SpherePtr->GetParam().m_Radius * 2.0f);
					Ptr->GetComponent<Transform>()->SetScale(Scale);
					Ptr->GetComponent<Transform>()->SetQuaternion(PtrPsBody->GetOrientation(0));
					Ptr->GetComponent<Transform>()->SetPosition(PtrPsBody->GetPosition(0));
					auto DrawPtr = Ptr->AddComponent<BcPNTStaticDraw>();
					DrawPtr->SetMeshResource(L"DEFAULT_SPHERE");
					m_RagdollObjVec.push_back(Ptr);
				}
			}
			{
				//腰
				auto Ptr = GetStage()->AddGameObject<GameObject>();
				auto SpherePtr = dynamic_pointer_cast<PsSphere>(PtrPsBody->GetPsObject(1));
				if (SpherePtr) {
					Vec3 Scale(SpherePtr->GetParam().m_Radius * 2.0f);
					Ptr->GetComponent<Transform>()->SetScale(Scale);
					Ptr->GetComponent<Transform>()->SetQuaternion(PtrPsBody->GetOrientation(1));
					Ptr->GetComponent<Transform>()->SetPosition(PtrPsBody->GetPosition(1));
					auto DrawPtr = Ptr->AddComponent<BcPNTStaticDraw>();
					DrawPtr->SetMeshResource(L"DEFAULT_SPHERE");
					m_RagdollObjVec.push_back(Ptr);
				}
			}
			{
				//胴体
				auto Ptr = GetStage()->AddGameObject<GameObject>();
				auto CapsulePtr = dynamic_pointer_cast<PsCapsule>(PtrPsBody->GetPsObject(2));
				if (CapsulePtr) {
					Vec3 Scale(
						CapsulePtr->GetParam().m_Radius * 2.0f,
						0.5f,//胴体の描画は高さ0.5fで描画する
						CapsulePtr->GetParam().m_Radius * 2.0f
					);
					Ptr->GetComponent<Transform>()->SetScale(Scale);
					Ptr->GetComponent<Transform>()->SetQuaternion(PtrPsBody->GetOrientation(2));
					Ptr->GetComponent<Transform>()->SetPosition(PtrPsBody->GetPosition(2));
					auto DrawPtr = Ptr->AddComponent<BcPNTStaticDraw>();
					DrawPtr->SetMeshResource(L"DEFAULT_CAPSULE");
					m_RagdollObjVec.push_back(Ptr);
				}
			}
			{
				//左太ももから先はループで作成
				for (size_t i = 3; i <= 10; i++) {
					auto Ptr = GetStage()->AddGameObject<GameObject>();
					auto CapsulePtr = dynamic_pointer_cast<PsCapsule>(PtrPsBody->GetPsObject(i));
					if (CapsulePtr) {
						Vec3 Scale(
							CapsulePtr->GetParam().m_Radius * 2.0f,
							CapsulePtr->GetParam().m_HalfLen * 1.5f,
							CapsulePtr->GetParam().m_Radius * 2.0f
						);
						Ptr->GetComponent<Transform>()->SetScale(Scale);
						Ptr->GetComponent<Transform>()->SetQuaternion(PtrPsBody->GetOrientation(i));
						Ptr->GetComponent<Transform>()->SetPosition(PtrPsBody->GetPosition(i));
						auto DrawPtr = Ptr->AddComponent<BcPNTStaticDraw>();
						DrawPtr->SetMeshResource(L"DEFAULT_CAPSULE");
						m_RagdollObjVec.push_back(Ptr);
					}

				}
			}
		}
	}

	void RagdollManager::OnUpdate() {
		auto RagSh = m_Ragdoll.lock();
		if (RagSh) {
			auto PtrPsBody = RagSh->GetComponent<PsMultiBody>();
			for (size_t i = 0; i < m_RagdollObjVec.size(); i++) {
				auto ShPtr = m_RagdollObjVec[i].lock();
				if (ShPtr) {
					ShPtr->GetComponent<Transform>()->SetQuaternion(PtrPsBody->GetOrientation(i));
					ShPtr->GetComponent<Transform>()->SetPosition(PtrPsBody->GetPosition(i));
				}
				if (m_Holded && i == m_HoldIndex) {
					auto DrawPtr = ShPtr->GetComponent<BcPNTStaticDraw>();
					DrawPtr->SetEmissive(Col4(1.0f, 0.0f, 0, 1.0f));
				}
				else {
					auto DrawPtr = ShPtr->GetComponent<BcPNTStaticDraw>();
					DrawPtr->SetEmissive(Col4(0.0f, 0.0f, 0, 0.0f));
				}
			}
		}
	}





}
//end basecross
