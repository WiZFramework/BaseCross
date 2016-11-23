/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	文字列を保持するだけのオブジェクト
	//--------------------------------------------------------------------------------------
	class StringGameObject : public GameObject {
	public:
		//構築と破棄
		StringGameObject(const shared_ptr<Stage>& StagePtr);
		virtual ~StringGameObject();
		//初期化
		virtual void OnCreate() override;
		void SetText(const wstring& str);
	};



}
//end basecross
