#pragma once
#include <atlstr.h>
#include "Windows.h"
#include "tChar.h"
///
static TCHAR* SZKEY_VS_VERSION_INFO = _T("VS_VERSION_INFO");
static TCHAR* SZKEY_STRINGFILEINFO = _T("StringFileInfo");
static TCHAR* SZKEY_VARFILEINFO = _T("VarFileInfo");
static TCHAR* SZKEY_TRANSLATION = _T("Translation");

enum enumWType
{
	WTYPE_TEXT = 1,
	WTYPE_BINARY = 0,
};
static TCHAR* StringKeys[] =
{
	_T("Comments"),
	_T("CompanyName"),
	_T("FileDescription"),
	_T("FileVersion"),
	_T("InternalName"),
	_T("LegalCopyright"),
	_T("LegalTrademarks"),
	_T("OriginalFilename"),
	_T("PrivateBuild"),
	_T("ProductName"),
	_T("ProductVersion"),
	_T("SpecialBuild")
};

class StringOperator {
public:
	static int getIndexInStringKeys(TCHAR* key);

	static DWORD getWStringValueLength(const CString& value);

	static void setLength(BYTE* resource, WORD length);

	static WORD GetLength(const BYTE* resource_);

};


typedef CString* PCString;
typedef CString** PPCString;


//#ifdef UNICODE
//#define QStringToTCHAR(str) ((wchar_t*) str.utf16())
//#define TCHARToQString(str) (QString::fromUtf16((str)))
//#else
//#define QStringToTCHAR(str) (str.toLocal8Bit().constData())
//#define TCHARToQString(str) (QString::fromLocal8Bit((str)))
//#endif