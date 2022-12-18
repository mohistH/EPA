#pragma once
#include "CVarFileInfo.h"
#include "CResourcePacker.h"
#include "CStringFileInfo.h"
class CVersionInfo
{
public:
	CVersionInfo();
	~CVersionInfo();

public:
	VS_FIXEDFILEINFO FixedFileInfo;
	CStringFileInfo* StringFileInfo;
	CVarFileInfo* VarFileInfo;

public:
	void Reset();
	void Pack(CResourcePacker& packer_) const;
	void Parse(const BYTE* data_);

};

