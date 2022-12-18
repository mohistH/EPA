#include "pch.h"
#include "CStringFileInfo.h"
#include "CResourceParser.h"
#include <assert.h>

CStringFileInfo::~CStringFileInfo() { Reset(); }

void CStringFileInfo::Reset()
{
	for (auto child : Children)
	{
		delete child;
	}
	Children.clear();
}
void CStringFileInfo::Pack(CResourcePacker& packer_) const
{
	packer_.Reset();
	packer_.AppendWord(0);
	packer_.AppendWord(0);
	packer_.AppendWord(WTYPE_TEXT);
	packer_.AppendWString(SZKEY_STRINGFILEINFO);
	packer_.AppendAlignment();

	for (auto st : Children)
	{
		if (!st) continue;
		CResourcePacker childPacker;
		st->Pack(childPacker);
		packer_.Append(childPacker.GetResource(), childPacker.GetSize());
		packer_.AppendAlignment();
	}
	packer_.AppendAlignment();
	StringOperator::setLength(packer_.GetResource(), packer_.GetSize());
}
void CStringFileInfo::Parse(const BYTE* data_)
{
	Reset();

	CResourceParser parser(data_);
	parser.Word();
	parser.Word();
	enumWType wType = (enumWType)parser.Word();
	assert(wType == WTYPE_TEXT);
	parser.Move(sizeof(WCHAR)*(wcslen(SZKEY_STRINGFILEINFO) + 1));
	parser.Alignment();

	long structLength = StringOperator::GetLength(data_);
	long parsedLength = parser.GetParsedSize();
	while (parsedLength < structLength)
	{
		CStringTable* st = new CStringTable;
		st->Parse(data_ + parsedLength);
		this->Children.push_back(st);
		parsedLength += StringOperator::GetLength(data_ + parsedLength);
		if (parsedLength % 4 != 0) parsedLength += 4 - (parsedLength % 4);
	}
}
