
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
		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		strTexture = DataDir + L"number.png";
		App::GetApp()->RegisterTexture(L"NUMBER_TX", strTexture);

		//ボーンモデルの通常リソース
		auto ModelMesh = MeshResource::CreateBoneModelMesh(DataDir, L"Chara_R.bmf");
		App::GetApp()->RegisterResource(L"Chara_R_MESH", ModelMesh);
		//ボーンモデルのタンジェント付きリソース
		ModelMesh = MeshResource::CreateBoneModelMeshWithTangent(DataDir, L"Chara_R.bmf");
		App::GetApp()->RegisterResource(L"Chara_R_MESH_WITH_TAN", ModelMesh);
		//法線マップ
		strTexture = DataDir + L"Chara_R_narmal.png";
		App::GetApp()->RegisterTexture(L"Chara_R_NORMAL_TX", strTexture);



		//スタティックモデルの通常リソース
		auto StaticModelMesh = MeshResource::CreateStaticModelMesh(DataDir, L"Character_01.bmf");
		App::GetApp()->RegisterResource(L"MODEL_MESH", StaticModelMesh);
		//スタティックモデルのタンジェント付きリソース
		StaticModelMesh = MeshResource::CreateStaticModelMeshWithTangent(DataDir, L"Character_01.bmf");
		App::GetApp()->RegisterResource(L"MODEL_MESH_WITH_TAN", StaticModelMesh);
		//法線マップ
		strTexture = DataDir + L"Character_2_normal.png";
		App::GetApp()->RegisterTexture(L"MODEL_NORMAL_TX", strTexture);
		

	}

	void Scene::OnCreate() {
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

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//最初のアクティブステージの設定
			ResetActiveStage<GameStage>();
		}
	}


}
//end basecross
