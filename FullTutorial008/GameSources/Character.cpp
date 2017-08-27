/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//	class EggAnime :  public SS5ssae;
	//	用途: 卵のアニメーション
	//--------------------------------------------------------------------------------------
	//構築と消滅
	EggAnime::EggAnime(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir) :
		SS5ssae(StagePtr, BaseDir, L"character_template_2head.ssae", L"attack1", true),
		m_Span(100.0f)
	{}
	EggAnime::~EggAnime() {}
	//初期化
	void EggAnime::OnCreate() {
		//元となるオブジェクトからアニメーションオブジェクトへの行列の設定
		//作成されたグラフィックとの調整に使用
		Mat4x4 mat;
		mat.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0, 0, 0),
			Vec3(0, 0, 0),
			Vec3(0, 0, 0)
		);
		SetToAnimeMatrix(mat);

		//位置の初期化
		auto PtrT = AddComponent<Transform>();
		//スプライト版のSpriteStdioは、サイズ32.0f基準のスプライトを作成するので
		//それに合わせてスケーリングする
		PtrT->SetScale(-8.0f, 8.0f, 1.0f);
		//ポジションはピクセル単位センター合わせ（固定）
		PtrT->SetPosition(Vec3(0, -250.0f, 0.0f));
		//親クラスのOnCreateを必ず呼ぶ
		SS5ssae::OnCreate();
		//値は秒あたりのフレーム数
		SetFps(30.0f);
	}
	void EggAnime::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();

		auto PtrT = GetComponent<Transform>();
		auto Pos = PtrT->GetPosition();
		Pos.x += (m_Span * ElapsedTime);
		if (abs(Pos.x) > 400.0f) {
			m_Span *= -1.0f;
		}
		PtrT->SetPosition(Pos);
		//移動方向に向きを変える
		if (m_Span < 0) {
			PtrT->SetScale(8, 8, 1);
		}
		else {
			PtrT->SetScale(-8, 8, 1);
		}
		//アニメーションを更新する
		UpdateAnimeTime(ElapsedTime);
	}

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vec3& Scale,
		const Vec3& Rotation,
		const Vec3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox() {}

	//初期化
	void FixedBox::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//衝突判定
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);
		PtrObb->SetDrawActive(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
	}



}
//end basecross
