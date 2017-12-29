/*!
@file BasePhysics.cpp
@brief 物理計算インターファイス実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"


namespace basecross {

	using namespace sce::PhysicsEffects;

#define NUM_RIGIDBODIES 500
#define NUM_JOINTS    500
#define NUM_CONTACTS  4000
#define NUM_KEEP_SHAPES  (NUM_RIGIDBODIES*4)

	namespace ps {

		const float timeStep = 0.016f;
		const float separateBias = 0.1f;
		int iteration = 8;

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
		int numProxiesInWorld = 0;

		//J プロキシ
		//E Proxies
		PfxBroadphaseProxy proxies[6][NUM_RIGIDBODIES]; // shared by simulation and raycast

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

		//J シミュレーションアイランド
		//E Island generation
		PfxIsland *island = NULL;
		PfxUInt8 SCE_PFX_ALIGNED(16) islandBuff[32 * NUM_RIGIDBODIES]; // Island buffer should be 32 * the number of rigid bodies.

		//J スリープ制御
		//E Sleep control
		/*
		A sleeping object wakes up, when
		* a new pair related to this rigid body is created
		* a pair releated to this rigid body is removed
		* a rigid body's velocity or position are updated
		*/

		//J スリープに入るカウント
		//E Count to enter sleeping
		const PfxUInt32 sleepCount = 180;

		//J 速度が閾値以下ならばスリープカウントが増加
		//E If velocity is under the following value, sleep count is increased.
		const PfxFloat sleepVelocity = 0.3f;

		//J 一時バッファ
		//E Temporary buffers
