/*!
@file BasePhysics.cpp
@brief �����v�Z�C���^�[�t�@�C�X����
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"


namespace basecross {

	using namespace sce::PhysicsEffects;

#define NUM_RIGIDBODIES 500
#define NUM_JOINTS    500
#define NUM_CONTACTS  4000

	namespace ps {

		const float timeStep = 0.016f;
		const float separateBias = 0.1f;
		int iteration = 8;

		//J ���[���h�T�C�Y
		//E World size
		PfxVector3 worldCenter(0.0f);
		PfxVector3 worldExtent(500.0f);

		//J ����
		//E Rigid body
		PfxRigidState states[NUM_RIGIDBODIES];
		PfxRigidBody  bodies[NUM_RIGIDBODIES];
		PfxCollidable collidables[NUM_RIGIDBODIES];
		PfxSolverBody solverBodies[NUM_RIGIDBODIES];
		int numRigidBodies = 0;
		int numProxiesInWorld = 0;

		//J �v���L�V
		//E Proxies
		PfxBroadphaseProxy proxies[6][NUM_RIGIDBODIES]; // shared by simulation and raycast

		//J �W���C���g
		//E Joint
		PfxConstraintPair jointPairs[NUM_JOINTS];
		PfxJoint joints[NUM_JOINTS];
		int numJoints = 0;

		//J �y�A
		//E Pairs
		unsigned int pairSwap;
		unsigned int numPairs[2];
		PfxBroadphasePair pairsBuff[2][NUM_CONTACTS];

		//J �R���^�N�g
		//E Contacts
		PfxContactManifold contacts[NUM_CONTACTS];
		int numContacts;

		PfxUInt32 contactIdPool[NUM_CONTACTS];
		int numContactIdPool;

		//J �V�~�����[�V�����A�C�����h
		//E Island generation
		PfxIsland *island = NULL;
		PfxUInt8 SCE_PFX_ALIGNED(16) islandBuff[32 * NUM_RIGIDBODIES]; // Island buffer should be 32 * the number of rigid bodies.

		//J �X���[�v����
		//E Sleep control
		/*
		A sleeping object wakes up, when
		* a new pair related to this rigid body is created
		* a pair releated to this rigid body is removed
		* a rigid body's velocity or position are updated
		*/

		//J �X���[�v�ɓ���J�E���g
		//E Count to enter sleeping
		const PfxUInt32 sleepCount = 180;

		//J ���x��臒l�ȉ��Ȃ�΃X���[�v�J�E���g������
		//E If velocity is under the following value, sleep count is increased.
		const PfxFloat sleepVelocity = 0.3f;

		//J �ꎞ�o�b�t�@
		//E Temporary buffers
#define POOL_BYTES (5*1024*1024)
		unsigned char SCE_PFX_ALIGNED(128) poolBuff[POOL_BYTES];

		//J �ꎞ�o�b�t�@�p�X�^�b�N�A���P�[�^
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

			//J ���̂��ł����U���Ă��鎲��������
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

			//J �u���[�h�t�F�[�Y�v���L�V�̍X�V

			numProxiesInWorld = 0;

			//E Create broadpahse proxies
			{
				//J ���C�L���X�g�Ƌ��p���邽�߁A�S�Ă̎��ɑ΂���v���L�V�z����쐬����
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

			//J �����y�A�T��
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

				//J �����y�A����
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

				//J �p���y�A�̃R���^�N�g���v�[���ɖ߂�
				//E Put removed contacts into the contact pool
				for (PfxUInt32 i = 0; i<numOutRemovePairs; i++) {
					contactIdPool[numContactIdPool++] = pfxGetContactId(outRemovePairs[i]);

					//J �Q�Ă鍄�̂��N����
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

				//J �V�K�y�A�̃R���^�N�g�̃����N�Ə�����
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

					//J �Q�Ă鍄�̂��N����
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

				//J �V�K�y�A�ƈێ��y�A������
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

			//J ���t���b�V��
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

			//J �Փˌ��o
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

			//J �A�C�����h����
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

				//J �W���C���g���̃y�A��ǉ�
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
					//����i�͋󂢂Ă���
					reservedConvexMeshes.insert(i);
					return i;
				}
			}
			//�S�����܂��Ă���
			throw BaseException(
				L"����ȏ�ʌ^���b�V���͑��₹�܂���",
				L"size >= NUM_RIGIDBODIES",
				L"ps::getNextConvexMesheIndex()"
			);
		}
		void rereaseConvexMesheIndex(uint32_t index) {
			pfxReleaseConvexMesh(convexMeshes[index]);
			::ZeroMemory(&convexMeshes[index], sizeof(PfxConvexMesh));
			reservedConvexMeshes.erase(index);
		}
	}

