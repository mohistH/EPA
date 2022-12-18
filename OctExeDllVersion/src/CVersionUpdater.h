#pragma once

class CVersionInfo;
#include <string>

class CVersionUpdater
{
public:
	CVersionUpdater();
	~CVersionUpdater();

public:
	bool Open(const std::string& exeOrDll, std::string& out_error);
	bool Update(WORD language_ = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL)) const;

private:
	void unInit();

public:
	CVersionInfo* VersionInfo;

private:
	std::string ExeOrDll;
};

