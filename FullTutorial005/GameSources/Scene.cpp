
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

		//プレート用
		wstring strTexture = DataDir + L"gray.png";
		App::GetApp()->RegisterTexture(L"GRAY_TX", strTexture);
		strTexture = DataDir + L"normal3.png";
		App::GetApp()->RegisterTexture(L"NORMAL3_TX", strTexture);
		//プレイヤー用
		strTexture = DataDir + L"Brown.png";
		App::GetApp()->RegisterTexture(L"BROWN_TX", strTexture);
		strTexture = DataDir + L"normal1.png";
		App::GetApp()->RegisterTexture(L"NORMAL1_TX", strTexture);
		//Seekオブジェクト用
		strTexture = DataDir + L"Green.png";
		App::GetApp()->RegisterTexture(L"GREEN_TX", strTexture);
		strTexture = DataDir + L"normal2.png";
		App::GetApp()->RegisterTexture(L"NORMAL2_TX", strTexture);
		//Fixed用
		strTexture = DataDir + L"Blue.png";
		App::GetApp()->RegisterTexture(L"BLUE_TX", strTexture);
		//L"NORMAL2_TX"
		strTexture = DataDir + L"Wall2.png";
		App::GetApp()->RegisterTexture(L"WALL2_TX", strTexture);
		strTexture = DataDir + L"Wall2_normal.png";
		App::GetApp()->RegisterTexture(L"WALL2_NORMAL_TX", strTexture);
		strTexture = DataDir + L"wall.jpg";
		App::GetApp()->RegisterTexture(L"WALL_TX", strTexture);
		strTexture = DataDir + L"wall_normal.png";
		App::GetApp()->RegisterTexture(L"WALL_NORMAL_TX", strTexture);
		//L"BROWN_TX"
		strTexture = DataDir + L"Floor_normal.png";
		App::GetApp()->RegisterTexture(L"FLOOR_NORMAL_TX", strTexture);
		//MoveBox用
		strTexture = DataDir + L"Red.png";
		App::GetApp()->RegisterTexture(L"RED_TX", strTexture);
		//FLOOR_NORMAL_TX
		//UnevenGround用
		strTexture = DataDir + L"Brown2.png";
		App::GetApp()->RegisterTexture(L"BROWN2_TX", strTexture);
		//WALL2_NORMAL_TX

		//その他
		strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"Floor.png";
		App::GetApp()->RegisterTexture(L"FLOOR_TX", strTexture);

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

	void Scene::OnEvent(const shared_ptr<Event>& event) {
		if (event->m_MsgStr == L"ToGameStage") {
			//最初のアクティブステージの設定
			ResetActiveStage<GameStage>();
		}
	}


}
//end basecross
