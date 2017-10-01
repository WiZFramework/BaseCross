/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	平面実体
	//--------------------------------------------------------------------------------------
	SquareObject::SquareObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	SquareObject::~SquareObject() {}

	void SquareObject::CreateBuffers(float WrapX, float WrapY) {
		float HelfSize = 0.5f;
		vector<VertexPositionNormalTexture> vertices = {
			{ VertexPositionNormalTexture(Vec3(-HelfSize, HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(HelfSize, HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(WrapX, 0.0f)) },
			{ VertexPositionNormalTexture(Vec3(-HelfSize, -HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(0.0f, WrapY)) },
			{ VertexPositionNormalTexture(Vec3(HelfSize, -HelfSize, 0), Vec3(0, 0, -1.0f), Vec2(WrapX, WrapY)) },

		};

		vector<uint16_t> indices = {
			0, 1, 2,
			1, 3, 2,
		};
		//メッシュの作成（変更できない）
		m_SquareMesh = MeshResource::CreateMeshResource(vertices, indices, false);
	}


	void SquareObject::OnCreate() {
		CreateBuffers(m_Scale.x, m_Scale.y);
		//テクスチャの作成
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}
	void SquareObject::OnUpdate() {
	}
	void SquareObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//行列の定義
		Mat4x4 World;
		//ワールド行列の決定
		World.affineTransformation(
			m_Scale,			//スケーリング
			Vec3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_SquareMesh,
			m_TextureResource,
			World,
			false,
			true
		);
	}


	//--------------------------------------------------------------------------------------
	///	シリンダー実体
	//--------------------------------------------------------------------------------------
	CylinderObject::CylinderObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	CylinderObject::~CylinderObject() {}

	CYLINDER CylinderObject::GetCYLINDER()const {
		CYLINDER cy;
		cy.m_Radius = m_Scale.x;
		float halfY = m_Scale.y * 0.5f;
		cy.m_PointTop = bsm::Vec3(m_Pos.x, m_Pos.y + halfY, m_Pos.z);
		cy.m_PointBottom = bsm::Vec3(m_Pos.x, m_Pos.y - halfY, m_Pos.z);
		return cy;
	}


	void CylinderObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCylinder(1.0f, 2.0f,18, vertices, indices);
		//メッシュの作成（変更できない）
		m_CylinderMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//テクスチャの作成
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}
	void CylinderObject::OnUpdate() {
	}

	void CylinderObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//行列の定義
		Mat4x4 World;
		//ワールド行列の決定
		World.affineTransformation(
			m_Scale,			//スケーリング
			Vec3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_CylinderMesh,
			m_TextureResource,
			World,
			true
		);
	}






	//--------------------------------------------------------------------------------------
	///	球実体
	//--------------------------------------------------------------------------------------
	SphereObject::SphereObject(const shared_ptr<Scene> PtrScene,
		UINT Division, const wstring& TextureFileName, bool Trace, const Vec3& Pos) :
		m_Scene(PtrScene),
		ObjectInterface(),
		ShapeInterface(),
		m_Division(Division),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(0.25f, 0.25f, 0.25f),
		m_BaseY(m_Scale.y / 2.0f),
		m_Qt(),
		m_Pos(Pos),
		m_Velocity(0,0,0),
		m_Gravity(0,-9.8f,0),
		m_GravityVelocity(0,0,0),
		m_JumpLock(false),
		m_BeforePos(Pos),
		m_Mass(1.0f)
	{}
	SphereObject::~SphereObject() {}

	Vec3 SphereObject::GetMoveVector() const {
		Vec3 Angle(0, 0, 0);
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return Angle;
		}
		Vec3 CameraEye, CameraAt;
		ShPtrScene->GetCameraEyeAt(CameraEye, CameraAt);

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
				float MoveSize = MoveVec.length();
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




	SPHERE SphereObject::GetSPHERE()const {
		SPHERE sp;
		sp.m_Center = m_Pos;
		sp.m_Radius =  m_Scale.y * 0.5f;
		return sp;
	}

	void SphereObject::CollisionWithCylinder(const Vec3& BeforePos) {
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//衝突判定
		auto ShPtrScene = m_Scene.lock();
		auto cyOb = ShPtrScene->GetCylinderObject();
		CYLINDER cy = cyOb->GetCYLINDER();
		SPHERE Sp = GetSPHERE();
		Sp.m_Center = BeforePos;
		float HitTime;
		//相手の速度
		Vec3 DestVelocity(0, 0, 0);
		Vec3 SrcVelocity = m_Pos - BeforePos;
		Vec3 CollisionVelosity = (SrcVelocity - DestVelocity) / ElapsedTime;
		if (HitTest::CollisionTestSphereCylinder(Sp, CollisionVelosity, cy, 0, ElapsedTime, HitTime)) {
			m_JumpLock = false;
			m_Pos = BeforePos + CollisionVelosity * HitTime;
			float SpanTime = ElapsedTime - HitTime;
			//m_Posが動いたのでSPHEREを再取得
			Sp = GetSPHERE();
			Vec3 HitPoint;
			//最近接点を得るための判定
			HitTest::SPHERE_CYLINDER(Sp, cy, HitPoint);
			//衝突法線をHitPointとm_Posから導く
			Vec3 Normal = m_Pos - HitPoint;
			Normal.normalize();
			Vec3 angle(XMVector3AngleBetweenNormals(Normal, Vec3(0, 1, 0)));
			if (angle.x <= 0.01f) {
				//平面の上
				m_GravityVelocity = Vec3(0, 0, 0);
			}
			else {
				//重力をスライドさせて設定する
				//これで、斜めのボックスを滑り落ちるようになる
				m_GravityVelocity = ProjUtil::Slide(m_GravityVelocity, Normal);
			}
			//速度をスライドさせて設定する
			m_Velocity = ProjUtil::Slide(m_Velocity, Normal);
			//Y方向は重力に任せる
			m_Velocity.y = 0;
			//最後に衝突点から余った時間分だけ新しい値で移動させる
			m_Pos = m_Pos + m_Velocity * SpanTime;
			m_Pos = m_Pos + m_GravityVelocity * SpanTime;
			//もう一度衝突判定
			//m_Posが動いたのでSPHEREを再取得
			Sp = GetSPHERE();
			if (HitTest::SPHERE_CYLINDER(Sp, cy, HitPoint)) {
				//衝突していたら追い出し処理
				Vec3 EscapeNormal = Sp.m_Center - HitPoint;
				EscapeNormal.normalize();
				m_Pos = HitPoint + EscapeNormal * Sp.m_Radius;
			}
		}
	}


	void SphereObject::CollisionWithBoxes(const Vec3& BeforePos) {
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//衝突判定
		auto ShPtrScene = m_Scene.lock();
		for (auto& v : ShPtrScene->GetBoxVec()) {
			OBB Obb = v->GetOBB();
			SPHERE Sp = GetSPHERE();
			Sp.m_Center = BeforePos;
			float HitTime;
			//相手の速度
			Vec3 DestVelocity(0, 0, 0);
			auto MovBoxPtr = dynamic_pointer_cast<MoveBoxObject>(v);
			if (MovBoxPtr) {
				DestVelocity = MovBoxPtr->GetPosition() - MovBoxPtr->GetBeforePos();
				Obb.m_Center = MovBoxPtr->GetBeforePos();
			}
			Vec3 SrcVelocity = m_Pos - BeforePos;

			Vec3 CollisionVelosity = (SrcVelocity - DestVelocity) / ElapsedTime;
			if (HitTest::CollisionTestSphereObb(Sp, CollisionVelosity, Obb, 0, ElapsedTime, HitTime)) {
				m_JumpLock = false;
				m_Pos = BeforePos + CollisionVelosity * HitTime;
				float SpanTime = ElapsedTime - HitTime;
				//m_Posが動いたのでSPHEREを再取得
				Sp = GetSPHERE();
				Vec3 HitPoint;
				//最近接点を得るための判定
				HitTest::SPHERE_OBB(Sp, Obb, HitPoint);
				//衝突法線をHitPointとm_Posから導く
				Vec3 Normal = m_Pos - HitPoint;
				Normal.normalize();
				Vec3 angle(XMVector3AngleBetweenNormals(Normal, Vec3(0, 1, 0)));
				if (angle.x <= 0.01f) {
					//平面の上
					m_GravityVelocity = Vec3(0, 0, 0);
				}
				else {
					//重力をスライドさせて設定する
					//これで、斜めのボックスを滑り落ちるようになる
					m_GravityVelocity = ProjUtil::Slide(m_GravityVelocity, Normal);
				}
				if (MovBoxPtr) {
					//お互いに反発する
					Vec3 TgtVelo = CollisionVelosity * 0.5f;
					if (TgtVelo.length() < 1.0f) {
						//衝突時の速度が小さかったら、速度を作り出す
						TgtVelo = MovBoxPtr->GetPosition() - m_Pos;
						TgtVelo.normalize();
						TgtVelo *= 2.0f;
					}
					Vec3 DestVelo(XMVector3Reflect(-TgtVelo, Normal));
					DestVelo.y = 0;
					MovBoxPtr->SetVelocity(DestVelo);
					//速度を反発させて設定する
					m_Velocity = XMVector3Reflect(TgtVelo, -Normal);
				}
				else {
					//速度をスライドさせて設定する
					m_Velocity = ProjUtil::Slide(m_Velocity, Normal);
				}
				//Y方向は重力に任せる
				m_Velocity.y = 0;
				//最後に衝突点から余った時間分だけ新しい値で移動させる
				m_Pos = m_Pos + m_Velocity * SpanTime;
				m_Pos = m_Pos + m_GravityVelocity * SpanTime;
				//もう一度衝突判定
				//m_Posが動いたのでSPHEREを再取得
				Sp = GetSPHERE();
				if (HitTest::SPHERE_OBB(Sp, Obb, HitPoint)) {
					//衝突していたら追い出し処理
					Vec3 EscapeNormal = Sp.m_Center - HitPoint;
					EscapeNormal.normalize();
					m_Pos = HitPoint + EscapeNormal * Sp.m_Radius;
				}
			}
		}
	}



	void SphereObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, m_Division,vertices, indices);
		//メッシュの作成（変更できない）
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//テクスチャの作成
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}
	void SphereObject::OnUpdate() {
		//1つ前の位置を取っておく
		m_BeforePos = m_Pos;
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
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
			if (Direction.length() < 0.1f) {
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
	}

	void SphereObject::OnCollision() {
		//衝突判定
		CollisionWithBoxes(m_BeforePos);
		CollisionWithCylinder(m_BeforePos);
	}

	void SphereObject::RotToHead(float LerpFact) {
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

	void SphereObject::OnRotation() {
		//回転
		RotToHead(0.1f);
	}



	void SphereObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//行列の定義
		Mat4x4 World;
		//ワールド行列の決定
		World.affineTransformation(
			m_Scale,			//スケーリング
			Vec3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_SphereMesh,
			m_TextureResource,
			World,
			true
		);
	}

	//--------------------------------------------------------------------------------------
	///	固定のボックス実体
	//--------------------------------------------------------------------------------------
	BoxObject::BoxObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, bool Trace, 
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Pos) :
		BoxBase(),
		m_Scene(PtrScene),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	BoxObject::~BoxObject() {}

	OBB BoxObject::GetOBB()const {
		Mat4x4 World;
		//ワールド行列の決定
		World.affineTransformation(
			m_Scale,			//スケーリング
			Vec3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		OBB obb(Vec3(1.0f, 1.0f, 1.0f), World);
		return obb;
	}


	void BoxObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		//メッシュの作成（変更できない）
		m_BoxMesh = MeshResource::CreateMeshResource(vertices, indices, false);

		//テクスチャの作成
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}
	void BoxObject::OnUpdate() {
	}

	void BoxObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//行列の定義
		Mat4x4 World;
		//ワールド行列の決定
		World.affineTransformation(
			m_Scale,			//スケーリング
			Vec3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_BoxMesh,
			m_TextureResource,
			World,
			true
		);
	}

	//--------------------------------------------------------------------------------------
	///	移動ボックス実体
	//--------------------------------------------------------------------------------------
	MoveBoxObject::MoveBoxObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, bool Trace,
		const Vec3& Scale,
		const Quat& Qt,
		const Vec3& Pos) :
		BoxBase(),
		m_Scene(PtrScene),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos),
		m_Velocity(0,0,0),
		m_Mass(1.0f),
		m_Speed(4.0f),
		m_BeforePos(Pos)
	{}
	MoveBoxObject::~MoveBoxObject() {}

	OBB MoveBoxObject::GetOBB()const {
		Mat4x4 World;
		//ワールド行列の決定
		World.affineTransformation(
			m_Scale,			//スケーリング
			Vec3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		OBB obb(Vec3(1.0f, 1.0f, 1.0f), World);
		return obb;
	}


	void MoveBoxObject::OnCreate() {
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		//メッシュの作成（変更できない）
		m_MoveBoxMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//テクスチャの作成
		m_TextureResource = ObjectFactory::Create<TextureResource>(m_TextureFileName, L"WIC");
	}

	void MoveBoxObject::UpdateVelosity() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//フォース（力）
		Vec3 Force(0, 0, 0);
		//プレイヤーを向く方向ベクトル
		Vec3 ToPlayerVec = 
			ShPtrScene->GetSphereObject()->GetPosition() - m_Pos;
		//縦方向は計算しない
		ToPlayerVec.y = 0;
		ToPlayerVec *= m_Speed;
		//力を掛ける方向を決める
		Force = ToPlayerVec - m_Velocity;
		//力と質量から加速を求める
		Vec3 Accel = Force / m_Mass;
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//速度を加速する
		m_Velocity += Accel * ElapsedTime;
	}

	void MoveBoxObject::CollisionWithBoxes(const Vec3& BeforePos) {
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//衝突判定
		auto ShPtrScene = m_Scene.lock();
		for (auto& v : ShPtrScene->GetBoxVec()) {
			if (v == GetThis<BoxBase>()) {
				//相手が自分自身なら処理しない
				continue;
			}
			OBB DestObb = v->GetOBB();
			OBB SrcObb = GetOBB();
			SrcObb.m_Center = BeforePos;
			float HitTime;
			Vec3 CollisionVelosity = (m_Pos - BeforePos) / ElapsedTime;
			if (HitTest::CollisionTestObbObb(SrcObb, CollisionVelosity, DestObb, 0, ElapsedTime, HitTime)) {
				m_Pos = BeforePos + CollisionVelosity * HitTime;
				float SpanTime = ElapsedTime - HitTime;
				//m_Posが動いたのでOBBを再取得
				SrcObb = GetOBB();
				Vec3 HitPoint;
				//最近接点を得るための判定
				HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, HitPoint);
				//衝突法線をHitPointとm_Posから導く
				Vec3 Normal = m_Pos - HitPoint;
				Normal.normalize();
				//速度をスライドさせて設定する
				m_Velocity = ProjUtil::Slide(m_Velocity, Normal);
				//Y方向はなし
				m_Velocity.y = 0;
				//最後に衝突点から余った時間分だけ新しい値で移動させる
				m_Pos = m_Pos + m_Velocity * SpanTime;
				//追い出し処理
				//少しづつ相手の領域から退避する
				//最大10回退避するが、それでも衝突していたら次回ターンに任せる
				int count = 0;
				while (count < 20) {
					//退避する係数
					float MiniSpan = 0.001f;
					//もう一度衝突判定
					//m_Posが動いたのでOBBを再取得
					SrcObb = GetOBB();
					if (HitTest::OBB_OBB(SrcObb, DestObb)) {
						//最近接点を得るための判定
						HitTest::ClosestPtPointOBB(SrcObb.m_Center, DestObb, HitPoint);
						//衝突していたら追い出し処理
						Vec3 EscapeNormal = SrcObb.m_Center - HitPoint;
						EscapeNormal.y = 0;
						EscapeNormal.normalize();
						m_Pos = m_Pos + EscapeNormal * MiniSpan;
					}
					else {
						break;
					}
					count++;
				}
			}
		}
	}

	void MoveBoxObject::RotToHead(float LerpFact) {
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

	void MoveBoxObject::OnUpdate() {
		//1つ前の位置を取っておく
		m_BeforePos = m_Pos;
		//速度を変化させる
		UpdateVelosity();
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//速度に合わせて位置の変更
		m_Pos += m_Velocity * ElapsedTime;
	}

	void MoveBoxObject::OnCollision() {
		//衝突判定
		CollisionWithBoxes(m_BeforePos);
	}

	void MoveBoxObject::OnRotation() {
		//回転
		RotToHead(0.1f);
	}



	void MoveBoxObject::OnDraw() {
		auto ShPtrScene = m_Scene.lock();
		if (!ShPtrScene) {
			return;
		}
		//行列の定義
		Mat4x4 World;
		//ワールド行列の決定
		World.affineTransformation(
			m_Scale,			//スケーリング
			Vec3(0, 0, 0),		//回転の中心（重心）
			m_Qt,				//回転角度
			m_Pos				//位置
		);
		ShPtrScene->GetPNTDrawObject()->AddDrawMesh(
			m_MoveBoxMesh,
			m_TextureResource,
			World,
			true
		);
	}


	//--------------------------------------------------------------------------------------
	///	PNT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	PNTDrawObject::PNTDrawObject(const shared_ptr<Scene> PtrScene) :
		m_Scene(PtrScene)
	{}
	PNTDrawObject::~PNTDrawObject() {}

	void PNTDrawObject::AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
		const shared_ptr<TextureResource>& TextureRes,
		const Mat4x4& WorldMat,
		bool Trace, bool Wrap) {
		DrawObject Obj;
		Obj.m_MeshRes = MeshRes;
		Obj.m_TextureRes = TextureRes;
		Obj.m_WorldMatrix = WorldMat;
		Obj.m_Trace = Trace;
		Obj.m_Wrap = Wrap;
		m_DrawObjectVec.push_back(Obj);
	}

	void PNTDrawObject::OnUpdate() {
		m_DrawObjectVec.clear();
	}

	void PNTDrawObject::OnDraw() {
		if (m_Scene.expired()) {
			//シーンが無効ならリターン
			return;
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();
		//各オブジェクト共通処理
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);
		//サンプラーの準備
		ID3D11SamplerState* pSamplerClamp = RenderState->GetLinearClamp();
		ID3D11SamplerState* pSamplerWrap = RenderState->GetLinearWrap();
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//行列の定義
		Mat4x4 View, Proj;
		//ライティング
		Vec4 LightDir;
		auto ShPtrScene = m_Scene.lock();
		ShPtrScene->GetViewProjMatrix(View, Proj);
		ShPtrScene->GetLightDir(LightDir);
		//ビュー行列の決定
		//転置する
		View.transpose();
		//射影行列の決定
		//転置する
		Proj.transpose();
		//コンスタントバッファの準備
		PNTStaticConstantBuffer sb;
		sb.View = View;
		sb.Projection = Proj;
		sb.LightDir = LightDir;
		//ディフューズ
		sb.Diffuse = Col4(1.0f, 1.0f, 1.0f, 1.0f);
		//エミッシブ加算。
		sb.Emissive = Col4(0.4f, 0.4f, 0.4f, 0);
		//個別処理
		for (auto& v : m_DrawObjectVec) {
			//転置する
			v.m_WorldMatrix.transpose();
			//ワールド行列の決定
			sb.World = v.m_WorldMatrix;
			//コンスタントバッファの更新
			pD3D11DeviceContext->UpdateSubresource(CBPNTStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBPNTStatic::GetPtr()->GetBuffer();
			ID3D11Buffer* pNullConstantBuffer = nullptr;
			//頂点シェーダに渡す
			pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			//ピクセルシェーダに渡す
			pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//頂点バッファのセット
			pD3D11DeviceContext->IASetVertexBuffers(0, 1, v.m_MeshRes->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//インデックスバッファのセット
			pD3D11DeviceContext->IASetIndexBuffer(v.m_MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//テクスチャの設定
			ID3D11ShaderResourceView* pNull[1] = { 0 };
			pD3D11DeviceContext->PSSetShaderResources(0, 1, v.m_TextureRes->GetShaderResourceView().GetAddressOf());
			//サンプラー
			if (v.m_Wrap) {
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerWrap);
			}
			else {
				pD3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerClamp);
			}
			//ブレンドステート
			if (v.m_Trace) {
				//透明処理
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
				//透明処理の場合は、ラスタライザステートを変更して2回描画
				//ラスタライザステート（裏面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
				//ラスタライザステート（表面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
			else {
				//透明処理しない
				pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
				//ラスタライザステート（表面描画）
				pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
				//描画
				pD3D11DeviceContext->DrawIndexed(v.m_MeshRes->GetNumIndicis(), 0, 0);
			}
		}
		//後始末
		Dev->InitializeStates();
	}

	//--------------------------------------------------------------------------------------
	///	ラッピング処理されたスプライト
	//--------------------------------------------------------------------------------------
	WrappedSprite::WrappedSprite(const wstring& TextureFileName, bool Trace, 
		const Vec2& StartScale, const Vec2& StartPos,
		UINT XWrap, UINT YWrap) :
		ObjectInterface(),
		ShapeInterface(),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(StartScale),
		m_Rot(0),
		m_Pos(StartPos),
		m_XWrap(XWrap),
		m_YWrap(YWrap),
		m_TotalTime(0)
	{}
	WrappedSprite::~WrappedSprite() {}
	void WrappedSprite::OnCreate() {
		float HelfSize = 0.5f;
		//頂点配列(縦横10個ずつ表示)
		m_BackupVertices = {
			{ VertexPositionColorTexture(Vec3(-HelfSize, HelfSize, 0),Col4(1.0f,0,0,1.0f), Vec2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, HelfSize, 0), Col4(0, 1.0f, 0, 1.0f), Vec2((float)m_XWrap, 0.0f)) },
			{ VertexPositionColorTexture(Vec3(-HelfSize, -HelfSize, 0), Col4(0, 0, 1.0f, 1.0f), Vec2(0.0f, (float)m_YWrap)) },
			{ VertexPositionColorTexture(Vec3(HelfSize, -HelfSize, 0), Col4(1.0f, 1.0f, 0, 1.0f), Vec2((float)m_XWrap, (float)m_YWrap)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		//メッシュの作成（変更できる）
		m_SquareMesh = MeshResource::CreateMeshResource(m_BackupVertices, indices, true);
		//テクスチャの作成
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
	}

	void WrappedSprite::UpdateVertex(float ElapsedTime) {
		m_TotalTime += ElapsedTime;
		if (m_TotalTime >= 1.0f) {
			m_TotalTime = 0;
		}

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		//頂点の変更
		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pD3D11DeviceContext->Map(m_SquareMesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer))) {
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"WrappedSprite::UpdateVertex()"
			);
		}
		//頂点の変更
		VertexPositionColorTexture* vertices
			= (VertexPositionColorTexture*)mappedBuffer.pData;
		for (size_t i = 0; i < m_SquareMesh->GetNumVertices(); i++) {
			Vec2 UV = m_BackupVertices[i].textureCoordinate;
			if (UV.x == 0.0f) {
				UV.x = m_TotalTime;
			}
			else if (UV.x == 4.0f) {
				UV.x += m_TotalTime;
			}
			vertices[i] = VertexPositionColorTexture(
				m_BackupVertices[i].position,
				m_BackupVertices[i].color,
				UV
			);

		}
		//アンマップ
		pD3D11DeviceContext->Unmap(m_SquareMesh->GetVertexBuffer().Get(), 0);

	}


	void WrappedSprite::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_Rot += ElapsedTime;
		if (m_Rot >= XM_2PI) {
			m_Rot = 0;
		}
		UpdateVertex(ElapsedTime);
	}



	void WrappedSprite::OnDraw() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		//行列の定義
		Mat4x4 World, Proj;
		//ワールド行列の決定
		World.affineTransformation2D(
			m_Scale,			//スケーリング
			Vec2(0, 0),		//回転の中心（重心）
			m_Rot,				//回転角度
			m_Pos				//位置
		);
		//射影行列の決定
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj = XMMatrixOrthographicLH(w, h, -1.0, 1.0f);
		//行列の合成
		World *= Proj;

		//コンスタントバッファの準備
		SpriteConstantBuffer sb;
		//エミッシブ加算は行わない。
		sb.Emissive = Col4(0, 0, 0, 0);
		//行列の設定
		sb.World = World;
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSprite::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionColorTexture);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_SquareMesh->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(m_SquareMesh->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBSprite::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPCTSprite::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPCTSprite::GetPtr()->GetInputLayout());

		//ブレンドステート
		if (m_Trace) {
			//透明処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		}
		else {
			//透明処理しない
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		}
		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthNone(), 0);
		//ラスタライザステート
		pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());

		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureResource->GetShaderResourceView().GetAddressOf());
		//ラッピングサンプラー
		ID3D11SamplerState* pSampler = RenderState->GetLinearWrap();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		//描画
		pD3D11DeviceContext->DrawIndexed(m_SquareMesh->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates();
	}


}
//end basecross
