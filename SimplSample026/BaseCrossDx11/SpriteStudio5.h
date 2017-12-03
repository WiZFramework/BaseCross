/*!
@file SpriteStudio5.h
@brief スプライトスタジオ
*/
#pragma once
#include "stdafx.h"

namespace basecross {

	//---------------------------------------------------------------
	/// ソートモード
	struct SsPartsSortMode
	{
		enum _enum
		{
			invalid = -1,
			prio,			///< 描画順は優先度で制御する。優先度を表示し、Ｚ座標を隠す。
			z,				///< 描画順はＺ座標で制御する。Ｚ座標を表示し、優先度を隠す。
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"prio") return prio;
			if (s == L"z") return z;
			return invalid;
		}
	};

	//---------------------------------------------------------------
	/// Animation Part Type
	struct SsPartType
	{
		enum _enum
		{
			invalid = -1,
			null,			///< null。領域を持たずSRT情報のみ。ただし円形の当たり判定は設定可能。
			normal,			///< 通常パーツ。領域を持つ。画像は無くてもいい。
			text,			///< テキスト(予約　未実装）
			instance,		///< インスタンス。他アニメ、パーツへの参照。シーン編集モードの代替になるもの
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"null") return null;
			if (s == L"normal") return normal;
			if (s == L"text") return text;
			if (s == L"instance") return instance;
			return invalid;
		}
	};


	//---------------------------------------------------------------
	/// 当たり判定形状
	struct SsBoundsType
	{
		enum _enum
		{
			invalid = -1,
			none,			///< 当たり判定として使わない。
			quad,			///< 自在に変形する四辺形。頂点変形など適用後の４角を結んだ領域。最も重い。
			aabb,			///< 回転しない全体を囲む矩形で交差判定
			circle,			///< 真円の半径で距離により判定する
			circle_smin,	///< 真円の半径で距離により判定する (スケールはx,yの最小値をとる）
			circle_smax,	///< 真円の半径で距離により判定する (スケールはx,yの最大値をとる）
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"none") return none;
			if (s == L"quad") return quad;
			if (s == L"aabb") return aabb;
			if (s == L"circle") return circle;
			if (s == L"circle_smin") return circle_smin;
			if (s == L"circle_smax") return circle_smax;
			return invalid;
		}
	};


	//---------------------------------------------------------------
	/// 継承タイプ
	struct SsInheritType
	{
		enum _enum
		{
			invalid = -1,
			parent,			///< 親の継承方法をそのまま引き継ぐ
			self,			///< 自身がアトリビュート別に持つ継承方法を使う
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"parent") return parent;
			if (s == L"self") return self;
			return invalid;
		}
	};

	//---------------------------------------------------------------
	/// ブレンドタイプ
	struct SsBlendType
	{
		enum _enum{
			invalid = -1,
			mix,			///< 0 ブレンド（ミックス）
			mul,			///< 1 乗算
			add,			///< 2 加算
			sub,			///< 3 減算
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"mix") return mix;
			if (s == L"mul") return mul;
			if (s == L"add") return add;
			if (s == L"sub") return sub;
			return invalid;
		}
	};


	///カラーブレンドキーが使用されている際のカラー適用範囲の定義
	struct SsColorBlendTarget
	{
		enum _enum{
			invalid = -1,
			whole,	///< 単色。全体にかける。
			vertex,	///< 頂点単位
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"whole") return whole;
			if (s == L"vertex") return vertex;
			return invalid;
		}
	};

	///補間モードの定義
	struct SsInterpolationType
	{
		enum _enum
		{
			invalid = -1,
			none,			///< なし
			linear,			///< 線形
			hermite,		///< エルミート
			bezier,			///< ベジェ
			acceleration,	///< 加速度
			deceleration,	///< 減速度
			num,
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"none") return none;
			if (s == L"linear") return linear;
			if (s == L"hermite") return hermite;
			if (s == L"bezier") return bezier;
			if (s == L"acceleration") return acceleration;
			if (s == L"deceleration") return deceleration;
			return invalid;
		}
	};


	/// テクスチャラップモード
	struct SsTexWrapMode
	{
		enum _enum
		{
			invalid = -1,	/// なし
			clamp,			/// クランプする
			repeat,			/// リピート
			mirror,			/// ミラー
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"clamp") return clamp;
			if (s == L"repeat") return repeat;
			if (s == L"mirror") return mirror;
			return invalid;
		}
	};

	/// テクスチャフィルターモード 画素補間方法
	struct SsTexFilterMode
	{
		enum _enum
		{
			invalid = -1,
			nearest,	///< ニアレストネイバー
			linear,		///< リニア、バイリニア
			num
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"nearest") return nearest;
			if (s == L"linear") return linear;
			return invalid;
		}
	};

	/// アトリビュートの種類
	struct SsAttributeKind
	{
		enum _enum
		{
			invalid = -1,	///< 無効値。旧データからの変換時など
			cell = 0,		///< [CELL]参照セル
			posx,		///< [POSX]位置.X
			posy,		///< [POSY]位置.Y
			posz,		///< [POSZ]位置.Z
			rotx,		///< [ROTX]回転.X
			roty,		///< [ROTY]回転.Y
			rotz,		///< [ROTZ]回転.Z
			sclx,		///< [SCLX]スケール.X
			scly,		///< [SCLY]スケール.Y
			alpha,		///< [ALPH]不透明度
			prio,		///< [PRIO]優先度
			fliph,		///< [FLPH]左右反転(セルの原点を軸にする)
			flipv,		///< [FLPV]上下反転(セルの原点を軸にする)
			hide,		///< [HIDE]非表示
			color,		///< [VCOL]カラーブレンド
			vertex,		///< [VERT]頂点変形
			pivotx,		///< [PVTX]原点オフセット.X
			pivoty,		///< [PVTY]原点オフセット.Y
			anchorx,	///< [ANCX]アンカーポイント.X
			anchory,	///< [ANCY]アンカーポイント.Y
			sizex,		///< [SIZX]表示サイズ.X
			sizey,		///< [SIZY]表示サイズ.Y
			imgfliph,	///< [IFLH]イメージ左右反転(常にイメージの中央を原点とする)
			imgflipv,	///< [IFLV]イメージ上下反転(常にイメージの中央を原点とする)
			uvtx,		///< [UVTX]UVアニメ.移動.X
			uvty,		///< [UVTY]UVアニメ.移動.Y
			uvrz,		///< [UVRZ]UVアニメ.回転
			uvsx,		///< [UVSX]UVアニメ.スケール.X
			uvsy,		///< [UVSY]UVアニメ.スケール.Y
			boundr,		///< [BNDR]当たり判定用の半径
			user,		///< [USER]ユーザーデータ
			instance,	///< [IPRM]インスタンスパーツパラメータ
			num,
		};
		static _enum ws2e(const wstring& s){
			if (s == L"invalid") return invalid;
			if (s == L"CELL") return cell;
			if (s == L"POSX") return posx;
			if (s == L"POSY") return posy;
			if (s == L"POSZ") return posz;
			if (s == L"ROTX") return rotx;
			if (s == L"ROTY") return roty;
			if (s == L"ROTZ") return rotz;
			if (s == L"SCLX") return sclx;
			if (s == L"SCLY") return scly;
			if (s == L"ALPH") return alpha;
			if (s == L"PRIO") return prio;
			if (s == L"FLPH") return fliph;
			if (s == L"FLPV") return flipv;
			if (s == L"HIDE") return hide;
			if (s == L"VCOL") return color;
			if (s == L"VERT") return vertex;
			if (s == L"PVTX") return pivotx;
			if (s == L"PVTY") return pivoty;
			if (s == L"ANCX") return anchorx;
			if (s == L"ANCY") return anchory;
			if (s == L"SIZX") return sizex;
			if (s == L"SIZY") return sizey;
			if (s == L"IFLH") return imgfliph;
			if (s == L"IFLV") return imgflipv;
			if (s == L"UVTX") return uvtx;
			if (s == L"UVTY") return uvty;
			if (s == L"UVRZ") return uvrz;
			if (s == L"UVSX") return uvsx;
			if (s == L"UVSY") return uvsy;
			if (s == L"BNDR") return boundr;
			if (s == L"USER") return user;
			if (s == L"IPRM") return instance;	//SDKに無し。実装漏れか？
			return invalid;
		}
	};

	struct SsKeyValueType
	{
		enum _enum
		{
			_unkown = -1,
			_bool = 0,
			_float,
			_int,
			_string,
			_cellmap,
			_vertexAnime,
			_colorAnime,
			_userData,
		};
	};

	/// 曲線補間計算用パラメータ
	struct SsCurve
	{
		float	startTime;		///< 始点キーの時間から制御点の時間へのオフセット値。Ｘ軸に当たる。
		float	startValue;		///< 始点キーの値から	〃	。Ｙ軸	〃
		float	endTime;		///< 終点キーの時間から制御点の時間へのオフセット値。Ｘ軸に当たる。
		float	endValue;		///< 終点キーの値から	〃	。Ｙ軸	〃

		float	startKeyTime;	///< [ワークパラメータ] 始点キーの時間 計算時のみ使用
		float	endKeyTime;		///< [ワークパラメータ] 終点キーの時間 計算時のみ使用

		bool	syncStartEnd;	///< [編集用パラメータ]カーブエディタでの編集時に始点・終点ハンドルを同期して動かすか？

		SsCurve() : startTime(0.f), startValue(0.f), endTime(0.f), endValue(0.f), startKeyTime(0.f), endKeyTime(0.f), syncStartEnd(false){}
		~SsCurve(){}

	};

	//--------------------------------------------------------------------------------------
	//	struct SS5Util;
	//	用途: SpriteStdioユーティリティ
	//--------------------------------------------------------------------------------------
	struct SS5Util{
		static bsm::Vec3 WstrToVector3(const wstring& src){
			bsm::Vec3 ret(0, 0, 0);
			vector<wstring> TokenVec;
			Util::WStrToTokenVector(TokenVec, src, L' ');
			if (TokenVec.size() >= 3){
				ret.x = (float)_wtof(TokenVec[0].c_str());
				ret.y = (float)_wtof(TokenVec[1].c_str());
				ret.z = (float)_wtof(TokenVec[2].c_str());
			}
			return ret;
		}

		static Point2D<float> WstrToPoint2DF(const wstring& src){
			Point2D<float> ret(0, 0);
			vector<wstring> TokenVec;
			Util::WStrToTokenVector(TokenVec, src, L' ');
			if (TokenVec.size() >= 2){
				ret.x = (float)_wtof(TokenVec[0].c_str());
				ret.y = (float)_wtof(TokenVec[1].c_str());
			}
			return ret;
		}
		static bsm::Col4 WstrARGBToColor4(const wstring& src){
			if (src.size() != 8){
				throw BaseException(
					L"カラー情報に間違いがあります",
					src,
					L"SS5Util::WstrARGBTobsm::Col4()"
					);
			}
			wstring a = L"0x" + src.substr(0, 2);
			wstring r = L"0x" + src.substr(2, 2);
			wstring g = L"0x" + src.substr(4, 2);
			wstring b = L"0x" + src.substr(6, 2);


			bsm::Col4 ret(
				(float)wcstoul(r.c_str(), nullptr, 0) / 255.0f,
				(float)wcstoul(g.c_str(), nullptr, 0) / 255.0f,
				(float)wcstoul(b.c_str(), nullptr, 0) / 255.0f,
				(float)wcstoul(a.c_str(), nullptr, 0) / 255.0f
				);
			return ret;


		}
		//
		static wstring TextToWstr(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				return XmlDocReader::GetText(pXMLDOMNode);
			}
			return wstring(L"");
		}

		static wstring AttributeToWstr(IXMLDOMNodePtr Node, const wstring& key){
			return XmlDocReader::GetAttribute(Node, key.c_str());
		}

		static bsm::Vec3 TextToVector3(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				return SS5Util::WstrToVector3(XmlDocReader::GetText(pXMLDOMNode));
			}
			return bsm::Vec3(0, 0, 0);
		}

		static bsm::Vec3 AttributeToVector3(IXMLDOMNodePtr Node, const wstring& key){
			wstring str = XmlDocReader::GetAttribute(Node, key.c_str());
			if (str != L""){
				return SS5Util::WstrToVector3(str);
			}
			return bsm::Vec3(0, 0, 0);
		}



		static Point2D<float> TextToPoint2DF(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				return SS5Util::WstrToPoint2DF(XmlDocReader::GetText(pXMLDOMNode));
			}
			return Point2D<float>(0, 0);
		}

		static Point2D<float> AttributeToPoint2DF(IXMLDOMNodePtr Node, const wstring& key){
			wstring str = XmlDocReader::GetAttribute(Node, key.c_str());
			if (str != L""){
				return SS5Util::WstrToPoint2DF(str);
			}
			return Point2D<float>(0, 0);
		}

		static size_t TextToSizet(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				return wcstoul(XmlDocReader::GetText(pXMLDOMNode).c_str(), nullptr, 0);
			}
			return 0;
		}

		static size_t AttributeToSizet(IXMLDOMNodePtr Node, const wstring& key){
			wstring str = XmlDocReader::GetAttribute(Node, key.c_str());
			if (str != L""){
				return wcstoul(str.c_str(), nullptr, 0);
			}
			return 0;
		}

		static int TextToInt(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				return _wtoi(XmlDocReader::GetText(pXMLDOMNode).c_str());
			}
			return 0;
		}

		static int AttributeToInt(IXMLDOMNodePtr Node, const wstring& key){
			wstring str = XmlDocReader::GetAttribute(Node, key.c_str());
			if (str != L""){
				return _wtoi(str.c_str());
			}
			return 0;
		}

		static float TextToFloat(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				return (float)_wtof(XmlDocReader::GetText(pXMLDOMNode).c_str());
			}
			return 0.0f;
		}

		static float AttributeToFloat(IXMLDOMNodePtr Node, const wstring& key){
			wstring str = XmlDocReader::GetAttribute(Node, key.c_str());
			if (str != L""){
				return (float)_wtof(str.c_str());
			}
			return 0.0f;
		}


		static bool TextToBool(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				if (XmlDocReader::GetText(pXMLDOMNode) == L"0"){
					return false;
				}
				else{
					return true;
				}
			}
			return false;
		}

		static bool AttributeToBool(IXMLDOMNodePtr Node, const wstring& key){
			wstring str = XmlDocReader::GetAttribute(Node, key.c_str());
			if (str != L""){
				if (str == L"0"){
					return false;
				}
				else{
					return true;
				}
			}
			return false;
		}


		static bsm::Col4 TextARGBToColor4(IXMLDOMNodePtr Node, const wstring& key){
			auto pXMLDOMNode = XmlDocReader::GetSelectSingleNode(Node, key.c_str());
			if (pXMLDOMNode){
				wstring Hex = XmlDocReader::GetText(pXMLDOMNode);
				return WstrARGBToColor4(Hex);
			}
			return bsm::Col4(0, 0, 0, 0);
		}

		static bsm::Col4 AttributeARGBToColor4(IXMLDOMNodePtr Node, const wstring& key){
			wstring str = XmlDocReader::GetAttribute(Node, key.c_str());
			if (str != L""){
				return WstrARGBToColor4(str);
			}
			return bsm::Col4(0, 0, 0, 0);
		}


		// 安全にD3Dリソースを作成するヘルパー関数.
		template<typename T, typename TCreateFunc>
		static T* DemandCreate(Microsoft::WRL::ComPtr<T>& comPtr, std::mutex& mutex, TCreateFunc createFunc)
		{
			T* result = comPtr.Get();
			// Double-checked lock pattern.
			MemoryBarrier();
			if (!result)
			{
				std::lock_guard<std::mutex> lock(mutex);
				result = comPtr.Get();
				if (!result)
				{
					createFunc(&result);
					MemoryBarrier();
					comPtr.Attach(result);
				}
			}
			return result;
		}

		///補間でカーブパラメータが必要なタイプか判定する
		static bool SsNeedsCurveParams(SsInterpolationType::_enum type)
		{
			switch (type)
			{
			case SsInterpolationType::bezier:
			case SsInterpolationType::hermite:
				return true;
			}
			return false;
		}
		//----------------------------------------------------------------------------
		/**
		タイプを指定して補間する
		*/
		//----------------------------------------------------------------------------
		static float SsInterpolate(SsInterpolationType::_enum type,
			float time, float start, float end, const SsCurve * curve = nullptr);
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeLine;
	//	用途: アニメーションライン（基底クラス）
	//--------------------------------------------------------------------------------------
	class SS5AnimeLine{
	protected:
		SsAttributeKind::_enum type;	//タイプ
		SS5AnimeLine();
		virtual ~SS5AnimeLine();
	public:
		SsAttributeKind::_enum GetType()const{ return type; }
	private:
		//コピー禁止
		SS5AnimeLine(const  SS5AnimeLine&) = delete;
		SS5AnimeLine& operator=(const  SS5AnimeLine&) = delete;
		//ムーブ禁止
		SS5AnimeLine(const  SS5AnimeLine&&) = delete;
		SS5AnimeLine& operator=(const  SS5AnimeLine&&) = delete;

	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeBoolVal : public SS5AnimeLine;
	//	用途: bool型アニメーションライン
	//--------------------------------------------------------------------------------------
	class SS5AnimeBoolVal : public SS5AnimeLine{
	public:
		SS5AnimeBoolVal(IXMLDOMNodePtr TgtNode, const wstring& typestr);
		virtual ~SS5AnimeBoolVal();
		//指定時間の値を返す
		bool GetValue(float Time);
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeFloatVal : public SS5AnimeLine;
	//	用途: float型の値を保持するアニメーションライン
	//--------------------------------------------------------------------------------------
	class SS5AnimeFloatVal : public SS5AnimeLine{
	public:
		SS5AnimeFloatVal(IXMLDOMNodePtr TgtNode, const wstring& typestr);
		virtual ~SS5AnimeFloatVal();
		//設定された補間をもとに指定時間の値を返す
		float GetValue(float Time);
		float GetScaleValue(float Time);
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	struct AnimeCell{
		UINT id;
		wstring name;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeCellVal : public SS5AnimeLine;
	//	用途: AnimeCell型の値を保持するアニメーションライン
	//--------------------------------------------------------------------------------------
	class SS5AnimeCellVal : public SS5AnimeLine{
	public:
		SS5AnimeCellVal(IXMLDOMNodePtr TgtNode, const wstring& typestr);
		virtual ~SS5AnimeCellVal();
		//指定時間の値を返す
		AnimeCell GetValue(float Time);
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




	//--------------------------------------------------------------------------------------
	//	class SS5AnimeWstrVal : public SS5AnimeLine;
	//	用途: wstrng型の値を保持するアニメーションライン
	//--------------------------------------------------------------------------------------
	class SS5AnimeWstrVal : public SS5AnimeLine{
	public:
		SS5AnimeWstrVal(IXMLDOMNodePtr TgtNode, const wstring& typestr);
		virtual ~SS5AnimeWstrVal();
		//指定時間の値を返す
		wstring GetValue(float Time);
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5AnimeColorVal : public SS5AnimeLine;
	//	用途: bsm::Col4型の値を保持するアニメーションライン
	//--------------------------------------------------------------------------------------
	class SS5AnimeColorVal : public SS5AnimeLine{
	public:
		SS5AnimeColorVal(IXMLDOMNodePtr TgtNode, const wstring& typestr);
		virtual ~SS5AnimeColorVal();
		//設定された補間をもとに指定時間の値を返す
		bsm::Col4 GetValue(float Time);
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	struct Vertex2DAnimeData{
		Point2D<float> LeftTop;
		Point2D<float> RightTop;
		Point2D<float> LeftBottom;
		Point2D<float> RightBottom;
		Vertex2DAnimeData() :
			LeftTop(0, 0),
			RightTop(0, 0),
			LeftBottom(0, 0),
			RightBottom(0, 0){}
	};

	//--------------------------------------------------------------------------------------
	//	class SS5Animebsm::Vec3Val : public SS5AnimeLine;
	//	用途: bsm::Vec3型の値を保持するアニメーションライン
	//--------------------------------------------------------------------------------------
	class SS5AnimeVector3Val : public SS5AnimeLine{
	public:
		SS5AnimeVector3Val(IXMLDOMNodePtr TgtNode, const wstring& typestr);
		virtual ~SS5AnimeVector3Val();
		//設定された補間をもとに指定時間の値を返す
		Vertex2DAnimeData GetValue(float Time);
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class SS5Cell;
	//	用途: SpriteStdioセル
	//--------------------------------------------------------------------------------------
	class SS5Cell{
	public:
		//構築と破棄
		SS5Cell(IXMLDOMNodePtr Node);
		virtual ~SS5Cell();
		//アクセッサ
		const wstring& get_name() const;
		const Point2D<float>& get_pos() const;
		const Point2D<float>& get_pivot() const;
		const Point2D<float>& get_size() const;
		bool get_rotated() const;
		bool is_rotated() const;
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		SS5Cell(const SS5Cell&) = delete;
		SS5Cell& operator=(const SS5Cell&) = delete;
		//ムーブ禁止
		SS5Cell(const SS5Cell&&) = delete;
		SS5Cell& operator=(const SS5Cell&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5ssce;
	//	用途: SpriteStdioのssceクラス
	//--------------------------------------------------------------------------------------
	class SS5ssce{
	public:
		//構築と消滅
		SS5ssce(const wstring& BaseDir, const wstring& Xmlfilename);
		virtual ~SS5ssce();
		//アクセッサ
		const Point2D<float>& get_pixelSize() const;
		//セルの検索
		shared_ptr<SS5Cell> GetCell(const wstring& name);
		//テクスチャリソース名
		const wstring& get_textureResname() const;
		//セルマップ名
		const wstring& get_name() const;
		//テクスチャ
		shared_ptr<TextureResource> GetTexture() const;

	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		SS5ssce(const SS5ssce&) = delete;
		SS5ssce& operator=(const SS5ssce&) = delete;
		//ムーブ禁止
		SS5ssce(const SS5ssce&&) = delete;
		SS5ssce& operator=(const SS5ssce&&) = delete;
	};





	class SS5Animation;
	//--------------------------------------------------------------------------------------
	//	class SS5PartAnimation;
	//	用途: パーツごとのアニメーション
	//--------------------------------------------------------------------------------------
	class SS5PartAnimation{
	public:
		//構築と消滅
		SS5PartAnimation(IXMLDOMNodePtr TgtNode, SS5Animation* pSS5Animation);
		~SS5PartAnimation();
		//アクセッサ
		const wstring& GetpartName() const;

		//グリッドサイズ
		float GetGridSize() const;

		//操作
		bsm::Mat4x4 GetLocalMatrix();

		bool GetHide();
		float GetPrio();
		float GetAlpha();
		AnimeCell GetCell();
		Vertex2DAnimeData GetVirtex2DData();

	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		SS5PartAnimation(const SS5PartAnimation&) = delete;
		SS5PartAnimation& operator=(const SS5PartAnimation&) = delete;
		//ムーブ禁止
		SS5PartAnimation(const SS5PartAnimation&&) = delete;
		SS5PartAnimation& operator=(const SS5PartAnimation&&) = delete;
	};


	//--------------------------------------------------------------------------------------
	//	class SS5Animation;
	//	用途: アニメーション
	//--------------------------------------------------------------------------------------
	class SS5Animation{
	public:
		//構築と消滅
		SS5Animation(IXMLDOMNodePtr TgtNode);
		~SS5Animation();

		//アクセッサ
		const wstring& get_name() const;
		vector< shared_ptr<SS5PartAnimation> >& get_PartAnimationVec();
		const Point2D<float>& get_canvasSize() const;
		float get_gridSize() const;
		const Point2D<float>& get_pivot() const;

		float  get_fps() const;
		void set_fps(float f);

		//ループするかどうかを得る
		bool IsLooped() const;
		bool GetLooped() const;
		//ループするかどうかを設定する
		void SetLooped(bool b);
		//アニメが終了したかどうか
		bool IsAnimeEnd() const;


		//操作
		//アニメーションタイマーの初期化
		void SetCurrentTime(float AnimeTime);
		//アニメーションタイマーの更新
		void AddCurrentTime(float TimeSpan);
		//現在のアニメーションタイマーをもとにフレームを計算し返す
		float GetAnimationTimer2Frame();
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		SS5Animation(SS5Animation const&);
		SS5Animation& operator=(SS5Animation const&);
	};

	class SS5ssae;

	//--------------------------------------------------------------------------------------
	//	class SSPart : public  GameObject;
	//	用途: SSPartクラス(各パーツ)
	//--------------------------------------------------------------------------------------
	class SSPart : public  GameObject{
		//描画データ
		shared_ptr<SimpleDrawObject> m_PtrObj;
		//描画オブジェクト(weak_ptr)
		weak_ptr<SimplePCTStaticRenderer> m_Renderer;
	public:
		//構築と消滅
		SSPart(const shared_ptr<Stage>& StagePtr, const wstring& SsaeName, vector< shared_ptr<SS5ssce> >& SS5sscePtrVec,
			IXMLDOMNodePtr ScanNode, IXMLDOMNodePtr TgtNode, const shared_ptr<SSPart>& Parent,bool SpriteType);
		virtual ~SSPart();
		//初期化
		virtual void OnCreate() override;
		//アクセサ
		int get_arrayIndex();
		float GetPrio();
		void SetSS5ssae(const shared_ptr<SS5ssae>& pSS5ssae);

		//スプライトかどうか(Getterのみ)
		bool GetSpriteType() const;
		bool IsSpriteType() const;

		//操作
		//アニメーションを変更する
		void ChangeAnimation(shared_ptr<SS5Animation> AnimationPtr);
		//アニメーション後の行列を計算する(ZSort無し)
		void CaluclateMatrix();
		void SetPartInVector(vector<SSPart*>& PartVec);

		virtual void OnUpdate() override {}
		virtual void OnDraw()override;

	private:
		//頂点の変更
		void ResetVirtex();
		void ResetSpriteVirtex();

		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class SS5ssae :  public GameObject;
	//	用途: SpriteStdioのssaeクラス（アニメーションパック）
	//--------------------------------------------------------------------------------------
	class SS5ssae : public  GameObject{
	public:
		//構築と消滅
		SS5ssae(const shared_ptr<Stage>& StagePtr, const wstring& BaseDir,
			const wstring& Xmlfilename, const wstring& StartAnimeName,bool SpriteType = false);
		virtual ~SS5ssae();
		//初期化
		virtual void OnCreate() override;
		//アクセッサ
		//SS5ssaeからアニメーションオブジェクトへの行列
		const bsm::Mat4x4& GetToAnimeMatrix() const;
		void SetToAnimeMatrix(const bsm::Mat4x4& mat);

		//スプライトかどうか(Getterのみ)
		bool GetSpriteType() const;
		bool IsSpriteType() const;

		//現在選択されているアニメーションがループするかどうかを得る
		bool IsLooped() const;
		bool GetLooped() const;
		//現在選択されているアニメーションがループするかどうかを設定する
		void SetLooped(bool b);
		//現在選択されているアニメーションが実行終了したかどうか
		bool IsAnimeEnd() const;

		void SetTextureOnlyNoLight(bool b);
		bool GetTextureOnlyNoLight() const;
		bool IsTextureOnlyNoLight() const;


		//操作
		//アニメーションを変更する
		void ChangeAnimation(const wstring& AnimationName, float AnimeTime = 0);
		//アニメーションを更新する
		void UpdateAnimeTime(float TimeSpan);
		//fps（再生スピード）を設定する
		void SetFps(float f);

		virtual void OnUpdate() override{}
		virtual void OnDraw()override;
	private:
		//Implイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




}
//end of namespace bascross.

