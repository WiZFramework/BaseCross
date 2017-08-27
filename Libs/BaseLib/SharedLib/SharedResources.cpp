/*!
@file SharedResources.cpp
@brief 共有のリソース（ビュー、カメラ、ライトなど）実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	struct Camera::Impl;
	//	用途: Implクラス
	//--------------------------------------------------------------------------------------
	struct Camera::Impl {
		bsm::Vec3 m_Eye;	//カメラ位置
		bsm::Vec3 m_At;	//注目点
		bsm::Vec3 m_Up;   //カメラの傾き（アップ）
		bool m_Pers;	//遠近法を使うかどうか
		//カメラ代わりのオブジェクト
		weak_ptr<GameObject> m_CameraObject;

		float m_FovY;   //射影角度
		float m_Aspect;	//アスペクト比

		float m_Width;	//遠近法を使わない場合（正射影）の幅
		float m_Height;	//遠近法を使わない場合（正射影）の高さ
		float m_Near;	//手前の最小距離
		float m_Far;	//奥の最大距離

		bsm::Mat4x4 m_ViewMatrix;
		bsm::Mat4x4 m_ProjMatrix;
		Impl() :
			m_Eye(0, 0.0f, -20.0f),	//デフォルトは後方
			m_At(0, 0, 0),
			m_Up(0, 1.0f, 0),
			m_Pers(true),
			m_FovY(XM_PIDIV4),
			m_Aspect(0),
			m_Width(0),
			m_Height(0),
			m_Near(0.5f),
			m_Far(1000.0f),
			m_ViewMatrix(),
			m_ProjMatrix()
		{
			try {
				m_Width = static_cast<float>(App::GetApp()->GetGameWidth());;
				m_Height = static_cast<float>(App::GetApp()->GetGameHeight());
				m_Aspect = m_Width / m_Height;
			}
			catch (...) {
				throw;
			}
		}
		~Impl() {}





	};

	//--------------------------------------------------------------------------------------
	//	class Camera : public Object, public ShapeInterface ;
	//	用途: カメラ（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	Camera::Camera()
		:ObjectInterface(),
		pImpl(new Impl())
	{

	}
	Camera::~Camera() {}

	//アクセサ
	const bsm::Vec3& Camera::GetEye() const { return pImpl->m_Eye; }
	void Camera::SetEye(const bsm::Vec3& Eye) { 
		pImpl->m_Eye = Eye;
		CalculateMatrix();
	}
	void Camera::SetEye(float x, float y, float z) { 
		pImpl->m_Eye = bsm::Vec3(x, y, z); 
		CalculateMatrix();
	}

	const bsm::Vec3& Camera::GetAt() const { return pImpl->m_At; }
	void Camera::SetAt(const bsm::Vec3& At) { 
		pImpl->m_At = At; 
		CalculateMatrix();
	}
	void Camera::SetAt(float x, float y, float z) { 
		pImpl->m_At = bsm::Vec3(x, y, z); 
		CalculateMatrix();
	}

	const bsm::Vec3& Camera::GetUp() const { return pImpl->m_Up;}
	void Camera::SetUp(const bsm::Vec3& Up) { 
		pImpl->m_Up = Up; 
		CalculateMatrix();
	}
	void Camera::SetUp(float x, float y, float z) { 
		pImpl->m_Up = bsm::Vec3(x, y, z); 
		CalculateMatrix();
	}

	bool Camera::IsPers()const { return pImpl->m_Pers; }
	bool Camera::GetPers()const { return pImpl->m_Pers; }
	void Camera::SetPers(bool b) { 
		pImpl->m_Pers = b; 
		CalculateMatrix();
	}

	float Camera::GetFovY() const { return pImpl->m_FovY; }
	void Camera::SetFovY(float f) { 
		pImpl->m_FovY = f; 
		CalculateMatrix();
	}

	float Camera::GetAspect() const { return pImpl->m_Aspect; }
	void Camera::SetAspect(float f) { 
		pImpl->m_Aspect = f; 
		CalculateMatrix();
	}

	float Camera::GetWidth() const { return pImpl->m_Width; }
	void Camera::SetWidth(float f) { 
		pImpl->m_Width = f; 
		CalculateMatrix();
	}

	float Camera::GetHeight() const { return pImpl->m_Height; }
	void Camera::SetHeight(float f) { 
		pImpl->m_Height = f; 
		CalculateMatrix();
	}

	float Camera::GetNear() const { return pImpl->m_Near; }
	void Camera::SetNear(float f) { 
		pImpl->m_Near = f; 
		CalculateMatrix();
	}

	float Camera::GetFar() const { return pImpl->m_Far; }
	void Camera::SetFar(float f) { 
		pImpl->m_Far = f; 
		CalculateMatrix();
	}

	shared_ptr<GameObject> Camera::GetCameraObject() const {
		if (auto sh = pImpl->m_CameraObject.lock()) {
			return sh;
		}
		else {
			return nullptr;
		}
	}
	void Camera::SetCameraObject(const shared_ptr<GameObject>& Obj) {
		if (!Obj) {
			pImpl->m_CameraObject.reset();
		}
		else {
			pImpl->m_CameraObject = Obj;
		}
		CalculateMatrix();
	}
	void Camera::ClearCameraObject() {
		SetCameraObject(nullptr);
	}

	void Camera::SetViewPort(const Viewport& v) {
		pImpl->m_Width = v.Width;
		pImpl->m_Height =v.Height;
		pImpl->m_Aspect = pImpl->m_Width / pImpl->m_Height;
		CalculateMatrix();
	}

	void Camera::CalculateMatrix() {
		if (auto ShPtr = pImpl->m_CameraObject.lock()) {
			auto TransPtr = ShPtr->GetComponent<Transform>();
			if (TransPtr) {
				pImpl->m_Eye = TransPtr->GetWorldPosition();
				pImpl->m_ViewMatrix = XMMatrixLookAtLH(pImpl->m_Eye,pImpl->m_At,pImpl->m_Up);
				if (pImpl->m_Pers) {
					pImpl->m_ProjMatrix = XMMatrixPerspectiveFovLH(pImpl->m_FovY, pImpl->m_Aspect, pImpl->m_Near, pImpl->m_Far);
				}
				else {
					pImpl->m_ProjMatrix = XMMatrixOrthographicLH(pImpl->m_Width, pImpl->m_Height, pImpl->m_Near, pImpl->m_Far);
				}
			}
		}
		else {
			if (pImpl->m_Pers) {
				pImpl->m_ViewMatrix = XMMatrixLookAtLH(pImpl->m_Eye, pImpl->m_At, pImpl->m_Up);
				pImpl->m_ProjMatrix = XMMatrixPerspectiveFovLH(pImpl->m_FovY, pImpl->m_Aspect, pImpl->m_Near, pImpl->m_Far);
			}
			else {
				pImpl->m_ViewMatrix.identity();
				pImpl->m_ProjMatrix = XMMatrixOrthographicLH(pImpl->m_Width, pImpl->m_Height, pImpl->m_Near, pImpl->m_Far);
			}
		}
	}

	const bsm::Mat4x4& Camera::GetViewMatrix() const { return pImpl->m_ViewMatrix; }
	const bsm::Mat4x4& Camera::GetProjMatrix() const { return pImpl->m_ProjMatrix; }

	void Camera::OnCreate() {
		CalculateMatrix();
	}

	void Camera::OnUpdate() {
		CalculateMatrix();
	}


	//--------------------------------------------------------------------------------------
	//	struct LookAtCamera::Impl;
	//	用途: Implクラス
	//--------------------------------------------------------------------------------------
	struct LookAtCamera::Impl {
		weak_ptr<GameObject> m_TargetObject;	//目標となるオブジェクト
		float m_ToTargetLerp;	//目標を追いかける際の補間値
		bsm::Vec3 m_TargetToAt;	//目標から視点を調整する位置ベクトル


		float m_RadY;
		float m_RadXZ;
		//カメラの上下スピード
		float m_CameraUpDownSpeed;
		//カメラを下げる下限角度
		float m_CameraUnderRot;
		//腕の長さの設定
		float m_ArmLen;
		float m_MaxArm;
		float m_MinArm;
		//回転スピード
		float m_RotSpeed;
		//ズームスピード
		float m_ZoomSpeed;
		//左右スティック変更のモード
		bool m_LRBaseMode;
		//上下スティック変更のモード
		bool m_UDBaseMode;


		Impl() :
			m_ToTargetLerp(1.0f),
			m_TargetToAt(0,0,0),
			m_RadY(0.5f),
			m_RadXZ(0),
			m_CameraUpDownSpeed(0.5f),
			m_CameraUnderRot(0.1f),
			m_ArmLen(5.0f),
			m_MaxArm(20.0f),
			m_MinArm(2.0f),
			m_RotSpeed(1.0f),
			m_ZoomSpeed(0.1f),
			m_LRBaseMode(true),
			m_UDBaseMode(true)
		{}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	//	class LookAtCamera : public Camera ;
	//	用途: LookAtカメラ（コンポーネントではない）
	//--------------------------------------------------------------------------------------
	//構築と破棄
	LookAtCamera::LookAtCamera() :
		Camera(),
		pImpl(new Impl())
	{}

	LookAtCamera::LookAtCamera(float ArmLen):
		Camera(),
		pImpl(new Impl())
	{
		pImpl->m_ArmLen = ArmLen;
		auto Eye = GetEye();
		Eye.y = pImpl->m_ArmLen;
		SetEye(Eye);
	}

	LookAtCamera::~LookAtCamera() {}
	//アクセサ
	shared_ptr<GameObject> LookAtCamera::GetTargetObject() const {
		if (!pImpl->m_TargetObject.expired()) {
			return pImpl->m_TargetObject.lock();
		}
		return nullptr;
	}

	void LookAtCamera::SetTargetObject(const shared_ptr<GameObject>& Obj) {
		pImpl->m_TargetObject = Obj;
	}

	float LookAtCamera::GetToTargetLerp() const {
		return pImpl->m_ToTargetLerp;
	}
	void LookAtCamera::SetToTargetLerp(float f) {
		pImpl->m_ToTargetLerp = f;
	}

	float LookAtCamera::GetArmLengh() const {
		return pImpl->m_ArmLen;
	}

	void LookAtCamera::UpdateArmLengh() {
		auto Vec = GetEye() - GetAt();
		pImpl->m_ArmLen = bsm::length(Vec);
		if (pImpl->m_ArmLen >= pImpl->m_MaxArm) {
			//m_MaxArm以上離れないようにする
			pImpl->m_ArmLen = pImpl->m_MaxArm;
		}
		if (pImpl->m_ArmLen <= pImpl->m_MinArm) {
			//m_MinArm以下近づかないようにする
			pImpl->m_ArmLen = pImpl->m_MinArm;
		}
	}

	float LookAtCamera::GetMaxArm() const {
		return pImpl->m_MaxArm;

	}
	void LookAtCamera::SetMaxArm(float f) {
		pImpl->m_MaxArm = f;
	}
	float LookAtCamera::GetMinArm() const {
		return pImpl->m_MinArm;
	}
	void LookAtCamera::SetMinArm(float f) {
		pImpl->m_MinArm = f;
	}

	float LookAtCamera::GetRotSpeed() const {
		return pImpl->m_RotSpeed;

	}
	void LookAtCamera::SetRotSpeed(float f) {
		pImpl->m_RotSpeed = abs(f);
	}

	bsm::Vec3 LookAtCamera::GetTargetToAt() const {
		return pImpl->m_TargetToAt;

	}
	void LookAtCamera::SetTargetToAt(const bsm::Vec3& v) {
		pImpl->m_TargetToAt = v;
	}

	bool LookAtCamera::GetLRBaseMode() const {
		return pImpl->m_LRBaseMode;

	}
	bool LookAtCamera::IsLRBaseMode() const {
		return pImpl->m_LRBaseMode;

	}
	void LookAtCamera::SetLRBaseMode(bool b) {
		pImpl->m_LRBaseMode = b;
	}
	bool LookAtCamera::GetUDBaseMode() const {
		return pImpl->m_UDBaseMode;

	}
	bool LookAtCamera::IsUDBaseMode() const {
		return pImpl->m_UDBaseMode;
	}
	void LookAtCamera::SetUDBaseMode(bool b) {
		pImpl->m_UDBaseMode = b;

	}


	void LookAtCamera::SetAt(const bsm::Vec3& At) {
		Camera::SetAt(At);
		bsm::Vec3 ArmVec = GetEye() - GetAt();
		ArmVec.normalize();
		ArmVec *= pImpl->m_ArmLen;
		bsm::Vec3 NewEye = GetAt() + ArmVec;
		SetEye(NewEye);
	}
	void LookAtCamera::SetAt(float x, float y, float z) {
		Camera::SetAt(x,y,z);
		bsm::Vec3 ArmVec = GetEye() - GetAt();
		ArmVec.normalize();
		ArmVec *= pImpl->m_ArmLen;
		bsm::Vec3 NewEye = GetAt() + ArmVec;
		SetEye(NewEye);

	}


	void LookAtCamera::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		auto KeyData = App::GetApp()->GetInputDevice().GetKeyState();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		bsm::Vec3 NewEye = GetEye();
		bsm::Vec3 NewAt = GetAt();
		//計算に使うための腕角度（ベクトル）
		bsm::Vec3 ArmVec = NewEye - NewAt;
		//正規化しておく
		ArmVec.normalize();
		if (CntlVec[0].bConnected) {
			//上下角度の変更
			if (CntlVec[0].fThumbRY >= 0.1f || KeyData.m_bPushKeyTbl[VK_UP]) {
				if (IsUDBaseMode()) {
					pImpl->m_RadY += pImpl->m_CameraUpDownSpeed * ElapsedTime;
				}
				else {
					pImpl->m_RadY -= pImpl->m_CameraUpDownSpeed * ElapsedTime;
				}
			}
			else if (CntlVec[0].fThumbRY <= -0.1f || KeyData.m_bPushKeyTbl[VK_DOWN]) {
				if (IsUDBaseMode()) {
					pImpl->m_RadY -= pImpl->m_CameraUpDownSpeed * ElapsedTime;
				}
				else {
					pImpl->m_RadY += pImpl->m_CameraUpDownSpeed * ElapsedTime;
				}
			}
			if (pImpl->m_RadY > XM_PI * 4 / 9.0f) {
				pImpl->m_RadY = XM_PI * 4 / 9.0f;
			}
			else if (pImpl->m_RadY <= pImpl->m_CameraUnderRot) {
				//カメラが限界下に下がったらそれ以上下がらない
				pImpl->m_RadY = pImpl->m_CameraUnderRot;
			}
			ArmVec.y = sin(pImpl->m_RadY);
			//ここでY軸回転を作成
			if (CntlVec[0].fThumbRX != 0 || KeyData.m_bPushKeyTbl[VK_LEFT] || KeyData.m_bPushKeyTbl[VK_RIGHT]) {
				//回転スピードを反映
				if (CntlVec[0].fThumbRX != 0) {
					if (IsLRBaseMode()) {
						pImpl->m_RadXZ += -CntlVec[0].fThumbRX * ElapsedTime * pImpl->m_RotSpeed;
					}
					else {
						pImpl->m_RadXZ += CntlVec[0].fThumbRX * ElapsedTime * pImpl->m_RotSpeed;
					}
				}
				else if (KeyData.m_bPushKeyTbl[VK_LEFT]) {
					if (IsLRBaseMode()) {
						pImpl->m_RadXZ += ElapsedTime * pImpl->m_RotSpeed;
					}
					else {
						pImpl->m_RadXZ -= ElapsedTime * pImpl->m_RotSpeed;
					}
				}
				else if (KeyData.m_bPushKeyTbl[VK_RIGHT]) {
					if (IsLRBaseMode()) {
						pImpl->m_RadXZ -= ElapsedTime * pImpl->m_RotSpeed;
					}
					else {
						pImpl->m_RadXZ += ElapsedTime * pImpl->m_RotSpeed;
					}

				}
				if (abs(pImpl->m_RadXZ) >= XM_2PI) {
					//1週回ったら0回転にする
					pImpl->m_RadXZ = 0;
				}
			}
			//クオータニオンでY回転（つまりXZベクトルの値）を計算
			bsm::Quat QtXZ;
			QtXZ.rotation(pImpl->m_RadXZ, bsm::Vec3(0, 1.0f, 0));
			QtXZ.normalize();
			//移動先行の行列計算することで、XZの値を算出
			bsm::Mat4x4 Mat;
			Mat.strTransformation(
				bsm::Vec3(1.0f, 1.0f, 1.0f),
				bsm::Vec3(0.0f, 0.0f, -1.0f),
				QtXZ
			);

			bsm::Vec3 PosXZ = Mat.transInMatrix();
			//XZの値がわかったので腕角度に代入
			ArmVec.x = PosXZ.x;
			ArmVec.z = PosXZ.z;
			//腕角度を正規化
			ArmVec.normalize();

			auto TargetPtr = GetTargetObject();
			if (TargetPtr) {
				//目指したい場所
				bsm::Vec3 ToAt = TargetPtr->GetComponent<Transform>()->GetWorldMatrix().transInMatrix();
				ToAt += pImpl->m_TargetToAt;
				NewAt = Lerp::CalculateLerp(GetAt(), ToAt, 0, 1.0f, 1.0, Lerp::Linear);
			}
			//アームの変更
			//Dパッド下
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				//カメラ位置を引く
				pImpl->m_ArmLen += pImpl->m_ZoomSpeed;
				if (pImpl->m_ArmLen >= pImpl->m_MaxArm) {
					//m_MaxArm以上離れないようにする
					pImpl->m_ArmLen = pImpl->m_MaxArm;
				}
			}
			//Dパッド上
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				//カメラ位置を寄る
				pImpl->m_ArmLen -= pImpl->m_ZoomSpeed;
				if (pImpl->m_ArmLen <= pImpl->m_MinArm) {
					//m_MinArm以下近づかないようにする
					pImpl->m_ArmLen = pImpl->m_MinArm;
				}
			}
			////目指したい場所にアームの値と腕ベクトルでEyeを調整
			bsm::Vec3 ToEye = NewAt + ArmVec * pImpl->m_ArmLen;
			NewEye = Lerp::CalculateLerp(GetEye(), ToEye, 0, 1.0f, pImpl->m_ToTargetLerp, Lerp::Linear);
		}
		if (KeyData.m_bPressedKeyTbl[VK_LEFT]) {
			int a = 0;
		}



		SetAt(NewAt);
		SetEye(NewEye);
		UpdateArmLengh();
		Camera::OnUpdate();
	}




	//--------------------------------------------------------------------------------------
	//	ビューのアイテム
	//--------------------------------------------------------------------------------------
	struct ViewItem {
		shared_ptr<Camera> m_Camera;
		Viewport m_Viewport;
		ViewItem() {
			m_Viewport.Width = static_cast<float>(App::GetApp()->GetGameWidth());
			m_Viewport.Height = static_cast<float>(App::GetApp()->GetGameHeight());
			m_Viewport.TopLeftX = 0;
			m_Viewport.TopLeftY = 0;
			m_Viewport.MinDepth = 0;
			m_Viewport.MaxDepth = 1.0f;
			m_Camera = ObjectFactory::Create<Camera>();
			m_Camera->SetViewPort(m_Viewport);
			m_Camera->CalculateMatrix();
		}
	};


	//--------------------------------------------------------------------------------------
	///	struct ViewBase::Impl;
	//--------------------------------------------------------------------------------------
	struct ViewBase::Impl {
		weak_ptr<Stage> m_Stage;
		bool m_UpdateActive;
		explicit Impl(const shared_ptr<Stage>& StagePtr) :
			m_Stage(StagePtr),
			m_UpdateActive(true)
		{}
		~Impl() {}
	};
	//--------------------------------------------------------------------------------------
	///	ビュー（親）
	//--------------------------------------------------------------------------------------
	ViewBase::ViewBase(const shared_ptr<Stage>& StagePtr) :
		ObjectInterface(),
		ShapeInterface(),
		pImpl(new Impl(StagePtr))
	{
	}

	ViewBase::~ViewBase() {}

	//アクセサ
	bool ViewBase::IsUpdateActive() const { return pImpl->m_UpdateActive; }
	bool ViewBase::GetUpdateActive() const { return pImpl->m_UpdateActive; }
	void ViewBase::SetUpdateActive(bool b) { pImpl->m_UpdateActive = b; }


	//--------------------------------------------------------------------------------------
	///	struct SingleView::Impl;
	//--------------------------------------------------------------------------------------
	struct SingleView::Impl {
		ViewItem m_ViewItem;
		Impl()
		{
		}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	シングルビューコンポーネント
	//--------------------------------------------------------------------------------------
	SingleView::SingleView(const shared_ptr<Stage>& StagePtr) :
		ViewBase(StagePtr), pImpl(new Impl())
	{}

	SingleView::~SingleView() {}

	void SingleView::OnCreate() {
	}

	void SingleView::OnUpdate() {
		//カメラの計算をする
		pImpl->m_ViewItem.m_Camera->OnUpdate();
	}

	void SingleView::SetViewport(const Viewport& v) {
		pImpl->m_ViewItem.m_Viewport = v;
		pImpl->m_ViewItem.m_Camera->SetViewPort(v);
	}
	const Viewport& SingleView::GetViewport() const {
		return pImpl->m_ViewItem.m_Viewport;
	}
	void SingleView::SetCamera(const shared_ptr<Camera>& c) {
		pImpl->m_ViewItem.m_Camera = c;
		//カメラの計算をする
		pImpl->m_ViewItem.m_Camera->CalculateMatrix();
	}
	const shared_ptr<Camera>& SingleView::GetCamera()const {
		return pImpl->m_ViewItem.m_Camera;
	}
	const shared_ptr<Camera>& SingleView::GetTargetCamera() const {
		return pImpl->m_ViewItem.m_Camera;
	}
	const Viewport& SingleView::GetTargetViewport() const {
		return pImpl->m_ViewItem.m_Viewport;
	}

	//--------------------------------------------------------------------------------------
	///	struct MultiView::Impl;
	//--------------------------------------------------------------------------------------
	struct MultiView::Impl {
		vector<ViewItem> m_ViewItemVec;
		size_t m_TargetIndex;
		Impl():
			m_TargetIndex(0)
		{
		}
		~Impl() {}
	};

	MultiView::MultiView(const shared_ptr<Stage>& StagePtr) :
		ViewBase(StagePtr), pImpl(new Impl())
	{}

	MultiView::~MultiView() {}

	size_t MultiView::AddView(const Viewport& v, const shared_ptr<Camera>& c) {
		ViewItem Item;
		Item.m_Viewport = v;
		Item.m_Camera = c;
		size_t ret = pImpl->m_ViewItemVec.size();
		pImpl->m_ViewItemVec.push_back(Item);
		return ret;
	}
	size_t MultiView::GetViewSize()const {
		return pImpl->m_ViewItemVec.size();
	}
	void MultiView::SetViewport(size_t Index, const Viewport& v) {
		if (Index >= pImpl->m_ViewItemVec.size()) {
			throw BaseException(
				L"インデックスが範囲外です。",
				Util::UintToWStr(Index),
				L"MultiView::SetViewport()"
			);
		}
		pImpl->m_ViewItemVec[Index].m_Viewport = v;
	}
	const Viewport& MultiView::GetViewport(size_t Index) const {
		if (Index >= pImpl->m_ViewItemVec.size()) {
			throw BaseException(
				L"インデックスが範囲外です。",
				Util::UintToWStr(Index),
				L"MultiView::GetViewport()"
			);
		}
		return pImpl->m_ViewItemVec[Index].m_Viewport;
	}
	void MultiView::SetCamera(size_t Index, const shared_ptr<Camera>& c) {
		if (Index >= pImpl->m_ViewItemVec.size()) {
			throw BaseException(
				L"インデックスが範囲外です。",
				Util::UintToWStr(Index),
				L"MultiView::SetCamera()"
			);
		}
		pImpl->m_ViewItemVec[Index].m_Camera = c;
	}
	const shared_ptr<Camera>& MultiView::GetCamera(size_t Index)const {
		if (Index >= pImpl->m_ViewItemVec.size()) {
			throw BaseException(
				L"インデックスが範囲外です。",
				Util::UintToWStr(Index),
				L"MultiView::GetCamera()"
			);
		}
		return pImpl->m_ViewItemVec[Index].m_Camera;
	}
	size_t MultiView::GetTargetIndex()const {
		if (pImpl->m_ViewItemVec.empty()) {
			throw BaseException(
				L"ビューが設定されてないのでこのコマンドは無意味です",
				L"if (pImpl->m_ViewItemVec.empty())",
				L"MultiView::GetTargetIndex()"
			);
		}
		return pImpl->m_TargetIndex;
	}
	void MultiView::SetTargetIndex(size_t Index) {
		if (Index >= pImpl->m_ViewItemVec.size()) {
			throw BaseException(
				L"インデックスが範囲外です。",
				Util::UintToWStr(Index),
				L"MultiView::SetTargetIndex()"
			);
		}
		pImpl->m_TargetIndex = Index;
	}
	void MultiView::ChangeNextView() {
		pImpl->m_TargetIndex++;
		if (pImpl->m_TargetIndex >= pImpl->m_ViewItemVec.size()) {
			pImpl->m_TargetIndex = 0;
		}
	}
	const shared_ptr<Camera>& MultiView::GetTargetCamera() const {
		if (pImpl->m_TargetIndex >= pImpl->m_ViewItemVec.size()) {
			throw BaseException(
				L"インデックスが範囲外です。",
				Util::UintToWStr(pImpl->m_TargetIndex),
				L"MultiView::GetTargetCamera()"
			);
		}
		return pImpl->m_ViewItemVec[pImpl->m_TargetIndex].m_Camera;
	}
	const Viewport& MultiView::GetTargetViewport() const {
		if (pImpl->m_TargetIndex >= pImpl->m_ViewItemVec.size()) {
			throw BaseException(
				L"インデックスが範囲外です。",
				Util::UintToWStr(pImpl->m_TargetIndex),
				L"MultiView::GetTargetViewport()"
			);
		}
		return pImpl->m_ViewItemVec[pImpl->m_TargetIndex].m_Viewport;
	}

	void MultiView::OnCreate() {
	}
	void MultiView::OnUpdate() {
		for (auto& v : pImpl->m_ViewItemVec) {
			v.m_Camera->OnUpdate();
		}
	}







	//--------------------------------------------------------------------------------------
	///	struct LightBase::Impl;
	//--------------------------------------------------------------------------------------
	struct LightBase::Impl {
		weak_ptr<Stage> m_Stage;
		bsm::Col4 m_AmbientLightColor;
		explicit Impl(const shared_ptr<Stage>& StagePtr) :
			m_Stage(StagePtr),
			m_AmbientLightColor(0, 0, 0, 0)
		{}
		~Impl() {}
	};
	//--------------------------------------------------------------------------------------
	///	ステージに設定するライト（親）
	//--------------------------------------------------------------------------------------
	LightBase::LightBase(const shared_ptr<Stage>& StagePtr) :
		ObjectInterface(),
		pImpl(new Impl(StagePtr))
	{
	}

	LightBase::~LightBase() {}

	bsm::Col4 LightBase::GetAmbientLightColor()const {
		return pImpl->m_AmbientLightColor;
	}

	void LightBase::SetAmbientLightColor(const bsm::Col4& value) {
		pImpl->m_AmbientLightColor = value;
	}


	//--------------------------------------------------------------------------------------
	///	struct SingleLight::Impl;
	//--------------------------------------------------------------------------------------
	struct SingleLight::Impl {
		Light m_Light;
		Impl() 
		{
		}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	シングルライト
	//--------------------------------------------------------------------------------------
	SingleLight::SingleLight(const shared_ptr<Stage>& StagePtr) :
		LightBase(StagePtr),
		pImpl(new Impl())
	{}

	SingleLight::~SingleLight() {}

	void SingleLight::SetLight(const Light& l) {
		pImpl->m_Light = l;
	}

	const Light& SingleLight::GetLight()const {
		return pImpl->m_Light;
	}

	Light& SingleLight::GetLight() {
		return pImpl->m_Light;
	}

	const Light& SingleLight::GetTargetLight() const {
		return pImpl->m_Light;
	}


	//--------------------------------------------------------------------------------------
	///	struct MultiLight::Impl;
	//--------------------------------------------------------------------------------------
	struct MultiLight::Impl {
		vector<Light> m_LightVec;
		size_t m_MainIndex;
		Impl():
			m_MainIndex(0)
		{
			m_LightVec.resize(3);
			for (auto& v : m_LightVec) {
				v.m_Directional = bsm::Vec3(0, -1, 0);
				v.m_DiffuseColor = bsm::Col4(1, 1, 1, 1);
				v.m_SpecularColor = bsm::Col4(1, 1, 1, 1);
			}
		}
		~Impl() {}
	};



	//--------------------------------------------------------------------------------------
	///	マルチライト
	//--------------------------------------------------------------------------------------

	void MultiLight::ValidateLightIndex(size_t index)const
	{
		if (index >= 3)
		{
			throw BaseException(
				L"ライトのインデックスが範囲外です。0から2の間で指定してください。",
				L"if (index >= 3)",
				L"MultiLight::ValidateLightIndex()"
			);
		}
	}

	MultiLight::MultiLight(const shared_ptr<Stage>& StagePtr) :
		LightBase(StagePtr),
		pImpl(new Impl())
	{
	}

	MultiLight::~MultiLight() {}

	void MultiLight::SetMainIndex(size_t index) {
		ValidateLightIndex(index);
		pImpl->m_MainIndex = index;
	}

	size_t  MultiLight::GetMainIndex()const {
		return pImpl->m_MainIndex;
	}

	void  MultiLight::SetLight(size_t index, const Light& l) {
		ValidateLightIndex(index);
		pImpl->m_LightVec[index] = l;
	}

	const Light& MultiLight::GetLight(size_t index)const {
		ValidateLightIndex(index);
		return pImpl->m_LightVec[index];
	}

	Light&  MultiLight::GetLight(size_t index) {
		ValidateLightIndex(index);
		return pImpl->m_LightVec[index];
	}

	void MultiLight::SetDefaultLighting() {
		static const bsm::Vec3 defaultDirections[3] =
		{
			{ -0.5265408f, -0.5735765f, -0.6275069f },
			{ 0.7198464f,  0.3420201f,  0.6040227f },
			{ 0.4545195f, -0.7660444f,  0.4545195f },
		};

		static const bsm::Col4 defaultDiffuse[3] =
		{
			{ 1.0000000f, 0.9607844f, 0.8078432f,0.0f },
			{ 0.9647059f, 0.7607844f, 0.4078432f,0.0f },
			{ 0.3231373f, 0.3607844f, 0.3937255f,0.0f },
		};

		static const bsm::Col4 defaultSpecular[3] =
		{
			{ 1.0000000f, 0.9607844f, 0.8078432f,0.0f },
			{ 0.0000000f, 0.0000000f, 0.0000000f,0.0f },
			{ 0.3231373f, 0.3607844f, 0.3937255f,0.0f },
		};
		static const bsm::Col4 defaultAmbient = { 0.05333332f, 0.09882354f, 0.1819608f ,0.0f };
		for (size_t i = 0; i < 3; i++) {
			pImpl->m_LightVec[i].m_Directional = defaultDirections[i];
			pImpl->m_LightVec[i].m_DiffuseColor = defaultDiffuse[i];
			pImpl->m_LightVec[i].m_SpecularColor = defaultSpecular[i];
		}
		SetAmbientLightColor(defaultAmbient);
		pImpl->m_MainIndex = 2;
	}

	void MultiLight::SetDefaultLighting2() {
		static const bsm::Vec3 defaultDirections[3] =
		{
			{ -0.5265408f, -0.5735765f, -0.6275069f },
			{ 0.7198464f,  0.3420201f,  0.6040227f },
			{ 0.4545195f, -0.7660444f,  0.4545195f },
		};

		static const bsm::Col4 defaultDiffuse[3] =
		{
			{ 0.3231373f, 0.3607844f, 0.3937255f,0.0f },
			{ 0.9647059f, 0.7607844f, 0.4078432f,0.0f },
			{ 1.0000000f, 0.9607844f, 0.8078432f,0.0f },
		};

		static const bsm::Col4 defaultSpecular[3] =
		{
			{ 0.3231373f, 0.3607844f, 0.3937255f,0.0f },
			{ 0.0000000f, 0.0000000f, 0.0000000f,0.0f },
			{ 1.0000000f, 0.9607844f, 0.8078432f,0.0f },
		};
		static const bsm::Col4 defaultAmbient = { 0.05333332f, 0.09882354f, 0.1819608f ,0.0f };
		for (size_t i = 0; i < 3; i++) {
			pImpl->m_LightVec[i].m_Directional = defaultDirections[i];
			pImpl->m_LightVec[i].m_DiffuseColor = defaultDiffuse[i];
			pImpl->m_LightVec[i].m_SpecularColor = defaultSpecular[i];
		}
		SetAmbientLightColor(defaultAmbient);
		pImpl->m_MainIndex = 2;
	}


	const Light& MultiLight::GetTargetLight() const {
		return pImpl->m_LightVec[pImpl->m_MainIndex];
	}








	//--------------------------------------------------------------------------------------
	//	struct Steering;
	//	用途: 操舵関連ユーティリティ
	//	＊static呼び出しをする
	//--------------------------------------------------------------------------------------
	//スタティックメンバ
	bool Steering::AccumulateForce(bsm::Vec3& Force, const bsm::Vec3& ForceToAdd, float MaxForce) {
		//現在の力の長さを得る
		float MagnitudeSoFar = bsm::length(Force);
		//最大値との差を求める
		float magnitudeRemaining = MaxForce - MagnitudeSoFar;
		//差が0以下（つまり最大値を超えていたら）
		//追加しないでリターン
		if (magnitudeRemaining <= 0.0f) {
			return false;
		}
		//追加する力の大きさを求める
		float MagnitudeToAdd = bsm::length(ForceToAdd);
		//力の追加
		if (MagnitudeToAdd < magnitudeRemaining) {
			Force += ForceToAdd;
		}
		else {
			Force += (bsm::normalize(ForceToAdd) * MagnitudeToAdd);
		}
		//追加された指標を返す  
		return true;
	}

	//--------------------------------------------------------------------------------------
	bsm::Vec3 Steering::Seek(const bsm::Vec3& Velocity, const bsm::Vec3& Target, const bsm::Vec3& Pos, float MaxSpeed) {
		bsm::Vec3 DesiredVelocity
			= bsm::normalize(Target - Pos) * MaxSpeed;
		return (DesiredVelocity - Velocity);
	}

	//--------------------------------------------------------------------------------------
	bsm::Vec3 Steering::Flee(const bsm::Vec3& Velocity, const bsm::Vec3& Target,
		const bsm::Vec3& Pos, float MaxSpeed, float PanicDistance) {
		float PanicDistanceSq = PanicDistance * PanicDistance;
		if (bsm::lengthSqr(Pos - Target) > PanicDistanceSq) {
			return bsm::Vec3(0, 0, 0);
		}
		bsm::Vec3 DesiredVelocity
			= bsm::normalize(Pos - Target) * MaxSpeed;
		return (DesiredVelocity - Velocity);
	}

	//--------------------------------------------------------------------------------------
	bsm::Vec3 Steering::Arrive(const bsm::Vec3& Velocity, const bsm::Vec3& Target, const bsm::Vec3& Pos, float MaxSpeed, float Decl) {
		bsm::Vec3 ToTarget = Target - Pos;
		float dist = bsm::length(ToTarget);
		if (dist > 0) {
			const float DecelerationTweaker = 0.3f;
			float speed = dist / (Decl * DecelerationTweaker);
			speed = Util::Minimum(speed, MaxSpeed);
			bsm::Vec3 DesiredVelocity = ToTarget * speed / dist;
			return (DesiredVelocity - Velocity);
		}
		return bsm::Vec3(0, 0, 0);
	}

	//--------------------------------------------------------------------------------------
	bsm::Vec3 Steering::Pursuit(const bsm::Vec3& Velocity, const bsm::Vec3& Pos, const bsm::Vec3& Rot, float MaxSpeed,
		const bsm::Vec3& TargetVelocity, const bsm::Vec3& Target, const bsm::Vec3& TargetRot) {
		bsm::Vec3 ToEvader = Target - Pos;
		double RelativeHeading = bsm::dot(Rot,TargetRot);
		if ((bsm::dot(ToEvader,Rot) > 0) &&
			(RelativeHeading < -0.95))  //acos(0.95)=18 degs
		{
			return Steering::Seek(Velocity, Target, Pos, MaxSpeed);
		}
		float LookAheadTime = bsm::length(ToEvader) /
			(MaxSpeed + bsm::length(TargetVelocity));
		return Steering::Seek(Velocity, Target + TargetVelocity * LookAheadTime, Pos, MaxSpeed);
	}

	//--------------------------------------------------------------------------------------
	bsm::Vec3 Steering::Wander(const bsm::Mat4x4 Matrix,
		float WanderRadius, float WanderDistance, float WanderJitter, bsm::Vec3& WanderTarget) {
		WanderTarget += bsm::Vec3(
			(Util::RandZeroToOne(true) * 2.0f - 1.0f)  * WanderJitter,
			0,
			(Util::RandZeroToOne(true) * 2.0f - 1.0f)  * WanderJitter
		);
		WanderTarget.normalize();
		WanderTarget *= WanderRadius;
		bsm::Vec3 target_Local = WanderTarget + bsm::Vec3(0, 0, WanderDistance);
		bsm::Vec3 Scale,  Pos;
		bsm::Quat Qt;
		Matrix.decompose(Scale, Qt, Pos);
		bsm::Mat4x4 mat;
		mat.affineTransformation(Scale, target_Local, Qt, Pos);
		target_Local *= Matrix;
		return target_Local - Matrix.transInMatrix();
	}

	struct ObstacleAvoidanceSphere {
		SPHERE m_Sp;
		float Len;
		ObstacleAvoidanceSphere(const SPHERE& Sp, float len) :
			m_Sp(Sp), Len(len) {}
	};

	//--------------------------------------------------------------------------------------
	bool SortSphereObstacleAvoidanceHandle(ObstacleAvoidanceSphere& Left, ObstacleAvoidanceSphere& Right) {
		return Left.Len < Right.Len;
	}

	//--------------------------------------------------------------------------------------
	bsm::Vec3 Steering::ObstacleAvoidance(const bsm::Mat4x4 Matrix,
		const bsm::Vec3& Velocity, float MaxSpeed, float Width, float Height,
		const vector<SPHERE>& SphereVec) {
		//現在の速度と位置と道幅から、衝突判定OBBを作成する
		bsm::Vec3 Scale(Width, Height, bsm::length(Velocity));
		bsm::Mat4x4 ObbMat;
		ObbMat.affineTransformation(
			Scale, 
			bsm::Vec3(0,0,0),
			Matrix.quatInMatrix(), 
			Matrix.transInMatrix() + Velocity / 2.0f);
		OBB Obb(bsm::Vec3(1.0f, 1.0f, 1.0f), ObbMat);
		vector<ObstacleAvoidanceSphere> ChangeVec;
		for (size_t i = 0; i < SphereVec.size(); i++) {
			float len = bsm::length(SphereVec[i].m_Center - Matrix.transInMatrix());
			ObstacleAvoidanceSphere Sp(SphereVec[i], len);
			ChangeVec.push_back(Sp);
		}
		//順序を変更ソート
		std::sort(ChangeVec.begin(), ChangeVec.end(), SortSphereObstacleAvoidanceHandle);
		//近い順に検査して何かと衝突していたら、ターゲットを決めSEEK
		for (size_t i = 0; i < ChangeVec.size(); i++) {
			bsm::Vec3 RetVec;
			if (HitTest::SPHERE_OBB(ChangeVec[i].m_Sp, Obb, RetVec)) {
				//進行方向のOBBと衝突した
				//OBB進行方向の線分とRetVecとの最近接点を求める
				float t;
				bsm::Vec3 d;
				HitTest::ClosetPtPointSegment(RetVec, Matrix.transInMatrix(), Matrix.transInMatrix() + Velocity, t, d);
				//退避方向を計算する
				bsm::Vec3 AvoidanceVec = (d - RetVec);
				//正規化
				AvoidanceVec.normalize();
				AvoidanceVec *= (Width + MaxSpeed);
				return AvoidanceVec;
			}
		}
		return bsm::Vec3(0, 0, 0);
	}

	struct AvoidanceSegment {
		bsm::Vec3 m_PointA;
		bsm::Vec3 m_PointB;
		AvoidanceSegment() {}
		AvoidanceSegment(const bsm::Vec3& pa, const bsm::Vec3& pb) :
			m_PointA(pa),
			m_PointB(pb)
		{
		}
	};

	//--------------------------------------------------------------------------------------
	bsm::Vec3 Steering::WallAvoidance(const bsm::Mat4x4 Matrix,
		const bsm::Vec3& Velocity, float MaxSpeed, const vector<PLANE>& PlaneVec) {
		//まず触覚になる線分配列を作成
		float Len = bsm::length(Velocity) * 0.5f;
		vector<AvoidanceSegment> Segments;
		Segments.push_back(AvoidanceSegment(bsm::Vec3(0, 0, Len), bsm::Vec3(0, 0, 0)));
		Segments.push_back(AvoidanceSegment(bsm::Vec3(cos(XM_PIDIV4) * Len, 0, sin(XM_PIDIV4) * Len), bsm::Vec3(0, 0, 0)));
		Segments.push_back(AvoidanceSegment(bsm::Vec3(-cos(XM_PIDIV4) * Len, 0, sin(XM_PIDIV4) * Len), bsm::Vec3(0, 0, 0)));
		Segments.push_back(AvoidanceSegment(bsm::Vec3(Len, 0, 0), bsm::Vec3(0, 0, 0)));
		Segments.push_back(AvoidanceSegment(bsm::Vec3(-Len, 0, 0), bsm::Vec3(0, 0, 0)));
		Segments.push_back(AvoidanceSegment(bsm::Vec3(cos(XM_PIDIV4) * Len, 0, -sin(XM_PIDIV4) * Len), bsm::Vec3(0, 0, 0)));
		Segments.push_back(AvoidanceSegment(bsm::Vec3(-cos(XM_PIDIV4) * Len, 0, -sin(XM_PIDIV4) * Len), bsm::Vec3(0, 0, 0)));
		Segments.push_back(AvoidanceSegment(bsm::Vec3(0, 0, -Len), bsm::Vec3(0, 0, 0)));
		for (size_t i = 0; i < Segments.size(); i++) {
			//触覚の各頂点に行列を計算して、ワールド座標に変換
			Segments[i].m_PointA *= Matrix;
			Segments[i].m_PointB *= Matrix;
			//線分と壁の衝突判定
			bsm::Vec3 RetVec;
			float t;
			for (size_t j = 0; j < PlaneVec.size(); j++) {
				if (HitTest::InsidePtPlane(Matrix.transInMatrix(), PlaneVec[j])) {
					return PlaneVec[j].m_Normal * MaxSpeed;
				}
				if (HitTest::SEGMENT_PLANE(Segments[i].m_PointA, Segments[i].m_PointB, PlaneVec[j], t, RetVec)) {
					//線分と面が衝突している
					//面の法線の方向に現在の速度でフォースを返す
					return PlaneVec[j].m_Normal * bsm::length(Velocity);
				}
			}
		}
		return bsm::Vec3(0, 0, 0);
	}



	//--------------------------------------------------------------------------------------
	bsm::Vec3 Steering::FollowPath(Path& rPath,
		float WaypointSeekDist,
		const bsm::Vec3& Pos, const bsm::Vec3& Velocity,
		float MaxSpeed, float Decl) {
		bsm::Vec3 Dis = Pos - rPath.GetCurWaypoint();
		float DisSq = bsm::lengthSqr(Dis);
		float WaypointSeekDistSq = WaypointSeekDist * WaypointSeekDist;

		if (DisSq < WaypointSeekDistSq) {
			rPath.SetNextWaypoint();
		}
		if (!rPath.IsFinished()) {
			return Seek(Velocity, rPath.GetCurWaypoint(), Pos, MaxSpeed);
		}
		else {
			return Arrive(Velocity, rPath.GetCurWaypoint(), Pos, MaxSpeed, Decl);
		}
	}

	bsm::Vec3 Steering::Separation(const shared_ptr<GameObjectGroup>& Group, const shared_ptr<GameObject>& MyObj) {
		bsm::Vec3 SteeringForce(0, 0, 0);
		auto Vec = Group->GetGroupVector();
		for (auto Ptr : Vec) {
			if (!Ptr.expired()) {
				auto PtrObj = Ptr.lock();
				if (PtrObj != MyObj) {
					PtrObj->GetComponent<Transform>();
					bsm::Vec3 ToAgent
						= MyObj->GetComponent<Transform>()->GetWorldPosition()
						- PtrObj->GetComponent<Transform>()->GetWorldPosition();
					SteeringForce += bsm::normalize(ToAgent) / bsm::length(ToAgent);
				}
			}
		}
		return SteeringForce;
	}




	//--------------------------------------------------------------------------------------
	//	static bsm::Vec3 Alignment(
	//	const shared_ptr<GameObjectGroup>& Group,	//設定するグループの配列
	//	const shared_ptr<GameObject>& MyObj				//自分自身
	//	);
	//	用途: 整列行動
	//	戻り値: フォース
	//--------------------------------------------------------------------------------------
	bsm::Vec3 Steering::Alignment(const shared_ptr<GameObjectGroup>& Group, const shared_ptr<GameObject>& MyObj) {
		bsm::Vec3 AverageHeading(0, 0, 0);
		auto Vec = Group->GetGroupVector();
		int count = 0;
		for (auto Ptr : Vec) {
			if (!Ptr.expired()) {
				auto PtrObj = Ptr.lock();
				if (PtrObj != MyObj) {
					auto PtrT = PtrObj->GetComponent<Transform>();
					AverageHeading += PtrT->GetQuaternion().toRotVec();
					count++;
				}
			}
		}
		if (count > 0) {
			AverageHeading /= (float)count;
			auto PtrT = MyObj->GetComponent<Transform>();
			AverageHeading -= PtrT->GetQuaternion().toRotVec();
		}
		return AverageHeading;
	}

	//--------------------------------------------------------------------------------------
	//	static bsm::Vec3 Cohesion(
	//	const shared_ptr<GameObjectGroup>& Group,	//設定するグループの配列
	//	const shared_ptr<GameObject>& MyObj			//自分自身
	//	const bsm::Vec3& Velocity,	//現在の速度
	//	float MaxSpeed				//最高速度
	//	);
	//	用途: 結合行動
	//	戻り値: フォース
	//--------------------------------------------------------------------------------------
	bsm::Vec3 Steering::Cohesion(const shared_ptr<GameObjectGroup>& Group, const shared_ptr<GameObject>& MyObj,
		const bsm::Vec3& Velocity, float MaxSpeed) {
		auto Vec = Group->GetGroupVector();
		bsm::Vec3 SteeringForce(0, 0, 0);
		//重心
		bsm::Vec3 CenterOfMass(0, 0, 0);
		int count = 0;
		for (auto Ptr : Vec) {
			if (!Ptr.expired()) {
				auto PtrObj = Ptr.lock();
				if (PtrObj != MyObj) {
					auto PtrT = PtrObj->GetComponent<Transform>();
					CenterOfMass += PtrT->GetWorldPosition();
					count++;
				}
			}
		}
		if (count > 0) {
			CenterOfMass /= (float)count;
			auto PtrT = MyObj->GetComponent<Transform>();
			SteeringForce = Seek(Velocity, CenterOfMass, PtrT->GetWorldPosition(), MaxSpeed);
			SteeringForce.normalize();
		}
		return SteeringForce;
	}



}
//end basecross
