/*!
@file ProjectBehavior.cpp
@brief プロジェク定義の行動クラス実体
*/

#include "stdafx.h"
#include "Project.h"


namespace basecross {

	//--------------------------------------------------------------------------------------
	///	プレイヤーの行動クラス
	//--------------------------------------------------------------------------------------
	Vec3 PlayerBehavior::GetMoveVector() const {
		Vec3 Angle(0, 0, 0);
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//動いた時のスピード
				auto PtrTransform = GetGameObject()->GetComponent<Transform>();
				auto PtrCamera = GetGameObject()->OnGetDrawCamera();
				//進行方向の向きを計算
				Vec3 Front = PtrTransform->GetWorldMatrix().transInMatrix() - PtrCamera->GetEye();
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

	void PlayerBehavior::MovePlayer() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vec3 Angle = GetMoveVector();
		//Rigidbodyを取り出す
		auto PtrRedit = GetGameObject()->GetComponent<Rigidbody>();
		auto Velo = PtrRedit->GetVelocity();
		if (Angle.length() <= 0.0f && Velo.y == 0.0f) {
			//コントローラを離したとき対策
			Velo *= GetDecel();
			PtrRedit->SetVelocity(Velo);
			return;
		}
		//Transform
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		//現在の速度を取り出す
		//目的地を最高速度を掛けて求める
		auto Target = Angle * GetMaxSpeed();
		//目的地に向かうために力のかける方向を計算する
		//Forceはフォースである
		auto Force = Target - Velo;
		//yは0にする
		Force.y = 0;
		//加速度を求める
		auto Accel = Force / GetMass();
		//ターン時間を掛けたものを速度に加算する
		Velo += (Accel * ElapsedTime);
		//速度を設定する
		PtrRedit->SetVelocity(Velo);
		//回転の計算
		if (Angle.length() > 0.0f) {
			auto UtilPtr = GetGameObject()->GetBehavior<UtilBehavior>();
			UtilPtr->RotToHead(Angle, 1.0f);
		}
	}

	void PlayerBehavior::FireAttackBall() {
		auto PtrCamera = GetGameObject()->OnGetDrawCamera();
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		auto Group = GetStage()->GetSharedObjectGroup(L"AttackBall");
		for (auto& v : Group->GetGroupVector()) {
			auto shptr = v.lock();
			if (shptr) {
				auto AttackPtr = dynamic_pointer_cast<AttackBall>(shptr);
				if (AttackPtr && !AttackPtr->IsUpdateActive()) {
					//回転の計算
					auto RotY = PtrTrans->GetRotation().y;
					auto Angle = Vec3(sin(RotY), 0, cos(RotY));
					Angle.normalize();
					auto Span = Angle * 0.5f;
					AttackPtr->Weakup(PtrTrans->GetWorldMatrix().transInMatrix() + Span, Angle * 10.0f);
					return;
				}
			}
		}

	}

	bool PlayerBehavior::OnHitObjMoveBox(vector<shared_ptr<GameObject>>& OtherVec, shared_ptr<GameObject>& retv) {
		for (auto& v : OtherVec) {
			if (v->FindTag(L"MoveBox")) {
				auto PtrTrans = GetGameObject()->GetComponent<Transform>();
				auto sp = GetGameObject()->GetComponent<CollisionSphere>()->GetSphere();
				auto obb = v->GetComponent<CollisionObb>()->GetObb();
				Vec3 RetPoint;
				HitTest::SPHERE_OBB(sp, obb, RetPoint);
				auto Normal = sp.m_Center - RetPoint;
				if (Normal.length() > sp.m_Radius * 1.5f) {
					return false;
				}
				Normal.normalize();
				Vec3 angle(XMVector3AngleBetweenNormals(Vec3(0, 1.0f, 0), Normal));
				if (angle.x >= 0.1f) {
					return false;
				}
				retv = v;
				return true;
			}
		}
		return false;
	}


	bool PlayerBehavior::OnMoveBox() {
		auto PtrTrans = GetGameObject()->GetComponent<Transform>();
		auto sp = GetGameObject()->GetComponent<CollisionSphere>()->GetSphere();
		auto obb = PtrTrans->GetParent()->GetComponent<CollisionObb>()->GetObb();
		Vec3 RetPoint;
		HitTest::SPHERE_OBB(sp, obb, RetPoint);
		auto Normal = sp.m_Center - RetPoint;
		if (Normal.length() > sp.m_Radius * 1.5f) {
			return false;
		}
		Normal.normalize();
		Vec3 angle(XMVector3AngleBetweenNormals(Vec3(0, 1.0f, 0), Normal));
		if (angle.x >= 0.1f) {
			return false;
		}
		return true;
	}

}

//end basecross

