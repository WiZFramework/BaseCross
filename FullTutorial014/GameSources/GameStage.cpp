/*!
@file GameStage.cpp
@brief �Q�[���X�e�[�W����
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	�Q�[���X�e�[�W�N���X����
	//--------------------------------------------------------------------------------------

	//�r���[�ƃ��C�g�̍쐬
	void GameStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<LookAtCamera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(Vec3(0.0f, 2.0f, -3.0f));
		PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();
		//�V���h�E�}�b�v�̃��C�g�̍����𒲐�(�X�e�[�W���L������)
		Shadowmap::SetLightHeight(50.0f);
	}


	//�����v�Z�I�u�W�F�N�g�̍쐬
	void GameStage::CreatePhysicsObjects() {
		//���̑�
		AddGameObject<FixedPsBox>(Vec3(50.0f, 1.0f, 50.0f), Quat(), Vec3(0.0f, -0.5f, 0.0f));

		Quat Qt1, Qt2, Qt3, Qt4;
		Qt1.rotationZ(2.0f);
		Qt2.rotationX(0.7f);
		Qt3.rotationZ(-2.0f);
		Qt4.rotationX(-0.7f);
		//�ォ��~���Ă���{�b�N�X
		AddGameObject<ActivePsBox>(Vec3(2.0f, 1.0f, 1.0f), Quat(), Vec3(2.0f, 1.0f, 4.0f));
		AddGameObject<ActivePsBox>(Vec3(1.0f, 1.0f, 2.0f), Qt1, Vec3(2.5f, 12.0f, 9.0f));
		AddGameObject<ActivePsBox>(Vec3(1.0f, 1.0f, 1.0f), Qt2, Vec3(-3.0f, 13.0f, 2.0f));
		AddGameObject<ActivePsBox>(Vec3(2.0f, 1.0f, 1.0f), Qt4, Vec3(2.5f, 15.0f, 10.0f));
		//�J�v�Z��
		AddGameObject<ActivePsCapsule>(1.0f, 1.0f, Qt2, Vec3(-2.0f, 4.0f, 3.5f));
		AddGameObject<ActivePsCapsule>(1.0f, 1.0f, Qt4, Vec3(2.0f, 10.0f, 2.0f));

		//�ォ��~���Ă��鋅��
		AddGameObject<ActivePsSphere>(1.0f, Quat(), Vec3(-0.5f, 7.0f, 7.0f));
		AddGameObject<ActivePsSphere>(1.0f, Quat(), Vec3(0.0f, 8.0f, 4.5f));

	}


	//�{�b�N�X�̍쐬
	void GameStage::CreateBox() {
		AddGameObject<Box>(Vec3(0.0f, 0.25f, 15.0f));
		AddGameObject<Box>(Vec3(-5.0f, 0.25f, 15.0f));
		AddGameObject<Box>(Vec3(5.0f, 0.25f, 15.0f));
		AddGameObject<Box>(Vec3(10.0f, 0.25f, 10.0f));
		AddGameObject<Box>(Vec3(-10.0f, 0.25f, 10.0f));
	}

	//�X�p�[�N�̍쐬
	void GameStage::CreateSpark() {
		auto MultiSparkPtr = AddGameObject<MultiSpark>();
		//���L�I�u�W�F�N�g�ɃX�p�[�N��o�^
		SetSharedGameObject(L"MultiSpark", MultiSparkPtr);
	}


	//�v���C���[�̍쐬
	void GameStage::CreatePlayer() {
		//�v���[���[�̍쐬
		auto PlayerPtr = AddGameObject<Player>(Vec3(0,0.125f,0));
		//�V�F�A�z��Ƀv���C���[��ǉ�
		SetSharedGameObject(L"Player", PlayerPtr);
		//�C�e�O���[�v�̍쐬
		CreateSharedObjectGroup(L"ShellGroup");

	}

	//BGM�̍Đ�
	void GameStage::PlayBGM() {
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		m_BGM = XAPtr->Start(L"Nanika", XAUDIO2_LOOP_INFINITE, 0.1f);
	}

	void GameStage::OnCreate() {
		try {
			//�����v�Z�L��
			SetPhysicsActive(true);
			//�r���[�ƃ��C�g�̍쐬
			CreateViewLight();
			//�����v�Z�I�u�W�F�N�g�̍쐬
			CreatePhysicsObjects();
			//�{�b�N�X�̍쐬
			CreateBox();
			//�X�p�[�N�̍쐬
			CreateSpark();
			//�v���[���[�̍쐬
			CreatePlayer();
			//BGM�̍Đ�
			PlayBGM();
		}
		catch (...) {
			throw;
		}
	}

	//�X�V
	void GameStage::OnUpdate() {
		//�L�[�{�[�h�i�}�E�X�j�̎擾
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		m_MousePoint = KeyState.m_MouseClientPoint;
		if (KeyState.m_bPressedKeyTbl[VK_LBUTTON]) {
			OnLButtonEnter();
		}
		else if (KeyState.m_bPushKeyTbl[VK_LBUTTON]) {
			OnLButtonDown();
		}
		else if (KeyState.m_bUpKeyTbl[VK_LBUTTON]) {
			OnLButtonUp();
		}
		else if (KeyState.m_bPressedKeyTbl[VK_RBUTTON]) {
			OnRButtonEnter();
		}
	}


	void GameStage::OnDestroy() {
		//BGM�̃X�g�b�v
		auto XAPtr = App::GetApp()->GetXAudio2Manager();
		XAPtr->Stop(m_BGM);
	}

	void GameStage::GetMouseRay(Vec3& Near, Vec3& Far) {
		Mat4x4 world, view, proj;
		world.affineTransformation(
			Vec3(1.0f, 1.0f, 1.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f),
			Vec3(0.0f, 0.0f, 0.0f)
		);
		auto PtrCamera = GetView()->GetTargetCamera();
		view = PtrCamera->GetViewMatrix();
		proj = PtrCamera->GetProjMatrix();
		auto viewport = GetView()->GetTargetViewport();

		Near = XMVector3Unproject(
			Vec3((float)m_MousePoint.x, (float)m_MousePoint.y, 0),
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			proj,
			view,
			world);

		Far = XMVector3Unproject(
			Vec3((float)m_MousePoint.x, (float)m_MousePoint.y, 1.0),
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			viewport.MinDepth,
			viewport.MaxDepth,
			proj,
			view,
			world);
	}

	void GameStage::SelectClear() {
		for (auto& v : GetGameObjectVec()) {
			auto PsPtr = dynamic_pointer_cast<ActivePsObject>(v);
			if (PsPtr) {
				PsPtr->SetSelected(false);
			}
		}
	}


	//�}�E�X�̍��{�^���������u��
	void GameStage::OnLButtonEnter() {
		SelectClear();
		auto PtrCamera = GetView()->GetTargetCamera();
		Vec3 Eye = PtrCamera->GetEye();

		vector<shared_ptr<ActivePsObject>> ObjVec;
		Vec3 NearPos, FarPos;
		GetMouseRay(NearPos, FarPos);
		for (auto& v : GetGameObjectVec()) {
			auto PsPtr = dynamic_pointer_cast<ActivePsObject>(v);
			if (PsPtr) {
				auto ColObb = PsPtr->GetComponent<CollisionObb>(false);
				auto ColSp = PsPtr->GetComponent<CollisionSphere>(false);
				auto ColCapsule = PsPtr->GetComponent<CollisionCapsule>(false);
				if (ColObb) {
					auto Obb = ColObb->GetObb();
					if (HitTest::SEGMENT_OBB(NearPos, FarPos, Obb)) {
						ObjVec.push_back(PsPtr);
					}
				}
				else if (ColSp) {
					auto Sp = ColSp->GetSphere();
					if (HitTest::SEGMENT_SPHERE(NearPos, FarPos, Sp)) {
						ObjVec.push_back(PsPtr);
					}
				}
				else if (ColCapsule) {
					auto Cap = ColCapsule->GetCapsule();
					if (HitTest::SEGMENT_CAPSULE(NearPos, FarPos, Cap)) {
						ObjVec.push_back(PsPtr);
					}
				}
			}
		}
		if (ObjVec.size() > 0) {
			float MinSpan = 1000.0f;
			shared_ptr<ActivePsObject> SelectObj = nullptr;
			for (auto& v : ObjVec) {
				float Span = length(v->GetComponent<Transform>()->GetPosition() - Eye);
				if (Span < MinSpan) {
					MinSpan = Span;
					SelectObj = v;
				}
			}
			if (SelectObj) {
				SelectObj->SetSelected(true);
			}
		}
	}

	//�}�E�X�̍��{�^����������
	void GameStage::OnLButtonDown() {

	}


	//�}�E�X�̍��{�^��(������)
	void GameStage::OnLButtonUp() {
		SelectClear();

	}

	//�}�E�X�̍��{�^��(������)
	void GameStage::OnRButtonEnter() {
		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
	}


	//--------------------------------------------------------------------------------------
	//	�^�C�g���X�e�[�W�N���X
	//--------------------------------------------------------------------------------------
	void TitleStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(Vec3(0.0f, 2.0f, -3.0f));
		PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();

	}

	//�X�v���C�g�̍쐬
	void TitleStage::CreateTitleSprite() {
		AddGameObject<TitleSprite>(L"MESSAGE_TX", false,
			Vec2(256.0f, 64.0f), Vec2(0.0f, 100.0f));

	}

	//������
	void TitleStage::OnCreate() {
		CreateViewLight();
		//�X�v���C�g�̍쐬
		CreateTitleSprite();
	}

	//�X�V
	void TitleStage::OnUpdate() {
		//�L�[�{�[�h�i�}�E�X�j�̎擾
		auto KeyState = App::GetApp()->GetInputDevice().GetKeyState();
		if (KeyState.m_bPressedKeyTbl[VK_RBUTTON]) {
			OnRButtonEnter();
		}
	}

	void TitleStage::OnRButtonEnter() {
		PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToGameStage");
	}


	//--------------------------------------------------------------------------------------
	//	�E�G�C�g�X�e�[�W�N���X
	//--------------------------------------------------------------------------------------

	bool WaitStage::m_Loaded = false;

	//���\�[�X���[�h�p�̃X���b�h�i�X�^�e�B�b�N�֐��j
	void WaitStage::LoadResourceFunc() {
		mutex m;
		m.lock();
		m_Loaded = false;
		m.unlock();

		wstring DataDir;
		//�T���v���̂��߃A�Z�b�g�f�B���N�g�����擾
		App::GetApp()->GetAssetsDirectory(DataDir);
		//�e�Q�[���͈ȉ��̂悤�Ƀf�[�^�f�B���N�g�����擾���ׂ�
		//App::GetApp()->GetDataDirectory(DataDir);
		wstring strTexture = DataDir + L"sky.jpg";
		App::GetApp()->RegisterTexture(L"SKY_TX", strTexture);
		strTexture = DataDir + L"trace.png";
		App::GetApp()->RegisterTexture(L"TRACE_TX", strTexture);
		strTexture = DataDir + L"spark.png";
		App::GetApp()->RegisterTexture(L"SPARK_TX", strTexture);
		strTexture = DataDir + L"StageMessage2.png";
		App::GetApp()->RegisterTexture(L"MESSAGE_TX", strTexture);
		//�T�E���h
		wstring CursorWav = DataDir + L"cursor.wav";
		App::GetApp()->RegisterWav(L"cursor", CursorWav);
		//BGM
		wstring strMusic = DataDir + L"nanika .wav";
		App::GetApp()->RegisterWav(L"Nanika", strMusic);

		m.lock();
		m_Loaded = true;
		m.unlock();

	}

	void WaitStage::CreateViewLight() {
		auto PtrView = CreateView<SingleView>();
		//�r���[�̃J�����̐ݒ�
		auto PtrCamera = ObjectFactory::Create<Camera>();
		PtrView->SetCamera(PtrCamera);
		PtrCamera->SetEye(Vec3(0.0f, 2.0f, -3.0f));
		PtrCamera->SetAt(Vec3(0.0f, 0.0f, 0.0f));
		//�}���`���C�g�̍쐬
		auto PtrMultiLight = CreateLight<MultiLight>();
		//�f�t�H���g�̃��C�e�B���O���w��
		PtrMultiLight->SetDefaultLighting();

	}

	//�X�v���C�g�̍쐬
	void WaitStage::CreateTitleSprite() {
		AddGameObject<AnimeSprite>(L"WAIT_TX", true,
			Vec2(256.0f, 64.0f), Vec2(0.0f, 100.0f));

	}

	//������
	void WaitStage::OnCreate() {
		wstring DataDir;
		//�T���v���̂��߃A�Z�b�g�f�B���N�g�����擾
		App::GetApp()->GetAssetsDirectory(DataDir);
		//���҂����������̃e�N�X�`���݂̂����œo�^
		wstring strTexture = DataDir + L"wait.png";
		App::GetApp()->RegisterTexture(L"WAIT_TX", strTexture);
		//���̃��\�[�X��ǂݍ��ރX���b�h�̃X�^�[�g
		std::thread LoadThread(LoadResourceFunc);
		//�I���܂ł͑҂��Ȃ�
		LoadThread.detach();

		CreateViewLight();
		//�X�v���C�g�̍쐬
		CreateTitleSprite();
	}

	//�X�V
	void WaitStage::OnUpdate() {
		if (m_Loaded) {
			//���\�[�X�̃��[�h���I��������^�C�g���X�e�[�W�Ɉڍs
			PostEvent(0.0f, GetThis<ObjectInterface>(), App::GetApp()->GetScene<Scene>(), L"ToTitleStage");
		}
	}


}
//end basecross