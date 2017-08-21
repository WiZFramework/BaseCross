/*!
@file GameStage.h
@brief ゲームステージ実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ゲームステージ
	//--------------------------------------------------------------------------------------
	GameStage::GameStage():
		m_LightDir(0.5f, -1.0f, 0.5f, 1.0f)
	{
		m_LightDir.normalize();
	}

	void GameStage::OnCreate() {
		//平面の作成
		Quat(Vector3(1.0f, 0, 0), XM_PIDIV2);
		m_SquareObject = ObjectFactory::Create<SquareObject>(
			GetThis<Stage>(),
			L"SKY_TX",
			Vector3(50.0f, 50.0f, 1.0f),
			Quat::rotationX(XM_PIDIV2),
			Vector3(0.0f, 0.0f, 0.0f)
			);
		//球の作成
		m_SphereObject = ObjectFactory::Create<SphereObject>(
			GetThis<Stage>(),
			18, L"TRACE_TX", true, Vector3(0.0f, 0.125f, 0.0f));
		//PNT描画オブジェクトの作成
		m_PNTDrawObject = ObjectFactory::Create<PNTDrawObject>(
			GetThis<Stage>()
		);
		m_RotateSprite = ObjectFactory::Create<RotateSprite>(
			GetThis<Stage>(),
			L"TRACE_TX",
			Vector2(160, 160),
			0.0f,
			Vector2(-480, 260),
			4, 4);
		m_MessageSprite = ObjectFactory::Create<MessageSprite>(
			GetThis<Stage>(),
			L"MESSAGE_TX",
			Vector2(256, 64),
			0.0f,
			Vector2(480, 260),
			1, 1);
	}

	void GameStage::OnUpdateStage() {
		//プレートの更新
		m_SquareObject->OnUpdate();
		//球体の更新
		m_SphereObject->OnUpdate();
		//回転処理
		m_SphereObject->OnRotation();
		//描画オブジェクトの更新
		m_PNTDrawObject->OnUpdate();
		//スプライトの更新
		m_RotateSprite->OnUpdate();
		m_MessageSprite->OnUpdate();
		//自分自身の更新
		this->OnUpdate();
	}


	void GameStage::OnUpdate() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {

			//Dパッド下
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				//カメラ位置を引く
				m_Camera.m_CameraArmLen += 0.1f;
				if (m_Camera.m_CameraArmLen >= 50.0f) {
					m_Camera.m_CameraArmLen = 50.0f;
				}
			}
			//Dパッド上
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				//カメラ位置を寄る
				m_Camera.m_CameraArmLen -= 0.1f;
				if (m_Camera.m_CameraArmLen <= 2.0f) {
					m_Camera.m_CameraArmLen = 2.0f;
				}
			}

			if (CntlVec[0].fThumbRX != 0) {
				m_Camera.m_CameraXZRad += CntlVec[0].fThumbRX * 0.02f;
				if (abs(m_Camera.m_CameraXZRad) >= XM_2PI) {
					m_Camera.m_CameraXZRad = 0;
				}
			}
			if (CntlVec[0].fThumbRY != 0) {
				m_Camera.m_CameraYRad -= CntlVec[0].fThumbRY * 0.02f;
				if (m_Camera.m_CameraYRad >= XM_PIDIV2 - 0.1f) {
					m_Camera.m_CameraYRad = XM_PIDIV2 - 0.1f;
				}
				else if (m_Camera.m_CameraYRad <= 0.2) {
					m_Camera.m_CameraYRad = 0.2;
				}
			}

			m_Camera.m_CamerAt = GetSphereObject()->GetPosition();
			Vec3 CameraLocalEye =
				Vec3(
					sin(m_Camera.m_CameraXZRad) * m_Camera.m_CameraArmLen * sin(m_Camera.m_CameraYRad),
					cos(m_Camera.m_CameraYRad) * m_Camera.m_CameraArmLen,
					-cos(m_Camera.m_CameraXZRad) * m_Camera.m_CameraArmLen * sin(m_Camera.m_CameraYRad)
				);
			m_Camera.m_CamerEye = m_Camera.m_CamerAt + CameraLocalEye;
			//Bボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToEmptyStage");
			}
		}
	}

	void GameStage::OnDrawStage() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Color4(0, 0, 0, 1.0f));
		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		//プレート描画
		m_SquareObject->OnDraw();
		//球体描画
		m_SphereObject->OnDraw();
		//描画オブジェクト描画
		m_PNTDrawObject->OnDraw();
		//スプライト描画
		m_RotateSprite->OnDraw();
		m_MessageSprite->OnDraw();
		//自分自身の描画
		this->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}


	void GameStage::OnDraw() {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	///	空のステージ（メッセージのみある）
	//--------------------------------------------------------------------------------------
	void EmptyStage::OnCreate() {
		//メッセージスプライト
		m_MessageSprite = ObjectFactory::Create<MessageSprite>(
			GetThis<Stage>(),
			L"MESSAGE_TX",
			Vector2(256, 64),
			0.0f,
			Vector2(0, 0),
			1, 1);
	}
	void EmptyStage::OnUpdateStage() {
		//スプライトの更新
		m_MessageSprite->OnUpdate();
		//自分自身の更新
		this->OnUpdate();
	}
	void EmptyStage::OnUpdate() {
		//コントローラの取得
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		if (CntlVec[0].bConnected) {
			//Bボタン
			if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
				PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
			}
		}
	}

	void EmptyStage::OnDrawStage() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefaultViews(Color4(0, 0, 0, 1.0f));
		//デフォルト描画の開始
		Dev->StartDefaultDraw();
		//スプライト描画
		m_MessageSprite->OnDraw();
		//自分自身の描画
		this->OnDraw();
		//デフォルト描画の終了
		Dev->EndDefaultDraw();
	}


	void EmptyStage::OnDraw() {
		//何もしない
	}


}
//end basecross
