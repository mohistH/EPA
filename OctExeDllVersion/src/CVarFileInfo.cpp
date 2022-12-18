#include "pch.h"
#include "CVarFileInfo.h"
#include "CResourcePacker.h"
#include "stdafx.h"
#include "CVar.h"
#include "CResourceParser.h"
#include <assert.h>
CVarFileInfo::~CVarFileInfo() { Reset(); }

void CVarFileInfo::Reset()
{
	for (auto &child : Children)
	{
		delete child;
	}
	Children.clear();
}
void CVarFileInfo::Pack(CResourcePacker& packer_) const
{
	packer_.Reset();
	packer_.AppendWord(0);
	packer_.AppendWord(0);
	packer_.AppendWord(WTYPE_TEXT);
	packer_.AppendWString(SZKEY_VARFILEINFO);
	packer_.AppendAlignment();

	for (CVar* v : Children)
	{
		if (!v) continue;
		CResourcePacker childPacker;
		v->Pack(childPacker);
		packer_.Append(childPacker.GetResource(), childPacker.GetSize());
		packer_.AppendAlignment();
	}
	packer_.AppendAlignment();
	StringOperator::setLength(packer_.GetResource(), packer_.GetSize());
}
void CVarFileInfo::Parse(const BYTE* data_)
{
	Reset();

	CResourceParser parser(data_);
	parser.Word();
	parser.Word();
	enumWType wType = (enumWType)parser.Word();
	assert(wType == WTYPE_TEXT);
	parser.Move(sizeof(WCHAR)*(wcslen(SZKEY_VARFILEINFO) + 1));
	parser.Alignment();

	long structLength = StringOperator::GetLength(data_);
	long parsedLength = parser.GetParsedSize();
	while (parsedLength < structLength)
	{
		CVar* v = new CVar;
		v->Parse(data_ + parsedLength);
		this->Children.push_back(v);
		parsedLength += StringOperator::GetLength(data_ + parsedLength);
		if (parsedLength % 4 != 0) parsedLength += 4 - (parsedLength % 4);
	}
}
