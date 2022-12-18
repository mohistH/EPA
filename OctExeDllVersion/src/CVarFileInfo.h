#pragma once
#include "CVar.h"
#include <vector>

class CVarFileInfo
{
public:
	~CVarFileInfo();

public:
	//CArray<CVar*, CVar*> Children;
	std::vector<CVar*> Children;
public:
	void Reset();
	void Pack(CResourcePacker& packer_) const;
	void Parse(const BYTE* data_);

};

