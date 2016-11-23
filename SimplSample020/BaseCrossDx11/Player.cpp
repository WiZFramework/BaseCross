/*!
@file Player.cpp
@brief プレイヤーなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ジャンプコマンド
	//--------------------------------------------------------------------------------------
	void JumpCommand::Excute(const shared_ptr<Player>& Obj) {
		//ジャンプスタート
		Obj->GetStateMachine()->Push(PlayerJump::Instance());
	}
	//--------------------------------------------------------------------------------------
	///	行動切り替えコマンド
	//--------------------------------------------------------------------------------------
	void BehaviorChangeCommand::Excute(const shared_ptr<Player>& Obj) {
		//行動切り替え
		Obj->ChangePlayerBehavior();
	}


	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	用途: プレイヤー
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Player::Player(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{
	}
	Player::~Player() {}

	//コントローラのチェック
	void Player::InputCheck() {
		//コントローラボタンチェック
		auto pCom = m_Handler.HandleInput();
		if (pCom) {
			pCom->Excute(GetThis<Player>());
		}
	}
	//行動の切り替え
	void Player::ChangePlayerBehavior() {
		//次の行動をチェンジする（仮想関数で作成する）
		m_PlayerBehavior = m_PlayerBehavior->CreateNextBehavior(GetThis<Player>());
	}

	//初期化
	void Player::OnCreate() {
		//コマンドの登録
		//Aボタン
		m_Handler.SetButtonA(&m_Jump);
		//Bボタン
		m_Handler.SetButtonB(&m_BehaviorChange);

		//初期位置などの設定
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(0.5f, 0.5, 0.5);	//直径50センチの球体
		Ptr->SetRotation(0.0f, 0.0f, 0.0f);
		Ptr->SetPosition(0, 0.25f, 0);

		//Rigidbodyをつける
		auto PtrRedid = AddComponent<Rigidbody>();
		//反発係数は0.5
		PtrRedid->SetReflection(0.5f);

		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();
		//最下地点
		PtrGravity->SetBaseY(0.25f);

		//衝突判定
		auto PtrColl = AddComponent<CollisionSphere>();
		//横部分のみ反発
		PtrColl->SetIsHitAction(IsHitAction::AutoOnObjectRepel);
//		PtrColl->SetDrawActive(true);

		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetFont(L"ＭＳゴシック", 32);
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


		Matrix4X4 SpanMat; // モデルとトランスフォームの間の差分行列
		SpanMat.DefTransformation(
			Vector3(0.8, 0.8, 0.8),
			Vector3(0.0f, XM_PI, 0.0f),
			Vector3(0.0f, -0.5f, 0.0f)
		);

		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(L"PLAYER_MESH");
		ShadowPtr->SetMeshToTransformMatrix(SpanMat);

		//描画コンポーネントの設定
		auto PtrDraw = AddComponent<PNTStaticModelDraw>();
		//描画するメッシュを設定
		PtrDraw->SetMeshResource(L"PLAYER_MESH");
		PtrDraw->SetMeshToTransformMatrix(SpanMat);

		auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(GetStage()->GetView()->GetTargetCamera());
		if (PtrCamera) {
			//LookAtCameraに注目するオブジェクト（プレイヤー）の設定
			PtrCamera->SetTargetObject(GetThis<GameObject>());
		}
		//行動クラスの構築(シンプル行動)
		m_PlayerBehavior = SimplePlayerBehavior::Instance();
		PtrString->SetText(L"デフォルトジャンプ: Bボタンで切り替え");
		//ステートマシンの構築
		m_StateMachine = make_shared< LayeredStateMachine<Player> >(GetThis<Player>());
		//最初のステートをPlayerDefaultに設定
		m_StateMachine->Reset(PlayerDefault::Instance());
	}

	void Player::OnUpdate() {
		//コントローラチェック
		InputCheck();
		//ステートマシン更新
		m_StateMachine->Update();
	}

	//--------------------------------------------------------------------------------------
	///	プレイヤー行動親
	//--------------------------------------------------------------------------------------
	Vector3 PlayerBehavior::GetAngle(const shared_ptr<Player>& Obj) {
		Vector3 Angle(0, 0, 0);
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			if (CntlVec[0].fThumbLX != 0 || CntlVec[0].fThumbLY != 0) {
				float MoveLength = 0;	//動いた時のスピード
				auto PtrTransform = Obj->GetComponent<Transform>();
				auto PtrCamera = dynamic_pointer_cast<LookAtCamera>(Obj->GetStage()->GetView()->GetTargetCamera());
				if (PtrCamera) {
					//進行方向の向きを計算
					Vector3 Front = PtrTransform->GetPosition() - PtrCamera->GetEye();
					Front.y = 0;
					Front.Normalize();
					//進行方向向きからの角度を算出
					float FrontAngle = atan2(Front.z, Front.x);
					//コントローラの向き計算
					float MoveX = CntlVec[0].fThumbLX;
					float MoveZ = CntlVec[0].fThumbLY;
					MoveLength = sqrt(MoveX * MoveX + MoveZ * MoveZ);
					//コントローラの向きから角度を計算
					float CntlAngle = atan2(-MoveX, MoveZ);
					//トータルの角度を算出
					float TotalAngle = FrontAngle + CntlAngle;
					//角度からベクトルを作成
					Angle = Vector3(cos(TotalAngle), 0, sin(TotalAngle));
					//正規化する
					Angle.Normalize();
					//Y軸は変化させない
					Angle.y = 0;
					Angle *= MoveLength;
				}
			}
		}
		return Angle;
	}

	//--------------------------------------------------------------------------------------
	///	シンプルプレイヤー行動
	//--------------------------------------------------------------------------------------
	//インスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(SimplePlayerBehavior)
	shared_ptr<PlayerBehavior> SimplePlayerBehavior::CreateNextBehavior(const shared_ptr<Player>& Obj) {
		Obj->GetComponent<StringSprite>()->SetText(L"ハイジャンプ: Bボタンで切り替え");
		//ハイジャンプ行動を返す
		return HighJumpPlayerBehavior::Instance();
	}


	void SimplePlayerBehavior::Move(const shared_ptr<Player>& Obj) {
		auto Angle = GetAngle(Obj);
		auto PtrTransform = Obj->GetComponent<Transform>();
		auto PtrRedid = Obj->GetComponent<Rigidbody>();
		PtrRedid->SetVelocity(Angle * 10.0f);
		//回転の計算
		float YRot = PtrTransform->GetRotation().y;
		Quaternion Qt;
		Qt.Identity();
		if (Angle.Length() > 0.0f) {
			//ベクトルをY軸回転に変換
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

	void SimplePlayerBehavior::StartJump(const shared_ptr<Player>& Obj) {
		auto PtrTrans = Obj->GetComponent<Transform>();
		//重力
		auto PtrGravity = Obj->GetComponent<Gravity>();
		//ジャンプスタート
		Vector3 JumpVec(0.0f, 4.0f, 0);
		PtrGravity->StartJump(JumpVec);
	}
	void SimplePlayerBehavior::JumpExcute(const shared_ptr<Player>& Obj) {
		auto PtrTransform = Obj->GetComponent<Transform>();
		//重力
		auto PtrGravity = Obj->GetComponent<Gravity>();
		if (PtrGravity->GetGravityVelocity().Length() <= 0) {
			//ジャンプ終了
			Obj->GetStateMachine()->Pop();
		}
	}

	//--------------------------------------------------------------------------------------
	///	ハイジャンププレイヤー行動
	//--------------------------------------------------------------------------------------
	//インスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(HighJumpPlayerBehavior)
	shared_ptr<PlayerBehavior> HighJumpPlayerBehavior::CreateNextBehavior(const shared_ptr<Player>& Obj) {
		Obj->GetComponent<StringSprite>()->SetText(L"デフォルトジャンプ: Bボタンで切り替え");
		//シンプル行動を返す
		return SimplePlayerBehavior::Instance();
	}
	void HighJumpPlayerBehavior::StartJump(const shared_ptr<Player>& Obj) {
		auto PtrTrans = Obj->GetComponent<Transform>();
		//重力
		auto PtrGravity = Obj->GetComponent<Gravity>();
		//ジャンプスタート
		Vector3 JumpVec(0.0f, 12.0f, 0);
		PtrGravity->StartJump(JumpVec);
	}

	//--------------------------------------------------------------------------------------
	///	通常ステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(PlayerDefault)
	//ステート実行中に毎ターン呼ばれる関数
	void PlayerDefault::Execute(const shared_ptr<Player>& Obj) {
		Obj->GetPlayerBehavior()->Move(Obj);
	}

	//--------------------------------------------------------------------------------------
	///	ジャンプステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(PlayerJump)
	//ステートに入ったときに呼ばれる関数
	void PlayerJump::Enter(const shared_ptr<Player>& Obj) {
		Obj->GetPlayerBehavior()->StartJump(Obj);
	}

	//ステート実行中に毎Updateに呼ばれる関数
	void PlayerJump::Execute(const shared_ptr<Player>& Obj) {
		Obj->GetPlayerBehavior()->Move(Obj);
		Obj->GetPlayerBehavior()->JumpExcute(Obj);
	}









}
//end basecross

