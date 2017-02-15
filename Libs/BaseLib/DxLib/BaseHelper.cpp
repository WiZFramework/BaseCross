/*!
@file BaseHelper.cpp
@brief 例外処理、ヘルパー関数等実体
@copyright Copyright (c) 2017 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"


namespace basecross{

	BinaryReader::BinaryReader(const wstring& fileName)
	{
		size_t dataSize;

		HRESULT hr = ReadEntireFile(fileName, mOwnedData, &dataSize);
		if (FAILED(hr))
		{
			// 初期化失敗
			throw BaseException(
				L"ファイルの読み込みに失敗しました。",
				L"if (FAILED(hr))",
				L"BinaryReader::BinaryReader()"
				);
		}

		mPos = mOwnedData.get();
		mEnd = mOwnedData.get() + dataSize;
	}


	BinaryReader::BinaryReader(_In_reads_bytes_(dataSize) uint8_t const* dataBlob, size_t dataSize)
	{
		mPos = dataBlob;
		mEnd = dataBlob + dataSize;
	}


	HRESULT BinaryReader::ReadEntireFile(const wstring& fileName, _Inout_ std::unique_ptr<uint8_t[]>& data, _Out_ size_t* dataSize)
	{
		//ファイルのオープン
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
		ScopedHandle hFile(safe_handle(CreateFile2(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, nullptr)));
#else
		ScopedHandle hFile(safe_handle(CreateFileW(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr)));
#endif

		if (!hFile)
			return HRESULT_FROM_WIN32(GetLastError());

		// サイズの取得
		LARGE_INTEGER fileSize = { 0 };

#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
		FILE_STANDARD_INFO fileInfo;

		if (!GetFileInformationByHandleEx(hFile.get(), FileStandardInfo, &fileInfo, sizeof(fileInfo)))
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}

		fileSize = fileInfo.EndOfFile;
#else
		GetFileSizeEx(hFile.get(), &fileSize);
#endif

		// ファイルが制限を超えたらエラー
		if (fileSize.HighPart > 0)
			return E_FAIL;

		// 同じサイズのメモリを作成
		data.reset(new uint8_t[fileSize.LowPart]);

		if (!data)
			return E_OUTOFMEMORY;

		// データの読み込み
		DWORD bytesRead = 0;

		if (!ReadFile(hFile.get(), data.get(), fileSize.LowPart, &bytesRead, nullptr))
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}

		if (bytesRead < fileSize.LowPart)
			return E_FAIL;

		*dataSize = bytesRead;

		return S_OK;
	}


	//--------------------------------------------------------------------------------------
	//	struct CsvFile::Impl;
	//	用途: コンポーネントImplクラス
	//--------------------------------------------------------------------------------------
	struct CsvFile::Impl{
		wstring m_FileName;					//ファイル名
		vector< wstring > m_CsvVec;	//文字列を保持する配列
		Impl()
		{}
		~Impl(){
			m_CsvVec.clear();
		}
	};


	//--------------------------------------------------------------------------------------
	//	class CsvFile;
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//	CsvFile();
	//--------------------------------------------------------------------------------------
	CsvFile::CsvFile():
		pImpl(new Impl)
	{
	}
	//--------------------------------------------------------------------------------------
	//	explicit CsvFile(
	//	const wstring& FileName	//CSVファイル名
	//	);
	//--------------------------------------------------------------------------------------
	CsvFile::CsvFile(const wstring& FileName):
		pImpl(new Impl)
	{
		try{
			if (FileName == L""){
				throw BaseException(
					L"Csvファイル名が不定です。",
					L"FileName == L\"\"",
					L"CsvFile::CsvFile()"
					);
			}
			pImpl->m_FileName = FileName;
		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	virtual CsvFile::~CsvFile();
	//--------------------------------------------------------------------------------------
	CsvFile::~CsvFile(){}
	//--------------------------------------------------------------------------------------
	//	const wstring& CsvFile::GetFileName() const;
	//--------------------------------------------------------------------------------------
	const wstring& CsvFile::GetFileName() const{
		return pImpl->m_FileName;
	}
	//--------------------------------------------------------------------------------------
	//	void CsvFile::SetFileName(
	//	const wstring& FileName	//CSVファイル名
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::SetFileName(const wstring& FileName){
		try{
			if (FileName == L""){
				throw BaseException(
					L"Csvファイル名が不定です。",
					L"FileName == L\"\"",
					L"CsvFile::SetFileName()"
					);
			}
			pImpl->m_FileName = FileName;
			pImpl->m_CsvVec.clear();
		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	vector< wstring >& CsvFile::GetCsvVec()const;
	//--------------------------------------------------------------------------------------
	vector< wstring >& CsvFile::GetCsvVec()const{
		return pImpl->m_CsvVec;
	}

	//--------------------------------------------------------------------------------------
	//	bool CsvFile::ReadCsv();
	//--------------------------------------------------------------------------------------
	bool CsvFile::ReadCsv(){
		try{
			if (pImpl->m_FileName == L""){
				throw BaseException(
					L"Csvファイル名が不定です。",
					L"(m_FileName == L\"\")",
					L"CsvFile::ReadCsv()"
					);
			}
			//CSVファイルの存在を確認
			DWORD RetCode;
			RetCode = GetFileAttributes(pImpl->m_FileName.c_str());
			if (RetCode == 0xFFFFFFFF){
				//ファイルがない。
				//読み込み処理終了
				return false;
			}
			wifstream ifs(pImpl->m_FileName.c_str());
			if (!ifs){
				//オープンに失敗した。
				//ファイルが存在するが失敗したことが考えられる
				throw BaseException(
					L"Csvファイル名のオープンに失敗しました。",
					pImpl->m_FileName.c_str(),
					L"CsvFile::ReadCsv()"
					);
			}
			locale loc("japanese");
			ifs.imbue(loc);
			//データをクリアする
			pImpl->m_CsvVec.clear();
			wstring line;
			//１行ずつlineに読み込む
			while (getline(ifs, line)){
				//行をデータに追加する
				pImpl->m_CsvVec.push_back(line);
			}
			ifs.close();
			return true;
		}
		catch (...){
			throw;
		}
	}

	bool CsvFile::ReadCsv(const wstring& str){
		//データをクリアする
		pImpl->m_CsvVec.clear();
		vector<wstring> lineVec;
		Util::WStrToTokenVector(lineVec, str, L'\n');
		for (auto& v : lineVec){
			auto Len = v.size();
			wstring setstr;
			if (Len > 0){
				if (v[Len - 1] == L'\r'){
					setstr = v.substr(0, Len - 1);

				}
				else{
					setstr = v;
				}
				pImpl->m_CsvVec.push_back(setstr);
			}
		}
		return true;
	}


	//--------------------------------------------------------------------------------------
	//	void CsvFile::SaveCsv(
	//	bool Exp = true	//例外処理するかどうか
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::SaveCsv(bool Exp){
		try{
			if (pImpl->m_FileName == L""){
				if (Exp){
					//例外処理する場合
					throw BaseException(
						L"Csvファイル名が不定です。",
						L"m_FileName == L\"\"",
						L"CsvFile::SaveCsv()"
						);
				}
				else{
					//例外処理しない
					return;
				}
			}
			wofstream ofs(pImpl->m_FileName.c_str());
			//例外処理する場合
			if (!ofs){
				if (Exp){
					throw BaseException(
						L"Csvファイル名のオープンに失敗しました。",
						pImpl->m_FileName.c_str(),
						L"CsvFile::SaveCsv()"
						);
				}
				else{
					//例外処理しない
					return;
				}
			}
			locale loc("japanese");
			ofs.imbue(loc);
			size_t lsz = pImpl->m_CsvVec.size();
			for (size_t i = 0; i < lsz; i++){
				ofs << pImpl->m_CsvVec[i] << endl;
			}
			ofs.close();
		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	size_t AddRow(
	//	wstring& Row	//Csvの1行（文字列は、「,」で区切られている必要がある）
	//	);
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//	size_t CsvFile::AddRow(
	//	vector<wstring>& Row	//Csvの1行（各セルは配列化されている必要がある）
	//	);
	//--------------------------------------------------------------------------------------
	size_t CsvFile::AddRow(vector<wstring>& Row){
		wstring temp = L"";
		bool flg = false;
		for (auto l : Row){
			if (flg){
				temp += L',';
			}
			temp += l;
			flg = true;
		}
		size_t ret = pImpl->m_CsvVec.size();
		pImpl->m_CsvVec.push_back(temp);
		return ret;
	}
	//--------------------------------------------------------------------------------------
	//	size_t CsvFile::AddRow(
	//	wstring& Row	//Csvの1行（文字列は、「,」で区切られている必要がある）
	//	);
	//--------------------------------------------------------------------------------------
	size_t CsvFile::AddRow(wstring& Row){
		size_t ret = pImpl->m_CsvVec.size();
		//データに追加する
		pImpl->m_CsvVec.push_back(Row);
		return ret;
	}
	//--------------------------------------------------------------------------------------
	//	void CsvFile::UpdateRow(
	//	size_t RowNum,			//変更したい行
	//	vector<wstring>& Row.	//Csvの1行（各セルは配列化されている必要がある）
	//	bool Exp = true			//例外処理するかどうか
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::UpdateRow(size_t RowNum, vector<wstring>& Row, bool Exp){
		try{
			if (pImpl->m_CsvVec.size() <= RowNum){
				if (Exp){
					//例外処理する場合
					throw BaseException(
						L"指定の行が範囲外です。",
						MakeRangeErr(L"m_CsvVec.size()", pImpl->m_CsvVec.size(), L"RowNum", RowNum).c_str(),
						L"CsvFile::UpdateRow()"
						);
				}
				else{
					//例外処理しない
					return;
				}
			}
			wstring temp = L"";
			bool flg = false;
			for (auto l : Row){
				if (flg){
					temp += L',';
				}
				temp += l;
				flg = true;
			}
			pImpl->m_CsvVec[RowNum] = temp;
		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	void CsvFile::UpdateRow(
	//	size_t RowNum,	//変更したい行
	//	wstring& Row,	//Csvの1行（文字列は、「,」で区切られている必要がある）
	//	bool Exp = true	//例外処理するかどうか
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::UpdateRow(size_t RowNum, wstring& Row, bool Exp){
		try{
			if (pImpl->m_CsvVec.size() <= RowNum){
				if (Exp){
					//例外処理する場合
					throw BaseException(
						L"指定の行が範囲外です。",
						MakeRangeErr(L"m_CsvVec.size()", pImpl->m_CsvVec.size(), L"RowNum", RowNum).c_str(),
						L"CsvFile::UpdateRow()"
						);
				}
				else{
					//例外処理しない
					return;
				}
			}
			pImpl->m_CsvVec[RowNum] = Row;
		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	void CsvFile::UpdateCell(
	//	size_t RowNum,	//変更行
	//	size_t ColNum,	//変更列
	//	wstring& Cell,	//差し替える文字列
	//	bool Exp = true			//例外処理するかどうか
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::UpdateCell(size_t RowNum, size_t ColNum, wstring& Cell, bool Exp){
		try{
			if (pImpl->m_CsvVec.size() <= RowNum){
				if (Exp){
					//例外処理する場合
					throw BaseException(
						L"指定の行が範囲外です。",
						MakeRangeErr(L"m_CsvVec.size()", pImpl->m_CsvVec.size(), L"RowNum", RowNum).c_str(),
						L"CsvFile::UpdateCell()"
						);
				}
				else{
					//例外処理しない
					return;
				}
			}
			vector<wstring> LineVec;
			Util::WStrToTokenVector(LineVec, pImpl->m_CsvVec[RowNum], L',');
			if (LineVec.size() <= ColNum){
				if (Exp){
					//例外処理する場合
					throw BaseException(
						L"指定の列が範囲外です。",
						MakeRangeErr(L"m_CsvVec.size()", pImpl->m_CsvVec.size(), L"ColNum", ColNum).c_str(),
						L"CsvFile::UpdateCell()"
						);
				}
				else{
					//例外処理しない
					return;
				}
			}
			//指定の位置のセルを差し替え
			LineVec[ColNum] = Cell;
			wstring temp = L"";
			bool flg = false;
			for (auto l : LineVec){
				if (flg){
					temp += L',';
				}
				temp += l;
				flg = true;
			}
			pImpl->m_CsvVec[RowNum] = temp;
		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	size_t CsvFile::GetRowCount() const;
	//--------------------------------------------------------------------------------------
	size_t CsvFile::GetRowCount() const{
		return pImpl->m_CsvVec.size();
	}
	//--------------------------------------------------------------------------------------
	//	void CsvFile::GetRowVec(
	//	size_t RowNum,			//受け取りたい行
	//	vector<wstring>& Row	//1行を受け取る文字列の配列
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::GetRowVec(size_t RowNum, vector<wstring>& Row){
		try{
			if (pImpl->m_CsvVec.size() <= RowNum){
				throw BaseException(
					L"指定の行が範囲外です。",
					MakeRangeErr(L"m_CsvVec.size()", pImpl->m_CsvVec.size(), L"RowNum", RowNum).c_str(),
					L"CsvFile::GetLineVec()"
					);
			}
			//渡された配列に値が入ってる場合があるのでクリア
			Row.clear();
			//指定行を、デリミタ「,」で区切って返す
			Util::WStrToTokenVector(Row, pImpl->m_CsvVec[RowNum], L',');
		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	wstring CsvFile::GetCell(
	//	size_t RowNum,	//受け取りたい行
	//	size_t ColNum	//受け取りたい列
	//	);
	//--------------------------------------------------------------------------------------
	wstring CsvFile::GetCell(size_t RowNum, size_t ColNum){
		try{
			if (pImpl->m_CsvVec.size() <= RowNum){
				throw BaseException(
					L"指定の行が範囲外です。",
					MakeRangeErr(L"m_CsvVec.size()", pImpl->m_CsvVec.size(), L"RowNum", RowNum).c_str(),
					L"CsvFile::GetCell()"
					);
			}
			vector<wstring> LineVec;
			Util::WStrToTokenVector(LineVec, pImpl->m_CsvVec[RowNum], L',');
			if (LineVec.size() <= ColNum){
				throw BaseException(
					L"指定の列が範囲外です。",
					MakeRangeErr(L"m_CsvVec.size()", pImpl->m_CsvVec.size(), L"ColNum", ColNum).c_str(),
					L"CsvFile::GetCell()"
					);
			}
			return LineVec[ColNum];
		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	void GetSelect(
	//	vector< wstring >& RetVec	//結果を受け取る文字列の配列
	//	size_t ColNum,			//条件を検査する列
	//	const wstring& Key,		//条件キー
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::GetSelect(vector< wstring >& RetVec,size_t ColNum, const wstring& Key){
		//渡された配列に値が入ってる場合があるのでクリア
		RetVec.clear();
		size_t sz = pImpl->m_CsvVec.size();
		for (size_t i = 0; i < sz; i++){
			vector<wstring> LineVec;
			Util::WStrToTokenVector(LineVec, pImpl->m_CsvVec[i], L',');
			if (LineVec.size() > ColNum){
				if (LineVec[ColNum] == Key){
					RetVec.push_back(pImpl->m_CsvVec[i]);
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------
	//	void CsvFile::GetSelect(
	//	vector< wstring >& RetVec,		//結果を受け取る文字列の配列
	//	bool (Func)(const wstring&),	//条件を検査するコールバック関数
	//	);
	//--------------------------------------------------------------------------------------
	void CsvFile::GetSelect(vector< wstring >& RetVec,bool(Func)(const wstring&)){
		//渡された配列に値が入ってる場合があるのでクリア
		RetVec.clear();
		size_t sz = pImpl->m_CsvVec.size();
		for (size_t i = 0; i < sz; i++){
			//コールバック関数を呼び出す
			if (Func(pImpl->m_CsvVec[i])){
				//条件があれば追加
				RetVec.push_back(pImpl->m_CsvVec[i]);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	///	CreateとPreCreateを持ち、Thisスマートポインタがとれるインターフェイス
	//--------------------------------------------------------------------------------------
	void ObjectInterface::PostEvent(float DispatchTime, const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info) {
		App::GetApp()->GetEventDispatcher()->PostEvent(DispatchTime, Sender, Receiver, MsgStr, Info);
	}
	void ObjectInterface::PostEvent(float DispatchTime, const shared_ptr<ObjectInterface>& Sender, const wstring& ReceiverKey,
		const wstring& MsgStr, shared_ptr<void>& Info) {
		App::GetApp()->GetEventDispatcher()->PostEvent(DispatchTime, Sender, ReceiverKey, MsgStr, Info);

	}

	void ObjectInterface::SendEvent(const shared_ptr<ObjectInterface>& Sender, const shared_ptr<ObjectInterface>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info) {
		App::GetApp()->GetEventDispatcher()->SendEvent(Sender, Receiver, MsgStr, Info);
	}

	void ObjectInterface::SendEvent(const shared_ptr<ObjectInterface>& Sender, const wstring& ReceiverKey,
		const wstring& MsgStr, shared_ptr<void>& Info) {
		App::GetApp()->GetEventDispatcher()->SendEvent(Sender, ReceiverKey, MsgStr, Info);
	}





}
//endof  basecross
