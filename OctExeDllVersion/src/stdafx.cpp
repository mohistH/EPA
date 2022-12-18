#include "pch.h"
#include "stdafx.h"

int StringOperator::getIndexInStringKeys(TCHAR* key)
{
	size_t length = _tcslen(key);
	for (int i = 0; i < 12; ++i)
	{
		if (_tcsnicmp(StringKeys[i], key, length) == 0) return i;
	}
	return -1;
}

DWORD StringOperator::getWStringValueLength(const CString& value)
{
	return value.GetLength();
}

void StringOperator::setLength(BYTE* resource, WORD length)
{
	*reinterpret_cast<WORD*>(resource) = length;
}

WORD StringOperator::GetLength(const BYTE* resource_)
{
	return *reinterpret_cast<const WORD*>(resource_);
}
