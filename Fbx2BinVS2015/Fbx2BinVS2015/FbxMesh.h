/*!
@file FbxMesh.h
@brief FbxMesh関連
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	// Bone構造体.
	//--------------------------------------------------------------------------------------
	struct	Bone
	{
		//!基本ポーズへの行列
		Matrix4X4	m_BindPose;
		//!現在の行列
		Matrix4X4	m_CurrentPose;
		//!計算された現在の行列
		Matrix4X4	m_ConbinedPose;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf コンストラクタ.
		*/
		//--------------------------------------------------------------------------------------
		Bone()
		{
			::ZeroMemory(this, sizeof(Bone));
		}
	};



	struct FbxMeshObjData {
		wstring m_DataDir;
		wstring m_FbxFileName;
		size_t m_MeshIndex;
		float m_Scale;
	};

	struct FbxMeshObjSaveData {
		wstring m_DataDir;
		wstring m_BinFileName;
		wstring m_Header;
		float m_Scale;
		UINT m_StartFrame;
		UINT m_FrameCount;
		float m_FrameParSecond;
		float m_SampleSpan;

		FbxMeshObjSaveData() :
			m_Scale(1.0f),
			m_StartFrame(0),
			m_FrameCount(1),
			m_FrameParSecond(30.0f),
			m_SampleSpan(1.0f / 30.0f)
		{}
	};

	struct SaveMaterialEx {
		//!開始インデックス
		UINT m_StartIndex;
		//!描画インデックスカウント
		UINT m_IndexCount;
		//! デフィーズ（物体の色）
		Color4 m_Diffuse;
		//! スペキュラー（反射光）
		Color4 m_Specular;
		//! アンビエント（環境色）
		Color4 m_Ambient;
		//! エミッシブ（放射光）
		Color4 m_Emissive;
	};

	class FbxSceneResource;

	//--------------------------------------------------------------------------------------
	///	FBXメッシュリソース(MeshResource派生版)
	//--------------------------------------------------------------------------------------
	class FbxMeshResource2 : public MeshResource {
		friend class ObjectFactory;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf スタティックFBXからのインスタンスの構築.
		@return	なし（例外がthrowされる）
		*/
		//--------------------------------------------------------------------------------------
		void CreateInstanceFromStaticFbx();
		//--------------------------------------------------------------------------------------
		/*!
		@breaf スキンメッシュFBXからのインスタンスの構築.
		@return	なし（例外がthrowされる）
		*/
		//--------------------------------------------------------------------------------------
		void CreateInstanceFromSkinFbx();
		//--------------------------------------------------------------------------------------
		/*!
		@breaf マテリアルを取り出す
		@param[out] materials	マテリアル配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetMaterialVec(vector<MaterialEx>& materials);
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf プロテクトコンストラクタ.<br/>
		FBXメッシュからデータを読み出す
		@param FbxSceneResourcePtr	FBXシーンリソース
		@param pFbxMesh	FBXメッシュリソース
		@param NeedStatic	スキンメッシュでもスタティックメッシュにするかどうか
		@param WithTangent	タンジェントを読むかどうか
		*/
		//--------------------------------------------------------------------------------------
		FbxMeshResource2(shared_ptr<FbxSceneResource> FbxSceneResourcePtr, FbxMesh* pFbxMesh, bool NeedStatic, bool WithTangent);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 頂点とインデックスとマテリアルを取り出す
		@param[out] vertices	頂点配列
		@param[out] indices	インデックス配列
		@param[out] materials	マテリアル配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetStaticVerticesIndicesMaterials(vector<VertexPositionNormalTexture>& vertices, 
			vector<uint16_t>& indices,vector<MaterialEx>& materials);
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 頂点とインデックスとマテリアルを取り出す(タンジェント付き)
		@param[out] vertices	頂点配列
		@param[out] indices	インデックス配列
		@param[out] materials	マテリアル配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetStaticVerticesIndicesMaterialsWithTangent(vector<VertexPositionNormalTangentTexture>& vertices,
			vector<uint16_t>& indices, vector<MaterialEx>& materials);

		//--------------------------------------------------------------------------------------
		/*!
		@breaf 頂点とインデックスとマテリアルを取り出す
		@param[out] vertices	頂点配列
		@param[out] indices	インデックス配列
		@param[out] materials	マテリアル配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetSkinVerticesIndicesMaterials(vector<VertexPositionNormalTextureSkinning>& vertices, 
			vector<uint16_t>& indices, vector<MaterialEx>& materials,
			vector<Bone>& Bones, map< string, UINT >& mapBoneList);

		//--------------------------------------------------------------------------------------
		/*!
		@breaf 頂点とインデックスとマテリアルを取り出す(タンジェント付き)
		@param[out] vertices	頂点配列
		@param[out] indices	インデックス配列
		@param[out] materials	マテリアル配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetSkinVerticesIndicesMaterialsWithTangent(vector<VertexPositionNormalTangentTextureSkinning>& vertices,
			vector<uint16_t>& indices, vector<MaterialEx>& materials,
			vector<Bone>& Bones, map< string, UINT >& mapBoneList);


		//--------------------------------------------------------------------------------------
		/*!
		@breaf スタティック呼び出し<br/>
		FBXメッシュからデータを読み出す
		@param FbxSceneResourcePtr	FBXシーンリソース
		@param pFbxMesh	FBXメッシュリソース
		@param NeedStatic	スキンメッシュでもスタティックメッシュにするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<FbxMeshResource2> CreateFbxMeshResource(shared_ptr<FbxSceneResource> FbxSceneResourcePtr, FbxMesh* pFbxMesh, bool NeedStatic = false,
			bool WithTangent = false);
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 初期化
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override;
		//--------------------------------------------------------------------------------------
		/// FBXメッシュのデリーター関数オブジェクト.
		//--------------------------------------------------------------------------------------
		struct FbxMeshDeleter
		{
			void operator()(FbxMesh *p) {
				//削除はマネージャが行うので何もしない
			}
		};
		//--------------------------------------------------------------------------------------
		/// FBXスキンのデリーター関数オブジェクト.
		//--------------------------------------------------------------------------------------
		struct FbxSkinDeleter
		{
			void operator()(FbxSkin *p) {
				//削除はマネージャが行うので何もしない
			}
		};
		//--------------------------------------------------------------------------------------
		/*!
		@breaf デストラクタ.
		*/
		//--------------------------------------------------------------------------------------
		virtual ~FbxMeshResource2();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	スキニングするかどうか.<br />
		仮想関数なので、派生クラスで独自に多重定義する
		@return	スキニングする場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		virtual bool IsSkining() const override;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf FBXメッシュの取得
		@return	FBXメッシュのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		const unique_ptr<FbxMesh, FbxMeshDeleter>& GetFbxMesh() const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf FBXスキンの取得
		@return	FBXスキンのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		const unique_ptr<FbxSkin, FbxSkinDeleter>& GetFbxSkin() const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ボーンの配列を得る
		@return	ボーンの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector< Bone >& GetBonesVec() const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ボーン数の取得
		@return	ボーン数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumBones() const {
			return GetBonesVec().size();
		}
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 指定のアニメーションの設定されている時間における、カレントポーズと合成変換行列を再計算する
		@param　tgtBones	変更すべきボーン配列
		@param　rAnimationData	指定のアニメーションデータ
		@param　CurrentTime	時間指定
		@return	ループなしで最後に到達したらtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GenerateCurrentPose(vector<Bone>& tgtBones, AnimationData& rAnimationData, float CurrentTime);

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	FBXシーンリソース
	//--------------------------------------------------------------------------------------
	class FbxSceneResource : public BaseResource {
		friend class ObjectFactory;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf FBXメッシュ配列を再帰的に読み込む
		@param　pFbxNode	ターゲットとなるノード
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CreateFbxMeshVector(FbxNode* pFbxNode);
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@breaf プロテクトコンストラクタ.<br/>
		FBXファイルからFBXシーンを構築する
		@param DataDir	データディレクトリ
		@param FileName	FBXファイル名
		@param SceneName	シーン名
		@param NeedStatic	static構築をするかどうか
		@param WithTangent	タンジェントを読み込むかどうか
		*/
		//--------------------------------------------------------------------------------------
		FbxSceneResource(const wstring& DataDir,
			const wstring& FileName, const string& SceneName, bool NeedStatic,bool WithTangent);
		//初期化
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 初期化
		@param　なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate()override;
	public:
		//--------------------------------------------------------------------------------------
		/// FBXシーンのデリーター関数オブジェクト.
		//--------------------------------------------------------------------------------------
		struct FbxSceneDeleter
		{
			//--------------------------------------------------------------------------------------
			/*!
			@breaf FBXシーンのdelete.<br />
			削除はマネージャが行うので何もしない
			@param FbxScene *p	FBXシーンのポインタ
			@return	なし
			*/
			//--------------------------------------------------------------------------------------
			void operator()(FbxScene *p) {
				//削除はマネージャが行うので何もしない
			}
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~FbxSceneResource();
		//--------------------------------------------------------------------------------------
		/*!
		@breaf 構築用static関数.
		@param DataDir	データディレクトリ
		@param FileName		ファイル名
		@param SceneName	シーン名
		@param NeedStati	staticメッシュとして構築するかどうか
		@param WithTangent	タンジェントを読み込むかどうか
		@return　FBXシーンリソース
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<FbxSceneResource> CreateFbxScene(const wstring& DataDir,
			const wstring& FileName, const string& SceneName = "", bool NeedStatic = false,bool WithTangent = false);
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@breaf データディレクトリを得る
		@param　なし
		@return	データディレクトリ
		*/
		//--------------------------------------------------------------------------------------
		wstring GetDataDir() const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf ファイル名を得る
		@param　なし
		@return	ファイル名
		*/
		//--------------------------------------------------------------------------------------
		wstring GetFileName() const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf FBXシーン名を得る
		@param　なし
		@return	FBXシーン名
		*/
		//--------------------------------------------------------------------------------------
		string GetFbxSceneName() const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf メッシュリソースを得る
		@param　Index	FBX内のメッシュID
		@return	FBXメッシュリソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<FbxMeshResource2> GetFbxMeshResource(size_t Index) const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf メッシュリソース数を得る
		@param　なし
		@return	FBXシーンに含まれるメッシュリソース数
		*/
		//--------------------------------------------------------------------------------------
		size_t GetFbxMeshResourceSize() const;
		//--------------------------------------------------------------------------------------
		/*!
		@breaf メッシュリソース配列を得る
		@param　なし
		@return	FBXシーンに含まれるメッシュリソース配列
		*/
		//--------------------------------------------------------------------------------------
		vector< shared_ptr<FbxMeshResource2> >& GetFbxMeshResourceVec() const;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	BcFbxPNTBoneModelDraw描画コンポーネント（ボーンモデル描画用）
	//--------------------------------------------------------------------------------------
	class BcFbxPNTBoneModelDraw : public BcPNTStaticModelDraw {
	protected:
		//ボーン行列をリソースから読み込む
		void SetBoneVec();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit BcFbxPNTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcFbxPNTBoneModelDraw();
		//アクセサ
		//ボーン行列をリソースから読み込む
		void GetBoneVec(vector< Bone >& Bones);
		//各オブジェクトごとにボーンを所持しておくポインタ
		const vector< Bone >& GetVecLocalFbxBones() const;
		const vector< Bone >* GetVecLocalFbxBonesPtr() const;

		virtual void SetMeshResource(const shared_ptr<MeshResource>& MeshRes)override;
		void SetMeshResource(const wstring& MeshKey);

		//--------------------------------------------------------------------------------------
		/*!
		@breaf アニメーションを設定する（すでにその定義があれば、差し替える）
		@param　Name	アニメーション名
		@param　StartFrame,	スタート位置
		@param　FrameLength	長さ
		@param　Loop,			ループするかどうか
		@param　FramesParSecond １秒あたりのフレーム数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddAnimation(const char* Name, UINT StartFrame, UINT FrameLength, bool Loop,
			float FramesParSecond = 30.0f);

		//--------------------------------------------------------------------------------------
		/*!
		@breaf アニメーションデータを得る。無ければ例外
		@return	アニメーションデータ
		*/
		//--------------------------------------------------------------------------------------
		const AnimationData& GetAnimationData(const string& AnimeName) const;

		//カレントアニメーション
		const string& GetCurrentAnimation() const;
		void SetCurrentAnimation(const string& AnemationName, float StartTime = 0.0f);
		//現在のアニメーションが終了しているかどうか
		bool IsTargetAnimeEnd() const;

		//指定したIDのボーンの現在の行列を取得する
		void GetBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const;

		//指定したIDのボーンの現在のローカル行列を取得する（親子関係を構築するなど用）
		void GetLocalBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	ローカルボーン行列配列を得る
		@return	ローカルボーン行列配列の先頭ポインタ
		*/
		//--------------------------------------------------------------------------------------
		virtual const vector< Matrix4X4 >* GetVecLocalBonesPtr() const;

		float GetCurrentTime() const;


		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のアニメーションを進める
		@param[in]	ElapsedTime	経過時間
		@return	アニメーションが終了すればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool UpdateAnimation(float ElapsedTime);
		//操作
		virtual void OnCreate()override;
		virtual void OnDraw()override;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	///	BcFbxPNTnTBoneModelDraw描画コンポーネント（ボーンモデル描画用）タンジェント付き
	//--------------------------------------------------------------------------------------
	class BcFbxPNTnTBoneModelDraw : public BcPNTnTStaticModelDraw {
	protected:
		//ボーン行列をリソースから読み込む
		void SetBoneVec();
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit BcFbxPNTnTBoneModelDraw(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BcFbxPNTnTBoneModelDraw();
		//アクセサ
		//ボーン行列をリソースから読み込む
		void GetBoneVec(vector< Bone >& Bones);
		//各オブジェクトごとにボーンを所持しておくポインタ
		const vector< Bone >& GetVecLocalFbxBones() const;
		const vector< Bone >* GetVecLocalFbxBonesPtr() const;

		virtual void SetMeshResource(const shared_ptr<MeshResource>& MeshRes)override;
		void SetMeshResource(const wstring& MeshKey);

		//--------------------------------------------------------------------------------------
		/*!
		@breaf アニメーションを設定する（すでにその定義があれば、差し替える）
		@param　Name	アニメーション名
		@param　StartFrame,	スタート位置
		@param　FrameLength	長さ
		@param　Loop,			ループするかどうか
		@param　FramesParSecond １秒あたりのフレーム数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddAnimation(const char* Name, UINT StartFrame, UINT FrameLength, bool Loop,
			float FramesParSecond = 30.0f);

		//--------------------------------------------------------------------------------------
		/*!
		@breaf アニメーションデータを得る。無ければ例外
		@return	アニメーションデータ
		*/
		//--------------------------------------------------------------------------------------
		const AnimationData& GetAnimationData(const string& AnimeName) const;

		//カレントアニメーション
		const string& GetCurrentAnimation() const;
		void SetCurrentAnimation(const string& AnemationName, float StartTime = 0.0f);
		//現在のアニメーションが終了しているかどうか
		bool IsTargetAnimeEnd() const;

		//指定したIDのボーンの現在の行列を取得する
		void GetBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const;

		//指定したIDのボーンの現在のローカル行列を取得する（親子関係を構築するなど用）
		void GetLocalBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const;

		//--------------------------------------------------------------------------------------
		/*!
		@brief	ローカルボーン行列配列を得る
		@return	ローカルボーン行列配列の先頭ポインタ
		*/
		//--------------------------------------------------------------------------------------
		virtual const vector< Matrix4X4 >* GetVecLocalBonesPtr() const;

		float GetCurrentTime() const;


		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のアニメーションを進める
		@param[in]	ElapsedTime	経過時間
		@return	アニメーションが終了すればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool UpdateAnimation(float ElapsedTime);
		//操作
		virtual void OnCreate()override;
		virtual void OnDraw()override;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class FbxMeshObject : public GameObject;
	//	用途: FBXメッシュのオブジェクト
	//--------------------------------------------------------------------------------------
	class FbxMeshObject : public GameObject {
		wstring m_DataDir;
		wstring m_FbxFileName;
		wstring m_FbxResName;
		size_t m_MeshIndex;
		wstring m_FbxMeshResName;

		float m_CharaLocalScale;
		Vector3 m_CharaLocalPosition;
		bool m_IsReadStaticMesh;
		bool m_WithTangent;

		//アニメーション実行中かどうか
		bool m_IsAnimeRun;

	public:
		FbxMeshObject(const shared_ptr<Stage>& StagePtr);
		virtual ~FbxMeshObject() {}
		void ResetFbxMesh(const wstring& DirName, const wstring& FbxName, size_t MeshIndex, float Scale, const Vector3& Position,
			bool IsReadStatic, bool WithTangent, const wstring& NormalFileName);

		void ClearFbxMesh();


		bool CheckSkinMesh();
		bool CheckMesh();


		void MoveFbxMesh(UINT FrameRate,UINT StartTime,UINT EndTime,bool IsLoop);

		void AnimePoseStart();

		void SaveStaticBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale);
		void SaveSkinBinFile(const wstring& Dir, const wstring& FileName, size_t MeshIndex, float Scale,
			UINT FrameParSec,UINT Start,UINT End);


		//初期化
		virtual void OnCreate() override;
		virtual void OnUpdate() override;
		virtual void OnUpdate2() override;


	};


}
//end basecross
