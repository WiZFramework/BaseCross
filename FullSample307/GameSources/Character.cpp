/*!
@file Character.cpp
@brief キャラクターなど実体
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross{


	//構築と破棄
	TilingPlate::TilingPlate(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode):
		GameObject(StagePtr)
	{
		try {
			auto ScaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");
			auto RotStr = XmlDocReader::GetAttribute(pNode, L"Rot");
			auto PosStr = XmlDocReader::GetAttribute(pNode, L"Pos");
			auto UPicStr = XmlDocReader::GetAttribute(pNode, L"UPic");
			auto VPicStr = XmlDocReader::GetAttribute(pNode, L"VPic");
			//トークン（カラム）の配列
			vector<wstring> Tokens;
			//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
			Tokens.clear();
			Util::WStrToTokenVector(Tokens, ScaleStr, L',');
			//各トークン（カラム）をスケール、回転、位置に読み込む
			m_Scale = Vec3(
				(float)_wtof(Tokens[0].c_str()),
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str())
			);
			Tokens.clear();
			Util::WStrToTokenVector(Tokens, RotStr, L',');
			Vec3 Rot;
			//回転は「XM_PIDIV2」の文字列になっている場合がある
			Rot.x = (Tokens[0] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[0].c_str());
			Rot.y = (Tokens[1] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[1].c_str());
			Rot.z = (Tokens[2] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[2].c_str());
			//プレートの回転の引数はクオータニオンになっているので変換
			m_Qt.rotationRollPitchYawFromVector(Rot);
			Tokens.clear();
			Util::WStrToTokenVector(Tokens, PosStr, L',');
			m_Position = Vec3(
				(float)_wtof(Tokens[0].c_str()),
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str())
			);
			m_UPic = (float)_wtof(UPicStr.c_str());
			m_VPic = (float)_wtof(VPicStr.c_str());
		}
		catch (...) {
			throw;
		}
	}

	TilingPlate::~TilingPlate() {

	}
	//初期化
	void TilingPlate::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetQuaternion(m_Qt);
		PtrTrans->SetPosition(m_Position);
		AddComponent<CollisionRect>();

		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateSquare(1.0f, vertices, indices);
		float UCount = m_Scale.x / m_UPic;
		float VCount = m_Scale.y / m_VPic;
		for (size_t i = 0; i < vertices.size(); i++) {
			if (vertices[i].textureCoordinate.x >= 1.0f) {
				vertices[i].textureCoordinate.x = UCount;
			}
			if (vertices[i].textureCoordinate.y >= 1.0f) {
				vertices[i].textureCoordinate.y = VCount;
			}
		}
		//描画コンポーネントの追加
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//自分に影が映りこむようにする
		PtrDraw->SetOwnShadowActive(true);
		//描画コンポーネントテクスチャの設定
		PtrDraw->SetTextureResource(L"SKY_TX");
		//タイリング設定
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);
	}

	TilingFixedBox::TilingFixedBox(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode) :
		GameObject(StagePtr)
	{
		auto ScaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");
		auto RotStr = XmlDocReader::GetAttribute(pNode, L"Rot");
		auto PosStr = XmlDocReader::GetAttribute(pNode, L"Pos");
		auto UPicStr = XmlDocReader::GetAttribute(pNode, L"UPic");
		auto VPicStr = XmlDocReader::GetAttribute(pNode, L"VPic");
		//トークン（カラム）の配列
		vector<wstring> Tokens;
		//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, ScaleStr, L',');
		//各トークン（カラム）をスケール、回転、位置に読み込む
		m_Scale = Vec3(
			(float)_wtof(Tokens[0].c_str()),
			(float)_wtof(Tokens[1].c_str()),
			(float)_wtof(Tokens[2].c_str())
		);
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, RotStr, L',');
		//回転は「XM_PIDIV2」の文字列になっている場合がある
		m_Rotation.x = (Tokens[0] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[0].c_str());
		m_Rotation.y = (Tokens[1] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[1].c_str());
		m_Rotation.z = (Tokens[2] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[2].c_str());
		Tokens.clear();
		Util::WStrToTokenVector(Tokens, PosStr, L',');
		m_Position = Vec3(
			(float)_wtof(Tokens[0].c_str()),
			(float)_wtof(Tokens[1].c_str()),
			(float)_wtof(Tokens[2].c_str())
		);
		m_UPic = (float)_wtof(UPicStr.c_str());
		m_VPic = (float)_wtof(VPicStr.c_str());
	}

	TilingFixedBox::~TilingFixedBox() {}
	//初期化
	void TilingFixedBox::OnCreate() {
		auto PtrTrans = GetComponent<Transform>();
		PtrTrans->SetScale(m_Scale);
		PtrTrans->SetRotation(m_Rotation);
		PtrTrans->SetPosition(m_Position);
		auto Coll = AddComponent<CollisionObb>();
		Coll->SetFixed(true);
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		MeshUtill::CreateCube(1.0f, vertices, indices);
		float UCount = m_Scale.x / m_UPic;
		float VCount = m_Scale.y / m_VPic;
		for (size_t i = 0; i < vertices.size(); i++) {
			if (vertices[i].textureCoordinate.x >= 1.0f) {
				vertices[i].textureCoordinate.x = UCount;
			}
			if (vertices[i].textureCoordinate.y >= 1.0f) {
				vertices[i].textureCoordinate.y = VCount;
			}
		}
		//描画コンポーネントの追加
		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		//描画コンポーネントに形状（メッシュ）を設定
		PtrDraw->CreateOriginalMesh(vertices, indices);
		PtrDraw->SetOriginalMeshUse(true);
		PtrDraw->SetFogEnabled(true);
		//自分に影が映りこむようにする
		PtrDraw->SetOwnShadowActive(true);
		//描画コンポーネントテクスチャの設定
		PtrDraw->SetTextureResource(L"WALL_TX");
		//タイリング設定
		PtrDraw->SetSamplerState(SamplerState::LinearWrap);

	}


	//--------------------------------------------------------------------------------------
	///	フレームボックス
	//--------------------------------------------------------------------------------------
	FrameBox::FrameBox(const shared_ptr<Stage>& StagePtr, IXMLDOMNodePtr pNode) :
		GameObject(StagePtr)
	{
		try {
			auto ScaleStr = XmlDocReader::GetAttribute(pNode, L"Scale");
			auto RotStr = XmlDocReader::GetAttribute(pNode, L"Rot");
			auto PosStr = XmlDocReader::GetAttribute(pNode, L"Pos");
			auto AddTagStr = XmlDocReader::GetAttribute(pNode, L"AddTag");
			//トークン（カラム）の配列
			vector<wstring> Tokens;
			//トークン（カラム）単位で文字列を抽出(L','をデリミタとして区分け)
			Tokens.clear();
			Util::WStrToTokenVector(Tokens, ScaleStr, L',');
			//各トークン（カラム）をスケール、回転、位置に読み込む
			m_Scale = Vec3(
				(float)_wtof(Tokens[0].c_str()),
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str())
			);
			Tokens.clear();
			Util::WStrToTokenVector(Tokens, RotStr, L',');
			//回転は「XM_PIDIV2」の文字列になっている場合がある
			m_Rotation.x = (Tokens[0] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[0].c_str());
			m_Rotation.y = (Tokens[1] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[1].c_str());
			m_Rotation.z = (Tokens[2] == L"XM_PIDIV2") ? XM_PIDIV2 : (float)_wtof(Tokens[2].c_str());
			Tokens.clear();
			Util::WStrToTokenVector(Tokens, PosStr, L',');
			m_Position = Vec3(
				(float)_wtof(Tokens[0].c_str()),
				(float)_wtof(Tokens[1].c_str()),
				(float)_wtof(Tokens[2].c_str())
			);

			if (AddTagStr == L"true") {
				AddTag(L"FrameBox");
			}
		}
		catch (...) {
			throw;
		}

	}
	FrameBox::~FrameBox() {}

	//初期化
	void FrameBox::OnCreate() {
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

		auto PtrDraw = AddComponent<BcPNTStaticDraw>();
		PtrDraw->SetFogEnabled(true);

		PtrDraw->SetMeshResource(L"DEFAULT_CUBE");
		PtrDraw->SetOwnShadowActive(true);
		PtrDraw->SetTextureResource(L"SKY_TX");

	}


}
//end basecross
