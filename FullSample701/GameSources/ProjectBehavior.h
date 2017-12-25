/*!
@file ProjectBehavior.h
@brief プロジェク定義の行動クラス
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	コントローラのボタンのハンドラ
	//--------------------------------------------------------------------------------------
	template<typename T>
	struct InputHandler {
		void ButtonHandle(const shared_ptr<T>& Obj) {
			//コントローラの取得
			auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
			if (CntlVec[0].bConnected) {
				//Aボタン
				if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_A) {
					Obj->OnPushA();
				}
				//Bボタン
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_B) {
					Obj->OnPushB();
				}
				//Xボタン
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_X) {
					Obj->OnPushX();
				}
				//Yボタン押した瞬間
				else if (CntlVec[0].wPressedButtons & XINPUT_GAMEPAD_Y) {
					Obj->OnPushY();
				}
				//Yボタン押しっぱなし
				else if (CntlVec[0].wButtons & XINPUT_GAMEPAD_Y) {
					Obj->OnPressY();
				}
				//Yボタン離す
				else if (CntlVec[0].wReleasedButtons & XINPUT_GAMEPAD_Y) {
					Obj->OnReleaseY();
				}
			}
		}
	};


}

//end basecross

