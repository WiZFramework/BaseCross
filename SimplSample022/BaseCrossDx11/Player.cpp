/*!
@file Player.cpp
@brief プレイヤー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	球体のプレイヤー実体
	//--------------------------------------------------------------------------------------
	Player::Player(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, bool Trace, const Vec3& Pos) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Trace(Trace),
		m_BaseY(0.25f / 2.0f),
		m_Posision(Pos),
		m_JumpLock(false)
	{}
	Player::~Player() {}

	Vec3 Player::GetMoveVector(){
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
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
				Vec3 Front = body.m_Pos - CameraEye;
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

	Vec3 Player::GetPosition() {
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		return body.m_Pos;
	}

	void Player::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		//メッシュの作成（変更できない）
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//タグの追加
		AddTag(L"Player");
		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = Vec3(0.25f);
		body.m_Quat = Quat();
		body.m_Pos = m_Posision;
		body.m_CollType = CollType::typeSPHERE;
		body.SetToBefore();

		PtrGameStage->AddRigidbody(body);

	}
	void Player::OnUpdate() {
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (!m_JumpLock) {
				//Aボタン
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					body.m_BeforePos.y += 0.01f;
					body.m_Pos.y += 0.01f;
					body.m_Velocity += Vec3(0, 4.0f, 0);
					m_JumpLock = true;
				}
			}
			Vec3 Direction = GetMoveVector();
			if (length(Direction) < 0.1f) {
				body.m_Velocity.x *= 0.9f;
				body.m_Velocity.z *= 0.9f;
			}
			else {
				//フォースで変更する場合は以下のように記述
				//body.m_Force += Direction * 10.0f;
				//速度で変更する場合は以下のように記述
				body.m_Velocity += Direction * 0.5f;
				Vec2 TempVelo(body.m_Velocity.x, body.m_Velocity.z);
				TempVelo = XMVector2ClampLength(TempVelo, 0, 5.0f);
				body.m_Velocity.x = TempVelo.x;
				body.m_Velocity.z = TempVelo.y;
			}
		}
		body.m_Force += body.m_Gravity * body.m_Mass;
	}

	void Player::OnUpdate2() {
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		if (body.m_Pos.y <= m_BaseY) {
			body.m_Pos.y = m_BaseY;
			body.m_Velocity.y = 0;
			m_JumpLock = false;
		}
		auto& StateVec = GetStage<GameStage>()->GetCollisionStateVec();
		for (auto& v : StateVec) {
			if (v.m_Src == &body) {
				Vec3 Normal = v.m_SrcHitNormal;
				Normal.normalize();
				Vec4 v = (Vec4)XMVector3AngleBetweenNormals(Vec3(0, 1, 0), Normal);
				if (v.x < 0.1f) {
					m_JumpLock = false;
					break;
				}
			}
			if (v.m_Dest == &body) {
				Vec3 Normal = v.m_SrcHitNormal;
				Normal.normalize();
				Vec4 v = (Vec4)XMVector3AngleBetweenNormals(Vec3(0, 1, 0), Normal);
				if (v.x < 0.1f) {
					m_JumpLock = false;
					break;
				}
			}
		}
		auto LenVec = body.m_Pos - body.m_BeforePos;
		LenVec.y = 0;
		auto Len = LenVec.length();
		if (Len > 0) {
			Vec3 Cross = cross(Vec3(0, 1, 0), LenVec);
			Quat Span(Cross, Len / 0.5f);
			body.m_Quat *= Span;
		}
	}

	void Player::OnDraw() {
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto PtrGameStage = GetStage<GameStage>();
		//ワールド行列の決定
		Mat4x4 World;
		World.affineTransformation(body.m_Scale, Vec3(0, 0, 0),
			body.m_Quat, body.m_Pos);
		PtrGameStage->GetPNTDrawObject()->AddDrawMesh(
			m_SphereMesh,
			TexPtr,
			World,
			true
		);
	}


	//--------------------------------------------------------------------------------------
	///	カプセルのプレイヤー実体
	//--------------------------------------------------------------------------------------
	CapsulePlayer::CapsulePlayer(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, bool Trace, const Vec3& Pos) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Trace(Trace),
		m_BaseY(0.25f),
		m_Posision(Pos),
		m_JumpLock(false)
	{}
	CapsulePlayer::~CapsulePlayer() {}

	Vec3 CapsulePlayer::GetMoveVector() {
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
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
				Vec3 Front = body.m_Pos - CameraEye;
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

	Vec3 CapsulePlayer::GetPosition() {
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		return body.m_Pos;
	}

	void CapsulePlayer::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCapsule(1.0f, Vec3(0, -0.5f, 0), Vec3(0, 0.5f, 0), 18, vertices, indices);
		//メッシュの作成（変更できない）
		m_CapsuleMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//タグの追加
		AddTag(L"Player");
		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = Vec3(0.25f);
		body.m_Quat = Quat();
		body.m_Pos = m_Posision;
		body.m_CollType = CollType::typeCAPSULE;
		body.SetToBefore();

		PtrGameStage->AddRigidbody(body);

	}
	void CapsulePlayer::OnUpdate() {
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (!m_JumpLock) {
				//Aボタン
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					body.m_BeforePos.y += 0.01f;
					body.m_Pos.y += 0.01f;
					body.m_Velocity += Vec3(0, 4.0f, 0);
					m_JumpLock = true;
				}
			}
			Vec3 Direction = GetMoveVector();
			if (length(Direction) < 0.1f) {
				body.m_Velocity.x *= 0.9f;
				body.m_Velocity.z *= 0.9f;
			}
			else {
				//フォースで変更する場合は以下のように記述
				//body.m_Force += Direction * 10.0f;
				//速度で変更する場合は以下のように記述
				body.m_Velocity += Direction * 0.5f;
				Vec2 TempVelo(body.m_Velocity.x, body.m_Velocity.z);
				TempVelo = XMVector2ClampLength(TempVelo, 0, 5.0f);
				body.m_Velocity.x = TempVelo.x;
				body.m_Velocity.z = TempVelo.y;
			}
		}
		body.m_Force += body.m_Gravity * body.m_Mass;
	}

	void CapsulePlayer::OnUpdate2() {
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		if (body.m_Pos.y <= m_BaseY) {
			body.m_Pos.y = m_BaseY;
			body.m_Velocity.y = 0;
			m_JumpLock = false;
		}
		auto& StateVec = GetStage<GameStage>()->GetCollisionStateVec();
		for (auto& v : StateVec) {
			if (v.m_Src == &body) {
				Vec3 Normal = v.m_SrcHitNormal;
				Normal.normalize();
				Vec4 v = (Vec4)XMVector3AngleBetweenNormals(Vec3(0, 1, 0), Normal);
				if (v.x < 0.1f) {
					m_JumpLock = false;
					break;
				}
			}
			if (v.m_Dest == &body) {
				Vec3 Normal = v.m_SrcHitNormal;
				Normal.normalize();
				Vec4 v = (Vec4)XMVector3AngleBetweenNormals(Vec3(0, 1, 0), Normal);
				if (v.x < 0.1f) {
					m_JumpLock = false;
					break;
				}
			}
		}
		//回転の更新
		//Velocityの値で、回転を変更する
		Vec3 Temp = body.m_Velocity;
		Temp.normalize();
		float ToAngle = atan2(Temp.x, Temp.z);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
		Qt.normalize();
		//現在と目標を補間
		body.m_Quat = XMQuaternionSlerp(body.m_Quat, Qt, 0.1f);
	}

	void CapsulePlayer::OnDraw() {
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto PtrGameStage = GetStage<GameStage>();
		//ワールド行列の決定
		Mat4x4 World;
		World.affineTransformation(body.m_Scale, Vec3(0, 0, 0),
			body.m_Quat, body.m_Pos);
		PtrGameStage->GetPNTDrawObject()->AddDrawMesh(
			m_CapsuleMesh,
			TexPtr,
			World,
			true
		);
	}

	//--------------------------------------------------------------------------------------
	///	OBBのプレイヤー実体
	//--------------------------------------------------------------------------------------
	ObbPlayer::ObbPlayer(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName, bool Trace, const Vec3& Pos) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName),
		m_Trace(Trace),
		m_BaseY(0.125f),
		m_Posision(Pos),
		m_JumpLock(false)
	{}
	ObbPlayer::~ObbPlayer() {}

	Vec3 ObbPlayer::GetMoveVector() {
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
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
				Vec3 Front = body.m_Pos - CameraEye;
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

	Vec3 ObbPlayer::GetPosition() {
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		return body.m_Pos;
	}

	void ObbPlayer::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		//メッシュの作成（変更できない）
		m_ObbMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//タグの追加
		AddTag(L"Player");
		//Rigidbodyの初期化
		auto PtrGameStage = GetStage<GameStage>();
		Rigidbody body;
		body.m_Owner = GetThis<GameObject>();
		body.m_Mass = 1.0f;
		body.m_Scale = Vec3(0.125f,0.25f,0.25f);
		body.m_Quat = Quat();
		body.m_Pos = m_Posision;
		body.m_CollType = CollType::typeOBB;
		body.SetToBefore();

		PtrGameStage->AddRigidbody(body);

	}
	void ObbPlayer::OnUpdate() {
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (!m_JumpLock) {
				//Aボタン
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					body.m_BeforePos.y += 0.01f;
					body.m_Pos.y += 0.01f;
					body.m_Velocity += Vec3(0, 4.0f, 0);
					m_JumpLock = true;
				}
			}
			Vec3 Direction = GetMoveVector();
			if (length(Direction) < 0.1f) {
				body.m_Velocity.x *= 0.9f;
				body.m_Velocity.z *= 0.9f;
			}
			else {
				//フォースで変更する場合は以下のように記述
				//body.m_Force += Direction * 10.0f;
				//速度で変更する場合は以下のように記述
				body.m_Velocity += Direction * 0.5f;
				Vec2 TempVelo(body.m_Velocity.x, body.m_Velocity.z);
				TempVelo = XMVector2ClampLength(TempVelo, 0, 5.0f);
				body.m_Velocity.x = TempVelo.x;
				body.m_Velocity.z = TempVelo.y;
			}
		}
		body.m_Force += body.m_Gravity * body.m_Mass;
	}

	void ObbPlayer::OnUpdate2() {
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		if (body.m_Pos.y <= m_BaseY) {
			body.m_Pos.y = m_BaseY;
			body.m_Velocity.y = 0;
			m_JumpLock = false;
		}
		auto& StateVec = GetStage<GameStage>()->GetCollisionStateVec();
		for (auto& v : StateVec) {
			if (v.m_Src == &body) {
				Vec3 Normal = v.m_SrcHitNormal;
				Normal.normalize();
				Vec4 v = (Vec4)XMVector3AngleBetweenNormals(Vec3(0, 1, 0), Normal);
				if (v.x < 0.1f) {
					m_JumpLock = false;
					break;
				}
			}
			if (v.m_Dest == &body) {
				Vec3 Normal = v.m_SrcHitNormal;
				Normal.normalize();
				Vec4 v = (Vec4)XMVector3AngleBetweenNormals(Vec3(0, 1, 0), Normal);
				if (v.x < 0.1f) {
					m_JumpLock = false;
					break;
				}
			}
		}
		//回転の更新
		//Velocityの値で、回転を変更する
		Vec3 Temp = body.m_Velocity;
		Temp.normalize();
		float ToAngle = atan2(Temp.x, Temp.z);
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(0, ToAngle, 0));
		Qt.normalize();
		//現在と目標を補間
		body.m_Quat = XMQuaternionSlerp(body.m_Quat, Qt, 0.1f);
	}

	void ObbPlayer::OnDraw() {
		auto& body = GetStage<GameStage>()->GetOwnRigidbody(GetThis<GameObject>());
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		auto PtrGameStage = GetStage<GameStage>();
		//ワールド行列の決定
		Mat4x4 World;
		World.affineTransformation(body.m_Scale, Vec3(0, 0, 0),
			body.m_Quat, body.m_Pos);
		PtrGameStage->GetPNTDrawObject()->AddDrawMesh(
			m_ObbMesh,
			TexPtr,
			World,
			true
		);
	}



}
//end basecross
