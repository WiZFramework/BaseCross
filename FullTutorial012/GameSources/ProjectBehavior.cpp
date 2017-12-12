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
				Vec3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
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
			UtilPtr->RotToHead(Angle, 0.1f);
		}
	}


}

//end basecross

