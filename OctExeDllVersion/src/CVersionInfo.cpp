#include "pch.h"
#include "CVersionInfo.h"
#include "stdafx.h"
#include "CResourcePacker.h"
#include "CResourceParser.h"
#include "CStringFileInfo.h"
#include "CVarFileInfo.h"
#include <assert.h>

CVersionInfo::CVersionInfo() :StringFileInfo(NULL), VarFileInfo(NULL)
{
	memset(&this->FixedFileInfo, 0, sizeof(VS_FIXEDFILEINFO));
}
CVersionInfo::~CVersionInfo() { Reset(); }

void CVersionInfo::Reset()
{
	if (this->StringFileInfo) { delete this->StringFileInfo; this->StringFileInfo = NULL; }
	if (this->VarFileInfo) { delete this->VarFileInfo; this->VarFileInfo = NULL; }
}
void CVersionInfo::Pack(CResourcePacker& packer_) const
{
	packer_.Reset();
	packer_.AppendWord(0);
	packer_.AppendWord(sizeof(VS_FIXEDFILEINFO));
	packer_.AppendWord(WTYPE_BINARY);
	packer_.AppendWString(SZKEY_VS_VERSION_INFO);
	packer_.AppendAlignment();
	packer_.Append(&this->FixedFileInfo, sizeof(VS_FIXEDFILEINFO));
	packer_.AppendAlignment();

	if (this->StringFileInfo)
	{
		CResourcePacker childPacker;
		this->StringFileInfo->Pack(childPacker);
		packer_.Append(childPacker.GetResource(), childPacker.GetSize());
		packer_.AppendAlignment();
	}
	if (this->VarFileInfo)
	{
		CResourcePacker childPacker;
		this->VarFileInfo->Pack(childPacker);
		packer_.Append(childPacker.GetResource(), childPacker.GetSize());
		packer_.AppendAlignment();
	}

	packer_.AppendAlignment();
	StringOperator::setLength(packer_.GetResource(), packer_.GetSize());
}
void CVersionInfo::Parse(const BYTE* data_)
{
	Reset();

	CResourceParser parser(data_);
	parser.Word();
	parser.Word();
	enumWType wType = (enumWType)parser.Word();
	assert(wType == WTYPE_BINARY);
	parser.Move(sizeof(WCHAR)*(wcslen(SZKEY_VS_VERSION_INFO) + 1));
	parser.Alignment();
	this->FixedFileInfo = *(VS_FIXEDFILEINFO*)parser.Move(sizeof(VS_FIXEDFILEINFO));
	parser.Alignment();

	const BYTE* next = parser.Move(sizeof(WORD) * 3);
	LPCTSTR v = (LPCTSTR)parser.Move(0);

	if (_wcsnicmp(v, SZKEY_STRINGFILEINFO, wcslen(SZKEY_STRINGFILEINFO)) == 0)
	{
		this->StringFileInfo = new CStringFileInfo;
		this->StringFileInfo->Parse(next);
		parser.Move(StringOperator::GetLength(next) - sizeof(WORD) * 3);
		parser.Alignment();
		next = parser.Move(sizeof(WORD) * 3);
		v = (LPCTSTR)parser.Move(0);
	}
	if (_wcsnicmp(v, SZKEY_VARFILEINFO, wcslen(SZKEY_VARFILEINFO)) == 0)
	{
		this->VarFileInfo = new CVarFileInfo;
		this->VarFileInfo->Parse(next);
	}
}
