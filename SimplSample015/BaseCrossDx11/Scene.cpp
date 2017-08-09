/*!
@file Scene.cpp
@brief シーンなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	Scene::Scene() :
		SceneInterface(),
		m_CamerEye(0, 5.0, -5.0f),
		m_CamerAt(0, 0, 0),
		m_CamerUp(0, 1.0f, 0),
		m_FovY(XM_PIDIV4),
		m_CameraXZRad(0.0f),
		m_CameraYRad(XM_PIDIV4 + 0.5f),
		m_CameraArmLen(5.0f),
		m_LightDir(0.5f, -1.0f, 0.5f, 1.0f)
	{
		m_LightDir.Normalize();
	}

	void Scene::GetViewProjMatrix(Matrix4X4& View, Matrix4X4& Proj)const {
		View.LookAtLH(m_CamerEye, m_CamerAt, m_CamerUp);
		float w = static_cast<float>(App::GetApp()->GetGameWidth());
		float h = static_cast<float>(App::GetApp()->GetGameHeight());
		Proj.PerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);

	}

	void Scene::GetLightDir(Vector4& LightDir)const {
		LightDir = m_LightDir;
	}


	void Scene::OnCreate() {
		wstring DataDir;
		//サンプルのためアセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(DataDir);
		//各ゲームは以下のようにデータディレクトリを取得すべき
		//App::GetApp()->GetDataDirectory(DataDir);

		wstring strTexture = DataDir + L"sky.jpg";
		//平面の作成
		Quaternion(Vector3(1.0f, 0, 0), XM_PIDIV2);
		m_SquareObject = ObjectFactory::Create<SquareObject>(
			GetThis<Scene>(),
			strTexture,
			Vector3(50.0f, 50.0f, 1.0f),
			Quaternion(Vector3(1.0f, 0, 0), XM_PIDIV2),
			Vector3(0.0f, 0.0f, 0.0f)
			);

		m_BoxVec.push_back(
			ObjectFactory::Create<BoxObject>(
				GetThis<Scene>(), strTexture, false,
				Vector3(5.0f, 0.5f, 5.0f),
				Quaternion(),
				Vector3(5.0f, 0.25f, 0.0f)
				)
		);

		m_BoxVec.push_back(
			ObjectFactory::Create<BoxObject>(
				GetThis<Scene>(), strTexture, false,
				Vector3(3.0f, 1.0f, 3.0f),
				Quaternion(),
				Vector3(5.0f, 0.5f, 0.0f)
				)
		);

		m_BoxVec.push_back(
			ObjectFactory::Create<BoxObject>(
				GetThis<Scene>(), strTexture, false,
				Vector3(3.0f, 1.0f, 3.0f),
				Quaternion(),
				Vector3(-3.5f, 0.5f, 0.0f)
				)
		);

		m_BoxVec.push_back(
			ObjectFactory::Create<BoxObject>(
				GetThis<Scene>(), strTexture, false,
				Vector3(5.0f, 0.5f, 5.0f),
				Quaternion(Vector3(0, 0, 1), -XM_PIDIV4),
				Vector3(-5.0f, 1.0f, 0.0f)
				)
		);


		strTexture = DataDir + L"wall.jpg";

		//移動ボックス
		m_BoxVec.push_back(
			ObjectFactory::Create<MoveBoxObject>(
				GetThis<Scene>(), strTexture, false,
				Vector3(0.25f, 0.5f, 0.5f),
				Quaternion(),
				Vector3(0.0f, 0.25f, 5.0f)
				)
		);


		wstring strTexture2 = DataDir + L"trace.png";
		//球の作成
		m_SphereObject = ObjectFactory::Create<SphereObject>(
			GetThis<Scene>(),
			18, strTexture2, true, Vector3(0.0f, 0.125f, 0.0f));


		//PNT描画オブジェクトの作成
		m_PNTDrawObject = ObjectFactory::Create<PNTDrawObject>(GetThis<Scene>());

		strTexture = DataDir + L"trace.png";
		m_WallSprite = ObjectFactory::Create<WrappedSprite>(strTexture, true,
			Vector2(160, 160),
			Vector2(-480, 260),
			4,4);


	}


	void Scene::OnUpdate() {
		//更新
		m_SquareObject->OnUpdate();
		m_SphereObject->OnUpdate();
		for (auto& v : m_BoxVec) {
			v->OnUpdate();
		}
		//衝突判定
		m_SphereObject->OnCollision();
		for (auto& v : m_BoxVec) {
			v->OnCollision();
		}
		//回転処理
		m_SphereObject->OnRotation();
		for (auto& v : m_BoxVec) {
			v->OnRotation();
		}
		//描画オブジェクトの更新
		m_PNTDrawObject->OnUpdate();
		m_WallSprite->OnUpdate();

		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {

			//Dパッド下
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				//カメラ位置を引く
				m_CameraArmLen += 0.1f;
				if (m_CameraArmLen >= 50.0f) {
					m_CameraArmLen = m_CameraArmLen;
				}
			}
			//Dパッド上
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				//カメラ位置を寄る
				m_CameraArmLen -= 0.1f;
				if (m_CameraArmLen <= 2.0f) {
					m_CameraArmLen = 2.0f;
				}
			}

			if (CntlVec[0].fThumbRX != 0) {
				m_CameraXZRad += CntlVec[0].fThumbRX * 0.02f;
				if (abs(m_CameraXZRad) >= XM_2PI) {
					m_CameraXZRad = 0;
				}
			}
			if (CntlVec[0].fThumbRY != 0) {
				m_CameraYRad -= CntlVec[0].fThumbRY * 0.02f;
				if (m_CameraYRad >= XM_PIDIV2 - 0.1f) {
					m_CameraYRad = XM_PIDIV2 - 0.1f;
				}
				else if (m_CameraYRad <= 0.2) {
					m_CameraYRad = 0.2;
				}
			}

			m_CamerAt = GetSphereObject()->GetPosition();
			Vector3 CameraLocalEye =
				Vector3(
					sin(m_CameraXZRad) * m_CameraArmLen * sin(m_CameraYRad),
					cos(m_CameraYRad) * m_CameraArmLen,
					-cos(m_CameraXZRad) * m_CameraArmLen * sin(m_CameraYRad)
				);
			m_CamerEye = m_CamerAt + CameraLocalEye;
		}
	}
	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Color4(0, 0, 0, 1.0f));
		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		m_SquareObject->OnDraw();
		for (auto& v : m_BoxVec) {
			v->OnDraw();
		}
		m_SphereObject->OnDraw();
		m_PNTDrawObject->OnDraw();
		m_WallSprite->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}

}
//end basecross
