#pragma once
#include "structLangageAndCodePage.h"
#include "CResourcePacker.h"
#include <vector>
class CVar
{
public:
	//CArray<structLangageAndCodePage, structLangageAndCodePage&> Value;
	std::vector<structLangageAndCodePage> Value;
public:
	void Pack(CResourcePacker& packer_) const;
	void Parse(const BYTE* data_);
};

