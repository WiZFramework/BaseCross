/*!
@file Player.cpp
@brief プレイヤー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	プレイヤー実体
	//--------------------------------------------------------------------------------------
	Player::Player(const shared_ptr<Stage>& StagePtr,
		UINT Division, const wstring& TextureResName, bool Trace, const Vec3& Pos) :
		GameObject(StagePtr),
		m_Division(Division),
		m_TextureResName(TextureResName),
		m_Trace(Trace),
		m_Scale(0.25f, 0.25f, 0.25f),
		m_BaseY(m_Scale.y / 2.0f),
		m_Qt(),
		m_Pos(Pos),
		m_Velocity(0, 0, 0),
		m_Gravity(0, -9.8f, 0),
		m_GravityVelocity(0, 0, 0),
		m_JumpLock(false),
		m_BeforePos(Pos),
		m_Mass(1.0f)
	{}
	Player::~Player() {}

	Vec3 Player::GetMoveVector() const {
		Vec3 Angle(0, 0, 0);
		auto PtrGameStage = GetStage<GameStage>();
		Vec3 CameraEye, CameraAt;
		PtrGameStage->GetCamera().GetCameraEyeAt(CameraEye, CameraAt);

		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//動いた時のスピード
										//進行方向の向きを計算
				Vec3 Front = m_Pos - CameraEye;
				Front.y = 0;
				Front.normalize();
				//進行方向向きからの角度を算出
				float FrontAngle = atan2(Front.z, Front.x);
				//コントローラの向き計算
				float MoveX = CntlVec[0].fThumbLX;
				float MoveZ = CntlVec[0].fThumbLY;
				Vec2 MoveVec(MoveX, MoveZ);
				float MoveSize = length(MoveVec);
				//コントローラの向きから角度を計算
				float CntlAngle = atan2(-MoveX, MoveZ);
				//トータルの角度を算出
				float TotalAngle = FrontAngle + CntlAngle;
				//角度からベクトルを作成
				Angle = Vec3(cos(TotalAngle), 0, sin(TotalAngle));
				//正規化する
				Angle.normalize();
				//移動サイズを設定。
				Angle *= MoveSize;
				//Y軸は変化させない
				Angle.y = 0;
			}
		}
		return Angle;
	}


	SPHERE Player::GetSPHERE()const {
		SPHERE sp;
		sp.m_Center = (XMFLOAT3)m_Pos;
		sp.m_Radius = m_Scale.y * 0.5f;
		return sp;
	}

	void Player::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, m_Division, vertices, indices);
		//メッシュの作成（変更できない）
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//タグの追加
		AddTag(L"Player");
	}
	void Player::OnUpdate() {
		//1つ前の位置を取っておく
		m_BeforePos = m_Pos;
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (!m_JumpLock) {
				//Aボタン
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					m_BeforePos.y += 0.01f;
					m_Pos.y += 0.01f;
					m_GravityVelocity = Vec3(0, 4.0f, 0);
					m_JumpLock = true;
				}
			}
			Vec3 Direction = GetMoveVector();
			if (length(Direction) < 0.1f) {
				m_Velocity *= 0.9f;
			}
			else {
				m_Velocity = Direction * 5.0f;
			}
		}
		m_Pos += (m_Velocity * ElapsedTime);
		m_GravityVelocity += m_Gravity * ElapsedTime;
		m_Pos += m_GravityVelocity * ElapsedTime;
		if (m_Pos.y <= m_BaseY) {
			m_Pos.y = m_BaseY;
			m_GravityVelocity = Vec3(0, 0, 0);
			m_JumpLock = false;
		}
		OnRotation();
	}


	void Player::RotToHead(float LerpFact) {
		if (LerpFact <= 0.0f) {
			//補間係数が0以下なら何もしない
			return;
		}
		//回転の更新
		//Velocityの値で、回転を変更する
		Vec3 Temp = m_Velocity;
		Temp.normalize();
		float ToAngle = atan2(Temp.x, Temp.z);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
		Qt.normalize();
		//現在と目標を補間
		if (LerpFact >= 1.0f) {
			m_Qt = Qt;
		}
		else {
			m_Qt = XMQuaternionSlerp(m_Qt, Qt, LerpFact);
		}
	}

	void Player::OnRotation() {
		//回転
		RotToHead(0.1f);
	}



	void Player::OnDraw() {
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto PtrGameStage = GetStage<GameStage>();
		//ワールド行列の決定
		Mat4x4 World;
		World.affineTransformation(m_Scale, Vec3(0, 0, 0),
			m_Qt, m_Pos);
		PtrGameStage->GetPNTDrawObject()->AddDrawMesh(
			m_SphereMesh,
			TexPtr,
			World,
			true
		);
	}


}
//end basecross
