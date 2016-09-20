/*!
@file SharedResources.cpp
@brief 共有のリソース（ビュー、カメラ、ライトなど）実体
*/
#include "stdafx.h"

namespace basecross {
	//--------------------------------------------------------------------------------------
	//	struct Camera::Impl;
	//	用途: Implクラス
	//--------------------------------------------------------------------------------------
	struct Camera::Impl {
		Vector3 m_Eye;	//カメラ位置
		Vector3 m_At;	//注目点
		Vector3 m_Up;   //カメラの傾き（アップ）
		bool m_Pers;	//遠近法を使うかどうか
		//カメラ代わりのオブジェクト
		weak_ptr<GameObject> m_CameraObject;

		float m_FovY;   //射影角度
		float m_Aspect;	//アスペクト比

		float m_Width;	//遠近法を使わない場合（正射影）の幅
		float m_Height;	//遠近法を使わない場合（正射影）の高さ
		float m_Near;	//手前の最小距離
		float m_Far;	//奥の最大距離

		Matrix4X4 m_ViewMatrix;
		Matrix4X4 m_ProjMatrix;
		Impl() :
			m_Eye(0, 10.0f, -20.0f),	//デフォルトは後方斜め上
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
	const Vector3& Camera::GetEye() const { return pImpl->m_Eye; }
	void Camera::SetEye(const Vector3& Eye) { 
		pImpl->m_Eye = Eye;
		CalculateMatrix();
	}
	void Camera::SetEye(float x, float y, float z) { 
		pImpl->m_Eye = Vector3(x, y, z); 
		CalculateMatrix();
	}

	const Vector3& Camera::GetAt() const { return pImpl->m_At; }
	void Camera::SetAt(const Vector3& At) { 
		pImpl->m_At = At; 
		CalculateMatrix();
	}
	void Camera::SetAt(float x, float y, float z) { 
		pImpl->m_At = Vector3(x, y, z); 
		CalculateMatrix();
	}

