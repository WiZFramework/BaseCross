/*!
@file StringComponents.cpp
@brief 文字列描画コンポーネント実体
*/
#include "stdafx.h"

namespace basecross {

	//--------------------------------------------------------------------------------------
	//	struct StringSprite::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct StringSprite::Impl{
		// テキスト レンダリングに関連するリソース。
		wstring m_text;
		float m_FoneSize;
		float m_TextBlockWidth;
		float m_TextBlockHeight;
		wstring m_FontName;
		Color4 m_FontColor;
		Point2D<float> m_StartPosition;
		StringSprite::TextAlignment m_TextAlignment;

		Color4 m_BackColor;		//背景色
		Point2D<float> m_BackTextMargin;	//背景色を塗りつぶすテキストのマージン(左右と上下)

		DWRITE_TEXT_METRICS m_textMetrics;
		ComPtr<ID2D1SolidColorBrush>    m_Brush;
		ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
		ComPtr<IDWriteTextLayout>       m_textLayout;
		ComPtr<IDWriteTextFormat>		m_textFormat;

		ComPtr<ID2D1SolidColorBrush>    m_BackBrush;


		Impl():
			m_text(),
			m_FoneSize(16.0f),
			m_TextBlockWidth(128.0f),
			m_TextBlockHeight(32.0f),
			m_FontName(L"ＭＳゴシック"),
			m_FontColor(Color4(1.0f, 1.0f, 1.0f, 1.0f)),
			m_StartPosition{16.0f,16.0f},
			m_TextAlignment(StringSprite::TextAlignment::m_Left),
			m_BackColor(Color4(0.0f, 0.0f, 0.0f, 0.0f)),
			m_BackTextMargin(4.0f,0.0f)
			{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Component;
	//	用途: StringSpriteコンポーネント
	//	文字列表示コンポーネント
	//--------------------------------------------------------------------------------------
	StringSprite::StringSprite(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl()){
		try{
			ZeroMemory(&pImpl->m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

			// デバイスに依存するリソースを作成します。
			auto Dev = App::GetApp()->GetDeviceResources();
			auto D2DFactory = Dev->GetD2DFactory();
			auto DWriteFactory = Dev->GetDWriteFactory();
			auto D2DDeviceContext = Dev->GetD2DDeviceContext();

			ThrowIfFailed(
				DWriteFactory->CreateTextFormat(
				pImpl->m_FontName.c_str(),
				nullptr,
				DWRITE_FONT_WEIGHT_LIGHT,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				pImpl->m_FoneSize,
				L"ja",
				&pImpl->m_textFormat
				),
				L"フォント作成に失敗しました。",
				L"DWriteFactory->CreateTextFormat()",
				L"StringSprite::StringSprite()"
				);

			ThrowIfFailed(
				pImpl->m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
				L"フォントアラインメント設定に失敗しました。",
				L"DWriteFactory->CreateTextFormat()",
				L"StringSprite::StringSprite()"
				);

			ThrowIfFailed(
				D2DFactory->CreateDrawingStateBlock(&pImpl->m_stateBlock),
				L"フォントステートブロック設定に失敗しました。",
				L"D2DFactory->CreateDrawingStateBlock()",
				L"StringSprite::StringSprite()"
				);

			auto Col = D2D1::ColorF(pImpl->m_FontColor.x, pImpl->m_FontColor.y, pImpl->m_FontColor.z, pImpl->m_FontColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
				Col,
				&pImpl->m_Brush
				),
				L"フォントブラシ設定に失敗しました。",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringSprite::StringSprite()"
				);

			Col = D2D1::ColorF(pImpl->m_BackColor.x, pImpl->m_BackColor.y, pImpl->m_BackColor.z, pImpl->m_BackColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
				Col,
				&pImpl->m_BackBrush
				),
				L"バックグラウンドブラシ設定に失敗しました。",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringSprite::StringSprite()"
				);


		}
		catch (...){
			throw;
		}
	}
	StringSprite::~StringSprite(){}
	//アクセサ
	void StringSprite::SetFont(const wstring& FontName, float FontSize){
		ZeroMemory(&pImpl->m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

		// デバイスに依存するリソースを作成します。
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DFactory = Dev->GetD2DFactory();
		auto DWriteFactory = Dev->GetDWriteFactory();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();

		ThrowIfFailed(
			DWriteFactory->CreateTextFormat(
			FontName.c_str(),
			nullptr,
			DWRITE_FONT_WEIGHT_LIGHT,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			FontSize,
			L"ja",
			&pImpl->m_textFormat
			),
			L"フォント作成に失敗しました。",
			L"DWriteFactory->CreateTextFormat()",
			L"StringSprite::SetFont()"
			);
		//フォントの作成に成功したので値を設定
		pImpl->m_FontName = FontName;
		pImpl->m_FoneSize = FontSize;

		ThrowIfFailed(
			pImpl->m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
			L"フォントアラインメント設定に失敗しました。",
			L"DWriteFactory->CreateTextFormat()",
			L"StringSprite::SetFont()"
			);

		ThrowIfFailed(
			D2DFactory->CreateDrawingStateBlock(&pImpl->m_stateBlock),
			L"フォントステートブロック設定に失敗しました。",
			L"D2DFactory->CreateDrawingStateBlock()",
			L"StringSprite::SetFont()"
			);


	}
	StringSprite::TextAlignment StringSprite::GetTextAlignment() const{
		return pImpl->m_TextAlignment;
	}
	void StringSprite::SetTextAlignment(StringSprite::TextAlignment Alignment){
		pImpl->m_TextAlignment = Alignment;
	}


	const wstring& StringSprite::GetText() const{
		return pImpl->m_text;
	}
	void StringSprite::SetText(const wstring& str){
		pImpl->m_text = str;
	}


	//最後尾に追加
	void StringSprite::AddText(const wstring& str){
		wstring TempText = GetText();
		TempText += str;
		SetText(TempText);
	}


	const Color4& StringSprite::GetFontColor() const{
		return pImpl->m_FontColor;
	}
	void StringSprite::SetFontColor(const Color4& Col){
		pImpl->m_FontColor = Col;
		auto ColBrush = D2D1::ColorF(pImpl->m_FontColor.x, pImpl->m_FontColor.y, pImpl->m_FontColor.z, pImpl->m_FontColor.w);
		// デバイスに依存するリソースを作成します。
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
			ColBrush,
			&pImpl->m_Brush
			),
			L"フォントブラシ設定に失敗しました。",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"StringSprite::SetFontColor()"
			);

	}

	const Color4& StringSprite::GetBackColor() const{
		return pImpl->m_BackColor;
	}
	void StringSprite::SetBackColor(const Color4& Col){
		pImpl->m_BackColor = Col;
		auto ColBrush = D2D1::ColorF(pImpl->m_BackColor.x, pImpl->m_BackColor.y, pImpl->m_BackColor.z, pImpl->m_BackColor.w);
		// デバイスに依存するリソースを作成します。
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
			ColBrush,
			&pImpl->m_BackBrush
			),
			L"バックブラシ設定に失敗しました。",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"StringSprite::SetCaretColor()"
			);
	}

	Point2D<float> StringSprite::GetBackTextMargin() const{
		return pImpl->m_BackTextMargin;
	}
	void StringSprite::SetBackTextMargin(Point2D<float> p){
		pImpl->m_BackTextMargin = p;
	}


	const wstring& StringSprite::GetFontName() const{
		return pImpl->m_FontName;
	}
	float StringSprite::GetFontSize() const{
		return pImpl->m_FoneSize;
	}

	float StringSprite::GetTextBlockWidth() const{
		return pImpl->m_TextBlockWidth;
	}
	void StringSprite::SetTextBlockWidth(float f){
		pImpl->m_TextBlockWidth = f;
	}
	float StringSprite::GetTextBlockHeight() const{
		return pImpl->m_TextBlockHeight;
	}
	void StringSprite::SetTextBlockHeight(float f){
		pImpl->m_TextBlockHeight = f;
	}




	ComPtr<IDWriteTextLayout>& StringSprite::GetTextLayout()const{
		return pImpl->m_textLayout;
	}
	ComPtr<IDWriteTextFormat>&	StringSprite::GetTextFormat()const{
		return pImpl->m_textFormat;
	}

	const DWRITE_TEXT_METRICS& StringSprite::GetDriteTextMetrics() const{
		return pImpl->m_textMetrics;
	}

	const Point2D<float>& StringSprite::GetStartPosition() const{
		return pImpl->m_StartPosition;
	}
	void StringSprite::SetStartPosition(const Point2D<float>& pos){
		pImpl->m_StartPosition = pos;
	}

	Rect2D<float> StringSprite::GetTextRect() const{
		Rect2D<float> ret;
		ret.left = GetStartPosition().x;
		ret.top = GetStartPosition().y;
		ret.right = ret.left + GetTextBlockWidth();
		ret.bottom = ret.top + GetTextBlockHeight();
		return ret;
	}
	void StringSprite::SetTextRect(const Rect2D<float>& rect){
		SetStartPosition(Point2D<float>(rect.left, rect.top));
		SetTextBlockWidth(rect.Width());
		SetTextBlockHeight(rect.Height());
	}

	void StringSprite::OnUpdate(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();

		ThrowIfFailed(
			DWriteFactory->CreateTextLayout(
			pImpl->m_text.c_str(),
			(uint32)pImpl->m_text.length(),
			pImpl->m_textFormat.Get(),
			pImpl->m_TextBlockWidth, // 入力テキストの最大幅。
			pImpl->m_TextBlockHeight, // 入力テキストの最大高さ。
			&pImpl->m_textLayout
			),
			L"フォントレイアウト設定に失敗しました。",
			L"DWriteFactory->CreateTextLayout()",
			L"StringSprite::Update()"
			);
		ThrowIfFailed(
			pImpl->m_textLayout->GetMetrics(&pImpl->m_textMetrics),
			L"テキストメトリクス取得に失敗しました。",
			L"pImpl->m_textLayout->GetMetrics()",
			L"StringSprite::Update()"
			);

	}



	void StringSprite::OnDraw(){

		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();
		float ElapsedTime = App::GetApp()->GetElapsedTime();


		D2D1_RECT_F BackRectangle = D2D1::RectF(
			pImpl->m_StartPosition.x, pImpl->m_StartPosition.y,
			pImpl->m_StartPosition.x + pImpl->m_TextBlockWidth,
			pImpl->m_StartPosition.y + pImpl->m_TextBlockHeight
			);
		BackRectangle.left -= pImpl->m_BackTextMargin.x;
		BackRectangle.top -= pImpl->m_BackTextMargin.y;
		BackRectangle.right += pImpl->m_BackTextMargin.x;
		BackRectangle.bottom += pImpl->m_BackTextMargin.y;

		D2DDeviceContext->SaveDrawingState(pImpl->m_stateBlock.Get());
		D2DDeviceContext->BeginDraw();

		//バックグラウンドの描画
		D2DDeviceContext->FillRectangle(&BackRectangle,pImpl->m_BackBrush.Get());


		D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
			pImpl->m_StartPosition.x,
			pImpl->m_StartPosition.y
			);

		D2DDeviceContext->SetTransform(screenTranslation);

		DWRITE_TEXT_ALIGNMENT Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;

		switch (pImpl->m_TextAlignment){
		case TextAlignment::m_Left:
			Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			break;
		case TextAlignment::m_Center:
			Alignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			break;
		case TextAlignment::m_Right:
			Alignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
			break;
		}


		ThrowIfFailed(
			pImpl->m_textFormat->SetTextAlignment(Alignment),
			L"テキストアラインメント設定に失敗しました。",
			L"pImpl->m_textFormat->SetTextAlignment()",
			L"StringSprite::Draw()"
			);

		D2DDeviceContext->DrawTextLayout(
			D2D1::Point2F(0.f, 0.f),
			pImpl->m_textLayout.Get(),
			pImpl->m_Brush.Get()
			);

		HRESULT hr = D2DDeviceContext->EndDraw();
		if (hr != D2DERR_RECREATE_TARGET)
		{
			if (FAILED(hr)){
				throw BaseException(
					L"文字列を表示できませんでした",
					L"if (hr != D2DERR_RECREATE_TARGET)",
					L"StringSprite::Draw()"
					);
			}
		}
		D2DDeviceContext->RestoreDrawingState(pImpl->m_stateBlock.Get());
	}


	struct StringSpriteItem {
		ComPtr<IDWriteTextLayout>  m_textLayout;
		DWRITE_TEXT_METRICS m_textMetrics;
		Rect2D<float> m_Rect;
		wstring m_String;
		bool m_Updated;
		//クリップされるかどうか
		bool m_Clip;
		StringSpriteItem():
			m_textLayout(nullptr),
			m_Rect(),
			m_String(L""),
			m_Updated(true),
			m_Clip(false)
		{
			ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));
		}
	};

	//--------------------------------------------------------------------------------------
	//	struct MultiStringSprite::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct MultiStringSprite::Impl {
		// テキスト レンダリングに関連するリソース。
		float m_FoneSize;
		wstring m_FontName;
		Color4 m_FontColor;
		StringSprite::TextAlignment m_TextAlignment;

		Color4 m_BackColor;		//背景色
		Point2D<float> m_BackTextMargin;	//背景色を塗りつぶすテキストのマージン(左右と上下)

		ComPtr<ID2D1SolidColorBrush>    m_Brush;
		ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
		ComPtr<IDWriteTextFormat>		m_textFormat;

		ComPtr<ID2D1SolidColorBrush>    m_BackBrush;

		vector<StringSpriteItem> m_Items;


		Impl() :
			m_FoneSize(16.0f),
			m_FontName(L"ＭＳゴシック"),
			m_FontColor(Color4(1.0f, 1.0f, 1.0f, 1.0f)),
			m_TextAlignment(StringSprite::TextAlignment::m_Left),
			m_BackColor(Color4(0.0f, 0.0f, 0.0f, 0.0f)),
			m_BackTextMargin(4.0f, 0.0f)
		{}
		~Impl() {}
	};

	//--------------------------------------------------------------------------------------
	//	class MultiStringSprite : public Component;
	//	用途: MultiStringSpriteコンポーネント
	//	文字列表示コンポーネント
	//--------------------------------------------------------------------------------------
	MultiStringSprite::MultiStringSprite(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl()) {
		try {

			// デバイスに依存するリソースを作成します。
			auto Dev = App::GetApp()->GetDeviceResources();
			auto D2DFactory = Dev->GetD2DFactory();
			auto DWriteFactory = Dev->GetDWriteFactory();
			auto D2DDeviceContext = Dev->GetD2DDeviceContext();

			ThrowIfFailed(
				DWriteFactory->CreateTextFormat(
					pImpl->m_FontName.c_str(),
					nullptr,
					DWRITE_FONT_WEIGHT_LIGHT,
					DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL,
					pImpl->m_FoneSize,
					L"ja",
					&pImpl->m_textFormat
				),
				L"フォント作成に失敗しました。",
				L"DWriteFactory->CreateTextFormat()",
				L"StringSprite::StringSprite()"
			);

			ThrowIfFailed(
				pImpl->m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
				L"フォントアラインメント設定に失敗しました。",
				L"DWriteFactory->CreateTextFormat()",
				L"StringSprite::StringSprite()"
			);

			ThrowIfFailed(
				D2DFactory->CreateDrawingStateBlock(&pImpl->m_stateBlock),
				L"フォントステートブロック設定に失敗しました。",
				L"D2DFactory->CreateDrawingStateBlock()",
				L"StringSprite::StringSprite()"
			);

			auto Col = D2D1::ColorF(pImpl->m_FontColor.x, pImpl->m_FontColor.y, pImpl->m_FontColor.z, pImpl->m_FontColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
					Col,
					&pImpl->m_Brush
				),
				L"フォントブラシ設定に失敗しました。",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringSprite::StringSprite()"
			);

			Col = D2D1::ColorF(pImpl->m_BackColor.x, pImpl->m_BackColor.y, pImpl->m_BackColor.z, pImpl->m_BackColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
					Col,
					&pImpl->m_BackBrush
				),
				L"バックグラウンドブラシ設定に失敗しました。",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringSprite::StringSprite()"
			);


		}
		catch (...) {
			throw;
		}
	}
	MultiStringSprite::~MultiStringSprite() {}
	//アクセサ
	void MultiStringSprite::SetFont(const wstring& FontName, float FontSize) {

		// デバイスに依存するリソースを作成します。
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DFactory = Dev->GetD2DFactory();
		auto DWriteFactory = Dev->GetDWriteFactory();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();

		ThrowIfFailed(
			DWriteFactory->CreateTextFormat(
				FontName.c_str(),
				nullptr,
				DWRITE_FONT_WEIGHT_LIGHT,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				FontSize,
				L"ja",
				&pImpl->m_textFormat
			),
			L"フォント作成に失敗しました。",
			L"DWriteFactory->CreateTextFormat()",
			L"MultiStringSprite::SetFont()"
		);
		//フォントの作成に成功したので値を設定
		pImpl->m_FontName = FontName;
		pImpl->m_FoneSize = FontSize;

		ThrowIfFailed(
			pImpl->m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
			L"フォントアラインメント設定に失敗しました。",
			L"DWriteFactory->CreateTextFormat()",
			L"MultiStringSprite::SetFont()"
		);

		ThrowIfFailed(
			D2DFactory->CreateDrawingStateBlock(&pImpl->m_stateBlock),
			L"フォントステートブロック設定に失敗しました。",
			L"D2DFactory->CreateDrawingStateBlock()",
			L"MultiStringSprite::SetFont()"
		);


	}
	StringSprite::TextAlignment MultiStringSprite::GetTextAlignment() const {
		return pImpl->m_TextAlignment;
	}
	void MultiStringSprite::SetTextAlignment(StringSprite::TextAlignment Alignment) {
		pImpl->m_TextAlignment = Alignment;
	}


	const Color4& MultiStringSprite::GetFontColor() const {
		return pImpl->m_FontColor;
	}
	void MultiStringSprite::SetFontColor(const Color4& Col) {
		pImpl->m_FontColor = Col;
		auto ColBrush = D2D1::ColorF(pImpl->m_FontColor.x, pImpl->m_FontColor.y, pImpl->m_FontColor.z, pImpl->m_FontColor.w);
		// デバイスに依存するリソースを作成します。
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
				ColBrush,
				&pImpl->m_Brush
			),
			L"フォントブラシ設定に失敗しました。",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"MultiStringSprite::SetFontColor()"
		);

	}

	const Color4& MultiStringSprite::GetBackColor() const {
		return pImpl->m_BackColor;
	}
	void MultiStringSprite::SetBackColor(const Color4& Col) {
		pImpl->m_BackColor = Col;
		auto ColBrush = D2D1::ColorF(pImpl->m_BackColor.x, pImpl->m_BackColor.y, pImpl->m_BackColor.z, pImpl->m_BackColor.w);
		// デバイスに依存するリソースを作成します。
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
				ColBrush,
				&pImpl->m_BackBrush
			),
			L"バックブラシ設定に失敗しました。",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"MultiStringSprite::SetCaretColor()"
		);
	}

	Point2D<float> MultiStringSprite::GetBackTextMargin() const {
		return pImpl->m_BackTextMargin;
	}
	void MultiStringSprite::SetBackTextMargin(Point2D<float> p) {
		pImpl->m_BackTextMargin = p;
	}


	const wstring& MultiStringSprite::GetFontName() const {
		return pImpl->m_FontName;
	}
	float MultiStringSprite::GetFontSize() const {
		return pImpl->m_FoneSize;
	}

	ComPtr<IDWriteTextFormat>&	MultiStringSprite::GetTextFormat()const {
		return pImpl->m_textFormat;
	}



	const wstring& MultiStringSprite::GetText(size_t Index) const {
		if (pImpl->m_Items.size() <= Index) {
			throw BaseException(
				L"インデックスが範囲外です。",
				L"if (pImpl->m_Items.size() <= Index)",
				L"MultiStringSprite::GetText()"
			);
		}
		return pImpl->m_Items[Index].m_String;
	}
	void MultiStringSprite::SetText(size_t Index, const wstring& str,bool Clip) {
		if (pImpl->m_Items.size() <= Index) {
			throw BaseException(
				L"インデックスが範囲外です。",
				L"if (pImpl->m_Items.size() <= Index)",
				L"MultiStringSprite::SetText()"
			);
		}
		pImpl->m_Items[Index].m_String =str;
		pImpl->m_Items[Index].m_Updated = true;
		pImpl->m_Items[Index].m_Clip = Clip;
	}
	void MultiStringSprite::AddText(size_t Index, const wstring& str, bool Clip) {
		if (pImpl->m_Items.size() <= Index) {
			throw BaseException(
				L"インデックスが範囲外です。",
				L"if (pImpl->m_Items.size() <= Index)",
				L"MultiStringSprite::AddText()"
			);
		}
		pImpl->m_Items[Index].m_String += str;
		pImpl->m_Items[Index].m_Updated = true;
		pImpl->m_Items[Index].m_Clip = Clip;
	}
	void MultiStringSprite::InsertTextBlock(const Rect2D<float>& Block, const wstring& str, bool Clip) {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();

		StringSpriteItem Item;
		Item.m_Rect = Block;
		Item.m_String = str;
		Item.m_Updated = true;
		Item.m_Clip = Clip;
		ZeroMemory(&Item.m_textMetrics, sizeof(DWRITE_TEXT_METRICS));
		pImpl->m_Items.push_back(Item);
	}
	void MultiStringSprite::UpdateTextBlock(size_t Index, const Rect2D<float>& Block, const wstring& str, bool Clip) {
		if (pImpl->m_Items.size() <= Index) {
			throw BaseException(
				L"インデックスが範囲外です。",
				L"if (pImpl->m_Items.size() <= Index)",
				L"MultiStringSprite::UpdateTextBlock()"
			);
		}
		StringSpriteItem Item;
		Item.m_Rect = Block;
		Item.m_String = str;
		Item.m_Updated = true;
		Item.m_Clip = Clip;
		ZeroMemory(&Item.m_textMetrics, sizeof(DWRITE_TEXT_METRICS));
		pImpl->m_Items[Index] = Item;

	}

	void MultiStringSprite::ClearTextBlock() {
		pImpl->m_Items.clear();
	}


	ComPtr<IDWriteTextLayout>& MultiStringSprite::GetTextLayout(size_t Index)const {
		if (pImpl->m_Items.size() <= Index) {
			throw BaseException(
				L"インデックスが範囲外です。",
				L"if (pImpl->m_Items.size() <= Index)",
				L"MultiStringSprite::GetTextLayout()"
			);
		}
		return pImpl->m_Items[Index].m_textLayout;
	}
	const DWRITE_TEXT_METRICS& MultiStringSprite::GetDriteTextMetrics(size_t Index) const {
		if (pImpl->m_Items.size() <= Index) {
			throw BaseException(
				L"インデックスが範囲外です。",
				L"if (pImpl->m_Items.size() <= Index)",
				L"MultiStringSprite::GetDriteTextMetrics()"
			);
		}
		return pImpl->m_Items[Index].m_textMetrics;
	}


	void MultiStringSprite::OnUpdate() {
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();

		for (auto& v : pImpl->m_Items) {
			if (v.m_Updated) {
				ZeroMemory(&v.m_textMetrics, sizeof(DWRITE_TEXT_METRICS));
				ThrowIfFailed(
					DWriteFactory->CreateTextLayout(
						v.m_String.c_str(),
						(uint32)v.m_String.length(),
						pImpl->m_textFormat.Get(),
						v.m_Rect.Width(), // 入力テキストの最大幅。
						v.m_Rect.Height(), // 入力テキストの最大高さ。
						&v.m_textLayout
					),
					L"フォントレイアウト設定に失敗しました。",
					L"DWriteFactory->CreateTextLayout()",
					L"MultiStringSprite::OnUpdate()"
				);
				ThrowIfFailed(
					v.m_textLayout->GetMetrics(&v.m_textMetrics),
					L"テキストメトリクス取得に失敗しました。",
					L"pImpl->m_textLayout->GetMetrics()",
					L"MultiStringSprite::OnUpdate()"
				);
				v.m_Updated = false;
			}
		}
	}

	void MultiStringSprite::OnDraw() {

		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();
		float ElapsedTime = App::GetApp()->GetElapsedTime();


		D2DDeviceContext->BeginDraw();
		for (auto& v : pImpl->m_Items) {
			if (v.m_Clip) {
				continue;
			}
			D2D1_RECT_F BackRectangle = D2D1::RectF(
				v.m_Rect.left, v.m_Rect.top,
				v.m_Rect.right,v.m_Rect.bottom);
			BackRectangle.left -= pImpl->m_BackTextMargin.x;
			BackRectangle.top -= pImpl->m_BackTextMargin.y;
			BackRectangle.right += pImpl->m_BackTextMargin.x;
			BackRectangle.bottom += pImpl->m_BackTextMargin.y;

			D2DDeviceContext->SaveDrawingState(pImpl->m_stateBlock.Get());

			//バックグラウンドの描画
			D2DDeviceContext->FillRectangle(&BackRectangle, pImpl->m_BackBrush.Get());


			D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
				v.m_Rect.left,
				v.m_Rect.top
			);

			D2DDeviceContext->SetTransform(screenTranslation);

			DWRITE_TEXT_ALIGNMENT Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;

			switch (pImpl->m_TextAlignment) {
			case StringSprite::TextAlignment::m_Left:
				Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
				break;
			case StringSprite::TextAlignment::m_Center:
				Alignment = DWRITE_TEXT_ALIGNMENT_CENTER;
				break;
			case StringSprite::TextAlignment::m_Right:
				Alignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
				break;
			}


			ThrowIfFailed(
				pImpl->m_textFormat->SetTextAlignment(Alignment),
				L"テキストアラインメント設定に失敗しました。",
				L"pImpl->m_textFormat->SetTextAlignment()",
				L"MultiStringSprite::Draw()"
			);

			D2DDeviceContext->DrawTextLayout(
				D2D1::Point2F(0.f, 0.f),
				v.m_textLayout.Get(),
				pImpl->m_Brush.Get()
			);
		}
		HRESULT hr = D2DDeviceContext->EndDraw();
		if (hr != D2DERR_RECREATE_TARGET)
		{
			if (FAILED(hr)) {
				throw BaseException(
					L"文字列を表示できませんでした",
					L"if (hr != D2DERR_RECREATE_TARGET)",
					L"MultiStringSprite::Draw()"
				);
			}
		}
		D2DDeviceContext->RestoreDrawingState(pImpl->m_stateBlock.Get());
	}



}

//end basecross
