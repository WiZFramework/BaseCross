/*!
@file StringComponents.h
@brief 文字列描画コンポーネント
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Component;
	//	用途: StringSpriteコンポーネント
	//	文字列表示コンポーネント
	//--------------------------------------------------------------------------------------
	class StringSprite : public Component{
	public:
		explicit StringSprite(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~StringSprite();
		//アクセサ
		enum TextAlignment{
			m_Left,		//左合わせ
			m_Center,	//中央合わせ
			m_Right		//右合わせ
		};
		//アクセサ
		void SetFont(const wstring& FontName, float FontSize);

		StringSprite::TextAlignment GetTextAlignment() const;
		void SetTextAlignment(StringSprite::TextAlignment Alignment);
		const wstring& GetText() const;
		void SetText(const wstring& str);
		void AddText(const wstring& str);


		const Color4& GetFontColor() const;
		void SetFontColor(const Color4& Col);
		const wstring& GetFontName() const;
		float GetFontSize() const;

		const Color4& GetBackColor() const;
		void SetBackColor(const Color4& Col);

		Point2D<float> GetBackTextMargin() const;
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
