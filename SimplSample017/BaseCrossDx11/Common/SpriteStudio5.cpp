
/*!
@file SpriteStudio5.cpp
@brief スプライトスタジオ実体
*/
#include "stdafx.h"

namespace basecross {

	//ローカル関数

	//---------------------------------------------------------------------------
	/**
	等速運動
	*/
	//---------------------------------------------------------------------------
	static float	// nowにおける補間された値
		linear_(
		float	start,	// 始点
		float	end,	// 終点
		float	now)	// 現在の時間 0.f~1.f
	{
			return start + (now * (end - start));
		}

	//---------------------------------------------------------------------------
	/**
	加速運動
	*/
	//---------------------------------------------------------------------------
	static float	// nowにおける補間された値
		accelerating_(
		float	start,	// 始点
		float	end,	// 終点
		float	now)	// 現在の時間 0.f~1.f
	{
			float rate = now * now;
			return start + (rate * (end - start));
	}

	//---------------------------------------------------------------------------
	/**
	減速運動
	*/
	//---------------------------------------------------------------------------
	static float	// nowにおける補間された値
		decelerating_(
		float	start,	// 始点
		float	end,	// 終点
		float	now)	// 現在の時間 0.f~1.f
	{
			float time = 1 - now;
			float rate = 1 - time * time;
			return start + (rate * (end - start));
	}


	static float bezier_(float start, float end, float time, const SsCurve * c)
	{
		float fCurrentPos = (c->endKeyTime - c->startKeyTime) * time + c->startKeyTime;

		float fRet = end;
		float fCurrentCalc = 0.5f;
		float fCalcRange = 0.5f;

		float fTemp1;
		float fTemp2;
		float fTemp3;

		float fCurrentX;

		for (int iLoop = 0; iLoop < 8; iLoop++)
		{// more count of loop, better precision increase
			fTemp1 = 1.0f - fCurrentCalc;
			fTemp2 = fTemp1 * fTemp1;
			fTemp3 = fTemp2 * fTemp1;
			fCurrentX = (fTemp3 * c->startKeyTime) +
				(3 * fTemp2 * fCurrentCalc * (c->startTime + c->startKeyTime)) +
				(3 * fTemp1 * fCurrentCalc * fCurrentCalc * (c->endTime + c->endKeyTime)) +
				(fCurrentCalc * fCurrentCalc * fCurrentCalc * c->endKeyTime);

			fCalcRange /= 2.0f;
			if (fCurrentX > fCurrentPos)
			{
				fCurrentCalc -= fCalcRange;
			}
			else
			{
				fCurrentCalc += fCalcRange;
			}
		}

		// finally calculate with current value
		fTemp1 = 1.0f - fCurrentCalc;
		fTemp2 = fTemp1 * fTemp1;
		fTemp3 = fTemp2 * fTemp1;
		fRet = (fTemp3 * start) +
			(3 * fTemp2 * fCurrentCalc * (c->startValue + start)) +
			(3 * fTemp1 * fCurrentCalc * fCurrentCalc * (c->endValue + end)) +
			(fCurrentCalc * fCurrentCalc * fCurrentCalc * end);

		return fRet;
	}

	/**
	エルミートでは c->startTime, c->endTime は必要ない

	スロープ値を事前計算しておけばカーブ計算用パラメータは１つになる
	が、ベジェと共用するためこのままの形にしておく。
	*/
	static float hermite_(float start, float end, float time, const SsCurve * c)
	{
		float t2 = time * time;
		float t3 = t2 * time;
		float result =
			(2 * t3 - 3 * t2 + 1) * start +
			(-2 * t3 + 3 * t2) * end +
			(t3 - 2 * t2 + time) * (c->startValue - start) +
			(t3 - t2) * (c->endValue - end);
		return result;
	}

	//--------------------------------------------------------------------------------------
	//	struct SS5Util;
	//	用途: SpriteStdioユーティリティ
	//--------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	/**
	タイプを指定して補間する
	*/
	//----------------------------------------------------------------------------
	float SS5Util::SsInterpolate(SsInterpolationType::_enum type,
		float time, float start, float end, const SsCurve * curve)
	{
		float r = 0.0f;
		switch (type)
		{
		case SsInterpolationType::none:
			r = start;
			break;
		case SsInterpolationType::linear:
			r = linear_(start, end, time);
			break;
		case SsInterpolationType::acceleration:
			r = accelerating_(start, end, time);
			break;
		case SsInterpolationType::deceleration:
			r = decelerating_(start, end, time);
			break;
		case SsInterpolationType::bezier:
			r = bezier_(start, end, time, curve);
			break;
		case SsInterpolationType::hermite:
			r = hermite_(start, end, time, curve);
			break;
		default:
			//SS_ASSERT_ID(type);
			break;
		}
		return r;
	}


	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeLine;
	//	用途: アニメーションライン（基底構造体）
	//--------------------------------------------------------------------------------------
	SS5AnimeLine::SS5AnimeLine() :
		type(SsAttributeKind::invalid){}
	SS5AnimeLine::~SS5AnimeLine(){}

	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeBoolVal::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct SS5AnimeBoolVal::Impl{
		struct data{
			float time;	//スタートタイム
			bool val;	//値
		};
		vector< data > line;
	};
	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeBoolVal : public SS5AnimeLine;
	//	用途: bool型アニメーションライン
	//--------------------------------------------------------------------------------------
	SS5AnimeBoolVal::SS5AnimeBoolVal(IXMLDOMNodePtr TgtNode, const wstring& typestr) :
		SS5AnimeLine(),
		pImpl(new Impl())
	{
		try{
			type = SsAttributeKind::ws2e(typestr);
			auto KeyNodes = XmlDocReader::GetSelectNodes(TgtNode, L"key");
			long lCountNode = XmlDocReader::GetLength(KeyNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(KeyNodes, i);
				if (pXMLDOMNode){
					Impl::data im;
					im.time = SS5Util::AttributeToFloat(pXMLDOMNode, L"time");
					im.val = SS5Util::TextToBool(pXMLDOMNode, L"value");
					pImpl->line.push_back(im);
				}
			}
		}
		catch (...){
			throw;
		}
	}
	SS5AnimeBoolVal::~SS5AnimeBoolVal(){}

