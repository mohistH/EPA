#include "pch.h"
#include "CResourcePacker.h"
void CResourcePacker::Reset()
{
	if (this->Resource)
	{
		delete[]this->Resource;
		this->Resource = NULL;
		this->Size = 0;
	}
}
void CResourcePacker::AppendWord(WORD value_)
{
	this->Append(&value_, sizeof(WORD));
}

void CResourcePacker::AppendWords(const WORD* value_, DWORD count_)
{
	this->Append(value_, sizeof(WORD)*count_);
}

void CResourcePacker::AppendWChar(WCHAR value_)
{
	this->Append(&value_, sizeof(WCHAR));
}

void CResourcePacker::AppendWString(const WCHAR* value_)
{
	this->Append(value_, sizeof(WCHAR)*(wcslen(value_) + 1));
}

void CResourcePacker::AppendAlignment()
{
	if (this->Size % 4 == 0) return;
	static BYTE aligmentsValues[3] = { 0,0,0 };
	DWORD aligments = 4 - (this->Size % 4);
	this->Append(aligmentsValues, aligments);
}

void CResourcePacker::Append(const void* value_, DWORD size_)
{
	DWORD newSize = this->Size + size_;
	BYTE* newResource = new BYTE[newSize];
	ZeroMemory(newResource, size_);
	if (this->Resource) memcpy(newResource, this->Resource, this->Size);
	memcpy(newResource + this->Size, value_, size_);
	delete[]this->Resource;
	this->Resource = newResource;
	this->Size = newSize;
}
