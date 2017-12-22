/*!
@file EfkInterface.cpp
@brief Effekseerエフェクトのインターフェイス実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	Effekseerエフェクトのエフェクト
	//--------------------------------------------------------------------------------------
	EfkEffect::EfkEffect(const shared_ptr<EfkInterface>& iface, const wstring& filename):
		m_FileName(filename),
		m_EfkInterface(iface),
		m_Effect(nullptr)
	{
		try {
			if (m_FileName == L"") {
				throw BaseException(
					L"エフェクトファイル名が空白です。",
					L"if (m_FileName == L\"\")",
					L"EfkEffect::EfkEffect()"
				);
			}
			// エフェクトの読込
			m_Effect = Effekseer::Effect::Create(iface->m_Manager, (const EFK_CHAR*)m_FileName.c_str());
		}
		catch (...) {
			throw;
		}
	}
	EfkEffect::~EfkEffect() {
		ES_SAFE_RELEASE(m_Effect);
	}

	void EfkEffect::OnCreate() {
	}

	//--------------------------------------------------------------------------------------
	///	EffekseerエフェクトのPlayオブジェクト
	//--------------------------------------------------------------------------------------
	EfkPlay::EfkPlay(const shared_ptr<EfkEffect>& effect, const bsm::Vec3& Emitter):
		m_Handle(-1)
	{
		try {
			auto iface = effect->m_EfkInterface.lock();
			if (iface) {
				m_Handle = iface->m_Manager->Play(effect->m_Effect, Emitter.x, Emitter.y, Emitter.z);
				m_EfkInterface = iface;
			}
		}
		catch (...) {
			throw;
		}
	}

	EfkPlay::~EfkPlay() {
		StopEffect();
	}

	void EfkPlay::AddLocation(const bsm::Vec3& Location) {
		auto shptr = m_EfkInterface.lock();
		if (shptr && m_Handle != -1) {
			shptr->m_Manager->AddLocation(m_Handle, ::Effekseer::Vector3D(Location.x, Location.y, Location.z));
		}
	}

	void EfkPlay::StopEffect() {
		auto shptr = m_EfkInterface.lock();
		if (shptr && m_Handle != -1) {
			shptr->m_Manager->StopEffect(m_Handle);
		}
	}

	//--------------------------------------------------------------------------------------
	///	Effekseerエフェクトのインターフェイス
	//--------------------------------------------------------------------------------------
	EfkInterface::EfkInterface():
		ObjectInterface(),
		m_Manager(nullptr),
		m_Renderer(nullptr)
	{}
	EfkInterface::~EfkInterface() {
		// 先にエフェクト管理用インスタンスを破棄
		m_Manager->Destroy();
		// 次に描画用インスタンスを破棄
		m_Renderer->Destroy();
	}

	void EfkInterface::OnCreate() {
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		// 描画用インスタンスの生成
		m_Renderer = ::EffekseerRendererDX11::Renderer::Create(pDx11Device, pID3D11DeviceContext, 2000);
		// エフェクト管理用インスタンスの生成
		m_Manager = ::Effekseer::Manager::Create(2000);

		// 描画用インスタンスから描画機能を設定
		m_Manager->SetSpriteRenderer(m_Renderer->CreateSpriteRenderer());
		m_Manager->SetRibbonRenderer(m_Renderer->CreateRibbonRenderer());
		m_Manager->SetRingRenderer(m_Renderer->CreateRingRenderer());
		m_Manager->SetTrackRenderer(m_Renderer->CreateTrackRenderer());
		m_Manager->SetModelRenderer(m_Renderer->CreateModelRenderer());

		// 描画用インスタンスからテクスチャの読込機能を設定
		// 独自拡張可能、現在はファイルから読み込んでいる。
		m_Manager->SetTextureLoader(m_Renderer->CreateTextureLoader());
		m_Manager->SetModelLoader(m_Renderer->CreateModelLoader());
	}

	void  EfkInterface::OnUpdate() {
		// エフェクトの更新処理を行う
		m_Manager->Update();
	}

	void EfkInterface::OnDraw() {
		// エフェクトの描画開始処理を行う。
		m_Renderer->BeginRendering();

		// エフェクトの描画を行う。
		m_Manager->Draw();

		// エフェクトの描画終了処理を行う。
		m_Renderer->EndRendering();

	}

	void Mat4x4ToMatrix44(const bsm::Mat4x4& src, Effekseer::Matrix44& dest) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				dest.Values[i][j] = src(i, j);
			}
		}
	}

	void  EfkInterface::SetViewProj(const bsm::Mat4x4& view, const bsm::Mat4x4& proj) {
		Effekseer::Matrix44 v,p;
		Mat4x4ToMatrix44(view, v);
		Mat4x4ToMatrix44(proj, p);
		m_Renderer->SetCameraMatrix(v);
		m_Renderer->SetProjectionMatrix(p);
	}







}
// end basecross
