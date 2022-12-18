#pragma once
#include "CStringTable.h"
#include <vector>

class CStringFileInfo
{
public:
	~CStringFileInfo();

public:
	//CArray<CStringTable*, CStringTable*> Children;
	std::vector<CStringTable*> Children;
public:
	void Reset();
	void Pack(CResourcePacker& packer_) const;
	void Parse(const BYTE* data_);

};

