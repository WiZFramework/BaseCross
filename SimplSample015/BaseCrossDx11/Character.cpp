/*!
@file Character.cpp
@brief �L�����N�^�[�Ȃǎ���
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	���ʎ���
	//--------------------------------------------------------------------------------------
	SquareObject::SquareObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, const Vector3& Scale, const Quaternion& Qt, const Vector3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	SquareObject::~SquareObject() {}

	void SquareObject::CreateBuffers() {
		float HelfSize = 0.5f;
		vector<VertexPositionNormalTexture> vertices = {
			{ VertexPositionNormalTexture(Vector3(-HelfSize, HelfSize, 0), Vector3(0, 0, -1.0f), Vector2(0.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vector3(HelfSize, HelfSize, 0), Vector3(0, 0, -1.0f), Vector2(1.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vector3(-HelfSize, -HelfSize, 0), Vector3(0, 0, -1.0f), Vector2(0.0f, 1.0f)) },
			{ VertexPositionNormalTexture(Vector3(HelfSize, -HelfSize, 0), Vector3(0, 0, -1.0f), Vector2(1.0f, 1.0f)) },

		};

		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2,
		};
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}


	void SquareObject::OnCreate() {
		CreateBuffers();
		//�e�N�X�`���̍쐬
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}
	void SquareObject::OnUpdate() {
	}
	void SquareObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//�s��̒�`
		Matrix4X4 World;
		//���[���h�s��̌���
		World.AffineTransformation(
			m_Scale,			//�X�P�[�����O
			Vector3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_SquareMesh,
			m_TextureResource,
			World,
			false
		);
	}



	//--------------------------------------------------------------------------------------
	///	������
	//--------------------------------------------------------------------------------------
	SphereObject::SphereObject(const shared_ptr<Scene> PtrScene,
		UINT Division, const wstring& TextureFileName, bool Trace, const Vector3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_Division(Division),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Pos(Pos),
		m_Velocity(0,0,0),
		m_Gravity(0,-9.8f,0),
		m_GravityVelocity(0,0,0),
		m_JumpLock(false)
	{}
	SphereObject::~SphereObject() {}

	SPHERE SphereObject::GetSPHERE()const {
		SPHERE sp;
		sp.m_Center = m_Pos;
		sp.m_Radius =  m_Scale.x * 0.5f;
		return sp;
	}

	void SphereObject::CollisionWithBoxes(const Vector3& BeforePos) {
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�Փ˔���
		auto ShPtrScene = m_Scene.lock();
		for (auto& v : ShPtrScene->GetBoxObjectVec()) {
			OBB Obb = v->GetOBB();
			SPHERE Sp = GetSPHERE();
			float HitTime;
			Vector3 CollisionVelosity = (m_Pos - BeforePos) / ElapsedTime;
			if (HitTest::CollisionTestSphereObb(Sp, CollisionVelosity, Obb, 0, ElapsedTime, HitTime)) {
				m_JumpLock = false;
				m_Pos = BeforePos + CollisionVelosity * HitTime;
				float SpanTime = ElapsedTime - HitTime;
				//m_Pos���������̂�SPHERE���Ď擾
				Sp = GetSPHERE();
				Vector3 HitPoint;
				//�ŋߐړ_�𓾂邽�߂̔���
				HitTest::SPHERE_OBB(Sp, Obb, HitPoint);
				//�Փ˖@����HitPoint��m_Pos���瓱��
				Vector3 Normal = m_Pos - HitPoint;
				Normal.Normalize();
				if (Vector3EX::AngleBetweenNormals(Normal, Vector3(0, 1, 0)) <= 0.01f) {
					//���ʂ̏�
					m_GravityVelocity = Vector3(0, 0, 0);
				}
				m_Velocity = Vector3EX::Slide(CollisionVelosity, Normal);
				m_Velocity.y = 0;
				//�Ō�ɏՓ˓_����]�������ԕ������X���C�h������
				m_Pos = m_Pos + m_Velocity * SpanTime;
				//������x�Փ˔���
				if (HitTest::SPHERE_OBB(Sp, Obb, HitPoint)) {
					//�Փ˂��Ă�����ǂ��o������
					Vector3 EscapeNormal = Sp.m_Center - HitPoint;
					EscapeNormal.Normalize();
					m_Pos = HitPoint + EscapeNormal * Sp.m_Radius;
				}
			}
		}
	}



	void SphereObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, m_Division,vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		//�e�N�X�`���̍쐬
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
		m_Scale = Vector3(1.0f, 1.0f, 1.0f);
		m_Qt.Identity();
	}
	void SphereObject::OnUpdate() {
		//1�O�̈ʒu������Ă���
		Vector3 BeforrPos = m_Pos;
		//�O��̃^�[������̌o�ߎ��Ԃ����߂�
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//�R���g���[���̎擾
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//�L�[�{�[�h�ƃ}�E�X�̎擾
		auto Key = App::GetApp()->GetInputDevice().GetKeyState();

		if (CntlVec[0].bConnected) {
			if (!m_JumpLock) {
				//A�{�^��
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					m_GravityVelocity = Vector3(0, 6.0f, 0);
					m_JumpLock = true;
				}
			}
			if (CntlVec[0].fThumbLX != 0) {
				m_Velocity.x = CntlVec[0].fThumbLX * 5.0f;
			}
			else {
				m_Velocity.x *= 0.1f;
				if (abs(m_Velocity.x) <= 0.01f) {
					m_Velocity.x = 0;
				}
			}
			if (CntlVec[0].fThumbLY != 0) {
				m_Velocity.z = CntlVec[0].fThumbLY * 5.0f;
			}
			else {
				m_Velocity.z *= 0.1f;
				if (abs(m_Velocity.z) <= 0.01f) {
					m_Velocity.z = 0;
				}
			}
		}
		m_Pos += (m_Velocity * ElapsedTime);
		m_GravityVelocity += m_Gravity * ElapsedTime;
		m_Pos += m_GravityVelocity * ElapsedTime;
		if (m_Pos.y <= 0.5f) {
			m_Pos.y = 0.5f;
			m_GravityVelocity = Vector3(0,0,0);
			m_JumpLock = false;
		}
		CollisionWithBoxes(BeforrPos);
	}
	void SphereObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//�s��̒�`
		Matrix4X4 World;
		//���[���h�s��̌���
		World.AffineTransformation(
			m_Scale,			//�X�P�[�����O
			Vector3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_SphereMesh,
			m_TextureResource,
			World,
			true
		);
	}

	//--------------------------------------------------------------------------------------
	///	�{�b�N�X����
	//--------------------------------------------------------------------------------------
	BoxObject::BoxObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, bool Trace, 
		const Vector3& Scale,
		const Quaternion& Qt,
		const Vector3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	BoxObject::~BoxObject() {}

	OBB BoxObject::GetOBB()const {
		Matrix4X4 World;
		//���[���h�s��̌���
		World.AffineTransformation(
			m_Scale,			//�X�P�[�����O
			Vector3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		OBB obb(Vector3(1.0f, 1.0f, 1.0f), World);
		return obb;
	}


	void BoxObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		//���b�V���̍쐬�i�ύX�ł��Ȃ��j
		m_BoxMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		//�e�N�X�`���̍쐬
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}
	void BoxObject::OnUpdate() {
	}

	void BoxObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//�s��̒�`
		Matrix4X4 World;
		//���[���h�s��̌���
		World.AffineTransformation(
			m_Scale,			//�X�P�[�����O
			Vector3(0, 0, 0),		//��]�̒��S�i�d�S�j
			m_Qt,				//��]�p�x
			m_Pos				//�ʒu
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_BoxMesh,
			m_TextureResource,
			World,
			true
		);
	}

	//--------------------------------------------------------------------------------------
	///	PNT���_�I�u�W�F�N�g�̕`��N���X
	//--------------------------------------------------------------------------------------
	PNTDrawObject::PNTDrawObject(const shared_ptr<Scene> PtrScene) :
		m_Scene(PtrScene)
	{}
	PNTDrawObject::~PNTDrawObject() {}

	void PNTDrawObject::AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
		const shared_ptr<TextureResource>& TextureRes,
		const Matrix4X4& WorldMat,
		bool Trace) {
		DrawObject Obj;
		Obj.m_MeshRes = MeshRes;
		Obj.m_TextureRes = TextureRes;
		Obj.m_WorldMatrix = WorldMat;
		Obj.m_Trace = Trace;
		m_DrawObjectVec.push_back(Obj);
	}

	void PNTDrawObject::OnUpdate() {
		m_DrawObjectVec.clear();
	}

	void PNTDrawObject::OnDraw() {
		if (m_Scene.expired()) {
			//�V�[���������Ȃ烊�^�[��
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//�e�I�u�W�F�N�g���ʏ���
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�f�v�X�X�e���V���X�e�[�g
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//�T���v���[
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//�s��̒�`
		Matrix4X4 View, Proj;
		//���C�e�B���O
		Vector4 LightDir;
		auto ShPtrScene = m_Scene.lock();
		ShPtrScene->GetViewProjMatrix(View, Proj);
		ShPtrScene->GetLightDir(LightDir);
		//�r���[�s��̌���
		//�]�u����
		View.Transpose();
		//�ˉe�s��̌���
		//�]�u����
		Proj.Transpose();
		//�R���X�^���g�o�b�t�@�̏���
		PNTStaticConstantBuffer sb;
		sb.View = View;
		sb.Projection = Proj;
		sb.LightDir = LightDir;
		//�f�B�t���[�Y
		sb.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		//�G�~�b�V�u���Z�͍s��Ȃ��B
		sb.Emissive = Color4(0, 0, 0, 0);
		//�ʏ���
		for (auto& v : m_DrawObjectVec) {
			//�]�u����
			v.m_WorldMatrix.Transpose();
			//���[���h�s��̌���
			sb.World = v.m_WorldMatrix;
			//�R���X�^���g�o�b�t�@�̍X�V
			pD3D11DeviceContext->UpdateSubresource(CBPNTStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBPNTStatic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//���_�V�F�[�_�ɓn��
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//�s�N�Z���V�F�[�_�ɓn��
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//���_�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, v.m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pD3D11DeviceContext->IASetIndexBuffer(v.m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//�e�N�X�`���̐ݒ�
			ID3D11ShaderResourceView* pNull[1] = { 0 };
			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.m_TextureRes->GetShaderResourceView().GetAddressOf());
			//�u�����h�X�e�[�g
			if (v.m_Trace) {
				//��������
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				//���������̏ꍇ�́A���X�^���C�U�X�e�[�g��ύX����2��`��
				//���X�^���C�U�X�e�[�g�i���ʕ`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//�`��
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
				//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//�`��
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//�����������Ȃ�
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
				//���X�^���C�U�X�e�[�g�i�\�ʕ`��j
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//�`��
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
		}
		//��n��
		Dev->InitializeStates();
	}



}
//end basecross