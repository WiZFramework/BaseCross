/*!
@file BasePhysics.cpp
@brief 物理計算インターファイス実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"


namespace basecross {

	using namespace sce::PhysicsEffects;


	namespace ps {

#define NUM_RIGIDBODIES 500
#define NUM_JOINTS    500
#define NUM_CONTACTS  4000

		const float timeStep = 0.016f;
		const float separateBias = 0.1f;
		int iteration = 5;

		//重力
		PfxVector3 PsGravity(0.0f, -9.8f, 0.0f);

		//J ワールドサイズ
		//E World size
		PfxVector3 worldCenter(0.0f);
		PfxVector3 worldExtent(500.0f);

		//J 剛体
		//E Rigid body
		PfxRigidState states[NUM_RIGIDBODIES];
		PfxRigidBody  bodies[NUM_RIGIDBODIES];
		PfxCollidable collidables[NUM_RIGIDBODIES];
		PfxSolverBody solverBodies[NUM_RIGIDBODIES];
		int numRigidBodies = 0;

		//J プロキシ
		//E Proxies
		PfxBroadphaseProxy proxies[NUM_RIGIDBODIES];

		//J ジョイント
		//E Joint
		PfxConstraintPair jointPairs[NUM_JOINTS];
		PfxJoint joints[NUM_JOINTS];
		int numJoints = 0;

		//J ペア
		//E Pairs
		unsigned int pairSwap;
		unsigned int numPairs[2];
		PfxBroadphasePair pairsBuff[2][NUM_CONTACTS];

		//J コンタクト
		//E Contacts
		PfxContactManifold contacts[NUM_CONTACTS];
		int numContacts;

		PfxUInt32 contactIdPool[NUM_CONTACTS];
		int numContactIdPool;

		//J 一時バッファ
		//E Temporary buffers
#define POOL_BYTES (5*1024*1024)
		unsigned char SCE_PFX_ALIGNED(128) poolBuff[POOL_BYTES];

		//J 一時バッファ用スタックアロケータ
		//E Stack allocator for temporary buffers
		PfxHeapManager pool(poolBuff, POOL_BYTES);

		void broadphase()
		{
			pairSwap = 1 - pairSwap;

			unsigned int &numPreviousPairs = numPairs[1 - pairSwap];
			unsigned int &numCurrentPairs = numPairs[pairSwap];
			PfxBroadphasePair *previousPairs = pairsBuff[1 - pairSwap];
			PfxBroadphasePair *currentPairs = pairsBuff[pairSwap];

			//J 剛体が最も分散している軸を見つける
			//E Find the axis along which all rigid bodies are most widely positioned
			int axis = 0;
			{
				PfxVector3 s(0.0f), s2(0.0f);
				for (int i = 0; i<numRigidBodies; i++) {
					PfxVector3 c = states[i].getPosition();
					s += c;
					s2 += mulPerElem(c, c);
				}
				PfxVector3 v = s2 - mulPerElem(s, s) / (float)numRigidBodies;
				if (v[1] > v[0]) axis = 1;
				if (v[2] > v[axis]) axis = 2;
			}

			//J ブロードフェーズプロキシの更新
			//E Create broadpahse proxies
			{
				for (int i = 0; i<numRigidBodies; i++) {
					pfxUpdateBroadphaseProxy(proxies[i], states[i], collidables[i], worldCenter, worldExtent, axis);
				}

				int workBytes = sizeof(PfxBroadphaseProxy) * numRigidBodies;
				void *workBuff = pool.allocate(workBytes);

				pfxSort(proxies, (PfxSortData32*)workBuff, numRigidBodies);

				pool.deallocate(workBuff);
			}

			//J 交差ペア探索
			//E Find overlapped pairs
			{
				PfxFindPairsParam findPairsParam;
				findPairsParam.pairBytes = pfxGetPairBytesOfFindPairs(NUM_CONTACTS);
				findPairsParam.pairBuff = pool.allocate(findPairsParam.pairBytes);
				findPairsParam.workBytes = pfxGetWorkBytesOfFindPairs(NUM_CONTACTS);
				findPairsParam.workBuff = pool.allocate(findPairsParam.workBytes);
				findPairsParam.proxies = proxies;
				findPairsParam.numProxies = numRigidBodies;
				findPairsParam.maxPairs = NUM_CONTACTS;
				findPairsParam.axis = axis;

				PfxFindPairsResult findPairsResult;

				int ret = pfxFindPairs(findPairsParam, findPairsResult);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxFindPairs failed %d\n", ret);

				pool.deallocate(findPairsParam.workBuff);

				//J 交差ペア合成
				//E Decompose overlapped pairs into 3 arrays
				PfxDecomposePairsParam decomposePairsParam;
				decomposePairsParam.pairBytes = pfxGetPairBytesOfDecomposePairs(numPreviousPairs, findPairsResult.numPairs);
				decomposePairsParam.pairBuff = pool.allocate(decomposePairsParam.pairBytes);
				decomposePairsParam.workBytes = pfxGetWorkBytesOfDecomposePairs(numPreviousPairs, findPairsResult.numPairs);
				decomposePairsParam.workBuff = pool.allocate(decomposePairsParam.workBytes);
				decomposePairsParam.previousPairs = previousPairs;
				decomposePairsParam.numPreviousPairs = numPreviousPairs;
				decomposePairsParam.currentPairs = findPairsResult.pairs; // Set pairs from pfxFindPairs()
				decomposePairsParam.numCurrentPairs = findPairsResult.numPairs; // Set the number of pairs from pfxFindPairs()

				PfxDecomposePairsResult decomposePairsResult;

				ret = pfxDecomposePairs(decomposePairsParam, decomposePairsResult);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxDecomposePairs failed %d\n", ret);

				pool.deallocate(decomposePairsParam.workBuff);

				PfxBroadphasePair *outNewPairs = decomposePairsResult.outNewPairs;
				PfxBroadphasePair *outKeepPairs = decomposePairsResult.outKeepPairs;
				PfxBroadphasePair *outRemovePairs = decomposePairsResult.outRemovePairs;
				PfxUInt32 numOutNewPairs = decomposePairsResult.numOutNewPairs;
				PfxUInt32 numOutKeepPairs = decomposePairsResult.numOutKeepPairs;
				PfxUInt32 numOutRemovePairs = decomposePairsResult.numOutRemovePairs;

				//J 廃棄ペアのコンタクトをプールに戻す
				//E Put removed contacts into the contact pool
				for (PfxUInt32 i = 0; i<numOutRemovePairs; i++) {
					contactIdPool[numContactIdPool++] = pfxGetContactId(outRemovePairs[i]);
				}

				//J 新規ペアのコンタクトのリンクと初期化
				//E Add new contacts and initialize
				for (PfxUInt32 i = 0; i<numOutNewPairs; i++) {
					int cId = 0;
					if (numContactIdPool > 0) {
						cId = contactIdPool[--numContactIdPool];
					}
					else {
						cId = numContacts++;
					}
					if (cId >= NUM_CONTACTS) {
						cId = 0;
					}
					SCE_PFX_ASSERT(cId < NUM_CONTACTS);
					pfxSetContactId(outNewPairs[i], cId);
					PfxContactManifold &contact = contacts[cId];
					contact.reset(pfxGetObjectIdA(outNewPairs[i]), pfxGetObjectIdB(outNewPairs[i]));
				}

				//J 新規ペアと維持ペアを合成
				//E Merge 'new' and 'keep' pairs
				numCurrentPairs = 0;
				for (PfxUInt32 i = 0; i<numOutKeepPairs; i++) {
					currentPairs[numCurrentPairs++] = outKeepPairs[i];
				}
				for (PfxUInt32 i = 0; i<numOutNewPairs; i++) {
					currentPairs[numCurrentPairs++] = outNewPairs[i];
				}

				pool.deallocate(decomposePairsParam.pairBuff);
				pool.deallocate(findPairsParam.pairBuff);
			}

			{
				int workBytes = sizeof(PfxBroadphasePair) * numCurrentPairs;
				void *workBuff = pool.allocate(workBytes);

				pfxSort(currentPairs, (PfxSortData16*)workBuff, numCurrentPairs);

				pool.deallocate(workBuff);
			}
		}

		void collision()
		{
			unsigned int numCurrentPairs = numPairs[pairSwap];
			PfxBroadphasePair *currentPairs = pairsBuff[pairSwap];

			//J リフレッシュ
			//E Refresh contacts
			{
				PfxRefreshContactsParam param;
				param.contactPairs = currentPairs;
				param.numContactPairs = numCurrentPairs;
				param.offsetContactManifolds = contacts;
				param.offsetRigidStates = states;
				param.numRigidBodies = numRigidBodies;

				int ret = pfxRefreshContacts(param);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxRefreshContacts failed %d\n", ret);
			}

			//J 衝突検出
			//E Detect collisions
			{
				PfxDetectCollisionParam param;
				param.contactPairs = currentPairs;
				param.numContactPairs = numCurrentPairs;
				param.offsetContactManifolds = contacts;
				param.offsetRigidStates = states;
				param.offsetCollidables = collidables;
				param.numRigidBodies = numRigidBodies;

				int ret = pfxDetectCollision(param);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxDetectCollision failed %d\n", ret);
			}
		}

		void constraintSolver()
		{

			unsigned int numCurrentPairs = numPairs[pairSwap];
			PfxBroadphasePair *currentPairs = pairsBuff[pairSwap];

			{
				PfxSetupSolverBodiesParam param;
				param.states = states;
				param.bodies = bodies;
				param.solverBodies = solverBodies;
				param.numRigidBodies = numRigidBodies;

				int ret = pfxSetupSolverBodies(param);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxSetupSolverBodies failed %d\n", ret);
			}

			{
				PfxSetupContactConstraintsParam param;
				param.contactPairs = currentPairs;
				param.numContactPairs = numCurrentPairs;
				param.offsetContactManifolds = contacts;
				param.offsetRigidStates = states;
				param.offsetRigidBodies = bodies;
				param.offsetSolverBodies = solverBodies;
				param.numRigidBodies = numRigidBodies;
				param.timeStep = timeStep;
				param.separateBias = separateBias;

				int ret = pfxSetupContactConstraints(param);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxSetupContactConstraints failed %d\n", ret);
			}

			{
				PfxSetupJointConstraintsParam param;
				param.jointPairs = jointPairs;
				param.numJointPairs = numJoints;
				param.offsetJoints = joints;
				param.offsetRigidStates = states;
				param.offsetRigidBodies = bodies;
				param.offsetSolverBodies = solverBodies;
				param.numRigidBodies = numRigidBodies;
				param.timeStep = timeStep;

				for (int i = 0; i<numJoints; i++) {
					pfxUpdateJointPairs(jointPairs[i], i, joints[i], states[joints[i].m_rigidBodyIdA], states[joints[i].m_rigidBodyIdB]);
				}

				int ret = pfxSetupJointConstraints(param);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxSetupJointConstraints failed %d\n", ret);
			}

			{
				PfxSolveConstraintsParam param;
				param.workBytes = pfxGetWorkBytesOfSolveConstraints(numRigidBodies, numCurrentPairs, numJoints);
				param.workBuff = pool.allocate(param.workBytes);
				param.contactPairs = currentPairs;
				param.numContactPairs = numCurrentPairs;
				param.offsetContactManifolds = contacts;
				param.jointPairs = jointPairs;
				param.numJointPairs = numJoints;
				param.offsetJoints = joints;
				param.offsetRigidStates = states;
				param.offsetSolverBodies = solverBodies;
				param.numRigidBodies = numRigidBodies;
				param.iteration = iteration;

				int ret = pfxSolveConstraints(param);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxSolveConstraints failed %d\n", ret);

				pool.deallocate(param.workBuff);
			}

		}

		void integrate()
		{
			PfxUpdateRigidStatesParam param;
			param.states = states;
			param.bodies = bodies;
			param.numRigidBodies = numRigidBodies;
			param.timeStep = timeStep;

			pfxUpdateRigidStates(param);
		}


	}

	using namespace basecross::ps;

	struct PxBoxParam {
		PsMotionType m_MotionType;
		PfxVector3 m_Size;
		PfxQuat m_Quat;
		PfxVector3 m_Pos;
		PfxVector3 m_Force;
		PfxVector3 m_Torque;
		PfxVector3 m_Velocity;
		float m_Mass;
		PxBoxParam() {}
		PxBoxParam(const PsBoxParam& param) {
			m_MotionType = param.m_MotionType;
			m_Size = param.m_HalfSize;
			m_Quat = param.m_Quat;
			m_Pos = param.m_Pos;
			m_Force = param.m_Force;
			m_Torque = param.m_Torque;
			m_Velocity = param.m_Velocity;
			m_Mass = param.m_Mass;
		}
	};

	struct PxSphereParam {
		PsMotionType m_MotionType;
		float m_Radius;
		PfxQuat m_Quat;
		PfxVector3 m_Pos;
		PfxVector3 m_Force;
		PfxVector3 m_Torque;
		PfxVector3 m_Velocity;
		float m_Mass;
		PxSphereParam() {}
		PxSphereParam(const PsSphereParam& param) {
			m_MotionType = param.m_MotionType;
			m_Radius = param.m_Radius;
			m_Quat = param.m_Quat;
			m_Pos = param.m_Pos;
			m_Force = param.m_Force;
			m_Torque = param.m_Torque;
			m_Velocity = param.m_Velocity;
			m_Mass = param.m_Mass;
		}
	};


	//--------------------------------------------------------------------------------------
	///	物理オブジェクトの親
	//--------------------------------------------------------------------------------------
	PhysicsObject::PhysicsObject() {}
	PhysicsObject::~PhysicsObject() {}

	//--------------------------------------------------------------------------------------
	//	Implイディオム
	//--------------------------------------------------------------------------------------
	struct PhysicsBox::Impl {
		//初期化パラメータ
		PxBoxParam m_PxBoxParam;
		Impl(const PsBoxParam& param):
			m_PxBoxParam(param)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	ボックス物理オブジェクト
	//--------------------------------------------------------------------------------------
	PhysicsBox::PhysicsBox(const PsBoxParam& param, uint16_t index):
		pImpl(new Impl(param))
	{
		m_Index = index;
	}

	PhysicsBox::~PhysicsBox() {}

	void PhysicsBox::OnCreate() {
		PfxBox box(pImpl->m_PxBoxParam.m_Size);
		PfxShape shape;
		shape.reset();
		shape.setBox(box);
		collidables[m_Index].reset();
		collidables[m_Index].addShape(shape);
		collidables[m_Index].finish();
		bodies[m_Index].reset();
		bodies[m_Index].setMass(pImpl->m_PxBoxParam.m_Mass);
		bodies[m_Index].setInertia(pfxCalcInertiaBox(pImpl->m_PxBoxParam.m_Size, pImpl->m_PxBoxParam.m_Mass));
		states[m_Index].reset();
		states[m_Index].setPosition(pImpl->m_PxBoxParam.m_Pos);
		states[m_Index].setOrientation(pImpl->m_PxBoxParam.m_Quat);
		states[m_Index].setMotionType((ePfxMotionType)pImpl->m_PxBoxParam.m_MotionType);
		states[m_Index].setRigidBodyId(m_Index);
		//フォースを加える
		pfxApplyExternalForce(
			states[m_Index], bodies[m_Index],
			bodies[m_Index].getMass() * pImpl->m_PxBoxParam.m_Force,
			pImpl->m_PxBoxParam.m_Torque,
			timeStep
		);
	}

	//--------------------------------------------------------------------------------------
	//	Implイディオム
	//--------------------------------------------------------------------------------------
	struct PhysicsSphere::Impl {
		//初期化パラメータ
		PxSphereParam m_PxSphereParam;
		Impl(const PsSphereParam& param) :
			m_PxSphereParam(param)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	球体物理オブジェクト
	//--------------------------------------------------------------------------------------
	PhysicsSphere::PhysicsSphere(const PsSphereParam& param, uint16_t index) :
		pImpl(new Impl(param))
	{
		m_Index = index;
	}

	PhysicsSphere::~PhysicsSphere() {}

	void PhysicsSphere::OnCreate() {
		PfxSphere sphere(pImpl->m_PxSphereParam.m_Radius);
		PfxShape shape;
		shape.reset();
		shape.setSphere(sphere);
		collidables[m_Index].reset();
		collidables[m_Index].addShape(shape);
		collidables[m_Index].finish();
		bodies[m_Index].reset();
		bodies[m_Index].setMass(pImpl->m_PxSphereParam.m_Mass);
		bodies[m_Index].setInertia(pfxCalcInertiaSphere(pImpl->m_PxSphereParam.m_Radius, pImpl->m_PxSphereParam.m_Mass));
		states[m_Index].reset();
		states[m_Index].setPosition(pImpl->m_PxSphereParam.m_Pos);
		states[m_Index].setOrientation(pImpl->m_PxSphereParam.m_Quat);
		states[m_Index].setMotionType((ePfxMotionType)pImpl->m_PxSphereParam.m_MotionType);
		states[m_Index].setLinearVelocity(pImpl->m_PxSphereParam.m_Velocity);
		states[m_Index].setRigidBodyId(m_Index);
		//フォースを加える
		pfxApplyExternalForce(
			states[m_Index], bodies[m_Index],
			bodies[m_Index].getMass() * pImpl->m_PxSphereParam.m_Force,
			pImpl->m_PxSphereParam.m_Torque,
			timeStep
		);
	}


	//--------------------------------------------------------------------------------------
	///	物理計算用のインターフェイス
	//--------------------------------------------------------------------------------------
	BasePhysics::BasePhysics()
	{
	}
	BasePhysics::~BasePhysics() {}

	shared_ptr<PhysicsBox> BasePhysics::AddSingleBox(const PsBoxParam& param, const wstring& indexKey) {
		uint16_t index;
		if (indexKey != L"") {
			index = GetMappedIndex(indexKey);
		}
		else {
			index = numRigidBodies++;
		}
		return ObjectFactory::Create<PhysicsBox>(param, index);
	}

	uint16_t BasePhysics::GetMappedIndex(const wstring& key) {
		uint16_t ret;
		auto it = m_ConstIndexMap.find(key);
		if (it != m_ConstIndexMap.end()) {
			//見つかった
			ret = it->second;
		}
		else {
			ret = numRigidBodies++;
			m_ConstIndexMap[key] = ret;
		}
		return ret;
	}

	bool BasePhysics::CheckBodyIndex(const wstring& indexKey) const {
		auto it = m_ConstIndexMap.find(indexKey);
		if (it == m_ConstIndexMap.end()) {
			return false;
		}
		//見つかった
		return true;
	}


	shared_ptr<PhysicsSphere> BasePhysics::AddSingleSphere(const PsSphereParam& param,const wstring& indexKey) {
		uint16_t index;
		if (indexKey != L"") {
			index = GetMappedIndex(indexKey);
		}
		else {
			index = numRigidBodies++;
		}
		return ObjectFactory::Create<PhysicsSphere>(param,index);
	}

	uint16_t BasePhysics::GetNumBodies() const {
		return numRigidBodies;
	}

	void BasePhysics::GetBodyStatus(uint16_t index, PsBodyStatus& st) {
		st.m_Position = states[index].getPosition();
		st.m_Orientation = states[index].getOrientation();
		st.m_LinearVelocity = states[index].getLinearVelocity();
		st.m_AngularVelocity = states[index].getAngularVelocity();
	}


	//void BasePhysics::GetBodyWorldQuatPos(uint16_t index, bsm::Quat& qt, bsm::Vec3& pos) {
	//	qt = states[index].getOrientation();
	//	pos = states[index].getPosition();
	//}

	uint16_t BasePhysics::GetNumShapes(uint16_t body_index) {
		return (uint16_t)collidables[body_index].getNumShapes();
	}

	void BasePhysics::GetShapeOffsetQuatPos(uint16_t body_index, uint16_t shape_index, bsm::Quat& qt, bsm::Vec3& pos) {
		auto& shape = collidables[body_index].getShape(shape_index);
		pos = shape.getOffsetPosition();
		qt = shape.getOffsetOrientation();
	}

	ePfxShapeType BasePhysics::GetShapeType(uint16_t body_index, uint16_t shape_index)const {
		auto& shape = collidables[body_index].getShape(shape_index);
		return (ePfxShapeType)shape.getType();
	}

	bsm::Vec3 BasePhysics::GetShapeBoxScale(uint16_t body_index, uint16_t shape_index) const {
		auto& shape = collidables[body_index].getShape(shape_index);
		bsm::Vec3 scale;
		scale = shape.getBox().m_half;
		return scale;
	}

	float BasePhysics::GetShapeSphereRadius(uint16_t body_index, uint16_t shape_index) const {
		auto& shape = collidables[body_index].getShape(shape_index);
		return (float)shape.getSphere().m_radius;
	}

	void BasePhysics::Reset() {
		numRigidBodies = 0;
		numJoints = 0;
		numContacts = 0;
		pairSwap = 0;
		numPairs[0] = 0;
		numPairs[1] = 0;
		numContactIdPool = 0;
		::ZeroMemory(states, sizeof(PfxRigidState) * NUM_RIGIDBODIES);
		::ZeroMemory(bodies, sizeof(PfxRigidBody) * NUM_RIGIDBODIES);
		::ZeroMemory(collidables, sizeof(PfxCollidable) * NUM_RIGIDBODIES);
		::ZeroMemory(solverBodies, sizeof(PfxSolverBody) * NUM_RIGIDBODIES);
		::ZeroMemory(proxies, sizeof(PfxBroadphaseProxy) * NUM_RIGIDBODIES);
		::ZeroMemory(jointPairs, sizeof(PfxConstraintPair) * NUM_JOINTS);
		::ZeroMemory(joints, sizeof(PfxJoint) * NUM_JOINTS);
		::ZeroMemory(pairsBuff, sizeof(PfxBroadphasePair) * NUM_CONTACTS * 2);

		::ZeroMemory(contacts, sizeof(PfxContactManifold) * NUM_CONTACTS);
		::ZeroMemory(contactIdPool, sizeof(PfxUInt32) * NUM_CONTACTS);
		::ZeroMemory(poolBuff, sizeof(poolBuff));
	}

	void BasePhysics::Update() {
		for (int i = 1; i<numRigidBodies; i++) {
			pfxApplyExternalForce(
				states[i], bodies[i], 
				bodies[i].getMass() * PsGravity,
				PfxVector3(0.0f),
				timeStep
			);
		}
		broadphase();
		collision();
		constraintSolver();
		integrate();
	}

}
//end basecross
