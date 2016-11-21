/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//構築と破棄
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~FixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};


	//--------------------------------------------------------------------------------------
	//	class RoomDoor : public GameObject;
	//	用途: 部屋のドア
	//--------------------------------------------------------------------------------------
	class RoomDoor : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
		wstring m_TexResName;
	public:
		//構築と破棄
		RoomDoor(const shared_ptr<Stage>& StagePtr,
			const wstring& TexResName,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~RoomDoor();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	部屋の窓
	//--------------------------------------------------------------------------------------
	class RoomWindow : public GameObject {
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//構築と破棄
		RoomWindow(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
		);
		virtual ~RoomWindow();
		//初期化
		virtual void OnCreate() override;
		//操作
		void WindowClear();
	};

	
	//--------------------------------------------------------------------------------------
	//	class PopupBalloon : public GameObject;
	//	用途:　吹き出し
	//--------------------------------------------------------------------------------------
	class PopupBalloon : public GameObject {
		Vector3 m_Scale;
		Vector3 m_EmitterPosition;
		wstring m_TexResName;
	public:
		//構築と破棄
		PopupBalloon(const shared_ptr<Stage>& StagePtr,
			const wstring& TexResName,
			const Vector3& Scale
		);
		virtual ~PopupBalloon();
		//初期化
		virtual void OnCreate() override;
		//操作
		virtual void OnUpdate() override;
		//表示
		void PopupOpen(const Vector3& EmitterPosition);
		//クローズ
		void PopupClose();
	};

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
