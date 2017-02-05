/*!
@file StringComponents.h
@brief 文字列描画コンポーネント
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	///	StringSpriteコンポーネント(文字列表示)
	//--------------------------------------------------------------------------------------
	class StringSprite : public Component{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	GameObjectPtr	ゲームオブジェクト
		*/
		//--------------------------------------------------------------------------------------
		explicit StringSprite(const shared_ptr<GameObject>& GameObjectPtr);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~StringSprite();
		//--------------------------------------------------------------------------------------
		/*!
		@enum TextAlignment
		*/
		//--------------------------------------------------------------------------------------
		enum TextAlignment{
			//左合わせ
			m_Left,
			//中央合わせ
			m_Center,
			//右合わせ
			m_Right
		};
		//アクセサ
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォントを設定する
		@param[in]	FontName フォント名
		@param[in]	FontSize フォントサイズ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFont(const wstring& FontName, float FontSize);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストのアラインメントを得る
		@return	テキストのアラインメント
		*/
		//--------------------------------------------------------------------------------------
		StringSprite::TextAlignment GetTextAlignment() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストのアラインメントを設定する
		@param[in]	Alignment アラインメント
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTextAlignment(StringSprite::TextAlignment Alignment);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを得る
		@return	テキスト
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetText() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを設定する
		@param[in]	str テキスト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetText(const wstring& str);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	テキストを追加する
		@param[in]	str テキスト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddText(const wstring& str);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォント色を得る
		@return	フォント色
		*/
		//--------------------------------------------------------------------------------------
		const Color4& GetFontColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォント色を設定する
		@param[in]	Col フォント色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFontColor(const Color4& Col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォント名を得る
		@return	フォント名
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetFontName() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	フォントサイズを得る
		@return	フォントサイズ
		*/
		//--------------------------------------------------------------------------------------
		float GetFontSize() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景色を得る
		@return	背景色
		*/
		//--------------------------------------------------------------------------------------
		const Color4& GetBackColor() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景色を設定する
		@param[in]	Col 背景色
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBackColor(const Color4& Col);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景のテキストマージンを得る
		@return	背景のテキストマージン
		*/
		//--------------------------------------------------------------------------------------
		Point2D<float> GetBackTextMargin() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	背景のテキストマージンを設定する
		@param[in]	p 背景のテキストマージン
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetBackTextMargin(Point2D<float> p);

		float GetTextBlockWidth() const;
		void SetTextBlockWidth(float f);
		float GetTextBlockHeight() const;
		void SetTextBlockHeight(float f);

		ComPtr<IDWriteTextLayout>& GetTextLayout()const;
		ComPtr<IDWriteTextFormat>&	GetTextFormat()const;
		const DWRITE_TEXT_METRICS& GetDriteTextMetrics() const;

		const Point2D<float>& GetStartPosition() const;
		void SetStartPosition(const Point2D<float>& pos);

		Rect2D<float> GetTextRect() const;
		void SetTextRect(const Rect2D<float>& rect);

		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MultiStringSprite : public Component;
	//	用途: MultiStringSpriteコンポーネント
	//	複数ブロックの文字列表示コンポーネント
	//--------------------------------------------------------------------------------------
	class MultiStringSprite : public Component {
	public:
		explicit MultiStringSprite(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~MultiStringSprite();

		//アクセサ
		//ブロック共通
		void SetFont(const wstring& FontName, float FontSize);
		const Color4& GetFontColor() const;
		void SetFontColor(const Color4& Col);
		const wstring& GetFontName() const;
		float GetFontSize() const;

		const Color4& GetBackColor() const;
		void SetBackColor(const Color4& Col);

		Point2D<float> GetBackTextMargin() const;
		void SetBackTextMargin(Point2D<float> p);
		ComPtr<IDWriteTextFormat>&	GetTextFormat()const;
		StringSprite::TextAlignment GetTextAlignment() const;
		void SetTextAlignment(StringSprite::TextAlignment Alignment);

		//ブロック単位
		const wstring& GetText(size_t Index) const;
		void SetText(size_t Index, const wstring& str, bool Clip = false);
		void AddText(size_t Index, const wstring& str, bool Clip = false);
		void InsertTextBlock(const Rect2D<float>& Block, const wstring& str, bool Clip = false);
		void UpdateTextBlock(size_t Index, const Rect2D<float>& Block, const wstring& str, bool Clip = false);
		void ClearTextBlock();
		ComPtr<IDWriteTextLayout>& GetTextLayout(size_t Index)const;
		const DWRITE_TEXT_METRICS& GetDriteTextMetrics(size_t Index) const;


		//操作
		virtual void OnUpdate()override;
		virtual void OnDraw()override;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




}

//end basecross
