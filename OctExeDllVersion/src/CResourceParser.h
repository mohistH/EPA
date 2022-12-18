#pragma once
#include "Windows.h"
class CResourceParser
{
public:
	CResourceParser(const BYTE* data_) : Data(data_), ParsedSize(0) {}

public:
	WORD Word();
	WCHAR WChar();
	void Alignment();
	const BYTE* Move(long size_);
	long GetParsedSize() const { return this->ParsedSize; }

private:
	const BYTE* Data;
	long ParsedSize;

};

