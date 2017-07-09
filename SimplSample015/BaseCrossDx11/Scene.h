/*!
@file Scene.h
@brief �V�[���Ȃ�
*/
#pragma once
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	///	�Q�[���V�[��
	//--------------------------------------------------------------------------------------
	class Scene : public SceneInterface {
		shared_ptr<SquareObject> m_SquareObject;				///<���ʃI�u�W�F�N�g
		shared_ptr<SphereObject> m_SphereObject;				///<���I�u�W�F�N�g
		vector<shared_ptr<BoxObject>> m_BoxObjectVec;		///<�{�b�N�X�I�u�W�F�N�g�̔z��
		shared_ptr<PNTDrawObject>	m_PNTDrawObject;				///<�`��I�u�W�F�N�g
		Vector3 m_CamerEye;			///<�J�����ʒu
		Vector3 m_CamerAt;			///<�J�������_
		Vector3 m_CamerUp;			///<�J�����X��
		Vector4 m_LightDir;			///<���C�g����
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���I�u�W�F�N�g�̎擾
		@return	���I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<SphereObject> GetSphereObject() const {
			return m_SphereObject;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �{�b�N�X�I�u�W�F�N�g�̔z��̎擾
		@return	�{�b�N�X�I�u�W�F�N�g�̔z��
		*/
		//--------------------------------------------------------------------------------------
		const vector<shared_ptr<BoxObject>>& GetBoxObjectVec() const {
			return m_BoxObjectVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��I�u�W�F�N�g�̎擾
		@return	�`��I�u�W�F�N�g
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<PNTDrawObject> GetPNTDrawObject() const {
			return m_PNTDrawObject;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief �r���[�s��Ǝˉe�s��̎擾�i�e�Q�ƃp�����[�^�ɑ���j
		@param[out]	View	�r���[�s����󂯎��Q��
		@param[out]	Proj	�ˉe�s����󂯎��Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetViewProjMatrix(Matrix4X4& View, Matrix4X4& Proj)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ���C�g�����̎擾
		@param[out]	LightDir	���C�g�����󂯎��Q��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		void GetLightDir(Vector4& LightDir)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ������
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �X�V
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief �`��
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


}
// end basecross