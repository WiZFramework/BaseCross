/*!
@file PhysicsObject.h
@brief 物理計算オブジェクト
*/

#pragma once
#include "stdafx.h"

namespace basecross {

#define PS_PI		3.14159265358979f
#define PS_EPSILON	1e-5f
#define PS_FLT_MAX FLT_MAX

#define PS_MIN(a,b) (((a)<(b))?(a):(b))
#define PS_MAX(a,b) (((a)>(b))?(a):(b))
#define PS_CLAMP(v,a,b) PS_MAX(a,PS_MIN(v,b))

#define PS_MAX_LINEAR_VELOCITY		340.0f
#define PS_MAX_ANGULAR_VELOCITY	(PS_PI * 60.0f)
#define PS_NUM_SHAPES 5
#define PS_CONVEX_MESH_MAX_VERTICES	34
#define PS_CONVEX_MESH_MAX_EDGES		96
#define PS_CONVEX_MESH_MAX_FACETS		64
#define PS_NUM_CONTACTS 4
#define PS_CONTACT_SAME_POINT			0.01f
#define PS_CONTACT_THRESHOLD_NORMAL	0.01f
#define PS_CONTACT_THRESHOLD_TANGENT	0.002f
#define PS_MAX_LINEAR_VELOCITY		340.0f
#define PS_MAX_ANGULAR_VELOCITY	(PS_PI * 60.0f)

#define PS_MAX_LINEAR_VELOCITY		340.0f
#define PS_MAX_ANGULAR_VELOCITY	(PS_PI * 60.0f)
#define PS_AABB_EXPAND 0.01f

	const uint32_t maxRigidBodies = 500;
	const uint32_t maxJoints = 100;
	const uint32_t maxPairs = 5000;
	const float timeStep = 0.016f;
	const float contactBias = 0.1f;
	const float contactSlop = 0.001f;
	const uint32_t iteration = 10;
	const Vec3 gravity(0.0f, -9.8f, 0.0f);

	enum class PsMotionType {
		MotionTypeActive,
		MotionTypeStatic
	};

	struct PsState {
		Vec3	m_position;
		Quat	m_orientation;
		Vec3	m_linearVelocity;
		Vec3	m_angularVelocity;
		PsMotionType m_motionType;
		void reset()
		{
			m_position = Vec3(0.0f);
			m_orientation.identity();
			m_linearVelocity = Vec3(0.0f);
			m_angularVelocity = Vec3(0.0f);
			m_motionType = PsMotionType::MotionTypeActive;
		}
	};

	struct PsRigidBody {
		Mat3x3     m_inertia;
		float       m_mass;
		float       m_restitution;
		float       m_friction;
		void reset()
		{
			m_mass = 1.0f;
			m_inertia.identity();
			m_restitution = 0.2f;
			m_friction = 0.6f;
		}
	};

	enum class PsEdgeType {
		EdgeTypeConvex,
		EdgeTypeConcave,
		EdgeTypeFlat,
	};

	struct PsEdge {
		PsEdgeType type;
		uint8_t vertId[2];
		uint8_t facetId[2];
	};

	struct PsFacet {
		uint8_t vertId[3];
		uint8_t edgeId[3];
		Vec3 normal;
	};

	struct PsConvexMesh {
		uint8_t m_numVertices;
		uint8_t m_numFacets;
		uint8_t m_numEdges; ///< エッジ数
		Vec3 m_vertices[PS_CONVEX_MESH_MAX_VERTICES]; ///< 頂点配列
		PsEdge m_edges[PS_CONVEX_MESH_MAX_EDGES]; ///< エッジ配列
		PsFacet m_facets[PS_CONVEX_MESH_MAX_FACETS]; ///< 面配列
		void reset()
		{
			m_numVertices = 0;
			m_numFacets = 0;
			m_numEdges = 0;
		}
	};

	enum class PsShapeType {
		ShapeTypeBox,
	};

	struct PsShape {
		PsConvexMesh m_geometry;
		Vec3 m_offsetPosition;
		Quat m_offsetOrientation;
		PsShapeType m_PsShapeType;
		Vec3 m_Scale;
		void reset()
		{
			m_geometry.reset();
			m_offsetPosition = Vec3(0.0f);
			m_offsetOrientation.identity();
			m_Scale = Vec3(1.0f, 1.0f, 1.0f);
		}
	};

	struct PsCollidable {
		uint8_t m_numShapes;
		PsShape m_shapes[PS_NUM_SHAPES];
		Vec3 m_center;
		Vec3 m_half;
		void reset()
		{
			m_numShapes = 0;
			m_center = Vec3(0.0f);
			m_half = Vec3(0.0f);
		}
		uint32_t addShape(const PsShape &shape)
		{
			if (m_numShapes >= PS_NUM_SHAPES) {
				throw BaseException(
					L"これ以上追加できません",
					L"if (m_numShapes >= PS_NUM_SHAPES)",
					L"PsCollidable::addShape()"
				);
			}
			uint32_t ret = m_numShapes;
			m_shapes[m_numShapes++] = shape;
			return ret;
		}

