#include "pch.h"
#include "CStringTable.h"
#include "CResourceParser.h"
#include <assert.h>

CStringTable::CStringTable()
	: Comments(NULL)
	, CompanyName(NULL)
	, FileDescription(NULL)
	, FileVersion(NULL)
	, InternalName(NULL)
	, LegalCopyright(NULL)
	, LegalTrademarks(NULL)
	, OriginalFilename(NULL)
	, PrivateBuild(NULL)
	, ProductName(NULL)
	, ProductVersion(NULL)
	, SpecialBuild(NULL)
{
	Language = 0;
	CodePage = 0;
}
CStringTable::~CStringTable() { Reset(); }

void CStringTable::Set(CString*& which_, LPCTSTR value_)
{
	if (which_) delete which_;
	which_ = value_ ? (new CString(value_)) : NULL;
}

void CStringTable::Reset()
{
	if (this->Comments) { delete this->Comments; this->Comments = NULL; }
	if (this->CompanyName) { delete this->CompanyName; this->CompanyName = NULL; }
	if (this->FileDescription) { delete this->FileDescription; this->FileDescription = NULL; }
	if (this->FileVersion) { delete this->FileVersion; this->FileVersion = NULL; }
	if (this->InternalName) { delete this->InternalName; this->InternalName = NULL; }
	if (this->LegalCopyright) { delete this->LegalCopyright; this->LegalCopyright = NULL; }
	if (this->LegalTrademarks) { delete this->LegalTrademarks; this->LegalTrademarks = NULL; }
	if (this->OriginalFilename) { delete this->OriginalFilename; this->OriginalFilename = NULL; }
	if (this->PrivateBuild) { delete this->PrivateBuild; this->PrivateBuild = NULL; }
	if (this->ProductName) { delete this->ProductName; this->ProductName = NULL; }
	if (this->ProductVersion) { delete this->ProductVersion; this->ProductVersion = NULL; }
	if (this->SpecialBuild) { delete this->SpecialBuild; this->SpecialBuild = NULL; }
}

void CStringTable::Pack(CResourcePacker& packer_) const
{
	packer_.Reset();
	packer_.AppendWord(0);
	packer_.AppendWord(0);
	packer_.AppendWord(WTYPE_TEXT);
	CString translate;
	translate.Format(_T("%04x%04x"), this->Language, this->CodePage);
	packer_.AppendWString(translate);
	packer_.AppendAlignment();
	PCString values[] =
	{
		this->Comments,
		this->CompanyName,
		this->FileDescription,
		this->FileVersion,
		this->InternalName,
		this->LegalCopyright,
		this->LegalTrademarks,
		this->OriginalFilename,
		this->PrivateBuild,
		this->ProductName,
		this->ProductVersion,
		this->SpecialBuild
	};
	for (int i = 0; i < 12; ++i)
	{
		if (!values[i]) continue;
		CResourcePacker childPacker;
		CStringTable_PackChild(childPacker, StringKeys[i], *values[i]);
		packer_.Append(childPacker.GetResource(), childPacker.GetSize());
		packer_.AppendAlignment();
	}
	packer_.AppendAlignment();
	StringOperator::setLength(packer_.GetResource(), packer_.GetSize());
}
void CStringTable::Parse(const BYTE* data_)
{
	Reset();

	CResourceParser parser(data_);
	parser.Word();
	parser.Word();
	enumWType wType = (enumWType)parser.Word();
	assert(wType == WTYPE_TEXT);
	int iLanguage, iCodePage;
	swscanf((WCHAR*)parser.Move(8 * sizeof(WCHAR)), L"%04x%04x", &iLanguage, &iCodePage);
	this->Language = iLanguage;
	this->CodePage = iCodePage;
	parser.Alignment();

	PPCString values[] =
	{
		&this->Comments,
		&this->CompanyName,
		&this->FileDescription,
		&this->FileVersion,
		&this->InternalName,
		&this->LegalCopyright,
		&this->LegalTrademarks,
		&this->OriginalFilename,
		&this->PrivateBuild,
		&this->ProductName,
		&this->ProductVersion,
		&this->SpecialBuild
	};

	long structLength = StringOperator::GetLength(data_);
	long parsedLength = parser.GetParsedSize();
	while (parsedLength < structLength)
	{
		CStringTable_ParseChild(data_ + parsedLength, values);
		parsedLength += StringOperator::GetLength(data_ + parsedLength);
		if (parsedLength % 4 != 0) parsedLength += 4 - (parsedLength % 4);
	}
}

void CStringTable::CStringTable_PackChild(CResourcePacker& packer_, const CString& which_, const CString& value_)
{
	packer_.Reset();
	packer_.AppendWord(0);
	packer_.AppendWord(StringOperator::getWStringValueLength(value_));
	packer_.AppendWord(WTYPE_TEXT);
	packer_.AppendWString((LPCTSTR)which_);
	packer_.AppendAlignment();
	packer_.AppendWString(value_);
	packer_.AppendAlignment();
	StringOperator::setLength(packer_.GetResource(), packer_.GetSize());
}

void CStringTable::CStringTable_ParseChild(const BYTE* data_, PPCString* values)
{
	CResourceParser parser(data_);
	parser.Word();
	WORD valueLength = parser.Word();
	enumWType wType = (enumWType)parser.Word();
	assert(wType == WTYPE_TEXT);
	WCHAR* key = (WCHAR*)parser.Move(0);
	parser.Move(sizeof(WCHAR)*(wcslen(key) + 1));
	parser.Alignment();

	*values[StringOperator::getIndexInStringKeys(key)] = new CString((WCHAR*)parser.Move(0));

}
