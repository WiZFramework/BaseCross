/*!
@file Character.h
@brief �L�����N�^�[�Ȃ�
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	�l�p�`�X�v���C�g
	//--------------------------------------------------------------------------------------
	class SquareSprite : public ObjectInterface, public ShapeInterface {
		///���b�V��
		shared_ptr<MeshResource> m_SquareMesh;
		wstring m_TextureFileName;		///<�e�N�X�`���t�@�C����
		bool m_Trace;					///<�����������邩�ǂ���
		shared_ptr<TextureResource> m_TextureResource;	///<�e�N�X�`�����\�[�X
		Vector2 m_Scale;				///<�X�P�[�����O
		float m_Rot;				///<��]�p�x
		Vector2 m_Pos;				///<�ʒu
		Vector2 m_PosSpan;				///<�ʒu�ύX�Ԋu
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief �R���X�g���N�^
		@param[in]	TextureFileName	�e�N�X�`���t�@�C����
		@param[in]	Trace	�����������邩�ǂ���
		@param[in]	StartPos	�����ʒu
		*/
		//--------------------------------------------------------------------------------------
		SquareSprite(const wstring& TextureFileName, bool Trace, const Vector2& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief �f�X�g���N�^
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareSprite();
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
//end basecross