#define POOL_BYTES (5*1024*1024)
		unsigned char SCE_PFX_ALIGNED(128) poolBuff[POOL_BYTES];

		//J 一時バッファ用スタックアロケータ
		//E Stack allocator for temporary buffers
		PfxHeapManager pool(poolBuff, POOL_BYTES);

		///////////////////////////////////////////////////////////////////////////////
		// Simulation Function

		int frame = 0;

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

			numProxiesInWorld = 0;

			//E Create broadpahse proxies
			{
				//J レイキャストと共用するため、全ての軸に対するプロキシ配列を作成する
				//E To share with ray casting, create proxy arrays for all axis

				PfxUpdateBroadphaseProxiesParam param;
				param.workBytes = pfxGetWorkBytesOfUpdateBroadphaseProxies(numRigidBodies);
				param.workBuff = pool.allocate(param.workBytes, 128);
				param.numRigidBodies = numRigidBodies;
				param.offsetRigidStates = states;
				param.offsetCollidables = collidables;
				param.proxiesX = proxies[0];
				param.proxiesY = proxies[1];
				param.proxiesZ = proxies[2];
				param.proxiesXb = proxies[3];
				param.proxiesYb = proxies[4];
				param.proxiesZb = proxies[5];
				param.worldCenter = worldCenter;
				param.worldExtent = worldExtent;
				param.outOfWorldBehavior = SCE_PFX_OUT_OF_WORLD_BEHAVIOR_FIX_MOTION | SCE_PFX_OUT_OF_WORLD_BEHAVIOR_REMOVE_PROXY;

				PfxUpdateBroadphaseProxiesResult result;

				int ret = pfxUpdateBroadphaseProxies(param, result);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxUpdateBroadphaseProxies failed %d\n", ret);

				pool.deallocate(param.workBuff);

				numProxiesInWorld = numRigidBodies - result.numOutOfWorldProxies;
			}

			//J 交差ペア探索
			//E Find overlapped pairs
			{
				PfxFindPairsParam findPairsParam;
				findPairsParam.pairBytes = pfxGetPairBytesOfFindPairs(NUM_CONTACTS);
				findPairsParam.pairBuff = pool.allocate(findPairsParam.pairBytes);
				findPairsParam.workBytes = pfxGetWorkBytesOfFindPairs(NUM_CONTACTS);
				findPairsParam.workBuff = pool.allocate(findPairsParam.workBytes);
				findPairsParam.proxies = proxies[axis];
				findPairsParam.numProxies = numProxiesInWorld;
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

					//J 寝てる剛体を起こす
					//E Wake up sleeping rigid bodies
					PfxRigidState &stateA = states[pfxGetObjectIdA(outRemovePairs[i])];
					PfxRigidState &stateB = states[pfxGetObjectIdB(outRemovePairs[i])];
					if (stateA.isAsleep()) {
						stateA.wakeup();
					//	SCE_PFX_PRINTF("wakeup %u\n", stateA.getRigidBodyId());
					}
					if (stateB.isAsleep()) {
						stateB.wakeup();
					//	SCE_PFX_PRINTF("wakeup %u\n", stateB.getRigidBodyId());
					}
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

					//J 寝てる剛体を起こす
					//E Wake up sleeping rigid bodies
					PfxRigidState &stateA = states[pfxGetObjectIdA(outNewPairs[i])];
					PfxRigidState &stateB = states[pfxGetObjectIdB(outNewPairs[i])];
					if (stateA.isAsleep()) {
						stateA.wakeup();
						SCE_PFX_PRINTF("wakeup %u\n", stateA.getRigidBodyId());
					}
					if (stateB.isAsleep()) {
						stateB.wakeup();
						SCE_PFX_PRINTF("wakeup %u\n", stateB.getRigidBodyId());
					}
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

			//J アイランド生成
			//E Create simulation islands
			{
				PfxGenerateIslandParam param;
				param.islandBuff = islandBuff;
				param.islandBytes = 32 * NUM_RIGIDBODIES;
				param.pairs = currentPairs;
				param.numPairs = numCurrentPairs;
				param.numObjects = numRigidBodies;

				PfxGenerateIslandResult result;

				int ret = pfxGenerateIsland(param, result);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxGenerateIsland failed %d\n", ret);
				island = result.island;

				//J ジョイント分のペアを追加
				//E Add joint pairs to islands
				ret = pfxAppendPairs(island, jointPairs, numJoints);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxAppendPairs failed %d\n", ret);
			}
		}

		void constraintSolver()
		{
			PfxPerfCounter pc;

			unsigned int numCurrentPairs = numPairs[pairSwap];
			PfxBroadphasePair *currentPairs = pairsBuff[pairSwap];

			pc.countBegin("setup solver bodies");
			{
				PfxSetupSolverBodiesParam param;
				param.states = states;
				param.bodies = bodies;
				param.solverBodies = solverBodies;
				param.numRigidBodies = numRigidBodies;

				int ret = pfxSetupSolverBodies(param);
				if (ret != SCE_PFX_OK) SCE_PFX_PRINTF("pfxSetupSolverBodies failed %d\n", ret);
			}
			pc.countEnd();

			pc.countBegin("setup contact constraints");
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
			pc.countEnd();

			pc.countBegin("setup joint constraints");
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
			pc.countEnd();

			pc.countBegin("solve constraints");
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
			pc.countEnd();

			//pc.printCount();
		}

		void sleepOrWakeup()
		{
			PfxFloat sleepVelSqr = sleepVelocity * sleepVelocity;

			for (PfxUInt32 i = 0; i<(PfxUInt32)numRigidBodies; i++) {
				PfxRigidState &state = states[i];
				if (SCE_PFX_MOTION_MASK_CAN_SLEEP(state.getMotionType())) {
					PfxFloat linVelSqr = lengthSqr(state.getLinearVelocity());
					PfxFloat angVelSqr = lengthSqr(state.getAngularVelocity());

					if (state.isAwake()) {
						if (linVelSqr < sleepVelSqr && angVelSqr < sleepVelSqr) {
							state.incrementSleepCount();
						}
						else {
							state.resetSleepCount();
						}
					}
				}
			}

			if (island) {
				for (PfxUInt32 i = 0; i<pfxGetNumIslands(island); i++) {
					int numActive = 0;
					int numSleep = 0;
					int numCanSleep = 0;

					{
						PfxIslandUnit *islandUnit = pfxGetFirstUnitInIsland(island, (PfxUInt32)i);
						for (; islandUnit != NULL; islandUnit = pfxGetNextUnitInIsland(islandUnit)) {
							if (!(SCE_PFX_MOTION_MASK_CAN_SLEEP(states[pfxGetUnitId(islandUnit)].getMotionType()))) continue;
							PfxRigidState &state = states[pfxGetUnitId(islandUnit)];
							if (state.isAsleep()) {
								numSleep++;
							}
							else {
								numActive++;
								if (state.getSleepCount() > sleepCount) {
									numCanSleep++;
								}
							}
						}
					}

					// Deactivate Island
					if (numCanSleep > 0 && numCanSleep == numActive + numSleep) {
						PfxIslandUnit *islandUnit = pfxGetFirstUnitInIsland(island, (PfxUInt32)i);
						for (; islandUnit != NULL; islandUnit = pfxGetNextUnitInIsland(islandUnit)) {
							if (!(SCE_PFX_MOTION_MASK_CAN_SLEEP(states[pfxGetUnitId(islandUnit)].getMotionType()))) continue;
							states[pfxGetUnitId(islandUnit)].sleep();
						}
					}

					// Activate Island
					else if (numSleep > 0 && numActive > 0) {
						PfxIslandUnit *islandUnit = pfxGetFirstUnitInIsland(island, (PfxUInt32)i);
						for (; islandUnit != NULL; islandUnit = pfxGetNextUnitInIsland(islandUnit)) {
							if (!(SCE_PFX_MOTION_MASK_CAN_SLEEP(states[pfxGetUnitId(islandUnit)].getMotionType()))) continue;
							states[pfxGetUnitId(islandUnit)].wakeup();
						}
					}
				}
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

	namespace ps {
		PfxConvexMesh convexMeshes[NUM_RIGIDBODIES];
		set<uint32_t> reservedConvexMeshes;
		uint32_t getNextConvexMesheIndex() {
			for (uint32_t i = 0; i < NUM_RIGIDBODIES; i++) {
				if (reservedConvexMeshes.find(i) == reservedConvexMeshes.end()) {
					//現在iは空いている
					reservedConvexMeshes.insert(i);
					return i;
				}
			}
			//全部埋まっている
			throw BaseException(
				L"これ以上凸型メッシュは増やせません",
				L"size >= NUM_RIGIDBODIES",
				L"ps::getNextConvexMesheIndex()"
			);
		}
		void rereaseConvexMesheIndex(uint32_t index) {
			pfxReleaseConvexMesh(convexMeshes[index]);
			::ZeroMemory(&convexMeshes[index], sizeof(PfxConvexMesh));
			reservedConvexMeshes.erase(index);
		}

		//Combined用shapeの配列
		PfxShape combinedShapes[NUM_KEEP_SHAPES];
		//shapeのインデックスはPfxUInt16
		PfxUInt16 numCombinedShapes = 0;
		void getNewCombinedShapesIndices(vector<PfxUInt16>& indices, PfxUInt16 count) {
			if ((numCombinedShapes + count) >= NUM_KEEP_SHAPES) {
				throw BaseException(
					L"これ以上Combined用shapeは増やせません",
					L"if ((numCombinedShapes + count) >= NUM_KEEP_SHAPES)",
					L"ps::getNewCombinedShapesIndices()"
				);
			}
			PfxUInt16 setIndex = numCombinedShapes;
			PfxUInt16 i = 0;
			while (i < count) {
				indices.push_back(setIndex);
				setIndex++;
				i++;
			}
			numCombinedShapes += count;
		}
	}

	//--------------------------------------------------------------------------------------
	///	物理オブジェクトの親
	//--------------------------------------------------------------------------------------
	PhysicsObject::PhysicsObject() {}
	PhysicsObject::~PhysicsObject() {}

	void PhysicsObject::SetParamStatus(const PsParamBase& param) {
		ps::states[m_Index].reset();
		ps::states[m_Index].setPosition((PfxVector3)param.m_Pos);
		ps::states[m_Index].setOrientation(
			(PfxQuat)param.m_Quat
		);
		ps::states[m_Index].setLinearVelocity(
			(PfxVector3)param.m_LinearVelocity
		);
		ps::states[m_Index].setAngularVelocity(
			(PfxVector3)param.m_AngularVelocity
		);
		if (param.m_UseSleep) {
			ps::states[m_Index].setUseSleep(1);
		}
		else {
			ps::states[m_Index].setUseSleep(0);
		}
		ps::states[m_Index].setMotionType((ePfxMotionType)param.m_MotionType);
		ps::states[m_Index].setRigidBodyId(m_Index);

	}


	//--------------------------------------------------------------------------------------
	//	ボックスImplイディオム
	//--------------------------------------------------------------------------------------
	struct PhysicsBox::Impl {
		//初期化パラメータ
		PsBoxParam m_PsBoxParam;
		Impl(const PsBoxParam& param):
			m_PsBoxParam(param)
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
		PfxBox box((PfxVector3)pImpl->m_PsBoxParam.m_HalfSize);
		PfxShape shape;
		shape.reset();
		shape.setBox(box);
		ps::collidables[m_Index].reset();
		ps::collidables[m_Index].addShape(shape);
		ps::collidables[m_Index].finish();
		ps::bodies[m_Index].reset();
		ps::bodies[m_Index].setMass((PfxFloat)pImpl->m_PsBoxParam.m_Mass);
		ps::bodies[m_Index].setInertia((PfxMatrix3)pImpl->m_PsBoxParam.m_Inertia);
		ps::bodies[m_Index].setRestitution((PfxFloat)pImpl->m_PsBoxParam.m_Restitution);
		ps::bodies[m_Index].setFriction((PfxFloat)pImpl->m_PsBoxParam.m_Friction);
		SetParamStatus(pImpl->m_PsBoxParam);
	}

	const PsBoxParam& PhysicsBox::GetParam() const {
		return pImpl->m_PsBoxParam;
	}


	//--------------------------------------------------------------------------------------
	//	球体Implイディオム
	//--------------------------------------------------------------------------------------
	struct PhysicsSphere::Impl {
		//初期化パラメータ
		PsSphereParam m_PsSphereParam;
		Impl(const PsSphereParam& param) :
			m_PsSphereParam(param)
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
		PfxSphere sphere((PfxFloat)pImpl->m_PsSphereParam.m_Radius);
		PfxShape shape;
		shape.reset();
		shape.setSphere(sphere);
		ps::collidables[m_Index].reset();
		ps::collidables[m_Index].addShape(shape);
		ps::collidables[m_Index].finish();
		ps::bodies[m_Index].reset();
		ps::bodies[m_Index].setMass((PfxFloat)pImpl->m_PsSphereParam.m_Mass);
		ps::bodies[m_Index].setInertia((PfxMatrix3)pImpl->m_PsSphereParam.m_Inertia);
		ps::bodies[m_Index].setRestitution((PfxFloat)pImpl->m_PsSphereParam.m_Restitution);
		ps::bodies[m_Index].setFriction((PfxFloat)pImpl->m_PsSphereParam.m_Friction);
		SetParamStatus(pImpl->m_PsSphereParam);
	}

	const PsSphereParam& PhysicsSphere::GetParam() const {
		return pImpl->m_PsSphereParam;
	}


	//--------------------------------------------------------------------------------------
	//	カプセルImplイディオム
	//--------------------------------------------------------------------------------------
	struct PhysicsCapsule::Impl {
		//初期化パラメータ
		PsCapsuleParam m_PsCapsuleParam;
		Impl(const PsCapsuleParam& param) :
			m_PsCapsuleParam(param)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	カプセル物理オブジェクト
	//--------------------------------------------------------------------------------------
	PhysicsCapsule::PhysicsCapsule(const PsCapsuleParam& param, uint16_t index):
		pImpl(new Impl(param))
	{
		m_Index = index;
	}
	PhysicsCapsule::~PhysicsCapsule() {}

	void PhysicsCapsule::OnCreate() {
		PfxCapsule capsule(
			(PfxFloat)pImpl->m_PsCapsuleParam.m_HalfLen,
			(PfxFloat)pImpl->m_PsCapsuleParam.m_Radius
		);
		PfxShape shape;
		shape.reset();
		shape.setCapsule(capsule);
		ps::collidables[m_Index].reset();
		ps::collidables[m_Index].addShape(shape);
		ps::collidables[m_Index].finish();
		ps::bodies[m_Index].reset();
		ps::bodies[m_Index].setMass((PfxFloat)pImpl->m_PsCapsuleParam.m_Mass);
		ps::bodies[m_Index].setInertia((PfxMatrix3)pImpl->m_PsCapsuleParam.m_Inertia);
		ps::bodies[m_Index].setRestitution((PfxFloat)pImpl->m_PsCapsuleParam.m_Restitution);
		ps::bodies[m_Index].setFriction((PfxFloat)pImpl->m_PsCapsuleParam.m_Friction);
		SetParamStatus(pImpl->m_PsCapsuleParam);
	}

	const PsCapsuleParam& PhysicsCapsule::GetParam() const {
		return pImpl->m_PsCapsuleParam;
	}

	//--------------------------------------------------------------------------------------
	//	シリンダーImplイディオム
	//--------------------------------------------------------------------------------------
	struct PhysicsCylinder::Impl {
		//初期化パラメータ
		PsCylinderParam m_PsCylinderParam;
		Impl(const PsCylinderParam& param) :
			m_PsCylinderParam(param)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	シリンダー物理オブジェクト
	//--------------------------------------------------------------------------------------
	PhysicsCylinder::PhysicsCylinder(const PsCylinderParam& param, uint16_t index):
		pImpl(new Impl(param))
	{
		m_Index = index;
	}
	PhysicsCylinder::~PhysicsCylinder() {}

	void PhysicsCylinder::OnCreate() {
		PfxCylinder cylinder(
			(PfxFloat)pImpl->m_PsCylinderParam.m_HalfLen,
			(PfxFloat)pImpl->m_PsCylinderParam.m_Radius
		);
		PfxShape shape;
		shape.reset();
		shape.setCylinder(cylinder);
		ps::collidables[m_Index].reset();
		ps::collidables[m_Index].addShape(shape);
		ps::collidables[m_Index].finish();
		ps::bodies[m_Index].reset();
		ps::bodies[m_Index].setMass((PfxFloat)pImpl->m_PsCylinderParam.m_Mass);
		ps::bodies[m_Index].setInertia((PfxMatrix3)pImpl->m_PsCylinderParam.m_Inertia);
		ps::bodies[m_Index].setRestitution((PfxFloat)pImpl->m_PsCylinderParam.m_Restitution);
		ps::bodies[m_Index].setFriction((PfxFloat)pImpl->m_PsCylinderParam.m_Friction);
		SetParamStatus(pImpl->m_PsCylinderParam);
	}

	const PsCylinderParam& PhysicsCylinder::GetParam() const {
		return pImpl->m_PsCylinderParam;
	}


	//--------------------------------------------------------------------------------------
	//	PsConvexMeshResource　Implイディオム
	//--------------------------------------------------------------------------------------
	struct PsConvexMeshResource::Impl {
		uint32_t m_Index;
		//バックアップ用の頂点(VertexPositionNormalTexture)とインデックス
		vector<VertexPositionNormalTexture> m_Vertices;
		vector<uint16_t> m_Indices;
		Impl(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices):
			m_Index(0)
		{
			try {
				m_Vertices = vertices;
				m_Indices = indices;

				PfxCreateConvexMeshParam param;
				vector<float> vertFloat;
				for (auto& v : vertices) {
					vertFloat.push_back(v.position.x);
					vertFloat.push_back(v.position.y);
					vertFloat.push_back(v.position.z);
					vertFloat.push_back(v.normal.x);
					vertFloat.push_back(v.normal.y);
					vertFloat.push_back(v.normal.z);
				}

				param.verts = &vertFloat.front();
				param.numVerts = vertices.size();
				param.vertexStrideBytes = sizeof(float) * 6;
				param.triangles = &indices.front();
				param.numTriangles = indices.size() / 3;
				param.triangleStrideBytes = sizeof(uint16_t) * 3;
				//numConvexMeshes
				//convexMeshes
				m_Index = ps::getNextConvexMesheIndex();
				PfxInt32 ret = pfxCreateConvexMesh(ps::convexMeshes[m_Index], param);
				if (ret != SCE_PFX_OK) {
					throw BaseException(
						L"ConvexMeshResourceの作成に失敗しました。",
						L"if (ret != SCE_PFX_OK)",
						L"PsConvexMeshResource::Impl::Impl()"
					);
				}
			}
			catch (...) {
				throw;
			}
		}
		~Impl() {
			ps::rereaseConvexMesheIndex(m_Index);
		}
	};

	//--------------------------------------------------------------------------------------
	///	ConvexMeshリソース
	//--------------------------------------------------------------------------------------
	PsConvexMeshResource::PsConvexMeshResource(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices):
		BaseResource(),
		pImpl(new Impl(vertices, indices))
	{}

	shared_ptr<PsConvexMeshResource> PsConvexMeshResource::CreateMeshResource(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices) {
		try {
			return ObjectFactory::Create<PsConvexMeshResource>(vertices, indices);
		}
		catch (...) {
			throw;
		}
	}


	PsConvexMeshResource::~PsConvexMeshResource() {}
	uint32_t PsConvexMeshResource::GetMeshIndex() const {
		return pImpl->m_Index;
	}

	const vector<VertexPositionNormalTexture>& PsConvexMeshResource::GetVertices() const {
		return pImpl->m_Vertices;
	}

	const vector<uint16_t>& PsConvexMeshResource::GetIndices() const {
		return pImpl->m_Indices;
	}




	//--------------------------------------------------------------------------------------
	//	PhysicsConvex　Implイディオム
	//--------------------------------------------------------------------------------------
	struct PhysicsConvex::Impl {
		//初期化パラメータ
		PsConvexParam m_PsConvexParam;
		Impl(const PsConvexParam& param) :
			m_PsConvexParam(param)
		{}
		~Impl() {
		}
	};



	//--------------------------------------------------------------------------------------
	///	ConvexMesh物理オブジェクト(頂点指定のオブジェクト)
	//--------------------------------------------------------------------------------------
	PhysicsConvex::PhysicsConvex(const PsConvexParam& param, uint16_t index):
		pImpl(new Impl(param))
	{
		m_Index = index;
	}
	PhysicsConvex::~PhysicsConvex() {}

	void PhysicsConvex::OnCreate() {
		if (!pImpl->m_PsConvexParam.m_ConvexMeshResource) {
			throw BaseException(
				L"ConvexMeshResourceが見つかりません",
				L"if (!pImpl->m_PsConvexParam.m_ConvexMeshResource)",
				L"PhysicsConvexMesh::OnCreate()"
			);
		}
		PfxShape shape;
		shape.reset();
		shape.setConvexMesh(&ps::convexMeshes[pImpl->m_PsConvexParam.m_ConvexMeshResource->GetMeshIndex()]);
		ps::collidables[m_Index].reset();
		ps::collidables[m_Index].addShape(shape);
		ps::collidables[m_Index].finish();
		ps::bodies[m_Index].reset();
		ps::bodies[m_Index].setMass((PfxFloat)pImpl->m_PsConvexParam.m_Mass);
		ps::bodies[m_Index].setInertia((PfxMatrix3)pImpl->m_PsConvexParam.m_Inertia);
		ps::bodies[m_Index].setRestitution((PfxFloat)pImpl->m_PsConvexParam.m_Restitution);
		ps::bodies[m_Index].setFriction((PfxFloat)pImpl->m_PsConvexParam.m_Friction);
		SetParamStatus(pImpl->m_PsConvexParam);
	}

	const PsConvexParam& PhysicsConvex::GetParam() const {
		return pImpl->m_PsConvexParam;
	}


	//--------------------------------------------------------------------------------------
	//	PhysicsCombined　Implイディオム
	//--------------------------------------------------------------------------------------
	struct PhysicsCombinedObject::Impl {
		//初期化パラメータ
		PsCombinedParam m_PsCombinedParam;
		Impl(const PsCombinedParam& param) :
			m_PsCombinedParam(param)
		{}
		~Impl() {
		}
	};


	//--------------------------------------------------------------------------------------
	///	プリミティブ合成物理オブジェクト
	//--------------------------------------------------------------------------------------
	PhysicsCombinedObject::PhysicsCombinedObject(const PsCombinedParam& param, uint16_t index):
		pImpl(new Impl(param))
	{
		m_Index = index;
	}

	PhysicsCombinedObject::~PhysicsCombinedObject() {}

	void PhysicsCombinedObject::OnCreate() {
		vector<PfxUInt16> PrimIndices;
		ps::getNewCombinedShapesIndices(PrimIndices, (PfxUInt16)pImpl->m_PsCombinedParam.m_Primitives.size());
		ps::collidables[m_Index].reset(ps::combinedShapes, &PrimIndices.front(), (int)PrimIndices.size());
		for (auto& v : pImpl->m_PsCombinedParam.m_Primitives) {
			switch (v.m_CombinedType) {
			case PsCombinedType::TypeSphere:
				{
					PfxSphere sphere((PfxFloat)v.m_Radius);
					PfxShape shape;
					shape.reset();
					shape.setSphere(sphere);
					shape.setOffsetOrientation((PfxQuat)v.m_OffsetOrientation);
					shape.setOffsetPosition((PfxVector3)v.m_OffsetPosition);
					ps::collidables[m_Index].addShape(shape);
				}
				break;
				case PsCombinedType::TypeBox:
				{
					PfxBox box((PfxVector3)v.m_HalfSize);
					PfxShape shape;
					shape.reset();
					shape.setBox(box);
					shape.setOffsetOrientation((PfxQuat)v.m_OffsetOrientation);
					shape.setOffsetPosition((PfxVector3)v.m_OffsetPosition);
					ps::collidables[m_Index].addShape(shape);
				}
				break;
				case PsCombinedType::TypeCapsule:
				{
					PfxCapsule capsule(
						(PfxFloat)v.m_HalfLen,
						(PfxFloat)v.m_Radius
					);
					PfxShape shape;
					shape.reset();
					shape.setCapsule(capsule);
					shape.setOffsetOrientation((PfxQuat)v.m_OffsetOrientation);
					shape.setOffsetPosition((PfxVector3)v.m_OffsetPosition);
					ps::collidables[m_Index].addShape(shape);
				}
				break;
				case PsCombinedType::TypeCylinder:
				{
					PfxCylinder cylinder(
						(PfxFloat)v.m_HalfLen,
						(PfxFloat)v.m_Radius
					);
					PfxShape shape;
					shape.reset();
					shape.setCylinder(cylinder);
					shape.setOffsetOrientation((PfxQuat)v.m_OffsetOrientation);
					shape.setOffsetPosition((PfxVector3)v.m_OffsetPosition);
					ps::collidables[m_Index].addShape(shape);
				}
				break;
				case PsCombinedType::TypeConvex:
				{
					if (!v.m_ConvexMeshResource) {
						throw BaseException(
							L"ConvexMeshResourceが見つかりません",
							L"if (!v.m_ConvexMeshResource)",
							L"PhysicsCombined::OnCreate()"
						);
					}
					PfxShape shape;
					shape.reset();
					shape.setConvexMesh(&ps::convexMeshes[v.m_ConvexMeshResource->GetMeshIndex()]);
					shape.setOffsetOrientation((PfxQuat)v.m_OffsetOrientation);
					shape.setOffsetPosition((PfxVector3)v.m_OffsetPosition);
					ps::collidables[m_Index].addShape(shape);
				}
				break;
			}

		}
		ps::collidables[m_Index].finish();
		ps::bodies[m_Index].reset();
		ps::bodies[m_Index].setMass((PfxFloat)pImpl->m_PsCombinedParam.m_Mass);
		ps::bodies[m_Index].setInertia((PfxMatrix3)pImpl->m_PsCombinedParam.m_Inertia);
		ps::bodies[m_Index].setRestitution((PfxFloat)pImpl->m_PsCombinedParam.m_Restitution);
		ps::bodies[m_Index].setFriction((PfxFloat)pImpl->m_PsCombinedParam.m_Friction);
		SetParamStatus(pImpl->m_PsCombinedParam);
	}

	const PsCombinedParam& PhysicsCombinedObject::GetParam() const {
		return pImpl->m_PsCombinedParam;
	}

	//--------------------------------------------------------------------------------------
	///	物理計算用のインターフェイス
	//--------------------------------------------------------------------------------------
	BasePhysics::BasePhysics()
	{
	}
	BasePhysics::~BasePhysics() {}

	shared_ptr<PhysicsBox> BasePhysics::AddBox(const PsBoxParam& param, uint16_t index) {
		if (ps::numRigidBodies >= NUM_RIGIDBODIES) {
			throw BaseException(
				L"これ以上物理オブジェクトを増やせません",
				L"if (ps::numRigidBodies >= ps::NUM_RIGIDBODIES)",
				L"BasePhysics::AddBox()"
			);
		}
		if (index >= ps::numRigidBodies) {
			index = ps::numRigidBodies++;
		}
		return ObjectFactory::Create<PhysicsBox>(param, index);
	}


	shared_ptr<PhysicsSphere> BasePhysics::AddSphere(const PsSphereParam& param, uint16_t index) {
		if (ps::numRigidBodies >= NUM_RIGIDBODIES) {
			throw BaseException(
				L"これ以上物理オブジェクトを増やせません",
				L"if (ps::numRigidBodies >= ps::NUM_RIGIDBODIES)",
				L"BasePhysics::AddSphere()"
			);
		}
		if (index >= ps::numRigidBodies) {
			index = ps::numRigidBodies++;
		}
		return ObjectFactory::Create<PhysicsSphere>(param, index);
	}

	shared_ptr<PhysicsCapsule> BasePhysics::AddCapsule(const PsCapsuleParam& param, uint16_t index) {
		if (ps::numRigidBodies >= NUM_RIGIDBODIES) {
			throw BaseException(
				L"これ以上物理オブジェクトを増やせません",
				L"if (ps::numRigidBodies >= ps::NUM_RIGIDBODIES)",
				L"BasePhysics::AddCapsule()"
			);
		}
		if (index >= ps::numRigidBodies) {
			index = ps::numRigidBodies++;
		}
		return ObjectFactory::Create<PhysicsCapsule>(param, index);
	}

	shared_ptr<PhysicsCylinder> BasePhysics::AddCylinder(const PsCylinderParam& param, uint16_t index) {
		if (ps::numRigidBodies >= NUM_RIGIDBODIES) {
			throw BaseException(
				L"これ以上物理オブジェクトを増やせません",
				L"if (ps::numRigidBodies >= ps::NUM_RIGIDBODIES)",
				L"BasePhysics::AddCylinder()"
			);
		}
		if (index >= ps::numRigidBodies) {
			index = ps::numRigidBodies++;
		}
		return ObjectFactory::Create<PhysicsCylinder>(param, index);
	}

	shared_ptr<PhysicsConvex> BasePhysics::AddConvex(const PsConvexParam& param, uint16_t index) {
		if (ps::numRigidBodies >= NUM_RIGIDBODIES) {
			throw BaseException(
				L"これ以上物理オブジェクトを増やせません",
				L"if (ps::numRigidBodies >= ps::NUM_RIGIDBODIES)",
				L"BasePhysics::AddConvex()"
			);
		}
		if (index >= ps::numRigidBodies) {
			index = ps::numRigidBodies++;
		}
		return ObjectFactory::Create<PhysicsConvex>(param, index);
	}

	shared_ptr<PhysicsCombinedObject> BasePhysics::AddCombinedObject(const PsCombinedParam& param, uint16_t index) {
		if (ps::numRigidBodies >= NUM_RIGIDBODIES) {
			throw BaseException(
				L"これ以上物理オブジェクトを増やせません",
				L"if (ps::numRigidBodies >= ps::NUM_RIGIDBODIES)",
				L"BasePhysics::AddCombined()"
			);
		}
		if (index >= ps::numRigidBodies) {
			index = ps::numRigidBodies++;
		}
		return ObjectFactory::Create<PhysicsCombinedObject>(param, index);
	}



	uint16_t BasePhysics::GetNumBodies() const {
		return ps::numRigidBodies;
	}

	void BasePhysics::GetBodyStatus(uint16_t body_index, PsBodyStatus& st)const {
		st.m_Position = ps::states[body_index].getPosition();
		st.m_Orientation = ps::states[body_index].getOrientation();
		st.m_LinearVelocity = ps::states[body_index].getLinearVelocity();
		st.m_AngularVelocity = ps::states[body_index].getAngularVelocity();
	}

	void BasePhysics::SetBodyStatus(uint16_t body_index, const PsBodyUpdateStatus& st) {
		ps::states[body_index].setAngularVelocity((PfxVector3)st.m_AngularVelocity);
		ps::states[body_index].setLinearVelocity((PfxVector3)st.m_LinearVelocity);
		//フォースを加える
		pfxApplyExternalForce(
			ps::states[body_index], ps::bodies[body_index],
			ps::bodies[body_index].getMass() * (PfxVector3)st.m_Force,
			ps::bodies[body_index].getMass() * (PfxVector3)st.m_Torque,
			ps::timeStep
		);
	}

	void BasePhysics::WakeUpBody(uint16_t body_index) {
		if (ps::states[body_index].isAsleep()) {
			ps::states[body_index].wakeup();
		}
	}

	void BasePhysics::SetBodyPosition(uint16_t body_index, const bsm::Vec3& pos) {
		if (ps::states[body_index].getMotionType() != ePfxMotionType::kPfxMotionTypeTrigger) {
			throw BaseException(
				L"モーションタイプがMotionTypeTrigger以外では設定できません",
				L"if (states[body_index].getMotionType != ePfxMotionType::kPfxMotionTypeTrigger)",
				L"BasePhysics::SetBodyPosition()"
			);
		}
		ps::states[body_index].setPosition((PfxVector3)pos);
	}


	void BasePhysics::SetBodyLinearVelocity(uint16_t body_index, const bsm::Vec3& v) {
		ps::states[body_index].setLinearVelocity((PfxVector3)v);
	}
	void BasePhysics::SetBodyAngularVelocity(uint16_t body_index, const bsm::Vec3& v) {
		ps::states[body_index].setAngularVelocity((PfxVector3)v);
	}

	void BasePhysics::ApplyBodyForce(uint16_t body_index, const bsm::Vec3& v) {
		pfxApplyExternalForce(
			ps::states[body_index], ps::bodies[body_index],
			ps::bodies[body_index].getMass() * (PfxVector3)v,
			PfxVector3(0.0f),
			ps::timeStep
		);

	}
	void BasePhysics::ApplyBodyTorque(uint16_t body_index, const bsm::Vec3& v) {
		pfxApplyExternalForce(
			ps::states[body_index], ps::bodies[body_index],
			PfxVector3(0.0f),
			ps::bodies[body_index].getMass() * (PfxVector3)v,
			ps::timeStep
		);
	}

	uint16_t BasePhysics::GetNumShapes(uint16_t body_index)const {
		return (uint16_t)ps::collidables[body_index].getNumShapes();
	}

	void BasePhysics::GetShapeOffsetQuatPos(uint16_t body_index, uint16_t shape_index, bsm::Quat& qt, bsm::Vec3& pos) {
		auto& shape = ps::collidables[body_index].getShape(shape_index);
		pos = shape.getOffsetPosition();
		qt = shape.getOffsetOrientation();
	}

	ePfxShapeType BasePhysics::GetShapeType(uint16_t body_index, uint16_t shape_index)const {
		auto& shape = ps::collidables[body_index].getShape(shape_index);
		return (ePfxShapeType)shape.getType();
	}

	uint16_t BasePhysics::GetNumJoints()const {
		return (uint16_t)ps::numJoints;
	}

	uint16_t BasePhysics::GetNumContacts()const {
		return (uint16_t)ps::numContacts;
	}

	bool BasePhysics::GetContactsVec(uint16_t body_index, vector<uint16_t>& contacts)const {
		bool ret = false;
		for (uint16_t i = 0; i < GetNumContacts(); i++) {
			auto& TgtPair = getPfxContactManifold(i);
			if (TgtPair.getRigidBodyIdA() == body_index) {
				contacts.push_back(TgtPair.getRigidBodyIdB());
				ret = true;
			}
			if (TgtPair.getRigidBodyIdB() == body_index) {
				contacts.push_back(TgtPair.getRigidBodyIdA());
				ret = true;
			}
		}
		return ret;
	}

	bool BasePhysics::GetContactsSet(uint16_t body_index, set<uint16_t>& contacts)const {
		bool ret = false;
		for (uint16_t i = 0; i < GetNumContacts(); i++) {
			auto& TgtPair = getPfxContactManifold(i);
			if (TgtPair.getRigidBodyIdA() == body_index) {
				contacts.insert(TgtPair.getRigidBodyIdB());
				ret = true;
			}
			if (TgtPair.getRigidBodyIdB() == body_index) {
				contacts.insert(TgtPair.getRigidBodyIdA());
				ret = true;
			}
		}
		return ret;
	}

	bsm::Mat3x3 BasePhysics::CalcInertiaBox(const bsm::Vec3& halfExtent, float mass) {
		// Box
		return (bsm::Mat3x3)pfxCalcInertiaBox((PfxVector3)halfExtent,(PfxFloat)mass);

	}
	bsm::Mat3x3 BasePhysics::CalcInertiaSphere(float radius, float mass) {
		// Sphere
		return (bsm::Mat3x3)pfxCalcInertiaSphere((PfxFloat)radius, (PfxFloat)mass);

	}
	bsm::Mat3x3 BasePhysics::CalcInertiaCylinderX(float halfLength, float radius, float mass) {
		// Cylinder
		return (bsm::Mat3x3)pfxCalcInertiaCylinderX((PfxFloat)halfLength, (PfxFloat)radius, (PfxFloat)mass);

	}
	bsm::Mat3x3 BasePhysics::CalcInertiaCylinderY(float halfLength, float radius, float mass) {
		return (bsm::Mat3x3)pfxCalcInertiaCylinderY((PfxFloat)halfLength, (PfxFloat)radius, (PfxFloat)mass);

	}
	bsm::Mat3x3 BasePhysics::CalcInertiaCylinderZ(float halfLength, float radius, float mass) {
		return (bsm::Mat3x3)pfxCalcInertiaCylinderZ((PfxFloat)halfLength, (PfxFloat)radius, (PfxFloat)mass);
	}

	const PfxShape& BasePhysics::getPfxShape(uint16_t body_index, uint16_t shape_index) const {
		return ps::collidables[body_index].getShape(shape_index);
	}
	PfxShape& BasePhysics::getPfxShape(uint16_t body_index, uint16_t shape_index){
		return ps::collidables[body_index].getShape(shape_index);
	}

	const PfxRigidState& BasePhysics::getPfxRigidState(uint16_t body_index) const {
		return ps::states[body_index];
	}
	PfxRigidState& BasePhysics::getPfxRigidState(uint16_t body_index){
		return ps::states[body_index];
	}
	const PfxRigidBody& BasePhysics::getPfxRigidBody(uint16_t body_index) const {
		return ps::bodies[body_index];
	}
	PfxRigidBody& BasePhysics::getPfxRigidBody(uint16_t body_index){
		return ps::bodies[body_index];
	}

	const PfxCollidable& BasePhysics::getPfxCollidable(uint16_t body_index) const {
		return ps::collidables[body_index];
	}
	PfxCollidable& BasePhysics::getPfxCollidable(uint16_t body_index){
		return ps::collidables[body_index];
	}

	const PfxSolverBody& BasePhysics::getPfxSolverBody(uint16_t body_index) const {
		return ps::solverBodies[body_index];
	}
	PfxSolverBody& BasePhysics::getPfxSolverBody(uint16_t body_index){
		return ps::solverBodies[body_index];
	}

	const PfxConstraintPair& BasePhysics::getPfxConstraintPair(uint16_t joint_index) const {
		return ps::jointPairs[joint_index];
	}
	PfxConstraintPair& BasePhysics::getPfxConstraintPair(uint16_t joint_index){
		return ps::jointPairs[joint_index];
	}

	const PfxJoint& BasePhysics::getPfxJoint(uint16_t joint_index) const {
		return ps::joints[joint_index];
	}
	PfxJoint& BasePhysics::getPfxJoint(uint16_t joint_index){
		return ps::joints[joint_index];
	}

	const PfxBroadphasePair& BasePhysics::getNowPfxBroadphasePair(uint16_t contact_index) const {
		return ps::pairsBuff[ps::pairSwap][contact_index];
	}
	PfxBroadphasePair& BasePhysics::getNowPfxBroadphasePair(uint16_t contact_index){
		return ps::pairsBuff[ps::pairSwap][contact_index];
	}

	const PfxBroadphasePair& BasePhysics::getPrevPfxBroadphasePair(uint16_t contact_index) const {
		return ps::pairsBuff[1 - ps::pairSwap][contact_index];
	}
	PfxBroadphasePair& BasePhysics::getPrevPfxBroadphasePair(uint16_t contact_index) {
		return ps::pairsBuff[1 - ps::pairSwap][contact_index];
	}


	const PfxContactManifold& BasePhysics::getPfxContactManifold(uint16_t contact_index) const {
		return ps::contacts[contact_index];
	}
	PfxContactManifold& BasePhysics::getPfxContactManifold(uint16_t contact_index){
		return ps::contacts[contact_index];
	}

	void BasePhysics::Reset() {
		ps::numRigidBodies = 0;
		ps::numJoints = 0;
		ps::numContacts = 0;
		ps::pairSwap = 0;
		ps::numPairs[0] = 0;
		ps::numPairs[1] = 0;
		ps::numContactIdPool = 0;
		::ZeroMemory(ps::states, sizeof(PfxRigidState) * NUM_RIGIDBODIES);
		::ZeroMemory(ps::bodies, sizeof(PfxRigidBody) * NUM_RIGIDBODIES);
		::ZeroMemory(ps::collidables, sizeof(PfxCollidable) * NUM_RIGIDBODIES);
		::ZeroMemory(ps::solverBodies, sizeof(PfxSolverBody) * NUM_RIGIDBODIES);
		::ZeroMemory(ps::proxies, sizeof(PfxBroadphaseProxy) * 6 * NUM_RIGIDBODIES);
		::ZeroMemory(ps::jointPairs, sizeof(PfxConstraintPair) * NUM_JOINTS);
		::ZeroMemory(ps::joints, sizeof(PfxJoint) * NUM_JOINTS);
		::ZeroMemory(ps::pairsBuff, sizeof(PfxBroadphasePair) * NUM_CONTACTS * 2);
		::ZeroMemory(ps::contacts, sizeof(PfxContactManifold) * NUM_CONTACTS);
		::ZeroMemory(ps::contactIdPool, sizeof(PfxUInt32) * NUM_CONTACTS);
		::ZeroMemory(ps::poolBuff, sizeof(ps::poolBuff));
		//合成用shape
		::ZeroMemory(ps::combinedShapes, sizeof(PfxShape) * NUM_KEEP_SHAPES);
		ps::numCombinedShapes = 0;

	}



	void BasePhysics::Update() {
		for (int i = 1; i<ps::numRigidBodies; i++) {
			pfxApplyExternalForce(ps::states[i], ps::bodies[i], ps::bodies[i].getMass()*PfxVector3(0.0f, -9.8f, 0.0f), PfxVector3(0.0f), ps::timeStep);
		}
		ps::broadphase();
		ps::collision();
		ps::constraintSolver();
		ps::sleepOrWakeup();
		ps::integrate();
		ps::frame++;
//		if (keyframeId >= 0) updateKeyframe();
//		if (triggerId >= 0) updateTrigger();
	}

}
//end basecross