	//指定時間の値を返す
	bool SS5AnimeBoolVal::GetValue(float Time){
		if (pImpl->line.size() <= 0){
			return false;
		}
		bool Start = false;
		float StartTime = 0;
		for (auto ptr : pImpl->line){
			if (ptr.time <= Time){
				Start = ptr.val;
				StartTime = ptr.time;
			}
		}
		return Start;
	}


	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeFloatVal::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct SS5AnimeFloatVal::Impl{
		struct data{
			float time;	//スタートタイム
			SsInterpolationType::_enum ipType;
			float val;	//値
			SsCurve curve;	//カーブ情報がある場合
		};
		vector< data > line;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeFloatVal : public SS5AnimeLine;
	//	用途: float型の値を保持するアニメーションライン
	//--------------------------------------------------------------------------------------
	SS5AnimeFloatVal::SS5AnimeFloatVal(IXMLDOMNodePtr TgtNode, const wstring& typestr) :
		SS5AnimeLine(),
		pImpl(new Impl())
	{
		try{
			type = SsAttributeKind::ws2e(typestr);
			auto KeyNodes = XmlDocReader::GetSelectNodes(TgtNode, L"key");
			long lCountNode = XmlDocReader::GetLength(KeyNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(KeyNodes, i);
				if (pXMLDOMNode){
					Impl::data im;
					im.time = SS5Util::AttributeToFloat(pXMLDOMNode, L"time");
					wstring iptype = SS5Util::AttributeToWstr(pXMLDOMNode, L"ipType");
					im.ipType = SsInterpolationType::ws2e(iptype);
					if (SS5Util::SsNeedsCurveParams(im.ipType)){
						wstring curvestr = SS5Util::TextToWstr(pXMLDOMNode, L"curve");
						vector<wstring> strvec;
						Util::WStrToTokenVector(strvec, curvestr, L' ');
						im.curve.startTime = (float)_wtof(strvec[0].c_str());
						im.curve.startValue = (float)_wtof(strvec[1].c_str());
						im.curve.endTime = (float)_wtof(strvec[2].c_str());
						im.curve.endValue = (float)_wtof(strvec[3].c_str());
					}
					im.val = SS5Util::TextToFloat(pXMLDOMNode, L"value");
					pImpl->line.push_back(im);
				}
			}
		}
		catch (...){
			throw;
		}
	}
	SS5AnimeFloatVal::~SS5AnimeFloatVal(){}

	//設定された補間をもとに指定時間の値を返す
	float SS5AnimeFloatVal::GetValue(float Time){
		if (pImpl->line.size() <= 0){
			return 0;
		}
		size_t TgtVecIndex = 0;
		SsInterpolationType::_enum Type;
		Type = SsInterpolationType::none;

		float Start = 0;
		float End = 0;

		float StartTime = 0;
		float EndTime = 0;
		SsCurve curve;
		for (auto ptr : pImpl->line){
			if (ptr.time <= Time){
				Start = ptr.val;
				StartTime = ptr.time;
				Type = ptr.ipType;
				curve = ptr.curve;
			}
		}
		End = Start;
		EndTime = StartTime;
		for (auto ptr : pImpl->line){
			if (ptr.time > Time){
				End = ptr.val;
				EndTime = ptr.time;
				break;
			}
		}

		if (Type == SsInterpolationType::bezier)
		{
			// ベジェのみキーの開始・終了時間が必要
			curve.startKeyTime = StartTime;
			curve.endKeyTime = EndTime;
		}
		double spanFreme = Time - StartTime;
		double totalFrame = abs(EndTime - StartTime);
		double tgtFrame = 0;
		if (totalFrame > 0){
			tgtFrame = spanFreme / totalFrame;
		}
		if (SS5Util::SsNeedsCurveParams(Type)){
			return SS5Util::SsInterpolate(Type, (float)tgtFrame, Start, End, &curve);
		}
		else{
			return SS5Util::SsInterpolate(Type, (float)tgtFrame, Start, End, nullptr);
		}
	}

	float SS5AnimeFloatVal::GetScaleValue(float Time){
		if (pImpl->line.size() <= 0){
			return 1.0f;
		}
		size_t TgtVecIndex = 0;
		SsInterpolationType::_enum Type;
		Type = SsInterpolationType::none;

		float Start = 1.0f;
		float End = 1.0f;

		float StartTime = 0;
		float EndTime = 0;
		SsCurve curve;
		for (auto ptr : pImpl->line){
			if (ptr.time <= Time){
				Start = ptr.val;
				StartTime = ptr.time;
				Type = ptr.ipType;
				curve = ptr.curve;
			}
		}
		End = Start;
		EndTime = StartTime;
		for (auto ptr : pImpl->line){
			if (ptr.time > Time){
				End = ptr.val;
				EndTime = ptr.time;
				break;
			}
		}

		if (Type == SsInterpolationType::bezier)
		{
			// ベジェのみキーの開始・終了時間が必要
			curve.startKeyTime = StartTime;
			curve.endKeyTime = EndTime;
		}
		double spanFreme = Time - StartTime;
		double totalFrame = abs(EndTime - StartTime);
		double tgtFrame = 0;
		if (totalFrame > 0){
			tgtFrame = spanFreme / totalFrame;
		}
		if (SS5Util::SsNeedsCurveParams(Type)){
			return SS5Util::SsInterpolate(Type, (float)tgtFrame, Start, End, &curve);
		}
		else{
			return SS5Util::SsInterpolate(Type, (float)tgtFrame, Start, End, nullptr);
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeCellVal::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct SS5AnimeCellVal::Impl{
		struct data{
			float time;	//スタートタイム
			wstring id;		//マップID
			wstring name;	//値
		};
		vector< data > line;
	};



	//--------------------------------------------------------------------------------------
	//	class SS5AnimeCellVal : public SS5AnimeLine;
	//	用途: AnimeCell型の値を保持するアニメーションライン
	//--------------------------------------------------------------------------------------
	SS5AnimeCellVal::SS5AnimeCellVal(IXMLDOMNodePtr TgtNode, const wstring& typestr) :
		SS5AnimeLine(),
		pImpl(new Impl())
	{
		try{
			type = SsAttributeKind::ws2e(typestr);
			auto KeyNodes = XmlDocReader::GetSelectNodes(TgtNode, L"key");
			long lCountNode = XmlDocReader::GetLength(KeyNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(KeyNodes, i);
				if (pXMLDOMNode){
					Impl::data im;
					im.time = SS5Util::AttributeToFloat(pXMLDOMNode, L"time");
					auto ValueNode = XmlDocReader::GetSelectSingleNode(pXMLDOMNode, L"value");
					if (ValueNode){
						im.id = SS5Util::TextToWstr(ValueNode, L"mapId");
						im.name = SS5Util::TextToWstr(ValueNode, L"name");
					}
					pImpl->line.push_back(im);
				}
			}
		}
		catch (...){
			throw;
		}
	}
	SS5AnimeCellVal::~SS5AnimeCellVal(){}

	AnimeCell SS5AnimeCellVal::GetValue(float Time){
		AnimeCell ret;
		ret.id = 0;
		ret.name = L"";
		if (pImpl->line.size() <= 0){
			return ret;
		}
		for (auto ptr : pImpl->line){
			if (ptr.time <= Time){
				ret.id = wcstoul(ptr.id.c_str(), nullptr, 10);
				ret.name = ptr.name;
			}
		}
		return ret;
	}




	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeWstrVal::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct SS5AnimeWstrVal::Impl{
		struct data{
			float time;	//スタートタイム
			wstring val;	//値
		};
		vector< data > line;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeWstrVal : public SS5AnimeLine;
	//	用途: wstrng型の値を保持するアニメーションライン
	//--------------------------------------------------------------------------------------
	SS5AnimeWstrVal::SS5AnimeWstrVal(IXMLDOMNodePtr TgtNode, const wstring& typestr) :
		SS5AnimeLine(),
		pImpl(new Impl())
	{
		try{
			type = SsAttributeKind::ws2e(typestr);
			auto KeyNodes = XmlDocReader::GetSelectNodes(TgtNode, L"key");
			long lCountNode = XmlDocReader::GetLength(KeyNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(KeyNodes, i);
				if (pXMLDOMNode){
					Impl::data im;
					im.time = SS5Util::AttributeToFloat(pXMLDOMNode, L"time");
					auto ValueNode = XmlDocReader::GetSelectSingleNode(pXMLDOMNode, L"value");
					if (ValueNode){
						im.val = SS5Util::TextToWstr(ValueNode, L"name");
					}
					pImpl->line.push_back(im);
				}
			}
		}
		catch (...){
			throw;
		}
	}
	SS5AnimeWstrVal::~SS5AnimeWstrVal(){}

	//指定時間の値を返す
	wstring SS5AnimeWstrVal::GetValue(float Time){
		if (pImpl->line.size() <= 0){
			return wstring(L"");
		}
		wstring Start(L"");
		for (auto ptr : pImpl->line){
			if (ptr.time <= Time){
				Start = ptr.val;
			}
		}
		return Start;
	}

	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeColorVal::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct SS5AnimeColorVal::Impl{
		struct data{
			float time;	//スタートタイム
			SsInterpolationType::_enum ipType;
			Color4 val;	//値
		};
		vector< data > line;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeColorVal : public SS5AnimeLine;
	//	用途: Color4型の値を保持するアニメーションライン
	//--------------------------------------------------------------------------------------
	SS5AnimeColorVal::SS5AnimeColorVal(IXMLDOMNodePtr TgtNode, const wstring& typestr) :
		SS5AnimeLine(),
		pImpl(new Impl())
	{
		try{
			type = SsAttributeKind::ws2e(typestr);
			auto KeyNodes = XmlDocReader::GetSelectNodes(TgtNode, L"key");
			long lCountNode = XmlDocReader::GetLength(KeyNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(KeyNodes, i);
				if (pXMLDOMNode){
					Impl::data im;
					im.time = SS5Util::AttributeToFloat(pXMLDOMNode, L"time");
					wstring iptype = SS5Util::AttributeToWstr(pXMLDOMNode, L"ipType");
					im.ipType = SsInterpolationType::ws2e(iptype);
					im.val = SS5Util::TextARGBToColor4(pXMLDOMNode, L"value");
					pImpl->line.push_back(im);
				}
			}
		}
		catch (...){
			throw;
		}
	}

	SS5AnimeColorVal::~SS5AnimeColorVal(){}


	//設定された補間をもとに指定時間の値を返す
	Color4 SS5AnimeColorVal::GetValue(float Time){
		if (pImpl->line.size() <= 0){
			return Color4(0, 0, 0, 0);
		}
		size_t TgtVecIndex = 0;
		SsInterpolationType::_enum Type;
		Type = SsInterpolationType::none;

		Color4 Start(0, 0, 0, 0);
		Color4 End(0, 0, 0, 0);

		float StartTime = 0;
		float EndTime = 0;
		for (auto ptr : pImpl->line){
			if (ptr.time <= Time){
				Start = ptr.val;
				StartTime = ptr.time;
				Type = ptr.ipType;
			}
		}
		End = Start;
		EndTime = StartTime;
		for (auto ptr : pImpl->line){
			if (ptr.time > Time){
				End = ptr.val;
				EndTime = ptr.time;
				break;
			}
		}
		if (SS5Util::SsNeedsCurveParams(Type)){
			throw BaseException(
				L"現在その補間方法は実装されません",
				L"ベジェもしくはエルミートを使用してませんか？",
				L"SS5AnimeColorVal::GetValue()"
				);
		}
		double spanFreme = Time - StartTime;
		double totalFrame = abs(EndTime - StartTime);
		double tgtFrame = 0;
		if (totalFrame > 0){
			tgtFrame = spanFreme / totalFrame;
		}
		Color4 ret(0, 0, 0, 0);
		ret.x = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.x, End.x, nullptr);
		ret.y = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.y, End.y, nullptr);
		ret.z = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.z, End.z, nullptr);
		ret.w = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.w, End.w, nullptr);
		ret.AdjustRange();
		return ret;
	}


	//--------------------------------------------------------------------------------------
	//	struct SS5AnimeVector3Val::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct SS5AnimeVector3Val::Impl{
		struct data{
			float time;	//スタートタイム
			SsInterpolationType::_enum ipType;
			Vertex2DAnimeData tgt;
		};
		vector< data > line;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeVector3Val : public SS5AnimeLine;
	//	用途: Vector3型の値を保持するアニメーションライン
	//--------------------------------------------------------------------------------------
	SS5AnimeVector3Val::SS5AnimeVector3Val(IXMLDOMNodePtr TgtNode, const wstring& typestr) :
		SS5AnimeLine(),
		pImpl(new Impl())
	{
		try{
			type = SsAttributeKind::ws2e(typestr);
			auto KeyNodes = XmlDocReader::GetSelectNodes(TgtNode, L"key");
			long lCountNode = XmlDocReader::GetLength(KeyNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(KeyNodes, i);
				if (pXMLDOMNode){
					Impl::data im;
					im.time = SS5Util::AttributeToFloat(pXMLDOMNode, L"time");
					wstring iptype = SS5Util::AttributeToWstr(pXMLDOMNode, L"ipType");
					im.ipType = SsInterpolationType::ws2e(iptype);

					auto VertNode = XmlDocReader::GetSelectSingleNode(pXMLDOMNode, L"value/LT");
					if (VertNode){
						im.tgt.LeftTop = SS5Util::WstrToPoint2DF(XmlDocReader::GetText(VertNode));
					}
					else{
						im.tgt.LeftTop = Point2D<float>(0, 0);
					}

					VertNode = XmlDocReader::GetSelectSingleNode(pXMLDOMNode, L"value/RT");
					if (VertNode){
						im.tgt.RightTop = SS5Util::WstrToPoint2DF(XmlDocReader::GetText(VertNode));
					}
					else{
						im.tgt.RightTop = Point2D<float>(0, 0);
					}

					VertNode = XmlDocReader::GetSelectSingleNode(pXMLDOMNode, L"value/LB");
					if (VertNode){
						im.tgt.LeftBottom = SS5Util::WstrToPoint2DF(XmlDocReader::GetText(VertNode));
					}
					else{
						im.tgt.LeftBottom = Point2D<float>(0, 0);
					}

					VertNode = XmlDocReader::GetSelectSingleNode(pXMLDOMNode, L"value/RB");
					if (VertNode){
						im.tgt.RightBottom = SS5Util::WstrToPoint2DF(XmlDocReader::GetText(VertNode));
					}
					else{
						im.tgt.RightBottom = Point2D<float>(0, 0);
					}
					pImpl->line.push_back(im);
				}
			}
		}
		catch (...){
			throw;
		}
	}
	SS5AnimeVector3Val::~SS5AnimeVector3Val(){}
	//設定された補間をもとに指定時間の値を返す
	Vertex2DAnimeData SS5AnimeVector3Val::GetValue(float Time){
		Vertex2DAnimeData Ret, Start, End;
		if (pImpl->line.size() <= 0){
			return Ret;
		}
		size_t TgtVecIndex = 0;
		SsInterpolationType::_enum Type;
		Type = SsInterpolationType::none;


		float StartTime = 0;
		float EndTime = 0;
		for (auto ptr : pImpl->line){
			if (ptr.time <= Time){
				Start = ptr.tgt;
				StartTime = ptr.time;
				Type = ptr.ipType;
			}
		}
		End = Start;
		EndTime = StartTime;
		for (auto ptr : pImpl->line){
			if (ptr.time > Time){
				End = ptr.tgt;
				EndTime = ptr.time;
				break;
			}
		}
		if (SS5Util::SsNeedsCurveParams(Type)){
			throw BaseException(
				L"現在その補間方法は実装されません",
				L"ベジェもしくはエルミートを使用してませんか？",
				L"SS5AnimeVector3Val::GetValue()"
				);
		}
		double spanFreme = Time - StartTime;
		double totalFrame = abs(EndTime - StartTime);
		double tgtFrame = 0;
		if (totalFrame > 0){
			tgtFrame = spanFreme / totalFrame;
		}
		Vector3 ret(0, 0, 0);
		Ret.LeftTop.x = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.LeftTop.x, End.LeftTop.x, nullptr);
		Ret.LeftTop.y = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.LeftTop.y, End.LeftTop.y, nullptr);