		void finish()
		{
			// AABBを計算する
			Vec3 aabbMax(-PS_FLT_MAX), aabbMin(PS_FLT_MAX);
			for (uint32_t i = 0; i<m_numShapes; i++) {
				const PsConvexMesh &mesh = m_shapes[i].m_geometry;
				for (uint32_t v = 0; v<mesh.m_numVertices; v++) {
					aabbMax = maxPerElem(aabbMax, m_shapes[i].m_offsetPosition + rotate(m_shapes[i].m_offsetOrientation, mesh.m_vertices[v]));
					aabbMin = minPerElem(aabbMin, m_shapes[i].m_offsetPosition + rotate(m_shapes[i].m_offsetOrientation, mesh.m_vertices[v]));
				}
			}
			m_center = (aabbMax + aabbMin) * 0.5f;
			m_half = (aabbMax - aabbMin) * 0.5f;
		}
	};

	struct PsConstraint {
		Vec3 axis;
		float jacDiagInv;
		float rhs;
		float lowerLimit;
		float upperLimit;
		float accumImpulse;
	};

	struct PsBallJoint {
		float bias;
		uint32_t rigidBodyA;
		uint32_t rigidBodyB;
		Vec3 anchorA;
		Vec3 anchorB;
		PsConstraint constraint;
		void reset()
		{
			bias = 0.1f;
			constraint.accumImpulse = 0.0f;
		}
	};

	struct PsContactPoint {
		float distance;
		Vec3 pointA;
		Vec3 pointB;
		Vec3 normal;
		PsConstraint constraints[3];
		void reset()
		{
			constraints[0].accumImpulse = 0.0f;
			constraints[1].accumImpulse = 0.0f;
			constraints[2].accumImpulse = 0.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	/// 衝突情報
	//--------------------------------------------------------------------------------------
	struct PsContact {
		uint32_t m_numContacts;
		float m_friction;
		PsContactPoint m_contactPoints[PS_NUM_CONTACTS];

		int findNearestContactPoint(const Vec3 &newPointA, const Vec3 &newPointB, const Vec3 &newNormal);

		int sort4ContactPoints(const Vec3 &newPoint, float newDistance);

		void removeContactPoint(int i);

		void reset();

		void refresh(const Vec3 &pA, const Quat &qA, const Vec3 &pB, const Quat &qB);

		void merge(const PsContact &contact);

		void addContact(
			float penetrationDepth,
			const Vec3 &normal,
			const Vec3 &contactPointA,
			const Vec3 &contactPointB);
	};


	/// ペアの種類
	enum class PsPairType {
		PairTypeNew,
		PairTypeKeep,
	};

	struct PsPair {
		PsPairType type;
		union {
			uint64_t key;
			struct {
				uint32_t rigidBodyA;
				uint32_t rigidBodyB;
			};
		};
		PsContact *contact;
	};

	struct PsTranform {
		uint32_t stateIndex;
		uint32_t shapeIndex;
		PsTranform():
			stateIndex(0), shapeIndex(0) {}
		PsTranform(uint32_t state, uint32_t shape) :
			stateIndex(state),  shapeIndex(shape) {}
	};


	//--------------------------------------------------------------------------------------
	///	物理計算マネージャ
	//--------------------------------------------------------------------------------------
	class PhysicsManager : public GameObject {
		shared_ptr<MeshResource> m_BoxMeshRes;

		PsState states[maxRigidBodies];
		PsRigidBody bodies[maxRigidBodies];
		PsCollidable collidables[maxRigidBodies];
		uint32_t numRigidBodies = 0;

		PsBallJoint joints[maxJoints];
		uint32_t numJoints = 0;

		unsigned int pairSwap;
		uint32_t numPairs[2];
		PsPair pairs[2][maxPairs];

		map<uint32_t, PsTranform > m_PsTranformMap;
		uint32_t m_PsTranformIndex;

		int physicsGetNumRigidbodies()
		{
			return numRigidBodies;
		}

		const PsState &physicsGetState(int i)
		{
			return states[i];
		}

		const PsRigidBody &physicsGetRigidBody(int i)
		{
			return bodies[i];
		}

		const PsCollidable &physicsGetCollidable(int i)
		{
			return collidables[i];
		}

		void DrawShapeWireFrame(const shared_ptr<MeshResource>& res, const Mat4x4& world);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	StagePtr	ステージ
		*/
		//--------------------------------------------------------------------------------------
		PhysicsManager(const shared_ptr<Stage>& StagePtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~PhysicsManager();
		//--------------------------------------------------------------------------------------
		/*!
		@brief オブジェクトの追加
		@param[in]	ShapeType	形状
		@param[in]	MotionType	ふるまい
		@param[in]	Scale	スケーリング
		@param[in]	Qt	回転
		@param[in]	Pos	位置
		@return	形状のインデックス
		*/
		//--------------------------------------------------------------------------------------
		uint32_t AddSingleShape(PsShapeType ShapeType, PsMotionType MotionType,const Vec3& Scale,const Quat& Qt,const Vec3& Pos);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 形状の現在の位置と回転を得る
		@param[in]	TransIndex	形状のインデックス
		@param[out]	RetQt	回転を得る参照
		@param[out]	RetPos	位置を得る参照
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetShapeQuatPos(uint32_t TransIndex,Quat& RetQt,Vec3& RetPos);
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
