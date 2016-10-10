
/*!
@file XmlDoc.h
@brief XML関連クラス
@copyright Copyright (c) 2016 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once

#include "stdafx.h"

namespace basecross{

	//--------------------------------------------------------------------------------------
	///	XML読み込みオンリークラス
	//--------------------------------------------------------------------------------------
	class XmlDocReader {
	protected:
		IXMLDOMDocumentPtr m_pXMLDOMDocument;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	StrReadContext XMLコンテクスト
		@param[in]	IsMemory XMLコンテクストがメモリ上のテキストかどうか（そうでなければXMLファイル）
		*/
		//--------------------------------------------------------------------------------------
		XmlDocReader(const wstring& StrReadContext, bool IsMemory = false);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~XmlDocReader();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPathによる単体Nodeの取得
		@param[in]	Query	クエリ文字列
		@return	Nodeインターフェイス
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodePtr GetSelectSingleNode(const CComBSTR& Query) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPathによる複数Nodeの取得
		@param[in]	Query	クエリ文字列
		@return	複数Nodeインターフェイス
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodeListPtr GetSelectNodes(const CComBSTR& Query) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Nodeの数取得
		@param[in]	NodeList	複数Node
		@return	Nodeの数
		*/
		//--------------------------------------------------------------------------------------
		static long GetLength(const IXMLDOMNodeListPtr& NodeList);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックスからNodeの取得
		@param[in]	IXMLDOMNodeListPtr	複数Node
		@param[in]	index	インデックス
		@return	ターゲットのNode
		*/
		//--------------------------------------------------------------------------------------
		static IXMLDOMNodePtr GetItem(const IXMLDOMNodeListPtr& NodeList, long index);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	子供Nodeリストの取得
		@param[in]	Node	親Node
		@return	子供Nodeリスト
		*/
		//--------------------------------------------------------------------------------------
		static IXMLDOMNodeListPtr GetChildNodes(const IXMLDOMNodePtr& Node);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Node名の取得
		@param[in]	Node	ターゲットNode
		@return	Node名
		*/
		//--------------------------------------------------------------------------------------
		static wstring GetBaseName(const IXMLDOMNodePtr& Node);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Nodeのテキストの取得
		@param[in]	Node	ターゲットNode
		@return	Nodeのテキスト
		*/
		//--------------------------------------------------------------------------------------
		static wstring GetText(const IXMLDOMNodePtr& Node);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	NodeのXMLの取得
		@param[in]	Node	ターゲットNode
		@return	NodeのXML
		*/
		//--------------------------------------------------------------------------------------
		static wstring GetXML(const IXMLDOMNodePtr& Node);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Nodeのアトリビュートの取得
		@param[in]	Node	ターゲットNode
		@param[in]	AttTag	アトリビュート名
		@return	Nodeのアトリビュート
		*/
		//--------------------------------------------------------------------------------------
		static wstring GetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPathによる単体Nodeの取得
		@param[in]	Node	ターゲットNode
		@param[in]	Query	クエリ文字列
		@return	Node
		*/
		//--------------------------------------------------------------------------------------
		static IXMLDOMNodePtr GetSelectSingleNode(const IXMLDOMNodePtr& Node, const CComBSTR& Query);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPathによる複数Nodeの取得
		@param[in]	Node	ターゲットNode
		@param[in]	Query	クエリ文字列
		@return	複数Node
		*/
		//--------------------------------------------------------------------------------------
		static IXMLDOMNodeListPtr GetSelectNodes(const IXMLDOMNodePtr& Node, const CComBSTR& Query);
	};



	//--------------------------------------------------------------------------------------
	///	XMLクラス
	//--------------------------------------------------------------------------------------
	class XmlDoc{
		void CheckXmlDoc() const;
		IXMLDOMNodePtr CreateNodeBase(const CComBSTR& NodeName);
		IXMLDOMNodePtr AddChildNodeBase(const IXMLDOMNodePtr& Node, const IXMLDOMNodePtr& NewChildNode);
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	XMLファイル名
		*/
		//--------------------------------------------------------------------------------------
		XmlDoc(const wstring& Filename);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~XmlDoc();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	保存する
		@param[in]	Filename	保存するファイル名
		@param[in]	Exp	例外処理するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Save(const wstring& Filename, bool Exp = true);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPathによる単体Nodeの取得
		@param[in]	Query	クエリ文字列
		@return	Nodeインターフェイス
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodePtr GetSelectSingleNode(const CComBSTR& Query) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPathによる複数Nodeの取得
		@param[in]	Query	クエリ文字列
		@return	複数Nodeインターフェイス
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodeListPtr GetSelectNodes(const CComBSTR& Query) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Nodeの数取得
		@param[in]	NodeList	複数Node
		@return	Nodeの数
		*/
		//--------------------------------------------------------------------------------------
		long GetLength(const IXMLDOMNodeListPtr& NodeList)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	インデックスからNodeの取得
		@param[in]	IXMLDOMNodeListPtr	複数Node
		@param[in]	index	インデックス
		@return	ターゲットのNode
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodePtr GetItem(const IXMLDOMNodeListPtr& NodeList, long index) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	子供Nodeリストの取得
		@param[in]	Node	親Node
		@return	子供Nodeリスト
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodeListPtr GetChildNodes(const IXMLDOMNodePtr& Node)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Node名の取得
		@param[in]	Node	ターゲットNode
		@return	Node名
		*/
		//--------------------------------------------------------------------------------------
		wstring GetBaseName(const IXMLDOMNodePtr& Node) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Nodeのテキストの取得
		@param[in]	Node	ターゲットNode
		@return	Nodeのテキスト
		*/
		//--------------------------------------------------------------------------------------
		wstring GetText(const IXMLDOMNodePtr& Node) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Nodeのテキストの設定
		@param[in]	Node	ターゲットNode
		@param[in]	text	設定するテキスト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetText(const IXMLDOMNodePtr& Node, const CComBSTR& text);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	NodeのXMLの取得（ノード内XMLへのアクセスはリードオンリー）
		@param[in]	Node	ターゲットNode
		@return	NodeのXML
		*/
		//--------------------------------------------------------------------------------------
		wstring GetXML(const IXMLDOMNodePtr& Node) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Nodeのアトリビュートの取得
		@param[in]	Node	ターゲットNode
		@param[in]	AttTag	アトリビュート名
		@return	Nodeのアトリビュート
		*/
		//--------------------------------------------------------------------------------------
		wstring GetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag)const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Nodeのアトリビュートの設定
		@param[in]	Node	ターゲットNode
		@param[in]	AttTag	アトリビュート名
		@param[in]	AttValue	設定する値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetAttribute(const IXMLDOMNodePtr& Node, const CComBSTR& AttTag, const CComBSTR& AttValue);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPathによる単体Nodeの取得
		@param[in]	Node	ターゲットNode
		@param[in]	Query	クエリ文字列
		@return	Node
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodePtr GetSelectSingleNode(const IXMLDOMNodePtr& Node, const CComBSTR& Query) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	XPathによる複数Nodeの取得
		@param[in]	Node	ターゲットNode
		@param[in]	Query	クエリ文字列
		@return	複数Node
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodeListPtr GetSelectNodes(const IXMLDOMNodePtr& Node, const CComBSTR& Query) const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	子供Nodeの追加
		@param[in]	Node	ターゲットNode
		@param[in]	NodeName	Node名
		@return	追加したNode
		*/
		//--------------------------------------------------------------------------------------
		IXMLDOMNodePtr AddChildNode(const IXMLDOMNodePtr& Node, const CComBSTR& NodeName);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		XmlDoc(const XmlDoc&) = delete;
		XmlDoc& operator=(const XmlDoc&) = delete;
		//ムーブ禁止
		XmlDoc(const XmlDoc&&) = delete;
		XmlDoc& operator=(const XmlDoc&&) = delete;
	};



}
//endof  basecross

