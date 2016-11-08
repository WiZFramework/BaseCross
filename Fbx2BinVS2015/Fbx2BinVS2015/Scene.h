/*!
@file Scene.h
@brief シーン
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	class Scene : public SceneBase{
		//FBXマネージャーのデリーター
		struct FbxManagerDeleter
		{
			void operator()(FbxManager *p) {
				p->Destroy();
			}
		};
		//FBXマネージャ
		unique_ptr<FbxManager, FbxManagerDeleter> m_pFbxManager;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene() :SceneBase(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief FBXマネージャの取得
		@return	FBXマネージャ
		*/
		//--------------------------------------------------------------------------------------
		unique_ptr<FbxManager, FbxManagerDeleter>& GetFbxManager() {
			return m_pFbxManager;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;

		void ReadFbxFile(const wstring& Dir, const wstring& FileName,size_t MeshIndex,float Scale,const Vector3& Position,
			bool IsReadStatic);

		bool CheckSkinMesh();
		bool CheckMesh();

		void ResetNewStage();

		void MoveFbxFile(UINT FrameRate, UINT StartTime, UINT EndTime, bool IsLoop);

		void SaveStaticBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale);

		void SaveSkinBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale,
			UINT FrameParSec, UINT Start, UINT End);


		void AnimePoseStart();

	};

}

//end basecross