		Ret.RightTop.x = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.RightTop.x, End.RightTop.x, nullptr);
		Ret.RightTop.y = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.RightTop.y, End.RightTop.y, nullptr);

		Ret.LeftBottom.x = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.LeftBottom.x, End.LeftBottom.x, nullptr);
		Ret.LeftBottom.y = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.LeftBottom.y, End.LeftBottom.y, nullptr);

		Ret.RightBottom.x = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.RightBottom.x, End.RightBottom.x, nullptr);
		Ret.RightBottom.y = SS5Util::SsInterpolate(Type, (float)tgtFrame, Start.RightBottom.y, End.RightBottom.y, nullptr);


		return Ret;
	}

	//--------------------------------------------------------------------------------------
	//	struct SS5Cell::Impl;
	//	用途: SpriteStdioセル内イディオム
	//--------------------------------------------------------------------------------------
	struct SS5Cell::Impl{
		wstring	name;			///< セル名称
		Point2D<float> pos;			///< 左上の座標
		Point2D<float> size;			///< WHピクセルサイズ
		Point2D<float> pivot;			///< WHピクセルサイズ
		bool	 rotated;		///< 左方向に９０度回転されている。uvs の割り当てが変わる。

		Impl(IXMLDOMNodePtr Node);
		~Impl(){}
	};
	SS5Cell::Impl::Impl(IXMLDOMNodePtr Node){
		try{
			//XMLの読み込み
			name = SS5Util::TextToWstr(Node, L"name");
			pos = SS5Util::TextToPoint2DF(Node, L"pos");
			size = SS5Util::TextToPoint2DF(Node, L"size");
			pivot = SS5Util::TextToPoint2DF(Node, L"pivot");
			rotated = SS5Util::TextToBool(Node, L"rotated");
		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	class SS5Cell;
	//	用途: SpriteStdioセル
	//--------------------------------------------------------------------------------------
	//構築と破棄
	SS5Cell::SS5Cell(IXMLDOMNodePtr Node) :
		pImpl(new Impl(Node))
	{
	}
	SS5Cell::~SS5Cell(){}
	//アクセッサ
	const wstring& SS5Cell::get_name() const { return pImpl->name; }
	const Point2D<float>& SS5Cell::get_pos() const { return pImpl->pos; }
	const Point2D<float>& SS5Cell::get_size() const { return pImpl->size; }
	const Point2D<float>& SS5Cell::get_pivot() const { return pImpl->pivot; }
	bool SS5Cell::get_rotated() const { return pImpl->rotated; }
	bool SS5Cell::is_rotated() const { return pImpl->rotated; }

	//--------------------------------------------------------------------------------------
	//	struct SS5ssce::Impl;
	//	用途: SpriteStdioのssceクラス内イディオム
	//--------------------------------------------------------------------------------------
	struct SS5ssce::Impl{
		wstring	basedir;			///< 基準ディレクトリ（\が最後につく）
		wstring	fname;				///< セルマップのファイルネーム
		wstring	name;				///< このセルマップの名称です。
		wstring	imagePath;			///< 参照画像ファイルパス。プロジェクトの画像基準相対
		wstring textureResname;		///テクスチャのリソース名
		Point2D<float> pixelSize;			///< 画像のピクセルWHサイズ
		bool overrideTexSettings;///< テクスチャ設定をプロジェクトの設定ではなく下記設定を使う
		SsTexWrapMode::_enum		wrapMode;			///< テクスチャのラップモード
		SsTexFilterMode::_enum		filterMode;			///< テクスチャのフィルタモード
		vector< shared_ptr<SS5Cell> > m_CellVec;
		shared_ptr<TextureResource> m_Texture;			//テクスチャ
		//構築と消滅
		Impl(const wstring& BaseDir, const wstring& Xmlfilename);
		~Impl(){}
	};
	SS5ssce::Impl::Impl(const wstring& BaseDir, const wstring& Xmlfilename) :
		basedir(BaseDir),
		fname(Xmlfilename)
	{
		try{

			XmlDocReader doc(basedir + fname);
			//基本データ
			auto Setting = doc.GetSelectSingleNode(L"SpriteStudioCellMap");
			name = SS5Util::TextToWstr(Setting, L"name");
			imagePath = SS5Util::TextToWstr(Setting, L"imagePath");
			pixelSize = SS5Util::TextToPoint2DF(Setting, L"pixelSize");
			overrideTexSettings = SS5Util::TextToBool(Setting, L"overrideTexSettings");

			wrapMode = SsTexWrapMode::ws2e(SS5Util::TextToWstr(Setting, L"wrapMode"));
			filterMode = SsTexFilterMode::ws2e(SS5Util::TextToWstr(Setting, L"filterMode"));

			//テクスチャのリソース化
			wstring strTexture = basedir + imagePath;
			textureResname = name;
			m_Texture = App::GetApp()->RegisterTexture(textureResname, strTexture);
			//各セル
			auto Cells = doc.GetSelectNodes(L"SpriteStudioCellMap/cells/cell");
			// ノードリストのノードの数の取得
			long lCountNode = XmlDocReader::GetLength(Cells);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(Cells, i);
				//セルはオブジェクト配列に追加しない
				shared_ptr<SS5Cell> cell(new SS5Cell(pXMLDOMNode));
				m_CellVec.push_back(cell);
			}

		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	class SS5ssce;
	//	用途: SpriteStdioのssceクラス
	//--------------------------------------------------------------------------------------
	//構築と消滅
	SS5ssce::SS5ssce(const wstring& BaseDir, const wstring& Xmlfilename) :
		pImpl(new Impl(BaseDir, Xmlfilename))
	{
	}
	SS5ssce::~SS5ssce(){}
	//アクセッサ
	const Point2D<float>& SS5ssce::get_pixelSize() const{
		return pImpl->pixelSize;
	}
	//セルの検索
	shared_ptr<SS5Cell> SS5ssce::GetCell(const wstring& name){
		for (auto ptr : pImpl->m_CellVec){
			if (name == ptr->get_name()){
				return ptr;
			}
		}
		//見つからない
		return nullptr;
	}
	//テクスチャリソース名
	const wstring& SS5ssce::get_textureResname() const{
		return pImpl->textureResname;
	}
	//セルマップ名
	const wstring& SS5ssce::get_name() const{
		return pImpl->name;
	}

	//テクスチャ
	shared_ptr<TextureResource> SS5ssce::GetTexture() const{
		return pImpl->m_Texture;
	}


	//--------------------------------------------------------------------------------------
	//	struct SS5PartAnimation::Impl;
	//	用途: パーツごとのアニメーション内イディオム
	//--------------------------------------------------------------------------------------
	struct SS5PartAnimation::Impl{
		wstring	partName;
		//アニメーションのポインタ（thisを渡すので生ポインタを使う）
		SS5Animation* m_pSS5Animation;
		vector< shared_ptr<SS5AnimeLine> > m_LineVec;
		Impl(IXMLDOMNodePtr TgtNode, SS5Animation* pSS5Animation);
		~Impl(){}
		//操作
		void FloatAnimationSub(IXMLDOMNodePtr TgtNode, const wstring& Key);
		//現在の姿勢を得る
		Matrix4X4 GetLocalMatrix();

		bool GetHide();
		float GetPrio();
		float GetAlpha();

		AnimeCell GetCell();

		Vertex2DAnimeData GetVirtex2DData();

		void GetIfFloatVal(bool b, shared_ptr<SS5AnimeLine>& ptr, float& dest, float AnimeFrame);
		void GetIfFloatScaleVal(bool b, shared_ptr<SS5AnimeLine>& ptr, float& dest, float AnimeFrame);

	};

	SS5PartAnimation::Impl::Impl(IXMLDOMNodePtr TgtNode, SS5Animation* pSS5Animation) :
		m_pSS5Animation(pSS5Animation)
	{
		try{
			partName = SS5Util::TextToWstr(TgtNode, L"partName");
			auto Node = XmlDocReader::GetSelectSingleNode(TgtNode, L"attributes/attribute[@tag=\'HIDE\']");
			if (Node){
				auto ptr = make_shared<SS5AnimeBoolVal>(Node, L"HIDE");
				m_LineVec.push_back(ptr);
			}
			Node = XmlDocReader::GetSelectSingleNode(TgtNode, L"attributes/attribute[@tag=\'CELL\']");
			if (Node){
				auto ptr = make_shared<SS5AnimeCellVal>(Node, L"CELL");
				m_LineVec.push_back(ptr);
			}

			Node = XmlDocReader::GetSelectSingleNode(TgtNode, L"attributes/attribute[@tag=\'VERT\']");
			if (Node){
				auto ptr = make_shared<SS5AnimeVector3Val>(Node, L"VERT");
				m_LineVec.push_back(ptr);
			}




			FloatAnimationSub(TgtNode, L"POSX");
			FloatAnimationSub(TgtNode, L"POSY");
			FloatAnimationSub(TgtNode, L"POSZ");
			FloatAnimationSub(TgtNode, L"ROTX");
			FloatAnimationSub(TgtNode, L"ROTY");
			FloatAnimationSub(TgtNode, L"ROTZ");
			FloatAnimationSub(TgtNode, L"SCLX");
			FloatAnimationSub(TgtNode, L"SCLY");
			FloatAnimationSub(TgtNode, L"ALPH");
			FloatAnimationSub(TgtNode, L"PRIO");

		}
		catch (...){
			throw;
		}
	}

	void SS5PartAnimation::Impl::FloatAnimationSub(IXMLDOMNodePtr TgtNode, const wstring& Key){
		wstring str = L"attributes/attribute[@tag=\'" + Key + L"\']";
		auto Node = XmlDocReader::GetSelectSingleNode(TgtNode, str.c_str());
		if (Node){
			auto ptr = make_shared<SS5AnimeFloatVal>(Node, Key.c_str());
			m_LineVec.push_back(ptr);
		}
	}

	//現在の姿勢行列を得る
	void SS5PartAnimation::Impl::GetIfFloatVal(bool b, shared_ptr<SS5AnimeLine>& ptr, float& dest, float AnimeFrame){
		if (b){
			auto v = dynamic_pointer_cast<SS5AnimeFloatVal>(ptr);
			dest = v->GetValue(AnimeFrame);
		}
	}

	void SS5PartAnimation::Impl::GetIfFloatScaleVal(bool b, shared_ptr<SS5AnimeLine>& ptr, float& dest, float AnimeFrame){
		if (b){
			auto v = dynamic_pointer_cast<SS5AnimeFloatVal>(ptr);
			dest = v->GetScaleValue(AnimeFrame);
		}
	}


	bool SS5PartAnimation::Impl::GetHide(){
		bool ret = false;
		float AnimeFrame = m_pSS5Animation->GetAnimationTimer2Frame();
		for (auto ptr : m_LineVec){
			if (ptr->GetType() == SsAttributeKind::hide){
				auto v = dynamic_pointer_cast<SS5AnimeBoolVal>(ptr);
				ret = v->GetValue(AnimeFrame);
			}
		}
		return ret;
	}
	float SS5PartAnimation::Impl::GetPrio(){
		float AnimeFrame = m_pSS5Animation->GetAnimationTimer2Frame();
		float prio = 0;
		for (auto ptr : m_LineVec){
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::prio, ptr, prio, AnimeFrame);
		}
		return prio;
	}

	float SS5PartAnimation::Impl::GetAlpha(){
		float AnimeFrame = m_pSS5Animation->GetAnimationTimer2Frame();
		float Alpha = 1.0f;
		for (auto ptr : m_LineVec){
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::alpha, ptr, Alpha, AnimeFrame);
		}
		return Alpha;
	}


	AnimeCell SS5PartAnimation::Impl::GetCell(){
		float AnimeFrame = m_pSS5Animation->GetAnimationTimer2Frame();
		AnimeCell cell;
		for (auto ptr : m_LineVec){
			if (ptr->GetType() == SsAttributeKind::cell){
				auto v = dynamic_pointer_cast<SS5AnimeCellVal>(ptr);
				cell = v->GetValue(AnimeFrame);
			}
		}
		return cell;
	}

	Vertex2DAnimeData SS5PartAnimation::Impl::GetVirtex2DData(){
		float AnimeFrame = m_pSS5Animation->GetAnimationTimer2Frame();
		Vertex2DAnimeData data;
		for (auto ptr : m_LineVec){
			if (ptr->GetType() == SsAttributeKind::vertex){
				auto v = dynamic_pointer_cast<SS5AnimeVector3Val>(ptr);
				data = v->GetValue(AnimeFrame);
			}
		}
		return data;
	}



	//現在の行列を得る。
	//アニメーションからのデータの取り出し
	Matrix4X4 SS5PartAnimation::Impl::GetLocalMatrix(){
		float AnimeFrame = m_pSS5Animation->GetAnimationTimer2Frame();

		Matrix4X4 LocalMatrix;
		Vector3 Pos(0, 0, 0);
		Vector3 Rot(0, 0, 0);
		Vector3 Scale(1.0f, 1.0f, 1.0f);
		for (auto ptr : m_LineVec){
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::posx, ptr, Pos.x, AnimeFrame);
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::posy, ptr, Pos.y, AnimeFrame);
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::posz, ptr, Pos.z, AnimeFrame);
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::rotx, ptr, Rot.x, AnimeFrame);
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::roty, ptr, Rot.y, AnimeFrame);
			GetIfFloatVal(ptr->GetType() == SsAttributeKind::rotz, ptr, Rot.z, AnimeFrame);
			GetIfFloatScaleVal(ptr->GetType() == SsAttributeKind::sclx, ptr, Scale.x, AnimeFrame);
			GetIfFloatScaleVal(ptr->GetType() == SsAttributeKind::scly, ptr, Scale.y, AnimeFrame);
		}

		Pos.x /= m_pSS5Animation->get_gridSize();
		Pos.y /= m_pSS5Animation->get_gridSize();
		//ラジアンに変換
		Rot.x = Rot.x * XM_PI / 180.0f;
		Rot.y = Rot.y * XM_PI / 180.0f;
		Rot.z = Rot.z * XM_PI / 180.0f;
		LocalMatrix.DefTransformation(Scale, Rot, Pos);
		return LocalMatrix;
	}

	//--------------------------------------------------------------------------------------
	//	class SS5PartAnimation;
	//	用途: パーツごとのアニメーション
	//--------------------------------------------------------------------------------------
	//構築と消滅
	SS5PartAnimation::SS5PartAnimation(IXMLDOMNodePtr TgtNode, SS5Animation* pSS5Animation) :
		pImpl(new Impl(TgtNode, pSS5Animation))
	{
	}
	SS5PartAnimation::~SS5PartAnimation(){}

	//アクセッサ
	const wstring& SS5PartAnimation::GetpartName() const{
		return pImpl->partName;
	}

	//グリッドサイズ
	float SS5PartAnimation::GetGridSize() const{
		return pImpl->m_pSS5Animation->get_gridSize();
	}


	Matrix4X4 SS5PartAnimation::GetLocalMatrix(){
		return pImpl->GetLocalMatrix();
	}

	bool SS5PartAnimation::GetHide(){
		return pImpl->GetHide();
	}
	float SS5PartAnimation::GetPrio(){
		return pImpl->GetPrio();
	}

	float SS5PartAnimation::GetAlpha(){
		return pImpl->GetAlpha();
	}


	AnimeCell SS5PartAnimation::GetCell(){
		return pImpl->GetCell();
	}

	Vertex2DAnimeData SS5PartAnimation::GetVirtex2DData(){
		return pImpl->GetVirtex2DData();
	}



	//--------------------------------------------------------------------------------------
	//	struct SS5Animation::Impl;
	//	用途: アニメーションのイディオム
	//--------------------------------------------------------------------------------------
	struct SS5Animation::Impl{
		wstring	name;
		bool overrideSettings;
		float fps;
		float frameCount;
		SsPartsSortMode::_enum sortMode;
		Point2D<float> canvasSize;
		Point2D<float> pivot;
		Color4 bgColor;
		float gridSize;
		Color4 gridColor;
		int outStartNum;
		float m_CurrentTime;
		bool m_Looped;
		bool m_AnimeEnd;
		vector< shared_ptr<SS5PartAnimation> > m_PartAnimationVec;
		//構築と破棄
		Impl(IXMLDOMNodePtr TgtNode);
		~Impl(){}
		//アニメーションタイマーの初期化
		void SetCurrentTime(float AnimeTime){
			if (AnimeTime < 0){
				m_CurrentTime = 0;
			}
			else{
				m_CurrentTime = AnimeTime;
			}
		}
		//アニメーションタイマーの更新
		void AddCurrentTime(float TimeSpan){
			m_CurrentTime += TimeSpan;
		}
		//現在のアニメーションタイマーをもとにフレームを計算し返す
		float GetAnimationTimer2Frame();
	};

	//構築と破棄
	SS5Animation::Impl::Impl(IXMLDOMNodePtr TgtNode) :
		m_CurrentTime(0),
		m_Looped(true),
		m_AnimeEnd(false)
	{
		try{
			name = SS5Util::TextToWstr(TgtNode, L"name");
			overrideSettings = SS5Util::TextToBool(TgtNode, L"overrideSettings");
			//基本データ
			auto Setting = XmlDocReader::GetSelectSingleNode(TgtNode, L"settings");
			if (!Setting){
				throw BaseException(
					L"settingsが見つかりません",
					L"settings",
					L"SS5Animation::Impl::Impl()"
					);
			}
			//デフォルトのFPS
			fps = SS5Util::TextToFloat(Setting, L"fps");
			frameCount = SS5Util::TextToFloat(Setting, L"frameCount");
			sortMode = SsPartsSortMode::ws2e(SS5Util::TextToWstr(Setting, L"sortMode"));
			canvasSize = SS5Util::TextToPoint2DF(Setting, L"canvasSize");
			pivot = SS5Util::TextToPoint2DF(Setting, L"pivot");
			bgColor = SS5Util::TextARGBToColor4(Setting, L"bgColor");
			gridSize = SS5Util::TextToFloat(Setting, L"gridSize");
			gridColor = SS5Util::TextARGBToColor4(Setting, L"gridColor");
			outStartNum = SS5Util::TextToInt(Setting, L"outStartNum");

		}
		catch (...){
			throw;
		}
	}

	//現在のアニメーションタイマーをもとにフレームを計算し返す
	float SS5Animation::Impl::GetAnimationTimer2Frame(){
		//経過時間からフレームを求める
		float fCurrentFrame = m_CurrentTime * fps;
		//アニメーションが最後までいってるかどうかを確認
		bool ret = false;
		if (!m_Looped && fCurrentFrame >= frameCount){
			//ループしないで、アニメが最後に到達したら
			ret = true;
		}
		int iAnimationFrame = static_cast< int >(fCurrentFrame * 10000.0f);
		if (frameCount > 0) {
			if (m_Looped){
				//ループする
				iAnimationFrame %= ((int)frameCount) * 10000;
				iAnimationFrame += outStartNum * 10000;
				m_AnimeEnd = false;
			}
			else{
				//ループしない
				if (ret){
					//最後のフレームにポーズを設定する
					iAnimationFrame = (outStartNum + ((int)frameCount)) * 10000;
					m_AnimeEnd = true;
				}
				else{
					iAnimationFrame += outStartNum * 10000;
					m_AnimeEnd = false;
				}
			}
		}
		else{
			m_AnimeEnd = false;
		}
		return ((float)iAnimationFrame) / 10000.0f;
	}




	//--------------------------------------------------------------------------------------
	//	class SS5Animation;
	//	用途: アニメーション
	//--------------------------------------------------------------------------------------
	//構築と消滅
	SS5Animation::SS5Animation(IXMLDOMNodePtr TgtNode) :
		pImpl(new Impl(TgtNode))
	{
		try{
			auto ModelNodes = XmlDocReader::GetSelectNodes(TgtNode, L"partAnimes/partAnime");
			long lCountNode = XmlDocReader::GetLength(ModelNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(ModelNodes, i);
				if (pXMLDOMNode){
					auto ptr = make_shared<SS5PartAnimation>(pXMLDOMNode, this);
					pImpl->m_PartAnimationVec.push_back(ptr);
				}
			}
		}
		catch (...){
			throw;
		}
	}
	SS5Animation::~SS5Animation(){}

	//アクセッサ
	const wstring& SS5Animation::get_name() const { return pImpl->name; }
	vector< shared_ptr<SS5PartAnimation> >& SS5Animation::get_PartAnimationVec(){
		return pImpl->m_PartAnimationVec;
	}
	const Point2D<float>& SS5Animation::get_canvasSize() const{
		return pImpl->canvasSize;
	}

	float SS5Animation::get_gridSize() const{
		return pImpl->gridSize;
	}

	const Point2D<float>& SS5Animation::get_pivot() const{
		return pImpl->pivot;
	}

	float  SS5Animation::get_fps() const{
		return pImpl->fps;
	}
	void SS5Animation::set_fps(float f){
		pImpl->fps = f;
	}

	//ループするかどうかを得る
	bool SS5Animation::IsLooped() const{
		return pImpl->m_Looped;
	}
	bool SS5Animation::GetLooped() const{
		return pImpl->m_Looped;
	}
	//ループするかどうかを設定する
	void SS5Animation::SetLooped(bool b){
		pImpl->m_Looped = b;
	}
	//アニメ終了したかどうか
	bool SS5Animation::IsAnimeEnd() const{
		return pImpl->m_AnimeEnd;
	}


	//操作
	//アニメーションタイマーの初期化
	void SS5Animation::SetCurrentTime(float AnimeTime){
		pImpl->SetCurrentTime(AnimeTime);
	}
	//アニメーションタイマーの更新
	void  SS5Animation::AddCurrentTime(float TimeSpan){
		pImpl->AddCurrentTime(TimeSpan);
	}
	//現在のアニメーションタイマーをもとにフレームを計算し返す
	float SS5Animation::GetAnimationTimer2Frame(){
		return pImpl->GetAnimationTimer2Frame();
	}

	//--------------------------------------------------------------------------------------
	//	struct SSPart::Impl;
	//	用途: SSPartクラス内イディオム
	//--------------------------------------------------------------------------------------
	struct SSPart::Impl{
		wstring	m_SsaeName;
		IXMLDOMNodePtr m_ScanNode;
		IXMLDOMNodePtr m_TgtNode;

		//メッシュ
		shared_ptr<MeshResource> m_SquareRes;
		//テクスチャ
		shared_ptr<TextureResource> m_TextureResource;

		wstring	name;
		int arrayIndex;
		int parentIndex;
		SsPartType::_enum type;
		SsBoundsType::_enum boundsType;
		SsInheritType::_enum inheritType;
		struct ineheritRatesTag{
			bool flph;	//FLPH 左右反転(セルの原点を軸にする)
			bool flpv;	//FLPV 上下反転(セルの原点を軸にする)
			bool hide;	//HIDE 非表示
			bool iflh;	//IFLH イメージ左右反転(常にイメージの中央を原点とする)
			bool iflv;	//IFLV イメージ左右反転(常にイメージの中央を原点とする)
		};
		ineheritRatesTag ineheritRates;
		SsBlendType::_enum alphaBlendType;
		bool show;
		bool locked;
		bool expandAttribute;
		bool expandChildren;
		//セルマップの配列
		vector< shared_ptr<SS5ssce> >& m_SS5sscePtrVec;
		size_t m_TgtSS5ssceIndex;
		//対応するセル
		shared_ptr<SS5Cell> m_SS5CellPtr;
		//親パーツ(thisポインタを使うので生ポインタを使う)
		weak_ptr<SSPart> m_Parent;
		//SS5ssaeクラス
		weak_ptr<SS5ssae> m_SS5ssae;
		//子パーツの配列
		vector< shared_ptr<SSPart> > m_Childlen;
		//現在の対応するアニメーション
		shared_ptr<SS5PartAnimation> m_PartAnime;
		//現在Hide状態かどうか
		bool m_Hide;
		//現在の優先
		float m_Prio;
		//アルファ値
		float m_Alpha;

		//スプライトかどうか
		bool m_SpriteType;

		//頂点変更時のデータ
		Vertex2DAnimeData m_Vertex2DAnimeData;
		//構築と破棄
		Impl(const wstring& SsaeName, vector< shared_ptr<SS5ssce> >& SS5sscePtrVec, 
			IXMLDOMNodePtr ScanNode, IXMLDOMNodePtr TgtNode, const shared_ptr<SSPart>& Parent, bool SpriteType);
		~Impl(){}
		//操作
		Matrix4X4 CaluclateMatrix();
	};
	//構築と破棄
	SSPart::Impl::Impl(const wstring& SsaeName, 
		vector< shared_ptr<SS5ssce> >& SS5sscePtrVec,
		IXMLDOMNodePtr ScanNode,IXMLDOMNodePtr TgtNode,
		const shared_ptr<SSPart>& Parent, bool SpriteType) :
		m_SsaeName(SsaeName),
		m_SS5sscePtrVec(SS5sscePtrVec),
		m_ScanNode(ScanNode),
		m_TgtNode(TgtNode),
		m_TgtSS5ssceIndex(0),
		m_SS5CellPtr(nullptr),
		m_Parent(Parent),
		m_Hide(false),
		m_Prio(0),
		m_Alpha(1.0f),
		m_SpriteType(SpriteType)
	{
		try{
			name = SS5Util::TextToWstr(TgtNode, L"name");
			arrayIndex = SS5Util::TextToInt(TgtNode, L"arrayIndex");
			parentIndex = SS5Util::TextToInt(TgtNode, L"parentIndex");
			type = SsPartType::ws2e(SS5Util::TextToWstr(TgtNode, L"type"));
			boundsType = SsBoundsType::ws2e(SS5Util::TextToWstr(TgtNode, L"boundsType"));
			inheritType = SsInheritType::ws2e(SS5Util::TextToWstr(TgtNode, L"inheritType"));
			auto ineheritRatesNodes = XmlDocReader::GetSelectNodes(TgtNode, L"ineheritRates/*");
			long lCountNode = XmlDocReader::GetLength(ineheritRatesNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(ineheritRatesNodes, i);
				if (pXMLDOMNode){
					wstring Name = XmlDocReader::GetBaseName(pXMLDOMNode);
					SsAttributeKind::_enum key = SsAttributeKind::ws2e(Name);
					switch (key){
					case SsAttributeKind::fliph:
						ineheritRates.flph = SS5Util::TextToBool(pXMLDOMNode, Name);
						break;
					case SsAttributeKind::flipv:
						ineheritRates.flpv = SS5Util::TextToBool(pXMLDOMNode, Name);
						break;
					case SsAttributeKind::hide:
						ineheritRates.hide = SS5Util::TextToBool(pXMLDOMNode, Name);
						break;
					case SsAttributeKind::imgfliph:
						ineheritRates.iflh = SS5Util::TextToBool(pXMLDOMNode, Name);
						break;
					case SsAttributeKind::imgflipv:
						ineheritRates.iflv = SS5Util::TextToBool(pXMLDOMNode, Name);
						break;
					}
				}
			}
			alphaBlendType = SsBlendType::ws2e(SS5Util::TextToWstr(TgtNode, L"alphaBlendType"));
			show = SS5Util::TextToBool(TgtNode, L"show");
			locked = SS5Util::TextToBool(TgtNode, L"locked");
			expandAttribute = SS5Util::TextToBool(TgtNode, L"expandAttribute");
			expandChildren = SS5Util::TextToBool(TgtNode, L"expandChildren");

		}
		catch (...){
			throw;
		}
	}

	//アニメーション後の行列を計算する
	Matrix4X4 SSPart::Impl::CaluclateMatrix(){
		Matrix4X4 ret;
		ret.Identity();
		if (m_PartAnime){
			m_Prio = -m_PartAnime->GetPrio();
			m_Hide = m_PartAnime->GetHide();
			m_Alpha = m_PartAnime->GetAlpha();
			m_Vertex2DAnimeData = m_PartAnime->GetVirtex2DData();

			AnimeCell TgtCell = m_PartAnime->GetCell();
			if (TgtCell.name != L""){
				m_TgtSS5ssceIndex = TgtCell.id;
				m_SS5CellPtr = m_SS5sscePtrVec[m_TgtSS5ssceIndex]->GetCell(TgtCell.name);
			}
			else{
				m_TgtSS5ssceIndex = 0;
				m_SS5CellPtr.reset();
			}
			ret = m_PartAnime->GetLocalMatrix();
			if (!m_Parent.expired()){
				auto ParPtr = m_Parent.lock();
				auto PtrT = ParPtr->GetComponent<Transform>();
				ret *= PtrT->GetWorldMatrix();
			}
			else{
				if (!m_SS5ssae.expired()){
					auto SS5ssaePtr = m_SS5ssae.lock();
					ret *= SS5ssaePtr->GetToAnimeMatrix();
					auto mat = SS5ssaePtr->GetComponent<Transform>()->GetWorldMatrix();
					ret *= mat;
				}
				else{
					throw BaseException(
						L"Ssaeが取得できません",
						L"if (m_SS5ssae.expired())",
						L"SSPart::Impl::CaluclateMatrix()"
						);

				}
			}
		}
		return ret;
	}



	//--------------------------------------------------------------------------------------
	//	class SSPart : public  GameObject;
	//	用途: SSPartクラス(各パーツ)
	//--------------------------------------------------------------------------------------
	//構築と消滅
	SSPart::SSPart(const shared_ptr<Stage>& StagePtr, const wstring& SsaeName, vector< shared_ptr<SS5ssce> >& SS5sscePtrVec,
		IXMLDOMNodePtr ScanNode, IXMLDOMNodePtr TgtNode, const shared_ptr<SSPart>& Parent, bool SpriteType) :
		GameObject(StagePtr),
		pImpl(new Impl(SsaeName, SS5sscePtrVec, ScanNode, TgtNode, Parent, SpriteType))
	{}
	SSPart::~SSPart(){}
	//初期化
	void SSPart::OnCreate(){
		try{
			AddComponent<Transform>();
			//頂点を変更できるようにするのでデフォルトのリソースは使えない
			wstring ResName = pImpl->m_SsaeName + L"_" + pImpl->name;
			//リソース作成。別のインスタンスでも同じリソースを使う
			if (!App::GetApp()->CheckResource<MeshResource>(ResName)){
				//頂点配列
				vector<VertexPositionNormalTexture> vertices;
				//インデックスを作成するための配列
				vector<uint16_t> indices;
				//Squareの作成(ヘルパー関数を利用)
				MeshUtill::CreateSquare(1.0f, vertices, indices);
				//頂点の型を変えた新しい頂点を作成
				vector<VertexPositionColorTexture> new_vertices;
				for (auto& v : vertices){
					VertexPositionColorTexture nv;
					nv.position = v.position;
					nv.color = Color4(1.0f, 1.0f, 1.0f, 1.0f);
					nv.textureCoordinate = v.textureCoordinate;
					new_vertices.push_back(nv);
				}
				//新しい頂点を使ってメッシュリソースの作成
				pImpl->m_SquareRes = MeshResource::CreateMeshResource<VertexPositionColorTexture>(new_vertices, indices, true);
				//リソースの登録
				App::GetApp()->RegisterResource(ResName, pImpl->m_SquareRes);
			}
			else{
				pImpl->m_SquareRes = App::GetApp()->GetResource<MeshResource>(ResName);
			}
			wstring Query = L"value[parentIndex/text()=" + SS5Util::TextToWstr(pImpl->m_TgtNode, L"arrayIndex") + L"]";
			auto ChildNodes = XmlDocReader::GetSelectNodes(pImpl->m_ScanNode, Query.c_str());
			long lCountNode = XmlDocReader::GetLength(ChildNodes);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(ChildNodes, i);
				if (pXMLDOMNode){
					auto ChildPtr = ObjectFactory::Create<SSPart>(GetStage(),pImpl->m_SsaeName, pImpl->m_SS5sscePtrVec, 
						pImpl->m_ScanNode, pXMLDOMNode, GetThis<SSPart>(),pImpl->m_SpriteType);
					pImpl->m_Childlen.push_back(ChildPtr);
				}
			}
			//スクエア用
			auto PtrDraw = AddComponent<PCTStaticDraw>();
			//スプライト用
			auto PtrSprite = AddComponent<PCTSpriteDraw>();
			//透明処理をする
			SetAlphaActive(true);
		}
		catch (...){
			throw;
		}
	}

	int SSPart::get_arrayIndex(){ 
		return pImpl->arrayIndex; 
	}
	float SSPart::GetPrio(){ return pImpl->m_Prio; }

	void SSPart::SetSS5ssae(const shared_ptr<SS5ssae>& pSS5ssae){
		pImpl->m_SS5ssae = pSS5ssae;
		//子供も設定
		for (auto ptr : pImpl->m_Childlen){
			ptr->SetSS5ssae(pSS5ssae);
		}
	}

	//スプライトかどうか(Getterのみ)
	bool SSPart::GetSpriteType() const{
		return pImpl->m_SpriteType;
	}
	bool SSPart::IsSpriteType() const{
		return pImpl->m_SpriteType;
	}


	//アニメーションを変更する
	void SSPart::ChangeAnimation(shared_ptr<SS5Animation> AnimationPtr){
		//子供配列アニメーションを変更する
		for (auto ptr : pImpl->m_Childlen){
			ptr->ChangeAnimation(AnimationPtr);
		}
		auto& Vec = AnimationPtr->get_PartAnimationVec();
		for (auto& PartAnime : Vec){
			if (PartAnime->GetpartName() == pImpl->name){
				//モデルが見つかった
				pImpl->m_PartAnime = PartAnime;
				return;
			}
		}
		//見つからない
		pImpl->m_PartAnime.reset();
	}

	//頂点の変更
	void SSPart::ResetVirtex(){
		//座標を変更する
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//頂点バッファをリソースから取り出す
		auto pVertexBuffer = pImpl->m_SquareRes->GetVertexBuffer().Get();
		//バックアップの頂点を取り出す
		vector<VertexPositionColorTexture>& BacukVertices = pImpl->m_SquareRes->GetBackupVerteces<VertexPositionColorTexture>();
		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))){
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"SSPart::ResetVirtex()"
				);
		}
		//UV値の変更
		VertexPositionColorTexture* vertices = (VertexPositionColorTexture*)mappedBuffer.pData;
		Rect2D<float> UVRectBase(0, 0, pImpl->m_SS5CellPtr->get_size().x, pImpl->m_SS5CellPtr->get_size().y);
		UVRectBase += pImpl->m_SS5CellPtr->get_pos();

		Rect2D<float> UVRect(
			UVRectBase.left / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().x,
			UVRectBase.top / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().y,
			UVRectBase.right / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().x,
			UVRectBase.bottom / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().y
			);

		float grid = 32.0f;
		if (pImpl->m_PartAnime){
			grid = pImpl->m_PartAnime->GetGridSize();
		}
		float CellWidth = pImpl->m_SS5CellPtr->get_size().x / grid;
		float CelHeight = pImpl->m_SS5CellPtr->get_size().y / grid;

		float pivotx = pImpl->m_SS5CellPtr->get_size().x * pImpl->m_SS5CellPtr->get_pivot().x / grid;
		float pivoty = pImpl->m_SS5CellPtr->get_size().y * pImpl->m_SS5CellPtr->get_pivot().y / grid;

		Point2D<float> Pivot(pivotx, pivoty);


		Rect2D<float> CellRect(-CellWidth / 2.0f, CelHeight / 2.0f, CellWidth / 2.0f, -CelHeight / 2.0f);


		CellRect -= Pivot;

		vertices[0] = VertexPositionColorTexture(
			XMFLOAT3(
			CellRect.left + pImpl->m_Vertex2DAnimeData.LeftTop.x / grid,
			CellRect.top + pImpl->m_Vertex2DAnimeData.LeftTop.y / grid,
			0),
			Color4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			XMFLOAT2(UVRect.left, UVRect.top));

		vertices[1] = VertexPositionColorTexture(
			XMFLOAT3(
			CellRect.right + pImpl->m_Vertex2DAnimeData.RightTop.x / grid,
			CellRect.top + pImpl->m_Vertex2DAnimeData.RightTop.y / grid,
			0), 
			Color4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			XMFLOAT2(UVRect.right, UVRect.top));

		vertices[2] = VertexPositionColorTexture(
			XMFLOAT3(
			CellRect.left + pImpl->m_Vertex2DAnimeData.LeftBottom.x / grid,
			CellRect.bottom + +pImpl->m_Vertex2DAnimeData.LeftBottom.y / grid,
			0), 
			Color4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			XMFLOAT2(UVRect.left, UVRect.bottom));
		vertices[3] = VertexPositionColorTexture(
			XMFLOAT3(
			CellRect.right + pImpl->m_Vertex2DAnimeData.RightBottom.x / grid,
			CellRect.bottom + pImpl->m_Vertex2DAnimeData.RightBottom.y / grid,
			0), 
			Color4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			XMFLOAT2(UVRect.right, UVRect.bottom));
		//アンマップ
		pID3D11DeviceContext->Unmap(pVertexBuffer, 0);
	}

	void SSPart::ResetSpriteVirtex(){
		vector<VertexPositionColorTexture> m_VertexVec;
		//頂点の変更
		Rect2D<float> UVRectBase(0, 0, pImpl->m_SS5CellPtr->get_size().x, pImpl->m_SS5CellPtr->get_size().y);
		UVRectBase += pImpl->m_SS5CellPtr->get_pos();
		Rect2D<float> UVRect(
			UVRectBase.left / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().x,
			UVRectBase.top / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().y,
			UVRectBase.right / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().x,
			UVRectBase.bottom / pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->get_pixelSize().y
			);

		float grid = 32.0f;
		if (pImpl->m_PartAnime){
			grid = pImpl->m_PartAnime->GetGridSize();
		}
		float CellWidth = pImpl->m_SS5CellPtr->get_size().x / grid;
		float CelHeight = pImpl->m_SS5CellPtr->get_size().y / grid;

		float pivotx = pImpl->m_SS5CellPtr->get_size().x * pImpl->m_SS5CellPtr->get_pivot().x / grid;
		float pivoty = pImpl->m_SS5CellPtr->get_size().y * pImpl->m_SS5CellPtr->get_pivot().y / grid;
		Point2D<float> Pivot(pivotx, pivoty);
		Rect2D<float> CellRect(-CellWidth / 2.0f, CelHeight / 2.0f, CellWidth / 2.0f, -CelHeight / 2.0f);
		CellRect -= Pivot;

		m_VertexVec.push_back(
			VertexPositionColorTexture(
			XMFLOAT3(CellRect.left + pImpl->m_Vertex2DAnimeData.LeftTop.x / grid,CellRect.top + pImpl->m_Vertex2DAnimeData.LeftTop.y / grid,0),
			Color4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			XMFLOAT2(UVRect.left, UVRect.top)
			)
		);
		m_VertexVec.push_back(
			VertexPositionColorTexture(
			XMFLOAT3(CellRect.right + pImpl->m_Vertex2DAnimeData.RightTop.x / grid,CellRect.top + pImpl->m_Vertex2DAnimeData.RightTop.y / grid,0),
			Color4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			XMFLOAT2(UVRect.right, UVRect.top)
			)
			);

		m_VertexVec.push_back(
			VertexPositionColorTexture(
			XMFLOAT3(CellRect.left + pImpl->m_Vertex2DAnimeData.LeftBottom.x / grid,CellRect.bottom + +pImpl->m_Vertex2DAnimeData.LeftBottom.y / grid,0),
			Color4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			XMFLOAT2(UVRect.left, UVRect.bottom)
			)
			);

		m_VertexVec.push_back(
			VertexPositionColorTexture(
			XMFLOAT3(CellRect.right + pImpl->m_Vertex2DAnimeData.RightBottom.x / grid,CellRect.bottom + pImpl->m_Vertex2DAnimeData.RightBottom.y / grid,0),
			Color4(1.0f, 1.0f, 1.0f, pImpl->m_Alpha),
			XMFLOAT2(UVRect.right, UVRect.bottom)
			)
			);

		auto PtrSprite = GetComponent<PCTSpriteDraw>();
		auto Mesh = PtrSprite->GetMeshResource();
		Mesh->UpdateVirtexBuffer(m_VertexVec);
	}

	//アニメーション後の行列を計算する
	void SSPart::CaluclateMatrix(){
		Matrix4X4 ret = pImpl->CaluclateMatrix();
		if (pImpl->m_SS5CellPtr){
			//頂点の変更（UVの変更）
			ResetVirtex();
			pImpl->m_TextureResource = pImpl->m_SS5sscePtrVec[pImpl->m_TgtSS5ssceIndex]->GetTexture();
			ResetSpriteVirtex();
		}
		auto PtrT = GetComponent<Transform>();
		PtrT->SetScale(ret.ScaleInMatrix());
		PtrT->SetQuaternion(ret.QtInMatrix());
		PtrT->SetPosition(ret.PosInMatrix());
		//子供の行列を計算する
		for (auto ptr : pImpl->m_Childlen){
			ptr->CaluclateMatrix();
		}
	}

	void SSPart::SetPartInVector(vector<SSPart*>& PartVec){
		PartVec.push_back(this);
		for (auto ptr : pImpl->m_Childlen){
			ptr->SetPartInVector(PartVec);
		}
	}

	void SSPart::OnDraw(){
		if (pImpl->type == SsPartType::normal && pImpl->m_SS5CellPtr){
			if (!pImpl->m_Hide && pImpl->show){
				//描画する
				if (pImpl->m_SpriteType){
					auto PtrSprite = GetComponent<PCTSpriteDraw>();
					PtrSprite->SetTextureResource(pImpl->m_TextureResource);
					PtrSprite->OnDraw();
				}
				else{
					auto PtrDraw = GetComponent<PCTStaticDraw>();
					PtrDraw->SetMeshResource(pImpl->m_SquareRes);
					PtrDraw->SetTextureResource(pImpl->m_TextureResource);
					PtrDraw->OnDraw();
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct SS5ssae::Impl;
	//	用途: SS5ssaeクラス内イディオム
	//--------------------------------------------------------------------------------------
	struct SS5ssae::Impl{
		wstring m_StartAnimeName;


		wstring	basedir;			///< 基準ディレクトリ（\が最後につく）
		wstring	fname;				///< アニメーションパックのファイルネーム
		wstring	name;
		float fps;
		float frameCount;
		SsPartsSortMode::_enum sortMode;
		Point2D<float> canvasSize;
		Point2D<float> pivot;
		Color4 bgColor;
		float gridSize;
		Color4 gridColor;
		int outStartNum;
		//複数セルマップ
		vector< shared_ptr<SS5ssce> > m_SS5ssces;
		//ルートのパーツ
		shared_ptr<SSPart> m_RootPart;
		//アニメーション
		map<wstring, shared_ptr<SS5Animation> > m_AnimeMap;
		//現在のアニメーション（名前）
		wstring m_NowAnimation;
		//現在のアニメーション（ポインタ）
		shared_ptr<SS5Animation> m_NowAnimationPtr;
		//SS5ssaeからアニメーションオブジェクトへの行列
		Matrix4X4 m_ToAnimeMatrix;
		//スプライトかどうか
		bool m_SpriteType;

		//ライティングしないかどうか（デフォルトtrue）
		bool m_TextureOnlyNoLight;

		//構築と破棄
		Impl(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir, const wstring& Xmlfilename, const wstring& StartAnimeName,bool SpriteType);
		~Impl(){}
		//操作
		void ChangeAnimation(const wstring& AnimationName, float AnimeTime);
		void UpdateAnimeTime(float TimeSpan);

	};

	SS5ssae::Impl::Impl(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir, const wstring& Xmlfilename, const wstring& StartAnimeName, bool SpriteType) :
		basedir(BaseDir),
		fname(Xmlfilename),
		m_StartAnimeName(StartAnimeName),
		m_NowAnimation(L""),
		m_NowAnimationPtr(nullptr),
		m_ToAnimeMatrix(),
		m_SpriteType(SpriteType),
		m_TextureOnlyNoLight(true)
	{
		try{

			//インスタンスナンバー
			//スタティック変数
			static size_t stInstansNumber = 0;

			XmlDocReader doc(basedir + fname);
			//ベースデータ
			auto BaseSetting = doc.GetSelectSingleNode(L"SpriteStudioAnimePack");
			if (!BaseSetting){
				throw BaseException(
					L"アニメパックが見つかりません",
					fname,
					L"SS5ssae::Impl::Impl()"
					);
			}
			name = SS5Util::TextToWstr(BaseSetting, L"name") + Util::UintToWStr(stInstansNumber);
			//インスタンスナンバーのインクリメント
			stInstansNumber++;

			//基本データ
			auto Setting = doc.GetSelectSingleNode(L"SpriteStudioAnimePack/settings");
			if (!Setting){
				throw BaseException(
					L"settingsが見つかりません",
					fname,
					L"SS5ssae::Impl::Impl()"
					);
			}

			fps = SS5Util::TextToFloat(Setting, L"fps");
			frameCount = SS5Util::TextToFloat(Setting, L"frameCount");
			sortMode = SsPartsSortMode::ws2e(SS5Util::TextToWstr(Setting, L"sortMode"));
			canvasSize = SS5Util::TextToPoint2DF(Setting, L"canvasSize");
			pivot = SS5Util::TextToPoint2DF(Setting, L"pivot");
			bgColor = SS5Util::TextARGBToColor4(Setting, L"bgColor");
			gridSize = SS5Util::TextToFloat(Setting, L"gridSize");
			gridColor = SS5Util::TextARGBToColor4(Setting, L"gridColor");
			outStartNum = SS5Util::TextToInt(Setting, L"outStartNum");
			//セルマップの読み込み
			//複数セルファイルへの対応
			auto CellMaps = doc.GetSelectNodes(L"SpriteStudioAnimePack/cellmapNames/value");
			if (!CellMaps){
				throw BaseException(
					L"セルマップが見つかりません",
					L"SpriteStudioAnimePack/cellmapNames",
					L"SS5ssae::Impl::Impl()"
					);
			}
			long lCountNode = XmlDocReader::GetLength(CellMaps);
			if (lCountNode <= 0){
				throw BaseException(
					L"セルマップが見つかりません",
					L"if(lCountNode <= 0)",
					L"SS5ssae::Impl::Impl()"
					);
			}
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(CellMaps, i);
				wstring SsceFilename = XmlDocReader::GetText(pXMLDOMNode);
				if (SsceFilename != L""){
					//セルマップの作成と設置
					auto ssce = make_shared<SS5ssce>(BaseDir, SsceFilename);
					m_SS5ssces.push_back(ssce);
				}
				else{
					throw BaseException(
						L"セルマップ名が空白です",
						L"SpriteStudioAnimePack/cellmapNames",
						L"SS5ssae::Impl::Impl()"
						);
				}
			}
			auto PartsRootNode = doc.GetSelectSingleNode(L"SpriteStudioAnimePack/Model/partList");
			if (!PartsRootNode){
				throw BaseException(
					L"パーツが見つかりません",
					fname,
					L"SS5ssae::Impl::Impl()"
					);
			}
			//ルートのノードを探す
			auto RootNode = XmlDocReader::GetSelectSingleNode(PartsRootNode, L"value[parentIndex/text()=-1]");
			if (!RootNode){
				throw BaseException(
					L"パーツのrootが見つかりません",
					fname,
					L"SS5ssae::Impl::Impl()"
					);
			}
			//パーツの読み込み
			m_RootPart = ObjectFactory::Create<SSPart>(StagePtr, name, m_SS5ssces, PartsRootNode, RootNode, nullptr, SpriteType);
			//アニメーションの読み込み
			auto Animations = doc.GetSelectNodes(L"SpriteStudioAnimePack/animeList/anime");
			if (!Animations){
				throw BaseException(
					L"アニメーションが見つかりません",
					L"SpriteStudioAnimePack/animeList/anime",
					L"SS5ssae::Impl::Impl()"
					);
			}

			lCountNode = XmlDocReader::GetLength(Animations);
			for (long i = 0; i < lCountNode; i++){
				auto pXMLDOMNode = XmlDocReader::GetItem(Animations, i);
				if (pXMLDOMNode){
					auto ptr = make_shared<SS5Animation>(pXMLDOMNode);
					m_AnimeMap[ptr->get_name()] = ptr;
				}
			}



		}
		catch (...){
			throw;
		}
	}
	void SS5ssae::Impl::ChangeAnimation(const wstring& AnimationName, float AnimeTime){
		try{
			if (AnimationName == L""){
				throw BaseException(
					L"アニメーション名が空白です",
					L"null",
					L"SS5ssae::Impl::ChangeAnimation()"
					);
			}
			if (m_NowAnimation == AnimationName){
				//同じ名前なら変更しない
				//タイマーのみ変更
				m_NowAnimationPtr->SetCurrentTime(AnimeTime);
				return;
			}
			map<wstring, shared_ptr<SS5Animation> >::const_iterator it;
			it = m_AnimeMap.find(AnimationName);
			if (it != m_AnimeMap.end()) {
				m_NowAnimation = AnimationName;
				m_NowAnimationPtr = it->second;
				m_NowAnimationPtr->SetCurrentTime(AnimeTime);
				//パーツのアニメーションを変更する
				m_RootPart->ChangeAnimation(m_NowAnimationPtr);
			}
			else{
				throw BaseException(
					L"指定のアニメーションが見つかりません",
					AnimationName,
					L"SS5ssae::Impl::ChangeAnimation()"
					);
			}
		}
		catch (...){
			throw;
		}
	}

	void SS5ssae::Impl::UpdateAnimeTime(float TimeSpan){
		//最初にアニメーションタイムを更新する
		m_NowAnimationPtr->AddCurrentTime(TimeSpan);
		//続いてモデルの行列を計算する
		m_RootPart->CaluclateMatrix();
	}

	//--------------------------------------------------------------------------------------
	//	class SS5ssae :  public GameObject;
	//	用途: SpriteStdioのssaeクラス（アニメーションパック）
	//--------------------------------------------------------------------------------------
	//構築と消滅
	SS5ssae::SS5ssae(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir,
		const wstring& Xmlfilename, const wstring& StartAnimeName, bool SpriteType) :
		GameObject(StagePtr),
		pImpl(new Impl(StagePtr, BaseDir, Xmlfilename, StartAnimeName, SpriteType))
	{}
	SS5ssae::~SS5ssae(){}
	//初期化
	void SS5ssae::OnCreate(){
		AddComponent<Transform>();
		pImpl->m_RootPart->SetSS5ssae(GetThis<SS5ssae>());
		//最初のアニメーションの設置
		ChangeAnimation(pImpl->m_StartAnimeName);
		SetAlphaActive(true);
		if (IsSpriteType()){
			//スプライトとして描画する
			SetSpriteDraw(true);
		}
	}


	//現在選択されているアニメーションがループするかどうかを得る
	bool SS5ssae::IsLooped() const{
		if (pImpl->m_NowAnimationPtr){
			return pImpl->m_NowAnimationPtr->IsLooped();
		}
		else{
			return false;
		}
	}
	bool SS5ssae::GetLooped() const{
		return IsLooped();
	}
	//現在選択されているアニメーションがループするかどうかを設定する
	void SS5ssae::SetLooped(bool b){
		if (pImpl->m_NowAnimationPtr){
			return pImpl->m_NowAnimationPtr->SetLooped(b);
		}
	}
	//現在選択されているアニメーションが終了したかどうか
	bool SS5ssae::IsAnimeEnd() const{
		if (pImpl->m_NowAnimationPtr){
			return pImpl->m_NowAnimationPtr->IsAnimeEnd();
		}
		return false;
	}



	void SS5ssae::ChangeAnimation(const wstring& AnimationName, float AnimeTime){
		pImpl->ChangeAnimation(AnimationName, AnimeTime);
	}

	void SS5ssae::UpdateAnimeTime(float TimeSpan){
		pImpl->UpdateAnimeTime(TimeSpan);
	}

	//fps（再生スピード）を設定する
	void SS5ssae::SetFps(float f){
		if (pImpl->m_NowAnimationPtr){
			pImpl->m_NowAnimationPtr->set_fps(f);
		}
	}
	//SS5ssaeからアニメーションオブジェクトへの行列
	const Matrix4X4& SS5ssae::GetToAnimeMatrix() const{
		return pImpl->m_ToAnimeMatrix;
	}
	void SS5ssae::SetToAnimeMatrix(const Matrix4X4& mat){
		pImpl->m_ToAnimeMatrix = mat;
	}

	//スプライトかどうか
	bool SS5ssae::GetSpriteType() const{
		return pImpl->m_SpriteType;
	}
	bool SS5ssae::IsSpriteType() const{
		return pImpl->m_SpriteType;
	}

	void SS5ssae::SetTextureOnlyNoLight(bool b){
		pImpl->m_TextureOnlyNoLight = b;
	}
	bool SS5ssae::GetTextureOnlyNoLight() const{
		return pImpl->m_TextureOnlyNoLight;
	}

	bool SS5ssae::IsTextureOnlyNoLight() const{
		return pImpl->m_TextureOnlyNoLight;
	}

	bool ZSortModelFunc(SSPart* rLeft, SSPart* rRight){
		if (rLeft->GetPrio() == rRight->GetPrio()){
			return rLeft->get_arrayIndex() < rRight->get_arrayIndex();
		}
		return rLeft->GetPrio() > rRight->GetPrio();
	}

	void SS5ssae::OnDraw(){
		ComponentDraw();
		vector<SSPart*> PartVec;
		pImpl->m_RootPart->SetPartInVector(PartVec);
		std::sort(PartVec.begin(), PartVec.end(), ZSortModelFunc);
		if (IsSpriteType()){
			for (auto ptr : PartVec){
				ptr->OnDraw();
			}
		}
		else{
			float count = 0;
			for (auto ptr : PartVec){
				auto PtrT = ptr->GetComponent<Transform>();
				auto TempPos = PtrT->GetPosition();
				auto Pos = PtrT->GetPosition();
				Vector3 Z(0, 0, count);
				Pos += Z;
				PtrT->SetPosition(Pos);
				ptr->OnDraw();
				PtrT->SetPosition(TempPos);
				count -= 0.001f;
			}
		}
	}


}
//endof  basecross
