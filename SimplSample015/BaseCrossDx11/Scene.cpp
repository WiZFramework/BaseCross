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
		m_CamerEye(0, 5.0, -10.0f),
		m_CamerAt(0, 0, 0),
		m_CamerUp(0, 1.0f, 0),
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
		wstring strTexture = App::GetApp()->m_wstrRelativeDataPath + L"sky.jpg";
		//平面の作成
		Quaternion(Vector3(1.0f, 0, 0), XM_PIDIV2);
		auto PtrSquareObject = ObjectFactory::Create<SquareObject>(
			GetThis<Scene>(),
			strTexture,
			Vector3(40.0f, 20.0f, 1.0f),
			Quaternion(Vector3(1.0f, 0, 0), XM_PIDIV2),
			Vector3(15.0f, 0.0f, 0.0f)
			);
		m_GameObjectVec.push_back(PtrSquareObject);

		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"trace.png";
		//球の作成
		m_Player = ObjectFactory::Create<Player>(
			GetThis<Scene>(),
			18, strTexture, true, Vector3(0.0f, 0.5f, 0.0f));
		m_GameObjectVec.push_back(m_Player);


		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"wall.jpg";
		//敵の作成
		auto EnemyPtr = ObjectFactory::Create<SphereObject>(
			GetThis<Scene>(),
			18, strTexture, false, Vector3(5.0f, 0.5f, 0.0f));
		m_GameObjectVec.push_back(EnemyPtr);



		strTexture = App::GetApp()->m_wstrRelativeDataPath + L"wall.jpg";
		//立方体の作成
		auto CubePtr = ObjectFactory::Create<CubeObject>(
			GetThis<Scene>(),
			strTexture,
			Vector3(5.0f, 1.0f, 5.0f),
			Quaternion(Vector3(0, 0, 1.0f), 0.5f),
			Vector3(0.0f, 0.5f, 5.0f)
			);
		m_CubeObjectVec.push_back(CubePtr);
		m_GameObjectVec.push_back(CubePtr);

		CubePtr = ObjectFactory::Create<CubeObject>(
			GetThis<Scene>(),
			strTexture,
			Vector3(5.0f, 1.0f, 5.0f),
			Quaternion(Vector3(0, 1.0f, 0), 0),
			Vector3(10.0f, 0.5f, 5.0f)
			);
		m_CubeObjectVec.push_back(CubePtr);
		m_GameObjectVec.push_back(CubePtr);

		CubePtr = ObjectFactory::Create<CubeObject>(
			GetThis<Scene>(),
			strTexture,
			Vector3(5.0f, 1.0f, 5.0f),
			Quaternion(Vector3(0, 1.0f, 0), 0),
			Vector3(15.0f, 0.5f, 5.0f)
			);
		m_CubeObjectVec.push_back(CubePtr);
		m_GameObjectVec.push_back(CubePtr);

		CubePtr = ObjectFactory::Create<CubeObject>(
			GetThis<Scene>(),
			strTexture,
			Vector3(8.0f, 1.0f, 5.0f),
			Quaternion(Vector3(0, 0, 1.0f), -0.5f),
			Vector3(20.5f, 1.5f, 5.0f)
			);
		m_CubeObjectVec.push_back(CubePtr);
		m_GameObjectVec.push_back(CubePtr);


	}

	void Scene::OnUpdate() {

		for (auto& v : m_GameObjectVec) {
			v->ComponentPreparation();
		}
		for (auto& v : m_GameObjectVec) {
			v->OnUpdate();
		}
		for (auto& v : m_GameObjectVec) {
			v->ComponentUpdate();
		}
		for (auto& v : m_GameObjectVec) {
			v->OnLastUpdate();
		}
		//プレイヤーのX座標にカメラを合わせる
		auto PtrTrans = m_Player->GetComponent<Transform>();
		m_CamerEye.x = PtrTrans->GetPosition().x;
		m_CamerAt.x = PtrTrans->GetPosition().x;
	}

	void Scene::OnDraw() {
		//描画デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		Dev->ClearDefultViews(Color4(0, 0, 0, 1.0f));
		//デフォルト描画の開始
		Dev->StartDefultDraw();
		for (auto& v : m_GameObjectVec) {
			v->OnDraw();
		}
		//デフォルト描画の終了
		Dev->EndDefultDraw();
	}

}
//end basecross
