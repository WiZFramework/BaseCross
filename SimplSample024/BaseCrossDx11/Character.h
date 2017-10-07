/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	class Scene;
	class Stage;
	class GameObject;

	//--------------------------------------------------------------------------------------
	///	平面
	//--------------------------------------------------------------------------------------
	class SquareObject : public GameObject {
		//メッシュ
		shared_ptr<MeshResource> m_SquareMesh;
		wstring m_TextureResName;		///<テクスチャリソース名
		Vec3 m_Scale;				///<スケーリング
		Quat m_Qt;			///<回転
		Vec3 m_Pos;				///<位置
		//--------------------------------------------------------------------------------------
		/*!
		@brief 頂点の作成
		@param[in]	WrapX	X方向のタイリング
		@param[in]	WrapY	Y方向のタイリング
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CreateBuffers(float WrapX, float WrapY);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	TextureResName	テクスチャリソース名
		@param[in]	Scale	スケーリング
		@param[in]	Qt	初期回転
		@param[in]	Pos	位置
		*/
		//--------------------------------------------------------------------------------------
		SquareObject(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, const Vec3& Scale, const Quat& Qt, const Vec3& Pos);
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


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
	///	ラッピング処理されたスプライト（親）
	//--------------------------------------------------------------------------------------
	class SpriteBase : public GameObject {
	public:
		//ブレンド方法
		enum class BlendState {
			Opaque,
			Trace,
			Additive
		};
	protected:
		//メッシュ
		shared_ptr<MeshResource> m_SquareMesh;
		//バックアップしておく頂点データ
		vector<VertexPositionColorTexture> m_BackupVertices;
		wstring m_TextureResName;		///<テクスチャリソース名
		Vec2 m_Scale;				///<スケーリング
		float m_Rot;				///<回転
		Vec2 m_Pos;				///<位置
		UINT m_XWrap;		//X方向のラップ数
		UINT m_YWrap;		//Y方向のラップ数
		Col4 m_Emissive;	//エミッシブ色
		BlendState m_BlendState;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	TextureResName	初期テクスチャリソース名
		@param[in]	StartScale	初期大きさ
		@param[in]	StartRot	初期回転
		@param[in]	StartPos	初期位置
		@param[in]	XWrap	X方向のラップ数
		@param[in]	YWrap	Y方向のラップ数
		*/
		//--------------------------------------------------------------------------------------
		SpriteBase(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName,
			const Vec2& StartScale,
			float StartRot,
			const Vec2& StartPos,
			UINT XWrap,UINT YWrap);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SpriteBase() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief バックアップ頂点の修正(仮想関数)。派生クラスは独自の頂点初期修正を実装
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AdjustVertex() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 頂点の変更(仮想関数)。派生クラスは独自の頂点変更を実装
		@param[in]	ElapsedTime	ターン時間
		@param[out]	vertices	マップされた頂点データ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateVertex(float ElapsedTime,VertexPositionColorTexture* vertices) {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャリソース名の取得
		@return	テクスチャリソース名
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetTextureResName() const {
			return m_TextureResName;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャリソース名の変更
		@param[in]	col	リソース名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResName(const wstring& textureResName) {
			m_TextureResName = textureResName;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ブレンド方法の取得
		@return	ブレンド方法
		*/
		//--------------------------------------------------------------------------------------
		const BlendState& GetBlendState() const {
			return m_BlendState;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ブレンド方法の変更
		@param[in]	col	変更色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBlendState(BlendState blendState) {
			m_BlendState = blendState;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief エミッシブ色の取得
		@return	エミッシブ色
		*/
		//--------------------------------------------------------------------------------------
		const Col4& GetEmissive() const {
			return m_Emissive;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief エミッシブ色の変更
		@param[in]	col	変更色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetEmissive(const Col4& col) {
			m_Emissive = col;
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
	};


	//--------------------------------------------------------------------------------------
	///	回転するスプライト
	//--------------------------------------------------------------------------------------
	class RotateSprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief バックアップ頂点の修正(仮想関数)。派生クラスは独自の頂点初期修正を実装
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AdjustVertex() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 頂点の変更
		@param[in]	ElapsedTime	ターン時間
		@param[out]	vertices	マップされた頂点データ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	TextureResName	初期テクスチャリソース名
		@param[in]	StartScale	初期大きさ
		@param[in]	StartRot	初期回転
		@param[in]	StartPos	初期位置
		@param[in]	XWrap	X方向のラップ数
		@param[in]	YWrap	Y方向のラップ数
		*/
		//--------------------------------------------------------------------------------------
		RotateSprite(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, 
			const Vec2& StartScale,
			float StartRot,
			const Vec2& StartPos,
			UINT XWrap, UINT YWrap);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~RotateSprite() {}
	};

	//--------------------------------------------------------------------------------------
	///	メッセージを表示するスプライト
	//--------------------------------------------------------------------------------------
	class MessageSprite : public SpriteBase {
		float m_TotalTime;	//頂点変更に使用するタイム
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief バックアップ頂点の修正(仮想関数)。派生クラスは独自の頂点初期修正を実装
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void AdjustVertex() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 頂点の変更
		@param[in]	ElapsedTime	ターン時間
		@param[out]	vertices	マップされた頂点データ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void UpdateVertex(float ElapsedTime, VertexPositionColorTexture* vertices) override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		@param[in]	TextureResName	初期テクスチャリソース名
		@param[in]	StartScale	初期大きさ
		@param[in]	StartRot	初期回転
		@param[in]	StartPos	初期位置
		@param[in]	XWrap	X方向のラップ数
		@param[in]	YWrap	Y方向のラップ数
		*/
		//--------------------------------------------------------------------------------------
		MessageSprite(const shared_ptr<Stage>& StagePtr,
			const wstring& TextureResName, 
			const Vec2& StartScale,
			float StartRot,
			const Vec2& StartPos,
			UINT XWrap, UINT YWrap);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MessageSprite() {}
	};




}
//end basecross
