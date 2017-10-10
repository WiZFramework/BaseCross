/*!
@file DrawObjects.h
@brief 描画オブジェクト
*/

#pragma once
#include "stdafx.h"

namespace basecross {
	class Scene;
	class Stage;
	class GameObject;

	//--------------------------------------------------------------------------------------
	///	PNT頂点描画に使用する構造体
	//--------------------------------------------------------------------------------------
	struct DrawObject {
		shared_ptr<MeshResource> m_MeshRes;
		shared_ptr<TextureResource> m_TextureRes;
		Mat4x4 m_WorldMatrix;
		bool m_Trace;
		bool m_Wrap;
		DrawObject() :
			m_MeshRes(nullptr),
			m_TextureRes(nullptr),
			m_WorldMatrix(),
			m_Trace(false),
			m_Wrap(false)
		{}
	};


	//--------------------------------------------------------------------------------------
	///	PNT頂点オブジェクトの描画クラス
	//--------------------------------------------------------------------------------------
	class PNTDrawObject : public GameObject {
		vector<DrawObject> m_DrawObjectVec;
		vector<DrawObject> m_TraceDrawObjectVec;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画サブ処理
		@param[in]	ObjectVec	描画する配列
		@param[in]	sb	コンスタントバッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void OnDrawSub(vector<DrawObject>& ObjectVec, SimpleConstants& sb);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージのポインタ
		*/
		//--------------------------------------------------------------------------------------
		PNTDrawObject(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PNTDrawObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画するオブジェクトを追加する
		@param[in]	MeshRes	メッシュ
		@param[in]	TextureRes テクスチャ
		@param[in]	WorldMat ワールド行列
		@param[in]	Trace 透明処理するかどうか
		@param[in]	Wrap ラッピング処理するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddDrawMesh(const shared_ptr<MeshResource>& MeshRes,
			const shared_ptr<TextureResource>& TextureRes,
			const Mat4x4& WorldMat,
			bool Trace, bool Wrap = false);
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
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};


	//--------------------------------------------------------------------------------------
	//Particle描画のオプション
	//--------------------------------------------------------------------------------------
	enum class ParticleDrawOption {
		Billboard,	//ビルボード
		Faceing,	//フェイシング
		FaceingY,	//フェイシング(Y軸のみ)
		Normal		//通常(手動。回転は正規化のみ行う)
	};
	//--------------------------------------------------------------------------------------
	//ParticleDraw準備のための構造体
	//--------------------------------------------------------------------------------------
	struct DrawParticleSprite {
		//パーティクルのカメラまでの距離
		float m_ToCaneraLength;
		//ワールド行列
		Mat4x4 m_WorldMatrix;
		//テクスチャ
		shared_ptr<TextureResource> m_TextureRes;
		//Diffuseカラー
		Col4 m_Diffuse;
		DrawParticleSprite() :
			m_ToCaneraLength(0),
			m_Diffuse(1, 1, 1, 1)

		{}
	};

	class ParticleDrawObject;

	//--------------------------------------------------------------------------------------
	///	パーティクル内の、1つのスプライトをあらわす構造体
	//--------------------------------------------------------------------------------------
	struct ParticleSprite {
		bool m_Active;				//アクティブかどうか
		Col4 m_Color;				//色（透けさせる場合はaを0にする）
		Vec2 m_LocalScale;		//ローカルスケーリング（Zは1.0固定）		
		Quat m_LocalQt;		//ローカル回転。手動のときのみ使用
		Vec3 m_LocalPos;			//ローカル位置
		Vec3 m_Velocity;			//速度(必要に応じて使用)
		ParticleSprite() :
			m_Active(true),
			m_Color(1.0f, 1.0f, 1.0f, 1.0f),
			m_LocalScale(1.0f, 1.0f),
			m_LocalQt(),
			m_LocalPos(0, 0, 0),
			m_Velocity(0, 0, 0)
		{}
		void Reflesh() {
			m_Active = true;
			m_Color = Col4(1.0f, 1.0f, 1.0f, 1.0f);
			m_LocalScale = Vec2(1.0f, 1.0f);
			m_LocalQt.identity();
			m_LocalPos = Vec3(0, 0, 0);
			m_Velocity = Vec3(0, 0, 0);
		}
	};

