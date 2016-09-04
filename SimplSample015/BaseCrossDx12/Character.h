/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Scene;
	class GameObject;

	//--------------------------------------------------------------------------------------
	///	平面
	//--------------------------------------------------------------------------------------
	class SquareObject : public GameObject {
		//メッシュ
		shared_ptr<MeshResource> m_SquareMesh;
		wstring m_TextureFileName;		///<テクスチャファイル名
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		Vector3 m_Scale;				///<スケーリング
		Quaternion m_Qt;			///<回転
		Vector3 m_Pos;				///<位置
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	PtrScene	シーンのポインタ
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		SquareObject(const shared_ptr<Scene> PtrScene,
			const wstring& TextureFileName, const Vector3& Scale, const Quaternion& Qt, const Vector3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SquareObject();
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
	};

	//--------------------------------------------------------------------------------------
	///	立方体
	//--------------------------------------------------------------------------------------
	class CubeObject : public GameObject {
		//メッシュ
		shared_ptr<MeshResource> m_CubeMesh;
		wstring m_TextureFileName;		///<テクスチャファイル名
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		Vector3 m_Scale;				///<スケーリング
		Quaternion m_Qt;			///<回転
		Vector3 m_Pos;				///<位置
		bool m_Trace;					///<透明処理するかどうか
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	PtrScene	シーンのポインタ
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		CubeObject(const shared_ptr<Scene> PtrScene,
			const wstring& TextureFileName, const Vector3& Scale, const Quaternion& Qt, const Vector3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CubeObject();
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
		@brief OBBを得る
		@return	立方体のOBB
		*/
		//--------------------------------------------------------------------------------------
		OBB GetObb()const;

	};


	enum class SphereMotion {
		RightMotion,
		LeftMotion
	};


	//--------------------------------------------------------------------------------------
	///	球体
	//--------------------------------------------------------------------------------------
	class SphereObject : public GameObject {
		shared_ptr< StateMachine<SphereObject> >  m_StateMachine;	//ステートマシーン
		//メッシュ
		shared_ptr<MeshResource> m_SphereMesh;
		UINT m_Division;				///<分割数
		wstring m_TextureFileName;		///<テクスチャファイル名
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		Vector3 m_Scale;				///<スケーリング
		Quaternion m_Qt;			///<回転
		Vector3 m_Pos;				///<位置
		bool m_Trace;					///<透明処理するかどうか
		float m_MoveXDir;	//X移動方向
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	PtrScene	シーンのポインタ
		@param[in]	Division	分割数
		@param[in]	TextureFileName	テクスチャファイル名
		@param[in]	Trace	透明処理するかどうか
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		SphereObject(const shared_ptr<Scene> PtrScene,
			UINT Division, const wstring& TextureFileName, bool Trace, const Vector3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SphereObject();

		//--------------------------------------------------------------------------------------
		/*!
		@brief ステートマシンの取得
		@return	ステートマシン
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr< StateMachine<SphereObject> > GetStateMachine() const {
			return m_StateMachine;
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
		@brief アクションを実行する
		@param[in]	motion	アクション
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetActionMotion(SphereMotion motion);

		//--------------------------------------------------------------------------------------
		/*!
		@brief 現在のアクションが終了したかどうか
		@return	終了していればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IstActionArrived();


	};


	//--------------------------------------------------------------------------------------
	//	class RightState : public ObjState<SphereObject>;
	//	用途: 右向き移動
	//--------------------------------------------------------------------------------------
	class RightState : public ObjState<SphereObject>
	{
		RightState() {}
	public:
		//ステートのインスタンス取得
		static shared_ptr<RightState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<SphereObject>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<SphereObject>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<SphereObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class LeftState : public ObjState<SphereObject>;
	//	用途: 左向き移動
	//--------------------------------------------------------------------------------------
	class LeftState : public ObjState<SphereObject>
	{
		LeftState() {}
	public:
		//ステートのインスタンス取得
		static shared_ptr<LeftState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<SphereObject>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<SphereObject>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<SphereObject>& Obj)override;
	};


}
//end basecross
