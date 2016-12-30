/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	壁模様のスプライト
	//--------------------------------------------------------------------------------------
	WallSprite::WallSprite(const shared_ptr<Stage>& StagePtr, const wstring& TextureKey, bool Trace,
		const Vector2& StartScale, const Vector2& StartPos):
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos)
	{}

	WallSprite::~WallSprite() {}
	void WallSprite::OnCreate() {
		float HelfSize = 0.5f;
		//頂点配列(縦横5個ずつ表示)
		vector<VertexPositionColorTexture> vertices = {
			{ VertexPositionColorTexture(Vector3(-HelfSize, HelfSize, 0),Color4(1.0f,1.0f,1.0f,1.0f), Vector2(0.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vector3(HelfSize, HelfSize, 0), Color4(0.0f, 1.0f, 1.0f, 1.0f), Vector2(5.0f, 0.0f)) },
			{ VertexPositionColorTexture(Vector3(-HelfSize, -HelfSize, 0), Color4(1.0f, 0.0f, 1.0f, 1.0f), Vector2(0.0f, 5.0f)) },
			{ VertexPositionColorTexture(Vector3(HelfSize, -HelfSize, 0), Color4(0.0f, 0.0f, 0, 1.0f), Vector2(5.0f, 5.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y, 1.0f);
		PtrTransform->SetRotation(0, 0, 0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y, 0.0f);

		//文字列をつける
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetText(L"Bボタンでゲームステージへ");
		PtrString->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PCTSpriteDraw>(vertices, indices);
		PtrDraw->SetWrapSampler(true);
		PtrDraw->SetTextureResource(m_TextureKey);
	}

	void WallSprite::OnUpdate() {

	}




	//--------------------------------------------------------------------------------------
	///	スクロールするスプライト
	//--------------------------------------------------------------------------------------
	ScrollSprite::ScrollSprite(const shared_ptr<Stage>& StagePtr,
		const wstring& TextureKey, bool Trace, 
		const Vector2& StartScale, const Vector2& StartPos):
		GameObject(StagePtr),
		m_TextureKey(TextureKey),
		m_Trace(Trace),
		m_StartScale(StartScale),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}

	ScrollSprite::~ScrollSprite() {}
	void ScrollSprite::OnCreate() {
		float HelfSize = 0.5f;
		//頂点配列
		m_BackupVertices = {
			{ VertexPositionTexture(Vector3(-HelfSize, HelfSize, 0), Vector2(0.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, HelfSize, 0), Vector2(4.0f, 0.0f)) },
			{ VertexPositionTexture(Vector3(-HelfSize, -HelfSize, 0), Vector2(0.0f, 1.0f)) },
			{ VertexPositionTexture(Vector3(HelfSize, -HelfSize, 0), Vector2(4.0f, 1.0f)) },
		};
		//インデックス配列
		vector<uint16_t> indices = { 0, 1, 2, 1, 3, 2 };
		SetAlphaActive(m_Trace);
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_StartScale.x, m_StartScale.y,1.0f);
		PtrTransform->SetRotation(0,0,0);
		PtrTransform->SetPosition(m_StartPos.x, m_StartPos.y,0.0f);
		//頂点とインデックスを指定してスプライト作成
		auto PtrDraw = AddComponent<PTSpriteDraw>(m_BackupVertices, indices);
		PtrDraw->SetWrapSampler(true);
		PtrDraw->SetTextureResource(m_TextureKey);
	}
	void ScrollSprite::OnUpdate() {
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		if (m_TotalTime > 1.0f) {
			m_TotalTime = 0;
		}
		vector<VertexPositionTexture> NewVertices;
		for (size_t i = 0; i < m_BackupVertices.size(); i++) {
			Vector2 UV = m_BackupVertices[i].textureCoordinate;
			if (UV.x == 0.0f) {
				UV.x = m_TotalTime;
			}
			else if (UV.x == 4.0f) {
				UV.x += m_TotalTime;
			}
			auto v = VertexPositionTexture(
				m_BackupVertices[i].position,
				UV
			);
			NewVertices.push_back(v);
		}
		auto PtrDraw = GetComponent<PTSpriteDraw>();
		PtrDraw->UpdateVertices(NewVertices);
	}
	

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox() {}

	//初期化
	void FixedBox::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();

		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//衝突判定
		auto PtrObb = AddComponent<CollisionObb>();
		PtrObb->SetFixed(true);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_CUBE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");
	}

	//--------------------------------------------------------------------------------------
	//	敵
	//--------------------------------------------------------------------------------------
	//構築と破棄
	Enemy::Enemy(const shared_ptr<Stage>& StagePtr,
		const shared_ptr<StageCellMap>& CellMap,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
	) :
		GameObject(StagePtr),
		m_CelMap(CellMap),
		m_Scale(Scale),
		m_StartRotation(Rotation),
		m_StartPosition(Position),
		m_CellIndex(-1),
		m_NextCellIndex(-1),
		m_TargetCellIndex(-1)
	{
	}
	Enemy::~Enemy() {}

	bool Enemy::SearchPlayer() {
		auto MapPtr = m_CelMap.lock();
		if (MapPtr) {
			auto PathPtr = GetComponent<PathSearch>();
			auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
			auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
			m_CellPath.clear();
			//パス検索をかける
			if (PathPtr->SearchCell(PlayerPos, m_CellPath)) {
				//検索が成功した
				m_CellIndex = 0;
				m_TargetCellIndex = m_CellPath.size() - 1;
				if (m_CellIndex == m_TargetCellIndex) {
					//すでに同じセルにいる
					m_NextCellIndex = m_CellIndex;
				}
				else {
					//離れている
					m_NextCellIndex = m_CellIndex + 1;

				}
				return true;
			}
			else {
				//失敗した
				m_CellIndex = -1;
				m_NextCellIndex = -1;
				m_TargetCellIndex = -1;
			}
		}
		return false;
	}

	bool Enemy::DefaultBehavior() {
		auto PtrRigid = GetComponent<Rigidbody>();
		auto Velo = PtrRigid->GetVelocity();
		Velo *= 0.95f;
		PtrRigid->SetVelocity(Velo);
		auto MapPtr = m_CelMap.lock();
		if (MapPtr) {
			auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
			auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
			CellIndex PlayerCell;
			if (MapPtr->FindCell(PlayerPos, PlayerCell)) {
				return false;
			}
		}
		return true;
	}


	bool Enemy::SeekBehavior() {
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PlayerPos = PlayerPtr->GetComponent<Transform>()->GetPosition();
		auto MyPos = GetComponent<Transform>()->GetPosition();

		auto MapPtr = m_CelMap.lock();
		if (MapPtr) {
			if (SearchPlayer()) {
				auto PtrSeek = GetComponent<SeekSteering>();
				if (m_NextCellIndex == 0) {
					auto PtrRigid = GetComponent<Rigidbody>();
					auto Velo = PtrRigid->GetVelocity();
					Velo *= 0.95f;
					PtrRigid->SetVelocity(Velo);
					PlayerPos.y = m_StartPosition.y;
					PtrSeek->SetTargetPosition(PlayerPos);
				}
				else {
					if (Vector3EX::Length(MyPos - PlayerPos) <= 3.0f){
						auto PtrRigid = GetComponent<Rigidbody>();
						auto Velo = PtrRigid->GetVelocity();
						Velo *= 0.95f;
						PtrRigid->SetVelocity(Velo);
					}
					AABB ret;
					MapPtr->FindAABB(m_CellPath[m_NextCellIndex], ret);
					auto Pos = ret.GetCenter();
					Pos.y = m_StartPosition.y;
					PtrSeek->SetTargetPosition(Pos);
				}
				return true;
			}
			else {
				CellIndex PlayerCell;
				if (MapPtr->FindCell(PlayerPos, PlayerCell)) {
					auto PtrSeek = GetComponent<SeekSteering>();
					AABB ret;
					MapPtr->FindAABB(PlayerCell, ret);
					auto Pos = ret.GetCenter();
					Pos.y = m_StartPosition.y;
					PtrSeek->SetTargetPosition(Pos);
					return true;
				}
			}
		}
		return false;
	}


	//初期化
	void Enemy::OnCreate() {
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetPosition(m_StartPosition);
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_StartRotation);
		//重力をつける
		auto PtrGravity = AddComponent<Gravity>();
		//Rigidbodyをつける
		auto PtrRigid = AddComponent<Rigidbody>();
		//反発係数は0.5（半分）
		PtrRigid->SetReflection(0.5f);
		auto PtrSeek = AddComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
		//パス検索
		auto MapPtr = m_CelMap.lock();
		if (!MapPtr) {
			throw BaseException(
				L"セルマップが不定です",
				L"if (!MapPtr) ",
				L" Enemy::OnCreate()"
			);
		}
		auto PathPtr = AddComponent<PathSearch>(MapPtr);

		//SPの衝突判定をつける
		auto PtrColl = AddComponent<CollisionSphere>();
		PtrColl->SetIsHitAction(IsHitAction::AutoOnParent);

		//影をつける
		auto ShadowPtr = AddComponent<Shadowmap>();
		ShadowPtr->SetMeshResource(L"DEFAULT_SPHERE");

		auto PtrDraw = AddComponent<PNTStaticDraw>();
		PtrDraw->SetMeshResource(L"DEFAULT_SPHERE");
		PtrDraw->SetTextureResource(L"TRACE2_TX");
		//透明処理をする
		SetAlphaActive(true);

		m_StateMachine = make_shared<StateMachine<Enemy>>(GetThis<Enemy>());
		m_StateMachine->ChangeState(EnemyDefault::Instance());
	}

	//更新
	void Enemy::OnUpdate() {
		//ステートマシンのUpdateを行う
		//この中でステートの切り替えが行われる
		m_StateMachine->Update();
	}

	//回転を進行方向に向かせる
	void Enemy::OnLastUpdate() {
		auto PtrRigidbody = GetComponent<Rigidbody>();
		//回転の更新
		//Velocityの値で、回転を変更する
		//これで進行方向を向くようになる
		auto PtrTransform = GetComponent<Transform>();
		Vector3 Velocity = PtrRigidbody->GetVelocity();
		if (Velocity.Length() > 0.0f) {
			Vector3 Temp = Velocity;
			Temp.Normalize();
			float ToAngle = atan2(Temp.x, Temp.z);
			Quaternion Qt;
			Qt.RotationRollPitchYaw(0, ToAngle, 0);
			Qt.Normalize();
			//現在の回転を取得
			Quaternion NowQt = PtrTransform->GetQuaternion();
			//現在と目標を補間（10分の1）
			NowQt.Slerp(NowQt, Qt, 0.1f);
			PtrTransform->SetQuaternion(NowQt);
		}
	}


	//--------------------------------------------------------------------------------------
	///	デフォルトステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(EnemyDefault)

	void EnemyDefault::Enter(const shared_ptr<Enemy>& Obj) {
	}

	void EnemyDefault::Execute(const shared_ptr<Enemy>& Obj) {
		if (!Obj->DefaultBehavior()) {
			Obj->GetStateMachine()->ChangeState(EnemySeek::Instance());
		}
	}
	void EnemyDefault::Exit(const shared_ptr<Enemy>& Obj) {
	}

	//--------------------------------------------------------------------------------------
	///	プレイヤーを追いかけるステート
	//--------------------------------------------------------------------------------------
	//ステートのインスタンス取得
	IMPLEMENT_SINGLETON_INSTANCE(EnemySeek)

	void EnemySeek::Enter(const shared_ptr<Enemy>& Obj) {
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(true);
	}

	void EnemySeek::Execute(const shared_ptr<Enemy>& Obj) {
		if (!Obj->SeekBehavior()) {
			Obj->GetStateMachine()->ChangeState(EnemyDefault::Instance());
		}
	}

	void EnemySeek::Exit(const shared_ptr<Enemy>& Obj) {
		auto PtrSeek = Obj->GetComponent<SeekSteering>();
		PtrSeek->SetUpdateActive(false);
	}

	

}
//end basecross
