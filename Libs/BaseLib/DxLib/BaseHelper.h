/*!
@file BaseHelper.h
@brief 例外処理、ヘルパー関数等
@copyright Copyright (c) 2016 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#pragma once
#include "stdafx.h"

namespace basecross{

	/// スマートポインタのヘルパー
	struct handle_closer { void operator()(HANDLE h) { if (h) CloseHandle(h); } };
	typedef public std::unique_ptr<void, handle_closer> ScopedHandle;
	inline HANDLE safe_handle(HANDLE h) { return (h == INVALID_HANDLE_VALUE) ? 0 : h; }

	//--------------------------------------------------------------------------------------
	/// 例外クラス
	//--------------------------------------------------------------------------------------
	class BaseException : public exception
	{
		// メッセージ変数
		wstring m_Message;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	m1  第1メッセージ（警告文）
		@param[in]	m2  第2メッセージ（問題となったデータ）
		@param[in]	m3  第3メッセージ（発生した位置）
		*/
		//--------------------------------------------------------------------------------------
		BaseException(const wstring&  m1, const wstring&  m2, const wstring& m3){
			m_Message = m1;
			m_Message += L"\r\n";
			m_Message += m2;
			m_Message += L"\r\n";
			m_Message += m3;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief メッセージの取得
		@return	メッセージ文字列の参照
		*/
		//--------------------------------------------------------------------------------------
		const wstring& what_w() const throw(){
			return m_Message;
		}
	};

	//--------------------------------------------------------------------------------------
	///	例外クラス（マルチバイト版）
	//--------------------------------------------------------------------------------------
	class BaseMBException : public exception{
		//メッセージ変数
		string m_Message;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	m1  第1メッセージ（警告文）
		@param[in]	m2  第2メッセージ（問題となったデータ）
		@param[in]	m3  第3メッセージ（発生した位置）
		*/
		//--------------------------------------------------------------------------------------
		BaseMBException(const string& m1, const string& m2, const string& m3)
		{
			m_Message = m1;
			m_Message += "\r\n";
			m_Message += m2;
			m_Message += "\r\n";
			m_Message += m3;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	メッセージの取得（マルチバイト）
		@return	メッセージ文字列の参照
		*/
		//--------------------------------------------------------------------------------------
		const string& what_m() const throw(){
			return m_Message;
		}
	};

	//--------------------------------------------------------------------------------------
	/*!
	@brief インターフェイスの例外
	@param[in]	hr	戻り値。通常はここにHRESULTを返す関数を記述する
	@param[in]	Str1	第1メッセージ（警告文）
	@param[in]	Str2	第2メッセージ（問題となったデータ）
	@param[in]	Str3	第3メッセージ（発生した位置）
	@return	HRESULT
	*/
	//--------------------------------------------------------------------------------------
	inline HRESULT ThrowIfFailed(HRESULT hr, const wstring& Str1, const wstring& Str2, const wstring& Str3)
	{
		if (FAILED(hr))
		{
			throw BaseException(
				Str1,
				Str2,
				Str3
				);
		}
		return hr;
	}


	//--------------------------------------------------------------------------------------
	///	バイナリデータ読み込み。DirectXTKカスタマイズ
	//--------------------------------------------------------------------------------------
	class BinaryReader
	{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	fileName	ファイル名
		*/
		//--------------------------------------------------------------------------------------
		explicit BinaryReader(const wstring& fileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ（メモリから読み込む）
		@param[in]	dataBlob	データの先頭ポインタ
		@param[in]	dataSize	サイズ
		*/
		//--------------------------------------------------------------------------------------
		BinaryReader(_In_reads_bytes_(dataSize) uint8_t const* dataBlob, size_t dataSize);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	T型のデータを1つ読み込む
		@tparam	T	データの型
		@return	T型の参照
		*/
		//--------------------------------------------------------------------------------------
		template<typename T> T const& Read()
		{
			return *ReadArray<T>(1);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	T型の配列を指定数だけ読み込む
		@tparam	T	配列の型
		@param[in]	elementCount	読み込むサイズ（数）
		@return	T型のポインタ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T> T const* ReadArray(size_t elementCount)
		{
			static_assert(std::is_pod<T>::value, "Can only read plain-old-data types");
			uint8_t const* newPos = mPos + sizeof(T)* elementCount;
			if (newPos > mEnd){
				throw std::exception("End of file");
			}
			auto result = reinterpret_cast<T const*>(mPos);
			mPos = newPos;
			return result;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 低レベルヘルパー（ファイルからメモリへの読み込み）
		@param[in]	fileName	ファイル名
		@param[in,out]	data	データの参照
		@param[out]	dataSize	読み込んだ数
		@return	HRESULT型
		*/
		//--------------------------------------------------------------------------------------
		static HRESULT ReadEntireFile(const wstring& fileName, _Inout_ std::unique_ptr<uint8_t[]>& data, _Out_ size_t* dataSize);
	private:
		// The data currently being read.
		uint8_t const* mPos;
		uint8_t const* mEnd;
		std::unique_ptr<uint8_t[]> mOwnedData;
		//コピー禁止
		BinaryReader(const BinaryReader&) = delete;
		BinaryReader& operator=(const BinaryReader&) = delete;
		//ムーブ禁止
		BinaryReader(const BinaryReader&&) = delete;
		BinaryReader& operator=(const BinaryReader&&) = delete;
	};




	//--------------------------------------------------------------------------------------
	///	Csvファイルアクセスクラス
	//--------------------------------------------------------------------------------------
	class CsvFile{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		CsvFile();
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		@param[in]	FileName	CSVファイル名
		*/
		//--------------------------------------------------------------------------------------
		explicit CsvFile(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CsvFile();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ファイル名を得る
		@return	csvファイル名
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetFileName() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ファイル名をセットする
		@param[in]	FileName	CSVファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFileName(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	CSV配列を得る
		@return	CSV配列
		*/
		//--------------------------------------------------------------------------------------
		vector< wstring >& GetCsvVec()const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Csvを読み込む<br />
		＊すでに持ってるデータは破棄される<br/>
		＊Csvファイルが存在しない場合は初期化したうえでfalseを返す。<br />
		＊存在しても読み込みに失敗したら例外
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		bool ReadCsv();
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Csvを書き出す<br />
		＊Csvファイルが存在しなければ作成する、存在していたら、データは初期化される<br/>
		＊アクセスに失敗し、例外処理フラグが立ってたら例外<br />
		＊シーンやステージの終了時にデストラクタで保存処理ができるように、例外処理を選べる<br />
		@param[in]	Exp = true	例外処理するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SaveCsv(bool Exp = true);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 1行最後尾に追加する.
		@param[in]	Row	Csvの1行（各セルは配列化されている必要がある）
		@return	追加した行番号
		*/
		//--------------------------------------------------------------------------------------
		size_t AddRow(vector<wstring>& Row);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1行最後尾に追加する.
		@param[in]	Row	Csvの1行（文字列は、「,」で区切られている必要がある）
		@return	追加した行番号
		*/
		//--------------------------------------------------------------------------------------
		size_t AddRow(wstring& Row);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	指定した1行を変更する.<br />
		＊必ずしも更新前と列の数は同じでなくてもよい。<br />
		＊指定した行が存在しない場合、例外を発生する<br />
		＊シーンやステージの終了時にデストラクタで保存処理ができるように、例外処理を選べる
		@param[in]	RowNum	変更したい行
		@param[in]	Row	Csvの1行（各セルは配列化されている必要がある）
		@param[in]	Exp = true	例外処理するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateRow(size_t RowNum, vector<wstring>& Row, bool Exp = true);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 指定した1行を変更する.
		＊Rowは「,」によって区切られ配列化され保存される<br />
		＊必ずしも更新前と列の数は同じでなくてもよい。<br />
		＊指定した行が存在しない場合、例外を発生する<br />
		＊シーンやステージの終了時にデストラクタで保存処理ができるように、例外処理を選べる
		@param[in]	RowNum	変更したい行
		@param[in]	Row	Csvの1行（文字列は、「,」で区切られている必要がある）
		@param[in]	Exp = true	例外処理するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateRow(size_t RowNum, wstring& Row, bool Exp = true);
		//--------------------------------------------------------------------------------------
		/*!
		@brief 1つのセルを設定する.
		@param[in]	RowNum	変更行
		@param[in]	ColNum	変更列
		@param[in]	Cell,	差し替える文字列
		@param[in]	Exp = true	例外処理するかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateCell(size_t RowNum, size_t ColNum, wstring& Cell, bool Exp = true);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	行数を得る.
		@return	行数
		*/
		//--------------------------------------------------------------------------------------
		size_t GetRowCount() const;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1行分の配列を得る.<br />
		＊RowNumが配列の上限を超えた場合は例外<br />
		＊受け取る文字列の配列が空白の状態のこともある
		@param[in]	RowNum	受け取りたい行
		@param[out]	Row	1行を受け取る文字列の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetRowVec(size_t RowNum, vector<wstring>& Row);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1つのセルを得る.<br />
		＊RowNumまたはColNumが配列の上限を超えた場合は例外<br/>
		＊受け取る文字列が空白の状態のこともある
		@param[in]	RowNum	受け取りたい行
		@param[in]	ColNum	受け取りたい列
		@return	セルの中身
		*/
		//--------------------------------------------------------------------------------------
		wstring GetCell(size_t RowNum, size_t ColNum);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	検査する列が条件キーと同じ行を抜出し文字列の配列に返す.<br />
		＊RetVecはクリアされる<br />
		＊受け取る文字列の配列が空白の状態のこともある<br />
		＊戻り値の各行は、デリミタに区切られた文字列なので<br />
		Util::WStrToTokenVectorなどで再処理する
		@param[out]	RetVec	結果を受け取る文字列の配列
		@param[in]	ColNum	条件を検査する列
		@param[in]	Key	条件キー
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetSelect(vector< wstring >& RetVec, size_t ColNum, const wstring& Key);
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コールバック関数を呼び出し、trueが返った行を抜出し文字列の配列に返す.<br />
		＊RetVecはクリアされる<br />
		＊受け取る文字列の配列が空白の状態のこともある<br />
		＊戻り値の各行は、デリミタに区切られた文字列なので<br />
		Util::WStrToTokenVectorなどで再処理する
		@param[out]	RetVec	結果を受け取る文字列の配列
		@param[in]	(Func)(const wstring&)	条件を検査するコールバック関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void GetSelect(vector< wstring >& RetVec,bool (Func)(const wstring&));
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ラムダ式を呼び出し、trueが返った行を抜出し文字列の配列に返す.<br />
		＊RetVecはクリアされる<br />
		＊受け取る文字列の配列が空白の状態のこともある<br />
		＊戻り値の各行は、デリミタに区切られた文字列なので<br />
		Util::WStrToTokenVectorなどで再処理する
		@tparam	Fct	ラムダ式の型
		@param[out]	RetVec	結果を受け取る文字列の配列
		@param[in]	f	条件を検査するラムダ式
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename Fct>
		void GetSelect2(vector< wstring >& RetVec, Fct f){
			//渡された配列に値が入ってる場合があるのでクリア
			RetVec.clear();
			auto& CsvVec = GetCsvVec();
			size_t sz = CsvVec.size();
			for (size_t i = 0; i < sz; i++){
				//コールバック関数を呼び出す
				if (f(CsvVec[i])){
					//条件があれば追加
					RetVec.push_back(CsvVec[i]);
				}
			}
		}
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
		//コピー禁止
		CsvFile(const CsvFile&) = delete;
		CsvFile& operator=(const CsvFile&) = delete;
		//ムーブ禁止
		CsvFile(const CsvFile&&) = delete;
		CsvFile& operator=(const CsvFile&&) = delete;
	};


	//--------------------------------------------------------------------------------------
	/*!
	@brief	比較のエラーの文字列を作成する.
	@tparam	T	比較する型
	@param[in]	key1	キー1名
	@param[in]	val1	値１
	@param[in]	key2	キー２名
	@param[in]	val2	値2
	@return	エラー文字列
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	wstring MakeRangeErr(const wchar_t* key1, T val1, const wchar_t* key2, T val2){
		//返す文字列
		wstring str;
		//書式を整えるストリーム
		wostringstream stream;
		stream << key1 << L"==" << val1 << L"," << key2 << L"==" << val2;
		str = stream.str();
		return str;
	}

	//--------------------------------------------------------------------------------------
	/*!
	@brief	安全にポインタをdeleteする.
	@tparam	T	ポインタの型
	@param[in,out]	p	T型のポインタの参照
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	inline void SafeDelete(T*& p){
		if (p){
			delete p;
		}
		p = nullptr;
	}
	//--------------------------------------------------------------------------------------
	/*!
	@brief	安全に配列をdeleteする.
	@tparam	T	配列の型
	@param[in,out]	p	T型の配列の参照
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	inline void SafeDeleteArr(T*& p){
		if (p){
			delete[] p;
		}
		p = nullptr;
	}
	//--------------------------------------------------------------------------------------
	/*!
	@brief	安全にインターフェイスをreleaseする.
	@tparam	T	インターフェイスの型
	@param[in,out]	p  T型のインターフェイスのポインタの参照
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	inline void SafeRelease(T*& p){
		if (p){
			p->Release();
		}
		p = nullptr;
	}
	//--------------------------------------------------------------------------------------
	/*!
	@brief	ポインタのリストを安全にクリアする.
	@tparam	T	リストのポインタ型
	@param[in,out]	List	T型のポインタのリストの参照
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	void SefeDeletePointerList(list<T*>& List){
		list<T*>::iterator it;
		for (it = List.begin(); it != List.end(); ++it){
			delete (*it);
		}
		List.clear();
	}
	//--------------------------------------------------------------------------------------
	/*!
	@brief	ポインタの配列（vector）を安全にクリアする.
	@tparam	T	配列のポインタ型
	@param[in,out]	Vec	T型のポインタの配列の参照
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	void SafeDeletePointerVector(vector<T*>& Vec){
		size_t maxsz = Vec.size();
		for (size_t i = 0; i < maxsz; i++){
			delete Vec[i];
		}
		Vec.clear();
	}

	//--------------------------------------------------------------------------------------
	/*!
	@brief	インターフェイスのリストを安全にリリースする.
	@tparam	T	インターフェイス型
	@param[in,out]	List	T型のポインタのリストの参照
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	void SefeReleasePointerList(list<T*>& List){
		list<T*>::iterator it;
		for (it = List.begin(); it != List.end(); ++it){
			if (*it){
				(*it)->Release();
			}
			*it = nullptr;
		}
		List.clear();
	}
	//--------------------------------------------------------------------------------------
	/*!
	@brief	インターフェイスの配列（vector）を安全にリリースする.
	@tparam	T	インターフェイス型
	@param[in,out]	Vec	T型インターフェイスのポインタ配列の参照
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	void SafeReleasePointerVector(vector<T*>& Vec){
		size_t maxsz = Vec.size();
		for (size_t i = 0; i < maxsz; i++){
			if (Vec[i]){
				Vec[i]->Release();
			}
			Vec[i] = nullptr;
		}
		Vec.clear();
	}
	//--------------------------------------------------------------------------------------
	/*!
	@brief	shared_ptrをvoid型に変換する.
	@tparam	T	shared_ptr型
	@param[in]	SrcPtr	変換元
	@return	void型のshared_ptr
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	shared_ptr<void> SharedToVoid(const shared_ptr<T>& SrcPtr){
		shared_ptr<void> RetPtr = static_pointer_cast<void>(SrcPtr);
		return RetPtr;
	}
	//--------------------------------------------------------------------------------------
	/*!
	@brief	void型のshared_ptrをT型のshared_ptrに変換する.
	@tparam	T	shared_ptr型
	@param[in]	SrcPtr	変換元
	@return	T型のshared_ptr
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	shared_ptr<T> VoidToShared(const shared_ptr<void>& SrcPtr){
		shared_ptr<T> RetPtr = static_pointer_cast<T>(SrcPtr);
		return RetPtr;
	}
	//--------------------------------------------------------------------------------------
	///	その他のユーティリティ。static呼び出しをする
	//--------------------------------------------------------------------------------------
	struct Util{
		//--------------------------------------------------------------------------------------
		/*!
		@brief	大きい方を求める.
		@tparam	T	比較する型
		@param[in]	v1	T型の値1
		@param[in]	v2	T型の値2
		@return	T型の大きい方
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static T Maximum(const T& v1, const T& v2){
			return v1 > v2 ? v1 : v2;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	小さい方を求める.
		@tparam	T	比較する型
		@param[in]	v1	T型の値1
		@param[in]	v2	T型の値2
		@return	T型の小さい方
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static T Minimum(const T& v1, const T& v2){
			return v1 < v2 ? v1 : v2;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	安全にCOMインターフェイスのポインタを取得するヘルパー関数.
		@tparam	T	取得する型
		@tparam	TCreateFunc	作成関数の型
		@param[in,out]	comPtr	COMポインタ
		@param[in]	mutex	ミューテックス
		@param[in]	createFunc	作成関数
		@return	COMが無効の場合は新しく作成して、それ以外はCOMから得たCOMインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename TCreateFunc>
		static T* DemandCreate(ComPtr<T>& comPtr, std::mutex& mutex, TCreateFunc createFunc)
		{
			T* result = comPtr.Get();
			//ロック状態をチェック
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

		//--------------------------------------------------------------------------------------
		/*!
		@brief	安全にCOMインターフェイスのポインタを取得するヘルパー関数.<br />
		無効の場合は例外を出す
		@tparam	T	取得する型
		@param[in]	comPtr	COMポインタ
		@return　COMが無効の場合は例外を出して、それ以外はCOMから得たCOMインターフェイスのポインタ
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static T* GetComPtr(ComPtr<T>& comPtr){
			T* result = comPtr.Get();
			//ロック状態をチェック
			MemoryBarrier();
			if (!result)
			{
				//失敗
				throw BaseException(
					L"このCOMインターフェイスを取得できません",
					Util::GetWSTypeName<T>(),
					L"Util::GetComPtr()"
					);
			}
			return result;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	両端から空白を取り除く<br/>
		元になる文字列から取り除く
		@param[in,out]	wstr	もとになる文字列（リターンにも使用される）
		@param[in]	trimCharacterList = L" \t\v\r\n"	取り除く文字
		@return　なし
		*/
		//--------------------------------------------------------------------------------------
		static void WStrTrim(wstring& wstr, const wchar_t* TrimCharList = L" \t\v\r\n"){
			wstring result(L"");
			if (wstr.size() <= 0){
				wstr = result;
				return;
			}
			wstring::size_type left = wstr.find_first_not_of(TrimCharList);
			if (left != wstring::npos){
				wstring::size_type right = wstr.find_last_not_of(TrimCharList);
				result = wstr.substr(left, right - left + 1);
			}
			wstr = result;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワイド文字列からマルチバイト文字列変換<br />
		ロケール依存のため、WinMain()等で、setlocale( LC_ALL, "JPN" );が必要
		@param[in] src	変換する文字列（ワイドキャラ）
		@param[out]	dest	変換後の文字列（マルチバイト）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void WStoMB(const wstring& src, string& dest){
			size_t i;
			char *pMBstr = new char[src.length() * MB_CUR_MAX + 1];
			wcstombs_s(
				&i,
				pMBstr,
				src.length() * MB_CUR_MAX + 1,
				src.c_str(),
				_TRUNCATE	//すべて変換できなかったら切り捨て
				);
			dest = pMBstr;
			delete[] pMBstr;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	ワイド文字列からマルチバイトUTF8文字列変換<br />
		ロケール依存のため、WinMain()等で、setlocale( LC_ALL, "JPN" );が必要
		@param[in]	src	変換する文字列（ワイドキャラ）
		@param[out]	dest	変換後の文字列（マルチバイト）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ConvertWstringtoUtf8(const wstring& src, string& dest){
			INT bufsize = ::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
			char* Temp = new char[bufsize + 1];
			::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, Temp, bufsize, NULL, NULL);
			dest = Temp;
			delete[] Temp;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチバイトUTF8文字からワイド文字列変換<br />
		ロケール依存のため、WinMain()等で、setlocale( LC_ALL, "JPN" );が必要
		@param[in]	src	変換する文字列（マルチバイトUTF8）
		@param[out]	dest	変換後文字列（ワイドキャラ）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void ConvertUtf8toWstring(const string& src, wstring& dest){
			INT bufsize = ::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, (wchar_t *)NULL, 0);
			wchar_t* Temp = (wchar_t*)new wchar_t[bufsize];
			::MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, Temp, bufsize);
			dest = Temp;
			delete[] Temp;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	マルチバイト文字列からワイド文字列変換<br />
		ロケール依存のため、WinMain()等で、setlocale( LC_ALL, "JPN" );が必要
		@param[in]	src	変換する文字列（マルチバイト）
		@param[out]	dest	変換後の文字列（ワイドキャラ）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void MBtoWS(const string& src, wstring& dest){
			size_t i;
			wchar_t* WCstr = new wchar_t[src.length() + 1];
			mbstowcs_s(
				&i,
				WCstr,
				src.length() + 1,
				src.c_str(),
				_TRUNCATE //すべて変換できなかったら切り捨て
				);
			dest = WCstr;
			delete[] WCstr;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	文字列をデリミタ文字により区切って配列に保存
		@param[out]	wstrvec	保存される配列
		@param[in]	line	元となる文字列
		@param[in]	delimiter	デリミタ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		static void WStrToTokenVector(vector<wstring>& wstrvec, const wstring& line, wchar_t delimiter){
			wstring::size_type i = 0;
			wstring wks(L"");
			for (i = 0; i < line.size(); i++){
				if (line[i] == delimiter){
					if (wks != L""){
						wstrvec.push_back(wks);
					}
					wks = L"";
				}
				else{
					wks += line[i];
				}
			}
			wstrvec.push_back(wks);
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	T型の内部機器別名を得る（wstring）
		@tparam T	型名を得る型
		@return	T型の内部機器別名
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static wstring GetWSTypeName(){
			wstring clsname;
			MBtoWS(typeid(T).name(), clsname);
			return clsname;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	T型の内部機器別名を得る（string）
		@tparam T	型名を得る型
		@return	T型の内部機器別名
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		static string GetMBTypeName(){
			string clsname = typeid(T).name();
			return clsname;
		}

		//--------------------------------------------------------------------------------------
		/// float型を文字列に変換する場合の形式
		//--------------------------------------------------------------------------------------
		enum FloatModify{
			Default = 0,	///< デフォルト（浮動小数点）
			Fixed,	///< 数字を出力
			Scientific,	///< e+09などの形式
		};

		//--------------------------------------------------------------------------------------
		/*!
		@brief	浮動小数点を文字列に変換する（wstring版）
		@param[in]	Val	浮動小数点値
		@param[in]	Precision = 0	精度0でデフォルト
		@param[in]	Modify = FloatModify::Default	表示形式
		@return	変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static wstring FloatToWStr(float Val, streamsize Precision = 0,
			FloatModify Modify = FloatModify::Default){
			//返す文字列
			wstring str;
			//書式を整えるストリーム
			wostringstream stream;
			//浮動小数点の精度を決める
			if (Precision > 0){
				stream.precision(Precision);
			}
			switch (Modify){
			case FloatModify::Fixed:
				stream << std::fixed << Val;
				break;
			case FloatModify::Scientific:
				stream << std::scientific << Val;
				break;
			case FloatModify::Default:
			default:
				stream << Val;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	浮動小数点を文字列に変換する（string版）
		@param[in]	Val	浮動小数点値
		@param[in]	Precision = 0	精度0でデフォルト
		@param[in]	Modify = FloatModify::Default	表示形式
		@return	変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static string FloatToStr(float Val, streamsize Precision = 0,
			FloatModify Modify = FloatModify::Default){
			//返す文字列
			string str;
			//書式を整えるストリーム
			ostringstream stream;
			//浮動小数点の精度を決める
			if (Precision > 0){
				stream.precision(Precision);
			}
			switch (Modify){
			case FloatModify::Fixed:
				stream << std::fixed << Val;
				break;
			case FloatModify::Scientific:
				stream << std::scientific << Val;
				break;
			case FloatModify::Default:
			default:
				stream << Val;
				break;
			}
			str = stream.str();
			return str;
		}


		//--------------------------------------------------------------------------------------
		/// 整数型を文字列に変換する場合の形式
		//--------------------------------------------------------------------------------------
		enum NumModify{
			Dec = 0,	///< 10進数
			Hex,	///< 16進数
			Oct,	///< 8進数
		};
		//--------------------------------------------------------------------------------------
		/*!
		@brief	unsigned数値を文字列に変換する（wstring版）
		@param[in]	num	unsigned数値
		@param[in]	Modify = NumModify::Dec	進数
		@return	変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static wstring UintToWStr(UINT num, NumModify Modify = NumModify::Dec){
			//返す文字列
			wstring str;
			//書式を整えるストリーム
			wostringstream stream;
			//表示形式を決める
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	unsigned数値を文字列に変換する（string版）
		@param[in]	num	unsigned数値
		@param[in]	Modify = NumModify::Dec	進数
		@return	変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static string UintToStr(UINT num, NumModify Modify = NumModify::Dec){
			//返す文字列
			string str;
			//書式を整えるストリーム
			ostringstream stream;
			//表示形式を決める
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	signed数値を文字列に変換する（wstring版）
		@param[in]	num	signed数値
		@param[in]	Modify = NumModify::Dec	進数
		@return	変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static wstring IntToWStr(int num, NumModify Modify = NumModify::Dec){
			//返す文字列
			wstring str;
			//書式を整えるストリーム
			wostringstream stream;
			//表示形式を決める
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	signed数値を文字列に変換する（string版）
		@param[in]	num	signed数値
		@param[in]	Modify = NumModify::Dec	進数
		@return	変換した文字列
		*/
		//--------------------------------------------------------------------------------------
		static string IntToStr(int num, NumModify Modify = NumModify::Dec){
			//返す文字列
			string str;
			//書式を整えるストリーム
			ostringstream stream;
			//表示形式を決める
			switch (Modify){
			case NumModify::Oct:
				stream << std::oct << num;
				break;
			case NumModify::Hex:
				stream << std::hex << num;
				break;
			case NumModify::Dec:
			default:
				stream << std::dec << num;
				break;
			}
			str = stream.str();
			return str;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	Probability分の1の確率で、trueを返す<br />
		Probabilityは0より大きくなければならない（0除算はできない）
		@param[in]	Probability	確率。何分の1かを指定する
		@return	Probability分の1の確率になればtrue
		*/
		//--------------------------------------------------------------------------------------
		static bool DivProbability(UINT Probability){
			if (Probability <= 0){
				return false;
			}
			double Prob = (double)rand() / (double)RAND_MAX;
			double Seed = 1.0 / (double)Probability;
			if (Prob <= Seed){
				return true;
			}
			return false;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	0から1.0fの間の乱数を返す<br />
		ZeroOKがfalseの場合、0が返る可能性はない（0.00001fになる）
		@param[in]	ZeroOK = false	0が返っていいかどうかの指定
		@return	0から1.0fの間の乱数
		*/
		//--------------------------------------------------------------------------------------
		static float RandZeroToOne(bool ZeroOK = false){
			double Prob = (double)rand() / (double)RAND_MAX;
			if (Prob <= 0){
				if (!ZeroOK){
					Prob = 0.00001;
				}
				else{
					Prob = 0.0;
				}
			}
			return (float)Prob;
		}

	};

	//--------------------------------------------------------------------------------------
	/*!
	@breaf ランタイム例外をthrowする（wstring版）
	@param[in] m1  第1メッセージ（警告文）
	@param[in] m2  第2メッセージ（問題となったデータ）
	@param[in] m3  第3メッセージ（発生した位置）
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	inline void ThrowBaseException(const wstring& m1, const wstring& m2, const wstring& m3) {
		wstring AllMess = m1;
		AllMess += L"\r\n";
		AllMess += m2;
		AllMess += L"\r\n";
		AllMess += m3;
		string Dest;
		Util::WStoMB(AllMess, Dest);
		throw runtime_error(Dest);
	}
	//--------------------------------------------------------------------------------------
	/*!
	@breaf ランタイム例外をthrowする（string版）
	@param[in] m1  第1メッセージ（警告文）
	@param[in] m2  第2メッセージ（問題となったデータ）
	@param[in] m3  第3メッセージ（発生した位置）
	@return	なし
	*/
	//--------------------------------------------------------------------------------------
	inline void ThrowBaseException(const string& m1, const string& m2, const string& m3) {
		string AllMess = m1;
		AllMess += "\r\n";
		AllMess += m2;
		AllMess += "\r\n";
		AllMess += m3;
		throw runtime_error(AllMess);
	}

	//--------------------------------------------------------------------------------------
	/*!
	@brief	T型のポインタがnullだった時のスロー（wstring版）
	@tparam T	型名を得る型
	@param[in]	ptr	検証するポインタ
	@param[in]	Str1	メッセージ1
	@param[in]	Str2	メッセージ2
	@param[in]	Str3	メッセージ3
	@return	T型のポインタ
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	shared_ptr<T> ThrowIfNullFailed(const shared_ptr<T>& ptr, const wstring& Str1, const wstring& Str2, const wstring& Str3)
	{
		if (!ptr) {
			ThrowBaseException(
				Str1,
				Str2,
				Str3
			);
		}
		return ptr;
	}
	//--------------------------------------------------------------------------------------
	/*!
	@brief	T型のポインタがnullだった時のスロー（string版）
	@tparam T	型名を得る型
	@param[in]	ptr	検証するポインタ
	@param[in]	Str1	メッセージ1
	@param[in]	Str2	メッセージ2
	@param[in]	Str3	メッセージ3
	@return	T型のポインタ
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	shared_ptr<T> ThrowIfNullFailed(const shared_ptr<T>& ptr, const string& Str1, const string& Str2, const string& Str3)
	{
		if (!ptr) {
			ThrowBaseException(
				Str1,
				Str2,
				Str3
			);
		}
		return ptr;
	}
	//--------------------------------------------------------------------------------------
	/*!
	@brief	T型のweakptrが無効だった時のスロー（string版）
	@tparam T	型名を得る型
	@param[in]	SrcPtr	検証するweakptr
	@return	T型のshared_ptr
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	shared_ptr<T> ThrowIfWeakToSharedFailed(const weak_ptr<T>& SrcPtr) {
		auto shptr = SrcPtr.lock();
		if (!shptr) {
			string Str = typeid(T).name();
			string Mess = Str += "は不定値になっています。"
				ThrowBaseException(
					"weak_ptrからshared_ptrへの変換に失敗しました。",
					Mess,
					"ThrowIfWeakToShared<T>()"
				);
		}
		return shptr;
	}





	typedef long long int64;
	typedef unsigned long long uint64;
	typedef unsigned int uint32;


	//--------------------------------------------------------------------------------------
	///	ステップタイマー
	//--------------------------------------------------------------------------------------
	class StepTimer{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		StepTimer() :
			m_elapsedTicks(0),
			m_totalTicks(0),
			m_leftOverTicks(0),
			m_frameCount(0),
			m_framesPerSecond(0),
			m_framesThisSecond(0),
			m_qpcSecondCounter(0),
			m_isFixedTimeStep(false),
			m_targetElapsedTicks(TicksPerSecond / 60)
		{
			if (!QueryPerformanceFrequency(&m_qpcFrequency))
			{
				throw BaseException(
					L"パフォーマンスカウンタの周波数を取得できません。",
					L"if (!QueryPerformanceFrequency(&m_qpcFrequency))",
					L"StepTimer::StepTimer()"
					);
			}

			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw BaseException(
					L"パフォーマンスカウンタを取得できません。",
					L"if (!QueryPerformanceCounter(&m_qpcLastTime))",
					L"StepTimer::StepTimer()"
					);
			}

			// 最大デルタを 1 秒の 1/10 に初期化します。
			m_qpcMaxDelta = m_qpcFrequency.QuadPart / 10;
		}

		//--------------------------------------------------------------------------------------
		/*!
		@brief	前の Update 呼び出しから経過したカウンタを取得.
		@return	経過カウンタ
		*/
		//--------------------------------------------------------------------------------------
		uint64 GetElapsedTicks() const						{ return m_elapsedTicks; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief 前の Update 呼び出しから経過した時間を取得.
		@return	経過時間
		*/
		//--------------------------------------------------------------------------------------
		double GetElapsedSeconds() const					{ return TicksToSeconds(m_elapsedTicks); }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プログラム開始から経過した合計カウンタを取得.
		@return	合計カウンタ
		*/
		//--------------------------------------------------------------------------------------
		uint64 GetTotalTicks() const						{ return m_totalTicks; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プログラム開始から経過した合計時間を取得.
		@return	合計時間
		*/
		//--------------------------------------------------------------------------------------
		double GetTotalSeconds() const						{ return TicksToSeconds(m_totalTicks); }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プログラム開始からの合計更新回数を取得.
		@return	合計更新回数
		*/
		//--------------------------------------------------------------------------------------
		uint32 GetFrameCount() const						{ return m_frameCount; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	現在のフレーム レートを取得.
		@return	現在のフレーム レート
		*/
		//--------------------------------------------------------------------------------------
		uint32 GetFramesPerSecond() const					{ return m_framesPerSecond; }

		//--------------------------------------------------------------------------------------
		/*!
		@brief	固定または可変のどちらのタイムステップ モードを使用するかを設定（デフォルトは可変）.
		@param[in]	isFixedTimestep	固定モードにするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetFixedTimeStep(bool isFixedTimestep)			{ m_isFixedTimeStep = isFixedTimestep; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	固定タイムステップ モードで、Update の呼び出し頻度を設定.
		@param[in]	targetElapsed	呼び出し頻度（カウンタ）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTargetElapsedTicks(uint64 targetElapsed)	{ m_targetElapsedTicks = targetElapsed; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	固定タイムステップ モードで、Update の呼び出し頻度を設定.
		@param[in]	targetElapsed	呼び出し頻度（時間）
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetTargetElapsedSeconds(double targetElapsed)	{ m_targetElapsedTicks = SecondsToTicks(targetElapsed); }
		// 整数形式は 1 秒あたり 10,000,000 ティックを使用して時間を表します。
		static const uint64 TicksPerSecond = 10000000;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	チックカウンタか時間に換算.
		@param[in]	ticks		チックカウンタ
		@return	換算された時間
		*/
		//--------------------------------------------------------------------------------------
		static double TicksToSeconds(uint64 ticks)			{ return static_cast<double>(ticks) / TicksPerSecond; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	時間をチックカウンタに換算.
		@param[in]	seconds	時間
		@return	換算されたチックカウンタ
		*/
		//--------------------------------------------------------------------------------------
		static uint64 SecondsToTicks(double seconds)		{ return static_cast<uint64>(seconds * TicksPerSecond); }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	呼び出しの更新<br />
		意図的なタイミングの不連続性の後 (ブロック IO 操作など)<br />
		これを呼び出すと、固定タイムステップ ロジックによって一連のキャッチアップが試行されるのを回避できる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void ResetElapsedTime()
		{
			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw BaseException(
					L"パフォーマンスカウンタを取得できません。",
					L"if (!QueryPerformanceCounter(&m_qpcLastTime))",
					L"StepTimer::ResetElapsedTime()"
					);
			}

			m_leftOverTicks = 0;
			m_framesPerSecond = 0;
			m_framesThisSecond = 0;
			m_qpcSecondCounter = 0;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	タイマー状態を更新し、指定の Update 関数を適切な回数だけ呼び出す
		@tparam	呼び出すべきUpdate関数型
		@param[in]	update	呼び出すべきUpdate関数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		template<typename TUpdate>
		void Tick(const TUpdate& update)
		{
			// 現在の時刻をクエリします。
			LARGE_INTEGER currentTime;

			if (!QueryPerformanceCounter(&currentTime))
			{
				throw BaseException(
					L"パフォーマンスカウンタを取得できません。",
					L"if (!QueryPerformanceCounter(&m_qpcLastTime))",
					L"StepTimer::ResetElapsedTime()"
					);
			}

			uint64 timeDelta = currentTime.QuadPart - m_qpcLastTime.QuadPart;

			m_qpcLastTime = currentTime;
			m_qpcSecondCounter += timeDelta;

			// 極端に大きな時間差 (デバッガーで一時停止した後など)  をクランプします。
			if (timeDelta > m_qpcMaxDelta)
			{
				timeDelta = m_qpcMaxDelta;
			}

			// QPC 単位を標準の目盛り形式に変換します。前のクランプが発生したため、この変換ではオーバーフローが不可能です。
			timeDelta *= TicksPerSecond;
			timeDelta /= m_qpcFrequency.QuadPart;

			uint32 lastFrameCount = m_frameCount;

			if (m_isFixedTimeStep)
			{
				// 固定タイムステップ更新ロジック

				// クランプしたターゲット経過時間 (1/4 ミリ秒以内) に接近して、アプリケーションが実行されている場合
				// ターゲット値に正確に一致させるクロック。これにより、小さい無関係なエラーの発生を防ぎます。
				// 一定期間内に蓄積された。このクランプなしで、60 fps を要求したゲームは
				// 固定された更新 (59.94 NTSC ディスプレイ上で有効な vsync で実行) は最終的には、
				// フレームをドロップするのに十分な小さいエラーを蓄積します。丸めることをお勧めします
				// わずかな偏差をゼロに持っていき、スムーズに進行するようにします。

				if (abs(static_cast<int64>(timeDelta - m_targetElapsedTicks)) < TicksPerSecond / 4000)
				{
					timeDelta = m_targetElapsedTicks;
				}

				m_leftOverTicks += timeDelta;

				while (m_leftOverTicks >= m_targetElapsedTicks)
				{
					m_elapsedTicks = m_targetElapsedTicks;
					m_totalTicks += m_targetElapsedTicks;
					m_leftOverTicks -= m_targetElapsedTicks;
					m_frameCount++;

					update();
				}
			}
			else
			{
				// 可変タイムステップ更新ロジック。
				m_elapsedTicks = timeDelta;
				m_totalTicks += timeDelta;
				m_leftOverTicks = 0;
				m_frameCount++;

				update();
			}

			// 現在のフレーム レートを追跡します。
			if (m_frameCount != lastFrameCount)
			{
				m_framesThisSecond++;
			}

			if (m_qpcSecondCounter >= static_cast<uint64>(m_qpcFrequency.QuadPart))
			{
				m_framesPerSecond = m_framesThisSecond;
				m_framesThisSecond = 0;
				m_qpcSecondCounter %= m_qpcFrequency.QuadPart;
			}
		}

	private:
		// ソース タイミング データでは QPC 単位を使用します。
		LARGE_INTEGER m_qpcFrequency;
		LARGE_INTEGER m_qpcLastTime;
		uint64 m_qpcMaxDelta;

		// 派生タイミング データでは、標準の目盛り形式を使用します。
		uint64 m_elapsedTicks;
		uint64 m_totalTicks;
		uint64 m_leftOverTicks;

		// フレーム レートの追跡用メンバー。
		uint32 m_frameCount;
		uint32 m_framesPerSecond;
		uint32 m_framesThisSecond;
		uint64 m_qpcSecondCounter;

		// 固定タイムステップ モードの構成用メンバー。
		bool m_isFixedTimeStep;
		uint64 m_targetElapsedTicks;

		//コピー禁止
		StepTimer(const StepTimer&) = delete;
		StepTimer& operator=(const StepTimer&) = delete;
		//ムーブ禁止
		StepTimer(const StepTimer&&) = delete;
		StepTimer& operator=(const StepTimer&&) = delete;

	};


	//--------------------------------------------------------------------------------------
	///	2次元ポイント
	/*!
	@tparam	T	ポイントを実装する型
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	struct Point2D{
		T x;
		T y;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Point2D() throw() :
			x{},
			y{}
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	initX	初期値X
		@param[in]	initY	初期値Y
		*/
		//--------------------------------------------------------------------------------------
		Point2D(T initX, T initY) throw() :
			x{ initX },
			y{ initY }
		{
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	等しいかどうか比べる
		@param[in]	point	相手
		@return	等しければtrue
		*/
		//--------------------------------------------------------------------------------------
		bool operator==(Point2D<T> point) const throw(){
			return (x == point.x && y == point.y);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	等しくないかどうか比べる
		@param[in]	point	相手
		@return	等しくなければtrue
		*/
		//--------------------------------------------------------------------------------------
		bool operator!=(Point2D<T> point) const throw(){
			return (x != point.x || y != point.y);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	追加加算
		@param[in]	point	相手
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void operator+=(Point2D<T> point) throw(){
			x += point.x; y += point.y;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	追加減算
		@param[in]	point	相手
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void operator-=(Point2D<T> point) throw(){
			x -= point.x; y -= point.y;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	加算
		@param[in]	point	相手
		@return	加算した結果
		*/
		//--------------------------------------------------------------------------------------
		Point2D operator+(Point2D<T> point) const throw(){
			return Point2D(x + point.x, y + point.y);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	減算
		@param[in]	point	相手
		@return	減算した結果
		*/
		//--------------------------------------------------------------------------------------
		Point2D operator-(Point2D<T> point) const throw(){
			return Point2DF(x - point.x, y - point.y);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	単項マイナス
		@return	マイナスを掛けた結果
		*/
		//--------------------------------------------------------------------------------------
		Point2D operator-() const throw(){
			return Point2DF(-x, -y);
		}
	};

	//--------------------------------------------------------------------------------------
	///	2次元矩形
	/*!
	@tparam	T	矩形を実装する型
	*/
	//--------------------------------------------------------------------------------------
	template<typename T>
	struct Rect2D{
		T left;
		T top;
		T right;
		T bottom;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		Rect2D() throw() :
			left{}, top{}, right{}, bottom{}
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	l	初期値Left
		@param[in]	t	初期値Top
		@param[in]	r	初期値Right
		@param[in]	b	初期値Bottom
		*/
		//--------------------------------------------------------------------------------------
		Rect2D(T l, T t, T r, T b)throw() :
			left{ l }, top{ t }, right{ r }, bottom{ b }
		{
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	空かどうか調べる
		@return	空であればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsRectEmpty()const throw(){
			if (left >= right || top >= bottom){
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	幅を得る
		@return	幅
		*/
		//--------------------------------------------------------------------------------------
		T Width()const throw(){
			return abs(right - left);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	高さを得る
		@return	高さ
		*/
		//--------------------------------------------------------------------------------------
		T Height()const throw(){
			return abs(bottom - top);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	加算移動する
		@param[in]	point	移動する分量
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void operator+=(Point2D<T> point) throw(){
			left += point.x;
			right += point.x;
			top += point.y;
			bottom += point.y;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	減算移動する
		@param[in]	point	移動する分量
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void operator-=(Point2D<T> point) throw(){
			left -= point.x;
			right -= point.x;
			top -= point.y;
			bottom -= point.y;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	加算する
		@param[in]	point	加算する分量
		@return	加算後の矩形
		*/
		//--------------------------------------------------------------------------------------
		Rect2D<T> operator+(Point2D<T> point) const throw(){
			Rect2D<T> rect(*this);
			rect += point;
			return rect;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	減算する
		@param[in]	point	減算する分量
		@return	減算後の矩形
		*/
		//--------------------------------------------------------------------------------------
		Rect2D<T> operator-(Point2D<T> point) const throw(){
			Rect2D<T> rect(*this);
			rect -= point;
			return rect;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ポイントが含まれるかどうかを調べる
		@param[in]	point	調べるポイント
		@return	含まれていればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool PtInRect(Point2D<T> point) const throw(){
			if (point.x >= left && point.x < right
				&& point.y >= top && point.y < bottom){
				return true;
			}
			return false;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心点を得る
		@return	中心のポイント
		*/
		//--------------------------------------------------------------------------------------
		Point2D<T> CenterPoint() const throw(){
			return Point2D<T>((left + right) / 2.0f, (top + bottom) / 2.0f);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	中心点をもとにスケーリング
		@param[in]	l	Left変化値
		@param[in]	t	Top変化値
		@param[in]	r	Right変化値
		@param[in]	b	Bottom変化値
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void InflateRect(T l, T t, T r, T b){
			left -= l;			top -= t;
			right += r;			bottom += b;
		}
	};




	class ObjectFactory;
	class ObjectInterface;

	//--------------------------------------------------------------------------------------
	///	イベント構造体
	//--------------------------------------------------------------------------------------
	struct Event {
		///	遅延時間（SendEventの場合は常に0）
		float m_DispatchTime;
		///	このメッセージを送ったオブジェクト
		weak_ptr<ObjectInterface> m_Sender;
		///	受け取るオブジェクト（nullptrの場合はアクティブステージ内すべてもしくはキーワードで識別するオブジェクト）
		weak_ptr<ObjectInterface> m_Receiver;
		///	メッセージ文字列
		wstring m_MsgStr;
		///	追加情報をもつオブジェクトのポインタ
		shared_ptr<void> m_Info;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	DispatchTime	配送までの時間
		@param[in]	Sender	送り側オブジェクト（nullptr可）
		@param[in]	Receiver	受け手側オブジェクト
		@param[in]	MsgStr	メッセージ文字列
		@param[in]	Info	追加情報をもつユーザーデータ
		*/
		//--------------------------------------------------------------------------------------
		Event(float DispatchTime, const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>()) :
			m_DispatchTime(DispatchTime),
			m_Sender(Sender),
			m_Receiver(Receiver),
			m_MsgStr(MsgStr),
			m_Info(Info)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		~Event() {}
	};



	//--------------------------------------------------------------------------------------
	///	CreateとPreCreateを持ち、Thisスマートポインタがとれるインターフェイス
	//--------------------------------------------------------------------------------------
	class ObjectInterface : public std::enable_shared_from_this<ObjectInterface> {
		friend class ObjectFactory;
		//クリエイト済みかどうか
		//Create関数が呼び出し後にtrueになる
		bool m_Created{ false };
		void SetCreated(bool b) {
			m_Created = b;
		}
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief プロテクトコンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		ObjectInterface(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ObjectInterface() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief thisポインタ取得
		@tparam T	thisポインタの型
		@return	thisポインタ（shared_ptr）
		*/
		//--------------------------------------------------------------------------------------
		template<typename T>
		std::shared_ptr<T> GetThis() {
			auto Ptr = dynamic_pointer_cast<T>(shared_from_this());
			if (Ptr) {
				return Ptr;
			}
			else {
				wstring str(L"thisを");
				str += Util::GetWSTypeName<T>();
				str += L"型にキャストできません";
				throw BaseException(
					str,
					L"if( ! dynamic_pointer_cast<T>(shared_from_this()) )",
					L"ObjectInterface::GetThis()"
					);
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 前初期化を行う（仮想関数）<br />
		＊thisポインタが必要なオブジェクトはこの関数を多重定義して、取得できる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreCreate(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化を行う（仮想関数）<br />
		＊thisポインタが必要なオブジェクトはこの関数を多重定義して、取得できる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief クリエイト済みかどうか
		@return	クリエイト済みならtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsCreated() {
			return m_Created;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	イベントのPOST（キューに入れる）
		@param[in]	DispatchTime	POSTする時間（0で次のターン）
		@param[in]	Sender	イベント送信者（nullptr可）
		@param[in]	Receiver	イベント受信者（nullptr不可）
		@param[in]	MsgStr	メッセージ
		@param[in,out]	Info	追加情報
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void PostEvent(float DispatchTime, const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());
		//--------------------------------------------------------------------------------------
		/*!
		@brief	イベントのPOST（キューに入れる）
		@param[in]	DispatchTime	POSTする時間（0で次のターン）
		@param[in]	Sender	イベント送信者（nullptr可）
		@param[in]	ReceiverKey	受け手側オブジェクトを判別するキー
		@param[in]	MsgStr	メッセージ
		@param[in,out]	Info	追加情報
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void PostEvent(float DispatchTime, const shared_ptr<ObjectInterface>& Sender, const wstring& ReceiverKey,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());

		//--------------------------------------------------------------------------------------
		/*!
		@brief	イベントのSEND（キューに入れずにそのまま送る）
		@param[in]	Sender	イベント送信者（nullptr可）
		@param[in]	ReceiverKey	受け手側オブジェクトを判別するキー
		@param[in]	MsgStr	メッセージ
		@param[in,out]	Info	追加情報
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SendEvent(const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());
		//--------------------------------------------------------------------------------------
		/*!
		@brief	イベントのSEND（キューに入れずにそのまま送る）
		@param[in]	Sender	イベント送信者（nullptr可）
		@param[in]	Receiver	イベント受信者（nullptr不可）
		@param[in]	MsgStr	メッセージ
		@param[in,out]	Info	追加情報
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SendEvent(const shared_ptr<ObjectInterface>& Sender, const wstring& ReceiverKey,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());
		//--------------------------------------------------------------------------------------
		/*!
		@brief	イベントを受け取る
		@param[in]	event	イベント
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnEvent(const shared_ptr<Event>& event) {}
	private:
		//コピー禁止
		ObjectInterface(const ObjectInterface&) = delete;
		ObjectInterface& operator=(const ObjectInterface&) = delete;
		//ムーブ禁止
		ObjectInterface(const ObjectInterface&&) = delete;
		ObjectInterface& operator=(const ObjectInterface&&) = delete;
	};

	class Stage;
	//--------------------------------------------------------------------------------------
	///	Objectインターフェイスの派生クラスを構築する
	//--------------------------------------------------------------------------------------
	class ObjectFactory {
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief オブジェクト作成（static関数）
		@tparam T	作成する型
		@tparam Ts...	可変長パラメータ型
		@param[in]	params	可変長パラメータ
		@return　作成したオブジェクトのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		static shared_ptr<T> Create(Ts&&... params) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T(params...));
			//仮想関数呼び出し
			Ptr->OnPreCreate();
			Ptr->OnCreate();
			Ptr->SetCreated(true);
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief オブジェクト作成（static関数）<br/>
		コンストラクタではなく、OnCreateWithParam()関数にパラメータを渡す場合
		@tparam T	作成する型
		@tparam Ts...	可変長パラメータ型
		@param[in]	params	可変長パラメータ
		@return　作成したオブジェクトのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		static shared_ptr<T> CreateWithParam(Ts&&... params) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T());
			//仮想関数呼び出し
			Ptr->OnPreCreate();
			Ptr->OnCreateWithParam(params...);
			Ptr->OnCreate();
			Ptr->SetCreated(true);
			return Ptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief ゲームオブジェクト作成（static関数）<br/>
		コンストラクタではなく、OnCreateWithParam()関数にパラメータを渡す場合
		@tparam T	作成する型
		@tparam Ts...	可変長パラメータ型
		@param[in]	StagePtr	ステージのshared_ptr
		@param[in]	params	可変長パラメータ
		@return　作成したオブジェクトのshared_ptr
		*/
		//--------------------------------------------------------------------------------------
		template<typename T, typename... Ts>
		static shared_ptr<T> CreateGameObjectWithParam(const shared_ptr<Stage>& StagePtr, Ts&&... params) {
			shared_ptr<T> Ptr = shared_ptr<T>(new T(StagePtr));
			//仮想関数呼び出し
			Ptr->OnPreCreate();
			Ptr->OnCreateWithParam(params...);
			Ptr->OnCreate();
			Ptr->SetCreated(true);
			return Ptr;
		}
	};


	//--------------------------------------------------------------------------------------
	///	UpdateとDrawのインターフェイス
	//--------------------------------------------------------------------------------------
	class ShapeInterface {
	public:
		//構築と破棄
		//--------------------------------------------------------------------------------------
		/*!
		@brief コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		ShapeInterface() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShapeInterface() {}
		//操作
		//--------------------------------------------------------------------------------------
		/*!
		@brief OnUpdate処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnUpdate() = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief OnLastUpdate処理.<br />
		＊最終アップデート
		＊純粋仮想関数にしない
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnLastUpdate() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	OnDraw処理
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnDraw() = 0;
	private:
		//コピー禁止
		ShapeInterface(const ShapeInterface&) = delete;
		ShapeInterface& operator=(const ShapeInterface&) = delete;
		//ムーブ禁止
		ShapeInterface(const ShapeInterface&&) = delete;
		ShapeInterface& operator=(const ShapeInterface&&) = delete;
	};

	//--------------------------------------------------------------------------------------
	///	シーンインターフェイス
	//--------------------------------------------------------------------------------------
	class SceneInterface :public ObjectInterface, public ShapeInterface {
	protected:
		SceneInterface() {}
		virtual ~SceneInterface() {}
	};





	//--------------------------------------------------------------------------------------
	///	メモリ上に保持するリソースの親クラス
	//--------------------------------------------------------------------------------------
	class BaseResource : public ObjectInterface {
	protected:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトコンスラトクタ
		*/
		//--------------------------------------------------------------------------------------
		BaseResource() {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	プロテクトデストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BaseResource() {}
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief 前初期化を行う（仮想関数）<br />
		＊thisポインタが必要なオブジェクトはこの関数を多重定義して、取得できる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnPreCreate() override {}
		//--------------------------------------------------------------------------------------
		/*!
		@brief 初期化を行う（仮想関数）<br />
		＊thisポインタが必要なオブジェクトはこの関数を多重定義して、取得できる
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void OnCreate() override {}
	};



	template <typename T>
	class StateMachine;

	//--------------------------------------------------------------------------------------
	///	ステート実装テンプレートクラス(抽象クラス)
	/*!
	@tparam T	オーナーの型
	*/
	//--------------------------------------------------------------------------------------
	template <typename T>
	class ObjState{
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		*/
		//--------------------------------------------------------------------------------------
		ObjState(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ObjState(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートに入ったときに実行される
		@param[in]	Obj	ステートを保持するオブジェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Enter(const shared_ptr<T>& Obj) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	Updateのときに実行される
		@param[in]	Obj	ステートを保持するオブジェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Execute(const shared_ptr<T>& Obj) = 0;
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートを出るときに実行される
		@param[in]	Obj	ステートを保持するオブジェクト
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Exit(const shared_ptr<T>& Obj) = 0;
	};

	//--------------------------------------------------------------------------------------
	///	ステートマシン実装テンプレートクラス(抽象クラス)
	/*!
	@tparam T	オーナーの型
	*/
	//--------------------------------------------------------------------------------------
	template <typename T>
	class StateMachine
	{
	private:
		//このステートマシンを持つオーナー
		weak_ptr<T> m_Owner;
		//現在のステート
		weak_ptr< ObjState<T> > m_CurrentState;
		//一つ前のステート
		weak_ptr< ObjState<T> > m_PreviousState;
	public:
		//--------------------------------------------------------------------------------------
		/*!
		@brief	コンストラクタ
		@param[in]	owner	オーナーのポインタ
		*/
		//--------------------------------------------------------------------------------------
		explicit StateMachine(const shared_ptr<T>& owner) :
			m_Owner(owner)
			//m_CurrentState(nullptr), m_PreviousState(nullptr)
		{}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	デストラクタ
		*/
		//--------------------------------------------------------------------------------------
		virtual ~StateMachine(){}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カレントステートを設定する
		@param[in]	s	設定するステート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetCurrentState(const shared_ptr< ObjState<T> >& s){ m_CurrentState = s; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1つ前のステートを設定する
		@param[in]	s	設定するステート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetPreviousState(const shared_ptr< ObjState<T> >& s){ m_PreviousState = s; }
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カレントステートを得る
		@return	カレントステート
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr< ObjState<T> >  GetCurrentState() const {
			auto shptr = m_CurrentState.lock();
			if (shptr){
				return shptr;
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	1つ前のステートを得る
		@return	1つ前のステート
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr< ObjState<T> >  GetPreviousState()const {
			auto shptr = m_pPreviousState.lock();
			if (shptr){
				return shptr;
			}
			return nullptr;
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートを更新する
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void Update() const{
			auto shptr = m_CurrentState.lock();
			auto ow_shptr = m_Owner.lock();
			if (shptr && ow_shptr){
				shptr->Execute(ow_shptr);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートを変更する
		@param[in]	NewState	設定するステート
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void  ChangeState(const shared_ptr< ObjState<T> >& NewState){
			//元のステートを保存
			m_PreviousState = m_CurrentState;
			auto shptr = m_CurrentState.lock();
			auto ow_shptr = m_Owner.lock();
			if (shptr && ow_shptr){
				//元のステートに終了を知らせる
				shptr->Exit(ow_shptr);
			}
			//新しいステートをカレントに設定
			m_CurrentState = NewState;
			shptr = m_CurrentState.lock();
			if (shptr && ow_shptr){
				//元のステートに終了を知らせる
				shptr->Enter(ow_shptr);
			}
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	ステートを一つ前のステートに戻す
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void RevertToPreviousState(){
			ChangeState(m_pPreviousState);
		}
		//--------------------------------------------------------------------------------------
		/*!
		@brief	カレントステートが指定したステートになってるかどうかをチェック
		@param[in]	st	調べるステート
		@return	指定したステートになってればtrue
		*/
		//--------------------------------------------------------------------------------------
		bool IsInState(const shared_ptr< ObjState<T> >& st)const{
			auto shptr = m_CurrentState.lock();
			if (!shptr){
				return false;
			}
			return typeid(shptr) == typeid(st);
		}
	};


}

//end basecross
