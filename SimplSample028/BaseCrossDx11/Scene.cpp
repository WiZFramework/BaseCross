/*!
@file Scene.cpp
@brief シーンなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	ステージ（シーンで管理するインターフェイス）
	//--------------------------------------------------------------------------------------
	void Stage::OnPreCreate() {
		//Rigidbodyマネージャの初期化
		m_RigidbodyManager
			= ObjectFactory::Create<RigidbodyManager>(GetThis<Stage>());
	}


	//追加オブジェクトの指定
	void Stage::PushBackGameObject(const shared_ptr<GameObject>& Ptr) {
		//このステージはクリエイト後である
		if (IsCreated()) {
			m_WaitAddObjectVec.push_back(Ptr);
		}
		else {
			//クリエイト前
			m_GameObjectVec.push_back(Ptr);
		}
	}
	//削除オブジェクトの指定
	void Stage::RemoveBackGameObject(const shared_ptr<GameObject>& Ptr) {
		m_WaitRemoveObjectVec.push_back(Ptr);
	}
	//オブジェクトの削除
	void Stage::RemoveTargetGameObject(const shared_ptr<GameObject>& targetobj) {
		auto it = m_GameObjectVec.begin();
		while (it != m_GameObjectVec.end()) {
			if (*it == targetobj) {
				m_GameObjectVec.erase(it);
				return;
			}
			it++;
		}
		m_RigidbodyManager->RemoveOwnRigidbody(targetobj);
	}
	//追加や削除待ちになってるオブジェクトを追加削除する
	void Stage::SetWaitToObjectVec() {
		if (!m_WaitRemoveObjectVec.empty()) {
			for (auto Ptr : m_WaitRemoveObjectVec) {
				RemoveTargetGameObject(Ptr);
			}
		}
		m_WaitRemoveObjectVec.clear();
		if (!m_WaitAddObjectVec.empty()) {
			for (auto Ptr : m_WaitAddObjectVec) {
				m_GameObjectVec.push_back(Ptr);
			}
		}
		m_WaitAddObjectVec.clear();
	}

	void Stage::FindTagGameObjectVec(const wstring& TagName, vector<shared_ptr<GameObject>>& Ret) const {
		Ret.clear();
		for (auto& v : GetGameObjectVec()) {
			if (v->FindTag(TagName)) {
				Ret.push_back(v);
			}
		}
	}

	shared_ptr<RigidbodyManager> Stage::GetRigidbodyManager() const {
		return m_RigidbodyManager;
	}

	shared_ptr<Rigidbody> Stage::AddRigidbody(const Rigidbody& body) {
		return m_RigidbodyManager->AddRigidbody(body);
	}

	void Stage::RemoveOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr) {
		m_RigidbodyManager->RemoveOwnRigidbody(OwnerPtr);
	}

	const vector<shared_ptr<Rigidbody>>& Stage::GetRigidbodyVec()const {
		return m_RigidbodyManager->GetRigidbodyVec();
	}
	const vector<CollisionState>& Stage::GetCollisionStateVec()const {
		return m_RigidbodyManager->GetCollisionStateVec();
	}
	shared_ptr<Rigidbody> Stage::GetOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr) {
		return m_RigidbodyManager->GetOwnRigidbody(OwnerPtr);
	}




	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	Scene::Scene() :
		SceneInterface()
	{
	}

	void Scene::CreateResources() {
		wstring DataDir;
		//サンプルのためアセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(DataDir);
		//各ゲームは以下のようにデータディレクトリを取得すべき
		//App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"StageMessage.png";
		App::GetApp()->RegisterTexture(L"MESSAGE_TX", strTexture);

		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		strTexture = DataDir + L"wall_normal.png";
		App::GetApp()->RegisterTexture(L"WALL_NORMAL_TX", strTexture);

		//ボーンモデルのタンジェント付きリソース
		auto ModelMesh = MeshResource::CreateBoneModelMeshWithTangent(DataDir, L"TangentTest.bmf");
		App::GetApp()->RegisterResource(L"TANGENT_TEST_MESH", ModelMesh);
		//法線マップ
		strTexture = DataDir + L"NormalMap.png";
		App::GetApp()->RegisterTexture(L"TANGENT_TEST_TX", strTexture);

		//エフェクト
		strTexture = DataDir + L"spark.png";
		App::GetApp()->RegisterTexture(L"SPARK_TX", strTexture);
		strTexture = DataDir + L"fire.png";
		App::GetApp()->RegisterTexture(L"FIRE_TX", strTexture);


		//サウンド
		wstring CursorWav = DataDir + L"cursor.wav";
		App::GetApp()->RegisterWav(L"cursor", CursorWav);

		//BGM
		wstring strMusic = DataDir + L"nanika .wav";
		App::GetApp()->RegisterWav(L"Nanika", strMusic);



	}


	void Scene::OnCreate() {
		CreateResources();

		//BGMの再生
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Start(L"Nanika", XAUDIO2_LOOP_INFINITE, 0.1f);

		//自分自身にイベントを送る
		//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
		PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
	}

	void Scene::OnUpdate() {
		GetActiveStage()->OnPreUpdateStage();
		GetActiveStage()->OnUpdateStage();
	}
	void Scene::OnDraw() {
		GetActiveStage()->OnDrawStage();
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//アクティブステージをGameStageに設定
			ResetActiveStage<GameStage>();
		}
		else if (event->m_MsgStr == L"ToEmptyStage") {
			//アクティブステージをEmptyStageに設定
			ResetActiveStage<EmptyStage>();
		}
	}

	void Scene::OnDestroy() {

		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Stop(m_BGM);

	}

}
//end basecross
