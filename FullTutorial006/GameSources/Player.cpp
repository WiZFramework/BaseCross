/*!
@file Player.cpp
@brief �v���C���[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	�p�r: �v���C���[
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_MaxSpeed(40.0f),	//�ō����x
		m_Decel(0.95f),	//�����l
		m_Mass(1.0f)	//����
	{}

	//������
	void Player::OnCreate() {
		//�����ʒu�Ȃǂ̐ݒ�
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(0.25f, 0.25f, 0.25f);	//���a25�Z���`�̋���
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.125f, 0);
		//Rigidbody������
		auto PtrRedid = AddComponent<Rigidbody>();
		//�����W����0.5�i�����j
		PtrRedid->SetReflection(0.5f);
		//�d�͂�����
		auto PtrGravity = AddComponent<Gravity>();
		//�ŉ��n�_
		PtrGravity->SetBaseY(0.125f);
		//�Փ˔���
		auto PtrColl = AddComponent<CollisionSphere>();
		//�������̂ݔ���
		PtrColl->SetIsHitAction(IsHitAction::AutoOnObjectRepel);

		PtrColl->SetDrawActive(true);

		Matrix4X4 SpanMat; // ���f���ƃg�����X�t�H�[���̊Ԃ̍����s��
		SpanMat.DefTransformation(
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, XM_PI, 0.0f),
			Vector3(0.0f, -0.5f, 0.0f)
		);

		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();

		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(L"Chara_R_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		//�`��R���|�[�l���g�̐ݒ�
		auto PtrDraw = AddComponent<PNTBoneModelDraw>();
		//�`�悷�郁�b�V����ݒ�
		PtrDraw->SetMeshResource(L"Chara_R_MESH");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);

		PtrDraw->AddAnimation(L"Default", 60, 20, true, 20.0f);
		PtrDraw->AddAnimation(L"Hit", 30, 30, false, 30.0f);
		PtrDraw->ChangeCurrentAnimation(L"Default");

		PtrDraw->SetLighting(ShaderLighting::Real);
		PtrDraw->SetEmissive(Color4(0.6, 0.6, 0.6, 0));
		PtrDraw->SetDiffuse(Color4(0.4, 0.4, 0.4, 1));

		//�����������
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

		//��������
		SetAlphaActive(true);
		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetView()->GetTargetCamera());
		if (PtrCamera) {
			//LookAtCamera�ɒ��ڂ���I�u�W�F�N�g�i�v���C���[�j�̐ݒ�
			PtrCamera->SetTargetObject(GetThis<GameObject>());
		}
		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< StateMachine<Player> >(GetThis<Player>());
		//�ŏ��̃X�e�[�g��DefaultState�ɐݒ�
		m_StateMachine->ChangeState(DefaultState::Instance());
	}

	//�ړ��̌����𓾂�
	Vector3 Player::GetAngle() {
		Vector3 Angle(0, 0, 0);
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//���������̃X�s�[�h
				auto PtrTransform = GetComponent<Transform>();
				auto PtrCamera = GetStage()->GetView()->GetTargetCamera();
				//�i�s�����̌������v�Z
				Vector3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
				Front.y = 0;
				Front.Normalize();
				//�i�s������������̊p�x���Z�o
				float FrontAngle = atan2(Front.z, Front.x);
				//�R���g���[���̌����v�Z
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				//�R���g���[���̌�������p�x���v�Z
				float CntlAngle = atan2(-MoveX, MoveZ);
				//�g�[�^���̊p�x���Z�o
				float TotalAngle = FrontAngle + CntlAngle;
				//�p�x����x�N�g�����쐬
				Angle = Vector3(cos(TotalAngle), 0, sin(TotalAngle));
				//���K������
				Angle.Normalize();
				//Y���͕ω������Ȃ�
				Angle.y = 0;
			}
		}
		return Angle;
	}


	//�X�V
	void Player::OnUpdate() {
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̍X�V���s����(Execute()�֐����Ă΂��)
		m_StateMachine->Update();
		//�A�j���[�V�������X�V����
		auto PtrDraw = GetComponent<PNTBoneModelDraw>();
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		if (PtrDraw->UpdateAnimation(ElapsedTime) &&
			PtrDraw->GetCurrentAnimation() == L"Hit") {
			PtrDraw->ChangeCurrentAnimation(L"Default");
		}

	}

	void Player::OnCollision(vector<shared_ptr<GameObject>>& OtherVec) {
		if (GetStateMachine()->GetCurrentState() == JumpState::Instance()) {
			GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}
	//�^�[���̍ŏI�X�V��
	void Player::OnLastUpdate() {

		//������\��
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		FPS += L"\n";


		auto Pos = GetComponent<Transform>()->GetWorldMatrix().PosInMatrix();
		wstring PositionStr(L"Position:\t");
		PositionStr += L"X=" + Util::FloatToWStr(Pos.x, 6, Util::FloatModify::Fixed) + L",\t";
		PositionStr += L"Y=" + Util::FloatToWStr(Pos.y, 6, Util::FloatModify::Fixed) + L",\t";
		PositionStr += L"Z=" + Util::FloatToWStr(Pos.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring RididStr(L"Velocity:\t");
		auto Velocity = GetComponent<Rigidbody>()->GetVelocity();
		RididStr += L"X=" + Util::FloatToWStr(Velocity.x, 6, Util::FloatModify::Fixed) + L",\t";
		RididStr += L"Y=" + Util::FloatToWStr(Velocity.y, 6, Util::FloatModify::Fixed) + L",\t";
		RididStr += L"Z=" + Util::FloatToWStr(Velocity.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring GravStr(L"Gravity:\t");
		auto Grav = GetComponent<Gravity>()->GetGravity();
		GravStr += L"X=" + Util::FloatToWStr(Grav.x, 6, Util::FloatModify::Fixed) + L",\t";
		GravStr += L"Y=" + Util::FloatToWStr(Grav.y, 6, Util::FloatModify::Fixed) + L",\t";
		GravStr += L"Z=" + Util::FloatToWStr(Grav.z, 6, Util::FloatModify::Fixed) + L"\n";


		wstring GravityStr(L"GravityVelocity:\t");
		auto GravityVelocity = GetComponent<Gravity>()->GetGravityVelocity();
		GravityStr += L"X=" + Util::FloatToWStr(GravityVelocity.x, 6, Util::FloatModify::Fixed) + L",\t";
		GravityStr += L"Y=" + Util::FloatToWStr(GravityVelocity.y, 6, Util::FloatModify::Fixed) + L",\t";
		GravityStr += L"Z=" + Util::FloatToWStr(GravityVelocity.z, 6, Util::FloatModify::Fixed) + L"\n";

		wstring HitObjectStr(L"HitObject: ");
		if (GetComponent<Collision>()->GetHitObjectVec().size() > 0) {
			for (auto&v : GetComponent<Collision>()->GetHitObjectVec()) {
				HitObjectStr += Util::UintToWStr((UINT)v.get()) + L",";
			}
			HitObjectStr += L"\n";
		}
		else {
			HitObjectStr += L"NULL\n";
		}
		wstring statestr = L"JUMP: ";
		if (m_StateMachine->GetCurrentState() == DefaultState::Instance()) {
			statestr = L"DEFAULT\n";
		}
		wstring str = FPS + PositionStr + RididStr + GravStr + GravityStr + HitObjectStr + statestr ;
		//�����������
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
		
		auto PtrCol = GetComponent<CollisionSphere>();
		auto Group = GetStage()->GetSharedObjectGroup(L"MoveBox");
		auto GVec = Group->GetGroupVector();
		auto PtrTrans = GetComponent<Transform>();
		for (auto& v : GVec) {
			auto shptr = dynamic_pointer_cast<MoveBox>(v.lock());
			if (shptr) {
				auto ParCol = shptr->GetComponent<CollisionObb>();
				if (PtrCol->OnObjectTest(ParCol)) {
					//�ړ��{�b�N�X�ɏ���Ă�
					auto ParVelo = shptr->GetComponent<Action>()->GetVelocity();
					float ElapsedTime = App::GetApp()->GetElapsedTime();
					ParVelo *= ElapsedTime;
					auto Pos = PtrTrans->GetPosition();
					Pos += ParVelo;
					PtrTrans->SetPosition(Pos);
					return;
				}
			}
		}
	}

	//���[�V��������������֐��Q
	//�ړ����Č������ړ������ɂ���
	void Player::MoveRotationMotion() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vector3 Angle = GetAngle();
		//Transform
		auto PtrTransform = GetComponent<Transform>();
		//Rigidbody�����o��
		auto PtrRedit = GetComponent<Rigidbody>();
		//���݂̑��x�����o��
		auto Velo = PtrRedit->GetVelocity();
		//�ړI�n���ō����x���|���ċ��߂�
		auto Target = Angle * m_MaxSpeed;
		//�ړI�n�Ɍ��������߂ɗ͂̂�����������v�Z����
		//Force�̓t�H�[�X�ł���
		auto Force = Target - Velo;
		//y��0�ɂ���
		Force.y = 0;
		//�����x�����߂�
		auto Accel = Force / m_Mass;
		//�^�[�����Ԃ��|�������̂𑬓x�ɉ��Z����
		Velo += (Accel * ElapsedTime);
		//��������
		Velo *= m_Decel;
		//���x��ݒ肷��
		PtrRedit->SetVelocity(Velo);
		//��]�̌v�Z
		float YRot = PtrTransform->GetRotation().y;
		Quaternion Qt;
		Qt.Identity();
		if (Angle.Length() > 0.0f) {
			//�x�N�g����Y����]�ɕϊ�
			float PlayerAngle = atan2(Angle.x, Angle.z);
			Qt.RotationRollPitchYaw(0, PlayerAngle, 0);
			Qt.Normalize();
		}
		else {
			Qt.RotationRollPitchYaw(0, YRot, 0);
			Qt.Normalize();
		}
		//Transform
		PtrTransform->SetQuaternion(Qt);
	}


	//A�{�^���ŃW�����v����ǂ����𓾂�
	bool Player::IsJumpMotion() {
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//A�{�^���������ꂽ�u�ԂȂ�W�����v
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
				return true;
			}
		}
		return false;
	}
	//A�{�^���ŃW�����v����u�Ԃ̏���
	void Player::JumpMotion() {
		auto PtrTrans = GetComponent<Transform>();
		//�d��
		auto PtrGravity = GetComponent<Gravity>();
		//�W�����v�X�^�[�g
		Vector3 JumpVec(0.0f, 4.0f, 0);
		PtrGravity->StartJump(JumpVec);

		//�A�j���[�V������ύX����
		auto PtrDraw = GetComponent<PNTBoneModelDraw>();
		PtrDraw->ChangeCurrentAnimation(L"Hit");

	}
	//A�{�^���ŃW�����v���Ă���Ԃ̏���
	//�W�����v�I��������true��Ԃ�
	bool Player::JumpMoveMotion() {
		auto PtrTransform = GetComponent<Transform>();
		//�d��
		auto PtrGravity = GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0) {
			return true;
		}
		return false;
	}


	//--------------------------------------------------------------------------------------
	//	class DefaultState : public ObjState<Player>;
	//	�p�r: �ʏ�ړ�
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<DefaultState> DefaultState::Instance() {
		static shared_ptr<DefaultState> instance;
		if (!instance) {
			instance = shared_ptr<DefaultState>(new DefaultState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void DefaultState::Enter(const shared_ptr<Player>& Obj) {
		//�������Ȃ�
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void DefaultState::Execute(const shared_ptr<Player>& Obj) {
		Obj->MoveRotationMotion();
		if (Obj->IsJumpMotion()) {
			//Jump�{�^���ŃX�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(JumpState::Instance());
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void DefaultState::Exit(const shared_ptr<Player>& Obj) {
		//�������Ȃ�
	}


	//--------------------------------------------------------------------------------------
	//	class JumpState : public ObjState<Player>;
	//	�p�r: �W�����v���
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<JumpState> JumpState::Instance() {
		static shared_ptr<JumpState> instance;
		if (!instance) {
			instance = shared_ptr<JumpState>(new JumpState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void JumpState::Enter(const shared_ptr<Player>& Obj) {
		//�W�����v�����ړ��\�Ƃ���
		Obj->MoveRotationMotion();
		Obj->JumpMotion();
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void JumpState::Execute(const shared_ptr<Player>& Obj) {
		//�W�����v�����ړ��\�Ƃ���
		Obj->MoveRotationMotion();
		if (Obj->JumpMoveMotion()) {
			//�ʏ��Ԃɖ߂�
			Obj->GetStateMachine()->ChangeState(DefaultState::Instance());
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void JumpState::Exit(const shared_ptr<Player>& Obj) {
		//�������Ȃ�
	}


}
//end basecross
