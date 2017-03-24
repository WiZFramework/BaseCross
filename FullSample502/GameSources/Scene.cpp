
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
	void Scene::CreateResourses() {
		wstring DataDir;
		//サンプルのためアセットディレクトリを取得
		App::GetApp()->GetAssetsDirectory(DataDir);
		wstring strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);

		//データディレクトリを取得
		wstring MediaDir;
		App::GetApp()->GetDataDirectory(MediaDir);
		//スタティックモデルのリソース
		auto StaticModelMesh = MeshResource::CreateStaticModelMesh(MediaDir + L"Chara1\\", L"Character_01.bmf");
		App::GetApp()->RegisterResource(L"MODEL_MESH", StaticModelMesh);
		//ボーンモデルのリソース
		auto BoneModelMesh = MeshResource::CreateBoneModelMesh(MediaDir + L"Chara_R\\", L"Chara_R.bmf");
		App::GetApp()->RegisterResource(L"BONE_MESH", BoneModelMesh);

	}

	void Scene::OnCreate(){
		try {
			//リソース作成
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
			//最初のアクティブステージの設定
			ResetActiveStage<GameStage>();
		}
	}



}
//end basecross
