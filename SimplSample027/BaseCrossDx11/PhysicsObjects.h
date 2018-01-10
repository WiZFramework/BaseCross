/*!
@file PhysicsObjects.h
@brief 物理計算オブジェクト
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Scene;
	class Stage;
	class GameObject;


	//--------------------------------------------------------------------------------------
	///	物理計算する固定のボックス
	//--------------------------------------------------------------------------------------
	class FixedPsBox : public GameObject {
		//メッシュ
		shared_ptr<MeshResource> m_BoxMesh;
		//物理計算用のオブジェクト
		shared_ptr<PsBox> m_PhysicsBox;
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;

		//描画データ
		shared_ptr<BcDrawObject> m_PtrObj;
		//描画オブジェクト(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
		//シャドウマップ用描画データ
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//シャドウマップ描画オブジェクト(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;


	public:
		//構築と破棄
		FixedPsBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~FixedPsBox();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override {}
		//シャドウマップ描画
		virtual void OnDrawShadowmap() override;
		//描画
		virtual void OnDraw()override;

	};

	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブなボックス
	//--------------------------------------------------------------------------------------
	class ActivePsBox : public GameObject {
		//メッシュ
		shared_ptr<MeshResource> m_BoxMesh;
		//物理計算用のオブジェクト
		shared_ptr<PsBox> m_PhysicsBox;
		Vec3 m_Scale;
		Quat m_Qt;
		Vec3 m_Position;

		//描画データ
		shared_ptr<BcDrawObject> m_PtrObj;
		//描画オブジェクト(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
		//シャドウマップ用描画データ
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//シャドウマップ描画オブジェクト(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;

	public:
		//構築と破棄
		ActivePsBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsBox();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//シャドウマップ描画
		virtual void OnDrawShadowmap() override;
		//描画
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	物理計算するアクティブな球体
	//--------------------------------------------------------------------------------------
	class ActivePsSphere : public GameObject {
		//メッシュ
		shared_ptr<MeshResource> m_SphereMesh;
		//物理計算用のオブジェクト
		shared_ptr<PsSphere> m_PhysicsSphere;
		float m_Scale;
		Quat m_Qt;
		Vec3 m_Position;

		//描画データ
		shared_ptr<BcDrawObject> m_PtrObj;
		//描画オブジェクト(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
		//シャドウマップ用描画データ
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//シャドウマップ描画オブジェクト(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;

	public:
		//構築と破棄
		ActivePsSphere(const shared_ptr<Stage>& StagePtr,
			float Scale,
			const Quat& Qt,
			const Vec3& Position
		);
		virtual ~ActivePsSphere();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//シャドウマップ描画
		virtual void OnDrawShadowmap() override;
		//描画
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	///	物理計算する発射する球体
	//--------------------------------------------------------------------------------------
	class FirePsSphere : public GameObject {
		//メッシュ
		shared_ptr<MeshResource> m_SphereMesh;
		//物理計算用のオブジェクト
		shared_ptr<PsSphere> m_PhysicsSphere;
		Vec3 m_Emitter;
		Vec3 m_Velocity;
		//描画データ
		shared_ptr<BcDrawObject> m_PtrObj;
		//描画オブジェクト(weak_ptr)
		weak_ptr<BcPNTStaticRenderer> m_Renderer;
		//シャドウマップ用描画データ
		shared_ptr<ShadowmapObject> m_PtrShadowmapObj;
		//シャドウマップ描画オブジェクト(weak_ptr)
		weak_ptr<ShadowmapRenderer> m_ShadowmapRenderer;

	public:
		//構築と破棄
		FirePsSphere(const shared_ptr<Stage>& StagePtr,
			const Vec3& Emitter, const Vec3& Velocity
		);
		virtual ~FirePsSphere();
		//初期化
		virtual void OnCreate() override;
		//更新
		virtual void OnUpdate() override;
		//物体をリセットする
		void Reset(const Vec3& Emitter, const Vec3& Velocity);

		//シャドウマップ描画
		virtual void OnDrawShadowmap() override;
		//描画
		virtual void OnDraw()override;

	};





}
// end basecross
