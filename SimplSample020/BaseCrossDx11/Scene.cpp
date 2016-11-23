
/*!
@file Scene.cpp
@brief シーン実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------

	//リソースの作成
	void Scene::CreateResourses() {
		wstring DataDir;
		App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);

		wstring ModelDir = DataDir + L"Model\\";
		auto StaticModelMesh = MeshResource::CreateStaticModelMesh(ModelDir, L"Character_01.bmf");
		App::GetApp()->RegisterResource(L"PLAYER_MESH", StaticModelMesh);


		Shadowmap::SetViewSize(8.0f);


	}


	void Scene::OnCreate(){
		try {
			CreateResourses();
			//自分自身にイベントを送る
			//これにより各ステージやオブジェクトがCreate時にシーンにアクセスできる
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToGameStage");
		}
		catch (...) {
			throw;
		}
	}

	Scene::~Scene() {
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			ResetActiveStage<GameStage>();
		}
	}





}
//end basecross
