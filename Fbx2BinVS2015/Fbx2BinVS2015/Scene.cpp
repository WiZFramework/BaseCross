
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

	void Scene::OnCreate(){
		try {
			m_pFbxManager.reset(FbxManager::Create());
			FbxIOSettings *ios = FbxIOSettings::Create(m_pFbxManager.get(), IOSROOT);
			m_pFbxManager->SetIOSettings(ios);

			//最初のアクティブステージの設定
			ResetActiveStage<GameStage>();
		}
		catch (...) {
			throw;
		}
	}

	void Scene::ReadFbxFile(const wstring& Dir, const wstring& FileName,size_t MeshIndex, float Scale, const Vec3& Position,
		bool IsReadStatic, bool WithTangent, const wstring& NormalFileName, bool TextureWrap) {
		try {
			GetActiveTypeStage<GameStage>()->ReadFbxFile(Dir, FileName, MeshIndex, Scale, Position, IsReadStatic, WithTangent,
				NormalFileName, TextureWrap);
		}
		catch (...) {
			throw;
		}
	}

	void Scene::SaveStaticBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale) {
		try {
			GetActiveTypeStage<GameStage>()->SaveStaticBinFile(Dir, FileName, MeshIndex, Scale);
		}
		catch (...) {
			throw;
		}
	}

	void Scene::SaveSkinBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale,
		UINT FrameParSec, UINT Start, UINT End) {
		try {
			GetActiveTypeStage<GameStage>()->SaveSkinBinFile(Dir, FileName, MeshIndex, Scale, FrameParSec, Start, End);
		}
		catch (...) {
			throw;
		}

	}



	bool Scene::CheckSkinMesh() {
		return GetActiveTypeStage<GameStage>()->CheckSkinMesh();
	}

	bool Scene::CheckMesh() {
		return GetActiveTypeStage<GameStage>()->CheckMesh();
	}
	void Scene::ResetNewStage() {
		try {
			GetActiveTypeStage<GameStage>()->ClearFbxMesh();
			//ゲームステージの作成しなおし
			ResetActiveStage<GameStage>();
		}
		catch (...) {
			throw;
		}

	}


	void Scene::MoveFbxFile(UINT FrameRate, UINT StartTime, UINT EndTime, bool IsLoop) {
		GetActiveTypeStage<GameStage>()->MoveFbxFile(FrameRate, StartTime, EndTime, IsLoop);
	}

	void Scene::AnimePoseStart() {
		GetActiveTypeStage<GameStage>()->AnimePoseStart();

	}






}
//end basecross
