#include "pch.h"
#include "CVersionInfo.h"
#include "CVersionUpdater.h"
#include "CResourcePacker.h"

#pragma comment(lib, "version.lib")
#pragma comment(lib, "shlwapi")


CVersionUpdater::CVersionUpdater() : VersionInfo(NULL) {}
CVersionUpdater::~CVersionUpdater()
{
	unInit();
}

bool CVersionUpdater::Open(const std::string& exeOrDll, std::string& out_error)
{
	DWORD handle = 0;
	DWORD blockBytesCount = GetFileVersionInfoSizeA(exeOrDll.c_str(), &handle);
	if (blockBytesCount == 0)
	{
		char arr[10] = { 0 };
		sprintf(arr, "%d", GetLastError());
		out_error = std::string("Failed to read file version information, get_lasterror_id=") + std::string(arr, 10);
		return false;
	}

	BYTE* blockBytes = new BYTE[blockBytesCount];
	if (!GetFileVersionInfoA(exeOrDll.c_str(), NULL, blockBytesCount, blockBytes))
	{
		char arr[10] = { 0 };
		sprintf(arr, "%d", GetLastError());
		out_error = std::string("Failed to read file version information, get_lasterror_id=") + std::string(arr, 10);

		delete[]blockBytes;
		return false;
	}

	CVersionInfo* versionInfo = new CVersionInfo;
	versionInfo->Parse(blockBytes);
	delete[]blockBytes;

	if (this->VersionInfo)
	{
		delete this->VersionInfo;
	}
	this->VersionInfo = versionInfo;
	this->ExeOrDll = exeOrDll;

	return true;
}

bool CVersionUpdater::Update(WORD language_) const
{
	if (!this->VersionInfo) return false;

	HANDLE resource = BeginUpdateResourceA(this->ExeOrDll.c_str(), FALSE);
	if (!resource) return false;

	CResourcePacker packer;
	this->VersionInfo->Pack(packer);

	if (!UpdateResource(resource, RT_VERSION, MAKEINTRESOURCE(VS_VERSION_INFO), language_, packer.GetResource(), packer.GetSize())) return false;
	return EndUpdateResource(resource, FALSE);
}

void CVersionUpdater::unInit()
{
	try
	{
		if (VersionInfo)
		{
			delete VersionInfo;
			VersionInfo = nullptr;
		}
	}
	catch (...)
	{
		;
	}
}
