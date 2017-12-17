/*!
@file Player.cpp
@brief プレイヤー実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	プレイヤー
	//--------------------------------------------------------------------------------------
	//構築
	Player::Player(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureResName) :
		GameObject(StagePtr),
		m_TextureResName(TextureResName)
	{}

	Vec3 Player::GetMoveVector()const {
		Vec3 Angle(0, 0, 0);
		auto PtrGameStage = GetStage<GameStage>();
		Vec3 CameraEye, CameraAt;
		PtrGameStage->GetCamera().GetCameraEyeAt(CameraEye, CameraAt);
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if(CntlVec[0].bConnected){
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//動いた時のスピード
				//進行方向の向きを計算
				Vec3 Front =m_Pos - CameraEye;
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

	//初期化
	void Player::OnCreate() {
		//タグの追加
		AddTag(L"Player");
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSphere(1.0f, 18, vertices, indices);
		//メッシュの作成（変更できない）
		m_SphereMesh = MeshResource::CreateMeshResource(vertices, indices, false);
		//初期位置などの設定
		m_Scale = Vec3(0.25f, 0.25f, 0.25f);	//直径25センチの球体
		m_Qt.identity();
		if (App::GetApp()->GetScene<Scene>()->IsFirstGame()) {
			m_Pos = Vec3(0, 0.125f, 0);
		}
		else {
			m_Pos = App::GetApp()->GetScene<Scene>()->GetBackupPlayerPos();
		}
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		
		m_PtrObj = make_shared<BcDrawObject>();
		auto TexPtr = App::GetApp()->GetResource<TextureResource>(m_TextureResName);
		m_PtrObj->m_MeshRes = m_SphereMesh;
		m_PtrObj->m_TextureRes = TexPtr;
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		m_PtrObj->m_OwnShadowmapActive = false;
		m_PtrObj->m_ShadowmapUse = true;
		m_PtrObj->m_BlendState = BlendState::AlphaBlend;
		m_PtrObj->m_RasterizerState = RasterizerState::DoubleDraw;
		
		//シャドウマップ描画データの構築
		m_PtrShadowmapObj = make_shared<ShadowmapObject>();
		m_PtrShadowmapObj->m_MeshRes = m_SphereMesh;
		//描画データの行列をコピー
		m_PtrShadowmapObj->m_WorldMatrix = World;

		//物理演算
		PsSphereParam param;
		//basecrossのスケーリングは直径基準なので、半径基準にする
		param.m_Radius = 0.25f * 0.5f;
		param.m_Mass = 1.0f;
		param.m_MotionType = PsMotionType::MotionTypeActive;
		param.m_Quat.identity();
		param.m_Pos = m_Pos;
		param.m_Velocity = Vec3(0);
		m_PhysicsSphere = GetStage()->GetPhysicsManager()->AddSingleSphere(param);

	}

	//更新
	void Player::OnUpdate() {
		//コントローラチェックして入力があればコマンド呼び出し
		m_InputHandler.PushHandle(GetThis<Player>());

		auto Vec = GetMoveVector();
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);

		PsBodyUpdateStatus SetStatus(Status);
		Vec3 Velo = GetMoveVector();
		SetStatus.m_LinearVelocity.x = Velo.x * 5.0f;
		SetStatus.m_LinearVelocity.z = Velo.z * 5.0f;
		GetStage()->GetPhysicsManager()->SetBodyStatus(m_PhysicsSphere->GetIndex(), SetStatus);
	}

	void Player::RotToHead(const bsm::Vec3& Velocity, float LerpFact) {
		if (LerpFact <= 0.0f) {
			//補間係数が0以下なら何もしない
			return;
		}
		//回転の更新
		if (Velocity.length() > 0.0f) {
			bsm::Vec3 Temp = Velocity;
			Temp.normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			bsm::Quat Qt;
			Qt.rotationRollPitchYawFromVector(bsm::Vec3(0, ToAngle, 0));
			Qt.normalize();
			//現在の回転を取得
			bsm::Quat NowQt = m_Qt;
			//現在と目標を補間
			if (LerpFact >= 1.0f) {
				NowQt = Qt;
			}
			else {
				NowQt = XMQuaternionSlerp(NowQt, Qt, LerpFact);
			}
			m_Qt = NowQt;
		}

	}

	//後更新
	void Player::OnUpdate2() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);
		m_Pos = Status.m_Position;
		//回転の計算
		Vec3 Angle = GetMoveVector();
		if (Angle.length() > 0.0f) {
			RotToHead(Angle, 0.1f);
		}
	}

	void Player::OnDrawShadowmap() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
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
	
	void Player::OnDraw() {
		//行列の定義
		Mat4x4 World;
		World.affineTransformation(
			m_Scale,
			Vec3(0, 0, 0),
			m_Qt,
			m_Pos
		);
		m_PtrObj->m_WorldMatrix = World;
		m_PtrObj->m_Camera = GetStage<Stage>()->GetCamera();
		auto shptr = m_Renderer.lock();
		if (!shptr) {
			auto PtrGameStage = GetStage<GameStage>();
			shptr = PtrGameStage->FindTagGameObject<BcPNTStaticRenderer>(L"BcPNTStaticRenderer");
			m_Renderer = shptr;
		}
		shptr->AddDrawObject(m_PtrObj);
	}


	//Aボタンハンドラ
	void  Player::OnPushA() {
		PsBodyStatus Status;
		GetStage()->GetPhysicsManager()->GetBodyStatus(m_PhysicsSphere->GetIndex(), Status);
		if (Status.m_Position.y > 0.125f) {
			//地面にいるときだけジャンプできる
			return;
		}
		PsBodyUpdateStatus SetStatus(Status);
		SetStatus.m_LinearVelocity += Vec3(0, 4.0f, 0.0);
		GetStage()->GetPhysicsManager()->SetBodyStatus(m_PhysicsSphere->GetIndex(), SetStatus);
	}


	//Xボタンハンドラ
	void Player::OnPushX() {
		Vec3 Pos = m_Pos;
		Pos.y += 0.25f;
		Quat Qt = m_Qt;
		Vec3 Rot = Qt.toRotVec();
		float RotY = Rot.y;
		Vec3 velo(sin(RotY), 0.05f, cos(RotY));
		velo.normalize();
		velo *= 20.0f;

		auto StagePtr = GetStage<GameStage>();

		auto ShPtr = StagePtr->GetFirePsSphere();
		if (ShPtr) {
			ShPtr->Reset(Pos, velo);
		}
		else {
			ShPtr = StagePtr->AddGameObject<FirePsSphere>(Pos, velo);
			StagePtr->SetFirePsSphere(ShPtr);
		}
	}

}
//end basecross
