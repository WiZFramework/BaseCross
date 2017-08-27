/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	//class MultiSpark : public MultiParticle;
	//用途: 複数のスパーククラス
	//--------------------------------------------------------------------------------------
	class MultiSpark : public MultiParticle {
	public:
		//構築と破棄
		MultiSpark(shared_ptr<Stage>& StagePtr);
		virtual ~MultiSpark();
		//初期化
		virtual void OnCreate() override;
		void InsertSpark(const Vec3& Pos);
	};

	//--------------------------------------------------------------------------------------
	//class MultiFire : public MultiParticle;
	//用途: 複数の炎クラス
	//--------------------------------------------------------------------------------------
	class MultiFire : public MultiParticle {
	public:
		//構築と破棄
		MultiFire(shared_ptr<Stage>& StagePtr);
		virtual ~MultiFire();
		//初期化
		virtual void OnCreate() override;
		void InsertFire(const Vec3& Pos);
	};


	//--------------------------------------------------------------------------------------
	///	半透明のスプライト
	//--------------------------------------------------------------------------------------
	class TraceSprite : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec3 m_StartPos;
		float m_TotalTime;
		//バックアップ頂点データ
		vector<VertexPositionColor> m_BackupVertices;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartScale	初期スケール
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		TraceSprite(const shared_ptr<Stage>& StagePtr, bool Trace,
			const Vec2& StartScale, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TraceSprite();
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
	///	壁模様のスプライト
	//--------------------------------------------------------------------------------------
	class WallSprite : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec2 m_StartPos;
		wstring m_TextureKey;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	TextureKey	テクスチャキー
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartScale	初期スケール
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		WallSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale, const Vec2& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~WallSprite();
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
		virtual void OnUpdate()override {}
	};



	//--------------------------------------------------------------------------------------
	///	スクロールするスプライト
	//--------------------------------------------------------------------------------------
	class ScrollSprite : public GameObject {
		bool m_Trace;
		Vec2 m_StartScale;
		Vec2 m_StartPos;
		wstring m_TextureKey;
		float m_TotalTime;
		//バックアップ頂点データ
		vector<VertexPositionTexture> m_BackupVertices;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	TextureKey	テクスチャキー
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartScale	初期スケール
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		ScrollSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec2& StartScale,const Vec2& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ScrollSprite();
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
	///	左上で回転する立方体
	//--------------------------------------------------------------------------------------
	class RollingCube : public GameObject {
		bool m_Trace;
		Vec3 m_StartScale;
		Quat m_StartQt;
		Vec3 m_StartPos;
		float m_TotalTime;
		//バックアップ頂点データ
		vector<VertexPositionColor> m_BackupVertices;
		//このオブジェクトを描画するカメラ
		shared_ptr<Camera> m_Camera;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartScale	初期スケール
		@param[in]	StartQt	初期回転
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		RollingCube(const shared_ptr<Stage>& StagePtr,bool Trace,
			const Vec3& StartScale, const Quat& StartQt,const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RollingCube();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	このオブジェクトを描画するためのカメラを得る（デフォルトはステージのビューから取得）
		@return	カメラのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		virtual const shared_ptr<Camera>& OnGetDrawCamera() const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	};

	//--------------------------------------------------------------------------------------
	///	左上で回転するWall立方体
	//--------------------------------------------------------------------------------------
	class RollingWallCube : public GameObject {
		wstring m_TextureKey;
		bool m_Trace;
		Vec3 m_StartScale;
		Quat m_StartQt;
		Vec3 m_StartPos;
		float m_TotalTime;
		shared_ptr<MeshResource> m_MeshResource;

		//このオブジェクトを描画するカメラ
		shared_ptr<Camera> m_Camera;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	TextureKey	テクスチャキー
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartScale	初期スケール
		@param[in]	StartQt	初期回転
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		RollingWallCube(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
			const Vec3& StartScale, const Quat& StartQt, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RollingWallCube();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	このオブジェクトを描画するためのカメラを得る（デフォルトはステージのビューから取得）
		@return	カメラのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		virtual const shared_ptr<Camera>& OnGetDrawCamera() const override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
	};


	//--------------------------------------------------------------------------------------
	///	白い立方体
	//--------------------------------------------------------------------------------------
	class WhiteCube : public GameObject {
		Vec3 m_StartScale;
		Quat m_StartQt;
		Vec3 m_StartPos;
		float m_TotalTime;
		shared_ptr<MeshResource> m_MeshResource;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	StartScale	初期スケール
		@param[in]	StartQt	初期回転
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		WhiteCube(const shared_ptr<Stage>& StagePtr,
			const Vec3& StartScale, const Quat& StartQt, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~WhiteCube() {}
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
	///	形状が変わる球体
	//--------------------------------------------------------------------------------------
	class TransSphere : public GameObject {
		wstring m_TextureKey;
		bool m_Trace;
		Vec3 m_StartScale;
		Quat m_StartQt;
		Vec3 m_StartPos;
		float m_TotalTime;
		//バックアップ頂点データ
		vector<VertexPositionColorTexture> m_BackupVertices;
		//ランタイム用頂点データ（変化させる）
		vector<VertexPositionColorTexture> m_RuntimeVertices;
		//このオブジェクトを描画するカメラ
		shared_ptr<Camera> m_Camera;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	TextureKey	テクスチャキー
		@param[in]	Trace	透明処理するかどうか
		@param[in]	StartScale	初期スケール
		@param[in]	StartQt	初期回転
		@param[in]	StartPos	初期位置
		*/
		//--------------------------------------------------------------------------------------
		TransSphere(const shared_ptr<Stage>& StagePtr,const wstring& TextureKey,bool Trace,
			const Vec3& StartScale, const Quat& StartQt, const Vec3& StartPos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TransSphere();
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
	//	class SeekObject : public GameObject;
	//	用途: 追いかける配置オブジェクト
	//--------------------------------------------------------------------------------------
	class SeekObject : public GameObject {
		//ステートマシーン
		unique_ptr< StateMachine<SeekObject> >  m_StateMachine;
		Vec3 m_StartPos;
		float m_StateChangeSize;
	public:
		//構築と破棄
		SeekObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~SeekObject();
		//初期化
		virtual void OnCreate() override;
		//アクセサ
		const unique_ptr<StateMachine<SeekObject>>& GetStateMachine() {
			return m_StateMachine;
		}
		float GetStateChangeSize() const {
			return m_StateChangeSize;
		}
		//操作
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;
		//衝突時
		virtual void OnCollision(vector<shared_ptr<GameObject>>& OtherVec) override;
	};

	//--------------------------------------------------------------------------------------
	//	class FarState : public ObjState<SeekObject>;
	//	用途: プレイヤーから遠いときの移動
	//--------------------------------------------------------------------------------------
	class FarState : public ObjState<SeekObject>
	{
		FarState() {}
	public:
		static shared_ptr<FarState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class NearState : public ObjState<SeekObject>;
	//	用途: プレイヤーから近いときの移動
	//--------------------------------------------------------------------------------------
	class NearState : public ObjState<SeekObject>
	{
		NearState() {}
	public:
		static shared_ptr<NearState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	//	数字のスクエア
	//--------------------------------------------------------------------------------------
	class NumberSquare : public GameObject {
		weak_ptr<SeekObject> m_SeekObject;
		//このオブジェクトのみで使用するスクエアメッシュ
		shared_ptr<MeshResource> m_SquareMeshResource;
		//背番号
		size_t m_Number;

		Quat Billboard(const Vec3& Line) {
			Vec3 Temp = Line;
			Mat4x4 RotMatrix;
			Vec3 DefUp(0, 1.0f, 0);
			Vec2 TempVec2(Temp.x, Temp.z);
			if (TempVec2.length() < 0.1f) {
				DefUp = Vec3(0, 0, 1.0f);
			}
			Temp.normalize();
			RotMatrix = XMMatrixLookAtLH(Vec3(0, 0, 0), Temp, DefUp);
			RotMatrix.inverse();
			Quat Qt;
			Qt = RotMatrix.quatInMatrix();
			Qt.normalize();
			return Qt;
		}

	public:
		//構築と破棄
		NumberSquare(const shared_ptr<Stage>& StagePtr, 
			const shared_ptr<SeekObject>& SeekObjectPtr, size_t Number);
		virtual ~NumberSquare();
		//初期化
		virtual void OnCreate() override;
		//変化
		virtual void OnUpdate() override;
	};





	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//構築と破棄
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~FixedBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	用途: 上下移動するボックス
	//--------------------------------------------------------------------------------------
	class MoveBox : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//構築と破棄
		MoveBox(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position
		);
		virtual ~MoveBox();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	class SphereObject : public GameObject;
	//	用途: 障害物球
	//--------------------------------------------------------------------------------------
	class SphereObject : public GameObject {
		Vec3 m_StartPos;
	public:
		//構築と破棄
		SphereObject(const shared_ptr<Stage>& StagePtr, const Vec3& StartPos);
		virtual ~SphereObject();
		//初期化
		virtual void OnCreate() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	class UnevenGroundData : public GameObject;
	//	用途: でこぼこ床のデータ
	//--------------------------------------------------------------------------------------
	class UnevenGroundData : public GameObject {
		vector<TRIANGLE> m_Triangles;
	public:
		//構築と破棄
		UnevenGroundData(const shared_ptr<Stage>& StagePtr);
		virtual ~UnevenGroundData();
		//初期化
		virtual void OnCreate() override;
		//三角形の配列を返す
		const vector<TRIANGLE>& GetTriangles() const {
			return m_Triangles;
		}
	};



	//--------------------------------------------------------------------------------------
	//	class UnevenGround : public GameObject;
	//	用途: でこぼこ床
	//--------------------------------------------------------------------------------------
	class UnevenGround : public GameObject {
		Vec3 m_Scale;
		Vec3 m_Rotation;
		Vec3 m_Position;
	public:
		//構築と破棄
		UnevenGround(const shared_ptr<Stage>& StagePtr,
			const Vec3& Scale,
			const Vec3& Rotation,
			const Vec3& Position);
		virtual ~UnevenGround();
		//初期化
		virtual void OnCreate() override;
		//操作
	};


}
//end basecross
