/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{
	//--------------------------------------------------------------------------------------
	//	文字列を保持するだけのオブジェクト
	//--------------------------------------------------------------------------------------
	StringGameObject::StringGameObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr)
	{}
	StringGameObject::~StringGameObject() {}

	//初期化
	void StringGameObject::OnCreate() {
		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetFont(L"ＭＳゴシック", 32);
		PtrString->SetText(L"");
		PtrString->SetTextRect(Rect2D<float>(440.0f, 640.0f, 1200.0f, 800.0f));

	}

	void StringGameObject::SetText(const wstring& str) {
		auto PtrString = GetComponent<StringSprite>();
		PtrString->SetText(str);
	}



}
//end basecross
