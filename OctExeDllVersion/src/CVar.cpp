#include "pch.h"
#include "CVar.h"
#include "stdafx.h"
#include "CResourceParser.h"
void CVar::Pack(CResourcePacker& packer_) const
{
	packer_.Reset();
	packer_.AppendWord(0);
	packer_.AppendWord(sizeof(structLangageAndCodePage)* (this->Value.size()));
	packer_.AppendWord(WTYPE_BINARY);
	packer_.AppendWString(SZKEY_TRANSLATION);
	packer_.AppendAlignment();

	for (const auto& v : Value)
	{
		packer_.Append(&v, sizeof(structLangageAndCodePage));
		packer_.AppendAlignment();
	}
	packer_.AppendAlignment();
	StringOperator::setLength(packer_.GetResource(), packer_.GetSize());
}
void CVar::Parse(const BYTE* data_)
{
	CResourceParser parser(data_);
	parser.Word();
	WORD valueLength = parser.Word();
	enumWType wType = (enumWType)parser.Word();
	parser.Move(sizeof(WCHAR)*(wcslen(SZKEY_TRANSLATION) + 1));
	parser.Alignment();

	long structLength = StringOperator::GetLength(data_);
	long parsedLength = parser.GetParsedSize();
	while (parsedLength < structLength)
	{
		this->Value.push_back(*(structLangageAndCodePage*)(data_ + parsedLength));
		parsedLength += sizeof(structLangageAndCodePage);
		if (parsedLength % 4 != 0) parsedLength += 4 - (parsedLength % 4);
	}
}
