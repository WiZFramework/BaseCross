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
		pImpl->m_ViewItem.m_Camera->CalculateMatrix();
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


	////--------------------------------------------------------------------------------------
	////	Implクラス
	////--------------------------------------------------------------------------------------
	//struct Light::Impl {
	//	Vector3 m_Directional;	//ライトの向き
	//	Color4 m_DiffuseColor;	//ディフィーズ色
	//	Color4 m_SpecularColor;	//スペキュラー色
	//	Impl() :
	//		m_Directional(0, 0, 0),
	//		m_DiffuseColor(1.0000000f, 0.9607844f, 0.8078432f, 1.0f),
	//		m_SpecularColor(1.0000000f, 0.9607844f, 0.8078432f, 1.0f)
	//	{
	//	}
	//	~Impl() {}
	//};

	////--------------------------------------------------------------------------------------
	////	ライト
	////--------------------------------------------------------------------------------------
	////構築と破棄
	//Light::Light() :
	//	ObjectInterface(),
	//	pImpl(new Impl())
	//{
	//	SetPositionToDirectional(Vector3(-1.0f, 1.0f, -1.0f));
	//}
	//Light::Light(
	//	const Vector3& Directional,
	//	const Color4& DiffuseColor,
	//	const Color4& SpecularColor
	//) :
	//	ObjectInterface(),
	//	pImpl(new Impl())
	//{
	//	pImpl->m_Directional = Directional;
	//	pImpl->m_DiffuseColor = DiffuseColor;
	//	pImpl->m_SpecularColor = SpecularColor;
	//}

	//Light::~Light() {}

	////アクセサ
	//const Vector3& Light::GetDirectional() const {
	//	return pImpl->m_Directional;
	//}
	//void Light::SetDirectional(const Vector3& Directional) {
	//	pImpl->m_Directional = Directional;
	//	pImpl->m_Directional.Normalize();
	//}
	//void Light::SetDirectional(float x, float y, float z) {
	//	pImpl->m_Directional = Vector3(x, y, z);
	//	pImpl->m_Directional.Normalize();
	//}
	//void Light::SetPositionToDirectional(const Vector3& Position) {
	//	pImpl->m_Directional = Position;
	//	pImpl->m_Directional *= -1.0f;
	//	pImpl->m_Directional.Normalize();
	//}
	//void Light::SetPositionToDirectional(float x, float y, float z) {
	//	pImpl->m_Directional = Vector3(x, y, z);
	//	pImpl->m_Directional *= -1.0f;
	//	pImpl->m_Directional.Normalize();
	//}

	//const Color4& Light::GetDiffuseColor() const {
	//	return pImpl->m_DiffuseColor;
	//}
	//void Light::SetDiffuseColor(const Color4& col) {
	//	pImpl->m_DiffuseColor = col;
	//}
	//void Light::SetDiffuseColor(float r, float g, float b, float a) {
	//	pImpl->m_DiffuseColor = Color4(r, g, b, a);
	//}

	//const Color4& Light::GetSpecularColor() const {
	//	return pImpl->m_SpecularColor;
	//}
	//void Light::SetSpecularColor(const Color4& col) {
	//	pImpl->m_SpecularColor = col;
	//}
	//void Light::SetSpecularColor(float r, float g, float b, float a) {
	//	pImpl->m_SpecularColor = Color4(r, g, b, a);
	//}

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
