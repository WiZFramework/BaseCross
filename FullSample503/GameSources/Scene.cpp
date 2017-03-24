
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
		//各ゲームは以下のようにデータディレクトリを取得すべき
		//App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);

		//データディレクトリを取得
		wstring MediaDir;
		App::GetApp()->GetDataDirectory(MediaDir);
		//スタティックモデルのタンジェント付きリソース
		auto StaticModelMesh = MeshResource::CreateStaticModelMeshWithTangent(MediaDir + L"TestFbx\\", L"TangentTestNonAnimation.bmf");
		App::GetApp()->RegisterResource(L"MODEL_MESH_WITH_TAN", StaticModelMesh);
		//ボーンモデルのタンジェント付きリソース
		auto BoneModelMesh = MeshResource::CreateBoneModelMeshWithTangent(MediaDir + L"TestFbx\\", L"TangentTest.bmf");
		App::GetApp()->RegisterResource(L"BONE_MESH_WITH_TAN", BoneModelMesh);

		//法線マップ
		strTexture = MediaDir + L"TestFbx\\" + L"NormalMap.png";
		App::GetApp()->RegisterTexture(L"MODEL_NORMAL_TX", strTexture);


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
