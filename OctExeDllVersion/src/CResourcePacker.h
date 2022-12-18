#pragma once
#include <Windows.h>
class CResourcePacker
{
public:
	CResourcePacker() : Resource(NULL), Size(0) {}
	~CResourcePacker() { Reset(); }

public:
	void Reset();

	void AppendWord(WORD value_);
	void AppendWords(const WORD* value_, DWORD count_);
	void AppendWChar(WCHAR value_);
	void AppendWString(const WCHAR* value_);
	void AppendAlignment();
	void Append(const void* value_, DWORD size_);

	inline BYTE* GetResource() const { return this->Resource; }
	inline DWORD GetSize() const { return this->Size; }

private:
	BYTE* Resource;
	DWORD Size;

};

