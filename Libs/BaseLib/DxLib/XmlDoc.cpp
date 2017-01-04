/*!
@file XmlDoc.cpp
@brief XML関連クラス実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class XmlDocReader;
	//	用途: XMLクラス
	//--------------------------------------------------------------------------------------
	//構築と消滅
	XmlDocReader::XmlDocReader(const wstring& StrReadContext, bool IsMemory){
		try{
			HRESULT hr = m_pXMLDOMDocument.CreateInstance(CLSID_DOMDocument);
			if (FAILED(hr))
			{
				// 初期化失敗
				throw BaseException(
					L"XML初期化に失敗しました。",
					L"if (FAILED(hr))",
					L"XmlDocReader::XmlDoc()"
					);
			}
			m_pXMLDOMDocument->put_async(VARIANT_FALSE);
			VARIANT_BOOL varbResult;
			if (!IsMemory){
				//ファイルとして読み込み
				hr = m_pXMLDOMDocument->load(CComVariant(StrReadContext.c_str()), &varbResult);
				if (FAILED(hr) || !varbResult)
				{
					// 初期化失敗
					throw BaseException(
						L"XMLファイル読み込みに失敗しました。",
						L"if (FAILED(hr) || !varbResult)",
						L"XmlDocReader::XmlDoc()"
						);
				}
			}
			else{
				//文字列として読み込み
				hr = m_pXMLDOMDocument->loadXML(BSTR(StrReadContext.c_str()), &varbResult);
				if (FAILED(hr) || !varbResult)
				{
					// 初期化失敗
					throw BaseException(
						L"XMLデータ読み込みに失敗しました。",
						L"if (FAILED(hr) || !varbResult)",
						L"XmlDocReader::XmlDoc()"
						);
				}
			}
		}
		catch (...){
			throw;
		}
	}
	XmlDocReader::~XmlDocReader(){
	}

	//操作
	//XPathによる取得
	IXMLDOMNodePtr XmlDocReader::GetSelectSingleNode(const CComBSTR& Query) const{
		if (Query.Length() <= 0){
			// クエリ失敗
			throw BaseException(
				L"XPathクエリが空です。",
				L"if (Query.Length() <= 0)",
				L"XmlDocReader::GetSelectSingleNode()"
				);
		}
		// ノードの取得
		IXMLDOMNodePtr pXMLDOMNode = nullptr;
		HRESULT hr = m_pXMLDOMDocument->selectSingleNode(Query, &pXMLDOMNode);
		if (FAILED(hr))
		{
			wstring errstr = Query;
			// クエリ失敗
			throw BaseException(
				L"ノードの取得に失敗しました",
				errstr,
				L"XmlDocReader::GetSelectSingleNode()"
				);
		}
		return pXMLDOMNode;
	}

	IXMLDOMNodeListPtr XmlDocReader::GetSelectNodes(const CComBSTR& Query)const{
		if (Query.Length() <= 0){
			// クエリ失敗
			throw BaseException(
				L"XPathクエリが空です。",
				L"if (Query.Length() <= 0)",
				L"XmlDocReader::GetSelectNodes()"
				);
		}
		// タグ名のノードリストの取得
		IXMLDOMNodeListPtr pXMLDOMNodeList = nullptr;
		HRESULT hr = m_pXMLDOMDocument->selectNodes(Query, &pXMLDOMNodeList);
		if (FAILED(hr))
		{
			wstring errstr = Query;
			// クエリ失敗
			throw BaseException(
				L"ノードリストの取得に失敗しました",
				errstr,
				L"XmlDocReader::GetSelectNodes()"
				);
		}
		return pXMLDOMNodeList;
	}

	//static関数群
	long XmlDocReader::GetLength(const IXMLDOMNodeListPtr& NodeList){
		long lCountNode = 0;
		HRESULT hr = NodeList->get_length(&lCountNode);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"ノード数の取得に失敗しました",
				L"NodeList->get_length()",
				L"XmlDocReader::GetLength()"
				);
		}
		return lCountNode;
	}
	IXMLDOMNodePtr XmlDocReader::GetItem(const IXMLDOMNodeListPtr& NodeList, long index){
		IXMLDOMNodePtr pXMLDOMNode = nullptr;
		HRESULT hr = NodeList->get_item(index, &pXMLDOMNode);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"ノードの取得に失敗しました",
				L"NodeList->get_item()",
				L"XmlDocReader::GetItem()"
				);
		}
		return pXMLDOMNode;

	}


	IXMLDOMNodeListPtr XmlDocReader::GetChildNodes(const IXMLDOMNodePtr& Node){
		IXMLDOMNodeListPtr NodeList = nullptr;
		HRESULT hr = Node->get_childNodes(&NodeList);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"ノードリストの取得に失敗しました",
				L"Node->get_childNodes()",
				L"XmlDocReader::GetChildNodes()"
				);
		}
		return NodeList;
	}
	wstring XmlDocReader::GetBaseName(const IXMLDOMNodePtr& Node){
		CComBSTR bstrText;
		HRESULT hr = Node->get_baseName(&bstrText);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"名前の取得に失敗しました",
				L"Node->get_baseName()",
				L"XmlDocReader::GetBaseName()"
				);
		}
		wstring ret = bstrText;
		return ret;
	}

	wstring XmlDocReader::GetText(const IXMLDOMNodePtr& Node){
		CComBSTR bstrText;
		HRESULT hr = Node->get_text(&bstrText);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"テキストの取得に失敗しました",
				L"Node->get_text()",
				L"XmlDocReader::GetText()"
				);
		}
		wstring ret = bstrText;
		return ret;
	}

	wstring XmlDocReader::GetXML(const IXMLDOMNodePtr& Node){
		CComBSTR bstrText;
		HRESULT hr = Node->get_xml(&bstrText);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"テキストの取得に失敗しました",
				L"Node->get_xml()",
				L"XmlDocReader::GetXML()"
				);
		}
		wstring ret = bstrText;
		return ret;
	}


	wstring XmlDocReader::GetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag){
		CComBSTR bstrText;
		IXMLDOMNamedNodeMap* pMap = nullptr;
		HRESULT hr = Node->get_attributes(&pMap);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"アトリビュートの取得に失敗しました",
				L"Node->get_attributes()",
				L"XmlDocReader::GetAttribute()"
				);
		}
		if (pMap){
			IXMLDOMNodePtr pXMLDOMNode = nullptr;
			pMap->getNamedItem(AttTag, &pXMLDOMNode);
			if (pXMLDOMNode){
				wstring ret = XmlDocReader::GetText(pXMLDOMNode);
				return ret;
			}
		}
		return wstring(L"");
	}

	IXMLDOMNodePtr XmlDocReader::GetSelectSingleNode(const IXMLDOMNodePtr& Node, const CComBSTR& Query){
		if (Query.Length() <= 0){
			// クエリ失敗
			throw BaseException(
				L"XPathクエリが空です。",
				L"if (Query.Length() <= 0)",
				L"XmlDocReader::GetSelectSingleNode()"
				);
		}
		IXMLDOMNodePtr pXMLDOMNode = nullptr;
		HRESULT hr = Node->selectSingleNode(Query, &pXMLDOMNode);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"ノードの取得に失敗しました",
				L"Node->selectSingleNode()",
				L"XmlDocReader::GetSelectSingleNode()"
				);
		}
		return pXMLDOMNode;
	}

	IXMLDOMNodeListPtr XmlDocReader::GetSelectNodes(const IXMLDOMNodePtr& Node, const CComBSTR& Query){
		if (Query.Length() <= 0){
			// クエリ失敗
			throw BaseException(
				L"XPathクエリが空です。",
				L"if (Query.Length() <= 0)",
				L"XmlDocReader::GetSelectNodes()"
				);
		}
		IXMLDOMNodeListPtr pXMLDOMNodeList = nullptr;
		HRESULT hr = Node->selectNodes(Query, &pXMLDOMNodeList);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"ノードリストの取得に失敗しました",
				L"Node->selectNodes()",
				L"XmlDocReader::GetSelectNodes()"
				);
		}
		return pXMLDOMNodeList;
	}

	//--------------------------------------------------------------------------------------
	//	struct XmlDoc::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct XmlDoc::Impl{
		IXMLDOMDocumentPtr m_pXMLDOMDocument;
		Impl(){}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class XmlDoc;
	//	用途: XMLクラス(スタティックを使わないバージョン)
	//--------------------------------------------------------------------------------------
	void XmlDoc::CheckXmlDoc()const{
		if (!pImpl->m_pXMLDOMDocument){
			// 初期化失敗
			throw BaseException(
				L"XMLが初期化されてません。",
				L"if (!pImpl->m_pXMLDOMDocument)",
				L"XmlDoc::CheckXmlDoc()"
				);
		}
	}

	//構築と消滅
	XmlDoc::XmlDoc(const wstring& Filename) :
		pImpl(new Impl){
		try{
			HRESULT hr = pImpl->m_pXMLDOMDocument.CreateInstance(CLSID_DOMDocument);
			if (FAILED(hr))
			{
				// 初期化失敗
				throw BaseException(
					L"XML初期化に失敗しました。",
					L"if (FAILED(hr))",
					L"XmlDoc::XmlDoc()"
					);
			}
			pImpl->m_pXMLDOMDocument->put_async(VARIANT_FALSE);
			VARIANT_BOOL varbResult;
			hr = pImpl->m_pXMLDOMDocument->load(CComVariant(Filename.c_str()), &varbResult);
			if (FAILED(hr) || !varbResult)
			{
				// 初期化失敗
				throw BaseException(
					L"XML読み込みに失敗しました。",
					L"if (FAILED(hr) || !varbResult)",
					L"XmlDoc::XmlDoc()"
					);
			}
		}
		catch (...){
			throw;
		}
	}
	XmlDoc::~XmlDoc(){}

	//操作
	void XmlDoc::Save(const wstring& Filename, bool Exp){
		try{
			if (!pImpl->m_pXMLDOMDocument || Filename == L""){
				if (Exp){
					//例外派生
					throw BaseException(
						L"XML保存に失敗しました。",
						Filename,
						L"XmlDoc::Save()"
						);

				}
				else{
					//例外発生せずにリターン
					return;
				}
			}
			HRESULT hr = pImpl->m_pXMLDOMDocument->save(CComVariant(Filename.c_str()));
			if (FAILED(hr) && Exp){
				//例外派生
				throw BaseException(
					L"XMLsaveに失敗しました。",
					Filename,
					L"XmlDoc::Save()"
					);
			}
		}
		catch (...){
			throw;
		}

	}
	IXMLDOMNodePtr XmlDoc::CreateNodeBase(const CComBSTR& NodeName){
		try{
			if (!pImpl->m_pXMLDOMDocument || NodeName.Length() <= 0){
				//例外派生
				wstring errstr = NodeName;
				throw BaseException(
					L"Nodeの作成に失敗しました。",
					errstr,
					L"XmlDoc::CreateNode()"
					);

			}
			CComVariant NodeType = NODE_ELEMENT;
			CComBSTR	NamespaceURI = "";
			IXMLDOMNodePtr NewChild;
			ThrowIfFailed(
				pImpl->m_pXMLDOMDocument->createNode(NodeType, NodeName, NamespaceURI, &NewChild),
				L"Nodeの作成に失敗しました",
				L"FAILED(m_pXMLDOMDocument->createNode(NodeType, NodeName, NamespaceURI, &NewChild))",
				L"XmlDoc::CreateNode()"
				);
			return NewChild;
		}
		catch (...){
			throw;
		}
	}
	IXMLDOMNodePtr XmlDoc::AddChildNodeBase(const IXMLDOMNodePtr& Node, const IXMLDOMNodePtr& NewChildNode){
		CheckXmlDoc();
		//追加されるノードと実際に配置されるノードは違う
		IXMLDOMNodePtr RetNewChild;
		Node->appendChild(NewChildNode, &RetNewChild);
		//配置されたノードを返す
		return RetNewChild;
	}




	//XPathによる取得
	IXMLDOMNodePtr XmlDoc::GetSelectSingleNode(const CComBSTR& Query) const{
		CheckXmlDoc();
		if (Query.Length() <= 0){
			// クエリ失敗
			throw BaseException(
				L"XPathクエリが空です。",
				L"if (Query.Length() <= 0)",
				L"XmlDoc::GetSelectSingleNode()"
				);
		}
		// ノードの取得
		IXMLDOMNodePtr pXMLDOMNode = nullptr;
		HRESULT hr = pImpl->m_pXMLDOMDocument->selectSingleNode(Query, &pXMLDOMNode);
		if (FAILED(hr))
		{
			wstring errstr = Query;
			// クエリ失敗
			throw BaseException(
				L"ノードの取得に失敗しました",
				errstr,
				L"XmlDoc::GetSelectSingleNode()"
				);
		}
		return pXMLDOMNode;
	}
	IXMLDOMNodeListPtr XmlDoc::GetSelectNodes(const CComBSTR& Query) const{
		CheckXmlDoc();
		if (Query.Length() <= 0){
			// クエリ失敗
			throw BaseException(
				L"XPathクエリが空です。",
				L"if (Query.Length() <= 0)",
				L"XmlDoc::GetSelectNodes()"
				);
		}
		// タグ名のノードリストの取得
		IXMLDOMNodeListPtr pXMLDOMNodeList = nullptr;
		HRESULT hr = pImpl->m_pXMLDOMDocument->selectNodes(Query, &pXMLDOMNodeList);
		if (FAILED(hr))
		{
			wstring errstr = Query;
			// クエリ失敗
			throw BaseException(
				L"ノードリストの取得に失敗しました",
				errstr,
				L"XmlDoc::GetSelectNodes()"
				);
		}
		return pXMLDOMNodeList;
	}

	//Node等から取得するstatic関数
	long XmlDoc::GetLength(const IXMLDOMNodeListPtr& NodeList)const{
		CheckXmlDoc();
		long lCountNode = 0;
		HRESULT hr = NodeList->get_length(&lCountNode);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"ノード数の取得に失敗しました",
				L"NodeList->get_length()",
				L"XmlDoc::GetLength()"
				);
		}
		return lCountNode;
	}

	IXMLDOMNodePtr XmlDoc::GetItem(const IXMLDOMNodeListPtr& NodeList, long index)const{
		CheckXmlDoc();
		IXMLDOMNodePtr pXMLDOMNode = nullptr;
		HRESULT hr = NodeList->get_item(index, &pXMLDOMNode);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"ノードの取得に失敗しました",
				L"NodeList->get_item()",
				L"XmlDoc::GetItem()"
				);
		}
		return pXMLDOMNode;

	}
	IXMLDOMNodeListPtr XmlDoc::GetChildNodes(const IXMLDOMNodePtr& Node)const{
		CheckXmlDoc();
		IXMLDOMNodeListPtr NodeList = nullptr;
		HRESULT hr = Node->get_childNodes(&NodeList);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"ノードリストの取得に失敗しました",
				L"Node->get_childNodes()",
				L"XmlDoc::GetChildNodes()"
				);
		}
		return NodeList;
	}
	wstring XmlDoc::GetBaseName(const IXMLDOMNodePtr& Node) const{
		CheckXmlDoc();
		CComBSTR bstrText;
		HRESULT hr = Node->get_baseName(&bstrText);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"名前の取得に失敗しました",
				L"Node->get_baseName()",
				L"XmlDoc::GetBaseName()"
				);
		}
		wstring ret = bstrText;
		return ret;
	}
	wstring XmlDoc::GetText(const IXMLDOMNodePtr& Node)const{
		CheckXmlDoc();
		CComBSTR bstrText;
		HRESULT hr = Node->get_text(&bstrText);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"テキストの取得に失敗しました",
				L"Node->get_text()",
				L"XmlDoc::GetText()"
				);
		}
		wstring ret = bstrText;
		return ret;
	}
	void XmlDoc::SetText(const IXMLDOMNodePtr& Node, const CComBSTR& text){
		CheckXmlDoc();
		ThrowIfFailed(
			Node->put_text(text),
			L"テキストの設定に失敗しました",
			L"Node->put_text()",
			L"XmlDoc::SetText()"
			);
	}

	//ノード内XMLへのアクセスはリードオンリー
	wstring XmlDoc::GetXML(const IXMLDOMNodePtr& Node)const{
		CheckXmlDoc();
		CComBSTR bstrText;
		HRESULT hr = Node->get_xml(&bstrText);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"テキストの取得に失敗しました",
				L"Node->get_xml()",
				L"XmlDoc::GetXML()"
				);
		}
		wstring ret = bstrText;
		return ret;
	}

	wstring XmlDoc::GetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag) const{
		CheckXmlDoc();
		CComBSTR bstrText;
		IXMLDOMNamedNodeMap* pMap = nullptr;
		HRESULT hr = Node->get_attributes(&pMap);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"アトリビュートの取得に失敗しました",
				L"Node->get_attributes()",
				L"XmlDoc::GetAttribute()"
				);
		}
		if (pMap){
			IXMLDOMNodePtr pXMLDOMNode = nullptr;
			pMap->getNamedItem(AttTag, &pXMLDOMNode);
			if (pXMLDOMNode){
				wstring ret = XmlDocReader::GetText(pXMLDOMNode);
				return ret;
			}
		}
		return wstring(L"");
	}

	void XmlDoc::SetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag, const CComBSTR& AttValue){
		CheckXmlDoc();
		CComBSTR bstrText;
		IXMLDOMNamedNodeMapPtr MapPtr = nullptr;
		HRESULT hr = Node->get_attributes(&MapPtr);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"アトリビュートの取得に失敗しました",
				L"Node->get_attributes()",
				L"XmlDoc::SetAttribute()"
				);
		}
		if (MapPtr){
			//Mapがある
			IXMLDOMNodePtr pXMLDOMNode = nullptr;
			MapPtr->getNamedItem(AttTag, &pXMLDOMNode);
			if (pXMLDOMNode){
				//すでにそのアトリビュートがある
				SetText(pXMLDOMNode, AttValue);
			}
			else{
				//そのアトリビュートがない
				IXMLDOMAttributePtr AttPtr;
				pImpl->m_pXMLDOMDocument->createAttribute(AttTag, &AttPtr);
				IXMLDOMNodePtr pXMLDOMNode = nullptr;
				MapPtr->setNamedItem(AttPtr, &pXMLDOMNode);
				SetText(pXMLDOMNode, AttValue);
			}
		}
		else{
			//Mapがない
			int a = 0;
		}
	}


	IXMLDOMNodePtr XmlDoc::GetSelectSingleNode(const IXMLDOMNodePtr& Node, const CComBSTR& Query)const{
		CheckXmlDoc();
		if (Query.Length() <= 0){
			// クエリ失敗
			throw BaseException(
				L"XPathクエリが空です。",
				L"if (Query.Length() <= 0)",
				L"XmlDoc::GetSelectSingleNode()"
				);
		}
		IXMLDOMNodePtr pXMLDOMNode = nullptr;
		HRESULT hr = Node->selectSingleNode(Query, &pXMLDOMNode);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"ノードの取得に失敗しました",
				L"Node->selectSingleNode()",
				L"XmlDoc::GetSelectSingleNode()"
				);
		}
		return pXMLDOMNode;

	}
	IXMLDOMNodeListPtr XmlDoc::GetSelectNodes(const IXMLDOMNodePtr& Node, const CComBSTR& Query)const{
		CheckXmlDoc();
		if (Query.Length() <= 0){
			// クエリ失敗
			throw BaseException(
				L"XPathクエリが空です。",
				L"if (Query.Length() <= 0)",
				L"XmlDoc::GetSelectNodes()"
				);
		}
		IXMLDOMNodeListPtr pXMLDOMNodeList = nullptr;
		HRESULT hr = Node->selectNodes(Query, &pXMLDOMNodeList);
		if (FAILED(hr))
		{
			// 失敗
			throw BaseException(
				L"ノードリストの取得に失敗しました",
				L"Node->selectNodes()",
				L"XmlDoc::GetSelectNodes()"
				);
		}
		return pXMLDOMNodeList;
	}

	IXMLDOMNodePtr XmlDoc::AddChildNode(const IXMLDOMNodePtr& Node, const CComBSTR& NodeName){
		CheckXmlDoc();
		auto NewNode = CreateNodeBase(NodeName);
		return AddChildNodeBase(Node, NewNode);
	}



}

//endof  basecross

