/*!
@file Scene.h
@brief シーンなど
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	class Scene;
	class GameObject;

	//--------------------------------------------------------------------------------------
	///	カメラ
	//--------------------------------------------------------------------------------------
	struct Camera {
		Vec3 m_CamerEye;			///<カメラ位置
		Vec3 m_CamerAt;			///<カメラ視点
		Vec3 m_CamerUp;			///<カメラ傾き
		float m_FovY;				///<画角
		float m_CameraXZRad;			//カメラのXZラジアン
		float m_CameraYRad;				//カメラのYラジアン
		float m_CameraArmLen;		//カメラの腕の長さ
		Camera():
			m_CamerEye(0, 5.0, -5.0f),
			m_CamerAt(0, 0, 0),
			m_CamerUp(0, 1.0f, 0),
			m_FovY(XM_PIDIV4),
			m_CameraXZRad(0.0f),
			m_CameraYRad(XM_PIDIV4 + 0.5f),
			m_CameraArmLen(5.0f)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ビュー行列と射影行列の取得（各参照パラメータに代入）
		@param[out]	View	ビュー行列を受け取る参照
		@param[out]	Proj	射影行列を受け取る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetViewProjMatrix(Mat4x4& View, Mat4x4& Proj)const {
			View = XMMatrixLookAtLH(m_CamerEye, m_CamerAt, m_CamerUp);
			float w = static_cast<float>(App::GetApp()->GetGameWidth());
			float h = static_cast<float>(App::GetApp()->GetGameHeight());
			Proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, w / h, 1.0f, 100.0f);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラのEyeとAtを取得
		@param[out]	Eye	Eyeを受け取る参照
		@param[out]	At	Atを受け取る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetCameraEyeAt(Vec3& Eye, Vec3& At)const {
			Eye = m_CamerEye;
			At = m_CamerAt;
		}
	};

	//--------------------------------------------------------------------------------------
	//	シングルライト
	//--------------------------------------------------------------------------------------
	struct SingleLight {
		Vec3 m_Directional;	//ライトの向き
		Col4 m_DiffuseColor;	//ディフィーズ色
		Col4 m_SpecularColor;	//スペキュラー色
		Col4 m_AmbientLightColor;			//アンビエント色
		SingleLight()
		{
			SetDefaultLighting();
		}
		SingleLight(const Vec3& dir, const Col4& def, const Col4& sp, const Col4& am) :
			m_Directional(dir),
			m_DiffuseColor(def),
			m_SpecularColor(sp),
			m_AmbientLightColor(am)
		{
		}
		~SingleLight() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ライト位置からの向きを設定する
		@param[in]	Position	ライトの位置
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPositionToDirectional(const Vec3& Position) {
			m_Directional = Position;
			m_Directional *= -1.0f;
			m_Directional.normalize();
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
			m_Directional = bsm::Vec3(x, y, z);
			m_Directional *= -1.0f;
			m_Directional.normalize();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デフォルトのライティングを設定する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDefaultLighting() {
			m_Directional = Vec3(0.4545195f, -0.7660444f,  0.4545195f);
			m_Directional.normalize();
			m_DiffuseColor = Col4(0.3231373f, 0.3607844f, 0.3937255f, 0.0f);
			m_SpecularColor = Col4(0.3231373f, 0.3607844f, 0.3937255f, 0.0f);
			m_AmbientLightColor = Col4( 0.05333332f, 0.09882354f, 0.1819608f ,0.0f );
		}
	};


	//--------------------------------------------------------------------------------------
	//	マルチライト
	//--------------------------------------------------------------------------------------
	struct MultiLights {
		Vec3 m_Directional[3];	//ライトの向き
		Col4 m_DiffuseColor[3];	//ディフィーズ色
		Col4 m_SpecularColor[3];	//スペキュラー色
		Col4 m_AmbientLightColor;			//アンビエント色
		size_t m_MainColorIndex;
		MultiLights()
		{
			SetDefaultLighting();
			m_MainColorIndex = 2;
		}
		~MultiLights() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デフォルトのライティングを設定する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetDefaultLighting() {
			static const Vec3 defaultDirections[3] =
			{
				{ -0.5265408f, -0.5735765f, -0.6275069f },
				{ 0.7198464f,  0.3420201f,  0.6040227f },
				{ 0.4545195f, -0.7660444f,  0.4545195f },
			};
			static const Col4 defaultDiffuse[3] =
			{
				{ 1.0000000f, 0.9607844f, 0.8078432f,0.0f },
				{ 0.9647059f, 0.7607844f, 0.4078432f,0.0f },
				{ 0.3231373f, 0.3607844f, 0.3937255f,0.0f },
			};

			static const Col4 defaultSpecular[3] =
			{
				{ 1.0000000f, 0.9607844f, 0.8078432f,0.0f },
				{ 0.0000000f, 0.0000000f, 0.0000000f,0.0f },
				{ 0.3231373f, 0.3607844f, 0.3937255f,0.0f },
			};


			m_AmbientLightColor = Col4(0.05333332f, 0.09882354f, 0.1819608f, 0.0f);
			for (size_t i = 0; i < 3; i++) {
				m_Directional[i] = defaultDirections[i];
				m_Directional[i].normalize();
				m_DiffuseColor[i] = defaultDiffuse[i];
				m_SpecularColor[i] = defaultSpecular[i];
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief メインのライトを取得する
		@param[out]	Light	受け取るライト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetMainLight(SingleLight& light) {
			light.m_Directional = m_Directional[m_MainColorIndex];
			light.m_DiffuseColor = m_DiffuseColor[m_MainColorIndex];
			light.m_SpecularColor = m_SpecularColor[m_MainColorIndex];
			light.m_AmbientLightColor = m_AmbientLightColor;
		}
	};

	class RigidbodyManager;
	struct Rigidbody;
	struct CollisionState;

	//--------------------------------------------------------------------------------------
	///	ステージ（シーンで管理するインターフェイス）
	//--------------------------------------------------------------------------------------
	class Stage : public ObjectInterface, public ShapeInterface {
		//マルチライト
		MultiLights m_MultiLights;
		//カメラ
		Camera m_Camera;
		//オブジェクトの配列
		vector< shared_ptr<GameObject> > m_GameObjectVec;
		//途中にオブジェクトが追加された場合、ターンの開始まで待つ配列
		vector< shared_ptr<GameObject> > m_WaitAddObjectVec;
		//途中にオブジェクトが削除された場合、ターンの開始まで待つ配列
		vector< shared_ptr<GameObject> > m_WaitRemoveObjectVec;
		//追加オブジェクトの指定
		void PushBackGameObject(const shared_ptr<GameObject>& Ptr);
		//削除オブジェクトの指定
		void RemoveBackGameObject(const shared_ptr<GameObject>& Ptr);
		//オブジェクトの削除
		void RemoveTargetGameObject(const shared_ptr<GameObject>& targetobj);
		//追加や削除待ちになってるオブジェクトを追加削除する
		void SetWaitToObjectVec();
	protected:
		//Rigidbodyマネージャ
		shared_ptr<RigidbodyManager> m_RigidbodyManager;
		//--------------------------------------------------------------------------------------
		/*!
		@brief プロテクトコンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Stage(){
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Stage() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief カメラの取得
		@return	カメラの参照
		*/
		//--------------------------------------------------------------------------------------
		const Camera& GetCamera()const {
			return m_Camera;
		}
		Camera& GetCamera() {
			return m_Camera;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief メインライト向きの取得
		@param[out]	LightDir	ライト向き受け取る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetLightDir(Vec4& LightDir)const {
			LightDir = m_MultiLights.m_Directional[m_MultiLights.m_MainColorIndex];
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief マルチライトの取得
		@return	マルチライトの参照
		*/
		//--------------------------------------------------------------------------------------
		const MultiLights& GetMultiLights()const {
			return m_MultiLights;
		}
		MultiLights& GetMultiLights() {
			return m_MultiLights;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 前初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステージ更新準備（仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreUpdateStage() {
			SetWaitToObjectVec();
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステージ更新（純粋仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdateStage() = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief ステージ描画（純粋仮想関数）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawStage() = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトを追加する
		@tparam	T	作成する型
		@tparam	Ts	可変長パラメータの型
		@param[in]	params	可変長パラメータ
		@return	作成されたゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> AddGameObject(Ts&&... params) {
			try {
				auto Ptr = ObjectFactory::Create<T>(GetThis<Stage>(), params...);
				PushBackGameObject(Ptr);
				return Ptr;
			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトの配列を得る
		@return	ゲームオブジェクトの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector< shared_ptr<GameObject> >& GetGameObjectVec()const {
			return m_GameObjectVec;
		}
		vector< shared_ptr<GameObject> >& GetGameObjectVec() {
			return m_GameObjectVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インスタンスからそのゲームオブジェクトが指定した型かどうかを得る
		@tparam	T	調べる型
		@param[in]	Obj	インスタンス
		@return	指定した型で存在すればtrue
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		bool FindGameObject(const shared_ptr<GameObject>& Obj) const{
			auto shptr = dynamic_pointer_cast<T>(Obj);
			if (shptr) {
				for (auto ptr : GetGameObjectVec()) {
					if (Obj == ptr) {
						return true;
					}
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	タグを持つそのゲームオブジェクトを得る
		@tparam	T	調べる型
		@param[in]	TagName	タグ名
		@param[in]	ExceptionActive	対象がnullだった場合に例外処理するかどうか
		@return	 指定のタグを持つ最初のオブジェクト（保存してはいけない）
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> FindTagGameObject(const wstring& TagName,bool ExceptionActive = true) const {
			for (auto& v : GetGameObjectVec()) {
				if (v->FindTag(TagName)) {
					auto shptr = dynamic_pointer_cast<T>(v);
					if (shptr) {
						return shptr;
					}
				}
			}
			if (ExceptionActive) {
				throw BaseException(
					L"オブジェクトが見つかりません",
					TagName,
					L"Stage::FindGameObject()"
				);
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	タグからそのゲームオブジェクトを得る
		@param[in]	TagName	タグ名
		@param[out]	Ret GameObjectの配列(shared_ptrの配列)
		@return	 指定のタグを持つオブジェクトの配列を得る。使用後は速やかに解放すること
		*/
		//--------------------------------------------------------------------------------------
		void FindTagGameObjectVec(const wstring& TagName, vector<shared_ptr<GameObject>>& Ret) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ゲームオブジェクトを削除する
		@tparam	T	削除する型
		@param[in]	Obj	インスタンス
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void RemoveGameObject(const shared_ptr<GameObject>& Obj) {
			if (FindGameObject<T>(Obj)) {
				RemoveBackGameObject(Obj);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Rigidbodyマネージャを得る
		@return	Rigidbodyマネージャ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<RigidbodyManager> GetRigidbodyManager() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief Rigidbodyを登録する
		@return	追加したRigidbodyのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Rigidbody> AddRigidbody(const Rigidbody& body);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 指定のオーナーのRigidbodyを削除する（見つからなければ何もしない）
		@param[in]	OwnerPtr	オーナーのポインタ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void RemoveOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief Rigidbodyの配列を得る
		@return	Rigidbodyの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<shared_ptr<Rigidbody>>& GetRigidbodyVec()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 衝突情報の配列を得る
		@return	衝突情報の配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<CollisionState>& GetCollisionStateVec()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 指定のオーナーのRigidbodyを得る
		@param[in]	OwnerPtr	オーナーのポインタ
		@return	指定のオーナーのRigidbodyのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Rigidbody> GetOwnRigidbody(const shared_ptr<GameObject>& OwnerPtr);
	};

	//--------------------------------------------------------------------------------------
	///	ゲームオブジェクトの親
	//--------------------------------------------------------------------------------------
	class GameObject : public ObjectInterface, public ShapeInterface {
		weak_ptr<Stage> m_Stage;	//所属ステージ
		set<wstring> m_Tag;	//タグ
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief プロテクトコンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit GameObject(const shared_ptr<Stage>& StagePtr) :
			m_Stage(StagePtr) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~GameObject() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	所属するステージを得る
		@return	所属するステージ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetStage() const {
			auto shptr = m_Stage.lock();
			if (!shptr) {
				throw BaseException(
					L"所属ステージがnullです。",
					L"if (!shptr)",
					L"GameObject::GetStage()"
				);
			}
			return shptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	所属するステージを得る(型チェックあり)
		@return	所属するステージ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		shared_ptr<T> GetStage() const {
			auto StagePtr = GetStage();
			auto TargetPtr = dynamic_pointer_cast<T>(StagePtr);
			if (!TargetPtr) {
				throw BaseException(
					L"ステージがありましたが、型キャストできません",
					Util::GetWSTypeName<T>(),
					L"GameObject::GetStage<T>()"
				);
			}
			return TargetPtr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	タグのセットを得る
		@return	タグのセット
		*/
		//--------------------------------------------------------------------------------------
		const set<wstring>& GetTagSet()const {
			return m_Tag;
		}
		set<wstring>& GetTagSet() {
			return m_Tag;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定するタグが存在するかどうかを得る
		@param[in]	tagstr	検証するタグ
		@return	存在すればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool FindTag(const wstring& tagstr) const {
			if (m_Tag.find(tagstr) == m_Tag.end()) {
				return false;
			}
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定するタグを追加する
		@param[in]	tagstr	追加するタグ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void  AddTag(const wstring& tagstr) {
			if (tagstr == L"") {
				//空白なら例外
				throw BaseException(
					L"設定するタグが空です",
					L"if (tagstr == L"")",
					L"GameObject::AddTag()"
				);
			}
			m_Tag.insert(tagstr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定するタグが存在したら削除する（存在しない場合は何もしない）
		@param[in]	tagstr	削除するタグ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void  RemoveTag(const wstring& tagstr) {
			m_Tag.erase(tagstr);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	オブジェクトの位置を得る（仮想関数）
		@return	オブジェクトの位置
		*/
		//--------------------------------------------------------------------------------------
		virtual Vec3 GetPosition() {
			return Vec3(0, 0, 0);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	シャドウマップの描画処理(仮想関数)
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDrawShadowmap() {}
	};


	//--------------------------------------------------------------------------------------
	///	ゲームシーン
	//--------------------------------------------------------------------------------------
	class Scene : public SceneInterface {
		//BGM
		shared_ptr<SoundItem> m_BGM;
		//アクティブなステージ
		shared_ptr<Stage> m_ActiveStage;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	アクティブなステージを設定して初期化する
		@tparam	T	取得する型（Stageに型変換できるもの）
		@tparam	Ts	可変長変数の型
		@param[in]	params	このステージを構築するのに使用するパラメータ。
		@return	ステージ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		shared_ptr<T> ResetActiveStage(Ts&&... params) {
			auto Ptr = ObjectFactory::Create<T>(params...);
			auto StagePtr = dynamic_pointer_cast<Stage>(Ptr);
			if (!StagePtr) {
				throw BaseException(
					L"以下はStageに型キャストできません。",
					Util::GetWSTypeName<T>(),
					L"Scene::ResetActiveStage<T>()"
				);
			}
			m_ActiveStage = StagePtr;
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	リソースの作成
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CreateResources();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Scene();
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Scene() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief アクティブなステージの取得
		@return	アクティブなステージ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Stage> GetActiveStage() const {
			if (!m_ActiveStage) {
				throw BaseException(
					L"アクティブなステージはありません",
					L"f (!m_ActiveStage)",
					L"Scene::GetActiveStage()"
				);
			}
			return m_ActiveStage;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief イベント取得
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 削除時
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDestroy() override;
	};


}
// end basecross
