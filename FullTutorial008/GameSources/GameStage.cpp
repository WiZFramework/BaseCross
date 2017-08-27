/*!
@file GameStage.cpp
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	// 個別カメラ
	//--------------------------------------------------------------------------------------
	//構築と破棄
	MyCamera::MyCamera() :
		Camera(),
		m_ToTargetLerp(1.0f)
	{
	}
	MyCamera::~MyCamera() {}
	//アクセサ
	shared_ptr<GameObject> MyCamera::GetTargetObject() const {
		if (!m_TargetObject.expired()) {
			return m_TargetObject.lock();
		}
		return nullptr;
	}

	void MyCamera::SetTargetObject(const shared_ptr<GameObject>& Obj) {
		m_TargetObject = Obj;
	}

	float MyCamera::GetToTargetLerp() const {
		return m_ToTargetLerp;
	}
	void MyCamera::SetToTargetLerp(float f) {
		m_ToTargetLerp = f;
	}

	void MyCamera::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		if (CntlVec[0].bConnected) {
			Vec3 NewAt(0, 0, 0);
			auto TargetPtr = GetTargetObject();
			if (TargetPtr) {
				//目指したい場所
				Vec3 ToAt = TargetPtr->GetComponent<Transform>()->GetPosition();
				NewAt = Lerp::CalculateLerp(GetAt(), ToAt, 0, 1.0f, m_ToTargetLerp, Lerp::Linear);
			}
			//ステップ1、注視点と位置の変更
			Vec3 Span = GetAt() - GetEye();
			Vec3 NewEye = NewAt - Span;
			SetAt(NewAt);
			SetEye(NewEye);
			//ステップ２、ズームの変更
			//カメラ位置と注視点の間のベクトルを算出
			Span = GetAt() - GetEye();
			//正規化
			Span.normalize();
			//変化値の決定
			Span = Span * ElapsedTime * 10.0f;

			Vec3 NewArm = GetAt() - GetEye();
			//Dパッド下
			//カメラを引く
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				//カメラ位置を引く
				NewEye = NewEye - Span;
				NewArm = NewAt - NewEye;
				if (NewArm.length() > (GetFar() * 0.1f)) {
					NewEye = NewEye + Span;
					NewArm = NewAt - NewEye;
				}
			}
			//Dパッド上
			//カメラを寄る
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				//カメラ位置を寄る
				NewEye = NewEye + Span;
				NewArm = NewAt - NewEye;
				if (NewArm.length() < GetNear() * 2.0f) {
					NewEye = NewEye - Span;
					NewArm = NewAt - NewEye;
				}
			}
			SetAt(NewAt);
			SetEye(NewEye);
			//ステップ3角度の変更
			//現在のAtとEyeの角度を得る
			Vec3 ArmInv = GetEye() - GetAt();
			//右スティックX方向
			FLOAT AngleY = 0;
			//右スティックY方向
			FLOAT AngleX = 0;
			FLOAT AngleZ = 0;
			if (CntlVec[0].fThumbRX != 0) {
				//右スティック反応しないようにする
			}
			if (CntlVec[0].fThumbRY != 0) {
				//右スティック反応しないようにする
			}
			if (ArmInv.z > 0) {
				AngleX *= -1.0f;
			}
			if (ArmInv.x < 0) {
				AngleZ *= -1.0f;
			}
			Quat QtSpan;
			QtSpan.rotationRollPitchYawFromVector(Vec3(AngleX, AngleY, AngleZ));
			QtSpan.normalize();
			//回転先計算の行列を作成
			Mat4x4 Mat, Mat2;
			Mat.strTransformation(
				Vec3(1.0f, 1.0f, 1.0f),
				ArmInv,
				QtSpan);
			Mat2.translation(GetAt());
			Mat *= Mat2;
			NewEye = Mat.transInMatrix();
			if (NewEye.y < 0.5f) {
				NewEye.y = 0.5f;
			}
			//カメラが一定以上、上から視線にならなように調整
			ArmInv = NewEye - GetAt();
			ArmInv.normalize();
			float y2 = ArmInv.y * ArmInv.y;
			float x2 = ArmInv.x * ArmInv.x;
			float z2 = ArmInv.z * ArmInv.z;
			if (y2 <= (x2 + z2)) {
				SetEye(NewEye);
			}

		}
		Camera::OnUpdate();
	}


	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス実体
	//--------------------------------------------------------------------------------------


	//リソースの作成
	void GameStage::CreateResourses() {
		wstring DataDir;
		//サンプルのためアセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(DataDir);
		//各ゲームは以下のようにデータディレクトリを取得すべき
		//App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
	}

	//ビューとライトの作成
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//ビューのカメラの設定
		auto PttCamera = ObjectFactory::Create<MyCamera>();
		//LookAtCameraにする場合は以下コメントを外す
//		auto PttCamera = ObjectFactory::Create<LookAtCamera>();

		PtrView->SetCamera(PttCamera);
		PttCamera->SetEye(Vec3(0.0f, 2.0f, -8.0f));
		PttCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));

		//マルチライトの作成
		auto PtrMultiLight = CreateLight<MultiLight>();
		//デフォルトのライティングを指定
		PtrMultiLight->SetDefaultLighting();
	}


	//プレートの作成
	void GameStage::CreatePlate() {
		//ステージへのゲームオブジェクトの追加
		auto Ptr = AddGameObject<GameObject>();
		auto PtrTrans = Ptr->GetComponent<Transform>();
		Quat Qt;
		Qt.rotationRollPitchYawFromVector(Vec3(XM_PIDIV2, 0, 0));
		PtrTrans->SetScale(50.0f, 50.0f, 1.0f);
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(0.0f, 0.0f, 0.0f);

		auto ColPtr = Ptr->AddComponent<CollisionRect>();
		//描画コンポーネントの追加
		auto DrawComp = Ptr->AddComponent<BcPNTStaticDraw>();
		DrawComp->SetFogEnabled(true);
		//描画コンポーネントに形状（メッシュ）を設定
		DrawComp->SetMeshResource(L"DEFAULT_SQUARE");
		//自分に影が映りこむようにする
		DrawComp->SetOwnShadowActive(true);

		//描画コンポーネントテクスチャの設定
		DrawComp->SetTextureResource(L"SKY_TX");
	}


	//固定のボックスの作成
	void GameStage::CreateFixedBox() {
		//配列の初期化
		vector< vector<Vec3> > Vec = {
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(10.0f, 0.25f, 0.0f)
			},
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-10.0f, 0.25f, 0.0f)
			},
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 0.25f, 2.0f)
			},
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-5.0f, 0.25f, 2.0f)
			},

			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(10.0f, 0.25f, 4.0f)
			},
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-10.0f, 0.25f, 4.0f)
			},
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(5.0f, 0.25f, 6.0f)
			},
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-5.0f, 0.25f, 6.0f)
			},

			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(10.0f, 0.25f, 8.0f)
			},
			{
				Vec3(5.0f, 0.5f, 1.0f),
				Vec3(0.0f, 0.0f, 0.0f),
				Vec3(-10.0f, 0.25f, 8.0f)
			},

		};
		//オブジェクトの作成
		for (auto v : Vec) {
			AddGameObject<FixedBox>(v[0], v[1], v[2]);
		}
	}

	//プレイヤーの作成
	void GameStage::CreatePlayer() {
		wstring DataDir;
		//サンプルのためアセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(DataDir);
		//各ゲームは以下のようにデータディレクトリを取得すべき
		//App::GetApp()->GetDataDirectory(DataDir);


		//プレーヤーの作成
		auto PlayerPtr = AddGameObject<Player>(DataDir + L"character_template_sample1\\");
		//シェア配列にプレイヤーを追加
		SetSharedGameObject(L"Player", PlayerPtr);
	}

	//Eggの作成
	void GameStage::CreateEgg() {
		wstring DataDir;
		//サンプルのためアセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(DataDir);
		//各ゲームは以下のようにデータディレクトリを取得すべき
		//App::GetApp()->GetDataDirectory(DataDir);

		AddGameObject<EggAnime>(DataDir + L"character_template_sample1\\");
	}

	void GameStage::OnCreate() {
		try {
			//リソースの作成
			CreateResourses();
			//ビューとライトの作成
			CreateViewLight();
			//プレートの作成
			CreatePlate();
			//固定のボックスの作成
			CreateFixedBox();
			//プレーヤーの作成
			CreatePlayer();
			//Eggの作成
			CreateEgg();
		}
		catch (...) {
			throw;
		}
	}

}
//end basecross
