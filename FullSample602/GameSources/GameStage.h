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
		//XML���[�_�[
		unique_ptr<XmlDocReader> m_XmlDocReader;
		//�r���[�̍쐬
		void CreateViewLight();
		//�v���[�g�̍쐬
		void CreatePlate();
		//�{�b�N�X�̍쐬
		void CreateFixedBox();
		//�v���C���[�̍쐬
		void CreatePlayer();
		//�G�̍쐬
		void CreateEnemy();
	public:
		//�\�z�Ɣj��
		GameStage() :Stage() {}
		virtual ~GameStage() {}
		//������
		virtual void OnCreate()override;
	};


}
//end basecross
