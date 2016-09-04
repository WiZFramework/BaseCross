/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	球実体
	//--------------------------------------------------------------------------------------
	Player::Player(const shared_ptr<Scene> PtrScene,
		UINT Division, const wstring& TextureFileName, bool Trace, const Vector3& Pos) :
		GameObject(PtrScene),
		m_Division(Division),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(Vector3(1.0f,1.0f,1.0f)),
		m_Qt(),
		m_Pos(Pos),
		m_OnObject(false)
	{
		m_Qt.Identity();
	}
	Player::~Player(){}

	void Player::CreateBuffers(){
		float Radius = 0.5f;
		//緯度方向
		UINT Vertical = m_Division;
		//経度方向
		UINT Horizontal = m_Division * 2;

		vector<VertexPositionNormalTexture> vertices;
		for (UINT i = 0; i <= Vertical; i++)
		{
			float v = 1 - (float)i / Vertical;

			float Latitude = (i * XM_PI / Vertical) - XM_PIDIV2;
			float Dy = sin(Latitude);
			float Dxz = cos(Latitude);

			for (UINT j = 0; j <= Horizontal; j++)
			{
				float u = 1 - (float)j / Horizontal;

				float Longitude = j * XM_2PI / Horizontal;
				float Dx = sin(Longitude) * Dxz;
				float Dz = cos(Longitude) * Dxz;
				Vector3 normal(Dx, Dy, Dz);
				VertexPositionNormalTexture Data;
				Data.position = normal * Radius;
				Data.normal = normal;
				Data.normal.Normalize();
				Data.textureCoordinate = Vector2(u, v);
				vertices.push_back(Data);
			}
		}
		UINT Stride = Horizontal + 1;
		vector<uint16_t> indices;
		for (UINT i = 0; i < Vertical; i++)
		{
			for (UINT j = 0; j <= Horizontal; j++)
			{
				UINT NextI = i + 1;
				UINT NextJ = (j + 1) % Stride;
				indices.push_back((uint16_t)(i * Stride + NextJ));
				indices.push_back((uint16_t)(NextI * Stride + j));
				indices.push_back((uint16_t)(i * Stride + j));

				indices.push_back((uint16_t)(NextI * Stride + NextJ));
				indices.push_back((uint16_t)(NextI * Stride + j));
				indices.push_back((uint16_t)(i * Stride + NextJ));
			}
		}
		//頂点バッファの作成（変更できない）
		m_Mesh = MeshResource::CreateMeshResource(vertices, indices ,false);

	}


	void Player::CollisionTest(){
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vector3 BeforPos = GetComponent<Transform>()->GetBeforePosition();
		Vector3 NowPos = GetComponent<Transform>()->GetPosition();
		Vector3 Velocity = NowPos - BeforPos;
		//秒速を得る
		Velocity /= ElapsedTime;

		for (auto v : GetScene()->GetCubeObjectVec()){
			OBB obb = v->GetObb();
			SPHERE sp;
			sp.m_Center = BeforPos;
			sp.m_Radius = 0.5f;
			float HitTime;
			if (HitTest::CollisionTestSphereObb(sp, Velocity, obb, 0, ElapsedTime, HitTime)){
				//HitTime時に衝突直前になった
				NowPos = BeforPos + Velocity * HitTime;
				Vector3 Ret;
				sp.m_Center = NowPos;
				if (OnObjectTest(sp, obb)){
					//オブジェクトの上に乗った
					auto PtrGravity = GetComponent<Gravity>();

					Vector3 RotDevY = obb.m_Rot[1];
					Vector3 RotDev;
					if (RotDevY.x == 0 && RotDevY.z == 0){
						RotDev = Vector3(0, 0, 0);
					}
					else{
						RotDevY *= (PtrGravity->GetDefaultGravity().Length() * RotDevY.y);
						RotDev = PtrGravity->GetDefaultGravity() + RotDevY;
					}
					PtrGravity->SetGravity(RotDev);
					PtrGravity->SetGravityVelocityZero();
					GetComponent<Transform>()->SetPosition(NowPos);
				}
				else{
					//オブジェクトの上に乗ってない
					auto PtrGravity = GetComponent<Gravity>();
					PtrGravity->SetGravityDefault();
					HitTest::SPHERE_OBB(sp, obb, Ret);
					Vector3 Normal = sp.m_Center - Ret;
					Normal.Normalize();
					Vector3 RefVelocity = Vector3EX::Reflect(Velocity, Normal);
					float OtherTime = ElapsedTime - HitTime;
					NowPos += RefVelocity * OtherTime;
					GetComponent<Transform>()->SetPosition(NowPos);
				}
				//いずれのオブジェクトかとヒットすればリターン
				return;
			}
		}
	}



	bool Player::OnObjectTest(const SPHERE& Src, const OBB& Dest){
		SPHERE SrcSphere = Src;
		OBB DestObb = Dest;
		Vector3 Ret;
		auto IsHit = HitTest::SPHERE_OBB(SrcSphere, DestObb, Ret);
		Vector3 BottomPoint(0, 0, 0);
		BottomPoint.y -= SrcSphere.m_Radius;
		//軸回転
		BottomPoint.Transform(DestObb.GetRotMatrix());
		BottomPoint += SrcSphere.m_Center;
		float BottomToRetLen = Vector3EX::Length(BottomPoint - Ret);
		if (!IsHit && BottomToRetLen < (SrcSphere.m_Radius * 0.1f)){
			//乗っている
			m_OnObject = true;
			m_OnObb = Dest;
			return true;
		}
		else{
			//乗ってない
			m_OnObject = false;
			return false;
		}

	}

	void Player::CollisionEscape(){
		Vector3 NowPos = GetComponent<Transform>()->GetPosition();
		SPHERE sp;
		sp.m_Center = NowPos;
		sp.m_Radius = 0.5f;
		for (auto v : GetScene()->GetCubeObjectVec()){
			OBB obb = v->GetObb();
			Vector3 Ret;
			if (HitTest::SPHERE_OBB(sp,obb, Ret)){
				Vector3 Normal = sp.m_Center - Ret;
				Normal.Normalize();
				Normal *= (sp.m_Radius * 1.05f);
				Vector3 NewPos = Ret + Normal;
				GetComponent<Transform>()->SetPosition(NewPos);
				return;
			}
		}
	}



	void Player::OnCreate(){
		CreateBuffers();
		//テクスチャの作成
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetQuaternion(m_Qt);
		PtrTrans->SetPosition(m_Pos);

		auto PtrGravity = AddComponent<Gravity>();
		PtrGravity->SetBaseY(0.5f);

		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetTrace(true);
		DrawComp->SetMeshResource(m_Mesh);
		DrawComp->SetTextureResource(m_TextureResource);
	}
	void Player::OnUpdate(){

		auto PtrTrans = GetComponent<Transform>();
		auto Pos  = PtrTrans->GetPosition();
		//前回のターンからの経過時間を求める
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//キーボードとマウスの取得
		auto Key = App::GetApp()->GetInputDevice().GetKeyState();
		Vector3 TempPos = Pos;
		//キーボードチェック
		if (Key.m_bPushKeyTbl['F']){
			Pos.x -= ElapsedTime * 5.0f;
		}
		else if (Key.m_bPushKeyTbl['G']){
			Pos.x += ElapsedTime * 5.0f;
		}
		if (Key.m_bPushKeyTbl['T']){
			Pos.z += ElapsedTime * 5.0f;
		}
		else if (Key.m_bPushKeyTbl['V']){
			Pos.z -= ElapsedTime * 5.0f;
		}
		//コントローラチェック
		if (CntlVec[0].bConnected){
			if (CntlVec[0].fThumbLX != 0){
				Pos.x += (CntlVec[0].fThumbLX * ElapsedTime * 5.0f);
			}
			if (CntlVec[0].fThumbLY != 0){
				Pos.z += (CntlVec[0].fThumbLY * ElapsedTime * 5.0f);
			}
		}
		TempPos = Pos - TempPos;
		if (TempPos.Length() > 0){
			//移動した
			TempPos.Normalize();
			float Angle = atan2(TempPos.x, TempPos.z);
			m_Qt.RotationAxis(Vector3(0, 1.0f, 0), Angle);
			m_Qt.Normalize();
		}

		PtrTrans->SetQuaternion(m_Qt);
		PtrTrans->SetPosition(Pos);
		//コントローラチェック
		if (CntlVec[0].bConnected){
			//Aボタンが押された瞬間ならジャンプ
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A){
				auto PtrGravity = GetComponent<Gravity>();
				if (m_OnObject){
					PtrGravity->StartJump(Vector3(0, 4.0f, 0.5f));
				}
				else{
					PtrGravity->StartJump(Vector3(0, 4.0f, 0));
				}
			}
		}

	}

	void Player::OnLastUpdate(){
		Vector3 NowPos = GetComponent<Transform>()->GetPosition();
		SPHERE sp;
		sp.m_Center = NowPos;
		sp.m_Radius = 0.5f;
		if (m_OnObject){
			if (OnObjectTest(sp, m_OnObb)){
				return;
			}
			else{
				auto PtrGravity = GetComponent<Gravity>();
				PtrGravity->SetGravityDefault();
				return;
			}
		}
		CollisionTest();
		CollisionEscape();
	}

}
//end basecross

