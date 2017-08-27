/*!
@file Character.h
@brief キャラクターなど
*/

#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	立方体
	//--------------------------------------------------------------------------------------
	struct CubeObject {
		Vec3 m_Scale;			///<スケーリング
		Quat m_Quaternion;	///<回転
		Vec3 m_QuaternionRot;	///<回転軸
		float m_QuaternionVelocity;	///<回転速度
		Vec3 m_Posision;				///<位置
		Vec3 m_Velocity;			///<速度
		CubeObject() {}
		void Refresh() {
			m_Scale = Vec3(0.1f, 0.1f, 0.1f);
			m_QuaternionRot = Vec3(Util::RandZeroToOne() - 0.5f, 1.0f, Util::RandZeroToOne() - 0.5f);
			m_Quaternion.rotation(m_QuaternionRot, Util::RandZeroToOne());
			m_QuaternionVelocity = (Util::RandZeroToOne() - 0.5f) * 10.0f;
			m_Posision = Vec3(Util::RandZeroToOne() - 0.5f, Util::RandZeroToOne(), Util::RandZeroToOne());
			m_Velocity = Vec3(Util::RandZeroToOne() - 0.5f, Util::RandZeroToOne() - 0.5f, Util::RandZeroToOne() - 0.5f);
			m_Velocity *= 2.0f;
		}
	};


	//--------------------------------------------------------------------------------------
	///	立方体グループ
	//--------------------------------------------------------------------------------------
	class CubeObjectGroup : public ObjectInterface, public ShapeInterface {
		//メッシュ
		shared_ptr<MeshResource> m_CubeMesh;
		wstring m_TextureFileName;		///<テクスチャファイル名
		shared_ptr<TextureResource> m_TextureResource;	///<テクスチャリソース
		void CreateBuffers();	///<バッファの作成
		const size_t m_MaxInstance;				///<インスタンス最大値
		vector<CubeObject> m_CubeObjectVec;		///<立方体のインスタンス描画配列
		ComPtr<ID3D11Buffer> m_MatrixBuffer;	///<行列用の頂点バッファ
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	TextureFileName	テクスチャファイル名
		*/
		//--------------------------------------------------------------------------------------
		CubeObjectGroup(const wstring& TextureFileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CubeObjectGroup();
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


}
//end basecross
