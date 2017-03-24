/*!
@file GameStage.h
@brief ゲームステージ
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	ゲームステージクラス
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		//リソースの作成
		void CreateResourses();
		//ビューの作成
		void CreateViewLight();
		//プレートの作成
		void CreatePlate();
		//FbxMeshObjectの作成
		void CreateFbxMeshObject();
		//プレイヤーの作成
		void CreatePlayer();
	public:
		//構築と破棄
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//初期化
		virtual void OnCreate()override;

		void ReadFbxFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale, const Vector3& Position,
			bool IsReadStatic,bool WithTangent, const wstring& NormalFileName);

		void SaveStaticBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale);
		void SaveSkinBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale,
			UINT FrameParSec, UINT Start, UINT End);



		void ClearFbxMesh();

		bool CheckSkinMesh();
		bool CheckMesh();


		void MoveFbxFile(UINT FrameRate, UINT StartTime, UINT EndTime, bool IsLoop);

		void AnimePoseStart();


	};


}
//end basecross

