/*!
@file ProjectBehavior.h
@brief プロジェク定義の行動クラス
*/

#pragma once
#include "stdafx.h"

namespace basecross {


	//--------------------------------------------------------------------------------------
	///頂点を変更する行動クラス
	//--------------------------------------------------------------------------------------
	class VertexBehavior : public Behavior {
		float m_TotalTime;

	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	このコンポーネントを所持するゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		VertexBehavior(const shared_ptr<GameObject>& GameObjectPtr) :
			Behavior(GameObjectPtr),
			m_TotalTime(0)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~VertexBehavior() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 伸び縮みする
		@tparam	T	頂点の型
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		void ExpandAndContract() {
			float ElapsedTime = App::GetApp()->GetElapsedTime();
			m_TotalTime += ElapsedTime;
			if (m_TotalTime >= XM_2PI) {
				m_TotalTime = 0;
			}
			auto PtrDraw = GetGameObject()->GetDynamicComponent<StaticBaseDraw>();
			const vector<T>& BackupVec = PtrDraw->GetOriginalMeshResource()->GetBackupVerteces<T>();
			vector<T> new_vec;
			for (auto& v : BackupVec) {
				T new_v;
				new_v = v;
				auto Len = (sin(m_TotalTime) * 0.5f) + 1.0f;
				new_v.position.x *= Len;
				new_v.position.z *= Len;
				new_vec.push_back(new_v);
			}
			PtrDraw->UpdateVertices(new_vec);
		}

	};


}

//end basecross

