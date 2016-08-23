/*!
@file GameObject.cpp
@brief コンポーネント、ゲームオブジェクト実体
*/

#include "stdafx.h"
#include "../Project.h"

namespace basecross{


	//--------------------------------------------------------------------------------------
	///	ゲームオブジェクトクラス
	//--------------------------------------------------------------------------------------
	shared_ptr<Component> GameObject::SearchComponent(type_index TypeIndex)const{
		auto it = m_CompMap.find(TypeIndex);
		if (it != m_CompMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	void GameObject::AddMakedComponent(type_index TypeIndex, const shared_ptr<Component>& Ptr){
		if (!SearchComponent(TypeIndex)){
			//そのコンポーネントがまだなければ新規登録
			m_CompOrder.push_back(TypeIndex);
		}
		//mapに追加もしくは更新
		m_CompMap[TypeIndex] = Ptr;
		Ptr->SetGameObject(GetThis<GameObject>());
	}

	GameObject::GameObject(const shared_ptr<Scene>& ScenePtr) :
		m_Scene(ScenePtr)
	{}

	GameObject::~GameObject(){}

	void GameObject::ComponentPreparation(){
		auto TMptr = GetComponent<Transform>();
		TMptr->SetToBefore();
	}


	void GameObject::ComponentUpdate(){
		auto TMptr = GetComponent<Transform>();
		auto GravityPtr = GetComponent<Gravity>(false);
		//マップを検証してUpdate
		auto it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			if (it2 != m_CompMap.end()) {
				//指定の型のコンポーネントが見つかった
				it2->second->OnUpdate();
			}
			it++;
		}
		if (GravityPtr) {
			//GravityPtrがあればUpdate()
			GravityPtr->OnUpdate();
		}
		//TransformのUpdate
		TMptr->OnUpdate();
	}

	void GameObject::ComponentDraw(){
		//GravityやTransformはDrawを呼ばない
		//マップを検証してUpdate
		auto it = m_CompOrder.begin();
		while (it != m_CompOrder.end()) {
			map<type_index, shared_ptr<Component> >::const_iterator it2;
			it2 = m_CompMap.find(*it);
			if (it2 != m_CompMap.end()) {
				//指定の型のコンポーネントが見つかった
				it2->second->OnDraw();
			}
			it++;
		}

	}




	//--------------------------------------------------------------------------------------
	///	コンポーネント親クラス
	//--------------------------------------------------------------------------------------
	Component::Component(const shared_ptr<GameObject>& GameObjectPtr) :
		m_GameObject(GameObjectPtr)
	{}

	Component::~Component(){}

	shared_ptr<GameObject> Component::GetGameObject() const{
		if (m_GameObject.expired()){
			throw BaseException(
				L"GameObjectは有効ではありません",
				L"if (pImpl->m_GameObject.expired())",
				L"Component::GetGameObject()"
				);
		}
		else{
			return m_GameObject.lock();
		}
	}

	void Component::SetGameObject(const shared_ptr<GameObject>& GameObjectPtr){
		m_GameObject = GameObjectPtr;
	}

	//--------------------------------------------------------------------------------------
	///	重力コンポーネント
	//--------------------------------------------------------------------------------------
	void Gravity::StartJump(const Vector3& StartVec, float EscapeSpan){
		auto Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
		SetGravityVelocity(StartVec);
		SetGravityDefault();
		//ジャンプして親オブジェクトボリュームから脱出できないとき対応
		Vector3 EscapeVec = StartVec;
		EscapeVec *= EscapeSpan;
		Position += EscapeVec;
		//位置を設定
		GetGameObject()->GetComponent<Transform>()->SetPosition(Position);
	}

	void Gravity::UpdateFromTime(float CalcTime){
		auto Position = GetGameObject()->GetComponent<Transform>()->GetPosition();
		//自由落下加速度を計算
		m_GravityVelocity += m_Gravity * CalcTime;
		Position += m_GravityVelocity * CalcTime;
		if (Position.y <= m_BaseY){
			Position.y = m_BaseY;
			SetGravityVelocityZero();
			SetGravityZero();
		}
		//位置を設定
		GetGameObject()->GetComponent<Transform>()->SetPosition(Position);
	}

	void Gravity::OnUpdate(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		UpdateFromTime(ElapsedTime);
	}

	//--------------------------------------------------------------------------------------
	///	PNTStaticDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	void PNTStaticDraw::OnDraw(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		auto RenderState = Dev->GetRenderState();

		auto GObject = GetGameObject();

		//行列の定義
		Matrix4X4 World, View, Proj;
		//ライティング
		Vector4 LightDir;
		auto PtrTrans = GObject->GetComponent<Transform>();
		World = PtrTrans->GetWorldMatrix();
		//転置する
		World.Transpose();
		GObject->GetScene()->GetViewProjMatrix(View, Proj);
		GObject->GetScene()->GetLightDir(LightDir);
		//ビュー行列の決定
		//転置する
		View.Transpose();
		//射影行列の決定
		//転置する
		Proj.Transpose();
		//コンスタントバッファの準備
		PNTStaticConstantBuffer sb;
		sb.World = World;
		sb.View = View;
		sb.Projection = Proj;
		sb.LightDir = LightDir;
		//ディフューズ
		sb.Diffuse = Color4(1.0f, 1.0f, 1.0f, 1.0f);
		//エミッシブ加算は行わない。
		sb.Emissive = Color4(0, 0, 0, 0);
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBPNTStatic::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//頂点バッファのセット
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pD3D11DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBPNTStatic::GetPtr()->GetBuffer();
		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//頂点シェーダに渡す
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		//ピクセルシェーダに渡す
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSPNTStatic::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSPNTStatic::GetPtr()->GetInputLayout());

		//ブレンドステート
		if (m_Trace){
			//透明処理
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetAlphaBlendEx(), nullptr, 0xffffffff);
		}
		else{
			//透明処理しない
			pD3D11DeviceContext->OMSetBlendState(RenderState->GetOpaque(), nullptr, 0xffffffff);
		}

		//デプスステンシルステート
		pD3D11DeviceContext->OMSetDepthStencilState(RenderState->GetDepthDefault(), 0);

		//テクスチャとサンプラーの設定
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureResource->GetShaderResourceView().GetAddressOf());
		ID3D11SamplerState* pSampler = RenderState->GetLinearClamp();
		pD3D11DeviceContext->PSSetSamplers(0, 1, &pSampler);

		if (m_Trace){
			//透明処理の場合は、ラスタライザステートを変更して2回描画
			//ラスタライザステート（裏面描画）
			pD3D11DeviceContext->RSSetState(RenderState->GetCullFront());
			//描画
			pD3D11DeviceContext->DrawIndexed(m_NumIndicis, 0, 0);
			//ラスタライザステート（表面描画）
			pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
			//描画
			pD3D11DeviceContext->DrawIndexed(m_NumIndicis, 0, 0);
		}
		else{
			//ラスタライザステート（表面描画）
			pD3D11DeviceContext->RSSetState(RenderState->GetCullBack());
			//描画
			pD3D11DeviceContext->DrawIndexed(m_NumIndicis, 0, 0);
		}
		//後始末
		Dev->InitializeStates();

	}



}
//end basecross
