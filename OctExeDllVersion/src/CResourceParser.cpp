#include "pch.h"
#include "CResourceParser.h"

WORD CResourceParser::Word()
{
	return *reinterpret_cast<const WORD*>(this->Move(sizeof(WORD)));
}

WCHAR CResourceParser::WChar()
{
	return *reinterpret_cast<const WORD*>(this->Move(sizeof(WCHAR)));
}

void CResourceParser::Alignment()
{
	if (this->ParsedSize % 4 == 0) return;
	DWORD alignments = 4 - (this->ParsedSize % 4);
	this->ParsedSize += alignments;
}

const BYTE* CResourceParser::Move(long size_)
{
	const BYTE* v = (this->Data + this->ParsedSize);
	this->ParsedSize += size_;
	return v;
}
