
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
		wstring strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"trace2.png";
		App::GetApp()->RegisterTexture(L"TRACE2_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"Floor.png";
		App::GetApp()->RegisterTexture(L"FLOOR_TX", strTexture);
		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		strTexture = DataDir + L"Wall2.png";
		App::GetApp()->RegisterTexture(L"WALL2_TX", strTexture);
		strTexture = DataDir + L"mozo.png";
		App::GetApp()->RegisterTexture(L"MOZO_TX", strTexture);
		strTexture = DataDir + L"Balloon0.png";
		App::GetApp()->RegisterTexture(L"BALLOON_0_TX", strTexture);
		
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
			PostEvent(0.0f, GetThis<ObjectInterface>(), GetThis<Scene>(), L"ToNowScienceRoom");
		}
		catch (...) {
			throw;
		}
	}

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ChangeScienceRoom") {
			auto NowStagePtr = dynamic_pointer_cast<NowScienceRoom>(GetActiveStage());
			if (NowStagePtr) {
				//現在の理科室
				ResetActiveStage<PastScienceRoom>();
			}
			else {
				//いにしえの理科室
				ResetActiveStage<NowScienceRoom>();
			}
		}
		else if (event->m_MsgStr == L"ToNowScienceRoom") {
			ResetActiveStage<NowScienceRoom>();
		}
		else if (event->m_MsgStr == L"ToPastScienceRoom") {
			ResetActiveStage<PastScienceRoom>();
		}
		else if (event->m_MsgStr == L"ToClear") {
			ResetActiveStage<ClearStage>();
		}
		else if (event->m_MsgStr == L"ToGameOver") {
			ResetActiveStage<GameOverStage>();
		}

	}




}
//end basecross
