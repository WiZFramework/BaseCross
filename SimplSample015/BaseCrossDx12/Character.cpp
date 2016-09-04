/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	平面実体
	//--------------------------------------------------------------------------------------
	SquareObject::SquareObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, const Vector3& Scale, const Quaternion& Qt, const Vector3& Pos) :
		GameObject(PtrScene),
		m_TextureFileName(TextureFileName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	SquareObject::~SquareObject() {}

	void SquareObject::OnCreate() {
		m_SquareMesh = MeshResource::CreateSquare(1.0f);
		//テクスチャの作成
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetQuaternion(m_Qt);
		PtrTrans->SetPosition(m_Pos);

		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetTrace(false);
		DrawComp->SetMeshResource(m_SquareMesh);
		DrawComp->SetTextureResource(m_TextureResource);
	}
	void SquareObject::OnUpdate() {
	}

	//--------------------------------------------------------------------------------------
	///	立方体実体
	//--------------------------------------------------------------------------------------
	CubeObject::CubeObject(const shared_ptr<Scene> PtrScene,
		const wstring& TextureFileName, const Vector3& Scale, const Quaternion& Qt, const Vector3& Pos) :
		GameObject(PtrScene),
		m_TextureFileName(TextureFileName),
		m_Scale(Scale),
		m_Qt(Qt),
		m_Pos(Pos)
	{}
	CubeObject::~CubeObject() {}

	void CubeObject::OnCreate() {
		m_CubeMesh = MeshResource::CreateCube(1.0f);
		//テクスチャの作成
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetQuaternion(m_Qt);
		PtrTrans->SetPosition(m_Pos);

		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetTrace(false);
		DrawComp->SetMeshResource(m_CubeMesh);
		DrawComp->SetTextureResource(m_TextureResource);
	}
	void CubeObject::OnUpdate() {
	}

	OBB CubeObject::GetObb()const {
		auto PtrTrans = GetComponent<Transform>();
		OBB obb(Vector3(1.0f, 1.0f, 1.0f), PtrTrans->GetWorldMatrix());
		return obb;
	}



	//--------------------------------------------------------------------------------------
	///	球体実体
	//--------------------------------------------------------------------------------------
	SphereObject::SphereObject(const shared_ptr<Scene> PtrScene,
		UINT Division, const wstring& TextureFileName, bool Trace, const Vector3& Pos) :
		GameObject(PtrScene),
		m_Division(Division),
		m_TextureFileName(TextureFileName),
		m_Trace(Trace),
		m_Scale(Vector3(1.0f, 1.0f, 1.0f)),
		m_Qt(),
		m_Pos(Pos),
		m_MoveXDir(4.0f)
	{
		m_Qt.Identity();
	}
	SphereObject::~SphereObject() {}


	void SphereObject::OnCreate() {
		m_SphereMesh = MeshResource::CreateSphere(1.0f, m_Division);
		//テクスチャの作成
		m_TextureResource = TextureResource::CreateTextureResource(m_TextureFileName, L"WIC");
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetQuaternion(m_Qt);
		PtrTrans->SetPosition(m_Pos);

		auto PtrMoveBy = AddComponent<MoveBy>();
		PtrMoveBy->SetParams(2.0f, Vector3(m_MoveXDir, 0, 0));
		PtrMoveBy->Run();

		auto DrawComp = AddComponent<PNTStaticDraw>();
		DrawComp->SetTrace(true);
		DrawComp->SetMeshResource(m_SphereMesh);
		DrawComp->SetTextureResource(m_TextureResource);

		//ステートマシンの構築
		m_StateMachine = make_shared< StateMachine<SphereObject> >(GetThis<SphereObject>());
		//最初のステートをDefaultStateに設定
		m_StateMachine->ChangeState(RightState::Instance());

	}
	void SphereObject::OnUpdate() {
		//ステートマシンのUpdateを行う
		//この中でステートの更新が行われる(Execute()関数が呼ばれる)
		m_StateMachine->Update();
	}

	void SphereObject::SetActionMotion(SphereMotion motion) {
		auto PtrMoveBy = GetComponent<MoveBy>();
		switch (motion) {
		case SphereMotion::RightMotion:
			PtrMoveBy->SetParams(2.0f, Vector3(4.0f, 0, 0));
			PtrMoveBy->Run();
			break;
		case SphereMotion::LeftMotion:
			PtrMoveBy->SetParams(2.0f, Vector3(-4.0f, 0, 0));
			PtrMoveBy->Run();
			break;
		}
	}

	bool SphereObject::IstActionArrived() {
		auto PtrMoveBy = GetComponent<MoveBy>();
		if (PtrMoveBy->IsArrived()) {
			return true;
		}
		return false;

	}




	//--------------------------------------------------------------------------------------
	//	class RightState : public ObjState<SphereObject>;
	//	用途: 右向き移動
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<RightState> RightState::Instance() {
		static shared_ptr<RightState> instance;
		if (!instance) {
			instance = shared_ptr<RightState>(new RightState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void RightState::Enter(const shared_ptr<SphereObject>& Obj) {
		Obj->SetActionMotion(SphereMotion::RightMotion);
	}
	//ステート実行中に毎ターン呼ばれる関数
	void RightState::Execute(const shared_ptr<SphereObject>& Obj) {
		if (Obj->IstActionArrived()) {
			Obj->GetStateMachine()->ChangeState(LeftState::Instance());
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void RightState::Exit(const shared_ptr<SphereObject>& Obj) {
		//何もしない
	}

	//--------------------------------------------------------------------------------------
	//	class LeftState : public ObjState<Enemy>;
	//	用途: 左向き移動
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	shared_ptr<LeftState> LeftState::Instance() {
		static shared_ptr<LeftState> instance;
		if (!instance) {
			instance = shared_ptr<LeftState>(new LeftState);
		}
		return instance;
	}
	//ステートに入ったときに呼ばれる関数
	void LeftState::Enter(const shared_ptr<SphereObject>& Obj) {
		Obj->SetActionMotion(SphereMotion::LeftMotion);
	}
	//ステート実行中に毎ターン呼ばれる関数
	void LeftState::Execute(const shared_ptr<SphereObject>& Obj) {
		if (Obj->IstActionArrived()) {
			Obj->GetStateMachine()->ChangeState(RightState::Instance());
		}
	}
	//ステートにから抜けるときに呼ばれる関数
	void LeftState::Exit(const shared_ptr<SphereObject>& Obj) {
		//何もしない
	}




}
//end basecross