	const Vector3& Camera::GetUp() const { return pImpl->m_Up;}
	void Camera::SetUp(const Vector3& Up) { 
		pImpl->m_Up = Up; 
		CalculateMatrix();
	}
	void Camera::SetUp(float x, float y, float z) { 
		pImpl->m_Up = Vector3(x, y, z); 
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
				pImpl->m_Eye = TransPtr->GetPosition();
				pImpl->m_ViewMatrix.LookAtLH(pImpl->m_Eye, pImpl->m_At, pImpl->m_Up);
				if (pImpl->m_Pers) {
					pImpl->m_ProjMatrix.PerspectiveFovLH(pImpl->m_FovY, pImpl->m_Aspect, pImpl->m_Near, pImpl->m_Far);
				}
				else {
					pImpl->m_ProjMatrix.OrthographicLH(pImpl->m_Width, pImpl->m_Height, pImpl->m_Near, pImpl->m_Far);
				}
			}
		}
		else {
			pImpl->m_ViewMatrix.LookAtLH(pImpl->m_Eye, pImpl->m_At, pImpl->m_Up);
			if (pImpl->m_Pers) {
				pImpl->m_ProjMatrix.PerspectiveFovLH(pImpl->m_FovY, pImpl->m_Aspect, pImpl->m_Near, pImpl->m_Far);
			}
			else {
				pImpl->m_ProjMatrix.OrthographicLH(pImpl->m_Width, pImpl->m_Height, pImpl->m_Near, pImpl->m_Far);
			}
		}
	}

	const Matrix4X4& Camera::GetViewMatrix() const { return pImpl->m_ViewMatrix; }
	const Matrix4X4& Camera::GetProjMatrix() const { return pImpl->m_ProjMatrix; }

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


		float m_RadY;
		float m_RadXZ;
		//カメラの上下スピード
		float m_CameraUpDownSpeed;
		//カメラを下げる下限角度
		float m_CameraUnderRot;
		float	m_Arm;
		//腕の長さの設定
		float m_MaxArm;
		float m_MinArm;
		//回転スピード
		float m_RotSpeed;
		//ズームスピード
		float m_ZoomSpeed;

		Impl() :
			m_ToTargetLerp(1.0f),
			m_RadY(0.5f),
			m_RadXZ(0),
			m_CameraUpDownSpeed(0.02f),
			m_CameraUnderRot(0.1f),
			m_Arm(5.0f),
			m_MaxArm(20.0f),
			m_MinArm(2.0f),
			m_RotSpeed(1.0f),
			m_ZoomSpeed(0.1f)
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
	{
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


	void LookAtCamera::OnUpdate() {
		auto CntlVec = App::GetApp()->GetInputDevice().GetControlerVec();
		//前回のターンからの時間
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Vector3 NewEye = GetEye();
		Vector3 NewAt = GetAt();
		//計算に使うための腕角度（ベクトル）
		Vector3 ArmVec = NewEye - NewAt;
		//正規化しておく
		ArmVec.Normalize();
		if (CntlVec[0].bConnected) {
			//上下角度の変更
			if (CntlVec[0].fThumbRY >= 0.1f) {
				pImpl->m_RadY += pImpl->m_CameraUpDownSpeed;
			}
			else if (CntlVec[0].fThumbRY <= -0.1f) {
				pImpl->m_RadY -= pImpl->m_CameraUpDownSpeed;
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
			if (CntlVec[0].fThumbRX != 0) {
				//回転スピードを反映
				pImpl->m_RadXZ += -CntlVec[0].fThumbRX * ElapsedTime * pImpl->m_RotSpeed;
				if (abs(pImpl->m_RadXZ) >= XM_2PI) {
					//1週回ったら0回転にする
					pImpl->m_RadXZ = 0;
				}
			}
			//クオータニオンでY回転（つまりXZベクトルの値）を計算
			Quaternion QtXZ;
			QtXZ.RotationAxis(Vector3(0, 1.0f, 0), pImpl->m_RadXZ);
			QtXZ.Normalize();
			//移動先行の行列計算することで、XZの値を算出
			Matrix4X4 Mat;
			Mat.STRTransformation(
				Vector3(1.0f, 1.0f, 1.0f),
				Vector3(0.0f, 0.0f, -1.0f),
				QtXZ
			);

			Vector3 PosXZ = Mat.PosInMatrixSt();
			//XZの値がわかったので腕角度に代入
			ArmVec.x = PosXZ.x;
			ArmVec.z = PosXZ.z;
			//腕角度を正規化
			ArmVec.Normalize();

			auto TargetPtr = GetTargetObject();
			if (TargetPtr) {
				//目指したい場所
				Matrix4X4 ToAtMat = TargetPtr->GetComponent<Transform>()->GetWorldMatrix();
				Vector3 ToAt = ToAtMat.PosInMatrixSt();
				NewAt = Lerp::CalculateLerp(GetAt(), ToAt, 0, 1.0f, 1.0f, Lerp::Linear);
			}
			//アームの変更
			//Dパッド下
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				//カメラ位置を引く
				pImpl->m_Arm += pImpl->m_ZoomSpeed;
				if (pImpl->m_Arm >= pImpl->m_MaxArm) {
					//m_MaxArm以上離れないようにする
					pImpl->m_Arm = pImpl->m_MaxArm;
				}
			}
			//Dパッド上
			if (CntlVec[0].wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				//カメラ位置を寄る
				pImpl->m_Arm -= pImpl->m_ZoomSpeed;
				if (pImpl->m_Arm <= pImpl->m_MinArm) {
					//m_MinArm以下近づかないようにする
					pImpl->m_Arm = pImpl->m_MinArm;
				}
			}
			////目指したい場所にアームの値と腕ベクトルでEyeを調整
			NewEye = NewAt + ArmVec * pImpl->m_Arm;
		}
		SetEye(NewEye);
		SetAt(NewAt);
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
		explicit Impl(const shared_ptr<Stage>& StagePtr) :
			m_Stage(StagePtr)
		{}
		~Impl() {}
	};
	//--------------------------------------------------------------------------------------
	///	ビューコンポーネント（親）
	//--------------------------------------------------------------------------------------
	ViewBase::ViewBase(const shared_ptr<Stage>& StagePtr) :
		ObjectInterface(),
		ShapeInterface(),
		pImpl(new Impl(StagePtr))
	{
	}

	ViewBase::~ViewBase() {}

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
	///	struct ViewBase::Impl;
	//--------------------------------------------------------------------------------------
	struct LightBase::Impl {
		weak_ptr<Stage> m_Stage;
		explicit Impl(const shared_ptr<Stage>& StagePtr) :
			m_Stage(StagePtr)
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




}
//end basecross