//	using namespace basecross::ps;
	//--------------------------------------------------------------------------------------
	///	�����I�u�W�F�N�g�̐e
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
	//	�{�b�N�XImpl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PhysicsBox::Impl {
		//�������p�����[�^
		PsBoxParam m_PsBoxParam;
		Impl(const PsBoxParam& param):
			m_PsBoxParam(param)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	�{�b�N�X�����I�u�W�F�N�g
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
		ps::bodies[m_Index].setInertia(pfxCalcInertiaBox(
			(PfxVector3)pImpl->m_PsBoxParam.m_HalfSize, 
			(PfxFloat)pImpl->m_PsBoxParam.m_Mass)
		);
		SetParamStatus(pImpl->m_PsBoxParam);
	}

	const PsBoxParam& PhysicsBox::GetParam() const {
		return pImpl->m_PsBoxParam;
	}


	//--------------------------------------------------------------------------------------
	//	����Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PhysicsSphere::Impl {
		//�������p�����[�^
		PsSphereParam m_PsSphereParam;
		Impl(const PsSphereParam& param) :
			m_PsSphereParam(param)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	///	���̕����I�u�W�F�N�g
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
		ps::bodies[m_Index].setInertia(pfxCalcInertiaSphere(
			(PfxFloat)pImpl->m_PsSphereParam.m_Radius,
			(PfxFloat)pImpl->m_PsSphereParam.m_Mass)
		);
		SetParamStatus(pImpl->m_PsSphereParam);
	}

	const PsSphereParam& PhysicsSphere::GetParam() const {
		return pImpl->m_PsSphereParam;
	}


	//--------------------------------------------------------------------------------------
	//	�J�v�Z��Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PhysicsCapsule::Impl {
		//�������p�����[�^
		PsCapsuleParam m_PsCapsuleParam;
		Impl(const PsCapsuleParam& param) :
			m_PsCapsuleParam(param)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	�J�v�Z�������I�u�W�F�N�g
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
		ps::bodies[m_Index].setInertia(
			pfxCalcInertiaCylinderX(
				(PfxFloat)pImpl->m_PsCapsuleParam.m_HalfLen + (PfxFloat)pImpl->m_PsCapsuleParam.m_Radius,
				(PfxFloat)pImpl->m_PsCapsuleParam.m_Radius,
				(PfxFloat)pImpl->m_PsCapsuleParam.m_Mass
			)
		);
		SetParamStatus(pImpl->m_PsCapsuleParam);
	}

	const PsCapsuleParam& PhysicsCapsule::GetParam() const {
		return pImpl->m_PsCapsuleParam;
	}

	//--------------------------------------------------------------------------------------
	//	�V�����_�[Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PhysicsCylinder::Impl {
		//�������p�����[�^
		PsCylinderParam m_PsCylinderParam;
		Impl(const PsCylinderParam& param) :
			m_PsCylinderParam(param)
		{}
		~Impl() {}
	};


	//--------------------------------------------------------------------------------------
	///	�V�����_�[�����I�u�W�F�N�g
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

		ps::bodies[m_Index].setInertia(
			pfxCalcInertiaCylinderX(
			(PfxFloat)pImpl->m_PsCylinderParam.m_HalfLen,
				(PfxFloat)pImpl->m_PsCylinderParam.m_Radius,
				(PfxFloat)pImpl->m_PsCylinderParam.m_Mass
			)
		);
		SetParamStatus(pImpl->m_PsCylinderParam);
	}

	const PsCylinderParam& PhysicsCylinder::GetParam() const {
		return pImpl->m_PsCylinderParam;
	}


	//--------------------------------------------------------------------------------------
	//	PsConvexMeshResource�@Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PsConvexMeshResource::Impl {
		uint32_t m_Index;
		float m_Radius;
		//�o�b�N�A�b�v�p�̒��_(VertexPositionNormalTexture)�ƃC���f�b�N�X
		vector<VertexPositionNormalTexture> m_Vertices;
		vector<uint16_t> m_Indices;
		Impl(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices, float radius):
			m_Index(0),
			m_Radius(radius)
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
						L"ConvexMeshResource�̍쐬�Ɏ��s���܂����B",
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
	///	ConvexMesh���\�[�X
	//--------------------------------------------------------------------------------------
	PsConvexMeshResource::PsConvexMeshResource(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices,
		float radius):
		BaseResource(),
		pImpl(new Impl(vertices, indices, radius))
	{}
	PsConvexMeshResource::~PsConvexMeshResource() {}
	uint32_t PsConvexMeshResource::GetMeshIndex() const {
		return pImpl->m_Index;
	}
	float PsConvexMeshResource::GetRadius() const {
		return pImpl->m_Radius;
	}

	const vector<VertexPositionNormalTexture>& PsConvexMeshResource::GetVertices() const {
		return pImpl->m_Vertices;
	}

	const vector<uint16_t>& PsConvexMeshResource::GetIndices() const {
		return pImpl->m_Indices;
	}




	//--------------------------------------------------------------------------------------
	//	PhysicsConvex�@Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct PhysicsConvex::Impl {
		//�������p�����[�^
		PsConvexParam m_PsConvexParam;
		Impl(const PsConvexParam& param) :
			m_PsConvexParam(param)
		{}
		~Impl() {
		}
	};



	//--------------------------------------------------------------------------------------
	///	ConvexMesh�����I�u�W�F�N�g(���_�w��̃I�u�W�F�N�g)
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
				L"ConvexMeshResource��������܂���",
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
		//�������[�����g�͋��̂ɂȂ炤
		ps::bodies[m_Index].setInertia(pfxCalcInertiaSphere(
			(PfxFloat)pImpl->m_PsConvexParam.m_ConvexMeshResource->GetRadius(),
			(PfxFloat)pImpl->m_PsConvexParam.m_Mass)
		);
		SetParamStatus(pImpl->m_PsConvexParam);
	}

	const PsConvexParam& PhysicsConvex::GetParam() const {
		return pImpl->m_PsConvexParam;
	}


	//--------------------------------------------------------------------------------------
	///	�����v�Z�p�̃C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	BasePhysics::BasePhysics()
	{
	}
	BasePhysics::~BasePhysics() {}

	shared_ptr<PhysicsBox> BasePhysics::AddSingleBox(const PsBoxParam& param, uint16_t index) {
		if (ps::numRigidBodies >= NUM_RIGIDBODIES) {
			throw BaseException(
				L"����ȏ㕨���I�u�W�F�N�g�𑝂₹�܂���",
				L"if (ps::numRigidBodies >= ps::NUM_RIGIDBODIES)",
				L"BasePhysics::AddSingleBox()"
			);
		}
		if (index >= ps::numRigidBodies) {
			index = ps::numRigidBodies++;
		}
		return ObjectFactory::Create<PhysicsBox>(param, index);
	}


	shared_ptr<PhysicsSphere> BasePhysics::AddSingleSphere(const PsSphereParam& param, uint16_t index) {
		if (ps::numRigidBodies >= NUM_RIGIDBODIES) {
			throw BaseException(
				L"����ȏ㕨���I�u�W�F�N�g�𑝂₹�܂���",
				L"if (ps::numRigidBodies >= ps::NUM_RIGIDBODIES)",
				L"BasePhysics::AddSingleSphere()"
			);
		}
		if (index >= ps::numRigidBodies) {
			index = ps::numRigidBodies++;
		}
		return ObjectFactory::Create<PhysicsSphere>(param, index);
	}

	shared_ptr<PhysicsCapsule> BasePhysics::AddSingleCapsule(const PsCapsuleParam& param, uint16_t index) {
		if (ps::numRigidBodies >= NUM_RIGIDBODIES) {
			throw BaseException(
				L"����ȏ㕨���I�u�W�F�N�g�𑝂₹�܂���",
				L"if (ps::numRigidBodies >= ps::NUM_RIGIDBODIES)",
				L"BasePhysics::AddSingleCapsule()"
			);
		}
		if (index >= ps::numRigidBodies) {
			index = ps::numRigidBodies++;
		}
		return ObjectFactory::Create<PhysicsCapsule>(param, index);
	}

	shared_ptr<PhysicsCylinder> BasePhysics::AddSingleCylinder(const PsCylinderParam& param, uint16_t index) {
		if (ps::numRigidBodies >= NUM_RIGIDBODIES) {
			throw BaseException(
				L"����ȏ㕨���I�u�W�F�N�g�𑝂₹�܂���",
				L"if (ps::numRigidBodies >= ps::NUM_RIGIDBODIES)",
				L"BasePhysics::AddSingleCylinder()"
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
				L"����ȏ㕨���I�u�W�F�N�g�𑝂₹�܂���",
				L"if (ps::numRigidBodies >= ps::NUM_RIGIDBODIES)",
				L"BasePhysics::AddConvex()"
			);
		}
		if (index >= ps::numRigidBodies) {
			index = ps::numRigidBodies++;
		}
		return ObjectFactory::Create<PhysicsConvex>(param, index);
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
		//�t�H�[�X��������
		pfxApplyExternalForce(
			ps::states[body_index], ps::bodies[body_index],
			ps::bodies[body_index].getMass() * (PfxVector3)st.m_Force,
			ps::bodies[body_index].getMass() * (PfxVector3)st.m_Torque,
			ps::timeStep
		);
	}

	void BasePhysics::WakeUpBody(uint16_t body_index) {
		ps::states[body_index].wakeup();
	}

	void BasePhysics::SetBodyPosition(uint16_t body_index, const bsm::Vec3& pos) {
		if (ps::states[body_index].getMotionType() != ePfxMotionType::kPfxMotionTypeTrigger) {
			throw BaseException(
				L"���[�V�����^�C�v��MotionTypeTrigger�ȊO�ł͐ݒ�ł��܂���",
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