/*!
@file GameStage.h
@brief �Q�[���X�e�[�W
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	class GameStage : public Stage {
		//���\�[�X�̍쐬
		void CreateResourses();
		//�r���[�̍쐬
		void CreateViewLight();
		//�v���[�g�̍쐬
		void CreatePlate();
		//FbxMeshObject�̍쐬
		void CreateFbxMeshObject();
		//�v���C���[�̍쐬
		void CreatePlayer();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;

		void ReadFbxFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale, const Vector3& Position,
			bool IsReadStatic);

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
