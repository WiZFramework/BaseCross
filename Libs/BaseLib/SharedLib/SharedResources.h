/*!
@file SharedResources.h
@brief 共有のリソース（ビュー、カメラ、ライトなど）
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	class GameObject;
	class Stage;

	//--------------------------------------------------------------------------------------
	///	ビュー関連
	//--------------------------------------------------------------------------------------
	struct Viewport {
		float TopLeftX;
		float TopLeftY;
		float Width;
		float Height;
		float MinDepth;
		float MaxDepth;
	};

	//--------------------------------------------------------------------------------------
	///	カメラ
	//--------------------------------------------------------------------------------------
	class Camera :public ObjectInterface {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		explicit Camera();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Camera();
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの位置を得る
		@return	カメラの位置
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetEye() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの位置を設定する
		@param[in]	Eye	カメラ位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetEye(const Vector3& Eye);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの位置を設定する
		@param[in]	x	x位置
		@param[in]	y	y位置
		@param[in]	z	z位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetEye(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの視点を得る
		@return	カメラの視点
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetAt() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの視点を設定する
		@param[in]	At	視点位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAt(const Vector3& At);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの視点を設定する
		@param[in]	x	x位置
		@param[in]	y	y位置
		@param[in]	z	z位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAt(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの傾きを得る
		@return	カメラの傾き
		*/
		//--------------------------------------------------------------------------------------
		const Vector3& GetUp() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの傾きを設定する
		@param[in]	Up	傾き
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetUp(const Vector3& Up);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの傾きを設定する
		@param[in]	x	x方向
		@param[in]	y	y方向
		@param[in]	z	z方向
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetUp(float x, float y, float z);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 遠近法を使うかどうかを得る
		@return	遠近法を使えばtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsPers()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 遠近法を使うかどうかを得る
		@return	遠近法を使えばtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetPers()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 遠近法を使うかどうかを設定する
		@param[in]	b	使えばtrue
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPers(bool b);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 画角を得る（遠近法が設定されているとき用）
		@return	画角
		*/
		//--------------------------------------------------------------------------------------
		float GetFovY() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 画角を設定する（遠近法が設定されているとき用）
		@param[in]	f	画角
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFovY(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief アスペクト比を得る（遠近法が設定されているとき用）
		@return	アスペクト比
		*/
		//--------------------------------------------------------------------------------------
		float GetAspect() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief アスペクト比を設定する（遠近法が設定されているとき用）
		@param[in]	f	アスペクト比
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAspect(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 画面の幅を得る（遠近法が設定されないとき用）
		@return	画面の幅
		*/
		//--------------------------------------------------------------------------------------
		float GetWidth() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 画面の幅を設定する（遠近法が設定されないとき用）
		@param[in]	f	画面の幅
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetWidth(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 画面の高さを得る（遠近法が設定されないとき用）
		@return	画面の高さ
		*/
		//--------------------------------------------------------------------------------------
		float GetHeight() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 画面の高さを設定する（遠近法が設定されないとき用）
		@param[in]	f	画面の高さ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetHeight(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief Near距離を得る
		@return	Near距離
		*/
		//--------------------------------------------------------------------------------------
		float GetNear() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief Near距離を設定する
		@param[in]	f	Far距離
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetNear(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief Far距離を得る
		@return	Far距離
		*/
		//--------------------------------------------------------------------------------------
		float GetFar() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief Far距離を設定する
		@param[in]	f	Far距離
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFar(float f);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 現在のカメラオブジェクトを得る
		@return	カメラオブジェクト（設定なければnullptr）
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<GameObject> GetCameraObject() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラオブジェクトを設定する
		@param[in]	Obj	カメラオブジェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetCameraObject(const shared_ptr<GameObject>& Obj);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラオブジェクトをクリアする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ClearCameraObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ビューポートを設定する
		@param[in]	v	ビューポート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetViewPort(const Viewport& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 行列を計算する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CalculateMatrix();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ビュー行列を得る
		@return	ビュー行列
		*/
		//--------------------------------------------------------------------------------------
		const Matrix4X4& GetViewMatrix() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 射影行列を得る
		@return	射影行列
		*/
		//--------------------------------------------------------------------------------------
		const Matrix4X4& GetProjMatrix() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化を行う
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	///	ビュー（親）
	//--------------------------------------------------------------------------------------
	class ViewBase :public ObjectInterface, public ShapeInterface {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	StagePtr	このコンポーネントを所持するステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit ViewBase(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ViewBase();
	public:
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理。デフォルトは何も行わない
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画に使用するカメラを得る
		@return	描画に使用するカメラ
		*/
		//--------------------------------------------------------------------------------------
		virtual const shared_ptr<Camera>& GetTargetCamera() const = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するビューポートを得る
		@return	描画するビューポート
		*/
		//--------------------------------------------------------------------------------------
		virtual const Viewport& GetTargetViewport() const = 0;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	///	シングルビュー
	//--------------------------------------------------------------------------------------
	class SingleView : public ViewBase {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	このコンポーネントを所持するステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit SingleView(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SingleView();
		//--------------------------------------------------------------------------------------
		/*!
		@brief ビューポートを設定する
		@param[in]	v	ビューポート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetViewport(const Viewport& v);
		//--------------------------------------------------------------------------------------
		/*!
		@brief ビューポートを得る
		@return	ビューポート
		*/
		//--------------------------------------------------------------------------------------
		const Viewport& GetViewport() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラを設定する
		@param[in]	c	カメラのポインタ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetCamera(const shared_ptr<Camera>& c);
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラを得る
		@return	カメラのポインタ
		*/
		//--------------------------------------------------------------------------------------
		const shared_ptr<Camera>& GetCamera()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画に使用するカメラを得る
		@return	描画に使用するカメラ
		*/
		//--------------------------------------------------------------------------------------
		virtual const shared_ptr<Camera>& GetTargetCamera() const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するビューポートを得る
		@return	描画するビューポート
		*/
		//--------------------------------------------------------------------------------------
		virtual const Viewport& GetTargetViewport() const override;

		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化を行う
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief	更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画処理。デフォルトは何も行わない
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override {}

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	ライト
	//--------------------------------------------------------------------------------------
	struct Light {
		Vector3 m_Directional;	//ライトの向き
		Color4 m_DiffuseColor;	//ディフィーズ色
		Color4 m_SpecularColor;	//スペキュラー色
		Light() :
			m_Directional(0, -1.0f, 0),
			m_DiffuseColor(1.0000000f, 0.9607844f, 0.8078432f, 1.0f),
			m_SpecularColor(1.0000000f, 0.9607844f, 0.8078432f, 1.0f)
		{
		}
		~Light() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ライト位置からの向きを設定する
		@param[in]	Position	ライトの位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPositionToDirectional(const Vector3& Position) {
			m_Directional = Position;
			m_Directional *= -1.0f;
			m_Directional.Normalize();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ライト位置からの向きを設定する
		@param[in]	x	ライト位置x
		@param[in]	y	ライト位置y
		@param[in]	z	ライト位置z
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPositionToDirectional(float x, float y, float z) {
			m_Directional = Vector3(x, y, z);
			m_Directional *= -1.0f;
			m_Directional.Normalize();
		}
	};

	//--------------------------------------------------------------------------------------
	///	ステージに設定するライト（親）
	//--------------------------------------------------------------------------------------
	class LightBase :public ObjectInterface {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンストラクタ
		@param[in]	StagePtr	このコンポーネントを所持するステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit LightBase(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~LightBase();
	public:
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画に使用するライトを得る
		@return	描画に使用するライト
		*/
		//--------------------------------------------------------------------------------------
		virtual const Light& GetTargetLight() const = 0;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	シングルライト
	//--------------------------------------------------------------------------------------
	class SingleLight : public LightBase {
		Light m_Light;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	このコンポーネントを所持するステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit SingleLight(const shared_ptr<Stage>& StagePtr) :LightBase(StagePtr) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SingleLight() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ライトを設定する
		@param[in]	l	ライトの参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetLight(const Light& l) {
			m_Light = l;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ライトを得る
		@return	ライトの参照
		*/
		//--------------------------------------------------------------------------------------
		const Light& GetLight()const {
			return m_Light;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ライトを得る
		@return	ライトの参照(更新用)
		*/
		//--------------------------------------------------------------------------------------
		Light& GetLight() {
			return m_Light;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画に使用するライトを得る
		@return	描画に使用するライト
		*/
		//--------------------------------------------------------------------------------------
		virtual const Light& GetTargetLight() const {
			return m_Light;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化を行う
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override {}
	};






	////--------------------------------------------------------------------------------------
	////	ライト
	////--------------------------------------------------------------------------------------
	//class Light : public ObjectInterface {
	//public:
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief	コンストラクタ
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	explicit Light();
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief	コンストラクタ
	//	@param[in]	Directional	ライトの向き
	//	@param[in]	DiffuseColor	デフューズ色
	//	@param[in]	SpecularColor	スペキュラー色
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	Light(
	//		const Vector3& Directional,
	//		const Color4& DiffuseColor,
	//		const Color4& SpecularColor
	//	);
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief	デストラクタ
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	virtual ~Light();
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief ライトの向きを得る
	//	@return	ライトの向き
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	const Vector3& GetDirectional() const;
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief ライトの向きを設定する
	//	@param[in]	Directional	ライトの向き
	//	@return	なし
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	void SetDirectional(const Vector3& Directional);
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief ライトの向きを設定する
	//	@param[in]	x	ライトの向きx
	//	@param[in]	y	ライトの向きy
	//	@param[in]	z	ライトの向きz
	//	@return	なし
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	void SetDirectional(float x, float y, float z);
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief ライト位置からの向きを設定する
	//	@param[in]	Position	ライトの位置
	//	@return	なし
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	void SetPositionToDirectional(const Vector3& Position);
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief ライト位置からの向きを設定する
	//	@param[in]	x	ライト位置x
	//	@param[in]	y	ライト位置y
	//	@param[in]	z	ライト位置z
	//	@return	なし
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	void SetPositionToDirectional(float x, float y, float z);
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief デフューズ色を得る
	//	@return	デフューズ色
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	const Color4& GetDiffuseColor()const;
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief デフューズ色を設定する
	//	@param[in]	col	デフューズ色
	//	@return	なし
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	void SetDiffuseColor(const Color4& col);
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief デフューズ色を設定する
	//	@param[in]	x	デフューズ色r
	//	@param[in]	g	デフューズ色g
	//	@param[in]	b	デフューズ色b
	//	@param[in]	a	デフューズ色a
	//	@return	なし
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	void SetDiffuseColor(float r, float g, float b, float a);
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief スペキュラー色を得る
	//	@return	スペキュラー色
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	const Color4& GetSpecularColor() const;
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief スペキュラー色を設定する
	//	@param[in]	col	スペキュラー色
	//	@return	なし
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	void SetSpecularColor(const Color4& col);
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief スペキュラー色を設定する
	//	@param[in]	x	スペキュラー色r
	//	@param[in]	g	スペキュラー色g
	//	@param[in]	b	スペキュラー色b
	//	@param[in]	a	スペキュラー色a
	//	@return	なし
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	void SetSpecularColor(float r, float g, float b, float a);
	//	//--------------------------------------------------------------------------------------
	//	/*!
	//	@brief 初期化を行う
	//	@return	なし
	//	*/
	//	//--------------------------------------------------------------------------------------
	//	virtual void OnCreate()override {}
	//private:
	//	// pImplイディオム
	//	struct Impl;
	//	unique_ptr<Impl> pImpl;
	//};








}
//end basecross