	//--------------------------------------------------------------------------------------
	///	パーティクル
	///	*1つのエミッターを持ち、複数のParticleSpriteを保持する
	//--------------------------------------------------------------------------------------
	struct Particle : public ObjectInterface {
		ParticleDrawOption m_DrawOption;		//表示オプション
		vector<ParticleSprite> m_ParticleSpriteVec;	//保存しておくスプライトの配列
		Vec3 m_EmitterPos;			//エミッター位置
		float m_TotalTime;				//タイマー制御する場合に使用する変数
		float m_MaxTime;				//このパーティクル集合体の表示時間
		weak_ptr<TextureResource> m_TextureResource;	//テクスチャ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	Count	パーティクルの数
		@param[in]	Option	描画オプション
		*/
		//--------------------------------------------------------------------------------------
		explicit Particle(size_t Count, ParticleDrawOption Option = ParticleDrawOption::Billboard);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~Particle();
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief トータルタイムの追加
		@param[in]	f	追加値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddTotalTime(float f) {
			m_TotalTime += f;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief アクティブかどうか
		@return	1つでもアクティブがあればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsActive() const {
			for (auto Psp : m_ParticleSpriteVec) {
				if (Psp.m_Active) {
					//1つでもアクティブがあればtrue
					return true;
				}
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief すべてアクティブかどうか
		@return	すべてアクティブならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsAllActive() const {
			for (auto Psp : m_ParticleSpriteVec) {
				if (!Psp.m_Active) {
					//1つでも非アクティブがあればfalse
					return false;
				}
			}
			return true;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief すべてアクティブにする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAllActive() {
			for (auto Psp : m_ParticleSpriteVec) {
				Psp.m_Active = true;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief すべて非アクティブにする
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAllNoActive() {
			for (auto Psp : m_ParticleSpriteVec) {
				Psp.m_Active = false;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 値を初期化する（同じ数と描画オプション）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Reflesh() {
			m_EmitterPos = Vec3(0, 0, 0);
			m_TotalTime = 0;
			m_MaxTime = 0;
			m_ParticleSpriteVec.clear();
			for (auto Psp : m_ParticleSpriteVec) {
				Psp.Reflesh();
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 値を初期化する
		@param[in]	Count	新しいパーティクルスプライト数
		@param[in]	Option	新しい描画オプション
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Reflesh(size_t Count, ParticleDrawOption Option) {
			m_DrawOption = Option;
			m_EmitterPos = Vec3(0, 0, 0);
			m_TotalTime = 0;
			m_MaxTime = 0;
			m_ParticleSpriteVec.clear();
			m_ParticleSpriteVec.resize(Count);
			for (auto Psp : m_ParticleSpriteVec) {
				Psp.Reflesh();
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief パーティクルスプライトの配列を得る
		@return	パーティクルスプライトの配列
		*/
		//--------------------------------------------------------------------------------------
		vector<ParticleSprite>& GetParticleSpriteVec() {
			return m_ParticleSpriteVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャリソースを得る
		@return	テクスチャリソースのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<TextureResource> GetTextureResource(bool ExceptionActive) const {
			if (!m_TextureResource.expired()) {
				return m_TextureResource.lock();
			}
			else {
				if (ExceptionActive) {
					throw BaseException(
						L"テクスチャリソースが見つかりません",
						L"if (pImpl->m_Texture.expired())",
						L"Particle::GetTextureResource()"
					);
				}
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャリソースを設定する
		@param[in]	ResKey	テクスチャリソース名
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const wstring& ResKey) {
			try {
				if (ResKey == L"") {
					throw BaseException(
						L"テクスチャキーが空白です",
						L"if (ResKey == L\"\"",
						L"Particle::SetTextureResource()"
					);
				}
				m_TextureResource = App::GetApp()->GetResource<TextureResource>(ResKey);
			}
			catch (...) {
				throw;
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief テクスチャリソースを設定する
		@param[in]	TextureResourcePtr	テクスチャリソース
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr) {
			m_TextureResource = TextureResourcePtr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Draw(const shared_ptr<ParticleDrawObject>& Manager);
	};


	//--------------------------------------------------------------------------------------
	///	マルチパーティクル(パーティクルエフェクト)
	//--------------------------------------------------------------------------------------
	class MultiParticle : public GameObject {
		vector< shared_ptr<Particle> > m_ParticleVec;
		//加算処理するかどうか
		bool m_AddType;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		explicit MultiParticle(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MultiParticle();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	パーティクルの配列を得る
		@return	パーティクルの配列
		*/
		//--------------------------------------------------------------------------------------
		vector< shared_ptr<Particle> >& GetParticleVec() {
			return m_ParticleVec;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	加算描画するかどうかを得る
		@return	加算描画するならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GetAddType() const {
			return m_AddType;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	加算描画するかどうかを得る
		@return	加算描画するならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsAddType() const {
			return m_AddType;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	加算描画するかどうかを設定する
		@param[in]	b	加算描画するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAddType(bool b) {
			m_AddType = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief パーティクルの追加
		@param[in]	Count	パーティクルの数
		@param[in]	Option	描画オプション
		@return	追加されたパーティクル
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<Particle> InsertParticle(size_t Count, ParticleDrawOption Option = ParticleDrawOption::Billboard);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate()override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};

	//--------------------------------------------------------------------------------------
	//Instance描画のための構造体
	//--------------------------------------------------------------------------------------
	struct InstanceDrawStr {
		size_t Start;
		size_t Count;
		shared_ptr<TextureResource> Tex;
		Col4 m_Diffuse;
		InstanceDrawStr(size_t s, size_t c, shared_ptr<TextureResource> t,
			const Col4& Diffuse) :
			Start(s), Count(c), Tex(t), m_Diffuse(Diffuse) {}
	};

	//--------------------------------------------------------------------------------------
	///	パーティクル描画オブジェクト
	//--------------------------------------------------------------------------------------
	class ParticleDrawObject : public GameObject {
		//メッシュリソース
		shared_ptr<MeshResource> m_MeshResource;
		//ParticleDraw準備のための構造体の配列
		vector<DrawParticleSprite> m_DrawParticleSpriteVec;
		//インスタンス描画の構造体の配列
		vector<InstanceDrawStr> m_InstancVec;
		//インスタンス最大値
		const size_t m_MaxInstance;
		//行列用のバッファ
		ComPtr<ID3D11Buffer> m_MatrixBuffer;
		//Zバッファを使用するかどうか
		bool m_ZBufferUse;
		//サンプラーのラッピングするかどうか
		bool m_SamplerWrap;
		//加算処理するかどうか
		bool m_AddType;
		//頂点バッファの作成
		void CreateParticleBuffers();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StagePtr	ステージ
		@param[in]	AddType	加算描画するかどうか
		*/
		//--------------------------------------------------------------------------------------
		explicit ParticleDrawObject(const shared_ptr<Stage>& StagePtr, bool AddType);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ParticleDrawObject();
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Zバッファを使用するかどうかを得る
		@return	Zバッファを使用するならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsZBufferUse() const {
			return m_ZBufferUse;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Zバッファを使用するかどうかを設定する
		@param[in]	b	Zバッファを使用するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetZBufferUse(bool b) {
			m_ZBufferUse = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ラッピングサンプラーかどうかを得る
		@returnラッピングサンプラーならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsSamplerWrap() const {
			return m_SamplerWrap;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ラッピングサンプラーを設定する
		@param[in]	b	ラッピングサンプラーにするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetSamplerWrap(bool b) {
			m_SamplerWrap = b;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	描画のためのパーティクルスプライトを追加する
		@param[in]	rParticleSprite	パーティクルスプライト
		@param[in]	Option	描画オプション
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddParticle(const ParticleSprite& rParticleSprite, ParticleDrawOption Option,
			const bsm::Vec3& EmitterPos, const shared_ptr<TextureResource>& TextureRes);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 更新処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 描画処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw()override;
	};



}
//end basecross
