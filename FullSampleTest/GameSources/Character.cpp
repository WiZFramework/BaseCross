/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class NormalTextureBox  : public ObjectInterface;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	NormalTextureBox::NormalTextureBox(const shared_ptr<Stage>& StagePtr):
		GameObject(StagePtr)
	{
	}
	NormalTextureBox::~NormalTextureBox() {}


	void NormalTextureBox::OnCreateWithParam(const Vector3& Pos) {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(Vector3(1.0f,1.0f,1.0f));
		Quaternion Qt;
		Qt.Identity();
		PtrTrans->SetQuaternion(Qt);
		PtrTrans->SetPosition(Pos);

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetTextureResource(L"WALL_TX");
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
	}

	void NormalTextureBox::OnUpdate() {
		auto PtrTrans = GetComponent<Transform>();
		auto Qt = PtrTrans->GetQuaternion();
		Quaternion QtSpan;
		QtSpan.RotationAxis(Vector3(0, 1.0f, 0), 0.02f);
		Qt *= QtSpan;
		Qt.Normalize();
		PtrTrans->SetQuaternion(Qt);
	}




}
//end basecross
